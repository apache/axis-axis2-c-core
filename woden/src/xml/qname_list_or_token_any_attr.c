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

#include <woden_qname_list_or_token_any_attr.h>
#include <woden_xml_attr.h>
#include <axiom_element.h>
#include <axiom_node.h>

typedef struct woden_qname_list_or_token_any_attr_impl 
        woden_qname_list_or_token_any_attr_impl_t;

/** 
 * @brief QName List or Token Any Attribute Struct Impl
 *   Axis2 QName List or Token Any Attribute  
 */ 
struct woden_qname_list_or_token_any_attr_impl
{
    woden_qname_list_or_token_any_attr_t list_token_attr;
    woden_xml_attr_t *xml_attr;
    woden_obj_types_t obj_type;
    axis2_hash_t *methods;
    axis2_bool_t is_token;
};

#define INTF_TO_IMPL(list_token_attr) \
    ((woden_qname_list_or_token_any_attr_impl_t *) \
     list_token_attr)

axis2_status_t AXIS2_CALL 
woden_qname_list_or_token_any_attr_free(
       void *list_token_attr,
       const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL 
woden_qname_list_or_token_any_attr_type(
       void *list_token_attr,
       const axis2_env_t *env);

woden_xml_attr_t *AXIS2_CALL
woden_qname_list_or_token_any_attr_get_base_impl(
       void *list_token_attr,
       const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
woden_qname_list_or_token_any_attr_is_qname_list(
       void *list_token_attr,
       const axis2_env_t *env); 

axis2_bool_t AXIS2_CALL
woden_qname_list_or_token_any_attr_is_token(
       void *list_token_attr,
       const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
woden_qname_list_or_token_any_attr_get_qnames(
       void *list_token_attr,
       const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
woden_qname_list_or_token_any_attr_get_token(
       void *list_token_attr,
       const axis2_env_t *env);

void *AXIS2_CALL
woden_qname_list_or_token_any_attr_convert(
       void *list_token_attr,
       const axis2_env_t *env,
       axiom_element_t *owner_el,
       axiom_node_t *owner_node,
       axis2_char_t *attr_value);

/*
 * TODO This constructor is not used for extension attributes, but may be useful if
 * parsing of native WSDL attributes is changed to use the XMLAttr interface.
 */
AXIS2_EXTERN woden_qname_list_or_token_any_attr_t * AXIS2_CALL
woden_qname_list_or_token_any_attr_create(
       const axis2_env_t *env,
       axiom_element_t *owner_el,
       axiom_node_t *owner_node,
       axis2_qname_t *attr_type,
       axis2_char_t *attr_value)
{
    woden_qname_list_or_token_any_attr_impl_t *
            list_token_attr_impl = NULL;
     
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    list_token_attr_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_qname_list_or_token_any_attr_impl_t));

    list_token_attr_impl->obj_type = WODEN_QNAME_LIST_OR_TOKEN_ANY_ATTR;
    list_token_attr_impl->xml_attr = NULL;
    list_token_attr_impl->methods = NULL;
    list_token_attr_impl->list_token_attr.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_qname_list_or_token_any_attr_ops_t));

    list_token_attr_impl->list_token_attr.ops->free = 
        woden_qname_list_or_token_any_attr_free;
    list_token_attr_impl->list_token_attr.ops->type = 
        woden_qname_list_or_token_any_attr_type;
    list_token_attr_impl->list_token_attr.ops->get_base_impl = 
        woden_qname_list_or_token_any_attr_get_base_impl;
    list_token_attr_impl->list_token_attr.ops->is_qname_list = 
        woden_qname_list_or_token_any_attr_is_qname_list;
    list_token_attr_impl->list_token_attr.ops->is_token = 
        woden_qname_list_or_token_any_attr_is_token;
    list_token_attr_impl->list_token_attr.ops->get_qnames = 
        woden_qname_list_or_token_any_attr_get_qnames;
    list_token_attr_impl->list_token_attr.ops->get_token = 
        woden_qname_list_or_token_any_attr_get_token;
    list_token_attr_impl->list_token_attr.ops->convert = 
        woden_qname_list_or_token_any_attr_convert;
    
    
    list_token_attr_impl->methods = axis2_hash_make(env);
    if(!list_token_attr_impl->methods) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(list_token_attr_impl->methods, "free", 
            AXIS2_HASH_KEY_STRING, woden_qname_list_or_token_any_attr_free);
    axis2_hash_set(list_token_attr_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, woden_qname_list_or_token_any_attr_type);
    axis2_hash_set(list_token_attr_impl->methods, "is_qname_list", 
            AXIS2_HASH_KEY_STRING, 
            woden_qname_list_or_token_any_attr_is_qname_list);
    axis2_hash_set(list_token_attr_impl->methods, "is_token", 
            AXIS2_HASH_KEY_STRING, 
            woden_qname_list_or_token_any_attr_is_token);
    axis2_hash_set(list_token_attr_impl->methods, "get_qnames", 
            AXIS2_HASH_KEY_STRING, 
            woden_qname_list_or_token_any_attr_get_qnames);
    axis2_hash_set(list_token_attr_impl->methods, "get_token", 
            AXIS2_HASH_KEY_STRING, 
            woden_qname_list_or_token_any_attr_get_token);
    axis2_hash_set(list_token_attr_impl->methods, "convert", 
            AXIS2_HASH_KEY_STRING, 
            woden_qname_list_or_token_any_attr_convert);

    list_token_attr_impl->xml_attr = woden_xml_attr_create(
            env, owner_el, owner_node, attr_type, attr_value);
    
    return &(list_token_attr_impl->list_token_attr);
}

woden_obj_types_t AXIS2_CALL
woden_qname_list_or_token_any_attr_type(
        void *list_token_attr,
        const axis2_env_t *env)
{
    woden_qname_list_or_token_any_attr_impl_t *
            list_token_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    list_token_attr_impl = INTF_TO_IMPL(list_token_attr);

    return list_token_attr_impl->obj_type;
}

axis2_status_t AXIS2_CALL
woden_qname_list_or_token_any_attr_free(
        void *list_token_attr,
        const axis2_env_t *env)
{
    woden_qname_list_or_token_any_attr_impl_t *
            list_token_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    list_token_attr_impl = INTF_TO_IMPL(list_token_attr);

    if(list_token_attr_impl->methods)
    {
        axis2_hash_free(list_token_attr_impl->methods, env);
        list_token_attr_impl->methods = NULL;
    }  

    if(list_token_attr_impl->xml_attr)
    {
        WODEN_XML_ATTR_FREE(list_token_attr_impl->xml_attr, 
                env);
        list_token_attr_impl->xml_attr = NULL;
    }
    
    if((&(list_token_attr_impl->list_token_attr))->ops)
    {
        AXIS2_FREE(env->allocator, (&(list_token_attr_impl->
                        list_token_attr))->ops);
        (&(list_token_attr_impl->list_token_attr))->ops = 
                NULL;
    }

    if(list_token_attr_impl)
    {
        AXIS2_FREE(env->allocator, list_token_attr_impl);
        list_token_attr_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

woden_xml_attr_t *AXIS2_CALL
woden_qname_list_or_token_any_attr_get_base_impl(
        void *list_token_attr,
        const axis2_env_t *env)
{
    woden_qname_list_or_token_any_attr_impl_t *
            list_token_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    list_token_attr_impl = 
        INTF_TO_IMPL(list_token_attr);

    return list_token_attr_impl->xml_attr;
}

axis2_status_t AXIS2_CALL
woden_qname_list_or_token_any_attr_resolve_methods(
        woden_qname_list_or_token_any_attr_t *list_token_attr,
        const axis2_env_t *env,
        axis2_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    
    list_token_attr->ops->free = axis2_hash_get(methods, 
            "free", AXIS2_HASH_KEY_STRING);
    list_token_attr->ops->to_qname_list_or_token_any_attr_free = 
            axis2_hash_get(methods, "to_qname_list_or_token_any_attr_free", 
            AXIS2_HASH_KEY_STRING);
    list_token_attr->ops->type = axis2_hash_get(methods, 
            "type", AXIS2_HASH_KEY_STRING);

    list_token_attr->ops->is_qname_list = axis2_hash_get(methods, 
            "is_qname_list", AXIS2_HASH_KEY_STRING);
    list_token_attr->ops->is_token = axis2_hash_get(methods, 
            "is_token", AXIS2_HASH_KEY_STRING);
    list_token_attr->ops->get_qnames = axis2_hash_get(methods, 
            "get_qnames", AXIS2_HASH_KEY_STRING);
    list_token_attr->ops->get_token = axis2_hash_get(methods, 
            "get_token", AXIS2_HASH_KEY_STRING);
    list_token_attr->ops->convert = axis2_hash_get(methods, 
            "convert", AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
woden_qname_list_or_token_any_attr_is_qname_list(
        void *list_token_attr,
        const axis2_env_t *env) 
{
    woden_qname_list_or_token_any_attr_impl_t *
            list_token_attr_impl = NULL;
    axis2_generic_obj_t *content = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    list_token_attr_impl = INTF_TO_IMPL(list_token_attr);

    content = WODEN_XML_ATTR_GET_CONTENT(list_token_attr_impl->xml_attr, 
            env);
    if(AXIS2_QNAME_LIST == AXIS2_GENERIC_OBJ_GET_TYPE(content, env))
        return AXIS2_TRUE;
    return AXIS2_FALSE;
}

axis2_bool_t AXIS2_CALL
woden_qname_list_or_token_any_attr_is_token(
        void *list_token_attr,
        const axis2_env_t *env) 
{
    woden_qname_list_or_token_any_attr_impl_t *
            list_token_attr_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    list_token_attr_impl = INTF_TO_IMPL(list_token_attr);

    if(AXIS2_TRUE != woden_qname_list_or_token_any_attr_is_qname_list(
                list_token_attr, env) && AXIS2_TRUE == 
            WODEN_XML_ATTR_IS_VALID(list_token_attr_impl->xml_attr, env))
    {
        return AXIS2_TRUE;
    } else 
    {
        return AXIS2_FALSE;
    }
}

axis2_array_list_t *AXIS2_CALL
woden_qname_list_or_token_any_attr_get_qnames(
        void *list_token_attr,
        const axis2_env_t *env)
{
    woden_qname_list_or_token_any_attr_impl_t *
            list_token_attr_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    list_token_attr_impl = INTF_TO_IMPL(list_token_attr);
    return (axis2_array_list_t *) WODEN_XML_ATTR_GET_CONTENT(
            list_token_attr_impl->xml_attr, env);
}

axis2_char_t *AXIS2_CALL
woden_qname_list_or_token_any_attr_get_token(
        void *list_token_attr,
        const axis2_env_t *env)
{
    woden_qname_list_or_token_any_attr_impl_t *
            list_token_attr_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    list_token_attr_impl = INTF_TO_IMPL(list_token_attr);
    if(AXIS2_TRUE == 
            woden_qname_list_or_token_any_attr_is_token(list_token_attr, 
                env))
    {
        return (axis2_char_t *) WODEN_XML_ATTR_GET_CONTENT(
                list_token_attr_impl->xml_attr, env);
    } else 
    {
        return NULL;
    }
}

void *AXIS2_CALL
woden_qname_list_or_token_any_attr_convert(
        void *list_token_attr,
        const axis2_env_t *env,
        axiom_element_t *owner_el,
        axiom_node_t *owner_node,
        axis2_char_t *attr_value)
{
    woden_qname_list_or_token_any_attr_impl_t *
            list_token_attr_impl = NULL;
    axis2_array_list_t *qn_array = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    list_token_attr_impl = INTF_TO_IMPL(list_token_attr);

    /* First, check if the attribute contains the xs:token '#any'.*/
    if(0 == AXIS2_STRCMP("#any", attr_value)) return attr_value;

    /* Second, assume the attribute contains a list of xs:QName. */
    if(attr_value)
    {
        /* TODO */
    }
    
    if(!qn_array)
    {
        WODEN_XML_ATTR_SET_VALID(list_token_attr_impl->
                xml_attr, env, AXIS2_FALSE);
        /* TODO handler error */
    }
    
    return qn_array;
}

