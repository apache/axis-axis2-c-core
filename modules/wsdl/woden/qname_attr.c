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

#include <woden/axis2_woden_qname_attr.h>
#include <axis2_om_element.h>
#include <axis2_om_node.h>

typedef struct axis2_woden_qname_attr_impl axis2_woden_qname_attr_impl_t;

/** 
 * @brief Qname Attribute Struct Impl
 *	Axis2 Qname Attribute  
 */ 
struct axis2_woden_qname_attr_impl
{
    axis2_woden_qname_attr_t qname_attr;
    axis2_woden_xml_attr_t *xml_attr;
    axis2_hash_t *methods;
};

#define INTF_TO_IMPL(qname_attr) \
    ((axis2_woden_qname_attr_impl_t *) qname_attr)

axis2_status_t AXIS2_CALL 
axis2_woden_qname_attr_free(void *qname_attr,
                        axis2_env_t **env);

axis2_woden_xml_attr_t *AXIS2_CALL
axis2_woden_qname_attr_get_base_impl(void *qname_attr,
                                axis2_env_t **env);

axis2_qname_t *AXIS2_CALL
axis2_woden_qname_attr_get_qname(void *qname_attr,
                                    axis2_env_t **env);

void *AXIS2_CALL
axis2_woden_qname_attr_convert(void *qname_attr,
                                axis2_env_t **env,
                                axis2_om_element_t *owner_el,
                                axis2_om_node_t *owner_node,
                                axis2_char_t *attr_value);

/*
 * TODO This constructor is not used for extension attributes, but may be useful if
 * parsing of native WSDL attributes is changed to use the XMLAttr interface.
 */
AXIS2_DECLARE(axis2_woden_qname_attr_t *)
axis2_woden_qname_attr_create(axis2_env_t **env,
                                axis2_om_element_t *owner_el,
                                axis2_om_node_t *owner_node,
                                axis2_qname_t *attr_type,
                                axis2_char_t *attr_value)
{
    axis2_woden_qname_attr_impl_t *qname_attr_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
     
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    qname_attr_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_qname_attr_impl_t));

    qname_attr_impl->xml_attr = NULL;
    qname_attr_impl->methods = NULL;
    qname_attr_impl->qname_attr.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_qname_attr_ops_t));

    qname_attr_impl->qname_attr.ops->free = 
        axis2_woden_qname_attr_free;
    qname_attr_impl->qname_attr.ops->get_base_impl = 
        axis2_woden_qname_attr_get_base_impl;
    qname_attr_impl->qname_attr.ops->get_qname = 
        axis2_woden_qname_attr_get_qname;
    qname_attr_impl->qname_attr.ops->convert = 
        axis2_woden_qname_attr_convert;
    
    
    qname_attr_impl->methods = axis2_hash_make(env);
    if(!qname_attr_impl->methods) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(qname_attr_impl->methods, "free", 
            AXIS2_HASH_KEY_STRING, axis2_woden_qname_attr_free);
    axis2_hash_set(qname_attr_impl->methods, "get_qname", 
            AXIS2_HASH_KEY_STRING, axis2_woden_qname_attr_get_qname);
    axis2_hash_set(qname_attr_impl->methods, "convert", 
            AXIS2_HASH_KEY_STRING, axis2_woden_qname_attr_convert);

    qname_attr_impl->xml_attr = axis2_woden_xml_attr_create(env, owner_el, 
            owner_node, attr_type, attr_value);
    status = axis2_woden_xml_attr_resolve_methods(&(qname_attr_impl->
                qname_attr.base), env, qname_attr_impl->
                xml_attr, qname_attr_impl->methods);
    if(AXIS2_SUCCESS != status) return NULL;
    return &(qname_attr_impl->qname_attr);
}

axis2_status_t AXIS2_CALL
axis2_woden_qname_attr_free(void *qname_attr,
                        axis2_env_t **env)
{
    axis2_woden_qname_attr_impl_t *qname_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    qname_attr_impl = INTF_TO_IMPL(qname_attr);

    if(qname_attr_impl->methods)
    {
        axis2_hash_free(qname_attr_impl->methods, env);
        qname_attr_impl->methods = NULL;
    }  

    if(qname_attr_impl->xml_attr)
    {
        AXIS2_WODEN_XML_ATTR_FREE(qname_attr_impl->xml_attr, env);
        qname_attr_impl->xml_attr = NULL;
    }
    
    if((&(qname_attr_impl->qname_attr))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(qname_attr_impl->qname_attr))->ops);
        (&(qname_attr_impl->qname_attr))->ops = NULL;
    }

    if(qname_attr_impl)
    {
        AXIS2_FREE((*env)->allocator, qname_attr_impl);
        qname_attr_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_woden_xml_attr_t *AXIS2_CALL
axis2_woden_qname_attr_get_base_impl(void *qname_attr,
                                axis2_env_t **env)
{
    axis2_woden_qname_attr_impl_t *qname_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    qname_attr_impl = INTF_TO_IMPL(qname_attr);

    return qname_attr_impl->xml_attr;
}

axis2_status_t AXIS2_CALL
axis2_woden_qname_attr_resolve_methods(
                    axis2_woden_qname_attr_t *qname_attr,
                    axis2_env_t **env,
                    axis2_woden_qname_attr_t *qname_attr_impl,
                    axis2_hash_t *methods)
{
    axis2_woden_qname_attr_impl_t *qname_attr_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname_attr_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    qname_attr_impl_l = (axis2_woden_qname_attr_impl_t *) qname_attr_impl;
    
    qname_attr->ops = AXIS2_MALLOC((*env)->allocator, 
                            sizeof(axis2_woden_qname_attr_ops_t));
    qname_attr->ops->free = 
                axis2_hash_get(methods, "free", AXIS2_HASH_KEY_STRING);
    qname_attr->ops->get_base_impl = 
                qname_attr_impl_l->qname_attr.ops->get_base_impl;
    qname_attr->ops->get_qname = qname_attr_impl_l->qname_attr.ops->get_qname;
    qname_attr->ops->convert = qname_attr_impl_l->qname_attr.ops->convert;
    
    return axis2_woden_xml_attr_resolve_methods(&(qname_attr->base), 
            env, qname_attr_impl_l->xml_attr, methods);
}

axis2_qname_t *AXIS2_CALL
axis2_woden_qname_attr_get_qname(void *qname_attr,
                                    axis2_env_t **env)
{
    axis2_woden_qname_attr_impl_t *qname_attr_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    qname_attr_impl = INTF_TO_IMPL(qname_attr);
    return (axis2_qname_t *) AXIS2_WODEN_XML_ATTR_GET_CONTENT(
            qname_attr_impl->xml_attr, env);
}

void *AXIS2_CALL
axis2_woden_qname_attr_convert(void *qname_attr,
                                axis2_env_t **env,
                                axis2_om_element_t *owner_el,
                                axis2_om_node_t *owner_node,
                                axis2_char_t *attr_value)
{
    axis2_woden_qname_attr_impl_t *qname_attr_impl = NULL;
    axis2_qname_t *qn = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    qname_attr_impl = INTF_TO_IMPL(qname_attr);

    if(attr_value)
    {
        /* TODO qn = axis2_dom_utils_get_qname(attr_value, owner_el, owner_node); */
    }
    
    if(!qn)
    {
        AXIS2_WODEN_XML_ATTR_SET_VALID(qname_attr_impl->xml_attr, env, AXIS2_FALSE);
        /* TODO handler error */
    }
    
    return qn;
}

