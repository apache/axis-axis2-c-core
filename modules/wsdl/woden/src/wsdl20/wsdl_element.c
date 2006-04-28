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
 
#include <woden/axis2_woden_wsdl_element.h>
#include <woden/axis2_woden_element_extensible.h>
#include <woden/axis2_woden_attr_extensible.h>
#include <woden/axis2_woden_xml_attr.h>
#include <woden/axis2_woden_ext_element.h>
#include <axis2_url.h>
#include <axis2_hash.h>

typedef struct axis2_woden_wsdl_element_impl axis2_woden_wsdl_element_impl_t;

/** 
 * @brief Wsdl Element Struct Impl
 *	Axis2 Wsdl Element  
 */ 
struct axis2_woden_wsdl_element_impl
{
    axis2_woden_wsdl_element_t extensible;
    axis2_woden_attr_extensible_t *f_attr_ext;
    axis2_woden_element_extensible_t *f_elem_ext;
};

#define INTF_TO_IMPL(extensible) \
    ((axis2_woden_wsdl_element_impl_t *) extensible)

axis2_status_t AXIS2_CALL 
axis2_woden_wsdl_element_free(void *extensible,
                axis2_env_t **envv);

axis2_status_t AXIS2_CALL 
axis2_woden_wsdl_element_set_ext_attr(
                                    void *extensible,
                                    axis2_env_t **env,
                                    axis2_qname_t *attr_type,
                                    axis2_woden_xml_attr_t *attr); 

axis2_woden_xml_attr_t *AXIS2_CALL 
axis2_woden_wsdl_element_get_ext_attr(
                                    void *extensible,
                                    axis2_env_t **env,
                                    axis2_qname_t *attr_type); 

axis2_array_list_t *AXIS2_CALL 
axis2_woden_wsdl_element_get_ext_attrs(void *extensible,
                                                axis2_env_t **env); 

axis2_array_list_t *AXIS2_CALL 
axis2_woden_wsdl_element_get_ext_attrs_for_namespace(void *extensible,
                                                        axis2_env_t **env,
                                                        axis2_url_t *namespc);

axis2_bool_t AXIS2_CALL 
axis2_woden_wsdl_element_has_ext_attrs_for_namespace(
                                                       void *extensible,
                                                       axis2_env_t **env,
                                                       axis2_url_t *namespc);

axis2_status_t AXIS2_CALL 
axis2_woden_wsdl_element_add_ext_element(
                                    void *extensible,
                                    axis2_env_t **env,
                                    axis2_woden_ext_element_t *ext_el); 

axis2_status_t AXIS2_CALL 
axis2_woden_wsdl_element_remove_ext_element(
                                    void *extensible,
                                    axis2_env_t **env,
                                    axis2_woden_ext_element_t *ext_el); 

axis2_array_list_t *AXIS2_CALL 
axis2_woden_wsdl_element_get_ext_elements(void *extensible,
                                                axis2_env_t **env); 

axis2_array_list_t *AXIS2_CALL 
axis2_woden_wsdl_element_get_ext_elements_of_type(void *extensible,
                                                  axis2_env_t **env,
                                                  axis2_qname_t *elem_type);

axis2_bool_t AXIS2_CALL 
axis2_woden_wsdl_element_has_ext_elements_for_namespace(
                                                       void *extensible,
                                                       axis2_env_t **env,
                                                       axis2_url_t *namespc);

axis2_status_t AXIS2_CALL 
axis2_woden_wsdl_element_add_ext_element(
                                    void *extensible,
                                    axis2_env_t **env,
                                    axis2_woden_ext_element_t *ext_el); 

axis2_status_t AXIS2_CALL 
axis2_woden_wsdl_element_remove_ext_element(
                                    void *extensible,
                                    axis2_env_t **env,
                                    axis2_woden_ext_element_t *ext_el);

axis2_array_list_t *AXIS2_CALL 
axis2_woden_wsdl_element_get_ext_elements(void *extensible,
                                            axis2_env_t **env); 

axis2_array_list_t *AXIS2_CALL 
axis2_woden_wsdl_element_get_ext_elements_of_type(void *extensible,
                                                    axis2_env_t **env,
                                                    axis2_qname_t *ext_type);

axis2_bool_t AXIS2_CALL 
axis2_woden_wsdl_element_has_ext_elements_for_namespace(void *extensible,
                                                            axis2_env_t **env,
                                                            axis2_url_t *namespc);


AXIS2_DECLARE(axis2_woden_wsdl_element_t *)
axis2_woden_wsdl_element_create(axis2_env_t **env)
{
    axis2_woden_wsdl_element_impl_t *extensible_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    extensible_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_wsdl_element_impl_t));

    extensible_impl->f_attr_ext = NULL;
    extensible_impl->f_elem_ext = NULL;

    extensible_impl->f_attr_ext = axis2_woden_attr_extensible_create(env);
    if(!extensible_impl->f_attr_ext)
    {
        return NULL;
    }
    extensible_impl->f_elem_ext = axis2_woden_element_extensible_create(env);
    if(!extensible_impl->f_elem_ext)
    {
        return NULL;
    }
    extensible_impl->extensible.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_wsdl_element_ops_t)); 
    
    extensible_impl->extensible.ops->free = 
        axis2_woden_wsdl_element_free;
    extensible_impl->extensible.ops->set_ext_attr = 
        axis2_woden_wsdl_element_set_ext_attr;
    extensible_impl->extensible.ops->get_ext_attr = 
        axis2_woden_wsdl_element_get_ext_attr;
    extensible_impl->extensible.ops->get_ext_attrs = 
        axis2_woden_wsdl_element_get_ext_attrs;
    extensible_impl->extensible.ops->get_ext_attrs_for_namespace = 
        axis2_woden_wsdl_element_get_ext_attrs_for_namespace;
    extensible_impl->extensible.ops->has_ext_attrs_for_namespace = 
        axis2_woden_wsdl_element_has_ext_attrs_for_namespace;
    extensible_impl->extensible.ops->add_ext_element = 
        axis2_woden_wsdl_element_add_ext_element;
    extensible_impl->extensible.ops->remove_ext_element = 
        axis2_woden_wsdl_element_remove_ext_element;
    extensible_impl->extensible.ops->get_ext_elements = 
        axis2_woden_wsdl_element_get_ext_elements;
    extensible_impl->extensible.ops->get_ext_elements_of_type = 
        axis2_woden_wsdl_element_get_ext_elements_of_type;
    extensible_impl->extensible.ops->has_ext_elements_for_namespace = 
        axis2_woden_wsdl_element_has_ext_elements_for_namespace;
    
    return &(extensible_impl->extensible);
}

axis2_status_t AXIS2_CALL
axis2_woden_wsdl_element_free(void *extensible,
                axis2_env_t **env)
{
    axis2_woden_wsdl_element_impl_t *extensible_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);

    if(extensible_impl->f_attr_ext)
    {
        AXIS2_WODEN_ATTR_EXTENSIBLE_FREE(extensible_impl->f_attr_ext, env);
        extensible_impl->f_attr_ext = NULL;
    }
    
    if(extensible_impl->f_elem_ext)
    {
        AXIS2_WODEN_ELEMENT_EXTENSIBLE_FREE(extensible_impl->f_elem_ext, env);
        extensible_impl->f_elem_ext = NULL;
    }

    if((&(extensible_impl->extensible))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(extensible_impl->extensible))->ops);
        (&(extensible_impl->extensible))->ops = NULL;
    }

    if(extensible_impl)
    {
        AXIS2_FREE((*env)->allocator, extensible_impl);
        extensible_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_wsdl_element_resolve_methods(axis2_woden_wsdl_element_t *extensible,
                                axis2_env_t **env,
                                axis2_woden_wsdl_element_t *extensible_impl,
                                axis2_hash_t *methods)
{
    axis2_woden_wsdl_element_impl_t *extensible_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, extensible_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    extensible_impl_l = (axis2_woden_wsdl_element_impl_t *) extensible_impl;
    
    extensible->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_woden_wsdl_element_ops_t));
    extensible->ops->free = axis2_hash_get(methods, "free", AXIS2_HASH_KEY_STRING);
    extensible->ops->set_ext_attr = 
        extensible_impl_l->extensible.ops->set_ext_attr;
    extensible->ops->get_ext_attr = 
        extensible_impl_l->extensible.ops->get_ext_attr;
    extensible->ops->get_ext_attrs = 
        extensible_impl_l->extensible.ops->get_ext_attrs;
    extensible->ops->get_ext_attrs_for_namespace = 
        extensible_impl_l->extensible.ops->get_ext_attrs_for_namespace;
    extensible->ops->has_ext_attrs_for_namespace = 
        extensible_impl_l->extensible.ops->has_ext_attrs_for_namespace;
    extensible->ops->add_ext_element = 
        extensible_impl_l->extensible.ops->add_ext_element;
    extensible->ops->remove_ext_element = 
        extensible_impl_l->extensible.ops->remove_ext_element;
    extensible->ops->get_ext_elements = 
        extensible_impl_l->extensible.ops->get_ext_elements;
    extensible->ops->get_ext_elements_of_type = 
        extensible_impl_l->extensible.ops->get_ext_elements_of_type;
    extensible->ops->has_ext_elements_for_namespace = 
        extensible_impl_l->extensible.ops->has_ext_elements_for_namespace;

    return AXIS2_SUCCESS;    
}

axis2_status_t AXIS2_CALL 
axis2_woden_wsdl_element_set_ext_attr(
                                    void *extensible,
                                    axis2_env_t **env,
                                    axis2_qname_t *attr_type,
                                    axis2_woden_xml_attr_t *attr)
{
    axis2_woden_wsdl_element_impl_t *extensible_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, attr_type, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, attr, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);
    return AXIS2_WODEN_ATTR_EXTENSIBLE_SET_EXT_ATTR(extensible_impl->f_attr_ext, 
            env, attr_type, attr);    
}

axis2_woden_xml_attr_t *AXIS2_CALL 
axis2_woden_wsdl_element_get_ext_attr(void *extensible,
                                            axis2_env_t **env,
                                            axis2_qname_t *attr_type) 
{
    axis2_woden_wsdl_element_impl_t *extensible_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, attr_type, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);
    
    return AXIS2_WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTR(extensible_impl->f_attr_ext, 
            env, attr_type);
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_wsdl_element_get_ext_attrs(void *extensible,
                                                axis2_env_t **env) 
{
    axis2_woden_wsdl_element_impl_t *extensible_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);

    return AXIS2_WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTRS(extensible_impl->f_attr_ext, 
            env);
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_wsdl_element_get_ext_attrs_for_namespace(void *extensible,
                                                        axis2_env_t **env,
                                                        axis2_url_t *namespc) 
{
    axis2_woden_wsdl_element_impl_t *extensible_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespc, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);

    return AXIS2_WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTRS_FOR_NAMESPACE(
            extensible_impl->f_attr_ext, env, namespc);
}

axis2_bool_t AXIS2_CALL 
axis2_woden_wsdl_element_has_ext_attrs_for_namespace(void *extensible,
                                                            axis2_env_t **env,
                                                            axis2_url_t *namespc)
{
    axis2_woden_wsdl_element_impl_t *extensible_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespc, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);

    return AXIS2_WODEN_ATTR_EXTENSIBLE_HAS_EXT_ATTRS_FOR_NAMESPACE(
            extensible_impl->f_attr_ext, env, namespc);
}

axis2_status_t AXIS2_CALL 
axis2_woden_wsdl_element_add_ext_element(
                                    void *extensible,
                                    axis2_env_t **env,
                                    axis2_woden_ext_element_t *ext_el) 
{
    axis2_woden_wsdl_element_impl_t *extensible_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, ext_el, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);
    
    return AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(extensible_impl->
            f_elem_ext, env, ext_el);
}

axis2_status_t AXIS2_CALL 
axis2_woden_wsdl_element_remove_ext_element(
                                    void *extensible,
                                    axis2_env_t **env,
                                    axis2_woden_ext_element_t *ext_el) 
{
    axis2_woden_wsdl_element_impl_t *extensible_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, ext_el, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);

    return AXIS2_WODEN_ELEMENT_EXTENSIBLE_REMOVE_EXT_ELEMENT(extensible_impl->
            f_elem_ext, env, ext_el);
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_wsdl_element_get_ext_elements(void *extensible,
                                            axis2_env_t **env) 
{
    axis2_woden_wsdl_element_impl_t *extensible_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);
    
    return AXIS2_WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS(extensible_impl->
            f_elem_ext, env);
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_wsdl_element_get_ext_elements_of_type(void *extensible,
                                                    axis2_env_t **env,
                                                    axis2_qname_t *ext_type) 
{
    axis2_woden_wsdl_element_impl_t *extensible_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, ext_type, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);

    return AXIS2_WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS_OF_TYPE(
            extensible_impl->f_elem_ext, env, ext_type);
}

axis2_bool_t AXIS2_CALL 
axis2_woden_wsdl_element_has_ext_elements_for_namespace(void *extensible,
                                                            axis2_env_t **env,
                                                            axis2_url_t *namespc)
{
    axis2_woden_wsdl_element_impl_t *extensible_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespc, AXIS2_FAILURE);
    extensible_impl = INTF_TO_IMPL(extensible);

    return AXIS2_WODEN_ELEMENT_EXTENSIBLE_HAS_EXT_ELEMENTS_FOR_NAMESPACE(
            extensible_impl->f_elem_ext, env, namespc);
}


