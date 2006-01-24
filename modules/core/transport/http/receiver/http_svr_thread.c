/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <axis2_http_svr_thread.h>
#include <axis2_http_transport.h>
#include <axis2_string.h>
#include <axis2_network_handler.h>
#include <axis2_http_simple_request.h>
#include <axis2_simple_http_svr_conn.h>
#include <axis2_url.h>


/** 
 * @brief HTTP Server Thread struct impl
 *	Axis2 HTTP Server Thread impl  
 */
typedef struct axis2_http_svr_thread_impl axis2_http_svr_thread_impl_t;  
  
struct axis2_http_svr_thread_impl
{
	axis2_http_svr_thread_t svr_thread;
	int listen_socket;
	axis2_bool_t stopped;
	axis2_http_worker_t *worker;
	int port;
};

#define AXIS2_INTF_TO_IMPL(http_svr_thread) \
                ((axis2_http_svr_thread_impl_t *)(http_svr_thread))

/***************************** Function headers *******************************/
axis2_status_t AXIS2_CALL 
axis2_http_svr_thread_run(axis2_http_svr_thread_t *svr_thread, 
						axis2_env_t **env);
axis2_status_t AXIS2_CALL 
axis2_http_svr_thread_destroy(axis2_http_svr_thread_t *svr_thread, 
						axis2_env_t **env);
int AXIS2_CALL 
axis2_http_svr_thread_get_local_port
						(axis2_http_svr_thread_t *svr_thread, 
						axis2_env_t **env);
axis2_bool_t AXIS2_CALL 
axis2_http_svr_thread_is_running(axis2_http_svr_thread_t *svr_thread, 
						axis2_env_t **env);
axis2_status_t AXIS2_CALL 
axis2_http_svr_thread_set_worker(axis2_http_svr_thread_t *svr_thread, 
						axis2_env_t **env, axis2_http_worker_t *worker);
axis2_status_t AXIS2_CALL 
axis2_http_svr_thread_free (axis2_http_svr_thread_t *svr_thread, 
						axis2_env_t **env);					

/***************************** End of function headers ************************/

axis2_http_svr_thread_t* AXIS2_CALL
axis2_http_svr_thread_create (axis2_env_t **env, int port)
{
    AXIS2_ENV_CHECK(env, NULL);
            
    axis2_http_svr_thread_impl_t *svr_thread_impl = 
                        (axis2_http_svr_thread_impl_t *)AXIS2_MALLOC 
                        ((*env)->allocator, sizeof(
						axis2_http_svr_thread_impl_t));
	
    if(NULL == svr_thread_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
	svr_thread_impl->worker = NULL;
	svr_thread_impl->stopped = AXIS2_FALSE;
	svr_thread_impl->port = port;
	svr_thread_impl->listen_socket = axis2_network_handler_create_server_socket
                        (env, svr_thread_impl->port);
    if(-1 == svr_thread_impl->listen_socket)
    {
        axis2_http_svr_thread_free((axis2_http_svr_thread_t*) svr_thread_impl,
                        env);
        return NULL;
    }	
    svr_thread_impl->svr_thread.ops = AXIS2_MALLOC((*env)->allocator,
						sizeof(axis2_http_svr_thread_ops_t));
    if(NULL == svr_thread_impl->svr_thread.ops)
	{
		axis2_http_svr_thread_free((axis2_http_svr_thread_t*) svr_thread_impl, 
						env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    svr_thread_impl->svr_thread.ops->run = axis2_http_svr_thread_run;                        
    svr_thread_impl->svr_thread.ops->destroy = 
						axis2_http_svr_thread_destroy;
    svr_thread_impl->svr_thread.ops->get_local_port = 
						axis2_http_svr_thread_get_local_port;
	svr_thread_impl->svr_thread.ops->is_running = 
						axis2_http_svr_thread_is_running;
	svr_thread_impl->svr_thread.ops->set_worker = 
						axis2_http_svr_thread_set_worker;
    svr_thread_impl->svr_thread.ops->free = axis2_http_svr_thread_free;
                        
	return &(svr_thread_impl->svr_thread);
}


axis2_status_t AXIS2_CALL 
axis2_http_svr_thread_free (axis2_http_svr_thread_t *svr_thread, 
						axis2_env_t **env)
{
	axis2_http_svr_thread_impl_t *svr_thread_impl = NULL;
	AXIS2_FUNC_PARAM_CHECK(svr_thread, env, AXIS2_FAILURE);
    svr_thread_impl = AXIS2_INTF_TO_IMPL(svr_thread);
	
    if(NULL != svr_thread_impl->worker)
    {
        AXIS2_URL_FREE(svr_thread_impl->worker, env);
        svr_thread_impl->worker = NULL;
    }
    if(-1 != svr_thread_impl->listen_socket)
    {
        axis2_network_handler_close_socket(env, svr_thread_impl->listen_socket);
        svr_thread_impl->listen_socket = -1;
    }
	svr_thread_impl->stopped = AXIS2_TRUE;
    if(NULL != svr_thread->ops)
        AXIS2_FREE((*env)->allocator, svr_thread->ops);
    
	AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(svr_thread));
	return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_http_svr_thread_run(axis2_http_svr_thread_t *svr_thread, 
						axis2_env_t **env)
{
    axis2_http_svr_thread_impl_t *svr_thread_impl = NULL;
	
	AXIS2_FUNC_PARAM_CHECK(svr_thread, env, AXIS2_FAILURE);
    svr_thread_impl = AXIS2_INTF_TO_IMPL(svr_thread);
	
	while(AXIS2_FALSE == svr_thread_impl->stopped)
	{
		int socket = -1;
		struct AXIS2_PLATFORM_TIMEB t1, t2;
		axis2_simple_http_svr_conn_t *svr_conn = NULL;
		axis2_http_simple_request_t *request = NULL;
		char log_str[128];
		
		socket = axis2_network_handler_svr_socket_accept(env, 
						svr_thread_impl->listen_socket);
		if(NULL == svr_thread_impl->worker)
		{
			continue;
		}
		AXIS2_PLATFORM_GET_TIME_IN_MILLIS(&t1);
		svr_conn = axis2_simple_http_svr_conn_create(env, socket);
		request = AXIS2_SIMPLE_HTTP_SVR_CONN_READ_REQUEST(svr_conn, env);
		axis2_http_worker_t *tmp = svr_thread_impl->worker;        
		tmp = svr_thread_impl->worker;
		AXIS2_HTTP_WORKER_PROCESS_REQUEST(tmp, env, svr_conn, request);
		AXIS2_SIMPLE_HTTP_SVR_CONN_FREE(svr_conn, env);
		AXIS2_PLATFORM_GET_TIME_IN_MILLIS(&t2);
		int millisecs = t2.millitm - t1.millitm;
		double secs = difftime(t2.time, t1.time);
		if(millisecs < 0)
		{
			millisecs += 1000;
			secs--;
		}
		secs += millisecs/1000.0;
		sprintf(log_str, "[Axis2]Request served in %.3f seconds", secs);
		AXIS2_LOG_WRITE((*env)->log, log_str, 
						AXIS2_LOG_INFO);
	}
	
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_http_svr_thread_destroy(axis2_http_svr_thread_t *svr_thread, 
						axis2_env_t **env)
{
	axis2_http_svr_thread_impl_t *svr_thread_impl = NULL;
		
    AXIS2_FUNC_PARAM_CHECK(svr_thread, env, AXIS2_CRTICAL_FAILURE);
	
	svr_thread_impl = AXIS2_INTF_TO_IMPL(svr_thread);
	if(AXIS2_TRUE == svr_thread_impl->stopped)
	{
		return AXIS2_SUCCESS;
	}
	svr_thread_impl->stopped = AXIS2_TRUE;
	/* TODO log*/
	if(svr_thread_impl->listen_socket)
	{
		axis2_network_handler_close_socket(env, svr_thread_impl->listen_socket);
		svr_thread_impl->listen_socket = -1;
	}
	/* TODO: stop all the chiled threads */
	return AXIS2_SUCCESS;
}


int AXIS2_CALL 
axis2_http_svr_thread_get_local_port
						(axis2_http_svr_thread_t *svr_thread, 
						axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svr_thread, env, AXIS2_CRTICAL_FAILURE);
    return AXIS2_INTF_TO_IMPL(svr_thread)->port;
}

axis2_bool_t AXIS2_CALL 
axis2_http_svr_thread_is_running(axis2_http_svr_thread_t *svr_thread, 
						axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svr_thread, env, AXIS2_FAILURE);
	return AXIS2_INTF_TO_IMPL(svr_thread)->port;
}

axis2_status_t AXIS2_CALL 
axis2_http_svr_thread_set_worker(axis2_http_svr_thread_t *svr_thread, 
						axis2_env_t **env, axis2_http_worker_t *worker)
{
    AXIS2_FUNC_PARAM_CHECK(svr_thread, env, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, worker, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(svr_thread)->worker = worker;
	return AXIS2_SUCCESS;
}
