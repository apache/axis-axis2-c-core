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

#include <woden/xml/axis2_woden_qname_or_token_any_attr.h>
#include <woden/xml/axis2_woden_xml_attr.h>
#include <axiom_element.h>
#include <axiom_node.h>

typedef struct axis2_woden_qname_or_token_any_attr_impl 
        axis2_woden_qname_or_token_any_attr_impl_t;

/** 
 * @brief QName or Token Any Attribute Struct Impl
 *   Axis2 QName or Token Any Attribute  
 */ 
struct axis2_woden_qname_or_token_any_attr_impl
{
    axis2_woden_qname_or_token_any_attr_t token_attr;
    axis2_woden_xml_attr_t *xml_attr;
    axis2_woden_obj_types_t obj_type;
    axis2_hash_t *methods;
};

#define INTF_TO_IMPL(token_attr) \
    ((axis2_woden_qname_or_token_any_attr_impl_t *) \
     token_attr)

axis2_status_t AXIS2_CALL 
axis2_woden_qname_or_token_any_attr_free(
        void *token_attr,
        const axis2_env_t *env);

axis2_woden_obj_types_t AXIS2_CALL 
axis2_woden_qname_or_token_any_attr_type(
        void *token_attr,
        const axis2_env_t *env);

axis2_woden_xml_attr_t *AXIS2_CALL
axis2_woden_qname_or_token_any_attr_get_base_impl(
        void *token_attr,
        const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
axis2_woden_qname_or_token_any_attr_is_qname(
        void *token_attr,
        const axis2_env_t *env); 

axis2_bool_t AXIS2_CALL
axis2_woden_qname_or_token_any_attr_is_token(
        void *token_attr,
        const axis2_env_t *env);

axis2_qname_t *AXIS2_CALL
axis2_woden_qname_or_token_any_attr_get_qname(
        void *token_attr,
        const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
axis2_woden_qname_or_token_any_attr_get_token(
        void *token_attr,
        const axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_qname_or_token_any_attr_convert(
        void *token_attr,
        const axis2_env_t *env,
        axiom_element_t *owner_el,
        axiom_node_t *owner_node,
        axis2_char_t *attr_value);

/*
 * TODO This constructor is not used for extension attributes, but may be useful if
 * parsing of native WSDL attributes is changed to use the XMLAttr interface.
 */
AXIS2_EXTERN axis2_woden_qname_or_token_any_attr_t * AXIS2_CALL
axis2_woden_qname_or_token_any_attr_create(const axis2_env_t *env,
        axiom_element_t *owner_el,
        axiom_node_t *owner_node,
        axis2_qname_t *attr_type,
        axis2_char_t *attr_value)
{
    axis2_woden_qname_or_token_any_attr_impl_t *
            token_attr_impl = NULL;
     
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    token_attr_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_woden_qname_or_token_any_attr_impl_t));

    token_attr_impl->obj_type = AXIS2_WODEN_QNAME_OR_TOKEN_ANY_ATTR;
    token_attr_impl->xml_attr = NULL;
    token_attr_impl->methods = NULL;

    token_attr_impl->token_attr.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_qname_or_token_any_attr_ops_t));

    token_attr_impl->token_attr.ops->free = 
        axis2_woden_qname_or_token_any_attr_free;
    token_attr_impl->token_attr.ops->type = 
        axis2_woden_qname_or_token_any_attr_type;
    token_attr_impl->token_attr.ops->get_base_impl = 
        axis2_woden_qname_or_token_any_attr_get_base_impl;

    token_attr_impl->token_attr.ops->is_qname = 
        axis2_woden_qname_or_token_any_attr_is_qname;
    token_attr_impl->token_attr.ops->is_token = 
        axis2_woden_qname_or_token_any_attr_is_token;
    token_attr_impl->token_attr.ops->get_qname = 
        axis2_woden_qname_or_token_any_attr_get_qname;
    token_attr_impl->token_attr.ops->get_token = 
        axis2_woden_qname_or_token_any_attr_get_token;
    token_attr_impl->token_attr.ops->convert = 
        axis2_woden_qname_or_token_any_attr_convert;
    
    
    token_attr_impl->methods = axis2_hash_make(env);
    if(!token_attr_impl->methods) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(token_attr_impl->methods, "free", 
            AXIS2_HASH_KEY_STRING, axis2_woden_qname_or_token_any_attr_free);
    axis2_hash_set(token_attr_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_woden_qname_or_token_any_attr_type);

    axis2_hash_set(token_attr_impl->methods, "is_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_qname_or_token_any_attr_is_qname);
    axis2_hash_set(token_attr_impl->methods, "is_token", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_qname_or_token_any_attr_is_token);
    axis2_hash_set(token_attr_impl->methods, "get_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_qname_or_token_any_attr_get_qname);
    axis2_hash_set(token_attr_impl->methods, "get_token", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_qname_or_token_any_attr_get_token);
    axis2_hash_set(token_attr_impl->methods, "convert", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_qname_or_token_any_attr_convert);

    token_attr_impl->xml_attr = axis2_woden_xml_attr_create(
            env, owner_el, owner_node, attr_type, attr_value);
    
    return &(token_attr_impl->token_attr);
}

axis2_woden_obj_types_t AXIS2_CALL
axis2_woden_qname_or_token_any_attr_type(
        void *token_attr,
        const axis2_env_t *env)
{
    axis2_woden_qname_or_token_any_attr_impl_t *
            token_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    token_attr_impl = INTF_TO_IMPL(token_attr);

    return token_attr_impl->obj_type;
}

axis2_status_t AXIS2_CALL
axis2_woden_qname_or_token_any_attr_free(
        void *token_attr,
        const axis2_env_t *env)
{
    axis2_woden_qname_or_token_any_attr_impl_t *
            token_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    token_attr_impl = INTF_TO_IMPL(token_attr);

    if(token_attr_impl->methods)
    {
        axis2_hash_free(token_attr_impl->methods, env);
        token_attr_impl->methods = NULL;
    }  

    if(token_attr_impl->xml_attr)
    {
        AXIS2_WODEN_XML_ATTR_FREE(token_attr_impl->xml_attr, 
                env);
        token_attr_impl->xml_attr = NULL;
    }
    
    if((&(token_attr_impl->token_attr))->ops)
    {
        AXIS2_FREE(env->allocator, (&(token_attr_impl->
                        token_attr))->ops);
        (&(token_attr_impl->token_attr))->ops = 
                NULL;
    }

    if(token_attr_impl)
    {
        AXIS2_FREE(env->allocator, token_attr_impl);
        token_attr_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_woden_xml_attr_t *AXIS2_CALL
axis2_woden_qname_or_token_any_attr_get_base_impl(
        void *token_attr,
        const axis2_env_t *env)
{
    axis2_woden_qname_or_token_any_attr_impl_t *
            token_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    token_attr_impl = 
        INTF_TO_IMPL(token_attr);

    return token_attr_impl->xml_attr;
}

axis2_status_t AXIS2_CALL
axis2_woden_qname_or_token_any_attr_resolve_methods(
        axis2_woden_qname_or_token_any_attr_t *
        token_attr,
        const axis2_env_t *env,
        axis2_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    
    token_attr->ops->free = axis2_hash_get(methods, 
            "free", AXIS2_HASH_KEY_STRING);
    token_attr->ops->type = axis2_hash_get(methods, 
            "type", AXIS2_HASH_KEY_STRING);

    token_attr->ops->is_qname = axis2_hash_get(methods, 
            "is_qname", AXIS2_HASH_KEY_STRING);
    token_attr->ops->is_token = axis2_hash_get(methods, 
            "is_token", AXIS2_HASH_KEY_STRING);
    token_attr->ops->get_qname = axis2_hash_get(methods, 
            "get_qname", AXIS2_HASH_KEY_STRING);
    token_attr->ops->get_token = axis2_hash_get(methods, 
            "get_token", AXIS2_HASH_KEY_STRING);
    token_attr->ops->convert = axis2_hash_get(methods, 
            "convert", AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_woden_qname_or_token_any_attr_is_qname(
        void *token_attr,
        const axis2_env_t *env) 
{
    axis2_woden_qname_or_token_any_attr_impl_t *
            token_attr_impl = NULL;
    axis2_generic_obj_t *content = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    token_attr_impl = INTF_TO_IMPL(token_attr);
    /* TODO Check this. We need to check whether the
     * f_content is an instance of qname
     */
    if(AXIS2_FALSE == axis2_woden_qname_or_token_any_attr_is_token(token_attr, 
                env))
        return AXIS2_TRUE;
    content = AXIS2_WODEN_XML_ATTR_GET_CONTENT(token_attr_impl->xml_attr, env);
    if(AXIS2_QNAME == AXIS2_GENERIC_OBJ_GET_TYPE(content, env))
        return AXIS2_TRUE;
    return AXIS2_FALSE;
}

axis2_bool_t AXIS2_CALL
axis2_woden_qname_or_token_any_attr_is_token(
        void *token_attr,
        const axis2_env_t *env) 
{
    axis2_woden_qname_or_token_any_attr_impl_t *
            token_attr_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    token_attr_impl = INTF_TO_IMPL(token_attr);

    if(AXIS2_TRUE != axis2_woden_qname_or_token_any_attr_is_qname(token_attr, 
                env) && AXIS2_TRUE == AXIS2_WODEN_XML_ATTR_IS_VALID(
                    token_attr_impl->xml_attr, env))
    {
        return AXIS2_TRUE;
    } else 
    {
        return AXIS2_FALSE;
    }
}

axis2_qname_t *AXIS2_CALL
axis2_woden_qname_or_token_any_attr_get_qname(
        void *token_attr,
        const axis2_env_t *env)
{
    axis2_woden_qname_or_token_any_attr_impl_t *
            token_attr_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    token_attr_impl = INTF_TO_IMPL(token_attr);

    if(AXIS2_TRUE == axis2_woden_qname_or_token_any_attr_is_qname(token_attr, 
                env))
        return (axis2_qname_t *) AXIS2_WODEN_XML_ATTR_GET_CONTENT(
                token_attr_impl->xml_attr, env);
    else
        return NULL;
}

axis2_char_t *AXIS2_CALL
axis2_woden_qname_or_token_any_attr_get_token(
        void *token_attr,
        const axis2_env_t *env)
{
    axis2_woden_qname_or_token_any_attr_impl_t *
            token_attr_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    token_attr_impl = INTF_TO_IMPL(token_attr);
    if(AXIS2_TRUE == axis2_woden_qname_or_token_any_attr_is_qname(token_attr, 
                env) && AXIS2_TRUE == AXIS2_WODEN_XML_ATTR_IS_VALID(
                    token_attr_impl->xml_attr, env) )
    {
        return (axis2_char_t *) AXIS2_WODEN_XML_ATTR_GET_CONTENT(
                token_attr_impl->xml_attr, env);
    } else 
    {
        return NULL;
    }
}

void *AXIS2_CALL
axis2_woden_qname_or_token_any_attr_convert(
        void *token_attr,
        const axis2_env_t *env,
        axiom_element_t *owner_el,
        axiom_node_t *owner_node,
        axis2_char_t *attr_value)
{
    axis2_woden_qname_or_token_any_attr_impl_t *
            token_attr_impl = NULL;
    axis2_qname_t *qn = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    token_attr_impl = INTF_TO_IMPL(token_attr);

    /* First, check if the attribute contains the xs:token '#any'.*/
    if(0 == AXIS2_STRCMP("#any", attr_value)) return attr_value;

    /* Second, assume the attribute contains a list of xs:QName. */
    if(attr_value)
    {
        /* TODO */
    }
    
    if(!qn)
    {
        AXIS2_WODEN_XML_ATTR_SET_VALID(token_attr_impl->
                xml_attr, env, AXIS2_FALSE);
        /* TODO handler error */
    }
    
    return qn;
}

