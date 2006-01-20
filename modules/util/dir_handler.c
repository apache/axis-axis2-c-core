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
 
#include <axis2_dir_handler.h>
#include <axis2_file.h>

extern  int alphasort();

/**
 * List the dll files in the given service or module folder path
 * @param pathname path to your service or module directory
 * @return array list of dll file names
 */
axis2_array_list_t *AXIS2_CALL
axis2_dir_handler_list_services_or_modules_in_dir(axis2_env_t **env,
                            axis2_char_t *pathname)   
{ 
    axis2_array_list_t *file_list = NULL;
    struct stat *buf = NULL;
	int count = 1;
    int i = 0;
	struct direct **files = NULL;
	int file_select();
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_ENV_CHECK(env, NULL);
    file_list = axis2_array_list_create(env, 100);
	count = scandir(pathname, &files, file_select, alphasort);
	/* If no files found, make a non-selectable menu item */
	if (count <= 0)
	{		 
		printf("No files in this directory:%s\n", pathname);
		return NULL;
	}
    
	for (i = 1; i < (count + 1) ; ++i )
    {
        int len = 0;
        axis2_char_t *fname = NULL;
        axis2_file_t *arch_file = NULL;
        axis2_char_t *path = NULL;
       
        fname = files[i-1]->d_name;
        printf("fname:%s\n", fname);
        arch_file = (axis2_file_t *) axis2_file_create(env);
        if(!arch_file)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        AXIS2_FILE_SET_NAME(arch_file, env, fname);
        len = strlen(pathname) + strlen(fname) + 3;
        path = AXIS2_MALLOC((*env)->allocator, len);
        if(!path)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        strcpy(path, pathname);
        strcat(path, AXIS2_PATH_SEP_STR);
        strcat(path, fname);
        buf = AXIS2_MALLOC((*env)->allocator, sizeof(struct stat));
        if(!buf)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        stat(path, buf);
        AXIS2_FILE_SET_TIMESTAMP(arch_file, env, (time_t) buf->st_ctime);
        status = AXIS2_ARRAY_LIST_ADD(file_list, env, arch_file);
        if(path)
        {
            AXIS2_FREE((*env)->allocator, path);
            path = NULL;
        }
        buf = NULL;
        arch_file = NULL;
        fname = NULL;
    }
    
    return file_list;
}

/**
 * List services or modules directories in the services or modules folder
 * respectively
 * @param pathname path  your modules or services folder
 * @return array list of contents of services or modules folder
 */
axis2_array_list_t *AXIS2_CALL
axis2_dir_handler_list_service_or_module_dirs(axis2_env_t **env,
                            axis2_char_t *pathname)   
{ 
    axis2_array_list_t *file_list = NULL;
    struct stat *buf = NULL;
	int count = 1;
    int i = 0;
	struct direct **files = NULL;
	int dir_select();
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_ENV_CHECK(env, NULL);
    file_list = axis2_array_list_create(env, 100);
	count = scandir(pathname, &files, dir_select, alphasort);
	/* If no files found, make a non-selectable menu item */
	if (count <= 0)
	{		 
		printf("No files in this directory:%s\n", pathname);
		return NULL;
	}
    
	for (i = 1; i < (count + 1) ; ++i )
    {
        int len = 0;
        axis2_char_t *fname = NULL;
        axis2_file_t *arch_file = NULL;
        axis2_char_t *path = NULL;
       
        fname = files[i-1]->d_name;
        printf("fname:%s\n", fname);
        arch_file = (axis2_file_t *) axis2_file_create(env);
        if(!arch_file)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        AXIS2_FILE_SET_NAME(arch_file, env, fname);
        len = strlen(pathname) + strlen(fname) + 3;
        path = AXIS2_MALLOC((*env)->allocator, len);
        if(!path)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        strcpy(path, pathname);
        strcat(path, AXIS2_PATH_SEP_STR);
        strcat(path, fname);
        buf = AXIS2_MALLOC((*env)->allocator, sizeof(struct stat));
        if(!buf)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        stat(path, buf);
        AXIS2_FILE_SET_TIMESTAMP(arch_file, env, (time_t) buf->st_ctime);
        status = AXIS2_ARRAY_LIST_ADD(file_list, env, arch_file);
        if(path)
        {
            AXIS2_FREE((*env)->allocator, path);
            path = NULL;
        }
        buf = NULL;
        arch_file = NULL;
        fname = NULL;
    }
    
    return file_list;
}


int file_select(struct direct *entry)
 
{
	axis2_char_t *ptr;
	axis2_char_t *rindex(const axis2_char_t *s, int c);
 
	if ((strcmp(entry->d_name, ".")== 0) ||
			(strcmp(entry->d_name, "..") == 0))
		return (AXIS2_FALSE);
 
	/* Check for filename extensions */
	ptr = rindex(entry->d_name, '.');
	if ((ptr != NULL) &&
		((strcmp(ptr, AXIS2_LIB_SUFFIX) == 0) ))
    {
		return (AXIS2_TRUE);
    }
	else
		return(AXIS2_FALSE);
}

int dir_select(struct direct *entry)
 
{
	axis2_char_t *ptr;
	axis2_char_t *rindex(const axis2_char_t *s, int c);
 
	if ((strcmp(entry->d_name, ".")== 0) ||
			(strcmp(entry->d_name, "..") == 0))
    {
		return (AXIS2_FALSE);
    }
 
	/* Check for filename extensions */
	ptr = rindex(entry->d_name, '.');
	if (ptr != NULL)
    {
		return (AXIS2_FALSE);
    }
	return AXIS2_TRUE;
}
