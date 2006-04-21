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

#include <woden/axis2_woden_bool_attr.h>
#include <axis2_om_element.h>
#include <axis2_om_node.h>

typedef struct axis2_woden_bool_attr_impl axis2_woden_bool_attr_impl_t;

/** 
 * @brief Bool Attribute Struct Impl
 *	Axis2 Bool Attribute  
 */ 
struct axis2_woden_bool_attr_impl
{
    axis2_woden_bool_attr_t bool_attr;
    axis2_woden_xml_attr_t *xml_attr;
    axis2_hash_t *methods;
};

#define INTF_TO_IMPL(bool_attr) \
    ((axis2_woden_bool_attr_impl_t *) bool_attr)

axis2_status_t AXIS2_CALL 
axis2_woden_bool_attr_free(void *bool_attr,
                        axis2_env_t **env);

axis2_woden_xml_attr_t *AXIS2_CALL
axis2_woden_bool_attr_get_base_impl(void *bool_attr,
                                axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_woden_bool_attr_get_boolean(void *bool_attr,
                                    axis2_env_t **env);

void *AXIS2_CALL
axis2_woden_bool_attr_convert(void *bool_attr,
                                axis2_env_t **env,
                                axis2_om_element_t *owner_el,
                                axis2_om_node_t *owner_node,
                                axis2_char_t *attr_value);

/*
 * TODO This constructor is not used for extension attributes, but may be useful if
 * parsing of native WSDL attributes is changed to use the XMLAttr interface.
 */
AXIS2_DECLARE(axis2_woden_bool_attr_t *)
axis2_woden_bool_attr_create(axis2_env_t **env,
                                axis2_om_element_t *owner_el,
                                axis2_om_node_t *owner_node,
                                axis2_qname_t *attr_type,
                                axis2_char_t *attr_value)
{
    axis2_woden_bool_attr_impl_t *bool_attr_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
     
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    bool_attr_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_bool_attr_impl_t));

    bool_attr_impl->xml_attr = NULL;
    bool_attr_impl->methods = NULL;
    bool_attr_impl->bool_attr.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_bool_attr_ops_t));

    bool_attr_impl->bool_attr.ops->free = 
        axis2_woden_bool_attr_free;
    bool_attr_impl->bool_attr.ops->get_base_impl = 
        axis2_woden_bool_attr_get_base_impl;
    bool_attr_impl->bool_attr.ops->get_boolean = 
        axis2_woden_bool_attr_get_boolean;
    bool_attr_impl->bool_attr.ops->convert = 
        axis2_woden_bool_attr_convert;
    
    
    bool_attr_impl->methods = axis2_hash_make(env);
    if(!bool_attr_impl->methods) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(bool_attr_impl->methods, "free", 
            AXIS2_HASH_KEY_STRING, axis2_woden_bool_attr_free);
    axis2_hash_set(bool_attr_impl->methods, "get_boolean", 
            AXIS2_HASH_KEY_STRING, axis2_woden_bool_attr_get_boolean);
    axis2_hash_set(bool_attr_impl->methods, "convert", 
            AXIS2_HASH_KEY_STRING, axis2_woden_bool_attr_convert);

    bool_attr_impl->xml_attr = axis2_woden_xml_attr_create(env, owner_el, 
            owner_node, attr_type, attr_value);
    status = axis2_woden_xml_attr_resolve_methods(&(bool_attr_impl->
                bool_attr.base), env, bool_attr_impl->
                xml_attr, bool_attr_impl->methods);
    if(AXIS2_SUCCESS != status) return NULL;
    return &(bool_attr_impl->bool_attr);
}

axis2_status_t AXIS2_CALL
axis2_woden_bool_attr_free(void *bool_attr,
                        axis2_env_t **env)
{
    axis2_woden_bool_attr_impl_t *bool_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    bool_attr_impl = INTF_TO_IMPL(bool_attr);

    if(bool_attr_impl->methods)
    {
        axis2_hash_free(bool_attr_impl->methods, env);
        bool_attr_impl->methods = NULL;
    }  

    if(bool_attr_impl->xml_attr)
    {
        AXIS2_WODEN_XML_ATTR_FREE(bool_attr_impl->xml_attr, env);
        bool_attr_impl->xml_attr = NULL;
    }
    
    if((&(bool_attr_impl->bool_attr))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(bool_attr_impl->bool_attr))->ops);
        (&(bool_attr_impl->bool_attr))->ops = NULL;
    }

    if(bool_attr_impl)
    {
        AXIS2_FREE((*env)->allocator, bool_attr_impl);
        bool_attr_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_woden_xml_attr_t *AXIS2_CALL
axis2_woden_bool_attr_get_base_impl(void *bool_attr,
                                axis2_env_t **env)
{
    axis2_woden_bool_attr_impl_t *bool_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    bool_attr_impl = INTF_TO_IMPL(bool_attr);

    return bool_attr_impl->xml_attr;
}

axis2_status_t AXIS2_CALL
axis2_woden_bool_attr_resolve_methods(
                    axis2_woden_bool_attr_t *bool_attr,
                    axis2_env_t **env,
                    axis2_woden_bool_attr_t *bool_attr_impl,
                    axis2_hash_t *methods)
{
    axis2_woden_bool_attr_impl_t *bool_attr_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, bool_attr_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    bool_attr_impl_l = (axis2_woden_bool_attr_impl_t *) bool_attr_impl;
    
    bool_attr->ops = AXIS2_MALLOC((*env)->allocator, 
                            sizeof(axis2_woden_bool_attr_ops_t));
    bool_attr->ops->free = 
                axis2_hash_get(methods, "free", AXIS2_HASH_KEY_STRING);
    bool_attr->ops->get_base_impl = 
                bool_attr_impl_l->bool_attr.ops->get_base_impl;
    bool_attr->ops->get_boolean = bool_attr_impl_l->bool_attr.ops->get_boolean;
    bool_attr->ops->convert = bool_attr_impl_l->bool_attr.ops->convert;
    
    return axis2_woden_xml_attr_resolve_methods(&(bool_attr->base), 
            env, bool_attr_impl_l->xml_attr, methods);
}

axis2_bool_t AXIS2_CALL
axis2_woden_bool_attr_get_boolean(void *bool_attr,
                                    axis2_env_t **env)
{
    axis2_woden_bool_attr_impl_t *bool_attr_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    bool_attr_impl = INTF_TO_IMPL(bool_attr);
    /* TODO return AXIS2_WODEN_XML_ATTR_GET_CONTENT(
            bool_attr_impl->xml_attr, env);
     */
    return AXIS2_FALSE;
}

void *AXIS2_CALL
axis2_woden_bool_attr_convert(void *bool_attr,
                                axis2_env_t **env,
                                axis2_om_element_t *owner_el,
                                axis2_om_node_t *owner_node,
                                axis2_char_t *attr_value)
{
    axis2_woden_bool_attr_impl_t *bool_attr_impl = NULL;
    axis2_qname_t *qn = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    bool_attr_impl = INTF_TO_IMPL(bool_attr);

    if(attr_value)
    {
        /* TODO */
    }
    
    if(!qn)
    {
        AXIS2_WODEN_XML_ATTR_SET_VALID(bool_attr_impl->xml_attr, env, AXIS2_FALSE);
        /* TODO handler error */
    }
    
    return qn;
}

