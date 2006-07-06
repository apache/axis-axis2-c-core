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
 
#include <woden_wsdl_element.h>
#include <woden_element_extensible.h>
#include <woden_attr_extensible.h>
#include <woden_ext_element.h>
#include <woden_xml_attr.h>
#include <axis2_uri.h>
#include <axis2_hash.h>

typedef struct woden_wsdl_element_impl woden_wsdl_element_impl_t;

/** 
 * @brief Wsdl Element Struct Impl
 *   Axis2 Wsdl Element  
 */ 
struct woden_wsdl_element_impl
{
    woden_wsdl_element_t wsdl_element;
    woden_obj_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    woden_attr_extensible_t *f_attr_ext;
    woden_element_extensible_t *f_elem_ext;
};

#define INTF_TO_IMPL(wsdl_element) \
    ((woden_wsdl_element_impl_t *) wsdl_element)

axis2_status_t AXIS2_CALL 
woden_wsdl_element_free(
        void *wsdl_element,
        const axis2_env_t *envv);

axis2_hash_t *AXIS2_CALL 
woden_wsdl_element_super_objs(
        void *wsdl_element,
        const axis2_env_t *envv);

woden_obj_types_t AXIS2_CALL 
woden_wsdl_element_type(
        void *wsdl_element,
        const axis2_env_t *envv);

axis2_status_t AXIS2_CALL 
woden_wsdl_element_set_ext_attr(
        void *wsdl_element,
        const axis2_env_t *env,
        axis2_qname_t *attr_type,
        woden_xml_attr_t *attr); 

void *AXIS2_CALL 
woden_wsdl_element_get_ext_attr(
        void *wsdl_element,
        const axis2_env_t *env,
        axis2_qname_t *attr_type); 

axis2_array_list_t *AXIS2_CALL 
woden_wsdl_element_get_ext_attrs(
        void *wsdl_element,
        const axis2_env_t *env); 

axis2_array_list_t *AXIS2_CALL 
woden_wsdl_element_get_ext_attrs_for_namespace(
        void *wsdl_element,
        const axis2_env_t *env,
        axis2_uri_t *namespc);

axis2_bool_t AXIS2_CALL 
woden_wsdl_element_has_ext_attrs_for_namespace(
        void *wsdl_element,
        const axis2_env_t *env,
        axis2_uri_t *namespc);

axis2_status_t AXIS2_CALL 
woden_wsdl_element_add_ext_element(
        void *wsdl_element,
        const axis2_env_t *env,
        woden_ext_element_t *ext_el); 

axis2_status_t AXIS2_CALL 
woden_wsdl_element_remove_ext_element(
        void *wsdl_element,
        const axis2_env_t *env,
        woden_ext_element_t *ext_el); 

axis2_array_list_t *AXIS2_CALL 
woden_wsdl_element_get_ext_elements(
        void *wsdl_element,
        const axis2_env_t *env); 

axis2_array_list_t *AXIS2_CALL 
woden_wsdl_element_get_ext_elements_of_type(
        void *wsdl_element,
        const axis2_env_t *env,
        axis2_qname_t *elem_type);

axis2_bool_t AXIS2_CALL 
woden_wsdl_element_has_ext_elements_for_namespace(
        void *wsdl_element,
        const axis2_env_t *env,
        axis2_uri_t *namespc);

axis2_status_t AXIS2_CALL 
woden_wsdl_element_add_ext_element(
        void *wsdl_element,
        const axis2_env_t *env,
woden_ext_element_t *ext_el); 

axis2_status_t AXIS2_CALL 
woden_wsdl_element_remove_ext_element(
        void *wsdl_element,
        const axis2_env_t *env,
        woden_ext_element_t *ext_el);

axis2_array_list_t *AXIS2_CALL 
woden_wsdl_element_get_ext_elements(
        void *wsdl_element,
        const axis2_env_t *env); 

axis2_array_list_t *AXIS2_CALL 
woden_wsdl_element_get_ext_elements_of_type(
        void *wsdl_element,
        const axis2_env_t *env,
        axis2_qname_t *ext_type);

axis2_bool_t AXIS2_CALL 
woden_wsdl_element_has_ext_elements_for_namespace(
        void *wsdl_element,
        const axis2_env_t *env,
        axis2_uri_t *namespc);

void *AXIS2_CALL 
woden_wsdl_element_get_element_extensible(
        void *wsdl_element,
        const axis2_env_t *env);

void *AXIS2_CALL 
woden_wsdl_element_get_attr_extensible(
        void *wsdl_element,
        const axis2_env_t *env);

static woden_wsdl_element_t *
create(
        const axis2_env_t *env);

static axis2_status_t
woden_wsdl_element_free_ops(
        void *wsdl_element,
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_wsdl_element_t * AXIS2_CALL
woden_wsdl_element_to_attr_extensible(
        void *wsdl_element,
        const axis2_env_t *env)
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if(!wsdl_element)
    {
        wsdl_element_impl = (woden_wsdl_element_impl_t *) create(env);
    }
    else
        wsdl_element_impl = (woden_wsdl_element_impl_t *) wsdl_element;

    woden_wsdl_element_free_ops(wsdl_element, env);

    wsdl_element_impl->wsdl_element.base.attr_extensible.ops = 
            AXIS2_MALLOC(env->allocator, 
            sizeof(woden_attr_extensible_ops_t));
    woden_attr_extensible_resolve_methods(&(wsdl_element_impl->
            wsdl_element.base.attr_extensible), env, wsdl_element_impl->
            f_attr_ext, wsdl_element_impl->methods);
    return wsdl_element;

}

AXIS2_EXTERN woden_wsdl_element_t * AXIS2_CALL
woden_wsdl_element_to_element_extensible(
        void *wsdl_element,
        const axis2_env_t *env)
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;


    AXIS2_ENV_CHECK(env, NULL);

    if(!wsdl_element)
    {
        wsdl_element_impl = (woden_wsdl_element_impl_t *) create(env);
    }
    else
        wsdl_element_impl = (woden_wsdl_element_impl_t *) wsdl_element;

    woden_wsdl_element_free_ops(wsdl_element, env);

    wsdl_element_impl->wsdl_element.base.element_extensible.ops = 
            AXIS2_MALLOC(env->allocator, 
            sizeof(woden_element_extensible_ops_t));
    woden_element_extensible_resolve_methods(&(wsdl_element_impl->
            wsdl_element.base.element_extensible), env, wsdl_element_impl->f_elem_ext, 
            wsdl_element_impl->methods);
    return wsdl_element;

}

/************************End of Woden C Internal Methods***********************/
static woden_wsdl_element_t *
create(
        const axis2_env_t *env)
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    wsdl_element_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_wsdl_element_impl_t));

    wsdl_element_impl->super = NULL;
    wsdl_element_impl->methods = NULL;
    wsdl_element_impl->obj_type = WODEN_WSDL_ELEMENT;
    wsdl_element_impl->f_attr_ext = NULL;
    wsdl_element_impl->f_elem_ext = NULL;

    wsdl_element_impl->wsdl_element.base.attr_extensible.ops = NULL;
    wsdl_element_impl->wsdl_element.base.element_extensible.ops = NULL;
    wsdl_element_impl->wsdl_element.ops = AXIS2_MALLOC(env->allocator, 
            sizeof(woden_wsdl_element_ops_t)); 
   
    wsdl_element_impl->wsdl_element.ops->free = woden_wsdl_element_free;
    wsdl_element_impl->wsdl_element.ops->super_objs = 
        woden_wsdl_element_super_objs;
    wsdl_element_impl->wsdl_element.ops->type = woden_wsdl_element_type;
    wsdl_element_impl->wsdl_element.ops->get_element_extensible = 
        woden_wsdl_element_get_element_extensible;
    wsdl_element_impl->wsdl_element.ops->get_attr_extensible = 
        woden_wsdl_element_get_attr_extensible;
        
    wsdl_element_impl->super = axis2_hash_make(env);
    if(!wsdl_element_impl->super) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(wsdl_element_impl->super, "WODEN_WSDL_ELEMENT", 
            AXIS2_HASH_KEY_STRING, &(wsdl_element_impl->wsdl_element));
    
    wsdl_element_impl->methods = axis2_hash_make(env);
    if(!wsdl_element_impl->methods) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(wsdl_element_impl->methods, "free", 
            AXIS2_HASH_KEY_STRING, woden_wsdl_element_free);
    axis2_hash_set(wsdl_element_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, woden_wsdl_element_super_objs);
    axis2_hash_set(wsdl_element_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, woden_wsdl_element_type);
    axis2_hash_set(wsdl_element_impl->methods, "set_ext_attr", 
            AXIS2_HASH_KEY_STRING, woden_wsdl_element_set_ext_attr);
    axis2_hash_set(wsdl_element_impl->methods, "get_ext_attr", 
            AXIS2_HASH_KEY_STRING, woden_wsdl_element_get_ext_attr);
    axis2_hash_set(wsdl_element_impl->methods, "get_ext_attrs", 
            AXIS2_HASH_KEY_STRING, woden_wsdl_element_get_ext_attrs);
    axis2_hash_set(wsdl_element_impl->methods, "get_ext_attrs_for_namespace", 
            AXIS2_HASH_KEY_STRING, 
            woden_wsdl_element_get_ext_attrs_for_namespace);
    axis2_hash_set(wsdl_element_impl->methods, "has_ext_attrs_for_namespace", 
            AXIS2_HASH_KEY_STRING, 
            woden_wsdl_element_has_ext_attrs_for_namespace);
    axis2_hash_set(wsdl_element_impl->methods, "add_ext_element", 
            AXIS2_HASH_KEY_STRING, woden_wsdl_element_add_ext_element);
    axis2_hash_set(wsdl_element_impl->methods, "remove_ext_element", 
            AXIS2_HASH_KEY_STRING, woden_wsdl_element_remove_ext_element);
    axis2_hash_set(wsdl_element_impl->methods, "get_ext_elements", 
            AXIS2_HASH_KEY_STRING, woden_wsdl_element_get_ext_elements);
    axis2_hash_set(wsdl_element_impl->methods, "get_ext_elements_of_type", 
            AXIS2_HASH_KEY_STRING, 
            woden_wsdl_element_get_ext_elements_of_type);
    axis2_hash_set(wsdl_element_impl->methods, "has_ext_elements_for_namespace", 
            AXIS2_HASH_KEY_STRING, 
            woden_wsdl_element_has_ext_elements_for_namespace);
    axis2_hash_set(wsdl_element_impl->methods, "get_element_extensible", 
            AXIS2_HASH_KEY_STRING, 
            woden_wsdl_element_get_element_extensible);
    axis2_hash_set(wsdl_element_impl->methods, "get_attr_extensible", 
            AXIS2_HASH_KEY_STRING, 
            woden_wsdl_element_get_attr_extensible);
    
    return &(wsdl_element_impl->wsdl_element);
}

AXIS2_EXTERN woden_wsdl_element_t * AXIS2_CALL
woden_wsdl_element_create(
        const axis2_env_t *env)
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    wsdl_element_impl = (woden_wsdl_element_impl_t *) create(env);

    wsdl_element_impl->f_attr_ext = woden_attr_extensible_create(env);
    if(!wsdl_element_impl->f_attr_ext)
    {
        return NULL;
    }
    wsdl_element_impl->f_elem_ext = woden_element_extensible_create(env);
    if(!wsdl_element_impl->f_elem_ext)
    {
        return NULL;
    }

    wsdl_element_impl->super = axis2_hash_make(env);
    if(!wsdl_element_impl->super) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(wsdl_element_impl->super, "WODEN_ELEMENT_EXTENSIBLE", 
            AXIS2_HASH_KEY_STRING, &(wsdl_element_impl->wsdl_element));
    axis2_hash_set(wsdl_element_impl->super, "WODEN_ATTR_EXTENSIBLE", 
            AXIS2_HASH_KEY_STRING, &(wsdl_element_impl->wsdl_element));
    axis2_hash_set(wsdl_element_impl->super, "WODEN_WSDL_ELEMENT", 
            AXIS2_HASH_KEY_STRING, &(wsdl_element_impl->wsdl_element));
 
    return &(wsdl_element_impl->wsdl_element);
}

static axis2_status_t
woden_wsdl_element_free_ops(
        void *wsdl_element,
        const axis2_env_t *env)
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_element_impl = INTF_TO_IMPL(wsdl_element);

    if(wsdl_element_impl->wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, wsdl_element_impl->wsdl_element.base.
                attr_extensible.ops);
        wsdl_element_impl->wsdl_element.base.attr_extensible.ops = NULL;
    }

    if(wsdl_element_impl->wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, wsdl_element_impl->wsdl_element.base.
                element_extensible.ops);
        wsdl_element_impl->wsdl_element.base.element_extensible.ops = NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_wsdl_element_free(
        void *wsdl_element,
        const axis2_env_t *env)
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_element_impl = INTF_TO_IMPL(wsdl_element);

    if(wsdl_element_impl->f_attr_ext)
    {
        WODEN_ATTR_EXTENSIBLE_FREE(wsdl_element_impl->f_attr_ext, env);
        wsdl_element_impl->f_attr_ext = NULL;
    }
    
    if(wsdl_element_impl->f_elem_ext)
    {
        WODEN_ELEMENT_EXTENSIBLE_FREE(wsdl_element_impl->f_elem_ext, env);
        wsdl_element_impl->f_elem_ext = NULL;
    }
    
    if(wsdl_element_impl->super)
    {
        axis2_hash_free(wsdl_element_impl->super, env);
        wsdl_element_impl->super = NULL;
    }
    
    if(wsdl_element_impl->methods)
    {
        axis2_hash_free(wsdl_element_impl->methods, env);
        wsdl_element_impl->methods = NULL;
    }

    woden_wsdl_element_free_ops(wsdl_element, env);

    if(wsdl_element_impl->wsdl_element.ops)
    {
        AXIS2_FREE(env->allocator, wsdl_element_impl->wsdl_element.ops);
        wsdl_element_impl->wsdl_element.ops = NULL;
    }

    if(wsdl_element_impl)
    {
        AXIS2_FREE(env->allocator, wsdl_element_impl);
        wsdl_element_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_wsdl_element_super_objs(
        void *wsdl_element,
        const axis2_env_t *env)
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    wsdl_element_impl = INTF_TO_IMPL(wsdl_element);
    
    return wsdl_element_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_wsdl_element_type(
        void *wsdl_element,
        const axis2_env_t *env)
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    wsdl_element_impl = INTF_TO_IMPL(wsdl_element);
    
    return wsdl_element_impl->obj_type;
}

axis2_status_t AXIS2_CALL
woden_wsdl_element_resolve_methods(
        woden_wsdl_element_t *wsdl_element,
        const axis2_env_t *env,
        woden_wsdl_element_t *wsdl_element_impl,
        axis2_hash_t *methods)
{
    woden_wsdl_element_impl_t *wsdl_element_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    wsdl_element_impl_l = INTF_TO_IMPL(wsdl_element);
    
    wsdl_element->ops = AXIS2_MALLOC(env->allocator, 
            sizeof(woden_wsdl_element_ops_t));
    wsdl_element->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    wsdl_element->ops->super_objs = axis2_hash_get(methods, 
            "super_objs", AXIS2_HASH_KEY_STRING);
    wsdl_element->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;    
}

axis2_status_t AXIS2_CALL 
woden_wsdl_element_set_ext_attr(
        void *wsdl_element,
        const axis2_env_t *env,
        axis2_qname_t *attr_type,
        woden_xml_attr_t *attr)
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, attr_type, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, attr, AXIS2_FAILURE);
    super = WODEN_WSDL_ELEMENT_SUPER_OBJS(wsdl_element, env);
    wsdl_element_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL_ELEMENT", AXIS2_HASH_KEY_STRING)); 

    return WODEN_ATTR_EXTENSIBLE_SET_EXT_ATTR(wsdl_element_impl->f_attr_ext, 
            env, attr_type, attr);    
}

void *AXIS2_CALL 
woden_wsdl_element_get_ext_attr(
        void *wsdl_element,
        const axis2_env_t *env,
        axis2_qname_t *attr_type) 
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, attr_type, NULL);
    super = WODEN_WSDL_ELEMENT_SUPER_OBJS(wsdl_element, env);
    wsdl_element_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL_ELEMENT", AXIS2_HASH_KEY_STRING)); 
    
    return WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTR(wsdl_element_impl->f_attr_ext, 
            env, attr_type);
}

axis2_array_list_t *AXIS2_CALL 
woden_wsdl_element_get_ext_attrs(
        void *wsdl_element,
        const axis2_env_t *env) 
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;
    axis2_hash_t *super = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_WSDL_ELEMENT_SUPER_OBJS(wsdl_element, env);
    wsdl_element_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL_ELEMENT", AXIS2_HASH_KEY_STRING)); 

    return WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTRS(wsdl_element_impl->f_attr_ext, 
            env);
}

axis2_array_list_t *AXIS2_CALL 
woden_wsdl_element_get_ext_attrs_for_namespace(
        void *wsdl_element,
        const axis2_env_t *env,
        axis2_uri_t *namespc) 
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;
    axis2_hash_t *super = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, namespc, NULL);
    super = WODEN_WSDL_ELEMENT_SUPER_OBJS(wsdl_element, env);
    wsdl_element_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL_ELEMENT", AXIS2_HASH_KEY_STRING)); 

    return WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTRS_FOR_NAMESPACE(
            wsdl_element_impl->f_attr_ext, env, namespc);
}

axis2_bool_t AXIS2_CALL 
woden_wsdl_element_has_ext_attrs_for_namespace(
        void *wsdl_element,
        const axis2_env_t *env,
        axis2_uri_t *namespc)
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, namespc, AXIS2_FAILURE);
    super = WODEN_WSDL_ELEMENT_SUPER_OBJS(wsdl_element, env);
    wsdl_element_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL_ELEMENT", AXIS2_HASH_KEY_STRING)); 

    return WODEN_ATTR_EXTENSIBLE_HAS_EXT_ATTRS_FOR_NAMESPACE(
            wsdl_element_impl->f_attr_ext, env, namespc);
}

axis2_status_t AXIS2_CALL 
woden_wsdl_element_add_ext_element(
        void *wsdl_element,
        const axis2_env_t *env,
woden_ext_element_t *ext_el) 
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ext_el, AXIS2_FAILURE);
    super = WODEN_WSDL_ELEMENT_SUPER_OBJS(wsdl_element, env);
    wsdl_element_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL_ELEMENT", AXIS2_HASH_KEY_STRING)); 
    
    return WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(wsdl_element_impl->
            f_elem_ext, env, ext_el);
}

axis2_status_t AXIS2_CALL 
woden_wsdl_element_remove_ext_element(
        void *wsdl_element,
        const axis2_env_t *env,
woden_ext_element_t *ext_el) 
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ext_el, AXIS2_FAILURE);
    super = WODEN_WSDL_ELEMENT_SUPER_OBJS(wsdl_element, env);
    wsdl_element_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL_ELEMENT", AXIS2_HASH_KEY_STRING)); 

    return WODEN_ELEMENT_EXTENSIBLE_REMOVE_EXT_ELEMENT(wsdl_element_impl->
            f_elem_ext, env, ext_el);
}

axis2_array_list_t *AXIS2_CALL 
woden_wsdl_element_get_ext_elements(
        void *wsdl_element,
const axis2_env_t *env) 
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_WSDL_ELEMENT_SUPER_OBJS(wsdl_element, env);
    wsdl_element_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL_ELEMENT", AXIS2_HASH_KEY_STRING)); 
    
    return WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS(wsdl_element_impl->
            f_elem_ext, env);
}

axis2_array_list_t *AXIS2_CALL 
woden_wsdl_element_get_ext_elements_of_type(
        void *wsdl_element,
        const axis2_env_t *env,
axis2_qname_t *ext_type) 
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;
    axis2_hash_t *super = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ext_type, NULL);

    super = WODEN_WSDL_ELEMENT_SUPER_OBJS(wsdl_element, env);
    wsdl_element_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL_ELEMENT", AXIS2_HASH_KEY_STRING)); 

    return WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS_OF_TYPE(
            wsdl_element_impl->f_elem_ext, env, ext_type);
}

axis2_bool_t AXIS2_CALL 
woden_wsdl_element_has_ext_elements_for_namespace(
        void *wsdl_element,
        const axis2_env_t *env,
        axis2_uri_t *namespc)
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, namespc, AXIS2_FAILURE);
    super = WODEN_WSDL_ELEMENT_SUPER_OBJS(wsdl_element, env);
    wsdl_element_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL_ELEMENT", AXIS2_HASH_KEY_STRING)); 

    return WODEN_ELEMENT_EXTENSIBLE_HAS_EXT_ELEMENTS_FOR_NAMESPACE(
            wsdl_element_impl->f_elem_ext, env, namespc);
}

void *AXIS2_CALL 
woden_wsdl_element_get_element_extensible(
        void *wsdl_element,
        const axis2_env_t *env) 
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_WSDL_ELEMENT_SUPER_OBJS(wsdl_element, env);
    wsdl_element_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL_ELEMENT", AXIS2_HASH_KEY_STRING)); 
    
    return wsdl_element_impl->f_elem_ext;
}

void *AXIS2_CALL 
woden_wsdl_element_get_attr_extensible(
        void *wsdl_element,
        const axis2_env_t *env) 
{
    woden_wsdl_element_impl_t *wsdl_element_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_WSDL_ELEMENT_SUPER_OBJS(wsdl_element, env);
    wsdl_element_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_WSDL_ELEMENT", AXIS2_HASH_KEY_STRING)); 
    
    return wsdl_element_impl->f_attr_ext;
}




