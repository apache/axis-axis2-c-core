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

/************************** End of function prototypes ************************/

axis2_file_t * AXIS2_CALL 
axis2_file_create(axis2_env_t **env)
{
    axis2_file_t *file = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	file = (axis2_file_t *) AXIS2_MALLOC((*env)->allocator, sizeof(axis2_file_t));
	
	if(NULL == file)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    file->name = NULL;
    file->time_stamp = 0;
	
	return file;
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_file_free (axis2_file_t *file, 
                    axis2_env_t **env)
{
    /*if(file->name)
    {
        AXIS2_FREE((*env)->allocator, file->name);
        file->name = NULL;
    }*/
    
    if(file)
    {
        AXIS2_FREE((*env)->allocator, file);
        file = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_file_t *AXIS2_CALL
axis2_file_clone(axis2_file_t *file,
                  axis2_env_t **env)
{
    axis2_file_t *new_file = NULL;
    AXIS2_FUNC_PARAM_CHECK(file, env, NULL);
    new_file = axis2_file_create(env);
    new_file->name = AXIS2_STRDUP(file->name, env);
    if(!new_file->name)
    {
        axis2_file_free(new_file, env);
        new_file = NULL;
    }
    new_file->time_stamp = file->time_stamp;
    return new_file;
}
