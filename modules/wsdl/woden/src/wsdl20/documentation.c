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

#include <woden/wsdl20/axis2_woden_documentation.h>
#include <woden/wsdl20/xml/axis2_woden_documentation_element.h>
#include <woden/wsdl20/extensions/axis2_woden_ext_element.h>
#include <woden/xml/axis2_woden_xml_attr.h>

typedef struct axis2_woden_documentation_impl axis2_woden_documentation_impl_t;

/** 
 * @brief Documentation Struct Impl
 *	Axis2 Documentation  
 */ 
struct axis2_woden_documentation_impl
{
    axis2_woden_documentation_t documentation;
    axis2_woden_obj_types_t obj_type;
    axis2_woden_wsdl_element_t *wsdl_element;
    axis2_hash_t *methods;
    void *f_content;
};

#define INTF_TO_IMPL(documentation) ((axis2_woden_documentation_impl_t *) documentation)

axis2_status_t AXIS2_CALL 
axis2_woden_documentation_free(
        void *documentation,
        axis2_env_t **env);

axis2_woden_obj_types_t AXIS2_CALL
axis2_woden_documentation_type(
        void *documentation,
        axis2_env_t **env);

axis2_woden_wsdl_element_t *AXIS2_CALL
axis2_woden_documentation_get_base_impl(
        void *documentation,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_documentation_set_content(
        void *documentation,
        axis2_env_t **env,
        void *doc_el);

void *AXIS2_CALL
axis2_woden_documentation_get_content(
        void *documentation,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_woden_documentation_set_ext_attr(
        void *documentation,
        axis2_env_t **env,
        axis2_qname_t *attr_type,
        axis2_woden_xml_attr_t *attr);

axis2_woden_xml_attr_t *AXIS2_CALL 
axis2_woden_documentation_get_ext_attr(
        void *documentation,
        axis2_env_t **env,
        axis2_qname_t *attr_type);

axis2_array_list_t *AXIS2_CALL 
axis2_woden_documentation_get_ext_attrs(
        void *documentation,
        axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL 
axis2_woden_documentation_get_ext_attrs_for_namespace(
        void *documentation,
        axis2_env_t **env,
        axis2_url_t *namespc);

axis2_bool_t AXIS2_CALL 
axis2_woden_documentation_has_ext_attrs_for_namespace(
        void *documentation,
        axis2_env_t **env,
        axis2_url_t *namespc);

axis2_status_t AXIS2_CALL 
axis2_woden_documentation_add_ext_element(
        void *documentation,
        axis2_env_t **env,
        axis2_woden_ext_element_t *ext_el);

axis2_status_t AXIS2_CALL 
axis2_woden_documentation_remove_ext_element(
        void *documentation,
        axis2_env_t **env,
        axis2_woden_ext_element_t *ext_el);

axis2_array_list_t *AXIS2_CALL 
axis2_woden_documentation_get_ext_elements(
        void *documentation,
        axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL 
axis2_woden_documentation_get_ext_elements_of_type(
        void *documentation,
        axis2_env_t **env,
        axis2_qname_t *ext_type);

axis2_bool_t AXIS2_CALL 
axis2_woden_documentation_has_ext_elements_for_namespace(
        void *documentation,
        axis2_env_t **env,
        axis2_url_t *namespc);

static axis2_woden_documentation_t *
create(axis2_env_t **env);

/************************Woden C Internal Methods******************************/
AXIS2_DECLARE(axis2_woden_documentation_t *)
axis2_woden_documentation_to_documentation_element(
        void *documentation,
        axis2_env_t **env)
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!documentation)
    {
        documentation_impl = (axis2_woden_documentation_impl_t *) create(env);
    }
    else
        documentation_impl = (axis2_woden_documentation_impl_t *) documentation;

    documentation_impl->documentation.base.documentation_element.ops = 
        AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_woden_documentation_element_ops_t));
    axis2_woden_documentation_element_resolve_methods(&(documentation_impl->
            documentation.base.documentation_element), env, 
            documentation_impl->methods);
    return documentation;
}

axis2_status_t AXIS2_CALL
axis2_woden_documentation_to_documentation_element_free(
        void *documentation,
        axis2_env_t **env)
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentation_impl = INTF_TO_IMPL(documentation);

    if(documentation_impl->documentation.base.documentation_element.ops)
    {
        AXIS2_FREE((*env)->allocator, documentation_impl->documentation.base.
                documentation_element.ops);
        documentation_impl->documentation.base.documentation_element.ops = 
            NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_documentation_t *)
axis2_woden_documentation_to_attr_extensible(
        void *documentation,
        axis2_env_t **env)
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!documentation)
    {
        documentation_impl = (axis2_woden_documentation_impl_t *) create(env);
    }
    else
        documentation_impl = (axis2_woden_documentation_impl_t *) documentation;

    documentation_impl->documentation.base.wsdl_element.base.attr_extensible.
            ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_woden_attr_extensible_ops_t));
    axis2_woden_attr_extensible_resolve_methods(&(documentation_impl->
            documentation.base.wsdl_element.base.attr_extensible), env, 
            documentation_impl->methods);
    return documentation;

}

axis2_status_t AXIS2_CALL
axis2_woden_documentation_to_attr_extensible_free(
        void *documentation,
        axis2_env_t **env)
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentation_impl = INTF_TO_IMPL(documentation);

    if(documentation_impl->documentation.base.wsdl_element.base.attr_extensible.
            ops)
    {
        AXIS2_FREE((*env)->allocator, documentation_impl->documentation.
                base.wsdl_element.base.attr_extensible.ops);
        documentation_impl->documentation.base.wsdl_element.base.
            attr_extensible.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_documentation_t *)
axis2_woden_documentation_to_element_extensible(
        void *documentation,
        axis2_env_t **env)
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!documentation)
    {
        documentation_impl = (axis2_woden_documentation_impl_t *) create(env);
    }
    else
        documentation_impl = (axis2_woden_documentation_impl_t *) documentation;
    documentation_impl->documentation.base.wsdl_element.base.
        element_extensible.ops = AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_woden_element_extensible_ops_t));
    axis2_woden_element_extensible_resolve_methods(&(documentation_impl->
            documentation.base.wsdl_element.base.element_extensible), 
            env, documentation_impl->methods);
    return documentation;

}

axis2_status_t AXIS2_CALL
axis2_woden_documentation_to_element_extensible_free(
        void *documentation,
        axis2_env_t **env)
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentation_impl = INTF_TO_IMPL(documentation);

    if(documentation_impl->documentation.base.wsdl_element.base.
            element_extensible.ops)
    {
        AXIS2_FREE((*env)->allocator, documentation_impl->documentation.base.
                wsdl_element.base.element_extensible.ops);
        documentation_impl->documentation.base.wsdl_element.base.
            element_extensible.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

/************************End of Woden C Internal Methods***********************/
static axis2_woden_documentation_t *
create(axis2_env_t **env)
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentation_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_documentation_impl_t));

    documentation_impl->wsdl_element = NULL;
    documentation_impl->obj_type = AXIS2_WODEN_DOCUMENTATION;
    documentation_impl->f_content = NULL;
    documentation_impl->methods = NULL;
    
    documentation_impl->documentation.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_documentation_ops_t));

    documentation_impl->documentation.ops->free = axis2_woden_documentation_free;
    documentation_impl->documentation.ops->type = axis2_woden_documentation_type;
    documentation_impl->documentation.ops->get_base_impl = 
        axis2_woden_documentation_get_base_impl;

    documentation_impl->methods = axis2_hash_make(env);
    if(!documentation_impl->methods) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(documentation_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_woden_documentation_free);
    axis2_hash_set(documentation_impl->methods, "to_documentation_element_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_documentation_to_documentation_element_free);
    axis2_hash_set(documentation_impl->methods, "to_attr_extensible_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_documentation_to_attr_extensible_free);
    axis2_hash_set(documentation_impl->methods, "to_element_extensible_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_documentation_to_element_extensible_free);
    axis2_hash_set(documentation_impl->methods, "type", AXIS2_HASH_KEY_STRING, 
            axis2_woden_documentation_type);
    axis2_hash_set(documentation_impl->methods, "set_content", 
            AXIS2_HASH_KEY_STRING, axis2_woden_documentation_set_content);
    axis2_hash_set(documentation_impl->methods, "get_content", 
            AXIS2_HASH_KEY_STRING, axis2_woden_documentation_get_content);
    axis2_hash_set(documentation_impl->methods, "set_ext_attr", 
            AXIS2_HASH_KEY_STRING, axis2_woden_documentation_set_ext_attr);
    axis2_hash_set(documentation_impl->methods, "get_ext_attr", 
            AXIS2_HASH_KEY_STRING, axis2_woden_documentation_get_ext_attr);
    axis2_hash_set(documentation_impl->methods, "get_ext_attrs", 
            AXIS2_HASH_KEY_STRING, axis2_woden_documentation_get_ext_attrs);
    axis2_hash_set(documentation_impl->methods, "get_ext_attrs_for_namespace", 
            AXIS2_HASH_KEY_STRING, axis2_woden_documentation_get_ext_attrs_for_namespace);
    axis2_hash_set(documentation_impl->methods, "has_ext_attrs_for_namespace", 
            AXIS2_HASH_KEY_STRING, axis2_woden_documentation_has_ext_attrs_for_namespace);
    axis2_hash_set(documentation_impl->methods, "add_ext_element", 
            AXIS2_HASH_KEY_STRING, axis2_woden_documentation_add_ext_element);
    axis2_hash_set(documentation_impl->methods, "remove_ext_element", 
            AXIS2_HASH_KEY_STRING, axis2_woden_documentation_remove_ext_element);
    axis2_hash_set(documentation_impl->methods, "get_ext_elements", 
            AXIS2_HASH_KEY_STRING, axis2_woden_documentation_get_ext_elements);
    axis2_hash_set(documentation_impl->methods, "get_ext_elements_of_type", 
            AXIS2_HASH_KEY_STRING, axis2_woden_documentation_get_ext_elements_of_type);
    axis2_hash_set(documentation_impl->methods, "has_ext_elements_for_namespace", 
            AXIS2_HASH_KEY_STRING, axis2_woden_documentation_has_ext_elements_for_namespace);
    
    return &(documentation_impl->documentation);
}

AXIS2_DECLARE(axis2_woden_documentation_t *)
axis2_woden_documentation_create(axis2_env_t **env)
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentation_impl = (axis2_woden_documentation_impl_t *) create(env);

    documentation_impl->wsdl_element = axis2_woden_wsdl_element_create(env);

    return &(documentation_impl->documentation);
}

axis2_status_t AXIS2_CALL
axis2_woden_documentation_free(void *documentation,
                        axis2_env_t **env)
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentation_impl = INTF_TO_IMPL(documentation);

    if(documentation_impl->f_content)
    {
        /*AXIS2_FREE(documentation_impl->f_content, env);*/
        documentation_impl->f_content = NULL;
    }
    
    if(documentation_impl->methods)
    {
        axis2_hash_free(documentation_impl->methods, env);
        documentation_impl->methods = NULL;
    }

    if(documentation_impl->wsdl_element)
    {
        AXIS2_WODEN_WSDL_OBJ_FREE(documentation_impl->wsdl_element, env);
        documentation_impl->wsdl_element = NULL;
    }

    if(documentation_impl->documentation.base.documentation_element.ops)
    {
        AXIS2_FREE((*env)->allocator, documentation_impl->documentation.base.
                documentation_element.ops);
        documentation_impl->documentation.base.documentation_element.ops = 
            NULL;
    }

    if(documentation_impl->documentation.base.wsdl_element.base.attr_extensible.
            ops)
    {
        AXIS2_FREE((*env)->allocator, documentation_impl->documentation.
                base.wsdl_element.base.attr_extensible.ops);
        documentation_impl->documentation.base.wsdl_element.base.
            attr_extensible.ops = NULL;
    }
    
    if(documentation_impl->documentation.base.wsdl_element.base.
            element_extensible.ops)
    {
        AXIS2_FREE((*env)->allocator, documentation_impl->documentation.base.
                wsdl_element.base.element_extensible.ops);
        documentation_impl->documentation.base.wsdl_element.base.
            element_extensible.ops = NULL;
    }

    if((&(documentation_impl->documentation))->ops)
    {
        AXIS2_FREE((*env)->allocator, documentation_impl->documentation.ops);
        (&(documentation_impl->documentation))->ops = NULL;
    }
    
    if(documentation_impl)
    {
        AXIS2_FREE((*env)->allocator, documentation_impl);
        documentation_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_woden_obj_types_t AXIS2_CALL
axis2_woden_documentation_type(void *documentation,
        axis2_env_t **env)
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentation_impl = INTF_TO_IMPL(documentation);

    return documentation_impl->obj_type;
}

axis2_woden_wsdl_element_t *AXIS2_CALL
axis2_woden_documentation_get_base_impl(void *documentation,
                                axis2_env_t **env)
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    documentation_impl = INTF_TO_IMPL(documentation);

    return documentation_impl->wsdl_element;
}

axis2_status_t AXIS2_CALL
axis2_woden_documentation_resolve_methods(
        axis2_woden_documentation_t *documentation,
        axis2_env_t **env,
        axis2_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    documentation->ops = AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_documentation_ops_t));
    documentation->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    documentation->ops->to_documentation_free = axis2_hash_get(methods, 
            "to_documentation_free", AXIS2_HASH_KEY_STRING);
    documentation->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_woden_documentation_set_ext_attr(
        void *documentation,
        axis2_env_t **env,
        axis2_qname_t *attr_type,
        axis2_woden_xml_attr_t *attr)
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;
    void *attr_ext = NULL;
    axis2_status_t status = AXIS2_FAILURE;
        
    documentation_impl = INTF_TO_IMPL(documentation);
    
    attr_ext = axis2_woden_wsdl_element_to_attr_extensible(
            documentation_impl->wsdl_element, env);
    status = AXIS2_WODEN_ATTR_EXTENSIBLE_SET_EXT_ATTR(attr_ext, env, attr_type, 
            attr);
    AXIS2_WODEN_ATTR_EXTENSIBLE_TO_ATTR_EXTENSIBLE_FREE(attr_ext, env);
    return status;
}

axis2_woden_xml_attr_t *AXIS2_CALL 
axis2_woden_documentation_get_ext_attr(
        void *documentation,
        axis2_env_t **env,
        axis2_qname_t *attr_type) 
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;
    void *attr_ext = NULL;
    void *ret = NULL;
    
    documentation_impl = INTF_TO_IMPL(documentation);
    
    attr_ext = axis2_woden_wsdl_element_to_attr_extensible(
            documentation_impl->wsdl_element, env);
    ret = AXIS2_WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTR(attr_ext, env, attr_type);
    AXIS2_WODEN_ATTR_EXTENSIBLE_TO_ATTR_EXTENSIBLE_FREE(attr_ext, env);
    return ret;
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_documentation_get_ext_attrs(
        void *documentation,
        axis2_env_t **env) 
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;
    void *attr_ext = NULL;
    void *ret = NULL;
    
    documentation_impl = INTF_TO_IMPL(documentation);

    attr_ext = axis2_woden_wsdl_element_to_attr_extensible(
            documentation_impl->wsdl_element, env);
    ret = AXIS2_WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTRS(attr_ext, env);
    AXIS2_WODEN_ATTR_EXTENSIBLE_TO_ATTR_EXTENSIBLE_FREE(attr_ext, env);
    return ret;
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_documentation_get_ext_attrs_for_namespace(
        void *documentation,
        axis2_env_t **env,
        axis2_url_t *namespc) 
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;
    void *attr_ext = NULL;
    void *ret = NULL;
    
    documentation_impl = INTF_TO_IMPL(documentation);

    attr_ext = axis2_woden_wsdl_element_to_attr_extensible(
            documentation_impl->wsdl_element, env);
    ret = AXIS2_WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTRS_FOR_NAMESPACE(
            attr_ext, env, namespc);
    AXIS2_WODEN_ATTR_EXTENSIBLE_TO_ATTR_EXTENSIBLE_FREE(attr_ext, env);
    return ret;
}

axis2_bool_t AXIS2_CALL 
axis2_woden_documentation_has_ext_attrs_for_namespace(
        void *documentation,
        axis2_env_t **env,
        axis2_url_t *namespc)
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;
    void *attr_ext = NULL;
    axis2_bool_t ret = AXIS2_FALSE;

    documentation_impl = INTF_TO_IMPL(documentation);

    attr_ext = axis2_woden_wsdl_element_to_attr_extensible(
            documentation_impl->wsdl_element, env);
    ret = AXIS2_WODEN_ATTR_EXTENSIBLE_HAS_EXT_ATTRS_FOR_NAMESPACE(
            attr_ext, env, namespc);
    AXIS2_WODEN_ATTR_EXTENSIBLE_TO_ATTR_EXTENSIBLE_FREE(attr_ext, env);
    return ret;
}

axis2_status_t AXIS2_CALL 
axis2_woden_documentation_add_ext_element(
        void *documentation,
        axis2_env_t **env,
        axis2_woden_ext_element_t *ext_el) 
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;
    void *element_ext = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    documentation_impl = INTF_TO_IMPL(documentation);
    
    element_ext = axis2_woden_wsdl_element_to_element_extensible(
            documentation_impl->wsdl_element, env);
    status = AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(element_ext, env, 
            ext_el);
    AXIS2_WODEN_ELEMENT_EXTENSIBLE_TO_ELEMENT_EXTENSIBLE_FREE(element_ext, env);
    return status;
}

axis2_status_t AXIS2_CALL 
axis2_woden_documentation_remove_ext_element(
        void *documentation,
        axis2_env_t **env,
        axis2_woden_ext_element_t *ext_el) 
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;
    void *element_ext = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    documentation_impl = INTF_TO_IMPL(documentation);

    element_ext = axis2_woden_wsdl_element_to_element_extensible(
            documentation_impl->wsdl_element, env);
    status = AXIS2_WODEN_ELEMENT_EXTENSIBLE_REMOVE_EXT_ELEMENT(element_ext, env, 
            ext_el);
    AXIS2_WODEN_ELEMENT_EXTENSIBLE_TO_ELEMENT_EXTENSIBLE_FREE(element_ext, env);
    return status;
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_documentation_get_ext_elements(
        void *documentation,
        axis2_env_t **env) 
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;
    void *element_ext = NULL;
    void *ret = NULL;

    documentation_impl = INTF_TO_IMPL(documentation);
    
    element_ext = axis2_woden_wsdl_element_to_element_extensible(
            documentation_impl->wsdl_element, env);
    ret = AXIS2_WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS(element_ext, env);
    AXIS2_WODEN_ELEMENT_EXTENSIBLE_TO_ELEMENT_EXTENSIBLE_FREE(element_ext, env);
    return ret;
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_documentation_get_ext_elements_of_type(
        void *documentation,
        axis2_env_t **env,
        axis2_qname_t *ext_type) 
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;
    void *element_ext = NULL;
    void *ret = NULL;
    
    documentation_impl = INTF_TO_IMPL(documentation);

    element_ext = axis2_woden_wsdl_element_to_element_extensible(
            documentation_impl->wsdl_element, env);
    ret = AXIS2_WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS_OF_TYPE(element_ext, env, 
            ext_type);
    AXIS2_WODEN_ELEMENT_EXTENSIBLE_TO_ELEMENT_EXTENSIBLE_FREE(element_ext, env);
    return ret;
}

axis2_bool_t AXIS2_CALL 
axis2_woden_documentation_has_ext_elements_for_namespace(
        void *documentation,
        axis2_env_t **env,
        axis2_url_t *namespc)
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;
    void *element_ext = NULL;
    axis2_bool_t ret = AXIS2_FALSE;

    documentation_impl = INTF_TO_IMPL(documentation);

    element_ext = axis2_woden_wsdl_element_to_element_extensible(
            documentation_impl->wsdl_element, env);
    ret = AXIS2_WODEN_ELEMENT_EXTENSIBLE_HAS_EXT_ELEMENTS_FOR_NAMESPACE(
            element_ext, env, namespc);
    AXIS2_WODEN_ELEMENT_EXTENSIBLE_TO_ELEMENT_EXTENSIBLE_FREE(element_ext, env);
    return ret;
}


axis2_status_t AXIS2_CALL
axis2_woden_documentation_set_content(
        void *documentation,
        axis2_env_t **env,
        void *doc_el)
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, documentation, AXIS2_FAILURE);
    documentation_impl = INTF_TO_IMPL(documentation);
   
    if(documentation_impl->f_content)
    {
        /* Free f_content */
    }
    documentation_impl->f_content = doc_el;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_woden_documentation_get_content(
        void *documentation,
        axis2_env_t **env)
{
    axis2_woden_documentation_impl_t *documentation_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    documentation_impl = INTF_TO_IMPL(documentation);

    return documentation_impl->f_content;
}

