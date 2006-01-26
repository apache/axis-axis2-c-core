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
 
#include <axis2_file.h>
#include <axis2_string.h>

typedef struct axis2_file_impl
{
    axis2_file_t file;
    axis2_char_t *name;
    axis2_char_t *path;
    AXIS2_TIME_T timestamp;
    
    
}axis2_file_impl_t;

#define AXIS2_INTF_TO_IMPL(file) \
    ((axis2_file_impl_t *) file)



axis2_status_t AXIS2_CALL
axis2_file_free (axis2_file_t *file, 
                    axis2_env_t **env); 

axis2_status_t AXIS2_CALL
axis2_file_set_name(axis2_file_t *file,
                        axis2_env_t **env,
                        axis2_char_t *name);

axis2_char_t *AXIS2_CALL
axis2_file_get_name(axis2_file_t *file,
                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_file_set_path(axis2_file_t *file,
                        axis2_env_t **env,
                        axis2_char_t *path);

axis2_char_t *AXIS2_CALL
axis2_file_get_path(axis2_file_t *file,
                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_file_set_timestamp(axis2_file_t *file,
                        axis2_env_t **env,
                        AXIS2_TIME_T timestamp);

AXIS2_TIME_T AXIS2_CALL
axis2_file_get_timestamp(axis2_file_t *file,
                            axis2_env_t **env);

axis2_file_t *AXIS2_CALL
axis2_file_clone(axis2_file_t *file,
                  axis2_env_t **env);

/************************** End of function prototypes ************************/

axis2_file_t *AXIS2_CALL 
axis2_file_create(axis2_env_t **env)
{
    axis2_file_impl_t *file_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	file_impl = (axis2_file_impl_t *) AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_file_impl_t));
	
	if(NULL == file_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    file_impl->name = NULL;
    file_impl->path = NULL;
    file_impl->timestamp = 0;
    
    file_impl->file.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_file_ops_t));
	if(NULL == file_impl->file.ops)
    {
        axis2_file_free(&(file_impl->file), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    file_impl->file.ops->free =  axis2_file_free;
    file_impl->file.ops->set_name = axis2_file_set_name;
    file_impl->file.ops->get_name = axis2_file_get_name;
    file_impl->file.ops->set_path = axis2_file_set_path;
    file_impl->file.ops->get_path = axis2_file_get_path;
    file_impl->file.ops->set_timestamp = axis2_file_set_timestamp;
    file_impl->file.ops->get_timestamp = axis2_file_get_timestamp;
    file_impl->file.ops->clone = axis2_file_clone;
	
	return &(file_impl->file);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_file_free (axis2_file_t *file, 
                    axis2_env_t **env)
{
    axis2_file_impl_t *file_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    file_impl = AXIS2_INTF_TO_IMPL(file);
    
    if(file_impl->name)
    {
        AXIS2_FREE((*env)->allocator, file_impl->name);
        file_impl->name = NULL;
    }
    
    if(file_impl->path)
    {
        AXIS2_FREE((*env)->allocator, file_impl->path);
        file_impl->path = NULL;
    }
    
    if(file_impl)
    {
        AXIS2_FREE((*env)->allocator, file_impl);
        file_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_file_set_name(axis2_file_t *file,
                    axis2_env_t **env,
                    axis2_char_t *name)
{
    axis2_file_impl_t *file_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, name, AXIS2_FAILURE);
    file_impl = AXIS2_INTF_TO_IMPL(file);
    
    if(file_impl->name)
    {
        AXIS2_FREE((*env)->allocator, file_impl->name);
        file_impl->name = NULL;
    }
    file_impl->name = AXIS2_STRDUP(name, env);
    if(!file_impl->name)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_file_get_name(axis2_file_t *file,
                    axis2_env_t **env)
{
    axis2_file_impl_t *file_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(file, env, NULL);
    file_impl = AXIS2_INTF_TO_IMPL(file);
    
    if(!file_impl->name)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_FILE_NAME_NOT_SET, 
            AXIS2_FAILURE);   
        return NULL;
    }
    
    return (file_impl->name);
}

axis2_status_t AXIS2_CALL
axis2_file_set_path(axis2_file_t *file,
                    axis2_env_t **env,
                    axis2_char_t *path)
{
    axis2_file_impl_t *file_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, path, AXIS2_FAILURE);
    file_impl = AXIS2_INTF_TO_IMPL(file);
    
    if(file_impl->path)
    {
        AXIS2_FREE((*env)->allocator, file_impl->path);
        file_impl->path = NULL;
    }
    file_impl->path = AXIS2_STRDUP(path, env);
    if(!(file_impl->path))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_file_get_path(axis2_file_t *file,
                    axis2_env_t **env)
{
    axis2_file_impl_t *file_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(file, env, NULL);
    file_impl = AXIS2_INTF_TO_IMPL(file);
    
    if(!(file_impl->path))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_FILE_NAME_NOT_SET, 
            AXIS2_FAILURE);   
        return NULL;
    }
    
    return (file_impl->path);
}

axis2_status_t AXIS2_CALL
axis2_file_set_timestamp(axis2_file_t *file,
                    axis2_env_t **env,
                    AXIS2_TIME_T timestamp)
{
    axis2_file_impl_t *file_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    file_impl = AXIS2_INTF_TO_IMPL(file);
    
    file_impl->timestamp = timestamp;
    
    return AXIS2_SUCCESS;
}

AXIS2_TIME_T AXIS2_CALL
axis2_file_get_timestamp(axis2_file_t *file,
                    axis2_env_t **env)
{
    axis2_file_impl_t *file_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    file_impl = AXIS2_INTF_TO_IMPL(file);
    return file_impl->timestamp;
}

axis2_file_t *AXIS2_CALL
axis2_file_clone(axis2_file_t *file,
                  axis2_env_t **env)
{
    axis2_file_impl_t *file_impl = NULL;
    axis2_file_t *new_file = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_ENV_CHECK(env, NULL);
    file_impl = AXIS2_INTF_TO_IMPL(file);
    
    new_file = axis2_file_create(env);
    status = AXIS2_FILE_SET_NAME(new_file, env, file_impl->name);
    if(AXIS2_SUCCESS != status)
    {
        return NULL;
    }
    status = AXIS2_FILE_SET_PATH(new_file, env, file_impl->path);
    if(AXIS2_SUCCESS != status)
    {
        return NULL;
    }
    AXIS2_FILE_SET_TIMESTAMP(new_file, env, file_impl->timestamp);
    return new_file;
}
