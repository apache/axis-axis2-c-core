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

#ifndef AXIS2_DLL_DESC_H
#define AXIS2_DLL_DESC_H

/**
 * @file axis2_dll_desc.h
 * @brief Axis2 dll_desc interface
 */

#include <axis2_defines.h>
#include <axis2_qname.h>
#include <axis2_error.h>
#include <axis2.h>
#include <axis2_svc_skeleton.h>
#include <axis2_platform_auto_sense.h>

#ifdef __cplusplus
extern "C"
{
#endif
    
/**
 * @defgroup axis2_dll_desc DLL Description
 * @ingroup axis2_description
 * @{
 */

    
typedef struct axis2_dll_desc axis2_dll_desc_t;
typedef struct axis2_dll_desc_ops axis2_dll_desc_ops_t;
     
    
typedef int (*CREATE_FUNCT) (void **inst);

typedef int (*DELETE_FUNCT) (void *inst);

typedef enum axis2_dll_type
{
    /** service dll */
    AXIS2_SVC_DLL = 0,
    /** handler dll */
    AXIS2_HANDLER_DLL,
    /** message receiver dll */
    AXIS2_MSG_RECV_DLL
       
}axis2_dll_type_t;

/**
 *   \brief DLL Description ops struct
 */
AXIS2_DECLARE_DATA struct axis2_dll_desc_ops
{ 
    axis2_status_t (AXIS2_CALL *
    free) (struct axis2_dll_desc *dll_desc, 
            axis2_env_t **env);
        
    axis2_status_t (AXIS2_CALL *
    set_name) (axis2_dll_desc_t *dll_desc,
                            axis2_env_t **env,
                            axis2_char_t *name);
    
    axis2_char_t *(AXIS2_CALL *
    get_name) (axis2_dll_desc_t *dll_desc,
                            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_type) (axis2_dll_desc_t *dll_desc,
                            axis2_env_t **env,
                            axis2_dll_type_t type);
    
    axis2_dll_type_t (AXIS2_CALL *
    get_type) (axis2_dll_desc_t *dll_desc,
                            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_load_options) (axis2_dll_desc_t *dll_desc,
                            axis2_env_t **env,
                            int options);
    
    int (AXIS2_CALL *
    get_load_options) (axis2_dll_desc_t *dll_desc,
                            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_dl_handler) (axis2_dll_desc_t *dll_desc,
                            axis2_env_t **env,
                            AXIS2_DLHANDLER dl_handler);
    
    AXIS2_DLHANDLER (AXIS2_CALL *
    get_dl_handler) (axis2_dll_desc_t *dll_desc,
                            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_create_funct) (axis2_dll_desc_t *dll_desc,
                            axis2_env_t **env,
                            CREATE_FUNCT funct);
    
    CREATE_FUNCT (AXIS2_CALL *
    get_create_funct) (axis2_dll_desc_t *dll_desc,
                            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_delete_funct) (axis2_dll_desc_t *dll_desc,
                            axis2_env_t **env,
                            DELETE_FUNCT funct);
    
    DELETE_FUNCT (AXIS2_CALL *
    get_delete_funct) (axis2_dll_desc_t *dll_desc,
                            axis2_env_t **env);

        
};
	
/** 
* \brief DLL Description struct
*/
AXIS2_DECLARE_DATA struct axis2_dll_desc
{
    /** DLL Description related ops */
    axis2_dll_desc_ops_t *ops;
    
};


/**
 * creates dll_desc struct
 * @param qname qname, can be NULL
 */
AXIS2_DECLARE(axis2_dll_desc_t*) 
axis2_dll_desc_create(axis2_env_t **env);

#define AXIS2_DLL_DESC_GET_NAME(dll_desc, env) \
    ((dll_desc)->ops->get_name(dll_desc, env))
    
#define AXIS2_DLL_DESC_SET_NAME(dll_desc, env, name) \
    ((dll_desc)->ops->set_name(dll_desc, env, name))

#define AXIS2_DLL_DESC_GET_TYPE(dll_desc, env) \
    ((dll_desc)->ops->get_type(dll_desc, env))
    
#define AXIS2_DLL_DESC_SET_TYPE(dll_desc, env, type) \
    ((dll_desc)->ops->set_type(dll_desc, env, type))

#define AXIS2_DLL_DESC_GET_LOAD_OPTIONS(dll_desc, env) \
    ((dll_desc)->ops->get_load_options(dll_desc, env))
    
#define AXIS2_DLL_DESC_SET_LOAD_OPTIONS(dll_desc, env, load_options) \
    ((dll_desc)->ops->set_load_options(dll_desc, env, load_options))
    
#define AXIS2_DLL_DESC_GET_DL_HANDLER(dll_desc, env) \
    ((dll_desc)->ops->get_dl_handler(dll_desc, env))
    
#define AXIS2_DLL_DESC_SET_DL_HANDLER(dll_desc, env, dl_handler) \
    ((dll_desc)->ops->set_dl_handler(dll_desc, env, dl_handler))
    
#define AXIS2_DLL_DESC_GET_CREATE_FUNCT(dll_desc, env) \
    ((dll_desc)->ops->get_create_funct(dll_desc, env))
    
#define AXIS2_DLL_DESC_SET_CREATE_FUNCT(dll_desc, env, funct) \
    ((dll_desc)->ops->set_create_funct(dll_desc, env, funct))
    
#define AXIS2_DLL_DESC_GET_DELETE_FUNCT(dll_desc, env) \
    ((dll_desc)->ops->get_delete_funct((dll_desc, env))
    
#define AXIS2_DLL_DESC_SET_DELETE_FUNCT(dll_desc, env, funct) \
    ((dll_desc)->ops->set_delete_funct(dll_desc, env, funct))
    
/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_DLL_DESC_H */
