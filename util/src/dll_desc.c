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
 
#include <axis2_dll_desc.h>
#include <axis2_class_loader.h>

/** 
 * @brief DLL Description struct impl
 * Axis2 DLL Description impl  
 */ 
typedef struct axis2_dll_desc_impl
{
   axis2_dll_desc_t dll_desc;
    
    axis2_char_t *dll_name;
    axis2_char_t *path_qualified_dll_name;
    axis2_dll_type_t dll_type;
    int load_options;
    AXIS2_DLHANDLER dl_handler;
    CREATE_FUNCT create_funct;
    DELETE_FUNCT delete_funct;
    AXIS2_TIME_T timestamp;
    axis2_error_codes_t error_code;
    
    
} axis2_dll_desc_impl_t;

#define AXIS2_INTF_TO_IMPL(dll_desc) ((axis2_dll_desc_impl_t *) dll_desc)
   
/*************************** Function headers *********************************/

axis2_status_t AXIS2_CALL
axis2_dll_desc_free (axis2_dll_desc_t *dll_desc,
                            const axis2_env_t *env);


axis2_status_t AXIS2_CALL
axis2_dll_desc_set_name(axis2_dll_desc_t *dll_desc,
                        const axis2_env_t *env,
                        axis2_char_t *name);

axis2_char_t *AXIS2_CALL
axis2_dll_desc_get_name(axis2_dll_desc_t *dll_desc,
                        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_dll_desc_set_type (axis2_dll_desc_t *dll_desc,
                            const axis2_env_t *env,
                            axis2_dll_type_t type);

axis2_dll_type_t AXIS2_CALL
axis2_dll_desc_get_type (axis2_dll_desc_t *dll_desc,
                            const axis2_env_t *env);
                            
axis2_status_t AXIS2_CALL
axis2_dll_desc_set_load_options(axis2_dll_desc_t *dll_desc,
                                const axis2_env_t *env,
                                int options);

int AXIS2_CALL
axis2_dll_desc_get_load_options(axis2_dll_desc_t *dll_desc,
                                const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_dll_desc_set_dl_handler(axis2_dll_desc_t *dll_desc,
                                const axis2_env_t *env,
                                AXIS2_DLHANDLER dl_handler);

AXIS2_DLHANDLER AXIS2_CALL
axis2_dll_desc_get_dl_handler(axis2_dll_desc_t *dll_desc,
                                const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_dll_desc_set_create_funct(axis2_dll_desc_t *dll_desc,
                                const axis2_env_t *env,
                                CREATE_FUNCT funct);

CREATE_FUNCT AXIS2_CALL
axis2_dll_desc_get_create_funct(axis2_dll_desc_t *dll_desc,
                                const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_dll_desc_set_delete_funct(axis2_dll_desc_t *dll_desc,
                                const axis2_env_t *env,
                                DELETE_FUNCT funct);

DELETE_FUNCT AXIS2_CALL
axis2_dll_desc_get_delete_funct(axis2_dll_desc_t *dll_desc,
                                const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_dll_desc_set_timestamp (axis2_dll_desc_t *dll_desc,
                                const axis2_env_t *env,
                                AXIS2_TIME_T timestamp);

axis2_status_t AXIS2_CALL
axis2_dll_desc_set_error_code (
        axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env,
        axis2_error_codes_t error_code);

axis2_error_codes_t AXIS2_CALL
axis2_dll_desc_get_error_code (
        axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env);

AXIS2_TIME_T AXIS2_CALL
axis2_dll_desc_get_timestamp (axis2_dll_desc_t *dll_desc,
                                const axis2_env_t *env);
axis2_char_t * AXIS2_CALL
axis2_dll_desc_create_platform_specific_dll_name(axis2_dll_desc_t *dll_desc,
                                                    const axis2_env_t *env,
                                                    axis2_char_t *class_name);
                       
/************************* End of function headers ****************************/   

AXIS2_EXTERN axis2_dll_desc_t * AXIS2_CALL
axis2_dll_desc_create (const axis2_env_t *env)
{
    axis2_dll_desc_impl_t *dll_desc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
   dll_desc_impl = (axis2_dll_desc_impl_t *) AXIS2_MALLOC (env->allocator
          , sizeof (axis2_dll_desc_impl_t));
    
   if(NULL == dll_desc_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    dll_desc_impl->dll_name = NULL;
    dll_desc_impl->path_qualified_dll_name = NULL;
    dll_desc_impl->dll_type = 0;
    dll_desc_impl->load_options = 0;
    dll_desc_impl->dl_handler = NULL;
    dll_desc_impl->create_funct = NULL;
    dll_desc_impl->delete_funct = NULL;
    dll_desc_impl->dll_desc.ops = NULL;
    dll_desc_impl->timestamp = 0;
    dll_desc_impl->error_code = AXIS2_ERROR_NONE;
    
    dll_desc_impl->dll_desc.ops = (axis2_dll_desc_ops_t *) AXIS2_MALLOC(
        env->allocator, sizeof(axis2_dll_desc_ops_t));
    
   if(NULL == dll_desc_impl->dll_desc.ops)
   {
        axis2_dll_desc_free(&(dll_desc_impl->dll_desc), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
   }
    
   dll_desc_impl->dll_desc.ops->free = axis2_dll_desc_free;
    dll_desc_impl->dll_desc.ops->get_name = axis2_dll_desc_get_name;
    dll_desc_impl->dll_desc.ops->set_name = axis2_dll_desc_set_name;
    dll_desc_impl->dll_desc.ops->get_load_options = axis2_dll_desc_get_load_options;
    dll_desc_impl->dll_desc.ops->set_load_options = axis2_dll_desc_set_load_options;
    dll_desc_impl->dll_desc.ops->get_dl_handler = axis2_dll_desc_get_dl_handler;
    dll_desc_impl->dll_desc.ops->set_dl_handler = axis2_dll_desc_set_dl_handler;
    dll_desc_impl->dll_desc.ops->get_create_funct = axis2_dll_desc_get_create_funct;
    dll_desc_impl->dll_desc.ops->set_create_funct = axis2_dll_desc_set_create_funct;
    dll_desc_impl->dll_desc.ops->get_delete_funct = axis2_dll_desc_get_delete_funct;
    dll_desc_impl->dll_desc.ops->set_delete_funct = axis2_dll_desc_set_delete_funct;
    dll_desc_impl->dll_desc.ops->set_type = axis2_dll_desc_set_type;
    dll_desc_impl->dll_desc.ops->get_type = axis2_dll_desc_get_type;
    dll_desc_impl->dll_desc.ops->get_timestamp = axis2_dll_desc_get_timestamp;
    dll_desc_impl->dll_desc.ops->set_timestamp = axis2_dll_desc_set_timestamp;
   dll_desc_impl->dll_desc.ops->set_error_code = axis2_dll_desc_set_error_code;               
   dll_desc_impl->dll_desc.ops->get_error_code = axis2_dll_desc_get_error_code;               
   dll_desc_impl->dll_desc.ops->create_platform_specific_dll_name = 
        axis2_dll_desc_create_platform_specific_dll_name;               
   return &(dll_desc_impl->dll_desc);
}

/******************************************************************************/

axis2_status_t AXIS2_CALL
axis2_dll_desc_free (axis2_dll_desc_t *dll_desc,
                        const axis2_env_t *env)
{
    axis2_dll_desc_impl_t *dll_desc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);    
    dll_desc_impl = AXIS2_INTF_TO_IMPL(dll_desc);
    
    if(dll_desc_impl->dl_handler)
    {
        axis2_class_loader_delete_dll(env, dll_desc);
    }
   
    if(dll_desc_impl->dll_name)
    {
        AXIS2_FREE(env->allocator, dll_desc_impl->dll_name);
        dll_desc_impl->dll_name = NULL;
    }
    
    if(dll_desc_impl->path_qualified_dll_name)
    {
        AXIS2_FREE(env->allocator, dll_desc_impl->path_qualified_dll_name);
        dll_desc_impl->path_qualified_dll_name = NULL;
    }
       
    if(NULL != dll_desc->ops)
    {
      AXIS2_FREE(env->allocator, dll_desc->ops);
        dll_desc->ops = NULL;
    }
    dll_desc_impl->create_funct = NULL;
    dll_desc_impl->delete_funct = NULL;
    
    if(dll_desc_impl)
    {
        AXIS2_FREE(env->allocator, dll_desc_impl);
        dll_desc_impl = NULL;
    }
    
   return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_dll_desc_free_void_arg (void *dll_desc,
                                    const axis2_env_t *env)
{
    axis2_dll_desc_t *dll_desc_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    dll_desc_l = (axis2_dll_desc_t *) dll_desc;
    return axis2_dll_desc_free(dll_desc_l, env);
}

axis2_status_t AXIS2_CALL
axis2_dll_desc_set_name(axis2_dll_desc_t *dll_desc,
                        const axis2_env_t *env,
                        axis2_char_t *name)
{
    axis2_dll_desc_impl_t *dll_desc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, name, AXIS2_FAILURE); 
    dll_desc_impl = AXIS2_INTF_TO_IMPL(dll_desc);
    
    if(dll_desc_impl->path_qualified_dll_name)
    {
        AXIS2_FREE(env->allocator, dll_desc_impl->path_qualified_dll_name);
        dll_desc_impl->path_qualified_dll_name = NULL;
    }
    dll_desc_impl->path_qualified_dll_name = AXIS2_STRDUP(name, env);
    if(!dll_desc_impl->path_qualified_dll_name)
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_dll_desc_get_name(axis2_dll_desc_t *dll_desc,
                        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(dll_desc)->path_qualified_dll_name;
}

axis2_status_t AXIS2_CALL
axis2_dll_desc_set_load_options(axis2_dll_desc_t *dll_desc,
                        const axis2_env_t *env,
                        int options)
{
    axis2_dll_desc_impl_t *dll_desc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    dll_desc_impl = AXIS2_INTF_TO_IMPL(dll_desc);
    
    
    dll_desc_impl->load_options = options;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_dll_desc_set_type (axis2_dll_desc_t *dll_desc,
                        const axis2_env_t *env,
                        axis2_dll_type_t type)
{
    axis2_dll_desc_impl_t *dll_desc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    dll_desc_impl = AXIS2_INTF_TO_IMPL(dll_desc);
    
    
    dll_desc_impl->dll_type = type;
    return AXIS2_SUCCESS;
}

axis2_dll_type_t AXIS2_CALL
axis2_dll_desc_get_type (axis2_dll_desc_t *dll_desc,
                        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(dll_desc)->dll_type;
}

int AXIS2_CALL
axis2_dll_desc_get_load_options(axis2_dll_desc_t *dll_desc,
                        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(dll_desc)->load_options;
}

axis2_status_t AXIS2_CALL
axis2_dll_desc_set_dl_handler(axis2_dll_desc_t *dll_desc,
                        const axis2_env_t *env,
                        AXIS2_DLHANDLER dl_handler)
{
    axis2_dll_desc_impl_t *dll_desc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, dl_handler, AXIS2_FAILURE);
    
    dll_desc_impl = AXIS2_INTF_TO_IMPL(dll_desc);
    
    if(dll_desc_impl->dl_handler)
    {
        AXIS2_FREE(env->allocator, dll_desc_impl->dl_handler);
        dll_desc_impl->dl_handler = NULL;
    }
    dll_desc_impl->dl_handler = dl_handler;
    return AXIS2_SUCCESS;
}

AXIS2_DLHANDLER AXIS2_CALL
axis2_dll_desc_get_dl_handler(axis2_dll_desc_t *dll_desc,
                        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(dll_desc)->dl_handler;
}

axis2_status_t AXIS2_CALL
axis2_dll_desc_set_create_funct(axis2_dll_desc_t *dll_desc,
                        const axis2_env_t *env,
                        CREATE_FUNCT funct)
{
    axis2_dll_desc_impl_t *dll_desc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    dll_desc_impl = AXIS2_INTF_TO_IMPL(dll_desc);
    
    
    dll_desc_impl->create_funct = funct;
    return AXIS2_SUCCESS;
}

CREATE_FUNCT AXIS2_CALL
axis2_dll_desc_get_create_funct(axis2_dll_desc_t *dll_desc,
                        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(dll_desc)->create_funct;
}

axis2_status_t AXIS2_CALL
axis2_dll_desc_set_delete_funct(axis2_dll_desc_t *dll_desc,
                        const axis2_env_t *env,
                        DELETE_FUNCT funct)
{
    axis2_dll_desc_impl_t *dll_desc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    dll_desc_impl = AXIS2_INTF_TO_IMPL(dll_desc);
    
    
    dll_desc_impl->delete_funct = funct;
    return AXIS2_SUCCESS;
}

DELETE_FUNCT AXIS2_CALL
axis2_dll_desc_get_delete_funct(axis2_dll_desc_t *dll_desc,
                        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(dll_desc)->delete_funct;
}

axis2_status_t AXIS2_CALL
axis2_dll_desc_set_timestamp (axis2_dll_desc_t *dll_desc,
                                const axis2_env_t *env,
                                AXIS2_TIME_T timestamp)
{
    axis2_dll_desc_impl_t *dll_desc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    dll_desc_impl = AXIS2_INTF_TO_IMPL(dll_desc);
   
    dll_desc_impl->timestamp = timestamp;
    return AXIS2_SUCCESS;
}

AXIS2_TIME_T AXIS2_CALL
axis2_dll_desc_get_timestamp (axis2_dll_desc_t *dll_desc,
                                const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(dll_desc)->timestamp;
}

axis2_status_t AXIS2_CALL
axis2_dll_desc_set_error_code (
        axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env,
        axis2_error_codes_t error_code)
{
    axis2_dll_desc_impl_t *dll_desc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    dll_desc_impl = AXIS2_INTF_TO_IMPL(dll_desc);

    dll_desc_impl->error_code = error_code; 

    return AXIS2_SUCCESS;
}

axis2_error_codes_t AXIS2_CALL
axis2_dll_desc_get_error_code (
        axis2_dll_desc_t *dll_desc,
        const axis2_env_t *env)
{
    axis2_dll_desc_impl_t *dll_desc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    dll_desc_impl = AXIS2_INTF_TO_IMPL(dll_desc);

    return dll_desc_impl->error_code; 
}

axis2_char_t* AXIS2_CALL
axis2_dll_desc_create_platform_specific_dll_name(axis2_dll_desc_t *dll_desc,
                                                    const axis2_env_t *env,
                                                    axis2_char_t *class_name)
{
    axis2_dll_desc_impl_t *dll_desc_impl = NULL;
    axis2_char_t *temp_name = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    dll_desc_impl = AXIS2_INTF_TO_IMPL(dll_desc);
    
    temp_name = AXIS2_STRACAT(AXIS2_LIB_PREFIX, class_name, env);
    dll_desc_impl->dll_name = AXIS2_STRACAT(temp_name, AXIS2_LIB_SUFFIX, env);
    AXIS2_FREE(env->allocator, temp_name);
    return dll_desc_impl->dll_name;
}
