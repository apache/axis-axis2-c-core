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

#include <woden/axis2_woden_wsdl_obj.h>
#include <woden/wsdl20/extensions/axis2_woden_component_exts.h>

typedef struct axis2_woden_wsdl_obj_impl axis2_woden_wsdl_obj_impl_t;

/** 
 * @brief Wsdl Object Struct Impl
 *	Axis2 Wsdl Object  
 */ 
struct axis2_woden_wsdl_obj_impl
{
    axis2_woden_wsdl_obj_t wsdl_obj;
    axis2_woden_wsdl_element_t *wsdl_element;
    axis2_woden_obj_types_t obj_type;
    axis2_hash_t *f_comp_exts;
};

#define INTF_TO_IMPL(wsdl_obj) ((axis2_woden_wsdl_obj_impl_t *) wsdl_obj)

axis2_status_t AXIS2_CALL 
axis2_woden_wsdl_obj_free(
        void *wsdl_obj,
        axis2_env_t **env);

axis2_woden_wsdl_element_t *AXIS2_CALL
axis2_woden_wsdl_obj_get_base_impl(void *wsdl_obj,
                                axis2_env_t **env);

axis2_woden_component_exts_t *AXIS2_CALL
axis2_woden_wsdl_obj_get_component_exts_for_namespace(void *wsdl_obj,
                                                        axis2_env_t **env,
                                                        axis2_url_t *namespc);

axis2_status_t AXIS2_CALL
axis2_woden_wsdl_obj_set_component_exts(void *wsdl_obj,
                                        axis2_env_t **env,
                                        axis2_url_t *namespc,
                                        axis2_woden_component_exts_t *exts);

static axis2_woden_wsdl_obj_t *
create(axis2_env_t **env)
{
    axis2_woden_wsdl_obj_impl_t *wsdl_obj_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_obj_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_wsdl_obj_impl_t));

    wsdl_obj_impl->wsdl_element = NULL;
    wsdl_obj_impl->f_comp_exts = NULL;
   
    wsdl_obj_impl->wsdl_obj.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_wsdl_obj_ops_t));
    wsdl_obj_impl->f_comp_exts = axis2_hash_make(env);
    if(!wsdl_obj_impl->f_comp_exts)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    wsdl_obj_impl->wsdl_obj.ops->free = axis2_woden_wsdl_obj_free;
    wsdl_obj_impl->wsdl_obj.ops->get_base_impl = axis2_woden_wsdl_obj_get_base_impl;
    wsdl_obj_impl->wsdl_obj.ops->get_component_exts_for_namespace = 
        axis2_woden_wsdl_obj_get_component_exts_for_namespace;
    wsdl_obj_impl->wsdl_obj.ops->set_component_exts = 
        axis2_woden_wsdl_obj_set_component_exts;

    return &(wsdl_obj_impl->wsdl_obj);
}

AXIS2_DECLARE(axis2_woden_wsdl_obj_t *)
axis2_woden_wsdl_obj_create(axis2_env_t **env)
{
    axis2_woden_wsdl_obj_impl_t *wsdl_obj_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_obj_impl = (axis2_woden_wsdl_obj_impl_t *) create(env);

    wsdl_obj_impl->wsdl_element = axis2_woden_wsdl_element_create(env);

    return &(wsdl_obj_impl->wsdl_obj);
}

axis2_status_t AXIS2_CALL
axis2_woden_wsdl_obj_free(void *wsdl_obj,
                        axis2_env_t **env)
{
    axis2_woden_wsdl_obj_impl_t *wsdl_obj_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_obj_impl = INTF_TO_IMPL(wsdl_obj);

    if(wsdl_obj_impl->wsdl_element)
    {
        AXIS2_WODEN_WSDL_ELEMENT_FREE(wsdl_obj_impl->wsdl_element, env);
        wsdl_obj_impl->wsdl_element = NULL;
    }
    
    if((&(wsdl_obj_impl->wsdl_obj))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(wsdl_obj_impl->wsdl_obj))->ops);
        (&(wsdl_obj_impl->wsdl_obj))->ops = NULL;
    }

    if(wsdl_obj_impl)
    {
        AXIS2_FREE((*env)->allocator, wsdl_obj_impl);
        wsdl_obj_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_woden_wsdl_element_t *AXIS2_CALL
axis2_woden_wsdl_obj_get_base_impl(void *wsdl_obj,
                                axis2_env_t **env)
{
    axis2_woden_wsdl_obj_impl_t *wsdl_obj_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    wsdl_obj_impl = INTF_TO_IMPL(wsdl_obj);

    return wsdl_obj_impl->wsdl_element;
}

axis2_status_t AXIS2_CALL
axis2_woden_wsdl_obj_resolve_methods(axis2_woden_wsdl_obj_t *wsdl_obj,
                                axis2_env_t **env,
                                axis2_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    wsdl_obj->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    wsdl_obj->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    wsdl_obj->ops->get_component_exts_for_namespace = axis2_hash_get(methods,
            "get_component_exts_for_namespace", AXIS2_HASH_KEY_STRING);
    wsdl_obj->ops->set_component_exts = axis2_hash_get(methods,
            "set_component_exts", AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;
}

axis2_woden_component_exts_t *AXIS2_CALL
axis2_woden_wsdl_obj_get_component_exts_for_namespace(void *wsdl_obj,
                                                        axis2_env_t **env,
                                                        axis2_url_t *namespc)
{
    axis2_woden_wsdl_obj_impl_t *wsdl_obj_impl = NULL;
    axis2_char_t *str_namespc = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_obj_impl = INTF_TO_IMPL(wsdl_obj);

    str_namespc = AXIS2_URL_TO_EXTERNAL_FORM(namespc, env);
    return (axis2_woden_component_exts_t *)axis2_hash_get(wsdl_obj_impl->
            f_comp_exts, str_namespc, AXIS2_HASH_KEY_STRING);
}

axis2_status_t AXIS2_CALL
axis2_woden_wsdl_obj_set_component_exts(void *wsdl_obj,
                                        axis2_env_t **env,
                                        axis2_url_t *namespc,
                                        axis2_woden_component_exts_t *exts)
{
    axis2_woden_wsdl_obj_impl_t *wsdl_obj_impl = NULL;
    axis2_char_t *str_namespc;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_obj_impl = INTF_TO_IMPL(wsdl_obj);

    if(exts)
    {
        axis2_hash_set(wsdl_obj_impl->f_comp_exts, str_namespc, 
                AXIS2_HASH_KEY_STRING, exts);
    }
    else
    {
        axis2_hash_set(wsdl_obj_impl->f_comp_exts, str_namespc, 
                AXIS2_HASH_KEY_STRING, NULL);
    }
    return AXIS2_SUCCESS;
}

