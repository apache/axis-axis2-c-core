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

#include <axis2_class_loader.h>

axis2_status_t 
axis2_class_loader_load_lib (axis2_env_t **env,
                                axis2_dll_desc_t *dll_desc);
                                
axis2_status_t
axis2_class_loader_unload_lib (axis2_env_t **env,
                                axis2_dll_desc_t *dll_desc);
                                
                                
AXIS2_DECLARE(axis2_status_t)
axis2_class_loader_init(axis2_env_t **env)
{
    AXIS2_PLATFORM_LOADLIBINIT();
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_status_t)
axis2_class_loader_delete_dll (axis2_env_t **env,
                                    axis2_param_t *impl_info_param)
{
    axis2_dll_desc_t *dll_desc = NULL;
    
    dll_desc = AXIS2_PARAM_GET_VALUE(impl_info_param, env);
    if(!dll_desc)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_DLL_CREATE_FAILED, 
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    axis2_class_loader_unload_lib(env, dll_desc);
    AXIS2_PLATFORM_LOADLIBEXIT()
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(void *)
axis2_class_loader_create_dll (axis2_env_t **env,
                                axis2_param_t *impl_info_param)
{
    void *svc_skeli = NULL; /* axis2_svc_skeleton */
    void *msg_recv = NULL; /* axis2_msg_recv */
    void *handler = NULL; /* axis2_handler */
    void *module = NULL; /* axis2_module */
    void *transport_recv = NULL; /* axis2_transport_receiver */
    void *transport_sender = NULL; /* axis2_transport_sender */
    CREATE_FUNCT create_funct = NULL;
    DELETE_FUNCT delete_funct = NULL;
    AXIS2_DLHANDLER dl_handler = NULL;
    axis2_dll_desc_t *dll_desc = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_dll_type_t dll_type = AXIS2_SVC_DLL;
    
    dll_desc = AXIS2_PARAM_GET_VALUE(impl_info_param, env);
    if(!dll_desc)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_DLL_CREATE_FAILED, 
            AXIS2_FAILURE);
        return NULL;
    }
    dl_handler = AXIS2_DLL_DESC_GET_DL_HANDLER(dll_desc, env);
    if(NULL == dl_handler)
    {
        status = axis2_class_loader_load_lib(env, dll_desc);
        if(AXIS2_SUCCESS != status)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_DLL_CREATE_FAILED, 
                    AXIS2_FAILURE);
            return NULL;
        }
        dl_handler = AXIS2_DLL_DESC_GET_DL_HANDLER(dll_desc, env);
        if(!dl_handler)
        {
            return NULL;
        }
        
        create_funct = (CREATE_FUNCT) AXIS2_PLATFORM_GETPROCADDR(dl_handler,
            AXIS2_CREATE_FUNCTION);
        if (!create_funct)
        {
            return NULL;
        }
        status = AXIS2_DLL_DESC_SET_CREATE_FUNCT(dll_desc, env, create_funct); 
        if(AXIS2_FAILURE == status)
        {
            axis2_class_loader_unload_lib(env, dll_desc);
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_DLL_CREATE_FAILED, 
                AXIS2_FAILURE);
            return NULL;
        }
            
        delete_funct = (DELETE_FUNCT) AXIS2_PLATFORM_GETPROCADDR(dl_handler,
            AXIS2_DELETE_FUNCTION);
        if (!delete_funct)
        {
            return NULL;
        }
        status = AXIS2_DLL_DESC_SET_DELETE_FUNCT(dll_desc, env, delete_funct);
        if(AXIS2_FAILURE == status)
        {
            axis2_class_loader_unload_lib(env, dll_desc);
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_DLL_CREATE_FAILED, 
                AXIS2_FAILURE);
            return NULL;
        }
    }
    create_funct = AXIS2_DLL_DESC_GET_CREATE_FUNCT(dll_desc, env);
    if(!create_funct)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_DLL_DESC,
            AXIS2_FAILURE);
        return NULL;
    }
    dll_type = AXIS2_DLL_DESC_GET_TYPE(dll_desc, env);
    if(AXIS2_SVC_DLL == dll_type)
    {
        create_funct(&svc_skeli, env);
        if(NULL == svc_skeli)
        {
            axis2_class_loader_unload_lib(env, dll_desc);
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_SVC_SKELETON_CREATION_FAILED,AXIS2_FAILURE);
            return NULL;
        }
        return svc_skeli;
    }
    if(AXIS2_HANDLER_DLL == dll_type)
    {
        create_funct(&handler, env);
        if(NULL == handler)
        {
            axis2_class_loader_unload_lib(env, dll_desc);
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_HANDLER_CREATION_FAILED,AXIS2_FAILURE);
            return NULL;
        }
        return handler;
    }
    if(AXIS2_MODULE_DLL == dll_type)
    {
        create_funct(&module, env);
        if(NULL == module)
        {
            axis2_class_loader_unload_lib(env, dll_desc);
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_MODULE_CREATION_FAILED,AXIS2_FAILURE);
            return NULL;
        }
        return module;
    }
    if(AXIS2_MSG_RECV_DLL == dll_type)
    {
        create_funct(&msg_recv, env);
        if(NULL == msg_recv)
        {
            axis2_class_loader_unload_lib(env, dll_desc);
            AXIS2_LOG_DEBUG((*env)->log, AXIS2_LOG_SI, "message receiver is NULL");
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_MSG_RECV_CREATION_FAILED,AXIS2_FAILURE);
            return NULL;
        }
        else
        {
            AXIS2_LOG_DEBUG((*env)->log, AXIS2_LOG_SI, "message receiver loaded successfully");
        }
        return msg_recv;
    }
    if(AXIS2_TRANSPORT_RECV_DLL == dll_type)
    {
        create_funct(&transport_recv, env);
        if(NULL == transport_recv)
        {
            axis2_class_loader_unload_lib(env, dll_desc);
            AXIS2_LOG_DEBUG((*env)->log, AXIS2_LOG_SI, "transport receiver is NULL");
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_TRANSPORT_RECV_CREATION_FAILED,AXIS2_FAILURE);
            return NULL;
        }
        else
        {
            AXIS2_LOG_DEBUG((*env)->log, AXIS2_LOG_SI, "transport receiver loaded successfully");
        }
        return transport_recv;
    }
    if(AXIS2_TRANSPORT_SENDER_DLL == dll_type)
    {
        create_funct(&transport_sender, env);
        if(NULL == transport_sender)
        {
            axis2_class_loader_unload_lib(env, dll_desc);
            AXIS2_LOG_DEBUG((*env)->log, AXIS2_LOG_SI, "transport sender is NULL");
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_TRANSPORT_SENDER_CREATION_FAILED,AXIS2_FAILURE);
            return NULL;
        }
        else
        {
            AXIS2_LOG_DEBUG((*env)->log, AXIS2_LOG_SI, "transport sender loaded successfully");
        }
        return transport_sender;
    }


    return NULL;    
}

axis2_status_t 
axis2_class_loader_load_lib (axis2_env_t **env,
                                axis2_dll_desc_t *dll_desc)
{
    axis2_char_t *dll_name = NULL;
    AXIS2_DLHANDLER dl_handler = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    dll_name = AXIS2_DLL_DESC_GET_NAME(dll_desc, env);
    dl_handler = AXIS2_PLATFORM_LOADLIB(dll_name);
    if(NULL == dl_handler)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_DLL_LOADING_FAILED, 
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    status = AXIS2_DLL_DESC_SET_DL_HANDLER(dll_desc, env, dl_handler);

    if(AXIS2_FAILURE == status)
    {
        AXIS2_FREE((*env)->allocator, dl_handler);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_DLL_LOADING_FAILED, 
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t
axis2_class_loader_unload_lib (axis2_env_t **env,
                                axis2_dll_desc_t *dll_desc)
{
    AXIS2_DLHANDLER dl_handler = AXIS2_DLL_DESC_GET_DL_HANDLER(dll_desc, env);
    
    AXIS2_PLATFORM_UNLOADLIB(dl_handler);

    return AXIS2_SUCCESS;
}
