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
 
#include <woden_element_extensible.h>
#include <woden_ext_element.h>
#include <woden_xml_attr.h>
#include <axis2_uri.h>
#include <axis2_hash.h>

typedef struct woden_element_extensible_impl woden_element_extensible_impl_t;

/** 
 * @brief Element Extensible Struct Impl
 *   Axis2 Element Extensible  
 */ 
struct woden_element_extensible_impl
{
    woden_element_extensible_t extensible;
    woden_obj_types_t obj_type;
    axis2_hash_t *super;
    axis2_array_list_t *f_ext_elements;
    axis2_array_list_t *temp_elems;
};

#define INTF_TO_IMPL(extensible) \
    ((woden_element_extensible_impl_t *) extensible)

axis2_status_t AXIS2_CALL 
woden_element_extensible_free(
        void *extensible,
        const axis2_env_t *envv);

axis2_hash_t *AXIS2_CALL 
woden_element_extensible_super_objs(
        void *extensible,
        const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL 
woden_element_extensible_type(
        void *extensible,
        const axis2_env_t *envv);

axis2_status_t AXIS2_CALL 
woden_element_extensible_add_ext_element(
        void *extensible,
        const axis2_env_t *env,
        woden_ext_element_t *ext_el); 

axis2_status_t AXIS2_CALL 
woden_element_extensible_remove_ext_element(
        void *extensible,
        const axis2_env_t *env,
        woden_ext_element_t *ext_el); 

axis2_array_list_t *AXIS2_CALL 
woden_element_extensible_get_ext_elements(
        void *extensible,
        const axis2_env_t *env); 

axis2_array_list_t *AXIS2_CALL 
woden_element_extensible_get_ext_elements_of_type(
        void *extensible,
        const axis2_env_t *env,
        axis2_qname_t *ext_type);

axis2_bool_t AXIS2_CALL 
woden_element_extensible_has_ext_elements_for_namespace(
        void *extensible,
        const axis2_env_t *env,
        axis2_uri_t *namespc);



AXIS2_EXTERN woden_element_extensible_t * AXIS2_CALL
woden_element_extensible_create(
        const axis2_env_t *env)
{
    woden_element_extensible_impl_t *extensible_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    extensible_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_element_extensible_impl_t));

    extensible_impl->obj_type = WODEN_ELEMENT_EXTENSIBLE;
    extensible_impl->super = NULL;
    extensible_impl->f_ext_elements = NULL;
    extensible_impl->temp_elems = NULL;

    extensible_impl->extensible.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_element_extensible_ops_t)); 
    
    extensible_impl->extensible.ops->free = 
        woden_element_extensible_free;
    extensible_impl->extensible.ops->type = 
        woden_element_extensible_type;
    extensible_impl->extensible.ops->add_ext_element = 
        woden_element_extensible_add_ext_element;
    extensible_impl->extensible.ops->remove_ext_element = 
        woden_element_extensible_remove_ext_element;
    extensible_impl->extensible.ops->get_ext_elements = 
        woden_element_extensible_get_ext_elements;
    extensible_impl->extensible.ops->get_ext_elements_of_type = 
        woden_element_extensible_get_ext_elements_of_type;
    extensible_impl->extensible.ops->has_ext_elements_for_namespace = 
        woden_element_extensible_has_ext_elements_for_namespace;

    extensible_impl->f_ext_elements = axis2_array_list_create(env, 0);
    extensible_impl->super = axis2_hash_make(env);
    if(!extensible_impl->super) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(extensible_impl->super, "WODEN_ELEMENT_EXTENSIBLE", 
            AXIS2_HASH_KEY_STRING, &(extensible_impl->extensible));
 
    return &(extensible_impl->extensible);
}

axis2_status_t AXIS2_CALL
woden_element_extensible_free(void *extensible,
                const axis2_env_t *env)
{
    woden_element_extensible_impl_t *extensible_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);

    if(extensible_impl->f_ext_elements)
    {
        AXIS2_ARRAY_LIST_FREE(extensible_impl->f_ext_elements, env);
        extensible_impl->f_ext_elements = NULL;
    }
    
    if(extensible_impl->temp_elems)
    {
        int size = 0, i = 0;
        size = AXIS2_ARRAY_LIST_SIZE(extensible_impl->temp_elems, env);
        for(i = 0; i < size; i++)
        {
            void *ext_el = NULL;

            ext_el = AXIS2_ARRAY_LIST_GET(extensible_impl->temp_elems, env, i);
            WODEN_XML_ATTR_FREE(ext_el, env);
        }
        AXIS2_ARRAY_LIST_FREE(extensible_impl->temp_elems, env);
        extensible_impl->temp_elems = NULL;
    }

    if(extensible_impl->super)
    {
        axis2_hash_free(extensible_impl->super, env);
        extensible_impl->super = NULL;
    }

    if((&(extensible_impl->extensible))->ops)
    {
        AXIS2_FREE(env->allocator, (&(extensible_impl->extensible))->ops);
        (&(extensible_impl->extensible))->ops = NULL;
    }

    if(extensible_impl)
    {
        AXIS2_FREE(env->allocator, extensible_impl);
        extensible_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_element_extensible_super_objs(
        void *extensible,
        const axis2_env_t *env)
{
    woden_element_extensible_impl_t *extensible_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    extensible_impl = INTF_TO_IMPL(extensible);

    return extensible_impl->super;
}

woden_obj_types_t AXIS2_CALL 
woden_element_extensible_type(
        void *extensible,
        const axis2_env_t *env)
{
    woden_element_extensible_impl_t *extensible_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);
    
    return extensible_impl->obj_type;
}

axis2_status_t AXIS2_CALL
woden_element_extensible_resolve_methods(
        woden_element_extensible_t *extensible,
        const axis2_env_t *env,
        woden_element_extensible_t *extensible_impl,
        axis2_hash_t *methods)
{
    woden_element_extensible_impl_t *extensible_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    extensible_impl_l = INTF_TO_IMPL(extensible_impl);
    
    extensible->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    extensible->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    extensible->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    
    extensible->ops->add_ext_element = axis2_hash_get(methods, 
            "add_ext_element", AXIS2_HASH_KEY_STRING); 
    if(!extensible->ops->add_ext_element && extensible_impl_l)
            extensible->ops->add_ext_element = 
            extensible_impl_l->extensible.ops->add_ext_element;
    
    extensible->ops->remove_ext_element = axis2_hash_get(methods, 
            "remove_ext_element", AXIS2_HASH_KEY_STRING); 
    if(!extensible->ops->remove_ext_element && extensible_impl_l)
            extensible->ops->remove_ext_element = 
            extensible_impl_l->extensible.ops->remove_ext_element;
    
    extensible->ops->get_ext_elements = axis2_hash_get(methods, 
            "get_ext_elements", AXIS2_HASH_KEY_STRING); 
    if(!extensible->ops->get_ext_elements && extensible_impl_l)
            extensible->ops->get_ext_elements = 
            extensible_impl_l->extensible.ops->get_ext_elements;
    
    extensible->ops->get_ext_elements_of_type = axis2_hash_get(methods, 
            "get_ext_elements_of_type", AXIS2_HASH_KEY_STRING);
    if(!extensible->ops->get_ext_elements_of_type && extensible_impl_l)
            extensible->ops->get_ext_elements_of_type = 
            extensible_impl_l->extensible.ops->get_ext_elements_of_type;
    
    extensible->ops->has_ext_elements_for_namespace = axis2_hash_get(methods, 
            "has_ext_elements_for_namespace", AXIS2_HASH_KEY_STRING);
    if(!extensible->ops->has_ext_elements_for_namespace && extensible_impl_l)
            extensible->ops->has_ext_elements_for_namespace = 
            extensible_impl_l->extensible.ops->has_ext_elements_for_namespace;

    return AXIS2_SUCCESS;    
}

axis2_status_t AXIS2_CALL 
woden_element_extensible_add_ext_element(
                                    void *extensible,
                                    const axis2_env_t *env,
                                    woden_ext_element_t *ext_el) 
{
    woden_element_extensible_impl_t *extensible_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ext_el, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);
    
    
    return AXIS2_ARRAY_LIST_ADD(extensible_impl->f_ext_elements, env, ext_el);
}

axis2_status_t AXIS2_CALL 
woden_element_extensible_remove_ext_element(
                                    void *extensible,
                                    const axis2_env_t *env,
                                    woden_ext_element_t *ext_el) 
{
    woden_element_extensible_impl_t *extensible_impl = NULL;
    int index = -1;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ext_el, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);

    index = AXIS2_ARRAY_LIST_INDEX_OF(extensible_impl->f_ext_elements, env, ext_el);
    AXIS2_ARRAY_LIST_REMOVE(extensible_impl->f_ext_elements, env, index);
    return AXIS2_SUCCESS;
}

axis2_array_list_t *AXIS2_CALL 
woden_element_extensible_get_ext_elements(void *extensible,
                                                const axis2_env_t *env) 
{
    woden_element_extensible_impl_t *extensible_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    extensible_impl = INTF_TO_IMPL(extensible);
    
    return extensible_impl->f_ext_elements;
}

axis2_array_list_t *AXIS2_CALL 
woden_element_extensible_get_ext_elements_of_type(void *extensible,
                                                        const axis2_env_t *env,
                                                        axis2_qname_t *ext_type) 
{
    woden_element_extensible_impl_t *extensible_impl = NULL;
    int i = 0;
    int size = 0;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ext_type, NULL);
    extensible_impl = INTF_TO_IMPL(extensible);

    if(extensible_impl->temp_elems)
    {
        int size = 0, i = 0;
        size = AXIS2_ARRAY_LIST_SIZE(extensible_impl->temp_elems, env);
        for(i = 0; i < size; i++)
        {
            void *ext_el = NULL;

            ext_el = AXIS2_ARRAY_LIST_GET(extensible_impl->temp_elems, env, i);
            WODEN_XML_ATTR_FREE(ext_el, env);
        }
        AXIS2_ARRAY_LIST_FREE(extensible_impl->temp_elems, env);
        extensible_impl->temp_elems = NULL;
    }

    extensible_impl->temp_elems = axis2_array_list_create(env, 0);
    size = AXIS2_ARRAY_LIST_SIZE(extensible_impl->f_ext_elements, env);
    for(i = 0; i < size; i++)
    {
        woden_ext_element_t *ext_elem = NULL;
        axis2_qname_t *ext_type_l = NULL;
        
        ext_elem = (woden_ext_element_t *)AXIS2_ARRAY_LIST_GET(
                extensible_impl->f_ext_elements, env, i);
        ext_type_l = WODEN_EXT_ELEMENT_GET_EXT_TYPE(ext_elem, env);
        if(AXIS2_TRUE == AXIS2_QNAME_EQUALS(ext_type, env, ext_type_l))
        {
            AXIS2_ARRAY_LIST_ADD(extensible_impl->temp_elems, env, ext_elem);
        }
    }
    return extensible_impl->temp_elems;
}

axis2_bool_t AXIS2_CALL 
woden_element_extensible_has_ext_elements_for_namespace(void *extensible,
                                                                const axis2_env_t *env,
                                                                axis2_uri_t *namespc)
{
    woden_element_extensible_impl_t *extensible_impl = NULL;
    axis2_bool_t result = AXIS2_FALSE;
    axis2_char_t *ext_ns = NULL;
    int i = 0;
    int size = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, namespc, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);

    ext_ns = AXIS2_URI_TO_STRING(namespc, env, AXIS2_URI_UNP_OMITUSERINFO);
    size = AXIS2_ARRAY_LIST_SIZE(extensible_impl->f_ext_elements, env);
    for(i = 0; i < size; i++)
    {
        woden_ext_element_t *ext_elem = NULL;
        axis2_qname_t *ext_type = NULL;
        axis2_char_t *uri = NULL;
        
        ext_elem = (woden_ext_element_t *) AXIS2_ARRAY_LIST_GET(
                extensible_impl->f_ext_elements, env, i);
        ext_type = WODEN_EXT_ELEMENT_GET_EXT_TYPE(ext_elem, env);
        uri = AXIS2_QNAME_GET_URI(ext_type, env);
        if(0 == AXIS2_STRCMP(uri, ext_ns))
        {
            result = AXIS2_TRUE;
            break;
        }
    }
    return result;
}


