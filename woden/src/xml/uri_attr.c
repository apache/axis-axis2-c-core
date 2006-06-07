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

#include <woden/xml/axis2_woden_uri_attr.h>
#include <axis2_uri.h>
#include <axis2_om_element.h>
#include <axis2_om_node.h>

typedef struct axis2_woden_uri_attr_impl axis2_woden_uri_attr_impl_t;

/** 
 * @brief URI Attribute Struct Impl
 *   Axis2 URI Attribute  
 */ 
struct axis2_woden_uri_attr_impl
{
    axis2_woden_uri_attr_t uri_attr;
    axis2_woden_xml_attr_t *xml_attr;
    axis2_woden_obj_types_t obj_type;
    axis2_hash_t *methods;
};

#define INTF_TO_IMPL(uri_attr) \
    ((axis2_woden_uri_attr_impl_t *) uri_attr)

axis2_status_t AXIS2_CALL 
axis2_woden_uri_attr_free(
        void *uri_attr,
        const axis2_env_t *env);

axis2_woden_obj_types_t AXIS2_CALL 
axis2_woden_uri_attr_type(
        void *uri_attr,
        const axis2_env_t *env);

axis2_woden_xml_attr_t *AXIS2_CALL
axis2_woden_uri_attr_get_base_impl(
        void *uri_attr,
        const axis2_env_t *env);

axis2_uri_t *AXIS2_CALL
axis2_woden_uri_attr_get_uri(
        void *uri_attr,
        const axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_uri_attr_convert(
        void *uri_attr,
        const axis2_env_t *env,
        axis2_om_element_t *owner_el,
        axis2_om_node_t *owner_node,
        axis2_char_t *attr_value);

/*
 * TODO This constructor is not used for extension attributes, but may be useful if
 * parsing of native WSDL attributes is changed to use the XMLAttr interface.
 */
AXIS2_EXTERN axis2_woden_uri_attr_t * AXIS2_CALL
axis2_woden_uri_attr_create(
        const axis2_env_t *env,
        axis2_om_element_t *owner_el,
        axis2_om_node_t *owner_node,
        axis2_qname_t *attr_type,
        axis2_char_t *attr_value)
{
    axis2_woden_uri_attr_impl_t *uri_attr_impl = NULL;
     
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    uri_attr_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_woden_uri_attr_impl_t));

    uri_attr_impl->obj_type = AXIS2_WODEN_URI_ATTR;
    uri_attr_impl->xml_attr = NULL;
    uri_attr_impl->methods = NULL;
    uri_attr_impl->uri_attr.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_uri_attr_ops_t));

    uri_attr_impl->uri_attr.ops->free = 
        axis2_woden_uri_attr_free;
    uri_attr_impl->uri_attr.ops->type = 
        axis2_woden_uri_attr_type;
    uri_attr_impl->uri_attr.ops->get_base_impl = 
        axis2_woden_uri_attr_get_base_impl;
    uri_attr_impl->uri_attr.ops->get_uri = 
        axis2_woden_uri_attr_get_uri;
    uri_attr_impl->uri_attr.ops->convert = 
        axis2_woden_uri_attr_convert;
    
    
    uri_attr_impl->methods = axis2_hash_make(env);
    if(!uri_attr_impl->methods) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(uri_attr_impl->methods, "free", 
            AXIS2_HASH_KEY_STRING, axis2_woden_uri_attr_free);
    axis2_hash_set(uri_attr_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_woden_uri_attr_type);
    axis2_hash_set(uri_attr_impl->methods, "get_uri", 
            AXIS2_HASH_KEY_STRING, axis2_woden_uri_attr_get_uri);
    axis2_hash_set(uri_attr_impl->methods, "convert", 
            AXIS2_HASH_KEY_STRING, axis2_woden_uri_attr_convert);

    uri_attr_impl->xml_attr = axis2_woden_xml_attr_create(env, owner_el, 
            owner_node, attr_type, attr_value);
    
    return &(uri_attr_impl->uri_attr);
}

axis2_woden_obj_types_t AXIS2_CALL
axis2_woden_uri_attr_type(
        void *uri_attr,
        const axis2_env_t *env)
{
    axis2_woden_uri_attr_impl_t *uri_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    uri_attr_impl = INTF_TO_IMPL(uri_attr);

    return uri_attr_impl->obj_type;
}

axis2_status_t AXIS2_CALL
axis2_woden_uri_attr_free(
        void *uri_attr,
        const axis2_env_t *env)
{
    axis2_woden_uri_attr_impl_t *uri_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    uri_attr_impl = INTF_TO_IMPL(uri_attr);

    if(uri_attr_impl->methods)
    {
        axis2_hash_free(uri_attr_impl->methods, env);
        uri_attr_impl->methods = NULL;
    }  

    if(uri_attr_impl->xml_attr)
    {
        AXIS2_WODEN_XML_ATTR_FREE(uri_attr_impl->xml_attr, env);
        uri_attr_impl->xml_attr = NULL;
    }
    
    if((&(uri_attr_impl->uri_attr))->ops)
    {
        AXIS2_FREE(env->allocator, (&(uri_attr_impl->uri_attr))->ops);
        (&(uri_attr_impl->uri_attr))->ops = NULL;
    }

    if(uri_attr_impl)
    {
        AXIS2_FREE(env->allocator, uri_attr_impl);
        uri_attr_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_woden_xml_attr_t *AXIS2_CALL
axis2_woden_uri_attr_get_base_impl(
        void *uri_attr,
        const axis2_env_t *env)
{
    axis2_woden_uri_attr_impl_t *uri_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    uri_attr_impl = INTF_TO_IMPL(uri_attr);

    return uri_attr_impl->xml_attr;
}

axis2_status_t AXIS2_CALL
axis2_woden_uri_attr_resolve_methods(
        axis2_woden_uri_attr_t *uri_attr,
        const axis2_env_t *env,
        axis2_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    
    uri_attr->ops->free = axis2_hash_get(methods, 
            "free", AXIS2_HASH_KEY_STRING);
    uri_attr->ops->to_uri_attr_free = axis2_hash_get(methods, 
            "to_uri_attr_free", AXIS2_HASH_KEY_STRING);
    uri_attr->ops->type = axis2_hash_get(methods, 
            "type", AXIS2_HASH_KEY_STRING);
    uri_attr->ops->get_uri = axis2_hash_get(methods, 
            "get_uri", AXIS2_HASH_KEY_STRING);
    uri_attr->ops->convert = axis2_hash_get(methods, 
            "convert", AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;
}

axis2_uri_t *AXIS2_CALL
axis2_woden_uri_attr_get_uri(
        void *uri_attr,
        const axis2_env_t *env)
{
    axis2_woden_uri_attr_impl_t *uri_attr_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    uri_attr_impl = INTF_TO_IMPL(uri_attr);
    return (axis2_uri_t *) AXIS2_WODEN_XML_ATTR_GET_CONTENT(
            uri_attr_impl->xml_attr, env);
}

void *AXIS2_CALL
axis2_woden_uri_attr_convert(
        void *uri_attr,
        const axis2_env_t *env,
        axis2_om_element_t *owner_el,
        axis2_om_node_t *owner_node,
        axis2_char_t *attr_value)
{
    axis2_woden_uri_attr_impl_t *uri_attr_impl = NULL;
    axis2_uri_t *url = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    uri_attr_impl = INTF_TO_IMPL(uri_attr);

    if(attr_value)
    {
         /* TODO url = axis2_uri_create(env, attr_value);*/
    }
    
    if(!url)
    {
        AXIS2_WODEN_XML_ATTR_SET_VALID(uri_attr_impl->xml_attr, env, AXIS2_FALSE);
        /* TODO handler error */
    }
    
    return url;
}

