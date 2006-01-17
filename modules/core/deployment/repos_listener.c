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
 
#include <axis2_repos_listener.h>
#include <axis2_ws_info_list.h>
#include <axis2_dir_handler.h>

/** 
 * @brief DLL Description struct impl
 * Axis2 DLL Description impl  
 */ 
typedef struct axis2_repos_listener_impl
{
	axis2_repos_listener_t repos_listener;
    /**
     * Referance to a WSInfoList
     */
    axis2_ws_info_list_t *info_list;

    int BUFFER;

    /**
     * The parent directory of the modules and services directories
     * that the listentner should listent
     */
    axis2_char_t *folder_name;

 
 
} axis2_repos_listener_impl_t;

#define AXIS2_INTF_TO_IMPL(repos_listener) \
        ((axis2_repos_listener_impl_t *) repos_listener)
	
/*************************** Function headers *********************************/

axis2_status_t AXIS2_CALL
axis2_repos_listener_free (axis2_repos_listener_t *repos_listener,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_repos_listener_check_modules(axis2_repos_listener_t *listener,
                                    axis2_env_t **env);


axis2_status_t AXIS2_CALL
axis2_repos_listener_check_svcs(axis2_repos_listener_t *listener,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_repos_listener_update(axis2_repos_listener_t *listener,
                                    axis2_env_t **env);


axis2_status_t AXIS2_CALL
axis2_repos_listener_init(axis2_repos_listener_t *listener,
                                    axis2_env_t **env);


axis2_status_t AXIS2_CALL
axis2_repos_listener_start_listen(axis2_repos_listener_t *listener,
                                    axis2_env_t **env);

/**
 * This method is to search a given folder  for jar files
 * and added them to a list wich is in the WSInfolist class
 */
axis2_status_t
axis2_repos_listener_search_WS(axis2_repos_listener_t *listener,
                                axis2_env_t **env,
                                axis2_char_t *folder_name,
                                int type);

/************************* End of function headers ****************************/	

axis2_repos_listener_t *AXIS2_CALL
axis2_repos_listener_create(axis2_env_t **env)
{
    axis2_repos_listener_impl_t *listener_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    listener_impl = (axis2_repos_listener_impl_t *) AXIS2_MALLOC ((*env)->allocator
		    , sizeof (axis2_repos_listener_impl_t));
    
	if(NULL == listener_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    } 
    listener_impl->folder_name = NULL;
    listener_impl->info_list = NULL;
    listener_impl->BUFFER = 2048;
    listener_impl->repos_listener.ops = NULL;
    
    listener_impl->repos_listener.ops = (axis2_repos_listener_ops_t *) 
        AXIS2_MALLOC((*env)->allocator, sizeof(axis2_repos_listener_ops_t));
    
	if(NULL == listener_impl->repos_listener.ops)
	{
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    listener_impl->repos_listener.ops->free = axis2_repos_listener_free;
    listener_impl->repos_listener.ops->check_modules = 
            axis2_repos_listener_check_modules;
    listener_impl->repos_listener.ops->check_svcs = 
            axis2_repos_listener_check_svcs;
    listener_impl->repos_listener.ops->update = axis2_repos_listener_update;
    listener_impl->repos_listener.ops->init = axis2_repos_listener_init;
    listener_impl->repos_listener.ops->start_listen = 
            axis2_repos_listener_start_listen;
    
    
    return &(listener_impl->repos_listener);
}


/**
 * This constructor take two argumnets folder name and referance to Deployment Engine
 * Fisrt it initilize the syetm , by loading all the modules in the /modules directory
 * and also create a WSInfoList to keep infor about available modules and services
 *
 * @param folderName    path to parent directory that the listener should listent
 * @param deploy_engine refearnce to engine registry  inorder to inform the updates
 */

axis2_repos_listener_t *AXIS2_CALL
axis2_repos_listener_create_with_folder_name_and_dep_engine(axis2_env_t **env,
                                                axis2_char_t *folder_name,
                                                axis2_dep_engine_t *dep_engine)
{
    axis2_repos_listener_impl_t *listener_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    listener_impl = (axis2_repos_listener_impl_t *) axis2_repos_listener_create(env);
    
	if(NULL == listener_impl)
    {
        return NULL;
    } 
    listener_impl->folder_name = AXIS2_STRDUP(folder_name, env);
    if(!listener_impl->folder_name)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    listener_impl->info_list = axis2_ws_info_list_create_with_dep_engine(
        env, dep_engine);
    if(!listener_impl->info_list)
    {
        return NULL;
    }
    status = axis2_repos_listener_init(&(listener_impl->repos_listener), env);
    printf("respos listner init status:%d\n", status);
    return &(listener_impl->repos_listener);
}

/******************************************************************************/

axis2_status_t AXIS2_CALL
axis2_repos_listener_free (axis2_repos_listener_t *repos_listener,
                        axis2_env_t **env)
{
    axis2_repos_listener_impl_t *listener_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(repos_listener, env, AXIS2_FAILURE);
    
    listener_impl = AXIS2_INTF_TO_IMPL(repos_listener);
   
    if(listener_impl->folder_name)
    {
        AXIS2_FREE((*env)->allocator, listener_impl->folder_name);
        listener_impl->folder_name = NULL;
    }
    
	if(NULL != repos_listener->ops)
    {
		AXIS2_FREE((*env)->allocator, repos_listener->ops);
        repos_listener->ops = NULL;
    }
    
    
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_repos_listener_check_modules(axis2_repos_listener_t *listener,
                                    axis2_env_t **env) 
{
    axis2_repos_listener_impl_t *listener_impl = NULL;
    axis2_char_t *module_path = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(listener, env, AXIS2_FAILURE);
    listener_impl = AXIS2_INTF_TO_IMPL(listener);
    
    module_path = AXIS2_STRACAT(listener_impl->folder_name, AXIS2_MODULE_PATH,
        env);
    return axis2_repos_listener_search_WS(listener, env, module_path, AXIS2_MODULE);
}

axis2_status_t AXIS2_CALL
axis2_repos_listener_check_svcs(axis2_repos_listener_t *listener,
                                    axis2_env_t **env) 
{
    axis2_repos_listener_impl_t *listener_impl = NULL;
    axis2_char_t *svc_path = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(listener, env, AXIS2_FAILURE);
    listener_impl = AXIS2_INTF_TO_IMPL(listener);
    svc_path = AXIS2_STRACAT(listener_impl->folder_name, AXIS2_SVC_PATH, env);
    return axis2_repos_listener_search_WS(listener, env, svc_path, AXIS2_SVC);
}

axis2_status_t AXIS2_CALL
axis2_repos_listener_update(axis2_repos_listener_t *listener,
                                    axis2_env_t **env) 
{
    axis2_repos_listener_impl_t *listener_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(listener, env, AXIS2_FAILURE);
    listener_impl = AXIS2_INTF_TO_IMPL(listener);
    /* TODO completet this */
    /* this call the update method of WSInfoList */
    return AXIS2_WS_INFO_LIST_UPDATE(listener_impl->info_list, env);
}

axis2_status_t AXIS2_CALL
axis2_repos_listener_init(axis2_repos_listener_t *listener,
                                    axis2_env_t **env) 
{
    axis2_repos_listener_impl_t *listener_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_FUNC_PARAM_CHECK(listener, env, AXIS2_FAILURE);
    listener_impl = AXIS2_INTF_TO_IMPL(listener);
    
    status = AXIS2_WS_INFO_LIST_INIT(listener_impl->info_list, env);
    if(AXIS2_SUCCESS != status)
    {
        return AXIS2_FAILURE;
    }
    /* if check_modules return AXIS2_FAILURE that means
     * there are no modules to load
     */
    axis2_repos_listener_check_modules(listener, env);
    
    /* if check_svcs return AXIS2_FAILURE that means
     * there are no services to load
     */
    axis2_repos_listener_check_svcs(listener, env);
    
    return axis2_repos_listener_update(listener, env);
}

axis2_status_t AXIS2_CALL
axis2_repos_listener_start_listen(axis2_repos_listener_t *listener,
                                    axis2_env_t **env) 
{
    axis2_repos_listener_impl_t *listener_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(listener, env, AXIS2_FAILURE);
    listener_impl = AXIS2_INTF_TO_IMPL(listener);
    
    AXIS2_WS_INFO_LIST_INIT(listener_impl->info_list, env);
    /* axis2_repos_listener_check_modules(listener, env); */
    axis2_repos_listener_check_svcs(listener, env);
    axis2_repos_listener_update(listener, env);
    return AXIS2_SUCCESS;
}

axis2_status_t
axis2_repos_listener_search_WS(axis2_repos_listener_t *listener,
                                axis2_env_t **env,
                                axis2_char_t *folder_name,
                                int type) 
{
    axis2_repos_listener_impl_t *listener_impl = NULL;
    int size = 0;
    int i = 0;
    axis2_array_list_t *current_info_list = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(listener, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, folder_name, AXIS2_FAILURE);
    listener_impl = AXIS2_INTF_TO_IMPL(listener);
    current_info_list = AXIS2_DIR_HANDLER_LIST_DIR(env, folder_name);
    if(!current_info_list)
    {
        return AXIS2_FAILURE;
    }
    size = AXIS2_ARRAY_LIST_SIZE(current_info_list, env);
    for (i = 0; i< size; i++) /* loop until empty */
    {    
        axis2_file_t *file = NULL;
        file = AXIS2_ARRAY_LIST_GET(current_info_list, env, i);
        AXIS2_WS_INFO_LIST_ADD_WS_INFO_ITEM(listener_impl->info_list, env,
            file, type);
        axis2_file_free(file, env);
        
    }
    return AXIS2_SUCCESS;
}
