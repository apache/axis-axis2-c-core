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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <axis2_file_handler.h>

void* AXIS2_CALL 
axis2_file_handler_open(const axis2_char_t *file_name, 
                            const axis2_char_t *options,
                            axis2_env_t **env)
{
    FILE *file_ptr;
    axis2_char_t *f_opt;
	axis2_char_t *f_name = (axis2_char_t*) AXIS2_STRDUP(file_name, env);
	if(!f_name) return NULL;
	    f_opt = (axis2_char_t*) AXIS2_STRDUP(options, env);
	if(!f_opt) return NULL;
	
	file_ptr = fopen (f_name, f_opt);
	return file_ptr;
}

axis2_status_t AXIS2_CALL 
axis2_file_handler_close(void *file_ptr)
{
	if(!file_ptr) return -1;
	return (axis2_status_t) fclose(file_ptr);
}

int AXIS2_CALL 
axis2_file_handler_access(axis2_char_t *path, 
                            int mode)
{
    return access(path, mode);
}
