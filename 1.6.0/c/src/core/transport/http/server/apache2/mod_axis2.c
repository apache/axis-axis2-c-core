
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
#include <axutil_log_default.h>
#include <axutil_thread_pool.h>
#include <axiom_xml_reader.h>
#include <axutil_version.h>
#include <apr_rmm.h>
#include <apr_shm.h>
#include <axis2_http_transport.h>

/* Configuration structure populated by apache2.conf */
typedef struct axis2_config_rec
{
    char *axutil_log_file;
    char *axis2_repo_path;
    axutil_log_levels_t log_level;
    int max_log_file_size;
	int axis2_global_pool_size;
}
axis2_config_rec_t;

axis2_apache2_worker_t *axis2_worker = NULL;
const axutil_env_t *axutil_env = NULL;
apr_rmm_t* rmm = NULL;
apr_global_mutex_t *global_mutex = NULL;

/******************************Function Headers********************************/
static void *axis2_create_svr(
    apr_pool_t * p,
    server_rec * s);

static const char *axis2_set_repo_path(
    cmd_parms * cmd,
    void *dummy,
    const char *arg);

static const char *axis2_set_log_file(
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

static const char *axis2_set_log_level(
    cmd_parms * cmd,
    void *dummy,
    const char *arg);

static const char *axis2_set_svc_url_prefix(
    cmd_parms * cmd,
    void *dummy,
    const char *arg);

static int axis2_handler(
    request_rec * req);

/* Shutdown Axis2 */
apr_status_t 
axis2_shutdown(void *tmp);

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

static void axis2_module_init(
    apr_pool_t * p,
    server_rec * svr_rec);

static void axis2_register_hooks(
    apr_pool_t * p);

/***************************End of Function Headers****************************/

static const command_rec axis2_cmds[] = {
    AP_INIT_TAKE1("Axis2RepoPath", axis2_set_repo_path, NULL, RSRC_CONF,
                  "Axis2/C repository path"),
    AP_INIT_TAKE1("Axis2LogFile", axis2_set_log_file, NULL, RSRC_CONF,
                  "Axis2/C log file name"),
    AP_INIT_TAKE1("Axis2LogLevel", axis2_set_log_level, NULL, RSRC_CONF,
                  "Axis2/C log level"),
    AP_INIT_TAKE1("Axis2MaxLogFileSize", axis2_set_max_log_file_size, NULL, RSRC_CONF,
                  "Axis2/C maximum log file size"),
    AP_INIT_TAKE1("Axis2GlobalPoolSize", axis2_set_global_pool_size, NULL, RSRC_CONF,
                  "Axis2/C global pool size"),
    AP_INIT_TAKE1("Axis2ServiceURLPrefix", axis2_set_svc_url_prefix, NULL,
                  RSRC_CONF,
                  "Axis2/C service URL prifix"),
    {NULL}
};

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA axis2_module = {
    STANDARD20_MODULE_STUFF,
    NULL,                       /* create per-dir    config structures */
    NULL,                       /* merge  per-dir    config structures */
    axis2_create_svr,           /* create per-server config structures */
    NULL,                       /* merge  per-server config structures */
    axis2_cmds,                 /* table of config file commands       */
    axis2_register_hooks        /* register hooks                      */
};

static void *
axis2_create_svr(
    apr_pool_t * p,
    server_rec * s)
{
    axis2_config_rec_t *conf = apr_palloc(p, sizeof(*conf));
    conf->axutil_log_file = NULL;
    conf->axis2_repo_path = NULL;
    conf->log_level = AXIS2_LOG_LEVEL_DEBUG;
    conf->axis2_global_pool_size = 0;
	conf->max_log_file_size = 1;
    return conf;
}

static const char *
axis2_set_repo_path(
    cmd_parms * cmd,
    void *dummy,
    const char *arg)
{
    axis2_config_rec_t *conf = NULL;
    const char *err = ap_check_cmd_context(cmd, GLOBAL_ONLY);
    if (err != NULL)
    {
        return err;
    }
    conf =
        (axis2_config_rec_t *) ap_get_module_config(cmd->server->module_config,
                                                    &axis2_module);
    conf->axis2_repo_path = apr_pstrdup(cmd->pool, arg);
    return NULL;
}

static const char *
axis2_set_log_file(
    cmd_parms * cmd,
    void *dummy,
    const char *arg)
{
    axis2_config_rec_t *conf = NULL;
    const char *err = ap_check_cmd_context(cmd, GLOBAL_ONLY);
    if (err != NULL)
    {
        return err;
    }

    conf =
        (axis2_config_rec_t *) ap_get_module_config(cmd->server->module_config,
                                                    &axis2_module);
    conf->axutil_log_file = apr_pstrdup(cmd->pool, arg);
    return NULL;
}

static const char *
axis2_set_max_log_file_size(
    cmd_parms * cmd,
    void *dummy,
    const char *arg)
{
    axis2_config_rec_t *conf = NULL;
    const char *err = ap_check_cmd_context(cmd, GLOBAL_ONLY);
    if (err != NULL)
    {
        return err;
    }

    conf =
        (axis2_config_rec_t *) ap_get_module_config(cmd->server->module_config,
                                                    &axis2_module);
    conf->max_log_file_size = 1024 * 1024 * atoi(arg);
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
    if (err != NULL)
    {
        return err;
    }

    conf =
        (axis2_config_rec_t *) ap_get_module_config(cmd->server->module_config,
                                                    &axis2_module);
    conf->axis2_global_pool_size = 1024 * 1024 * atoi(arg);
    return NULL;
}

static const char *
axis2_set_log_level(
    cmd_parms * cmd,
    void *dummy,
    const char *arg)
{
    char *str;
    axutil_log_levels_t level = AXIS2_LOG_LEVEL_DEBUG;
    axis2_config_rec_t *conf = NULL;
    const char *err = ap_check_cmd_context(cmd, GLOBAL_ONLY);
    if (err != NULL)
    {
        return err;
    }

    conf =
        (axis2_config_rec_t *) ap_get_module_config(cmd->server->module_config,
                                                    &axis2_module);

    str = ap_getword_conf(cmd->pool, &arg);
    if (str)
    {
        if (!strcasecmp(str, "crit"))
        {
            level = AXIS2_LOG_LEVEL_CRITICAL;
        }
        else if (!strcasecmp(str, "error"))
        {
            level = AXIS2_LOG_LEVEL_ERROR;
        }
        else if (!strcasecmp(str, "warn"))
        {
            level = AXIS2_LOG_LEVEL_WARNING;
        }
        else if (!strcasecmp(str, "info"))
        {
            level = AXIS2_LOG_LEVEL_INFO;
        }
        else if (!strcasecmp(str, "debug"))
        {
            level = AXIS2_LOG_LEVEL_DEBUG;
        }
        else if (!strcasecmp(str, "user"))
        {
            level = AXIS2_LOG_LEVEL_USER;
        }
        else if (!strcasecmp(str, "trace"))
        {
            level = AXIS2_LOG_LEVEL_TRACE;
        }
    }
    conf->log_level = level;
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
    if (!err)
    {
        axis2_char_t *prefix = apr_pstrdup(cmd->pool, arg);
        if (prefix)
            axis2_request_url_prefix = prefix;
    }

    return NULL;
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

    if (strcmp(req->handler, "axis2_module"))
    {
        return DECLINED;
    }

    /* Set up the read policy from the client. */
    if ((rv = ap_setup_client_block(req, REQUEST_CHUNKED_DECHUNK)) != OK)
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

    allocator = (axutil_allocator_t *) apr_palloc(local_pool,
                                                  sizeof(axutil_allocator_t));

    if (!allocator)
    {
        return HTTP_INTERNAL_SERVER_ERROR;
    }
    allocator->malloc_fn = axis2_module_malloc;
    allocator->realloc = axis2_module_realloc;
    allocator->free_fn = axis2_module_free;
    allocator->local_pool = (void *)local_pool;
    allocator->current_pool = (void *)local_pool;
    allocator->global_pool = axutil_env->allocator->global_pool;

    error = axutil_error_create(allocator);
    thread_env = axutil_env_create_with_error_log_thread_pool(allocator,
                                                        error, axutil_env->log,
                                                        axutil_env->
                                                        thread_pool);
    thread_env->allocator = allocator;

    rv = AXIS2_APACHE2_WORKER_PROCESS_REQUEST(axis2_worker, thread_env, req);
    if (AXIS2_CRITICAL_FAILURE == rv)
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
	return apr_palloc((apr_pool_t *) (allocator->current_pool), size);
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

static int axis2_post_config(apr_pool_t *pconf, apr_pool_t *plog,
								 apr_pool_t *ptemp, server_rec *svr_rec)
{
	apr_status_t status = APR_SUCCESS;
    axutil_allocator_t *allocator = NULL;
    axutil_error_t *error = NULL;
    axutil_log_t *axutil_logger = NULL;
    axutil_thread_pool_t *thread_pool = NULL;
	void *data = NULL;
	const char *userdata_key = "axis2_init";
	axis2_config_rec_t *conf =
		(axis2_config_rec_t *) ap_get_module_config(svr_rec->module_config,
													&axis2_module);

	/* axis2_post_config() will be called twice. Don't bother
	 * going through all of the initialization on the first call
	 * because it will just be thrown away.*/
    
    ap_add_version_component(pconf, AXIS2_HTTP_HEADER_SERVER_AXIS2C);

	apr_pool_userdata_get(&data, userdata_key, svr_rec->process->pool);
	if (!data) 
	{
		apr_pool_userdata_set((const void *)1, userdata_key,
								apr_pool_cleanup_null, svr_rec->process->pool);
		return OK;
	}

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
		allocator->local_pool = (void *) rmm;
		allocator->current_pool = (void *) rmm;
		allocator->global_pool = (void *) rmm;

        /* We need to init xml readers before we go into threaded env
         */
        axiom_xml_reader_init();
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

	    axis2_worker = axis2_apache2_worker_create(axutil_env,
												    conf->axis2_repo_path);
        if (!axis2_worker)
        {
            ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
                         "[Axis2] Error creating mod_axis2 apache2 worker");
            exit(APEXIT_CHILDFATAL);
        }
	}
	
#endif
    
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
    axis2_config_rec_t *conf = (axis2_config_rec_t*)ap_get_module_config(
                svr_rec->module_config, &axis2_module);

    if (conf->axis2_global_pool_size > 0)
	{
        /*
            If we are using shared memory, no need to init the child, as the 
            worker has been created in post config.
        */
        return;
    }


    /* We need to init xml readers before we go into threaded env
     */
    axiom_xml_reader_init();

    /* create an allocator that uses APR memory pools and lasts the
     * lifetime of the httpd server child process
     */
    status = apr_pool_create(&pool, p);
    if (status)
    {
        ap_log_error(APLOG_MARK, APLOG_EMERG, status, svr_rec,
                     "[Axis2] Error allocating mod_axis2 memory pool");
        exit(APEXIT_CHILDFATAL);
    }
    allocator = (axutil_allocator_t*) apr_palloc(pool,
                                                sizeof(axutil_allocator_t));
    if (! allocator)
    {
        ap_log_error(APLOG_MARK, APLOG_EMERG, APR_ENOMEM, svr_rec,
                     "[Axis2] Error allocating mod_axis2 allocator");
        exit(APEXIT_CHILDFATAL);
    }
    allocator->malloc_fn = axis2_module_malloc;
    allocator->realloc = axis2_module_realloc;
    allocator->free_fn = axis2_module_free;
    allocator->local_pool = (void*) pool;
    allocator->current_pool = (void*) pool;
    allocator->global_pool = (void*) pool;

    if (! allocator)
    {
        ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
                         "[Axis2] Error initializing mod_axis2 allocator");
        exit(APEXIT_CHILDFATAL);
    }
    
    axutil_error_init();
    
    error = axutil_error_create(allocator);
    if (! error)
    {
        ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
                     "[Axis2] Error creating mod_axis2 error structure");
        exit(APEXIT_CHILDFATAL);
    }
    axutil_logger = axutil_log_create(allocator, NULL, conf->axutil_log_file);
    if (! axutil_logger)
    {
        ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
                     "[Axis2] Error creating mod_axis2 log structure");
        exit(APEXIT_CHILDFATAL);
    }
    thread_pool = axutil_thread_pool_init(allocator);
    if (! thread_pool)
    {
        ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
                     "[Axis2] Error initializing mod_axis2 thread pool");
        exit(APEXIT_CHILDFATAL);
    }
    axutil_env = axutil_env_create_with_error_log_thread_pool(allocator, error,
            axutil_logger, thread_pool);
    if (! axutil_env)
    {
        ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
                     "[Axis2] Error creating mod_axis2 environment");
        exit(APEXIT_CHILDFATAL);
    }
    if (axutil_logger)
    {

        axutil_logger->level = conf->log_level;
        AXIS2_LOG_INFO(axutil_env->log, "Apache Axis2/C version in use : %s", 
            axis2_version_string());
        AXIS2_LOG_INFO(axutil_env->log, "Starting log with log level %d",
            conf->log_level);
    }
    axis2_worker = axis2_apache2_worker_create(axutil_env,
            conf->axis2_repo_path);
    if (! axis2_worker)
    {
        ap_log_error(APLOG_MARK, APLOG_EMERG, APR_EGENERAL, svr_rec,
                     "[Axis2] Error creating mod_axis2 apache2 worker");
        exit(APEXIT_CHILDFATAL);
    }
	/* If we are initialized we register a clean up as well */
	/* apr_pool_cleanup_register(p, NULL, axis2_shutdown, apr_pool_cleanup_null);*/
}

static void
axis2_register_hooks(
    apr_pool_t * p)
{
	ap_hook_post_config(axis2_post_config, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_handler(axis2_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_child_init(axis2_module_init, NULL, NULL, APR_HOOK_MIDDLE);
}

apr_status_t 
axis2_shutdown(void *tmp)
{
	if (axis2_worker)
	{
		axis2_apache2_worker_free(axis2_worker, axutil_env);
	}
	return APR_SUCCESS;
}

