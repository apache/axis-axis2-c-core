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

#include <woden_wsdl_obj.h>
#include <woden_include.h>
#include <woden_xml_attr.h>
#include <woden_documentation_element.h>
#include <woden_generic_obj.h>
#include <woden_documentable.h>

typedef struct woden_include_impl woden_include_impl_t;

/** 
 * @brief Feature Struct Impl
 *   Axis2 Feature  
 */ 
struct woden_include_impl
{
    woden_include_t include;
    woden_obj_types_t obj_type;
    woden_wsdl_ref_t *wsdl_ref;
    axis2_hash_t *super;
    axis2_hash_t *methods;
};

#define INTF_TO_IMPL(include) ((woden_include_impl_t *) include)

axis2_status_t AXIS2_CALL 
woden_include_free(
        void *include,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
woden_include_super_objs(
        void *include,
        const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL 
woden_include_type(
        void *include,
        const axis2_env_t *env);

woden_wsdl_ref_t *AXIS2_CALL
woden_include_get_base_impl(
        void *include,
        const axis2_env_t *env);

static woden_include_t *
create(const axis2_env_t *env);

static axis2_status_t
woden_include_free_ops(
        void *include,
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_include_t * AXIS2_CALL
woden_include_to_include_element(
        void *include,
        const axis2_env_t *env)
{
    woden_include_impl_t *include_impl = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);
    if(!include)
    {
        include_impl = (woden_include_impl_t *) create(env);
    }
    else
        include_impl = (woden_include_impl_t *) include;

    woden_include_free_ops(include, env);

    include_impl->include.base.include_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_include_element_ops_t));
    woden_include_element_resolve_methods(&(include_impl->include.base.
            include_element), env, include_impl->methods);
    return include;
}

AXIS2_EXTERN woden_include_t * AXIS2_CALL
woden_include_to_wsdl_ref(
        void *include,
        const axis2_env_t *env)
{
    woden_include_impl_t *include_impl = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);
    if(!include)
    {
        include_impl = (woden_include_impl_t *) create(env);
    }
    else
        include_impl = (woden_include_impl_t *) include;
    woden_include_free_ops(include, env);

    include_impl->include.base.wsdl_ref.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_wsdl_ref_ops_t));
    woden_documentable_resolve_methods((woden_documentable_t *)&(include_impl->include.base.
            wsdl_ref), env, (woden_documentable_t *)include_impl->wsdl_ref, 
            include_impl->methods);
    return include;
}

AXIS2_EXTERN woden_include_t * AXIS2_CALL
woden_include_to_attr_extensible(
        void *include,
        const axis2_env_t *env)
{
    woden_include_impl_t *include_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if(!include)
    {
        include_impl = (woden_include_impl_t *) create(env);
    }
    else
        include_impl = (woden_include_impl_t *) include;
    woden_include_free_ops(include, env);

    include_impl->include.base.wsdl_ref.wsdl_element.base.
            attr_extensible.ops =
            AXIS2_MALLOC(env->allocator, 
            sizeof(woden_attr_extensible_ops_t));
    woden_attr_extensible_resolve_methods(&(include_impl->include.base.
            wsdl_ref.wsdl_element.base.attr_extensible), 
            env, NULL, include_impl->methods);
    return include;

}

AXIS2_EXTERN woden_include_t * AXIS2_CALL
woden_include_to_element_extensible(
        void *include,
        const axis2_env_t *env)
{
    woden_include_impl_t *include_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if(!include)
    {
        include_impl = (woden_include_impl_t *) create(env);
    }
    else
        include_impl = (woden_include_impl_t *) include;
    woden_include_free_ops(include, env);
    include_impl->include.base.wsdl_ref.wsdl_element.
                base.element_extensible.ops = AXIS2_MALLOC(env->allocator, 
                sizeof(woden_element_extensible_ops_t));
    woden_element_extensible_resolve_methods(&(include_impl->include.base.
            wsdl_ref.wsdl_element.base.element_extensible), 
            env, NULL, include_impl->methods);
    return include;

}

/************************End of Woden C Internal Methods***********************/
static woden_include_t *
create(const axis2_env_t *env)
{
    woden_include_impl_t *include_impl = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);
    include_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_include_impl_t));

    include_impl->obj_type= WODEN_INCLUDE;
    include_impl->super = NULL;
    include_impl->methods = NULL;
    
    include_impl->include.base.include_element.ops = NULL;
    include_impl->include.base.wsdl_ref.ops = NULL;
    include_impl->include.base.wsdl_ref.wsdl_element.base.attr_extensible.ops = NULL;
    include_impl->include.base.wsdl_ref.wsdl_element.base.element_extensible.ops = NULL;
    
    include_impl->include.ops = AXIS2_MALLOC(env->allocator, 
            sizeof(woden_include_ops_t));

    include_impl->include.ops->free = woden_include_free;
    include_impl->include.ops->super_objs = woden_include_super_objs;
    include_impl->include.ops->type = woden_include_type;
    include_impl->include.ops->get_base_impl = woden_include_get_base_impl;

    include_impl->methods = axis2_hash_make(env);
    if(!include_impl->methods) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(include_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            woden_include_free);
    axis2_hash_set(include_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING, 
            woden_include_super_objs);
    axis2_hash_set(include_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, woden_include_type);

    return &(include_impl->include);
}

AXIS2_EXTERN woden_include_t * AXIS2_CALL
woden_include_create(const axis2_env_t *env)
{
    woden_include_impl_t *include_impl = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);
    include_impl = (woden_include_impl_t *) create(env);

    include_impl->wsdl_ref = woden_wsdl_ref_create(env);

    include_impl->super = axis2_hash_make(env);
    if(!include_impl->super) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(include_impl->super, "WODEN_INCLUDE", 
            AXIS2_HASH_KEY_STRING, &(include_impl->include));
    axis2_hash_set(include_impl->super, "WODEN_WSDL_REF", 
            AXIS2_HASH_KEY_STRING, include_impl->wsdl_ref);
    
    return &(include_impl->include);
}

static axis2_status_t
woden_include_free_ops(
        void *include,
        const axis2_env_t *env)
{
    woden_include_impl_t *include_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    include_impl = INTF_TO_IMPL(include);

    if(include_impl->include.base.include_element.ops)
    {
        AXIS2_FREE(env->allocator, include_impl->include.base.
                include_element.ops);
        include_impl->include.base.include_element.ops = NULL;
    }

    if(include_impl->include.base.wsdl_ref.ops)
    {
        AXIS2_FREE(env->allocator, include_impl->include.base.wsdl_ref.ops);
        include_impl->include.base.wsdl_ref.ops = NULL;
    }

    if(include_impl->include.base.wsdl_ref.wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, include_impl->include.base.wsdl_ref.
                wsdl_element.base.attr_extensible.ops);
        include_impl->include.base.wsdl_ref.wsdl_element.
                base.attr_extensible.ops = NULL;
    }

    if(include_impl->include.base.wsdl_ref.
            wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, include_impl->include.base.wsdl_ref.
                wsdl_element.base.element_extensible.ops);
        include_impl->include.base.wsdl_ref.wsdl_element.
                base.element_extensible.ops = NULL;
    }
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_include_free(void *include,
                        const axis2_env_t *env)
{
    woden_include_impl_t *include_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    include_impl = INTF_TO_IMPL(include);

    if(include_impl->super)
    {
        axis2_hash_free(include_impl->super, env);
        include_impl->super = NULL;
    }
    
    if(include_impl->methods)
    {
        axis2_hash_free(include_impl->methods, env);
        include_impl->methods = NULL;
    }

    if(include_impl->wsdl_ref)
    {
        WODEN_WSDL_REF_FREE(include_impl->wsdl_ref, env);
        include_impl->wsdl_ref = NULL;
    }

    woden_include_free_ops(include, env);

    if((&(include_impl->include))->ops)
    {
        AXIS2_FREE(env->allocator, (&(include_impl->include))->ops);
        (&(include_impl->include))->ops = NULL;
    }
    
    if(include_impl)
    {
        AXIS2_FREE(env->allocator, include_impl);
        include_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_include_super_objs(
        void *include,
        const axis2_env_t *env)
{
    woden_include_impl_t *include_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    include_impl = INTF_TO_IMPL(include);

    return include_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_include_type(
        void *include,
        const axis2_env_t *env)
{
    woden_include_impl_t *include_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    include_impl = INTF_TO_IMPL(include);

    return include_impl->obj_type;
}

woden_wsdl_ref_t *AXIS2_CALL
woden_include_get_base_impl(void *include,
                                const axis2_env_t *env)
{
    woden_include_impl_t *include_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    include_impl = INTF_TO_IMPL(include);

    return include_impl->wsdl_ref;
}

axis2_status_t AXIS2_CALL
woden_include_resolve_methods(
        woden_include_t *include,
        const axis2_env_t *env,
        woden_include_t *include_impl,
        axis2_hash_t *methods)
{
    woden_include_impl_t *include_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    include_impl_l = INTF_TO_IMPL(include_impl);
    
    include->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    include->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    include->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;
}

