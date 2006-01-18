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
 
#include <axis2_arch_file_data.h>
#include <string.h>
#include <axis2_class_loader.h>

/** 
 * @brief
 * ArchiveFileData = Hot Deployment File Item.
 * To store infromation of the 
 * module or service item to be deployed
 */
typedef struct axis2_arch_file_data_impl
{
	axis2_arch_file_data_t arch_file_data;
    axis2_file_t *file;
    int type;
    axis2_char_t *msg_recv;
    axis2_char_t *module_dll_name;
    axis2_char_t *name;
    /*
     * To store services in a serviceGroup. If there are wsdl for those 
     * servics ,so wsdl service will be created for each wsdl an those will be 
     * temporarily store in this table
     */
    axis2_hash_t *svc_map;
    axis2_array_list_t *deployable_svcs;
    	
} axis2_arch_file_data_impl_t;

#define AXIS2_INTF_TO_IMPL(arch_file_data) \
    ((axis2_arch_file_data_impl_t *) arch_file_data)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
axis2_arch_file_data_free (axis2_arch_file_data_t *arch_file_data, 
                            axis2_env_t **env);

axis2_char_t *AXIS2_CALL
axis2_arch_file_data_get_msg_recv(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_arch_file_data_set_msg_recv(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env,
                                    axis2_char_t *msg_recv);

axis2_char_t *AXIS2_CALL
axis2_arch_file_data_get_name(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env);

axis2_char_t *AXIS2_CALL
axis2_arch_file_data_get_svc_name(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env);

int AXIS2_CALL
axis2_arch_file_data_get_type(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env);

axis2_file_t *AXIS2_CALL
axis2_arch_file_data_get_file(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env);

axis2_char_t *AXIS2_CALL
axis2_arch_file_data_get_module_dll_name(axis2_arch_file_data_t *file_data,
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_arch_file_data_set_module_dll_name(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env,
                                    axis2_char_t *module_dll_name);
axis2_status_t AXIS2_CALL
axis2_arch_file_data_add_svc(axis2_arch_file_data_t *file_data,
                                axis2_env_t **env,
                                struct axis2_svc *svc_desc);

struct axis2_svc *AXIS2_CALL
axis2_arch_file_data_get_svc(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env,
                                    axis2_char_t *svc_name);

axis2_hash_t *AXIS2_CALL
axis2_arch_file_data_get_svc_map(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL
axis2_arch_file_data_get_deployable_svcs(axis2_arch_file_data_t *file_data,
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_arch_file_data_set_deployable_svcs(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env,
                                    axis2_array_list_t *deployable_svcs);

                                
/************************** End of function prototypes ************************/

axis2_arch_file_data_t * AXIS2_CALL 
axis2_arch_file_data_create(axis2_env_t **env)
{
    axis2_arch_file_data_impl_t *arch_file_data_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	arch_file_data_impl = (axis2_arch_file_data_impl_t *) AXIS2_MALLOC((*env)->
        allocator, sizeof(axis2_arch_file_data_impl_t));
	
	
	if(NULL == arch_file_data_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    arch_file_data_impl->file = NULL;
    arch_file_data_impl->type = 0;
    arch_file_data_impl->msg_recv = NULL;
    arch_file_data_impl->module_dll_name = NULL;
    arch_file_data_impl->name = NULL;
    arch_file_data_impl->svc_map = NULL;
    arch_file_data_impl->deployable_svcs = NULL;
    arch_file_data_impl->arch_file_data.ops = NULL;
    
    arch_file_data_impl->deployable_svcs = axis2_array_list_create(env, 20);
    if(!arch_file_data_impl->deployable_svcs)
    {
        axis2_arch_file_data_free(&(arch_file_data_impl->arch_file_data), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	arch_file_data_impl->arch_file_data.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_arch_file_data_ops_t));
	if(NULL == arch_file_data_impl->arch_file_data.ops)
    {
        axis2_arch_file_data_free(&(arch_file_data_impl->arch_file_data), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	arch_file_data_impl->arch_file_data.ops->free =  axis2_arch_file_data_free;
    arch_file_data_impl->arch_file_data.ops->get_msg_recv = 
        axis2_arch_file_data_get_msg_recv;
    arch_file_data_impl->arch_file_data.ops->set_msg_recv = 
        axis2_arch_file_data_set_msg_recv;
    arch_file_data_impl->arch_file_data.ops->get_name = 
        axis2_arch_file_data_get_name;
    arch_file_data_impl->arch_file_data.ops->get_svc_name = 
        axis2_arch_file_data_get_svc_name;
    arch_file_data_impl->arch_file_data.ops->get_type = 
        axis2_arch_file_data_get_type;
    arch_file_data_impl->arch_file_data.ops->get_file = 
        axis2_arch_file_data_get_file;
    arch_file_data_impl->arch_file_data.ops->get_module_dll_name = 
        axis2_arch_file_data_get_module_dll_name;
    arch_file_data_impl->arch_file_data.ops->set_module_dll_name =
        axis2_arch_file_data_set_module_dll_name;
    arch_file_data_impl->arch_file_data.ops->add_svc = 
        axis2_arch_file_data_add_svc;
    arch_file_data_impl->arch_file_data.ops->get_svc =
        axis2_arch_file_data_get_svc;    
    arch_file_data_impl->arch_file_data.ops->get_svc_map = 
        axis2_arch_file_data_get_svc_map;
    arch_file_data_impl->arch_file_data.ops->get_deployable_svcs = 
        axis2_arch_file_data_get_deployable_svcs;
    arch_file_data_impl->arch_file_data.ops->set_deployable_svcs =
        axis2_arch_file_data_set_deployable_svcs;    
	
	return &(arch_file_data_impl->arch_file_data);
}

axis2_arch_file_data_t * AXIS2_CALL 
axis2_arch_file_data_create_with_type_and_file(axis2_env_t **env,
                                                    int type,
                                                    axis2_file_t *file)
{
    axis2_arch_file_data_impl_t *file_data_impl = NULL;
    file_data_impl = (axis2_arch_file_data_impl_t *) 
        axis2_arch_file_data_create(env);
    if(NULL == file_data_impl)
    {
        return NULL;
    }
    file_data_impl->type = type;
    file_data_impl->file = AXIS2_FILE_CLONE(file, env);
    return &(file_data_impl->arch_file_data);    
}

axis2_arch_file_data_t * AXIS2_CALL 
axis2_arch_file_data_create_with_type_and_name(axis2_env_t **env,
                                                    int type,
                                                    axis2_char_t *name)
{
    axis2_arch_file_data_impl_t *file_data_impl = NULL;
    file_data_impl = (axis2_arch_file_data_impl_t *)
        axis2_arch_file_data_create(env);
    if(NULL == file_data_impl)
    {
        return NULL;
    }
    file_data_impl->type = type;
    file_data_impl->name = AXIS2_STRDUP(name, env);
    return &(file_data_impl->arch_file_data);    
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_arch_file_data_free (axis2_arch_file_data_t *arch_file_data, 
                            axis2_env_t **env)
{
    axis2_arch_file_data_impl_t *file_data_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(arch_file_data, env, AXIS2_FAILURE);
    
    file_data_impl = AXIS2_INTF_TO_IMPL(arch_file_data);
    
    if(file_data_impl->file)
    {
        AXIS2_FILE_FREE(file_data_impl->file, env);
        file_data_impl->file = NULL;        
    }
    if(file_data_impl->msg_recv)
    {
        AXIS2_FREE((*env)->allocator, file_data_impl->msg_recv);
        file_data_impl->msg_recv = NULL;        
    }
    if(file_data_impl->module_dll_name)
    {
        AXIS2_FREE((*env)->allocator, file_data_impl->module_dll_name);
        file_data_impl->module_dll_name = NULL;        
    }
    if(file_data_impl->name)
    {
        AXIS2_FREE((*env)->allocator, file_data_impl->name);
        file_data_impl->name = NULL;        
    }
    
    if(file_data_impl->svc_map)
    {
        axis2_hash_free(file_data_impl->svc_map, env);
        file_data_impl->svc_map = NULL;        
    }
    if(file_data_impl->deployable_svcs)
    {
        AXIS2_ARRAY_LIST_FREE(file_data_impl->deployable_svcs, env);
        file_data_impl->deployable_svcs = NULL;        
    }
    
    file_data_impl->arch_file_data.ops = NULL;
    
	if(NULL != arch_file_data->ops)
    {
        AXIS2_FREE((*env)->allocator, arch_file_data->ops);
        arch_file_data->ops = NULL;
    }
    
    if(file_data_impl)
    {
        AXIS2_FREE((*env)->allocator, file_data_impl);
        file_data_impl = NULL;
    }
    
	return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_arch_file_data_get_msg_recv(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(file_data, env, NULL);
    return AXIS2_INTF_TO_IMPL(file_data)->msg_recv;
}

axis2_status_t AXIS2_CALL
axis2_arch_file_data_set_msg_recv(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env,
                                    axis2_char_t *msg_recv)
{
    axis2_arch_file_data_impl_t *file_data_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(file_data, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_recv, AXIS2_FAILURE);
    
    file_data_impl = AXIS2_INTF_TO_IMPL(file_data);
    if(file_data_impl->msg_recv)
    {
        AXIS2_FREE((*env)->allocator, file_data_impl->msg_recv);
        file_data_impl->msg_recv = NULL;
    }
    file_data_impl->msg_recv = AXIS2_STRDUP(msg_recv, env);
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_arch_file_data_get_name(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env) 
{
    axis2_arch_file_data_impl_t *file_data_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(file_data, env, NULL);
    file_data_impl = AXIS2_INTF_TO_IMPL(file_data);
    
    return AXIS2_FILE_GET_NAME(file_data_impl->file, env);
}

axis2_char_t *AXIS2_CALL
axis2_arch_file_data_get_svc_name(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env)
{
    axis2_arch_file_data_impl_t *file_data_impl = NULL;
    axis2_char_t *svc_name = NULL;
    AXIS2_FUNC_PARAM_CHECK(file_data, env, NULL);
    file_data_impl = AXIS2_INTF_TO_IMPL(file_data);
    if(NULL != file_data_impl->file)
    {
        svc_name = AXIS2_FILE_GET_NAME(file_data_impl->file, env);
    }
    else
    {
        svc_name = file_data_impl->name;
    }
    return svc_name;
}

int AXIS2_CALL
axis2_arch_file_data_get_type(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(file_data, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(file_data)->type;
}

axis2_file_t *AXIS2_CALL
axis2_arch_file_data_get_file(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(file_data, env, NULL);
    return AXIS2_INTF_TO_IMPL(file_data)->file;
}

axis2_char_t *AXIS2_CALL
axis2_arch_file_data_get_module_dll_name(axis2_arch_file_data_t *file_data,
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(file_data, env, NULL);
    return AXIS2_INTF_TO_IMPL(file_data)->module_dll_name;
}

axis2_status_t AXIS2_CALL
axis2_arch_file_data_set_module_dll_name(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env,
                                    axis2_char_t *module_dll_name) 
{
    axis2_arch_file_data_impl_t *file_data_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(file_data, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_dll_name, AXIS2_FAILURE);
    
    file_data_impl = AXIS2_INTF_TO_IMPL(file_data);
    if(file_data_impl->module_dll_name)
    {
        AXIS2_FREE((*env)->allocator, file_data_impl->module_dll_name);
        file_data_impl->module_dll_name = NULL;
    }
    file_data_impl->module_dll_name = module_dll_name;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_arch_file_data_add_svc(axis2_arch_file_data_t *file_data,
                                axis2_env_t **env,
                                struct axis2_svc *svc_desc)
{
    axis2_arch_file_data_impl_t *file_data_impl = NULL;
    axis2_qname_t *svc_qname = NULL;
    axis2_char_t *svc_name = NULL;
    AXIS2_FUNC_PARAM_CHECK(file_data, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_desc, AXIS2_FAILURE);
    
    file_data_impl = AXIS2_INTF_TO_IMPL(file_data);
    
    svc_qname = AXIS2_SVC_GET_QNAME(svc_desc, env);
    svc_name = AXIS2_QNAME_GET_LOCALPART(svc_qname, env);
    if(!file_data_impl->svc_map)
    {
        file_data_impl->svc_map = axis2_hash_make(env);
        if(!file_data_impl->svc_map)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    axis2_hash_set(file_data_impl->svc_map, svc_name, AXIS2_HASH_KEY_STRING,
        svc_desc);
    return AXIS2_SUCCESS;
}

struct axis2_svc *AXIS2_CALL
axis2_arch_file_data_get_svc(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env,
                                    axis2_char_t *svc_name)
{
    axis2_arch_file_data_impl_t *file_data_impl = NULL;
    axis2_svc_t *svc = NULL;
        
    AXIS2_FUNC_PARAM_CHECK(file_data, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_name, AXIS2_FAILURE);
    file_data_impl = AXIS2_INTF_TO_IMPL(file_data);
    
    if(file_data_impl->svc_map)
    {
        svc = (axis2_svc_t *) axis2_hash_get(file_data_impl->svc_map, svc_name, 
            AXIS2_HASH_KEY_STRING);
    }
    else
    {
        return NULL;
    }
    return svc;
}

axis2_hash_t *AXIS2_CALL
axis2_arch_file_data_get_svc_map(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(file_data, env, NULL);
    return AXIS2_INTF_TO_IMPL(file_data)->svc_map;
}

axis2_array_list_t *AXIS2_CALL
axis2_arch_file_data_get_deployable_svcs(axis2_arch_file_data_t *file_data,
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(file_data, env, NULL);
    return AXIS2_INTF_TO_IMPL(file_data)->deployable_svcs;
}

axis2_status_t AXIS2_CALL
axis2_arch_file_data_set_deployable_svcs(axis2_arch_file_data_t *file_data,
                                    axis2_env_t **env,
                                    axis2_array_list_t *deployable_svcs) 
{
    axis2_arch_file_data_impl_t *file_data_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(file_data, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, deployable_svcs, AXIS2_FAILURE);
    
    file_data_impl = AXIS2_INTF_TO_IMPL(file_data);
    if(file_data_impl->deployable_svcs)
    {
        AXIS2_FREE((*env)->allocator, file_data_impl->deployable_svcs);
        file_data_impl->deployable_svcs = NULL;
    }
    file_data_impl->deployable_svcs = deployable_svcs;
    return AXIS2_SUCCESS;
}
