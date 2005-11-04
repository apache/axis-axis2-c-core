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

#include <axis2_env.h>
#include <stdlib.h>

AXIS2_DECLARE(axis2_status_t) axis2_environment_free (axis2_env_t *env)
{
    if(NULL != env && NULL != env->allocator)
        free (env->allocator);

    if(NULL != env && NULL != env->stream)
		AXIS2_STREAM_FREE(env->stream);

    if(NULL != env && NULL != env->log)
        AXIS2_LOG_FREE(env->log);
	
	if(NULL != env && NULL != env->error)
        AXIS2_ERROR_FREE(env->error);
    
    if(NULL != env)
        free (env);
	
	return 0;  
}

AXIS2_DECLARE(axis2_env_t *)
axis2_environment_create (axis2_allocator_t *allocator)
{
	axis2_env_t *environment;
    if (NULL == allocator)
        return NULL;

    environment =
        (axis2_env_t *) AXIS2_MALLOC (allocator, sizeof (axis2_env_t));

    if (NULL == environment)
        return NULL;

    environment->allocator = allocator;
	
	return environment;
    
}

AXIS2_DECLARE(axis2_env_t *)
axis2_environment_create_with_error_stream (axis2_allocator_t *allocator
                          , axis2_error_t *error, axis2_stream_t *stream)
{
	return axis2_environment_create_with_error_stream_log(allocator, error
		, stream, NULL);    
}

AXIS2_DECLARE(axis2_env_t *)
axis2_environment_create_with_error_stream_log (axis2_allocator_t *allocator
                          , axis2_error_t *error, axis2_stream_t *stream
                          , axis2_log_t *log)
{
    axis2_env_t *environment;
    if (NULL == allocator)
        return NULL;

    environment =
        (axis2_env_t *) AXIS2_MALLOC (allocator,
                                              sizeof (axis2_env_t));

    if (NULL == environment)
        return NULL;

    environment->allocator = allocator;

    if (NULL == error)
        return NULL;
    environment->error = error;

	if(NULL == stream)
		return NULL;
    environment->stream = stream;    

    if (NULL == log)
        environment->log_enabled = AXIS2_FALSE;
	environment->log_enabled = AXIS2_TRUE;
    environment->log = log;

    return environment;
}


AXIS2_DECLARE(axis2_status_t)
axis2_environment_check_status (axis2_env_t *env)
{
    if(NULL == env)
    {
        AXIS2_ERROR_SET_STATUS_CODE(env->error, AXIS2_FAILURE);
		AXIS2_ERROR_SET_ERROR_NUMBER(env->error, AXIS2_ERROR_ENVIRONMENT_IS_NULL);		
    }
	return AXIS2_ERROR_GET_STATUS_CODE(env->error);
}
