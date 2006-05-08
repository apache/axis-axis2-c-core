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

#include <woden/axis2_woden_wsdl_obj.h>
#include <woden/wsdl20/axis2_woden_feature.h>
#include <woden/wsdl20/extensions/axis2_woden_ext_element.h>
#include <woden/wsdl20/extensions/axis2_woden_component_exts.h>
#include <woden/xml/axis2_woden_xml_attr.h>
#include <woden/wsdl20/xml/axis2_woden_documentation_element.h>

typedef struct axis2_woden_feature_impl axis2_woden_feature_impl_t;

/** 
 * @brief Feature Struct Impl
 *	Axis2 Feature  
 */ 
struct axis2_woden_feature_impl
{
    axis2_woden_feature_t feature;
    axis2_woden_obj_types_t obj_type;
    axis2_woden_documentable_t *documentable;
    axis2_hash_t *methods;
    axis2_url_t *f_ref;
    axis2_bool_t f_required;
    void *f_parent;
};

#define INTF_TO_IMPL(feature) ((axis2_woden_feature_impl_t *) feature)

axis2_status_t AXIS2_CALL 
axis2_woden_feature_free(
        void *feature,
        axis2_env_t **env);

axis2_woden_obj_types_t AXIS2_CALL 
axis2_woden_feature_type(
        void *feature,
        axis2_env_t **env);

axis2_woden_documentable_t *AXIS2_CALL
axis2_woden_feature_get_base_impl(
        void *feature,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_feature_set_ref(
        void *feature,
        axis2_env_t **env,
        axis2_url_t *ref);

axis2_url_t *AXIS2_CALL
axis2_woden_feature_get_ref(
        void *feature,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_feature_set_required(
        void *feature,
        axis2_env_t **env,
        axis2_bool_t required);

axis2_bool_t AXIS2_CALL
axis2_woden_feature_is_required(
        void *feature,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_feature_set_component_exts(
        void *feature,
        axis2_env_t **env,
        axis2_url_t *namespc,
        axis2_woden_component_exts_t *exts);

void *AXIS2_CALL
axis2_woden_feature_get_component_exts_for_namespace(
        void *feature,
        axis2_env_t **env,
        axis2_url_t *namespc);

axis2_status_t AXIS2_CALL
axis2_woden_feature_set_parent_element(
        void *feature,
        axis2_env_t **env,
        axis2_woden_wsdl_element_t *parent);

void *AXIS2_CALL
axis2_woden_feature_get_parent_element(
        void *feature,
        axis2_env_t **env);

void *AXIS2_CALL
axis2_woden_feature_get_parent(
        void *feature,
        axis2_env_t **env);

void *AXIS2_CALL
axis2_woden_feature_to_element(
        void *feature,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_feature_add_documentation_element(
        void *feature,
        axis2_env_t **env,
        axis2_woden_documentation_element_t *documentation);

axis2_array_list_t *AXIS2_CALL
axis2_woden_feature_get_documentation_elements(
        void *feature,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_woden_feature_set_ext_attr(
        void *feature,
        axis2_env_t **env,
        axis2_qname_t *attr_type,
        axis2_woden_xml_attr_t *attr); 

void *AXIS2_CALL 
axis2_woden_feature_get_ext_attr(
        void *feature,
        axis2_env_t **env,
        axis2_qname_t *attr_type); 

axis2_array_list_t *AXIS2_CALL 
axis2_woden_feature_get_ext_attrs(
        void *feature,
        axis2_env_t **env); 

axis2_array_list_t *AXIS2_CALL 
axis2_woden_feature_get_ext_attrs_for_namespace(
        void *feature,
        axis2_env_t **env,
        axis2_url_t *namespc);

axis2_bool_t AXIS2_CALL 
axis2_woden_feature_has_ext_attrs_for_namespace(
        void *feature,
        axis2_env_t **env,
        axis2_url_t *namespc);

axis2_status_t AXIS2_CALL 
axis2_woden_feature_add_ext_element(
        void *feature,
        axis2_env_t **env,
        axis2_woden_ext_element_t *ext_el); 

axis2_status_t AXIS2_CALL 
axis2_woden_feature_remove_ext_element(
        void *feature,
        axis2_env_t **env,
        axis2_woden_ext_element_t *ext_el); 

axis2_array_list_t *AXIS2_CALL 
axis2_woden_feature_get_ext_elements(
        void *feature,
        axis2_env_t **env); 

axis2_array_list_t *AXIS2_CALL 
axis2_woden_feature_get_ext_elements_of_type(
        void *feature,
        axis2_env_t **env,
        axis2_qname_t *elem_type);

axis2_bool_t AXIS2_CALL 
axis2_woden_feature_has_ext_elements_for_namespace(
void *feature,
        axis2_env_t **env,
        axis2_url_t *namespc);

axis2_status_t AXIS2_CALL 
axis2_woden_feature_add_ext_element(
        void *feature,
        axis2_env_t **env,
        axis2_woden_ext_element_t *ext_el); 

axis2_status_t AXIS2_CALL 
axis2_woden_feature_remove_ext_element(
        void *feature,
        axis2_env_t **env,
        axis2_woden_ext_element_t *ext_el);

axis2_array_list_t *AXIS2_CALL 
axis2_woden_feature_get_ext_elements(
        void *feature,
        axis2_env_t **env); 

axis2_array_list_t *AXIS2_CALL 
axis2_woden_feature_get_ext_elements_of_type(
        void *feature,
        axis2_env_t **env,
        axis2_qname_t *ext_type);

axis2_bool_t AXIS2_CALL 
axis2_woden_feature_has_ext_elements_for_namespace(
        void *feature,
        axis2_env_t **env,
        axis2_url_t *namespc);

static axis2_woden_feature_t *
create(axis2_env_t **env);

/************************Woden C Internal Methods******************************/
AXIS2_DECLARE(axis2_woden_feature_t *)
axis2_woden_feature_to_feature_element(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!feature)
    {
        feature_impl = (axis2_woden_feature_impl_t *) create(env);
    }
    else
        feature_impl = (axis2_woden_feature_impl_t *) feature;

    feature_impl->feature.base.feature_element.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_feature_element_ops_t));
    axis2_woden_feature_element_resolve_methods(&(feature_impl->feature.base.
            feature_element), env, feature_impl->methods);
    return feature;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_to_feature_element_free(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->feature.base.feature_element.ops)
    {
        AXIS2_FREE((*env)->allocator, feature_impl->feature.base.
                feature_element.ops);
        feature_impl->feature.base.feature_element.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_feature_t *)
axis2_woden_feature_to_nested_element(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!feature)
    {
        feature_impl = (axis2_woden_feature_impl_t *) create(env);
    }
    else
        feature_impl = (axis2_woden_feature_impl_t *) feature;

    feature_impl->feature.base.feature_element.base.nested_element.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_nested_element_ops_t));
    axis2_woden_nested_element_resolve_methods(&(feature_impl->feature.base.
            feature_element.base.nested_element), env, feature_impl->methods);
    return feature;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_to_nested_element_free(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->feature.base.feature_element.base.nested_element.ops)
    {
        AXIS2_FREE((*env)->allocator, feature_impl->feature.base.
                feature_element.base.nested_element.ops);
        feature_impl->feature.base.feature_element.base.nested_element.ops = 
            NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_feature_t *)
axis2_woden_feature_to_documentable_element(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!feature)
    {
        feature_impl = (axis2_woden_feature_impl_t *) create(env);
    }
    else
        feature_impl = (axis2_woden_feature_impl_t *) feature;

    feature_impl->feature.base.feature_element.base.documentable_element.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_documentable_element_ops_t));
    axis2_woden_documentable_element_resolve_methods(&(feature_impl->feature.base.
            feature_element.base.documentable_element), env, 
            feature_impl->methods);
    return feature;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_to_documentable_element_free(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->feature.base.feature_element.base.documentable_element.ops)
    {
        AXIS2_FREE((*env)->allocator, feature_impl->feature.base.
                feature_element.base.documentable_element.ops);
        feature_impl->feature.base.feature_element.base.documentable_element.ops = 
            NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_feature_t *)
axis2_woden_feature_to_documentable(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!feature)
    {
        feature_impl = (axis2_woden_feature_impl_t *) create(env);
    }
    else
        feature_impl = (axis2_woden_feature_impl_t *) feature;

    feature_impl->feature.base.documentable.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_documentable_ops_t));
    axis2_woden_documentable_resolve_methods(&(feature_impl->feature.base.
            documentable), env, feature_impl->methods);
    return feature;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_to_documentable_free(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->feature.base.documentable.ops)
    {
        AXIS2_FREE((*env)->allocator, feature_impl->feature.base.
                documentable.ops);
        feature_impl->feature.base.documentable.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_feature_t *)
axis2_woden_feature_to_wsdl_obj(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!feature)
    {
        feature_impl = (axis2_woden_feature_impl_t *) create(env);
    }
    else
        feature_impl = (axis2_woden_feature_impl_t *) feature;

    feature_impl->feature.base.documentable.base.wsdl_obj.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_wsdl_obj_ops_t));
    axis2_woden_wsdl_obj_resolve_methods(&(feature_impl->feature.base.
            documentable.base.wsdl_obj), env, feature_impl->methods);
    return feature;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_to_wsdl_obj_free(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->feature.base.documentable.base.wsdl_obj.ops)
    {
        AXIS2_FREE((*env)->allocator, feature_impl->feature.base.
                documentable.base.wsdl_obj.ops);
        feature_impl->feature.base.documentable.base.wsdl_obj.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_feature_t *)
axis2_woden_feature_to_nested_component(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!feature)
    {
        feature_impl = (axis2_woden_feature_impl_t *) create(env);
    }
    else
        feature_impl = (axis2_woden_feature_impl_t *) feature;
  
    feature_impl->feature.base.nested_component.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_nested_component_ops_t));
    axis2_woden_nested_component_resolve_methods(&(feature_impl->feature.base.
            nested_component), env, feature_impl->methods);
    return feature;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_to_nested_component_free(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->feature.base.nested_component.ops)
    {
        AXIS2_FREE((*env)->allocator, feature_impl->feature.base.
                nested_component.ops);
        feature_impl->feature.base.nested_component.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_feature_t *)
axis2_woden_feature_to_wsdl_component(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!feature)
    {
        feature_impl = (axis2_woden_feature_impl_t *) create(env);
    }
    else
        feature_impl = (axis2_woden_feature_impl_t *) feature;
    feature_impl->feature.base.nested_component.wsdl_component.ops = 
        AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_woden_wsdl_component_ops_t));
    axis2_woden_wsdl_component_resolve_methods(&(feature_impl->feature.base.
            nested_component.wsdl_component), env, feature_impl->methods);
    return feature;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_to_wsdl_component_free(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->feature.base.nested_component.wsdl_component.ops)
    {
        AXIS2_FREE((*env)->allocator, feature_impl->feature.base.
                nested_component.wsdl_component.ops);
        feature_impl->feature.base.nested_component.wsdl_component.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_feature_t *)
axis2_woden_feature_to_attr_extensible(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!feature)
    {
        feature_impl = (axis2_woden_feature_impl_t *) create(env);
    }
    else
        feature_impl = (axis2_woden_feature_impl_t *) feature;

    feature_impl->feature.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.attr_extensible.ops =
            AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_woden_attr_extensible_ops_t));
    axis2_woden_attr_extensible_resolve_methods(&(feature_impl->feature.base.
            documentable.base.wsdl_obj.base.wsdl_element.base.attr_extensible), 
            env, feature_impl->methods);
    return feature;

}

axis2_status_t AXIS2_CALL
axis2_woden_feature_to_attr_extensible_free(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->feature.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE((*env)->allocator, feature_impl->feature.base.documentable.
                base.wsdl_obj.base.wsdl_element.base.attr_extensible.ops);
        feature_impl->feature.base.documentable.base.wsdl_obj.base.wsdl_element.
                base.attr_extensible.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_feature_t *)
axis2_woden_feature_to_element_extensible(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!feature)
    {
        feature_impl = (axis2_woden_feature_impl_t *) create(env);
    }
    else
        feature_impl = (axis2_woden_feature_impl_t *) feature;
        feature_impl->feature.base.documentable.base.wsdl_obj.base.wsdl_element.
                base.element_extensible.ops = AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_element_extensible_ops_t));
    axis2_woden_element_extensible_resolve_methods(&(feature_impl->feature.base.
            documentable.base.wsdl_obj.base.wsdl_element.base.element_extensible), 
            env, feature_impl->methods);
    return feature;

}

axis2_status_t AXIS2_CALL
axis2_woden_feature_to_element_extensible_free(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->feature.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE((*env)->allocator, feature_impl->feature.base.documentable.
                base.wsdl_obj.base.wsdl_element.base.element_extensible.ops);
        feature_impl->feature.base.documentable.base.wsdl_obj.base.wsdl_element.
                base.element_extensible.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

/************************End of Woden C Internal Methods***********************/
static axis2_woden_feature_t *
create(axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_feature_impl_t));

    feature_impl->obj_type= AXIS2_WODEN_FEATURE;
    feature_impl->methods = NULL;
    feature_impl->f_parent = NULL;
    feature_impl->f_ref = NULL;
    feature_impl->f_required = AXIS2_FALSE;
    
    feature_impl->feature.base.feature_element.ops = NULL;
    feature_impl->feature.base.feature_element.base.nested_element.ops = NULL;
    feature_impl->feature.base.feature_element.base.documentable_element.ops = 
            NULL;
    feature_impl->feature.base.documentable.ops = NULL;
    feature_impl->feature.base.nested_component.ops = NULL; 
    feature_impl->feature.base.nested_component.wsdl_component.ops = NULL;
    feature_impl->feature.base.documentable.base.wsdl_obj.ops = NULL;
    feature_impl->feature.base.documentable.base.wsdl_obj.base.wsdl_element.
            base.attr_extensible.ops = NULL;
    feature_impl->feature.base.documentable.base.wsdl_obj.base.wsdl_element.
            base.element_extensible.ops = NULL;
    
    feature_impl->feature.ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_woden_feature_ops_t));

    feature_impl->feature.ops->free = axis2_woden_feature_free;
    feature_impl->feature.ops->type = axis2_woden_feature_type;
    feature_impl->feature.ops->get_base_impl = axis2_woden_feature_get_base_impl;
    feature_impl->feature.ops->get_ref = axis2_woden_feature_get_ref;
    feature_impl->feature.ops->is_required = axis2_woden_feature_is_required;
    feature_impl->feature.ops->get_parent = 
        axis2_woden_feature_get_parent;
    feature_impl->feature.ops->to_element = 
        axis2_woden_feature_to_element;
 
    feature_impl->methods = axis2_hash_make(env);
    if(!feature_impl->methods) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(feature_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_free);
    axis2_hash_set(feature_impl->methods, "to_feature_element_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_to_feature_element_free);
    axis2_hash_set(feature_impl->methods, "to_nested_element_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_to_nested_element_free);
    axis2_hash_set(feature_impl->methods, "to_documentable_element_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_to_documentable_element_free);
    axis2_hash_set(feature_impl->methods, "to_documentable_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_to_documentable_free);
    axis2_hash_set(feature_impl->methods, "to_wsdl_obj_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_to_wsdl_obj_free);
    axis2_hash_set(feature_impl->methods, "to_nested_component_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_to_nested_component_free);
    axis2_hash_set(feature_impl->methods, "to_wsdl_component_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_to_wsdl_component_free);
    axis2_hash_set(feature_impl->methods, "to_attr_extensible_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_to_attr_extensible_free);
    axis2_hash_set(feature_impl->methods, "to_element_extensible_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_to_element_extensible_free);
    axis2_hash_set(feature_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_woden_feature_type);

    axis2_hash_set(feature_impl->methods, "get_parent", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_get_parent);
    axis2_hash_set(feature_impl->methods, "set_parent_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_set_parent_element);
    axis2_hash_set(feature_impl->methods, "get_parent_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_get_parent_element);
    axis2_hash_set(feature_impl->methods, "set_ref", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_set_ref);
    axis2_hash_set(feature_impl->methods, "get_ref", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_get_ref);
    axis2_hash_set(feature_impl->methods, "set_required", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_set_required);
    axis2_hash_set(feature_impl->methods, "is_required", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_is_required);
    axis2_hash_set(feature_impl->methods, "get_component_exts_for_namespace", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_get_component_exts_for_namespace);
    axis2_hash_set(feature_impl->methods, "set_component_exts", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_set_component_exts);
    axis2_hash_set(feature_impl->methods, "to_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_to_element);
    axis2_hash_set(feature_impl->methods, "add_documentation_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_add_documentation_element);
    axis2_hash_set(feature_impl->methods, "get_documentation_elements", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_get_documentation_elements);
    axis2_hash_set(feature_impl->methods, "set_ext_attr", 
            AXIS2_HASH_KEY_STRING, axis2_woden_feature_set_ext_attr);
    axis2_hash_set(feature_impl->methods, "get_ext_attr", 
            AXIS2_HASH_KEY_STRING, axis2_woden_feature_get_ext_attr);
    axis2_hash_set(feature_impl->methods, "get_ext_attrs", 
            AXIS2_HASH_KEY_STRING, axis2_woden_feature_get_ext_attrs);
    axis2_hash_set(feature_impl->methods, "get_ext_attrs_for_namespace", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_get_ext_attrs_for_namespace);
    axis2_hash_set(feature_impl->methods, "has_ext_attrs_for_namespace", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_has_ext_attrs_for_namespace);
    axis2_hash_set(feature_impl->methods, "add_ext_element", 
            AXIS2_HASH_KEY_STRING, axis2_woden_feature_add_ext_element);
    axis2_hash_set(feature_impl->methods, "remove_ext_element", 
            AXIS2_HASH_KEY_STRING, axis2_woden_feature_remove_ext_element);
    axis2_hash_set(feature_impl->methods, "get_ext_elements", 
            AXIS2_HASH_KEY_STRING, axis2_woden_feature_get_ext_elements);
    axis2_hash_set(feature_impl->methods, "get_ext_elements_of_type", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_get_ext_elements_of_type);
    axis2_hash_set(feature_impl->methods, "has_ext_elements_for_namespace", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_has_ext_elements_for_namespace);
    

    return &(feature_impl->feature);
}

AXIS2_DECLARE(axis2_woden_feature_t *)
axis2_woden_feature_create(axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = (axis2_woden_feature_impl_t *) create(env);

    feature_impl->documentable = axis2_woden_documentable_create(env);

    return &(feature_impl->feature);
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_free(void *feature,
                        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->f_ref)
    {
        AXIS2_URL_FREE(feature_impl->f_ref, env);
        feature_impl->f_ref = NULL;
    }
   
    /* TODO free f_parent */
    
    if(feature_impl->methods)
    {
        axis2_hash_free(feature_impl->methods, env);
        feature_impl->methods = NULL;
    }

    if(feature_impl->documentable)
    {
        AXIS2_WODEN_DOCUMENTABLE_FREE(feature_impl->documentable, env);
        feature_impl->documentable = NULL;
    }

    if(feature_impl->feature.base.feature_element.ops)
    {
        AXIS2_FREE((*env)->allocator, feature_impl->feature.base.
                feature_element.ops);
        feature_impl->feature.base.feature_element.ops = NULL;
    }

    if(feature_impl->feature.base.feature_element.base.nested_element.ops)
    {
        AXIS2_FREE((*env)->allocator, feature_impl->feature.base.
                feature_element.base.nested_element.ops);
        feature_impl->feature.base.feature_element.base.nested_element.ops = 
            NULL;
    }
    
    if(feature_impl->feature.base.feature_element.base.documentable_element.ops)
    {
        AXIS2_FREE((*env)->allocator, feature_impl->feature.base.
                feature_element.base.documentable_element.ops);
        feature_impl->feature.base.feature_element.base.documentable_element.ops = 
            NULL;
    }
    
    if(feature_impl->feature.base.documentable.ops)
    {
        AXIS2_FREE((*env)->allocator, feature_impl->feature.base.
                documentable.ops);
        feature_impl->feature.base.documentable.ops = 
            NULL;
    }
    
    if(feature_impl->feature.base.documentable.base.wsdl_obj.ops)
    {
        AXIS2_FREE((*env)->allocator, feature_impl->feature.base.
                documentable.base.wsdl_obj.ops);
        feature_impl->feature.base.documentable.base.wsdl_obj.ops = 
            NULL;
    }

    if(feature_impl->feature.base.nested_component.ops)
    {
        AXIS2_FREE((*env)->allocator, feature_impl->feature.base.
                nested_component.ops);
        feature_impl->feature.base.nested_component.ops = NULL;
    }

    if(feature_impl->feature.base.nested_component.wsdl_component.ops)
    {
        AXIS2_FREE((*env)->allocator, feature_impl->feature.base.
                nested_component.wsdl_component.ops);
        feature_impl->feature.base.nested_component.wsdl_component.ops = NULL;
    }


    if(feature_impl->feature.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE((*env)->allocator, feature_impl->feature.base.documentable.
                base.wsdl_obj.base.wsdl_element.base.attr_extensible.ops);
        feature_impl->feature.base.documentable.base.wsdl_obj.base.wsdl_element.
                base.attr_extensible.ops = NULL;
    }

    if(feature_impl->feature.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE((*env)->allocator, feature_impl->feature.base.documentable.
                base.wsdl_obj.base.wsdl_element.base.element_extensible.ops);
        feature_impl->feature.base.documentable.base.wsdl_obj.base.wsdl_element.
                base.element_extensible.ops = NULL;
    }
    if((&(feature_impl->feature))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(feature_impl->feature))->ops);
        (&(feature_impl->feature))->ops = NULL;
    }
    
    if(feature_impl)
    {
        AXIS2_FREE((*env)->allocator, feature_impl);
        feature_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_woden_obj_types_t AXIS2_CALL
axis2_woden_feature_type(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    return feature_impl->obj_type;
}

axis2_woden_documentable_t *AXIS2_CALL
axis2_woden_feature_get_base_impl(void *feature,
                                axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    feature_impl = INTF_TO_IMPL(feature);

    return feature_impl->documentable;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_resolve_methods(axis2_woden_feature_t *feature,
                                axis2_env_t **env,
                                axis2_hash_t *methods)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    feature->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    feature->ops->to_feature_free = axis2_hash_get(methods, "to_feature_free", 
            AXIS2_HASH_KEY_STRING);
    feature->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    feature->ops->get_ref = axis2_hash_get(methods, 
            "get_ref", AXIS2_HASH_KEY_STRING);
    feature->ops->is_required = axis2_hash_get(methods, 
            "is_required", AXIS2_HASH_KEY_STRING);
    feature->ops->get_parent = axis2_hash_get(methods, 
            "get_parent", AXIS2_HASH_KEY_STRING);
    feature->ops->to_element = axis2_hash_get(methods, 
            "to_element", AXIS2_HASH_KEY_STRING);
   
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_add_documentation_element(
        void *feature,
        axis2_env_t **env,
        axis2_woden_documentation_element_t *documentation)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, documentation, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    return AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(
            feature_impl->documentable, env, documentation);
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_feature_get_documentation_elements(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    return AXIS2_WODEN_DOCUMENTABLE_GET_DOCUMENTATION_ELEMENTS(
            feature_impl->documentable, env);
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_set_ref(
        void *feature,
        axis2_env_t **env,
        axis2_url_t *ref)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->f_ref)
    {
        AXIS2_URL_FREE(feature_impl->f_ref, env);
    }
    feature_impl->f_ref = ref;
    return AXIS2_SUCCESS;
}

axis2_url_t *AXIS2_CALL
axis2_woden_feature_get_ref(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    feature_impl = INTF_TO_IMPL(feature);
    
    return feature_impl->f_ref;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_set_required(
        void *feature,
        axis2_env_t **env,
        axis2_bool_t required)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    feature_impl->f_required = required;
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_woden_feature_is_required(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    return feature_impl->f_required;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_set_component_exts(
        void *feature,
        axis2_env_t **env,
        axis2_url_t *namespc,
        axis2_woden_component_exts_t *exts)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    void *wsdl_obj = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    wsdl_obj = AXIS2_WODEN_DOCUMENTABLE_GET_BASE_IMPL(feature_impl->documentable, 
            env);
    return AXIS2_WODEN_WSDL_OBJ_SET_COMPONENT_EXTS(wsdl_obj, env, namespc, exts);
}

void *AXIS2_CALL
axis2_woden_feature_get_component_exts_for_namespace(
        void *feature,
        axis2_env_t **env,
        axis2_url_t *namespc)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    void *wsdl_obj = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);
    
    wsdl_obj = AXIS2_WODEN_DOCUMENTABLE_GET_BASE_IMPL(feature_impl->documentable, 
            env);
    return AXIS2_WODEN_WSDL_OBJ_GET_COMPONENT_EXTS_FOR_NAMESPACE(wsdl_obj, env, 
            namespc);

}
axis2_status_t AXIS2_CALL
axis2_woden_feature_set_parent_element(
        void *feature,
        axis2_env_t **env,
        axis2_woden_wsdl_element_t *parent)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->f_parent)
    {
        /* TODO free this */
    }
    feature_impl->f_parent = parent;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_woden_feature_get_parent_element(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    return feature_impl->f_parent;
}

void *AXIS2_CALL
axis2_woden_feature_get_parent(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    /* TODO complete this */
    return NULL;
}

void *AXIS2_CALL
axis2_woden_feature_to_element(
        void *feature,
        axis2_env_t **env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);
    
    return &(feature_impl->feature);
}

axis2_status_t AXIS2_CALL 
axis2_woden_feature_set_ext_attr(
        void *feature,
        axis2_env_t **env,
        axis2_qname_t *attr_type,
        axis2_woden_xml_attr_t *attr)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    void *wsdl_obj = NULL;
    void *wsdl_element = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    feature_impl = INTF_TO_IMPL(feature);
    
    wsdl_obj = AXIS2_WODEN_DOCUMENTABLE_GET_BASE_IMPL(feature_impl->documentable, 
            env);
    wsdl_element = AXIS2_WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
    wsdl_element = axis2_woden_wsdl_element_to_attr_extensible(wsdl_element, env);
    status = AXIS2_WODEN_ATTR_EXTENSIBLE_SET_EXT_ATTR(wsdl_element, env, attr_type, 
            attr);
    AXIS2_ATTR_EXTENSIBLE_TO_ATTR_EXTENSIBLE_FREE(wsdl_element, env);
    return status;
}

void *AXIS2_CALL 
axis2_woden_feature_get_ext_attr(
        void *feature,
        axis2_env_t **env,
        axis2_qname_t *attr_type) 
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    void *wsdl_obj = NULL;
    void *wsdl_element = NULL;
    void *ret = NULL;

    feature_impl = INTF_TO_IMPL(feature);
    
    wsdl_obj = AXIS2_WODEN_DOCUMENTABLE_GET_BASE_IMPL(feature_impl->documentable, 
            env);
    wsdl_element = AXIS2_WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
    wsdl_element = axis2_woden_wsdl_element_to_attr_extensible(wsdl_element, env);
    ret = AXIS2_WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTR(wsdl_element, env, attr_type);
    AXIS2_ATTR_EXTENSIBLE_TO_ATTR_EXTENSIBLE_FREE(wsdl_element, env);
    return ret;
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_feature_get_ext_attrs(
        void *feature,
        axis2_env_t **env) 
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    void *wsdl_obj = NULL;
    void *wsdl_element = NULL;
    void *ret = NULL;
    
    feature_impl = INTF_TO_IMPL(feature);

    wsdl_obj = AXIS2_WODEN_DOCUMENTABLE_GET_BASE_IMPL(feature_impl->documentable, 
            env);
    wsdl_element = AXIS2_WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
    wsdl_element = axis2_woden_wsdl_element_to_attr_extensible(wsdl_element, env);
    ret = AXIS2_WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTRS(wsdl_element, env);
    AXIS2_ATTR_EXTENSIBLE_TO_ATTR_EXTENSIBLE_FREE(wsdl_element, env);
    return ret;
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_feature_get_ext_attrs_for_namespace(
        void *feature,
        axis2_env_t **env,
        axis2_url_t *namespc) 
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    void *wsdl_obj = NULL;
    void *wsdl_element = NULL;
    void *ret = NULL;
    
    feature_impl = INTF_TO_IMPL(feature);

    wsdl_obj = AXIS2_WODEN_DOCUMENTABLE_GET_BASE_IMPL(feature_impl->documentable, 
            env);
    wsdl_element = AXIS2_WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
    wsdl_element = axis2_woden_wsdl_element_to_attr_extensible(wsdl_element, env);
    ret = AXIS2_WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTRS_FOR_NAMESPACE(
            wsdl_element, env, namespc);
    AXIS2_ATTR_EXTENSIBLE_TO_ATTR_EXTENSIBLE_FREE(wsdl_element, env);
    return ret;
}

axis2_bool_t AXIS2_CALL 
axis2_woden_feature_has_ext_attrs_for_namespace(
        void *feature,
        axis2_env_t **env,
        axis2_url_t *namespc)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    void *wsdl_obj = NULL;
    void *wsdl_element = NULL;
    axis2_bool_t ret = AXIS2_FALSE;

    feature_impl = INTF_TO_IMPL(feature);

    wsdl_obj = AXIS2_WODEN_DOCUMENTABLE_GET_BASE_IMPL(feature_impl->documentable, 
            env);
    wsdl_element = AXIS2_WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
    wsdl_element = axis2_woden_wsdl_element_to_attr_extensible(wsdl_element, env);
    ret = AXIS2_WODEN_ATTR_EXTENSIBLE_HAS_EXT_ATTRS_FOR_NAMESPACE(
            wsdl_element, env, namespc);
    AXIS2_ATTR_EXTENSIBLE_TO_ATTR_EXTENSIBLE_FREE(wsdl_element, env);
    return ret;
}

axis2_status_t AXIS2_CALL 
axis2_woden_feature_add_ext_element(
        void *feature,
        axis2_env_t **env,
        axis2_woden_ext_element_t *ext_el) 
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    void *wsdl_obj = NULL;
    void *wsdl_element = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    feature_impl = INTF_TO_IMPL(feature);
    
    wsdl_obj = AXIS2_WODEN_DOCUMENTABLE_GET_BASE_IMPL(feature_impl->documentable, 
            env);
    wsdl_element = AXIS2_WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
    wsdl_element = axis2_woden_wsdl_element_to_element_extensible(wsdl_element, 
            env);
    status = AXIS2_WSDL_ELEMENT_ADD_EXT_ELEMENT(wsdl_element, env, ext_el);
    AXIS2_ELEMENT_EXTENSIBLE_TO_ELEMENT_EXTENSIBLE_FREE(wsdl_element, env);
    return status;
}

axis2_status_t AXIS2_CALL 
axis2_woden_feature_remove_ext_element(
        void *feature,
        axis2_env_t **env,
        axis2_woden_ext_element_t *ext_el) 
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    void *wsdl_obj = NULL;
    void *wsdl_element = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    feature_impl = INTF_TO_IMPL(feature);

    wsdl_obj = AXIS2_WODEN_DOCUMENTABLE_GET_BASE_IMPL(feature_impl->documentable, 
            env);
    wsdl_element = AXIS2_WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
    wsdl_element = axis2_woden_wsdl_element_to_element_extensible(wsdl_element, 
            env);
    status = AXIS2_WSDL_ELEMENT_REMOVE_EXT_ELEMENT(wsdl_element, env, ext_el);
    AXIS2_ELEMENT_EXTENSIBLE_TO_ELEMENT_EXTENSIBLE_FREE(wsdl_element, env);
    return status;
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_feature_get_ext_elements(
        void *feature,
        axis2_env_t **env) 
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    void *wsdl_obj = NULL;
    void *wsdl_element = NULL;
    void *ret = NULL;

    feature_impl = INTF_TO_IMPL(feature);
    
    wsdl_obj = AXIS2_WODEN_DOCUMENTABLE_GET_BASE_IMPL(feature_impl->documentable, 
            env);
    wsdl_element = AXIS2_WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
    wsdl_element = axis2_woden_wsdl_element_to_element_extensible(wsdl_element, 
            env);
    ret = (axis2_array_list_t *) AXIS2_WSDL_ELEMENT_GET_EXT_ELEMENTS(
            wsdl_element, env);
    AXIS2_ELEMENT_EXTENSIBLE_TO_ELEMENT_EXTENSIBLE_FREE(wsdl_element, env);
    return ret;
}

axis2_array_list_t *AXIS2_CALL 
axis2_woden_feature_get_ext_elements_of_type(
        void *feature,
        axis2_env_t **env,
        axis2_qname_t *ext_type) 
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    void *wsdl_obj = NULL;
    void *wsdl_element = NULL;
    void *ret = NULL;
    
    feature_impl = INTF_TO_IMPL(feature);

    wsdl_obj = AXIS2_WODEN_DOCUMENTABLE_GET_BASE_IMPL(feature_impl->documentable, 
            env);
    wsdl_element = AXIS2_WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
    wsdl_element = axis2_woden_wsdl_element_to_element_extensible(wsdl_element, 
            env);
    ret = (axis2_array_list_t *) AXIS2_WSDL_ELEMENT_GET_EXT_ELEMENTS_OF_TYPE(
            wsdl_element, env, ext_type);
    AXIS2_ELEMENT_EXTENSIBLE_TO_ELEMENT_EXTENSIBLE_FREE(wsdl_element, env);
    return ret;
}

axis2_bool_t AXIS2_CALL 
axis2_woden_feature_has_ext_elements_for_namespace(
        void *feature,
        axis2_env_t **env,
        axis2_url_t *namespc)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    void *wsdl_obj = NULL;
    void *wsdl_element = NULL;
    axis2_bool_t ret = AXIS2_FALSE;

    feature_impl = INTF_TO_IMPL(feature);

    wsdl_obj = AXIS2_WODEN_DOCUMENTABLE_GET_BASE_IMPL(feature_impl->documentable, 
            env);
    wsdl_element = AXIS2_WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
    wsdl_element = axis2_woden_wsdl_element_to_element_extensible(wsdl_element, 
            env);
    ret = AXIS2_WSDL_ELEMENT_HAS_EXT_ELEMENTS_FOR_NAMESPACE(wsdl_element, env, 
            namespc);
    AXIS2_ELEMENT_EXTENSIBLE_TO_ELEMENT_EXTENSIBLE_FREE(wsdl_element, env);
    return ret;
}


