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

#include <platforms/axis2_platform_auto_sense.h> 
#include <axis2_http_svr_thread.h>
#include <axis2_http_transport.h>
#include <axis2_string.h>
#include <axis2_network_handler.h>
#include <axis2_http_simple_request.h>
#include <axis2_simple_http_svr_conn.h>
#include <axis2_url.h>
#include <axis2_error_default.h>
#include <axis2_xml_reader.h>
#include <signal.h>



/** 
 * @brief HTTP Server Thread struct impl
 *	Axis2 HTTP Server Thread impl  
 */
typedef struct axis2_http_svr_thread_impl axis2_http_svr_thread_impl_t;  
typedef struct axis2_http_svr_thd_args axis2_http_svr_thd_args_t;
struct axis2_http_svr_thread_impl
{
	axis2_http_svr_thread_t svr_thread;
	int listen_socket;
	axis2_bool_t stopped;
	axis2_http_worker_t *worker;
	int port;
};

struct axis2_http_svr_thd_args
{
	axis2_env_t **env;
	axis2_socket_t socket;
	axis2_http_worker_t *worker;	
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
AXIS2_DECLARE (axis2_env_t*)
init_thread_env(axis2_env_t **system_env);

void * AXIS2_THREAD_FUNC
worker_func(axis2_thread_t *thd, void *data);
/***************************** End of function headers ************************/

axis2_http_svr_thread_t* AXIS2_CALL
axis2_http_svr_thread_create (axis2_env_t **env, int port)
{
    axis2_http_svr_thread_impl_t *svr_thread_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
            
    svr_thread_impl = (axis2_http_svr_thread_impl_t *)AXIS2_MALLOC 
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
    svr_thread_impl->svr_thread.ops = NULL;
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
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
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
	
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svr_thread_impl = AXIS2_INTF_TO_IMPL(svr_thread);
	
	while(AXIS2_FALSE == svr_thread_impl->stopped)
	{
		int socket = -1;
	 	axis2_http_svr_thd_args_t *arg_list = NULL;
		axis2_thread_t *worker_thread = NULL;
		
		socket = axis2_network_handler_svr_socket_accept(env, 
						svr_thread_impl->listen_socket);
		if(NULL == svr_thread_impl->worker)
		{
			AXIS2_LOG_WARNING((*env)->log, AXIS2_LOG_SI, "Worker not ready yet."
						" Cannot serve the request");
			axis2_network_handler_close_socket(env, socket);
			continue;
		}
		arg_list = AXIS2_MALLOC((*env)->allocator, 
						sizeof(axis2_http_svr_thd_args_t));
		if(NULL == arg_list)
		{
			AXIS2_LOG_ERROR((*env)->log, AXIS2_LOG_SI, 
						"Memory allocation error in the svr thread loop");
			continue;			
		}
		arg_list->env = env;
		arg_list->socket = socket;
		arg_list->worker = svr_thread_impl->worker;
#ifdef AXIS2_SVR_MULTI_THREADED
		worker_thread = AXIS2_THREAD_POOL_GET_THREAD((*env)->thread_pool, 
                        worker_func, (void*)arg_list);
		if(NULL == worker_thread)
		{
			AXIS2_LOG_ERROR((*env)->log, AXIS2_LOG_SI, "Thread creation failed"
						"server thread loop");
			continue;
		}
		AXIS2_THREAD_POOL_THREAD_DETACH((*env)->thread_pool, worker_thread);
#else
		worker_func(NULL, (void*)arg_list);
#endif
	}
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_http_svr_thread_destroy(axis2_http_svr_thread_t *svr_thread, 
						axis2_env_t **env)
{
	axis2_http_svr_thread_impl_t *svr_thread_impl = NULL;
		
    AXIS2_ENV_CHECK(env, AXIS2_CRTICAL_FAILURE);
	
	svr_thread_impl = AXIS2_INTF_TO_IMPL(svr_thread);
	if(AXIS2_TRUE == svr_thread_impl->stopped)
	{
		return AXIS2_SUCCESS;
	}
	svr_thread_impl->stopped = AXIS2_TRUE;
	AXIS2_LOG_DEBUG((*env)->log, AXIS2_LOG_SI, "Terminating HTTP server "
						"thread.");
	if(svr_thread_impl->listen_socket)
	{
		axis2_network_handler_close_socket(env, svr_thread_impl->listen_socket);
		svr_thread_impl->listen_socket = -1;
	}
	/* TODO: stop all the child threads */
	return AXIS2_SUCCESS;
}


int AXIS2_CALL 
axis2_http_svr_thread_get_local_port
						(axis2_http_svr_thread_t *svr_thread, 
						axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, AXIS2_CRTICAL_FAILURE);
    return AXIS2_INTF_TO_IMPL(svr_thread)->port;
}

axis2_bool_t AXIS2_CALL 
axis2_http_svr_thread_is_running(axis2_http_svr_thread_t *svr_thread, 
						axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	return AXIS2_INTF_TO_IMPL(svr_thread)->port;
}

axis2_status_t AXIS2_CALL 
axis2_http_svr_thread_set_worker(axis2_http_svr_thread_t *svr_thread, 
						axis2_env_t **env, axis2_http_worker_t *worker)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, worker, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(svr_thread)->worker = worker;
	return AXIS2_SUCCESS;
}


AXIS2_DECLARE (axis2_env_t*)
axis2_init_thread_env(axis2_env_t **system_env)
{
	axis2_error_t *error = axis2_error_create((*system_env)->allocator);
	return axis2_env_create_with_error_log((*system_env)->allocator, error, 
						(*system_env)->log);
}

/**
 * Thread worker function. 
 */
void * AXIS2_THREAD_FUNC
worker_func(axis2_thread_t *thd, void *data)
{
	struct AXIS2_PLATFORM_TIMEB t1, t2;  
	axis2_simple_http_svr_conn_t *svr_conn = NULL;
	axis2_http_simple_request_t *request = NULL;
	int millisecs = 0;
	double secs = 0;
	axis2_http_worker_t *tmp = NULL;
	axis2_status_t status = AXIS2_FAILURE;
	axis2_env_t **env = NULL;
	axis2_socket_t socket;
	axis2_env_t *thread_env = NULL;
	axis2_http_svr_thd_args_t *arg_list = NULL;

#ifndef WIN32
#ifdef AXIS2_SVR_MULTI_THREADED
    signal(SIGPIPE, SIG_IGN);
#endif
#endif
    	
	arg_list = (axis2_http_svr_thd_args_t*)data;
	if(NULL == arg_list)
	{
		return NULL;
	}
	AXIS2_PLATFORM_GET_TIME_IN_MILLIS(&t1);
	env = arg_list->env;
	thread_env = axis2_init_thread_env(env);
	socket = arg_list->socket;
	svr_conn = axis2_simple_http_svr_conn_create(&thread_env, socket);
	AXIS2_SIMPLE_HTTP_SVR_CONN_SET_RCV_TIMEOUT(svr_conn, &thread_env, 
					axis2_http_socket_read_timeout);
	request = AXIS2_SIMPLE_HTTP_SVR_CONN_READ_REQUEST(svr_conn, &thread_env);
	tmp = arg_list->worker;
	status = AXIS2_HTTP_WORKER_PROCESS_REQUEST(tmp, &thread_env, svr_conn, 
						request);
	AXIS2_SIMPLE_HTTP_SVR_CONN_FREE(svr_conn, &thread_env);
    if (request)
        AXIS2_HTTP_SIMPLE_REQUEST_FREE(request, &thread_env);
	AXIS2_FREE(thread_env->allocator, arg_list);
	AXIS2_PLATFORM_GET_TIME_IN_MILLIS(&t2);
	millisecs = t2.millitm - t1.millitm;
	secs = difftime(t2.time, t1.time);
	if(millisecs < 0)
	{
		millisecs += 1000;
		secs--;
	}
	secs += millisecs/1000.0;
	
	if(status == AXIS2_SUCCESS)
	{
#if defined(WIN32)
		AXIS2_LOG_INFO(thread_env->log, "Request served successfully");
#else 		
       	AXIS2_LOG_INFO(thread_env->log, "Request served in %.3f seconds", secs);
#endif       	
	}
	else
	{
#if defined(WIN32)	
		AXIS2_LOG_WARNING(thread_env->log, AXIS2_LOG_SI, 
						"Error occured in processing request ");
#else
	    AXIS2_LOG_WARNING(thread_env->log, AXIS2_LOG_SI, 
						"Error occured in processing request (%.3f seconds)", 
						secs);
#endif											
	}
    axis2_env_free_masked(thread_env, 0x2);
    /*axis2_xml_reader_cleanup();*/
#ifdef AXIS2_SVR_MULTI_THREADED
	AXIS2_THREAD_POOL_EXIT_THREAD((*env)->thread_pool, thd);
#endif
	return NULL;
}
