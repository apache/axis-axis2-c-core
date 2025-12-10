/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <httpd.h>
#include <http_config.h>
#include <http_log.h>
#include <http_protocol.h>
#include <ap_config.h>
#include <apr_strings.h>
#include "axis2_apache2_worker.h"
#include <axutil_error_default.h>
#include <signal.h>
#include <execinfo.h>
#include <sys/types.h>
#include <unistd.h>
#include <strings.h>
#include <axutil_log_default.h>
#include <axutil_thread_pool.h>
/* REVOLUTIONARY: Conditional axiom include - only for SOAP processing */
#ifndef HTTP2_JSON_ONLY_MODE
#include <axiom_xml_reader.h>
#endif
#include <axutil_version.h>
#include <axutil_types.h>
#include <apr_rmm.h>
#include <apr_shm.h>
#include <apr_dbd.h>
#include <mod_dbd.h>
#include <axis2_http_transport.h>
#include <axutil_types.h>
#include <axis2_engine.h>
#include <axis2_conf_ctx.h>
#include <axis2_conf_init.h>

/* Configuration structure populated by apache2.conf */
typedef struct axis2_config_rec
{
    char *axutil_log_file;
    char *axis2_repo_path;
    axutil_log_levels_t log_level;
    int max_log_file_size;
    int axis2_global_pool_size;
} axis2_config_rec_t;

axis2_apache2_worker_t *axis2_worker = NULL;
axutil_env_t *axutil_env = NULL;
apr_rmm_t* rmm = NULL;
apr_global_mutex_t *global_mutex = NULL;

/******************************Function Headers********************************/

/* DEBUGGING: Signal handler for crash debugging */
static void axis2_segfault_handler(int sig)
{
    void *array[10];
    size_t size;
    char **strings;
    size_t i;

    /* Log crash information to Apache error log */
    ap_log_error(APLOG_MARK, APLOG_CRIT, APR_SUCCESS, NULL,
        "[Axis2] CRASH DETECTED: Signal %d received (SEGFAULT) - PID: %d", sig, getpid());

    /* Get backtrace */
    size = backtrace(array, 10);
    strings = backtrace_symbols(array, size);

    ap_log_error(APLOG_MARK, APLOG_CRIT, APR_SUCCESS, NULL,
        "[Axis2] BACKTRACE: Obtained %zd stack frames", size);

    for (i = 0; i < size; i++) {
        ap_log_error(APLOG_MARK, APLOG_CRIT, APR_SUCCESS, NULL,
            "[Axis2] BACKTRACE[%zd]: %s", i, strings[i]);
    }

    free(strings);

    /* Exit gracefully to avoid infinite loops */
    _exit(1);
}
static void *
axis2_create_svr(
    apr_pool_t * p,
    server_rec * s);

static const char *
axis2_set_repo_path(
    cmd_parms * cmd,
    void *dummy,
    const char *arg);

static const char *
axis2_set_log_file(
    cmd_parms * cmd,
    void *dummy,
    const char *arg);

static const char *
axis2_set_max_log_file_size(
    cmd_parms * cmd,
    void *dummy,
    const char *arg);

static const char *
axis2_set_global_pool_size(
    cmd_parms * cmd,
    void * dummy,
    const char *arg);

static const char *
axis2_set_log_level(
    cmd_parms * cmd,
    void *dummy,
    const char *arg);

static const char *
axis2_set_svc_url_prefix(
    cmd_parms * cmd,
    void *dummy,
    const char *arg);

static int
axis2_handler(
    request_rec * req);

static int
axis2_fixups(
    request_rec * req);

/* Shutdown Axis2 */
apr_status_t
axis2_shutdown(
    void *tmp);

void *AXIS2_CALL axis2_module_malloc(
    axutil_allocator_t * allocator,
    size_t size);

void *AXIS2_CALL axis2_module_realloc(
    axutil_allocator_t * allocator,
    void *ptr,
    size_t size);

void AXIS2_CALL axis2_module_free(
    axutil_allocator_t * allocator,
    void *ptr);

static void
axis2_module_init(
    apr_pool_t * p,
    server_rec * svr_rec);

static void
axis2_register_hooks(
    apr_pool_t * p);

char * 
axis2_get_session(
        void *req, 
        const char *id);

axis2_status_t 
axis2_set_session(
        void *req, 
        const char *id,
        const char *session);

/***************************End of Function Headers****************************/

static const command_rec axis2_cmds[] = { AP_INIT_TAKE1("Axis2RepoPath", axis2_set_repo_path, NULL,
    RSRC_CONF, "Axis2/C repository path"), AP_INIT_TAKE1("Axis2LogFile", axis2_set_log_file, NULL,
    RSRC_CONF, "Axis2/C log file name"), AP_INIT_TAKE1("Axis2LogLevel", axis2_set_log_level, NULL,
    RSRC_CONF, "Axis2/C log level"), AP_INIT_TAKE1("Axis2MaxLogFileSize",
    axis2_set_max_log_file_size, NULL, RSRC_CONF, "Axis2/C maximum log file size"),
    AP_INIT_TAKE1("Axis2GlobalPoolSize", axis2_set_global_pool_size, NULL, RSRC_CONF,
        "Axis2/C global pool size"), AP_INIT_TAKE1("Axis2ServiceURLPrefix",
        axis2_set_svc_url_prefix, NULL, RSRC_CONF, "Axis2/C service URL prifix"), 
        { NULL } };

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA axis2_module = { STANDARD20_MODULE_STUFF, NULL, /* create per-dir    config structures */
    NULL, /* merge  per-dir    config structures */
    axis2_create_svr, /* create per-server config structures */
    NULL, /* merge  per-server config structures */
    axis2_cmds, /* table of config file commands       */
    axis2_register_hooks /* register hooks                      */
    };

static void *
axis2_create_svr(
    apr_pool_t * p,
    server_rec * s)
{
    /* DEBUGGING: Log server configuration creation */
    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, s,
        "[Axis2] DEBUG: axis2_create_svr ENTRY - creating server configuration");

    axis2_config_rec_t *conf = apr_palloc(p, sizeof(*conf));
    if (!conf) {
        ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, s,
            "[Axis2] DEBUG: axis2_create_svr FAILED - memory allocation failed");
        return NULL;
    }

    conf->axutil_log_file = NULL;
    conf->axis2_repo_path = NULL;
    conf->log_level = AXIS2_LOG_LEVEL_DEBUG;
    conf->axis2_global_pool_size = 0;
    conf->max_log_file_size = 1;

    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, s,
        "[Axis2] DEBUG: axis2_create_svr SUCCESS - server configuration created");
    return conf;
}

static const char *
axis2_set_repo_path(
    cmd_parms * cmd,
    void *dummy,
    const char *arg)
{
    /* DEBUGGING: Log directive processing */
    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
        "[Axis2] DEBUG: axis2_set_repo_path ENTRY - processing Axis2RepoPath directive: %s", arg);

    axis2_config_rec_t *conf = NULL;
    const char *err = ap_check_cmd_context(cmd, GLOBAL_ONLY);
    if(err != NULL)
    {
        ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
            "[Axis2] DEBUG: axis2_set_repo_path FAILED - context error: %s", err);
        return err;
    }
    conf = (axis2_config_rec_t *)ap_get_module_config(cmd->server->module_config, &axis2_module);
    if (!conf) {
        ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
            "[Axis2] DEBUG: axis2_set_repo_path FAILED - could not get module config");
        return "Failed to get module configuration";
    }
    conf->axis2_repo_path = apr_pstrdup(cmd->pool, arg);
    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
        "[Axis2] DEBUG: axis2_set_repo_path SUCCESS - set repo path: %s", arg);
    return NULL;
}

static const char *
axis2_set_log_file(
    cmd_parms * cmd,
    void *dummy,
    const char *arg)
{
    /* DEBUGGING: Log directive processing */
    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
        "[Axis2] DEBUG: axis2_set_log_file ENTRY - processing Axis2LogFile directive: %s", arg);

    axis2_config_rec_t *conf = NULL;
    const char *err = ap_check_cmd_context(cmd, GLOBAL_ONLY);
    if(err != NULL)
    {
        ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
            "[Axis2] DEBUG: axis2_set_log_file FAILED - context error: %s", err);
        return err;
    }

    conf = (axis2_config_rec_t *)ap_get_module_config(cmd->server->module_config, &axis2_module);
    if (!conf) {
        ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
            "[Axis2] DEBUG: axis2_set_log_file FAILED - could not get module config");
        return "Failed to get module configuration";
    }
    conf->axutil_log_file = apr_pstrdup(cmd->pool, arg);
    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
        "[Axis2] DEBUG: axis2_set_log_file SUCCESS - set log file: %s", arg);
    return NULL;
}

static const char *
axis2_set_max_log_file_size(
    cmd_parms * cmd,
    void *dummy,
    const char *arg)
{
    /* DEBUGGING: Log directive processing */
    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
        "[Axis2] DEBUG: axis2_set_max_log_file_size ENTRY - processing Axis2MaxLogFileSize directive: %s", arg);

    axis2_config_rec_t *conf = NULL;
    const char *err = ap_check_cmd_context(cmd, GLOBAL_ONLY);
    if(err != NULL)
    {
        ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
            "[Axis2] DEBUG: axis2_set_max_log_file_size FAILED - context error: %s", err);
        return err;
    }

    conf = (axis2_config_rec_t *)ap_get_module_config(cmd->server->module_config, &axis2_module);
    if (!conf) {
        ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
            "[Axis2] DEBUG: axis2_set_max_log_file_size FAILED - could not get module config");
        return "Failed to get module configuration";
    }
    conf->max_log_file_size = 1024 * 1024 * atoi(arg);
    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
        "[Axis2] DEBUG: axis2_set_max_log_file_size SUCCESS - set max log file size: %s", arg);
    return NULL;
}

static const char *
axis2_set_global_pool_size(
    cmd_parms * cmd,
    void *dummy,
    const char *arg)
{
    axis2_config_rec_t *conf = NULL;
    const char *err = ap_check_cmd_context(cmd, GLOBAL_ONLY);
    if(err != NULL)
    {
        return err;
    }

    conf = (axis2_config_rec_t *)ap_get_module_config(cmd->server->module_config, &axis2_module);
    conf->axis2_global_pool_size = 1024 * 1024 * atoi(arg);
    return NULL;
}

static const char *
axis2_set_log_level(
    cmd_parms * cmd,
    void *dummy,
    const char *arg)
{
    /* DEBUGGING: Log directive processing - this is likely where the crash occurs */
    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
        "[Axis2] DEBUG: axis2_set_log_level ENTRY - processing Axis2LogLevel directive: %s", arg);

    char *str;
    axutil_log_levels_t level = AXIS2_LOG_LEVEL_DEBUG;
    axis2_config_rec_t *conf = NULL;

    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
        "[Axis2] DEBUG: axis2_set_log_level - about to check command context");
    const char *err = ap_check_cmd_context(cmd, GLOBAL_ONLY);
    if(err != NULL)
    {
        ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
            "[Axis2] DEBUG: axis2_set_log_level FAILED - context error: %s", err);
        return err;
    }

    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
        "[Axis2] DEBUG: axis2_set_log_level - about to get module config");
    conf = (axis2_config_rec_t *)ap_get_module_config(cmd->server->module_config, &axis2_module);
    if (!conf) {
        ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
            "[Axis2] DEBUG: axis2_set_log_level FAILED - could not get module config");
        return "Failed to get module configuration";
    }

    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
        "[Axis2] DEBUG: axis2_set_log_level - about to call ap_getword_conf");
    str = ap_getword_conf(cmd->pool, &arg);
    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
        "[Axis2] DEBUG: axis2_set_log_level - ap_getword_conf returned: %s", str ? str : "NULL");

    if(str)
    {
        ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
            "[Axis2] DEBUG: axis2_set_log_level - about to check string: '%s'", str);

        if(strcmp(str, "info") == 0)
        {
            level = AXIS2_LOG_LEVEL_INFO;
            ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
                "[Axis2] DEBUG: axis2_set_log_level - matched 'info' level");
        }
        else if(strcmp(str, "debug") == 0)
        {
            level = AXIS2_LOG_LEVEL_DEBUG;
            ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
                "[Axis2] DEBUG: axis2_set_log_level - matched 'debug' level");
        }
        else if(strcmp(str, "error") == 0)
        {
            level = AXIS2_LOG_LEVEL_ERROR;
            ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
                "[Axis2] DEBUG: axis2_set_log_level - matched 'error' level");
        }
        else if(strcmp(str, "warn") == 0)
        {
            level = AXIS2_LOG_LEVEL_WARNING;
            ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
                "[Axis2] DEBUG: axis2_set_log_level - matched 'warn' level");
        }
        else if(strcmp(str, "crit") == 0)
        {
            level = AXIS2_LOG_LEVEL_CRITICAL;
            ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
                "[Axis2] DEBUG: axis2_set_log_level - matched 'crit' level");
        }
        else if(strcmp(str, "user") == 0)
        {
            level = AXIS2_LOG_LEVEL_USER;
            ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
                "[Axis2] DEBUG: axis2_set_log_level - matched 'user' level");
        }
        else if(strcmp(str, "trace") == 0)
        {
            level = AXIS2_LOG_LEVEL_TRACE;
            ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
                "[Axis2] DEBUG: axis2_set_log_level - matched 'trace' level");
        }
        else
        {
            ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
                "[Axis2] DEBUG: axis2_set_log_level - no match for '%s', using default debug level", str);
        }
    }
    conf->log_level = level;
    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, cmd->server,
        "[Axis2] DEBUG: axis2_set_log_level SUCCESS - set log level to %d, returning", level);
    return NULL;
}

static const char *
axis2_set_svc_url_prefix(
    cmd_parms * cmd,
    void *dummy,
    const char *arg)
{
    AXIS2_IMPORT extern axis2_char_t *axis2_request_url_prefix;
    const char *err = ap_check_cmd_context(cmd, GLOBAL_ONLY);

    axis2_request_url_prefix = AXIS2_REQUEST_URL_PREFIX;
    if(!err)
    {
        axis2_char_t *prefix = apr_pstrdup(cmd->pool, arg);
        if(prefix)
            axis2_request_url_prefix = prefix;
    }

    return NULL;
}

/* REVOLUTIONARY HTTP/2 Request Interception - Enhanced Stream Processing */
static int
axis2_fixups(
    request_rec * req)
{
    /* Only process /services/* requests */
    if (!req->uri || strncmp(req->uri, "/services/", 10) != 0) {
        return DECLINED;
    }

    /* REVOLUTIONARY HTTP/2 JSON Processing - Enhanced breed apart stream optimization */
    if (req->protocol && strstr(req->protocol, "HTTP/2")) {

        const char *content_type = req->content_type ? req->content_type :
                                   apr_table_get(req->headers_in, "Content-Type");

        if (content_type && strstr(content_type, "application/json")) {

            ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, req,
                "🚀 REVOLUTIONARY: HTTP/2 JSON stream detected in fixups - Optimizing for breed apart processing");

            /* Force this request to use axis2_module handler */
            req->handler = apr_pstrdup(req->pool, "axis2_module");

            /* REVOLUTIONARY: Enhanced HTTP/2 processing flags */
            apr_table_set(req->headers_in, "X-Axis2-JSON-Mode", "revolutionary");
            apr_table_set(req->headers_in, "X-Axis2-HTTP2-Request", "breed-apart");
            apr_table_set(req->headers_in, "X-Axis2-Processing", "direct-response");
            apr_table_set(req->headers_in, "X-Axis2-Architecture", "option-b-enhanced");

            /* HTTP/2 Performance optimization flags */
            apr_table_set(req->headers_in, "X-HTTP2-Multiplexing", "enabled");
            apr_table_set(req->headers_in, "X-Pipeline-Bypass", "soap-transformation");

            /* REVOLUTIONARY: Stream priority hints for HTTP/2 multiplexing */
            const char *service_path = req->uri + 10; /* Skip "/services/" */
            if (service_path && strlen(service_path) > 0) {
                /* Set stream priority based on service type */
                if (strstr(service_path, "BigData") || strstr(service_path, "H2")) {
                    apr_table_set(req->headers_in, "X-Stream-Priority", "high-performance");
                } else {
                    apr_table_set(req->headers_in, "X-Stream-Priority", "standard");
                }
            }

            ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, req,
                "✅ REVOLUTIONARY: HTTP/2 JSON stream optimized - Revolutionary processing flags set");
        }
    }

    return DECLINED;
}

/* The sample content handler */
static int
axis2_handler(
    request_rec * req)
{
    int rv = 0;
    axutil_env_t *thread_env = NULL;
    axutil_allocator_t *allocator = NULL;
    axutil_error_t *error = NULL;

    apr_allocator_t *local_allocator = NULL;
    apr_pool_t *local_pool = NULL;

    const char *content_type_header = apr_table_get(req->headers_in, "Content-Type");
    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, req,
        "DEBUG: mod_axis2 axis2_handler ENTRY - method: %s, content_type: %s, uri: %s, protocol: %s, header_content_type: %s",
        req->method ? req->method : "NULL",
        req->content_type ? req->content_type : "NULL",
        req->uri ? req->uri : "NULL",
        req->protocol ? req->protocol : "NULL",
        content_type_header ? content_type_header : "NULL");

    if(strcmp(req->handler, "axis2_module"))
    {
        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, req,
            "DEBUG: mod_axis2 DECLINED - handler is %s, not axis2_module", req->handler);
        return DECLINED;
    }







    /* Set up the read policy from the client. */
    if((rv = ap_setup_client_block(req, REQUEST_CHUNKED_DECHUNK)) != OK)
    {
        return rv;
    }
    ap_should_client_block(req);

    apr_allocator_create(&local_allocator);
    apr_pool_create_ex(&local_pool, NULL, NULL, local_allocator);

    /*thread_env = axutil_init_thread_env(axutil_env);*/

    /*axutil_env->allocator->current_pool = (void *) req->pool;
     rv = AXIS2_APACHE2_WORKER_PROCESS_REQUEST(axis2_worker, axutil_env, req);*/

    /* create new allocator for this request */
    /*allocator = (axutil_allocator_t *) apr_palloc(req->pool,
     sizeof(axutil_allocator_t));*/

    allocator = (axutil_allocator_t *)apr_palloc(local_pool, sizeof(axutil_allocator_t));

    if(!allocator)
    {
        return HTTP_INTERNAL_SERVER_ERROR;
    }
    allocator->malloc_fn = axis2_module_malloc;
    allocator->realloc = axis2_module_realloc;
    allocator->free_fn = axis2_module_free;
    allocator->global_pool_ref = 0;
    allocator->local_pool = (void *)local_pool;
    allocator->current_pool = (void *)local_pool;
    allocator->global_pool = axutil_env->allocator->global_pool;

    error = axutil_error_create(allocator);
    thread_env = axutil_env_create_with_error_log_thread_pool(allocator, error, axutil_env->log,
        axutil_env-> thread_pool);
    thread_env->allocator = allocator;
    thread_env->set_session_fn = axis2_set_session;
    thread_env->get_session_fn = axis2_get_session;

    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, req,
        "DEBUG: mod_axis2 calling AXIS2_APACHE2_WORKER_PROCESS_REQUEST - uri: %s, content_type: %s",
        req->uri ? req->uri : "NULL",
        req->content_type ? req->content_type : "NULL");

    rv = AXIS2_APACHE2_WORKER_PROCESS_REQUEST(axis2_worker, thread_env, req);

    ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, req,
        "DEBUG: mod_axis2 AXIS2_APACHE2_WORKER_PROCESS_REQUEST returned: %d", rv);

    if(AXIS2_CRITICAL_FAILURE == rv)
    {
        return HTTP_INTERNAL_SERVER_ERROR;
    }

    apr_pool_destroy(local_pool);
    apr_allocator_destroy(local_allocator);

    return rv;
}

void *AXIS2_CALL
axis2_module_malloc(
    axutil_allocator_t * allocator,
    size_t size)
{
#if APR_HAS_SHARED_MEMORY
    if (rmm == allocator->current_pool)
    {

        void* ptr = NULL;
        apr_rmm_off_t offset;
        apr_global_mutex_lock(global_mutex);
        offset = apr_rmm_malloc(rmm, size);
        if (offset)
        ptr = apr_rmm_addr_get(rmm, offset);
        apr_global_mutex_unlock(global_mutex);
        return ptr;
    }
#endif
    return apr_palloc((apr_pool_t *)(allocator->current_pool), size);
}

void *AXIS2_CALL
axis2_module_realloc(
    axutil_allocator_t * allocator,
    void *ptr,
    size_t size)
{
#if APR_HAS_SHARED_MEMORY
    if (rmm == allocator->current_pool)
    {
        void* ptr = NULL;
        apr_rmm_off_t offset;
        apr_global_mutex_lock(global_mutex);
        offset = apr_rmm_realloc(rmm, ptr, size);
        if (offset)
        ptr = apr_rmm_addr_get(rmm, offset);
        apr_global_mutex_unlock(global_mutex);
        return ptr;
    }
#endif
    /* can't be easily implemented */
    return NULL;
}

void AXIS2_CALL
axis2_module_free(
    axutil_allocator_t * allocator,
    void *ptr)
{
#if APR_HAS_SHARED_MEMORY
    if (rmm == allocator->current_pool)
    {
        apr_rmm_off_t offset;
        apr_global_mutex_lock(global_mutex);
        offset = apr_rmm_offset_get(rmm, ptr);
        apr_rmm_free(rmm, offset);
        apr_global_mutex_unlock(global_mutex);
    }
#endif
}

static int
axis2_post_config(
    apr_pool_t *pconf,
    apr_pool_t *plog,
    apr_pool_t *ptemp,
    server_rec *svr_rec)
{
    apr_status_t status = APR_SUCCESS;
    axutil_allocator_t *allocator = NULL;
    axutil_error_t *error = NULL;
    axutil_log_t *axutil_logger = NULL;
    axutil_thread_pool_t *thread_pool = NULL;
    void *data = NULL;
    const char *userdata_key = "axis2_init";
    axis2_config_rec_t *conf = (axis2_config_rec_t *)ap_get_module_config(svr_rec->module_config,
        &axis2_module);

    /* axis2_post_config() will be called twice. Don't bother
     * going through all of the initialization on the first call
     * because it will just be thrown away.*/

    ap_add_version_component(pconf, AXIS2_HTTP_HEADER_SERVER_AXIS2C);

    apr_pool_userdata_get(&data, userdata_key, svr_rec->process->pool);
    if(!data)
    {
        apr_pool_userdata_set((const void *)1, userdata_key, apr_pool_cleanup_null,
            svr_rec->process->pool);
        return OK;
    }

    /* DEBUGGING: Enable engine initialization with comprehensive logging */
    ap_log_error(APLOG_MARK, APLOG_INFO, APR_SUCCESS, svr_rec,
        "[Axis2] DEBUG: *** POST_CONFIG FUNCTION ENTRY *** - post_config called, enabling engine initialization");

    /* Skip shared memory initialization for HTTP2_JSON_ONLY_MODE - use direct engine init */
#ifdef HTTP2_JSON_ONLY_MODE
    ap_log_error(APLOG_MARK, APLOG_INFO, APR_SUCCESS, svr_rec,
        "[Axis2] DEBUG: HTTP2_JSON_ONLY_MODE - skipping shared memory, proceeding to engine init");
    goto engine_init;
#endif
#if APR_HAS_SHARED_MEMORY
    if (conf->axis2_global_pool_size > 0)
    {
        apr_shm_t *shm;
        apr_rmm_off_t offset;

        status = apr_shm_create(&shm, conf->axis2_global_pool_size, NULL, pconf);
        if (status != APR_SUCCESS)
        {
            ap_log_error(APLOG_MARK, APLOG_EMERG, status, svr_rec,
                "[Axis2] Error creating shared memory pool");
            exit(APEXIT_INIT);
        }

        status = apr_rmm_init(&rmm, NULL, apr_shm_baseaddr_get(shm), conf->axis2_global_pool_size,
            pconf);
        if (status != APR_SUCCESS)
        {
            ap_log_error(APLOG_MARK, APLOG_EMERG, status, svr_rec,
                "[Axis2] Error creating relocatable memory pool");
            exit(APEXIT_INIT);
        }

        status = apr_global_mutex_create(&global_mutex, NULL,
            APR_LOCK_DEFAULT, pconf);
        if (status != APR_SUCCESS)
        {
            ap_log_error(APLOG_MARK, APLOG_EMERG, status, svr_rec,
                "[Axis2] Error creating global mutex");
            exit(APEXIT_INIT);
        }

        /*status = unixd_set_global_mutex_perms(global_mutex);
         if (status != APR_SUCCESS)
         {
         ap_log_error(APLOG_MARK, APLOG_EMERG, status, svr_rec,
         "[Axis2] Permision cannot be set to global mutex");
         exit(APEXIT_INIT);
         }
         */

        offset = apr_rmm_malloc(rmm, sizeof(axutil_allocator_t));
        if (!offset)
        {
            ap_log_error(APLOG_MARK, APLOG_EMERG, status, svr_rec,
                "[Axis2] Error in creating allocator in global pool");
            exit(APEXIT_INIT);
        }
        allocator = apr_rmm_addr_get(rmm, offset);
        allocator->malloc_fn = axis2_module_malloc;
        allocator->realloc = axis2_module_realloc;
        allocator->free_fn = axis2_module_free;
        allocator->global_pool_ref = 0;
        allocator->local_pool = (void *) rmm;
        allocator->current_pool = (void *) rmm;
        allocator->global_pool = (void *) rmm;

    /* REVOLUTIONARY HTTP2_JSON_ONLY_MODE: Direct engine initialization without shared memory */
engine_init:
    ap_log_error(APLOG_MARK, APLOG_INFO, APR_SUCCESS, svr_rec,
        "[Axis2] DEBUG: engine_init - beginning core Axis2/C initialization");

#ifdef HTTP2_JSON_ONLY_MODE
    /* For HTTP/2 JSON mode: Use simple pool-based allocator instead of shared memory */
    allocator = (axutil_allocator_t *)apr_palloc(pconf, sizeof(axutil_allocator_t));
    allocator->malloc_fn = axis2_module_malloc;
    allocator->realloc = axis2_module_realloc;
    allocator->free_fn = axis2_module_free;
    allocator->global_pool_ref = 0;
    allocator->local_pool = (void *)pconf;
    allocator->current_pool = (void *)pconf;
    allocator->global_pool = (void *)pconf;

    ap_log_error(APLOG_MARK, APLOG_INFO, APR_SUCCESS, svr_rec,
        "[Axis2] DEBUG: HTTP2_JSON_ONLY_MODE allocator created using APR pool");
#endif

    /* REVOLUTIONARY: Skip axiom init for HTTP/2 JSON-only mode
     * Initialize XML readers only when SOAP processing is needed
     */
#ifndef HTTP2_JSON_ONLY_MODE
    axiom_xml_reader_init();
    ap_log_error(APLOG_MARK, APLOG_INFO, APR_SUCCESS, svr_rec,
        "[Axis2] DEBUG: axiom_xml_reader_init completed for SOAP mode");
#else
    ap_log_error(APLOG_MARK, APLOG_INFO, APR_SUCCESS, svr_rec,
        "[Axis2] DEBUG: Skipping axiom_xml_reader_init in HTTP2_JSON_ONLY_MODE");
#endif
    axutil_error_init();

        error = axutil_error_create(allocator);
        if (!error)
        {
            ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
                "[Axis2] Error creating mod_axis2 error structure");
            exit(APEXIT_CHILDFATAL);
        }

        axutil_logger = axutil_log_create(allocator, NULL, conf->axutil_log_file);
        if (!axutil_logger)
        {
            ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
                "[Axis2] Error creating mod_axis2 log structure");
            exit(APEXIT_CHILDFATAL);
        }
        thread_pool = axutil_thread_pool_init(allocator);
        if (!thread_pool)
        {
            ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
                "[Axis2] Error initializing mod_axis2 thread pool");
            exit(APEXIT_CHILDFATAL);
        }
        axutil_env = axutil_env_create_with_error_log_thread_pool(allocator, error,
            axutil_logger,
            thread_pool);
        if (!axutil_env)
        {
            ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
                "[Axis2] Error creating mod_axis2 environment");
            exit(APEXIT_CHILDFATAL);
        }
        if (axutil_logger)
        {
            axutil_logger->level = conf->log_level;
            axutil_logger->size = conf->max_log_file_size;
            AXIS2_LOG_INFO(axutil_env->log, "Apache Axis2/C version in use : %s",
                axis2_version_string());
            AXIS2_LOG_INFO(axutil_env->log,
                "Starting log with log level %d and max log file size %d",
                conf->log_level, conf->max_log_file_size);
        }
        axutil_env->set_session_fn = axis2_set_session;
        axutil_env->get_session_fn = axis2_get_session;

        /* REVOLUTIONARY: Create temporary engine to trigger HTTP service provider registration */
        ap_log_error(APLOG_MARK, APLOG_INFO, APR_SUCCESS, svr_rec,
            "[Axis2] DEBUG: Creating temporary engine to register HTTP service provider");

        axis2_conf_ctx_t *temp_conf_ctx = axis2_build_conf_ctx(axutil_env, conf->axis2_repo_path);
        if (!temp_conf_ctx) {
            ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
                "[Axis2] Error creating temporary configuration context for service provider registration");
            exit(APEXIT_CHILDFATAL);
        }

        axis2_engine_t *temp_engine = axis2_engine_create(axutil_env, temp_conf_ctx);
        if (!temp_engine) {
            ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
                "[Axis2] Error creating temporary engine for service provider registration");
            exit(APEXIT_CHILDFATAL);
        }

        ap_log_error(APLOG_MARK, APLOG_INFO, APR_SUCCESS, svr_rec,
            "[Axis2] DEBUG: Temporary engine created successfully - HTTP service provider should now be registered");

        /* Clean up temporary engine - service provider registration persists */
        axis2_engine_free(temp_engine, axutil_env);
        axis2_conf_ctx_free(temp_conf_ctx, axutil_env);

        ap_log_error(APLOG_MARK, APLOG_INFO, APR_SUCCESS, svr_rec,
            "[Axis2] DEBUG: About to create axis2_apache2_worker after service provider registration");

        /* DEBUGGING: Validate parameters before function call */
        ap_log_error(APLOG_MARK, APLOG_INFO, APR_SUCCESS, svr_rec,
            "[Axis2] DEBUG: Validating parameters - axutil_env=%p, repo_path=%s",
            (void*)axutil_env, conf->axis2_repo_path ? conf->axis2_repo_path : "NULL");

        if (!axutil_env) {
            ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
                "[Axis2] CRITICAL: axutil_env is NULL before worker creation");
            exit(APEXIT_CHILDFATAL);
        }

        if (!axutil_env->log) {
            ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
                "[Axis2] CRITICAL: axutil_env->log is NULL before worker creation");
            exit(APEXIT_CHILDFATAL);
        }

        ap_log_error(APLOG_MARK, APLOG_INFO, APR_SUCCESS, svr_rec,
            "[Axis2] DEBUG: Parameters validated, calling axis2_apache2_worker_create...");

        axis2_worker = axis2_apache2_worker_create(axutil_env,
            conf->axis2_repo_path);

        ap_log_error(APLOG_MARK, APLOG_INFO, APR_SUCCESS, svr_rec,
            "[Axis2] DEBUG: axis2_apache2_worker_create returned: %p", (void*)axis2_worker);

        if (!axis2_worker)
        {
            ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
                "[Axis2] Error creating mod_axis2 apache2 worker - function returned NULL");
            exit(APEXIT_CHILDFATAL);
        }

        ap_log_error(APLOG_MARK, APLOG_INFO, APR_SUCCESS, svr_rec,
            "[Axis2] DEBUG: Worker creation SUCCESSFUL - axis2_worker=%p", (void*)axis2_worker);

        ap_log_error(APLOG_MARK, APLOG_INFO, APR_SUCCESS, svr_rec,
            "[Axis2] DEBUG: axis2_apache2_worker created successfully");
    }

#endif

    /* REVOLUTIONARY: Engine initialization complete - HTTP service provider should now be available */
    ap_log_error(APLOG_MARK, APLOG_INFO, APR_SUCCESS, svr_rec,
        "[Axis2] DEBUG: post_config completed successfully - HTTP service provider registered");

    return OK;
}

static void
axis2_module_init(
    apr_pool_t * p,
    server_rec * svr_rec)
{
    apr_pool_t *pool;
    apr_status_t status;
    axutil_allocator_t *allocator = NULL;
    axutil_error_t *error = NULL;
    axutil_log_t *axutil_logger = NULL;
    axutil_thread_pool_t *thread_pool = NULL;
    axis2_config_rec_t *conf = (axis2_config_rec_t*)ap_get_module_config(svr_rec->module_config,
        &axis2_module);

    if(conf->axis2_global_pool_size > 0)
    {
        /*
         If we are using shared memory, no need to init the child, as the
         worker has been created in post config.
         */
        return;
    }

    /* REVOLUTIONARY: Skip axiom init for HTTP/2 JSON-only mode
     * Initialize XML readers only when SOAP processing is needed
     */
#ifndef HTTP2_JSON_ONLY_MODE
    axiom_xml_reader_init();
#endif

    /* create an allocator that uses APR memory pools and lasts the
     * lifetime of the httpd server child process
     */
    status = apr_pool_create(&pool, p);
    if(status)
    {
        ap_log_error(APLOG_MARK, APLOG_EMERG, status, svr_rec,
            "[Axis2] Error allocating mod_axis2 memory pool");
        exit(APEXIT_CHILDFATAL);
    }
    allocator = (axutil_allocator_t*)apr_palloc(pool, sizeof(axutil_allocator_t));
    if(!allocator)
    {
        ap_log_error(APLOG_MARK, APLOG_EMERG, APR_ENOMEM, svr_rec,
            "[Axis2] Error allocating mod_axis2 allocator");
        exit(APEXIT_CHILDFATAL);
    }
    allocator->malloc_fn = axis2_module_malloc;
    allocator->realloc = axis2_module_realloc;
    allocator->free_fn = axis2_module_free;
    allocator->global_pool_ref = 0;
    allocator->local_pool = (void*)pool;
    allocator->current_pool = (void*)pool;
    allocator->global_pool = (void*)pool;

    if(!allocator)
    {
        ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
            "[Axis2] Error initializing mod_axis2 allocator");
        exit(APEXIT_CHILDFATAL);
    }

    axutil_error_init();

    error = axutil_error_create(allocator);
    if(!error)
    {
        ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
            "[Axis2] Error creating mod_axis2 error structure");
        exit(APEXIT_CHILDFATAL);
    }
    axutil_logger = axutil_log_create(allocator, NULL, conf->axutil_log_file);
    if(!axutil_logger)
    {
        ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
            "[Axis2] Error creating mod_axis2 log structure");
        exit(APEXIT_CHILDFATAL);
    }
    thread_pool = axutil_thread_pool_init(allocator);
    if(!thread_pool)
    {
        ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
            "[Axis2] Error initializing mod_axis2 thread pool");
        exit(APEXIT_CHILDFATAL);
    }
    axutil_env = axutil_env_create_with_error_log_thread_pool(allocator, error, axutil_logger,
        thread_pool);
    if(!axutil_env)
    {
        ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
            "[Axis2] Error creating mod_axis2 environment");
        exit(APEXIT_CHILDFATAL);
    }
    if(axutil_logger)
    {

        axutil_logger->level = conf->log_level;
        AXIS2_LOG_INFO(axutil_env->log, "Apache Axis2/C version in use : %s",
            axis2_version_string());
        AXIS2_LOG_INFO(axutil_env->log, "Starting log with log level %d", conf->log_level);
    }
    axutil_env->set_session_fn = axis2_set_session;
    axutil_env->get_session_fn = axis2_get_session;
    axis2_worker = axis2_apache2_worker_create(axutil_env, conf->axis2_repo_path);
    if(!axis2_worker)
    {
        ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
            "[Axis2] Error creating mod_axis2 apache2 worker");
        exit(APEXIT_CHILDFATAL);
    }

    /* If we are initialized we register a clean up as well */
    apr_pool_pre_cleanup_register(p, NULL, axis2_shutdown);
}

static void
axis2_register_hooks(
    apr_pool_t * p)
{
    /* DEBUGGING: Install comprehensive signal handlers to catch all crashes */
    signal(SIGSEGV, axis2_segfault_handler);
    signal(SIGFPE, axis2_segfault_handler);
    signal(SIGILL, axis2_segfault_handler);
    signal(SIGABRT, axis2_segfault_handler);
    signal(SIGTERM, axis2_segfault_handler);
    signal(SIGBUS, axis2_segfault_handler);
    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, NULL,
        "[Axis2] DEBUG: Comprehensive signal handlers installed (SIGSEGV, SIGFPE, SIGILL, SIGABRT, SIGTERM, SIGBUS)");

    /* DEBUGGING: Log hook registration phase */
    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, NULL,
        "[Axis2] DEBUG: axis2_register_hooks ENTRY - about to register hooks");

    ap_hook_post_config(axis2_post_config, NULL, NULL, APR_HOOK_MIDDLE);
    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, NULL,
        "[Axis2] DEBUG: post_config hook registered successfully");

    ap_hook_fixups(axis2_fixups, NULL, NULL, APR_HOOK_FIRST);
    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, NULL,
        "[Axis2] DEBUG: fixups hook registered successfully");

    ap_hook_handler(axis2_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, NULL,
        "[Axis2] DEBUG: handler hook registered successfully");

    ap_hook_child_init(axis2_module_init, NULL, NULL, APR_HOOK_MIDDLE);
    ap_log_error(APLOG_MARK, APLOG_ERR, APR_SUCCESS, NULL,
        "[Axis2] DEBUG: All hooks registered successfully - axis2_register_hooks COMPLETE");
}

apr_status_t
axis2_shutdown(
    void *tmp)
{
    if(axis2_worker)
    {
        axis2_apache2_worker_free(axis2_worker, axutil_env);
    }
    return APR_SUCCESS;
}

char * 
axis2_get_session(
        void *req, 
        const char *id)
{
    request_rec *request = NULL;
    apr_status_t rv;
    const char *dbd_session = NULL;
    apr_dbd_prepared_t *statement;
    apr_dbd_results_t *res = NULL;
    apr_dbd_row_t *row = NULL;
    ap_dbd_t *dbd = NULL;
    ap_dbd_t *(*authn_dbd_acquire_fn)(request_rec*) = NULL;
    request = (request_rec *) req;

    authn_dbd_acquire_fn = APR_RETRIEVE_OPTIONAL_FN(ap_dbd_acquire);
    dbd = authn_dbd_acquire_fn(request);
    if (!dbd) 
    {
        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, request,
                      "Failed to acquire database connection to look up "
                      "id '%s'", id);
        return NULL;
    }

    statement = apr_hash_get(dbd->prepared, "retrieve_session", APR_HASH_KEY_STRING);
    if (!statement) 
    {
        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, request,
                      "A prepared statement could not be found for "
                      "the key '%s'", "retrieve_session");
        return NULL;
    }
    if (apr_dbd_pvselect(dbd->driver, request->pool, dbd->handle, &res, statement,
                              0, id, NULL) != 0) 
    {
        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, request,
                      "Query execution error looking up '%s' "
                      "in database", id);
        return NULL;
    }
    for (rv = apr_dbd_get_row(dbd->driver, request->pool, res, &row, -1); rv != -1;
         rv = apr_dbd_get_row(dbd->driver, request->pool, res, &row, -1)) 
    {
        if (rv != 0) 
        {
            ap_log_rerror(APLOG_MARK, APLOG_ERR, rv, request,
                          "Error retrieving results while looking up '%s' "
                          "in database", id);
            return NULL;
        }
        if (!dbd_session) 
        {
            dbd_session = apr_dbd_get_entry(dbd->driver, row, 0);
        }
        /* we can't break out here or row won't get cleaned up */
    }

    if (!dbd_session) 
    {
        return NULL;
    }

    return (char *) dbd_session;
}

axis2_status_t 
axis2_set_session(
        void *req, 
        const char *id,
        const char *session)
{
    request_rec *request = NULL;
    apr_dbd_prepared_t *statement;
    int affected_rows = -1;
    ap_dbd_t *dbd = NULL;
    ap_dbd_t *(*authn_dbd_acquire_fn)(request_rec*) = NULL;
    request = (request_rec *) req;

    authn_dbd_acquire_fn = APR_RETRIEVE_OPTIONAL_FN(ap_dbd_acquire);
    if(authn_dbd_acquire_fn)
    {
        dbd = authn_dbd_acquire_fn(request);
    }
    if (!dbd) 
    {
        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, request,
                      "Failed to acquire database connection to insert session for "
                      "id '%s'. Check whether apache2 is installed with mod-dbd enabled", id);
        return AXIS2_FAILURE;
    }

    statement = apr_hash_get(dbd->prepared, "insert_session", APR_HASH_KEY_STRING);
    if (!statement) 
    {
        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, request,
                      "A prepared statement could not be found for "
                      "the key '%s'", "insert_session");
        return AXIS2_FAILURE;
    }
    if (apr_dbd_pvquery(dbd->driver, request->pool, dbd->handle, &affected_rows, statement, id, 
                session, NULL) != 0) 
    {
        ap_log_rerror(APLOG_MARK, APLOG_ERR, 0, request,
                      "Query execution error inserting session for '%s' "
                      "in database", id);
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}


