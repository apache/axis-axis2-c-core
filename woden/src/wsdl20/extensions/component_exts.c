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
 
#include <woden/wsdl20/extensions/axis2_woden_component_exts.h>
#include <woden/wsdl20/xml/axis2_woden_wsdl_element.h>
#include <axis2_url.h>
#include <axis2_hash.h>

typedef struct axis2_woden_component_exts_impl axis2_woden_component_exts_impl_t;

/** 
 * @brief Component Extensions Struct Impl
 *	Axis2 Component Extensions  
 */ 
struct axis2_woden_component_exts_impl
{
    axis2_woden_component_exts_t component_exts;
    void *f_parent_element;
    axis2_url_t *f_namespc;
};

#define INTF_TO_IMPL(component_exts) ((axis2_woden_component_exts_impl_t *) component_exts)

axis2_status_t AXIS2_CALL 
axis2_woden_component_exts_free(
        void *component_exts,
        const axis2_env_t *envv);

axis2_url_t *AXIS2_CALL
axis2_woden_component_exts_get_namespace(
        void *component_exts,
        const axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_component_exts_get_parent_element(
        void *component_exts,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_woden_component_exts_init(
        void *component_exts,
        const axis2_env_t *env,
        axis2_woden_wsdl_element_t *parent_el,
        axis2_url_t *namespc);
 
AXIS2_EXTERN axis2_woden_component_exts_t * AXIS2_CALL
axis2_woden_component_exts_create(
        const axis2_env_t *env)
{
    axis2_woden_component_exts_impl_t *component_exts_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    component_exts_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_woden_component_exts_impl_t));

    component_exts_impl->f_parent_element = NULL;
    component_exts_impl->f_namespc = NULL;

    component_exts_impl->component_exts.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_woden_component_exts_ops_t)); 
    
    component_exts_impl->component_exts.ops->free = 
            axis2_woden_component_exts_free;
    component_exts_impl->component_exts.ops->init = 
            axis2_woden_component_exts_init;
    component_exts_impl->component_exts.ops->get_namespace = 
            axis2_woden_component_exts_get_namespace;
    component_exts_impl->component_exts.ops->get_parent_element = 
            axis2_woden_component_exts_get_parent_element;
    
    return &(component_exts_impl->component_exts);
}

axis2_status_t AXIS2_CALL
axis2_woden_component_exts_free(
        void *component_exts,
        const axis2_env_t *env)
{
    axis2_woden_component_exts_impl_t *component_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    component_exts_impl = INTF_TO_IMPL(component_exts);

    if(component_exts_impl->f_parent_element)
    {
        AXIS2_WODEN_ELEMENT_FREE(component_exts_impl->f_parent_element, env);
        component_exts_impl->f_parent_element = NULL;
    }
    
    if(component_exts_impl->f_namespc)
    {
        AXIS2_URL_FREE(component_exts_impl->f_namespc, env);
        component_exts_impl->f_namespc = NULL;
    }
    
    if((&(component_exts_impl->component_exts))->ops)
    {
        AXIS2_FREE(env->allocator, (&(component_exts_impl->component_exts))->ops);
        (&(component_exts_impl->component_exts))->ops = NULL;
    }

    if(component_exts_impl)
    {
        AXIS2_FREE(env->allocator, component_exts_impl);
        component_exts_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_component_exts_resolve_methods(
        axis2_woden_component_exts_t *component_exts,
        const axis2_env_t *env,
        axis2_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    
    component_exts->ops = AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_woden_component_exts_ops_t));
    component_exts->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    component_exts->ops->get_parent_element = axis2_hash_get(methods, 
            "get_parent_element", AXIS2_HASH_KEY_STRING);
    component_exts->ops->to_component_exts_free = axis2_hash_get(methods, 
            "to_component_exts_free", AXIS2_HASH_KEY_STRING);
    component_exts->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    component_exts->ops->init = axis2_hash_get(methods, 
            "init", AXIS2_HASH_KEY_STRING); ;
    component_exts->ops->get_namespace = axis2_hash_get(methods, 
            "get_namespace", AXIS2_HASH_KEY_STRING); 

    return AXIS2_SUCCESS;    
}

axis2_url_t *AXIS2_CALL
axis2_woden_component_exts_get_namespace(
        void *component_exts,
        const axis2_env_t *env)
{
    axis2_woden_component_exts_impl_t *component_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    component_exts_impl = INTF_TO_IMPL(component_exts);
    return component_exts_impl->f_namespc;
}

void *AXIS2_CALL
axis2_woden_component_exts_get_parent_element(
        void *component_exts,
        const axis2_env_t *env)
{
    axis2_woden_component_exts_impl_t *component_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    component_exts_impl = INTF_TO_IMPL(component_exts);
    return component_exts_impl->f_parent_element;
}

axis2_status_t AXIS2_CALL
axis2_woden_component_exts_init(
        void *component_exts,
        const axis2_env_t *env,
        axis2_woden_wsdl_element_t *parent_el,
        axis2_url_t *namespc) 
{
    axis2_woden_component_exts_impl_t *component_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    component_exts_impl = INTF_TO_IMPL(component_exts);
    component_exts_impl->f_parent_element = parent_el;
    component_exts_impl->f_namespc = namespc;
    return AXIS2_SUCCESS;
}
