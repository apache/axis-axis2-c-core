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

#ifndef CUT_HTTP_SERVER_H
#define CUT_HTTP_SERVER_H

#include <axis2_http_server.h>
#include <axis2_http_transport.h>
#include <platforms/axutil_platform_auto_sense.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axutil_thread_pool.h>
#include <axutil_types.h>
#include <axiom_xml_reader.h>
#include <axutil_version.h>
#include <axutil_thread.h>
#include <axutil_file_handler.h>
#include <axis2_const.h>

#ifdef __cplusplus
extern "C"
{
#endif

axutil_env_t *system_env = NULL;
axutil_thread_t *td_http_server = NULL;
axis2_transport_receiver_t *server = NULL;
AXIS2_IMPORT extern int axis2_http_socket_read_timeout;
AXIS2_IMPORT extern axis2_char_t *axis2_request_url_prefix;
axutil_thread_t *thread_http_server = NULL;

#define DEFAULT_REPO_PATH "/usr/local/axis2c/"

/***************************** Function headers *******************************/
axutil_env_t *
init_system_env(
    axutil_allocator_t * allocator,
    const axis2_char_t * log_file);

void
server_free(
    axutil_env_t * env);

axutil_env_t *
init_system_env(
    axutil_allocator_t * allocator,
    const axis2_char_t * log_file);

void * AXIS2_CALL
http_server(axutil_thread_t *td, void *param);

/***************************** End of function headers ************************/
axutil_env_t *
init_system_env(
    axutil_allocator_t * allocator,
    const axis2_char_t * log_file)
{
    axutil_error_t *error = axutil_error_create(allocator);
    axutil_log_t *log = axutil_log_create(allocator, NULL, log_file);
    /* if (!log) */

    /* 		  log = axutil_log_create_default (allocator); */
    axutil_thread_pool_t *thread_pool = axutil_thread_pool_init(allocator);
    /* We need to init the parser in main thread before spawning child
     * threads
     */
    axiom_xml_reader_init();
    return axutil_env_create_with_error_log_thread_pool(allocator, error, log, thread_pool);
}

void
server_free(
    axutil_env_t * env)
{
    axutil_allocator_t *allocator = NULL;
    if(server)
    {
        axis2_transport_receiver_free(server, system_env);
    }
    if(env)
    {
        allocator = env->allocator;
        axutil_env_free(env);
    }
    /*axutil_allocator_free(allocator); */
    axiom_xml_reader_cleanup();
}

void * AXIS2_CALL
http_server(axutil_thread_t *td, void *param)
{
    axutil_allocator_t *allocator = NULL;
    axutil_env_t *env = NULL;
    unsigned int len;
    int log_file_size = AXUTIL_LOG_FILE_SIZE;
    unsigned int file_flag = 0;
    axutil_log_levels_t log_level = AXIS2_LOG_LEVEL_DEBUG;
    const axis2_char_t *log_file = "axis2.log";
    const axis2_char_t *repo_path = NULL;
    int port = 9090;
    axis2_status_t status;

	repo_path = getenv("AXIS2C_HOME");
	if (!repo_path) repo_path = DEFAULT_REPO_PATH;
    td_http_server = td;
    /* Set the service URL prefix to be used. This could default to services if not 
     set with AXIS2_REQUEST_URL_PREFIX macro at compile time */
    axis2_request_url_prefix = AXIS2_REQUEST_URL_PREFIX;

    allocator = axutil_allocator_init(NULL);

    if(!allocator)
    {
        server_free(NULL);
        return NULL;
    }

    env = init_system_env(allocator, log_file);
    CUT_ASSERT(env != NULL);
    if (env == NULL) 
    {
        server_free(NULL);
        return NULL;
    }
    env->log->level = log_level;
    env->log->size = log_file_size;

    axutil_error_init();
    system_env = env;

    AXIS2_LOG_INFO(env->log, "Starting Axis2 HTTP server....");
    AXIS2_LOG_INFO(env->log, "Apache Axis2/C version in use : %s", axis2_version_string());
    AXIS2_LOG_INFO(env->log, "Server port : %d", port);
    AXIS2_LOG_INFO(env->log, "Repo location : %s", repo_path);
    AXIS2_LOG_INFO(env->log, "Read Timeout : %d ms", axis2_http_socket_read_timeout);

    status = axutil_file_handler_access(repo_path, AXIS2_R_OK);
    if(status == AXIS2_SUCCESS)
    {
        len = (unsigned int)strlen(repo_path);
        /* We are sure that the difference lies within the unsigned int range */
        if((len >= 9) && !strcmp((repo_path + (len - 9)), "axis2.xml"))
        {
            file_flag = 1;
        }
    }
    else
    {
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI, "provided repo path %s does "
            "not exist or no permissions to read, set "
            "repo_path to DEFAULT_REPO_PATH", repo_path);
        repo_path = DEFAULT_REPO_PATH;
    }

    if(!file_flag)
        server = axis2_http_server_create(env, repo_path, port);
    else
        server = axis2_http_server_create_with_file(env, repo_path, port);

    if(!server)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Server creation failed: Error code:" " %d :: %s",
            env->error->error_number, AXIS2_ERROR_GET_MESSAGE(env->error));
        server_free(env);
        return NULL;

    }
    printf("Start Simple Axis2 HTTP Server ...\n");
    if(axis2_transport_receiver_start(server, env) == AXIS2_FAILURE)
    {
        printf("start error\n");
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Server start failed: Error code:" " %d :: %s",
            env->error->error_number, AXIS2_ERROR_GET_MESSAGE(env->error));
        server_free(env);
        return NULL;
    }
    printf("OK\n");
    return NULL;
}

static int ut_start_http_server(axutil_env_t *env)
{
    thread_http_server = axutil_thread_create(env->allocator, NULL, http_server, NULL);
    CUT_ASSERT(thread_http_server != NULL);
    if (!thread_http_server) return -1;
    AXIS2_SLEEP(2);
	return 0;
}

static void ut_stop_http_server(axutil_env_t *env)
{
    axis2_status_t rv;
    if ( !thread_http_server ) return;
	rv = axutil_thread_exit(thread_http_server, env->allocator);
	thread_http_server = NULL;
}

#ifdef __cplusplus
}
#endif
#endif                          /* CUT_HTTP_SERVER_H */
