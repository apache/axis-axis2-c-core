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

#include <woden_documentation.h>
#include <woden_documentation_element.h>
#include <woden_ext_element.h>
#include <woden_xml_attr.h>
#include <woden_wsdl_obj.h>

typedef struct woden_documentation_impl woden_documentation_impl_t;

/** 
 * @brief Documentation Struct Impl
 *   Axis2 Documentation  
 */ 
struct woden_documentation_impl
{
    woden_documentation_t documentation;
    woden_obj_types_t obj_type;
    woden_wsdl_element_t *wsdl_element;
    axis2_hash_t *super;
    axis2_hash_t *methods;

    void *f_content;
};

#define INTF_TO_IMPL(documentation) ((woden_documentation_impl_t *) documentation)

axis2_status_t AXIS2_CALL 
woden_documentation_free(
        void *documentation,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
woden_documentation_super_objs(
        void *documentation,
        const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_documentation_type(
        void *documentation,
        const axis2_env_t *env);

woden_wsdl_element_t *AXIS2_CALL
woden_documentation_get_base_impl(
        void *documentation,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
woden_documentation_set_content(
        void *documentation,
        const axis2_env_t *env,
        void *doc_el);

void *AXIS2_CALL
woden_documentation_get_content(
        void *documentation,
        const axis2_env_t *env);

static woden_documentation_t *
create(const axis2_env_t *env);

static axis2_status_t
woden_documentation_free_ops(
        void *documentation,
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_documentation_t * AXIS2_CALL
woden_documentation_to_documentation_element(
        void *documentation,
        const axis2_env_t *env)
{
    woden_documentation_impl_t *documentation_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!documentation)
    {
        documentation_impl = (woden_documentation_impl_t *) create(env);
    }
    else
        documentation_impl = (woden_documentation_impl_t *) documentation;

    woden_documentation_free_ops(documentation, env);

    documentation_impl->documentation.base.documentation_element.ops = 
        AXIS2_MALLOC(env->allocator, 
        sizeof(woden_documentation_element_ops_t));
    woden_documentation_element_resolve_methods(&(documentation_impl->
            documentation.base.documentation_element), env, 
            documentation_impl->methods);
    return documentation;
}

AXIS2_EXTERN woden_documentation_t * AXIS2_CALL
woden_documentation_to_attr_extensible(
        void *documentation,
        const axis2_env_t *env)
{
    woden_documentation_impl_t *documentation_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!documentation)
    {
        documentation_impl = (woden_documentation_impl_t *) create(env);
    }
    else
        documentation_impl = (woden_documentation_impl_t *) documentation;

    woden_documentation_free_ops(documentation, env);

    documentation_impl->documentation.base.wsdl_element.base.attr_extensible.
            ops = AXIS2_MALLOC(env->allocator, 
            sizeof(woden_attr_extensible_ops_t));
    woden_attr_extensible_resolve_methods(&(documentation_impl->
            documentation.base.wsdl_element.base.attr_extensible), env, 
            NULL, documentation_impl->methods);
    return documentation;

}

AXIS2_EXTERN woden_documentation_t * AXIS2_CALL
woden_documentation_to_element_extensible(
        void *documentation,
        const axis2_env_t *env)
{
    woden_documentation_impl_t *documentation_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!documentation)
    {
        documentation_impl = (woden_documentation_impl_t *) create(env);
    }
    else
        documentation_impl = (woden_documentation_impl_t *) documentation;

    woden_documentation_free_ops(documentation, env);

    documentation_impl->documentation.base.wsdl_element.base.
        element_extensible.ops = AXIS2_MALLOC(env->allocator, 
        sizeof(woden_element_extensible_ops_t));
    woden_element_extensible_resolve_methods(&(documentation_impl->
            documentation.base.wsdl_element.base.element_extensible), 
            env, NULL, documentation_impl->methods);
    return documentation;

}

/************************End of Woden C Internal Methods***********************/
static woden_documentation_t *
create(
        const axis2_env_t *env)
{
    woden_documentation_impl_t *documentation_impl = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);
    documentation_impl = AXIS2_MALLOC(env->allocator, sizeof(
                woden_documentation_impl_t));

    documentation_impl->wsdl_element = NULL;
    documentation_impl->super = NULL;
    documentation_impl->obj_type = WODEN_DOCUMENTATION;
    documentation_impl->f_content = NULL;
    documentation_impl->methods = NULL;
    
    documentation_impl->documentation.base.documentation_element.ops = NULL; 
    documentation_impl->documentation.base.wsdl_element.base.attr_extensible.
        ops = NULL; 
    documentation_impl->documentation.base.wsdl_element.base.element_extensible.
        ops = NULL; 
   
    documentation_impl->documentation.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_documentation_ops_t));

    documentation_impl->documentation.ops->free = woden_documentation_free;
    documentation_impl->documentation.ops->super_objs = 
        woden_documentation_super_objs;
    documentation_impl->documentation.ops->type = woden_documentation_type;
    documentation_impl->documentation.ops->get_base_impl = 
        woden_documentation_get_base_impl;

    documentation_impl->methods = axis2_hash_make(env);
    if(!documentation_impl->methods) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(documentation_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            woden_documentation_free);
    axis2_hash_set(documentation_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING, 
            woden_documentation_super_objs);
    axis2_hash_set(documentation_impl->methods, "type", AXIS2_HASH_KEY_STRING, 
            woden_documentation_type);
    axis2_hash_set(documentation_impl->methods, "set_content", 
            AXIS2_HASH_KEY_STRING, woden_documentation_set_content);
    axis2_hash_set(documentation_impl->methods, "get_content", 
            AXIS2_HASH_KEY_STRING, woden_documentation_get_content);
    
    return &(documentation_impl->documentation);
}

AXIS2_EXTERN woden_documentation_t * AXIS2_CALL
woden_documentation_create(
        const axis2_env_t *env)
{
    woden_documentation_impl_t *documentation_impl = NULL;
   
    documentation_impl = (woden_documentation_impl_t *) create(env);

    documentation_impl->wsdl_element = woden_wsdl_element_create(env);

    documentation_impl->super = axis2_hash_make(env);
    if(!documentation_impl->super) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(documentation_impl->super, "WODEN_DOCUMENTATION", 
            AXIS2_HASH_KEY_STRING, &(documentation_impl->documentation));
    axis2_hash_set(documentation_impl->super, "WODEN_WSDL_ELEMENT", 
            AXIS2_HASH_KEY_STRING, documentation_impl->wsdl_element);

    return &(documentation_impl->documentation);
}

static axis2_status_t
woden_documentation_free_ops(
        void *documentation,
        const axis2_env_t *env)
{
    woden_documentation_impl_t *documentation_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentation_impl = INTF_TO_IMPL(documentation);

    if(documentation_impl->documentation.base.documentation_element.ops)
    {
        AXIS2_FREE(env->allocator, documentation_impl->documentation.base.
                documentation_element.ops);
        documentation_impl->documentation.base.documentation_element.ops = 
            NULL;
    }

    if(documentation_impl->documentation.base.wsdl_element.base.attr_extensible.
            ops)
    {
        AXIS2_FREE(env->allocator, documentation_impl->documentation.
                base.wsdl_element.base.attr_extensible.ops);
        documentation_impl->documentation.base.wsdl_element.base.
            attr_extensible.ops = NULL;
    }
    
    if(documentation_impl->documentation.base.wsdl_element.base.
            element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, documentation_impl->documentation.base.
                wsdl_element.base.element_extensible.ops);
        documentation_impl->documentation.base.wsdl_element.base.
            element_extensible.ops = NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_documentation_free(
        void *documentation,
        const axis2_env_t *env)
{
    woden_documentation_impl_t *documentation_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentation_impl = INTF_TO_IMPL(documentation);

    if(documentation_impl->f_content)
    {
        /*AXIS2_FREE(documentation_impl->f_content, env);*/
        documentation_impl->f_content = NULL;
    }
    
    if(documentation_impl->super)
    {
        axis2_hash_free(documentation_impl->super, env);
        documentation_impl->super = NULL;
    }
    
    if(documentation_impl->methods)
    {
        axis2_hash_free(documentation_impl->methods, env);
        documentation_impl->methods = NULL;
    }

    if(documentation_impl->wsdl_element)
    {
        WODEN_WSDL_OBJ_FREE(documentation_impl->wsdl_element, env);
        documentation_impl->wsdl_element = NULL;
    }

    woden_documentation_free_ops(documentation, env);

    if((&(documentation_impl->documentation))->ops)
    {
        AXIS2_FREE(env->allocator, documentation_impl->documentation.ops);
        (&(documentation_impl->documentation))->ops = NULL;
    }
    
    if(documentation_impl)
    {
        AXIS2_FREE(env->allocator, documentation_impl);
        documentation_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_documentation_super_objs(
        void *documentation,
        const axis2_env_t *env)
{
    woden_documentation_impl_t *documentation_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentation_impl = INTF_TO_IMPL(documentation);

    return documentation_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_documentation_type(void *documentation,
        const axis2_env_t *env)
{
    woden_documentation_impl_t *documentation_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentation_impl = INTF_TO_IMPL(documentation);

    return documentation_impl->obj_type;
}

woden_wsdl_element_t *AXIS2_CALL
woden_documentation_get_base_impl(
        void *documentation,
        const axis2_env_t *env)
{
    woden_documentation_impl_t *documentation_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    documentation_impl = INTF_TO_IMPL(documentation);

    return documentation_impl->wsdl_element;
}

axis2_status_t AXIS2_CALL
woden_documentation_resolve_methods(
        woden_documentation_t *documentation,
        const axis2_env_t *env,
        woden_documentation_t *documentation_impl,
        axis2_hash_t *methods)
{
    woden_documentation_impl_t *documentation_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    documentation_impl_l = INTF_TO_IMPL (documentation_impl);
    
    documentation->ops = AXIS2_MALLOC(env->allocator, 
                sizeof(woden_documentation_ops_t));
    
    documentation->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    documentation->ops->super_objs = axis2_hash_get(methods, 
            "super_objs", AXIS2_HASH_KEY_STRING);
    documentation->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_documentation_set_content(
        void *documentation,
        const axis2_env_t *env,
        void *doc_el)
{
    woden_documentation_impl_t *documentation_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, documentation, AXIS2_FAILURE);
    super = WODEN_DOCUMENTATION_SUPER_OBJS(documentation, env);
    documentation_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_DOCUMENTATION", AXIS2_HASH_KEY_STRING));
   
    if(documentation_impl->f_content)
    {
        /* Free f_content */
    }
    documentation_impl->f_content = doc_el;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
woden_documentation_get_content(
        void *documentation,
        const axis2_env_t *env)
{
    woden_documentation_impl_t *documentation_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_DOCUMENTATION_SUPER_OBJS(documentation, env);
    documentation_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_DOCUMENTATION", AXIS2_HASH_KEY_STRING));

    return documentation_impl->f_content;
}

