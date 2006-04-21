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

#include <woden/axis2_woden_qname_or_token_any_attr.h>
#include <axis2_om_element.h>
#include <axis2_om_node.h>

typedef struct axis2_woden_qname_or_token_any_attr_impl 
        axis2_woden_qname_or_token_any_attr_impl_t;

/** 
 * @brief QName or Token Any Attribute Struct Impl
 *	Axis2 QName or Token Any Attribute  
 */ 
struct axis2_woden_qname_or_token_any_attr_impl
{
    axis2_woden_qname_or_token_any_attr_t list_token_attr;
    axis2_woden_xml_attr_t *xml_attr;
    axis2_hash_t *methods;
    axis2_bool_t is_token;
};

#define INTF_TO_IMPL(list_token_attr) \
    ((axis2_woden_qname_or_token_any_attr_impl_t *) \
     list_token_attr)

axis2_status_t AXIS2_CALL 
axis2_woden_qname_or_token_any_attr_free(
                        void *list_token_attr,
                        axis2_env_t **env);

axis2_woden_xml_attr_t *AXIS2_CALL
axis2_woden_qname_or_token_any_attr_get_base_impl(
                                void *list_token_attr,
                                axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_woden_qname_or_token_any_attr_is_qname(
                                    void *list_token_attr,
                                    axis2_env_t **env); 

axis2_bool_t AXIS2_CALL
axis2_woden_qname_or_token_any_attr_is_token(
                                    void *list_token_attr,
                                    axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL
axis2_woden_qname_or_token_any_attr_get_qnames(
                                    void *list_token_attr,
                                    axis2_env_t **env);

axis2_char_t *AXIS2_CALL
axis2_woden_qname_or_token_any_attr_get_token(
                                    void *list_token_attr,
                                    axis2_env_t **env);

void *AXIS2_CALL
axis2_woden_qname_or_token_any_attr_convert(
                                void *list_token_attr,
                                axis2_env_t **env,
                                axis2_om_element_t *owner_el,
                                axis2_om_node_t *owner_node,
                                axis2_char_t *attr_value);

/*
 * TODO This constructor is not used for extension attributes, but may be useful if
 * parsing of native WSDL attributes is changed to use the XMLAttr interface.
 */
AXIS2_DECLARE(axis2_woden_qname_or_token_any_attr_t *)
axis2_woden_qname_or_token_any_attr_create(axis2_env_t **env,
                                axis2_om_element_t *owner_el,
                                axis2_om_node_t *owner_node,
                                axis2_qname_t *attr_type,
                                axis2_char_t *attr_value)
{
    axis2_woden_qname_or_token_any_attr_impl_t *
            list_token_attr_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
     
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    list_token_attr_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_qname_or_token_any_attr_impl_t));

    list_token_attr_impl->xml_attr = NULL;
    list_token_attr_impl->methods = NULL;
    list_token_attr_impl->list_token_attr.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_qname_or_token_any_attr_ops_t));

    list_token_attr_impl->list_token_attr.ops->free = 
        axis2_woden_qname_or_token_any_attr_free;
    list_token_attr_impl->list_token_attr.ops->get_base_impl = 
        axis2_woden_qname_or_token_any_attr_get_base_impl;
    list_token_attr_impl->list_token_attr.ops->is_qname = 
        axis2_woden_qname_or_token_any_attr_is_qname;
    list_token_attr_impl->list_token_attr.ops->is_token = 
        axis2_woden_qname_or_token_any_attr_is_token;
    list_token_attr_impl->list_token_attr.ops->get_qnames = 
        axis2_woden_qname_or_token_any_attr_get_qnames;
    list_token_attr_impl->list_token_attr.ops->get_token = 
        axis2_woden_qname_or_token_any_attr_get_token;
    list_token_attr_impl->list_token_attr.ops->convert = 
        axis2_woden_qname_or_token_any_attr_convert;
    
    
    list_token_attr_impl->methods = axis2_hash_make(env);
    if(!list_token_attr_impl->methods) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(list_token_attr_impl->methods, "free", 
            AXIS2_HASH_KEY_STRING, axis2_woden_qname_or_token_any_attr_free);
    axis2_hash_set(list_token_attr_impl->methods, "is_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_qname_or_token_any_attr_is_qname);
    axis2_hash_set(list_token_attr_impl->methods, "is_token", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_qname_or_token_any_attr_is_token);
    axis2_hash_set(list_token_attr_impl->methods, "get_qnames", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_qname_or_token_any_attr_get_qnames);
    axis2_hash_set(list_token_attr_impl->methods, "get_token", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_qname_or_token_any_attr_get_token);
    axis2_hash_set(list_token_attr_impl->methods, "convert", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_qname_or_token_any_attr_convert);

    list_token_attr_impl->xml_attr = axis2_woden_xml_attr_create(
            env, owner_el, owner_node, attr_type, attr_value);
    status = axis2_woden_xml_attr_resolve_methods(&(list_token_attr_impl->
                list_token_attr.base), env, 
                list_token_attr_impl->xml_attr, 
                list_token_attr_impl->methods);
    if(AXIS2_SUCCESS != status) return NULL;
    return &(list_token_attr_impl->list_token_attr);
}

axis2_status_t AXIS2_CALL
axis2_woden_qname_or_token_any_attr_free(void *list_token_attr,
                        axis2_env_t **env)
{
    axis2_woden_qname_or_token_any_attr_impl_t *
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
        AXIS2_WODEN_XML_ATTR_FREE(list_token_attr_impl->xml_attr, 
                env);
        list_token_attr_impl->xml_attr = NULL;
    }
    
    if((&(list_token_attr_impl->list_token_attr))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(list_token_attr_impl->
                        list_token_attr))->ops);
        (&(list_token_attr_impl->list_token_attr))->ops = 
                NULL;
    }

    if(list_token_attr_impl)
    {
        AXIS2_FREE((*env)->allocator, list_token_attr_impl);
        list_token_attr_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_woden_xml_attr_t *AXIS2_CALL
axis2_woden_qname_or_token_any_attr_get_base_impl(
                                void *list_token_attr,
                                axis2_env_t **env)
{
    axis2_woden_qname_or_token_any_attr_impl_t *
            list_token_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    list_token_attr_impl = 
        INTF_TO_IMPL(list_token_attr);

    return list_token_attr_impl->xml_attr;
}

axis2_status_t AXIS2_CALL
axis2_woden_qname_or_token_any_attr_resolve_methods(
                    axis2_woden_qname_or_token_any_attr_t *
                            list_token_attr,
                    axis2_env_t **env,
                    axis2_woden_qname_or_token_any_attr_t *
                            list_token_attr_impl,
                    axis2_hash_t *methods)
{
    axis2_woden_qname_or_token_any_attr_impl_t *
            list_token_attr_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, list_token_attr_impl, 
            AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    list_token_attr_impl_l = 
            (axis2_woden_qname_or_token_any_attr_impl_t *) 
                list_token_attr_impl;
    
    list_token_attr->ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_qname_or_token_any_attr_ops_t));
    list_token_attr->ops->free = 
                axis2_hash_get(methods, "free", AXIS2_HASH_KEY_STRING);
    list_token_attr->ops->get_base_impl = 
                list_token_attr_impl_l->
                list_token_attr.ops->get_base_impl;
    list_token_attr->ops->is_qname = 
        list_token_attr_impl_l->list_token_attr.ops->is_qname;
    list_token_attr->ops->is_token = 
        list_token_attr_impl_l->list_token_attr.ops->is_token;
    list_token_attr->ops->get_qnames = 
        list_token_attr_impl_l->list_token_attr.ops->get_qnames;
    list_token_attr->ops->get_token = 
        list_token_attr_impl_l->list_token_attr.ops->get_token;
    list_token_attr->ops->convert = 
        list_token_attr_impl_l->list_token_attr.ops->convert;
    
    return axis2_woden_xml_attr_resolve_methods(&(list_token_attr->
                base), env, list_token_attr_impl_l->xml_attr, 
                    methods);
}

axis2_bool_t AXIS2_CALL
axis2_woden_qname_or_token_any_attr_is_qname(
                                    void *list_token_attr,
                                    axis2_env_t **env) 
{
    axis2_woden_qname_or_token_any_attr_impl_t *
            list_token_attr_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    list_token_attr_impl = INTF_TO_IMPL(list_token_attr);
    if(AXIS2_FALSE == list_token_attr_impl->is_token)
        return AXIS2_TRUE;
    return AXIS2_FALSE;
}

axis2_bool_t AXIS2_CALL
axis2_woden_qname_or_token_any_attr_is_token(
                                    void *list_token_attr,
                                    axis2_env_t **env) 
{
    axis2_woden_qname_or_token_any_attr_impl_t *
            list_token_attr_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    list_token_attr_impl = INTF_TO_IMPL(list_token_attr);

    if(AXIS2_TRUE == 
            AXIS2_WODEN_XML_ATTR_IS_VALID(list_token_attr_impl->xml_attr, env) && 
            list_token_attr_impl->is_token)
    {
        return AXIS2_TRUE;
    } else 
    {
        return AXIS2_FALSE;
    }
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_qname_or_token_any_attr_get_qnames(
                                    void *list_token_attr,
                                    axis2_env_t **env)
{
    axis2_woden_qname_or_token_any_attr_impl_t *
            list_token_attr_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    list_token_attr_impl = INTF_TO_IMPL(list_token_attr);
    return (axis2_array_list_t *) AXIS2_WODEN_XML_ATTR_GET_CONTENT(
            list_token_attr_impl->xml_attr, env);
}

axis2_char_t *AXIS2_CALL
axis2_woden_qname_or_token_any_attr_get_token(
                                    void *list_token_attr,
                                    axis2_env_t **env)
{
    axis2_woden_qname_or_token_any_attr_impl_t *
            list_token_attr_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    list_token_attr_impl = INTF_TO_IMPL(list_token_attr);
    if(AXIS2_TRUE == 
            axis2_woden_qname_or_token_any_attr_is_token(list_token_attr, 
                env))
    {
        return (axis2_char_t *) AXIS2_WODEN_XML_ATTR_GET_CONTENT(
                list_token_attr_impl->xml_attr, env);
    } else 
    {
        return NULL;
    }
}

void *AXIS2_CALL
axis2_woden_qname_or_token_any_attr_convert(
                                void *list_token_attr,
                                axis2_env_t **env,
                                axis2_om_element_t *owner_el,
                                axis2_om_node_t *owner_node,
                                axis2_char_t *attr_value)
{
    axis2_woden_qname_or_token_any_attr_impl_t *
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
        AXIS2_WODEN_XML_ATTR_SET_VALID(list_token_attr_impl->
                xml_attr, env, AXIS2_FALSE);
        /* TODO handler error */
    }
    
    return qn_array;
}

