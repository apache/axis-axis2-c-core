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

#include <axis2_dir_handler.h>
#include <axis2_file.h>
#include <platforms/axis2_platform_auto_sense.h>
#include <axis2_string.h>
#ifndef S_ISDIR
#   define S_ISDIR(m) ((m & S_IFMT) == S_IFDIR)
#endif

#include <minizip/axis2_archive_extract.h>

extern int AXIS2_ALPHASORT();

int dir_select(struct dirent *entry);


/**
 * List the dll files in the given service or module folder path
 * @param pathname path to your service or module directory
 * @return array list of dll file names
 */
AXIS2_EXTERN axis2_array_list_t * AXIS2_CALL
axis2_dir_handler_list_services_or_modules_in_dir(const axis2_env_t *env,
        axis2_char_t *pathname)
{
    axis2_array_list_t *file_list = NULL;
    struct stat *buf = NULL;
    int count = 1;
    int i = 0;
    struct dirent **files = NULL;
    int file_select();
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, NULL);
    file_list = axis2_array_list_create(env, 100);
    count = AXIS2_SCANDIR(pathname, &files, file_select, AXIS2_ALPHASORT);
    /* If no files found, make a non-selectable menu item */
    if (count <= 0)
    {
        AXIS2_ARRAY_LIST_FREE(file_list, env);
        file_list = NULL;
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "No files in the path %s.", pathname);
        return NULL;
    }

    for (i = 1; i < (count + 1) ; ++i)
    {
        axis2_char_t *fname = NULL;
        axis2_file_t *arch_file = NULL;
        axis2_char_t *path = NULL;
        axis2_char_t *temp_path = NULL;

        fname = files[i-1]->d_name;
        arch_file = (axis2_file_t *) axis2_file_create(env);
        if (!arch_file)
        {
            int size = 0;
            int j = 0;
            axis2_file_t *del_file = NULL;

            size = AXIS2_ARRAY_LIST_SIZE(file_list, env);
            for (j = 0; j < size; j++)
            {
                del_file = AXIS2_ARRAY_LIST_GET(file_list, env, j);
                AXIS2_FILE_FREE(del_file, env);
                del_file = NULL;
            }
            AXIS2_ARRAY_LIST_FREE(file_list, env);
            file_list = NULL;
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        AXIS2_FILE_SET_NAME(arch_file, env, fname);
        temp_path = AXIS2_STRACAT(pathname, AXIS2_PATH_SEP_STR, env);
        path = AXIS2_STRACAT(temp_path, fname, env);
        AXIS2_FREE(env->allocator, temp_path);
        if (!path)
        {
            int size = 0;
            int j = 0;
            axis2_file_t *del_file = NULL;

            AXIS2_FILE_FREE(arch_file, env);
            arch_file = NULL;
            size = AXIS2_ARRAY_LIST_SIZE(file_list, env);
            for (j = 0; j < size; j++)
            {
                del_file = AXIS2_ARRAY_LIST_GET(file_list, env, j);
                AXIS2_FILE_FREE(del_file, env);
                del_file = NULL;
            }
            AXIS2_ARRAY_LIST_FREE(file_list, env);
            file_list = NULL;
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        AXIS2_FILE_SET_PATH(arch_file, env, path);
        buf = AXIS2_MALLOC(env->allocator, sizeof(struct stat));
        if (!buf)
        {
            int size = 0;
            int j = 0;
            axis2_file_t *del_file = NULL;

            AXIS2_FREE(env->allocator, path);
            path = NULL;
            AXIS2_FILE_FREE(arch_file, env);
            arch_file = NULL;
            size = AXIS2_ARRAY_LIST_SIZE(file_list, env);
            for (j = 0; j < size; j++)
            {
                del_file = AXIS2_ARRAY_LIST_GET(file_list, env, j);
                AXIS2_FILE_FREE(del_file, env);
                del_file = NULL;
            }
            AXIS2_ARRAY_LIST_FREE(file_list, env);
            file_list = NULL;
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        stat(path, buf);
        AXIS2_FILE_SET_TIMESTAMP(arch_file, env, (time_t) buf->st_ctime);
        status = AXIS2_ARRAY_LIST_ADD(file_list, env, arch_file);
        if (AXIS2_SUCCESS != status)
        {
            int size = 0;
            int j = 0;
            axis2_file_t *del_file = NULL;

            AXIS2_FILE_FREE(arch_file, env);
            arch_file = NULL;
            AXIS2_FREE(env->allocator, path);
            path = NULL;
            AXIS2_FREE(env->allocator, buf);
            size = AXIS2_ARRAY_LIST_SIZE(file_list, env);
            for (j = 0; j < size; j++)
            {
                del_file = AXIS2_ARRAY_LIST_GET(file_list, env, j);
                AXIS2_FILE_FREE(del_file, env);
                del_file = NULL;
            }
            AXIS2_ARRAY_LIST_FREE(file_list, env);
            file_list = NULL;
            return NULL;
        }
        AXIS2_FREE(env->allocator, path);
        path = NULL;
        AXIS2_FREE(env->allocator, buf);
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
AXIS2_EXTERN axis2_array_list_t * AXIS2_CALL
axis2_dir_handler_list_service_or_module_dirs(const axis2_env_t *env,
        axis2_char_t *pathname)
{
    axis2_array_list_t *file_list = NULL;
    struct stat *buf = NULL;
    int count = 1;
    int i = 0;
    struct dirent **files = NULL;
    char cwd[500];

    /**FIXME:
     * This magic number 500 was selected as a temperary solution. It has to be
     * replaced with dinamic memory allocation. This will be done once the use of
     * errno after getwcd() on Windows is figured out.
     */

    axis2_status_t status = AXIS2_FAILURE;
    AXIS2_ENV_CHECK(env, NULL);
    file_list = axis2_array_list_create(env, 0);
    if (!getcwd(cwd, 500)) exit(1);
    chdir(pathname);
    axis2_archive_extract();

    count = AXIS2_SCANDIR(pathname, &files, dir_select, AXIS2_ALPHASORT);
    chdir(cwd);

    /* If no files found, make a non-selectable menu item */
    if (count <= 0)
    {
        AXIS2_ARRAY_LIST_FREE(file_list, env);
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "No files in the path %s.", pathname);
        return NULL;
    }

    for (i = 1; i < (count + 1) ; ++i)
    {
        axis2_char_t *fname = NULL;
        axis2_file_t *arch_file = NULL;
        axis2_char_t *path = NULL;
        axis2_char_t *temp_path = NULL;

        fname = files[i-1]->d_name;
        arch_file = (axis2_file_t *) axis2_file_create(env);
        if (!arch_file)
        {
            int size = 0;
            int j = 0;
            axis2_file_t *del_file = NULL;

            size = AXIS2_ARRAY_LIST_SIZE(file_list, env);
            for (j = 0; j < size; j++)
            {
                del_file = AXIS2_ARRAY_LIST_GET(file_list, env, j);
                AXIS2_FILE_FREE(del_file, env);
                del_file = NULL;
            }
            AXIS2_ARRAY_LIST_FREE(file_list, env);
            file_list = NULL;
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        AXIS2_FILE_SET_NAME(arch_file, env, fname);

        temp_path = AXIS2_STRACAT(pathname, AXIS2_PATH_SEP_STR, env);
        path = AXIS2_STRACAT(temp_path, fname, env);
        if (!path)
        {
            int size = 0;
            int j = 0;
            axis2_file_t *del_file = NULL;

            AXIS2_FILE_FREE(arch_file, env);
            arch_file = NULL;
            size = AXIS2_ARRAY_LIST_SIZE(file_list, env);
            for (j = 0; j < size; j++)
            {
                del_file = AXIS2_ARRAY_LIST_GET(file_list, env, j);
                AXIS2_FILE_FREE(del_file, env);
                del_file = NULL;
            }
            AXIS2_ARRAY_LIST_FREE(file_list, env);
            file_list = NULL;
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        AXIS2_FILE_SET_PATH(arch_file, env, path);
        /* free temp vars */
        AXIS2_FREE(env->allocator, temp_path);
        buf = AXIS2_MALLOC(env->allocator, sizeof(struct stat));
        if (!buf)
        {
            int size = 0;
            int j = 0;
            axis2_file_t *del_file = NULL;

            AXIS2_FILE_FREE(arch_file, env);
            arch_file = NULL;
            AXIS2_FREE(env->allocator, path);
            path = NULL;
            size = AXIS2_ARRAY_LIST_SIZE(file_list, env);
            for (j = 0; j < size; j++)
            {
                del_file = AXIS2_ARRAY_LIST_GET(file_list, env, j);
                AXIS2_FILE_FREE(del_file, env);
                del_file = NULL;
            }
            AXIS2_ARRAY_LIST_FREE(file_list, env);
            file_list = NULL;
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        stat(path, buf);
        AXIS2_FILE_SET_TIMESTAMP(arch_file, env, (time_t) buf->st_ctime);
        status = AXIS2_ARRAY_LIST_ADD(file_list, env, arch_file);
        if (AXIS2_SUCCESS != status)
        {
            int size = 0;
            int j = 0;
            axis2_file_t *del_file = NULL;

            AXIS2_FILE_FREE(arch_file, env);
            arch_file = NULL;
            AXIS2_FREE(env->allocator, path);
            path = NULL;
            AXIS2_FREE(env->allocator, buf);
            buf = NULL;
            size = AXIS2_ARRAY_LIST_SIZE(file_list, env);
            for (j = 0; j < size; j++)
            {
                del_file = AXIS2_ARRAY_LIST_GET(file_list, env, j);
                AXIS2_FILE_FREE(del_file, env);
                del_file = NULL;
            }
            AXIS2_ARRAY_LIST_FREE(file_list, env);
            file_list = NULL;
            return NULL;

        }
        AXIS2_FREE(env->allocator, path);
        path = NULL;
        AXIS2_FREE(env->allocator, buf);
        buf = NULL;
        arch_file = NULL;
        fname = NULL;
    }
#ifndef WIN32
    for (i = 0; i < count; i++)
    {
        AXIS2_FREE(env->allocator, files[i]);
    }
    AXIS2_FREE(env->allocator, files);
#endif
    return file_list;
}


int file_select(struct dirent *entry)

{
    axis2_char_t *ptr;

    if ((strcmp(entry->d_name, ".") == 0) ||
            (strcmp(entry->d_name, "..") == 0))
        return (AXIS2_FALSE);

    /* Check for filename extensions */
    ptr = AXIS2_RINDEX(entry->d_name, '.');
    if ((ptr) &&
            ((strcmp(ptr, AXIS2_LIB_SUFFIX) == 0)))
    {
        return (AXIS2_TRUE);
    }
    else
        return(AXIS2_FALSE);
}

int dir_select(struct dirent *entry)

{
    struct stat stat_p;

    if (-1 ==  stat(entry->d_name, &stat_p))
        return (AXIS2_FALSE);

    if ((entry->d_name[0] == '.') || (!S_ISDIR(stat_p.st_mode)))
    {
        return (AXIS2_FALSE);
    }

    return AXIS2_TRUE;
}

