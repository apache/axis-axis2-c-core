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
 
#include <axis2_ws_info_list.h>
#include <string.h>
#include <axis2_class_loader.h>
/* #include <axis2_dep_engine.h> */

/** 
 * @brief
 * To do the common tasks for all Builder
 */ 
typedef struct axis2_ws_info_list_impl
{
	axis2_ws_info_list_t ws_info_list;
    /**
     * This is to store all the jar files in a specified folder (WEB_INF)
     */
    axis2_array_list_t *info_list;
    
    /**
     * All the curently updated jars
     */
    axis2_array_list_t *current_info_lists;
    
    /**
     * Referance to DeploymentEngine to make update
     */
    struct axis2_dep_engine *deployer;
    	
} axis2_ws_info_list_impl_t;

#define AXIS2_INTF_TO_IMPL(ws_info_list) \
    ((axis2_ws_info_list_impl_t *) ws_info_list)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
axis2_ws_info_list_free (axis2_ws_info_list_t *ws_info_list, 
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_ws_info_list_init(axis2_ws_info_list_t *info_list,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_ws_info_list_add_ws_info_item(axis2_ws_info_list_t *info_list,
                                    axis2_env_t **env,
                                    axis2_file_t *file, 
                                    int type);

axis2_ws_info_t *AXIS2_CALL
axis2_ws_info_list_get_file_item(axis2_ws_info_list_t *info_list,
                                    axis2_env_t **env,
                                    axis2_char_t *file_name);

axis2_bool_t AXIS2_CALL
axis2_ws_info_list_is_modified(axis2_ws_info_list_t *info_list,
                                axis2_env_t **env,
                                axis2_file_t *file, 
                                axis2_ws_info_t *ws_info);

axis2_bool_t AXIS2_CALL
axis2_ws_info_list_is_file_exist(axis2_ws_info_list_t *info_list,
                                    axis2_env_t **env,
                                    axis2_char_t *file_name);

axis2_status_t AXIS2_CALL
axis2_ws_info_list_check_for_undeploy(axis2_ws_info_list_t *info_list,
                                        axis2_env_t **env);


axis2_status_t AXIS2_CALL
axis2_ws_info_list_update(axis2_ws_info_list_t *info_list,
                            axis2_env_t **env);


                                
/************************** End of function prototypes ************************/

axis2_ws_info_list_t * AXIS2_CALL 
axis2_ws_info_list_create_with_dep_engine (axis2_env_t **env,
                                            struct axis2_dep_engine *dep_engine)
{
    axis2_ws_info_list_impl_t *ws_info_list_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	ws_info_list_impl = (axis2_ws_info_list_impl_t *) AXIS2_MALLOC((*env)->
        allocator, sizeof(axis2_ws_info_list_impl_t));
	
	
	if(NULL == ws_info_list_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    ws_info_list_impl->ws_info_list.ops = NULL;
    ws_info_list_impl->deployer = NULL;
    ws_info_list_impl->info_list = NULL;
    ws_info_list_impl->current_info_lists = NULL;
    
    ws_info_list_impl->deployer = dep_engine;

    ws_info_list_impl->info_list = axis2_array_list_create(env, 10);
    if (!(ws_info_list_impl->info_list))
    {
        axis2_ws_info_list_free(&(ws_info_list_impl->ws_info_list), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    ws_info_list_impl->current_info_lists = axis2_array_list_create(env, 10);
    if (!(ws_info_list_impl->current_info_lists))
    {
        axis2_ws_info_list_free(&(ws_info_list_impl->ws_info_list), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	ws_info_list_impl->ws_info_list.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_ws_info_list_ops_t));
	if(NULL == ws_info_list_impl->ws_info_list.ops)
    {
        axis2_ws_info_list_free(&(ws_info_list_impl->ws_info_list), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	ws_info_list_impl->ws_info_list.ops->free = axis2_ws_info_list_free;
    ws_info_list_impl->ws_info_list.ops->init = axis2_ws_info_list_init;
    ws_info_list_impl->ws_info_list.ops->add_ws_info_item = 
        axis2_ws_info_list_add_ws_info_item;
    ws_info_list_impl->ws_info_list.ops->get_file_item = 
        axis2_ws_info_list_get_file_item;
    ws_info_list_impl->ws_info_list.ops->is_modified = 
        axis2_ws_info_list_is_modified;
    ws_info_list_impl->ws_info_list.ops->is_file_exist = 
        axis2_ws_info_list_is_file_exist;
    ws_info_list_impl->ws_info_list.ops->check_for_undeploy = 
        axis2_ws_info_list_check_for_undeploy;
    ws_info_list_impl->ws_info_list.ops->update = axis2_ws_info_list_update;
	
	return &(ws_info_list_impl->ws_info_list);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_ws_info_list_free (axis2_ws_info_list_t *ws_info_list, 
                            axis2_env_t **env)
{
    axis2_ws_info_list_impl_t *info_list_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(ws_info_list, env, AXIS2_FAILURE);
    
    info_list_impl = AXIS2_INTF_TO_IMPL(ws_info_list);
    
    if(info_list_impl->current_info_lists)
    {
        AXIS2_ARRAY_LIST_FREE(info_list_impl->current_info_lists, env);
        info_list_impl->current_info_lists = NULL;
    }
    if(info_list_impl->info_list)
    {
        AXIS2_ARRAY_LIST_FREE(info_list_impl->info_list, env);
        info_list_impl->info_list = NULL;
    }
	if(NULL != ws_info_list->ops)
        AXIS2_FREE((*env)->allocator, ws_info_list->ops);
    
    if(info_list_impl)
    {
        AXIS2_FREE((*env)->allocator, info_list_impl);
        info_list_impl = NULL;
    }
    
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_ws_info_list_init(axis2_ws_info_list_t *info_list,
                            axis2_env_t **env) 
{
    axis2_ws_info_list_impl_t *info_list_impl = NULL;
    int size = 0;
    int i = 0;
    
    AXIS2_FUNC_PARAM_CHECK(info_list, env, AXIS2_FAILURE);
    info_list_impl = AXIS2_INTF_TO_IMPL(info_list);
    
    size = AXIS2_ARRAY_LIST_SIZE(info_list_impl->info_list, env);
    if(AXIS2_TRUE != AXIS2_ERROR_GET_STATUS_CODE((*env)->error))
    {
        return AXIS2_FAILURE;
    }
    
    for (i = 0; i < size; i++) 
    {
        axis2_ws_info_t *ws_info = NULL;
            
        ws_info = (axis2_ws_info_t *)
            AXIS2_ARRAY_LIST_GET(info_list_impl->info_list, env, i);
        AXIS2_WS_INFO_FREE(ws_info, env);
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_ws_info_list_add_ws_info_item(axis2_ws_info_list_t *info_list,
                                    axis2_env_t **env,
                                    axis2_file_t *file, 
                                    int type) 
{
    axis2_ws_info_list_impl_t *info_list_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_FUNC_PARAM_CHECK(info_list, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, file, AXIS2_FAILURE);
    
    info_list_impl = AXIS2_INTF_TO_IMPL(info_list);
    switch (type) 
    {
        case AXIS2_SVC:
        {
            /* check whether the file is already deployed */
            status = axis2_file_handler_access(file->name, AXIS2_F_OK);
            if(AXIS2_SUCCESS != status)
            {
                axis2_ws_info_t *ws_info = NULL;
                long last_modified_date = 0;
                axis2_arch_file_data_t *file_data = NULL;
                
                last_modified_date = file->time_stamp;
                ws_info = 
            axis2_ws_info_create_with_file_name_and_last_modified_date_and_type(
                env, file->name, last_modified_date, AXIS2_SVC);
                AXIS2_ARRAY_LIST_ADD(info_list_impl->info_list, env, ws_info);
                file_data = axis2_arch_file_data_create_with_type_and_file(env,
                    AXIS2_SVC, file);
                /* to inform that new web service is deployed */
                AXIS2_DEP_ENGINE_ADD_WS_TO_DEPLOY(info_list_impl->deployer, 
                    env, file_data);            
                
            }
            
            break;
        }
        case AXIS2_MODULE:
        {
            /* check whether the file is already deployed */
            status = axis2_file_handler_access(file->name, AXIS2_F_OK);
            if(AXIS2_SUCCESS != status)
            {
                axis2_ws_info_t *ws_info = NULL;
                long last_modified_date = 0;
                axis2_arch_file_data_t *file_data = NULL;
                
                last_modified_date = file->time_stamp;
                ws_info = 
            axis2_ws_info_create_with_file_name_and_last_modified_date_and_type(
                env, file->name, last_modified_date, AXIS2_MODULE);
                AXIS2_ARRAY_LIST_ADD(info_list_impl->info_list, env, ws_info);
                file_data = axis2_arch_file_data_create_with_type_and_file(env,
                    AXIS2_MODULE, file);
                /* to inform that new web service is deployed */
                AXIS2_DEP_ENGINE_ADD_WS_TO_DEPLOY(info_list_impl->deployer, env, file_data);
            }    
                
            break;
        }
    }
    axis2_char_t *info_list_name = file->name;
    status = AXIS2_ARRAY_LIST_ADD(info_list_impl->current_info_lists, env,
        info_list_name);
    return status;
}

axis2_ws_info_t *AXIS2_CALL
axis2_ws_info_list_get_file_item(axis2_ws_info_list_t *info_list,
                                    axis2_env_t **env,
                                    axis2_char_t *file_name) 
{
    axis2_ws_info_list_impl_t *info_list_impl = NULL;
    int i = 0;
    
    AXIS2_FUNC_PARAM_CHECK(info_list, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, file_name, NULL);
    info_list_impl = AXIS2_INTF_TO_IMPL(info_list);
    
    int size = AXIS2_ARRAY_LIST_SIZE(info_list_impl->info_list, env);
    for (i = 0; i < size; i++) 
    {
        axis2_ws_info_t *ws_info = NULL;
        axis2_char_t *file_name_l = NULL;
        
        ws_info = (axis2_ws_info_t *) AXIS2_ARRAY_LIST_GET(info_list_impl->
            info_list, env, i);
        file_name_l = AXIS2_WS_INFO_GET_FILE_NAME(ws_info, env);
        if (0 == AXIS2_STRCMP(file_name_l, file_name)) 
        {
            return ws_info;
        }
    }
    return NULL;
}

axis2_bool_t AXIS2_CALL
axis2_ws_info_list_is_modified(axis2_ws_info_list_t *info_list,
                                axis2_env_t **env,
                                axis2_file_t *file, 
                                axis2_ws_info_t *ws_info) 
{
    long last_modified_date = 0;
    
    AXIS2_FUNC_PARAM_CHECK(info_list, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, file, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, ws_info, AXIS2_FAILURE);
    
    last_modified_date = AXIS2_WS_INFO_GET_LAST_MODIFIED_DATE(ws_info, env);
    return (last_modified_date != file->time_stamp);
}

axis2_bool_t AXIS2_CALL
axis2_ws_info_list_is_file_exist(axis2_ws_info_list_t *info_list,
                                    axis2_env_t **env,
                                    axis2_char_t *file_name) 
{
    axis2_ws_info_t *ws_info = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(info_list, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, file_name, AXIS2_FAILURE);
    ws_info = axis2_ws_info_list_get_file_item(info_list, env, file_name);
    return !(ws_info == NULL);
}

axis2_status_t AXIS2_CALL
axis2_ws_info_list_check_for_undeploy(axis2_ws_info_list_t *info_list,
                                        axis2_env_t **env) 
{
    axis2_ws_info_list_impl_t *info_list_impl = NULL;
    int list_size = 0;
    int current_lists_size = 0;
    axis2_array_list_t *temp_list = NULL;
    axis2_char_t *file_name = NULL;
    axis2_bool_t exist = AXIS2_FALSE;
    int i = 0;
    int j = 0;
    
    AXIS2_FUNC_PARAM_CHECK(info_list, env, AXIS2_FAILURE);
    info_list_impl = AXIS2_INTF_TO_IMPL(info_list);
    
    for(i = 0; i < list_size; i++)
    {
        axis2_ws_info_t *file_item = NULL;
        axis2_char_t *file_item_name = NULL;
        
        file_item = (axis2_ws_info_t *) AXIS2_ARRAY_LIST_GET(info_list_impl->
            info_list, env, i);
        exist = AXIS2_FALSE;
        for (j = 0; j < current_lists_size; j++) 
        {
            file_item_name = AXIS2_WS_INFO_GET_FILE_NAME(file_item, env);            
            file_name = (axis2_char_t *) AXIS2_ARRAY_LIST_GET(info_list_impl->
                current_info_lists, env, j);
            if(0 == AXIS2_STRCMP(file_name, file_item_name))
            {
                exist = AXIS2_TRUE;
                break;
            }
        }

        if (!exist) 
        {
            axis2_ws_info_t *ws_info = NULL;
            long last_modified_date = 0;
            
            last_modified_date = AXIS2_WS_INFO_GET_LAST_MODIFIED_DATE(file_item,
                env);
            AXIS2_ARRAY_LIST_ADD(temp_list, env, file_item);
            ws_info = axis2_ws_info_create_with_file_name_and_last_modified_date(
                env, file_item_name, last_modified_date);
            /* this is to be undeployed */
            AXIS2_DEP_ENGINE_ADD_WS_TO_UNDEPLOY(info_list_impl->deployer, env, 
                ws_info);
        }

    }

    list_size = AXIS2_ARRAY_LIST_SIZE(temp_list, env);
    for (i = 0; i < list_size; i++) 
    {
        axis2_ws_info_t *file_item = NULL;
        int index = 0;
        
        file_item = (axis2_ws_info_t*) AXIS2_ARRAY_LIST_GET(temp_list, env, i);
        index = AXIS2_ARRAY_LIST_INDEX_OF(info_list_impl->info_list, env,
            file_item);
        AXIS2_ARRAY_LIST_REMOVE(info_list_impl->info_list, env, index);
    }
    AXIS2_ARRAY_LIST_FREE(temp_list, env);
    AXIS2_ARRAY_LIST_FREE(info_list_impl->current_info_lists, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_ws_info_list_update(axis2_ws_info_list_t *info_list,
                            axis2_env_t **env) 
{
    axis2_ws_info_list_impl_t *info_list_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_FUNC_PARAM_CHECK(info_list, env, AXIS2_FAILURE);
    info_list_impl = AXIS2_INTF_TO_IMPL(info_list);
    
    axis2_ws_info_list_check_for_undeploy(info_list, env);
    /* TODO uncomment when hot deployment supported */
    /*
    if (deployer.isHotUpdate()) {
        deployer.unDeploy();
    }
    */
    status = AXIS2_DEP_ENGINE_DO_DEPLOY(info_list_impl->deployer, env);
    return status;
}
