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

#include <axis2_repos_listener.h>
#include <axis2_ws_info_list.h>
#include <axutil_dir_handler.h>

struct axis2_repos_listener
{

    /**
     * Referance to a WSInfoList
     */
    axis2_ws_info_list_t *info_list;

    /**
     * The parent directory of the modules and services directories
     * that the listentner should listent
     */
    axis2_char_t *folder_name;
    axis2_conf_t *conf;
};

static axis2_status_t axis2_repos_listener_search(
    axis2_repos_listener_t * repos_listener,
    const axutil_env_t * env,
    axis2_char_t * folder_name,
    int type);


AXIS2_EXTERN axis2_repos_listener_t *AXIS2_CALL
axis2_repos_listener_create(
    const axutil_env_t * env)
{
    axis2_repos_listener_t *repos_listener = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    repos_listener = (axis2_repos_listener_t *)
        AXIS2_MALLOC(env->allocator,
                     sizeof (axis2_repos_listener_t));

    if (!repos_listener)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    repos_listener->folder_name = NULL;
    repos_listener->info_list = NULL;
    repos_listener->folder_name = AXIS2_FALSE;

    return repos_listener;
}

/**
 * This constructor take two argumnets folder name and referance to
 * Deployment Engine Fisrt it initilize the syetm , by loading all the
 * modules in the /modules directory and also create a WSInfoList to
 * keep infor about available modules and services
 * @param folderName path to parent directory that the repos_listener
 * should listens
 * @param deploy_engine refearnce to engine registry  inorder to
 * inform the updates
 */

AXIS2_EXTERN axis2_repos_listener_t *AXIS2_CALL
axis2_repos_listener_create_with_folder_name_and_dep_engine(
    const axutil_env_t * env,
    axis2_char_t * folder_name,
    axis2_dep_engine_t * dep_engine)
{
    axis2_repos_listener_t *repos_listener = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    axis2_conf_t *conf;
    axis2_bool_t file_flag;

    AXIS2_ENV_CHECK(env, NULL);
    repos_listener =
        (axis2_repos_listener_t *) axis2_repos_listener_create(env);

    if (!repos_listener)
    {
        return NULL;
    }

    file_flag = axis2_dep_engine_get_file_flag (dep_engine, env);

    if (file_flag == AXIS2_FALSE)
    {
        repos_listener->folder_name = axutil_strdup(env, folder_name);
        if (!repos_listener->folder_name)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
    }

    repos_listener->info_list =
        axis2_ws_info_list_create_with_dep_engine(env, dep_engine);
    if (!repos_listener->info_list)
    {
        return NULL;
    }
    
    /* dep_engine's conf will set to repo_listner then we could get
     * details fo the axis2.xml through axis2 conf. 
     */

    conf = axis2_dep_engine_get_axis_conf (dep_engine, env);
    if (!conf)
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                         "Deployment engines axis2 configuration not available");
        return NULL;
    }
   
    status = axis2_repos_listener_set_conf (repos_listener, env, conf);

    if (status != AXIS2_SUCCESS)
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "setting conf to repos listener failed.");
        return AXIS2_FAILURE;
    }


    status = axis2_repos_listener_init(repos_listener, env);
    if (AXIS2_SUCCESS != status)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_REPOS_LISTENER_INIT_FAILED,
                        AXIS2_FAILURE);
        return NULL;
    }

    return repos_listener;
}


AXIS2_EXTERN void AXIS2_CALL
axis2_repos_listener_free(
    axis2_repos_listener_t * repos_listener,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, void);
    if (!repos_listener)
        return;

    if (repos_listener->folder_name)
    {
        AXIS2_FREE(env->allocator, repos_listener->folder_name);
    }

    if (repos_listener->info_list)
    {
        axis2_ws_info_list_free(repos_listener->info_list, env);
    }

    AXIS2_FREE(env->allocator, repos_listener);

    return;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_repos_listener_check_modules(
    axis2_repos_listener_t * repos_listener,
    const axutil_env_t * env)
{
    axis2_char_t *module_path = NULL;
    axis2_char_t *temp_path = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_bool_t axis2_flag = AXIS2_FALSE;
    axis2_conf_t *conf;

    axutil_param_t *module_param;
    axis2_char_t *module_dir;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, repos_listener, AXIS2_FAILURE);

    conf = axis2_repos_listener_get_conf (repos_listener, env);

    /* conf is needed only to decide we are using axis2.xml. Other
     * case we don't need. Hence else part of following if-else
     * condition not used.
     */

    if (conf)
    {
        axis2_flag = axis2_conf_get_axis2_flag (conf, env);
    }

    if (axis2_flag == AXIS2_FALSE)
    {

        temp_path =
            axutil_stracat(env, repos_listener->folder_name, AXIS2_PATH_SEP_STR);
        module_path = axutil_stracat(env, temp_path, AXIS2_MODULE_PATH);
        AXIS2_FREE(env->allocator, temp_path);
    }
    else
    {
        module_param = axis2_conf_get_param (conf, env, "moduleDir");
        if (module_param)
        {
            module_dir = (axis2_char_t *)axutil_param_get_value (module_param, env);
            module_path = axutil_strdup (env, module_dir);
        }
        else
        {
            AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                             "creating axis2 conf using axis2.xml, modulesDir \
parameter not available."); 
        }

    }
    status =
        axis2_repos_listener_search(repos_listener, env, module_path,
                                    AXIS2_MODULE);
    AXIS2_FREE(env->allocator, module_path);
    return status;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_repos_listener_check_svcs(
    axis2_repos_listener_t * repos_listener,
    const axutil_env_t * env)
{
    axis2_char_t *svc_path = NULL;
    axis2_char_t *temp_path = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    axis2_conf_t *conf;
    axis2_bool_t axis2_flag = AXIS2_FALSE;

    axutil_param_t *svc_param;
    axis2_char_t *svc_dir;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, repos_listener, AXIS2_FAILURE);

    conf = axis2_repos_listener_get_conf (repos_listener, env);

    if (conf)
        axis2_flag = axis2_conf_get_axis2_flag (conf, env);

    if (axis2_flag == AXIS2_FALSE)
    {
        temp_path =
            axutil_stracat(env, repos_listener->folder_name, 
                           AXIS2_PATH_SEP_STR);
        svc_path = axutil_stracat(env, temp_path, AXIS2_SVC_PATH);
        AXIS2_FREE(env->allocator, temp_path);
    }
    else
    {
        svc_param = axis2_conf_get_param (conf, env, "servicesDir");
        if (svc_param)
        {
            svc_dir = axutil_param_get_value (svc_param, env);
            svc_path = axutil_strdup (env, svc_dir);
        }
        else
        {
            AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI,
                             "repos listener create using axis2.xml, \
\"serviceDir\" param not available");
        }
    }

    status =
        axis2_repos_listener_search(repos_listener, 
                                    env, svc_path, AXIS2_SVC);
    AXIS2_FREE(env->allocator, svc_path);
    return status;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_repos_listener_update(
    axis2_repos_listener_t * repos_listener,
    const axutil_env_t * env)
{
    AXIS2_PARAM_CHECK (env->error, repos_listener, AXIS2_FAILURE);
    return axis2_ws_info_list_update(repos_listener->info_list, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_repos_listener_init(
    axis2_repos_listener_t * repos_listener,
    const axutil_env_t * env)
{
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, repos_listener, AXIS2_FAILURE);

    status = axis2_ws_info_list_init(repos_listener->info_list, env);
    if (AXIS2_SUCCESS != status)
    {
        return status;
    }
    /* if check_modules return AXIS2_FAILURE that means
     * there are no modules to load
     */
    axis2_repos_listener_check_modules(repos_listener, env);

    /* if check_svcs return AXIS2_FAILURE that means
     * there are no services to load
     */
    axis2_repos_listener_check_svcs(repos_listener, env);
    return axis2_repos_listener_update(repos_listener, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_repos_listener_start_listen(
    axis2_repos_listener_t * repos_listener,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, repos_listener, AXIS2_FAILURE);

    axis2_ws_info_list_init(repos_listener->info_list, env);
    /* axis2_repos_listener_check_modules(repos_listener, env); */
    axis2_repos_listener_check_svcs(repos_listener, env);
    axis2_repos_listener_update(repos_listener, env);
    return AXIS2_SUCCESS;
}

static axis2_status_t
axis2_repos_listener_search(
    axis2_repos_listener_t * repos_listener,
    const axutil_env_t * env,
    axis2_char_t * folder_name,
    int type)
{
    int size = 0;
    int i = 0;
    axutil_array_list_t *current_info_list = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, repos_listener, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, folder_name, AXIS2_FAILURE);
    current_info_list = axutil_dir_handler_list_service_or_module_dirs(env,
                                                                       folder_name);


    if (!current_info_list)
    {
        axis2_status_t status_code = AXIS2_FAILURE;

        status_code = AXIS2_ERROR_GET_STATUS_CODE(env->error);
        if (AXIS2_SUCCESS != status)
        {
            return status;
        }
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "No %s in the folder.",
                        folder_name);
        return AXIS2_SUCCESS;
    }

    size = axutil_array_list_size(current_info_list, env);

    for (i = 0; i < size; i++)  /* loop until empty */
    {
        axutil_file_t *file = NULL;
        file = axutil_array_list_get(current_info_list, env, i);
        status =
            axis2_ws_info_list_add_ws_info_item(repos_listener->info_list, env,
                                                file, type);
        if (AXIS2_SUCCESS != status)
        {
            int size_j = 0;
            int j = 0;

            size_j = axutil_array_list_size(current_info_list, env);
            for (j = 0; j < size_j; j++)
            {
                axutil_file_t *del_file = NULL;

                del_file = axutil_array_list_get(current_info_list, env, j);
                axutil_file_free(del_file, env);
            }
            axutil_array_list_free(current_info_list, env);
            current_info_list = NULL;
            return status;
        }
    }

    for (i = 0; i < size; i++)
    {
        axutil_file_t *del_file = NULL;

        del_file = axutil_array_list_get(current_info_list, env, i);
        axutil_file_free(del_file, env);
    }
    axutil_array_list_free(current_info_list, env);
    current_info_list = NULL;
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_repos_listener_set_conf(
    axis2_repos_listener_t *repo_listener,        
    const axutil_env_t * env,
    axis2_conf_t *conf)
{
    AXIS2_PARAM_CHECK (env->error, repo_listener, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK (env->error, conf, AXIS2_FAILURE);
    repo_listener->conf = conf;
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_conf_t *AXIS2_CALL
axis2_repos_listener_get_conf (
    axis2_repos_listener_t *repo_listener,        
    const axutil_env_t * env)
{
    AXIS2_PARAM_CHECK (env->error, repo_listener, AXIS2_FAILURE);
    return repo_listener->conf;
}
