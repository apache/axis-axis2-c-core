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
    axis2_hash_t *super;
    axis2_hash_t *methods;
    axis2_url_t *f_ref;
    axis2_bool_t f_required;
    void *f_parent;
};

#define INTF_TO_IMPL(feature) ((axis2_woden_feature_impl_t *) feature)

axis2_status_t AXIS2_CALL 
axis2_woden_feature_free(
        void *feature,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
axis2_woden_feature_super_objs(
        void *feature,
        const axis2_env_t *env);

axis2_woden_obj_types_t AXIS2_CALL 
axis2_woden_feature_type(
        void *feature,
        const axis2_env_t *env);

axis2_woden_documentable_t *AXIS2_CALL
axis2_woden_feature_get_base_impl(
        void *feature,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_woden_feature_set_ref(
        void *feature,
        const axis2_env_t *env,
        axis2_url_t *ref);

axis2_url_t *AXIS2_CALL
axis2_woden_feature_get_ref(
        void *feature,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_woden_feature_set_required(
        void *feature,
        const axis2_env_t *env,
        axis2_bool_t required);

axis2_bool_t AXIS2_CALL
axis2_woden_feature_is_required(
        void *feature,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_woden_feature_set_parent_element(
        void *feature,
        const axis2_env_t *env,
        axis2_woden_wsdl_element_t *parent);

void *AXIS2_CALL
axis2_woden_feature_get_parent_element(
        void *feature,
        const axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_feature_get_parent(
        void *feature,
        const axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_feature_to_element(
        void *feature,
        const axis2_env_t *env);

static axis2_woden_feature_t *
create(const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_to_feature_element(
        void *feature,
        const axis2_env_t *env)
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
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_feature_element_ops_t));
    axis2_woden_feature_element_resolve_methods(&(feature_impl->feature.base.
            feature_element), env, feature_impl->methods);
    return feature;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_to_feature_element_free(
        void *feature,
        const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->feature.base.feature_element.ops)
    {
        AXIS2_FREE(env->allocator, feature_impl->feature.base.
                feature_element.ops);
        feature_impl->feature.base.feature_element.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_to_nested_element(
        void *feature,
        const axis2_env_t *env)
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
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_nested_element_ops_t));
    axis2_woden_nested_element_resolve_methods(&(feature_impl->feature.base.
            feature_element.base.nested_element), env, feature_impl->methods);
    return feature;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_to_nested_element_free(
        void *feature,
        const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->feature.base.feature_element.base.nested_element.ops)
    {
        AXIS2_FREE(env->allocator, feature_impl->feature.base.
                feature_element.base.nested_element.ops);
        feature_impl->feature.base.feature_element.base.nested_element.ops = 
            NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_to_documentable_element(
        void *feature,
        const axis2_env_t *env)
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
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_documentable_element_ops_t));
    axis2_woden_documentable_element_resolve_methods(&(feature_impl->feature.base.
            feature_element.base.documentable_element), env, 
            feature_impl->methods);
    return feature;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_to_documentable_element_free(
        void *feature,
        const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->feature.base.feature_element.base.documentable_element.ops)
    {
        AXIS2_FREE(env->allocator, feature_impl->feature.base.
                feature_element.base.documentable_element.ops);
        feature_impl->feature.base.feature_element.base.documentable_element.ops = 
            NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_to_documentable(
        void *feature,
        const axis2_env_t *env)
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
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_documentable_ops_t));
    axis2_woden_documentable_resolve_methods(&(feature_impl->feature.base.
            documentable), env, feature_impl->documentable, 
            feature_impl->methods);
    return feature;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_to_documentable_free(
        void *feature,
        const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->feature.base.documentable.ops)
    {
        AXIS2_FREE(env->allocator, feature_impl->feature.base.
                documentable.ops);
        feature_impl->feature.base.documentable.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_to_wsdl_obj(
        void *feature,
        const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    void *wsdl_obj = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!feature)
    {
        feature_impl = (axis2_woden_feature_impl_t *) create(env);
    }
    else
        feature_impl = (axis2_woden_feature_impl_t *) feature;

    feature_impl->feature.base.documentable.base.wsdl_obj.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_wsdl_obj_ops_t));
    wsdl_obj = AXIS2_WODEN_DOCUMENTABLE_GET_BASE_IMPL(
            feature_impl->documentable, env);
    axis2_woden_wsdl_obj_resolve_methods(&(feature_impl->feature.base.
            documentable.base.wsdl_obj), env, wsdl_obj, feature_impl->methods);
    return feature;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_to_wsdl_obj_free(
        void *feature,
        const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->feature.base.documentable.base.wsdl_obj.ops)
    {
        AXIS2_FREE(env->allocator, feature_impl->feature.base.
                documentable.base.wsdl_obj.ops);
        feature_impl->feature.base.documentable.base.wsdl_obj.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_to_nested_component(
        void *feature,
        const axis2_env_t *env)
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
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_nested_component_ops_t));
    axis2_woden_nested_component_resolve_methods(&(feature_impl->feature.base.
            nested_component), env, feature_impl->methods);
    return feature;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_to_nested_component_free(
        void *feature,
        const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->feature.base.nested_component.ops)
    {
        AXIS2_FREE(env->allocator, feature_impl->feature.base.
                nested_component.ops);
        feature_impl->feature.base.nested_component.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_to_wsdl_component(
        void *feature,
        const axis2_env_t *env)
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
        AXIS2_MALLOC(env->allocator, 
        sizeof(axis2_woden_wsdl_component_ops_t));
    axis2_woden_wsdl_component_resolve_methods(&(feature_impl->feature.base.
            nested_component.wsdl_component), env, feature_impl->methods);
    return feature;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_to_wsdl_component_free(
        void *feature,
        const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->feature.base.nested_component.wsdl_component.ops)
    {
        AXIS2_FREE(env->allocator, feature_impl->feature.base.
                nested_component.wsdl_component.ops);
        feature_impl->feature.base.nested_component.wsdl_component.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_to_attr_extensible(
        void *feature,
        const axis2_env_t *env)
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
            AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_woden_attr_extensible_ops_t));
    axis2_woden_attr_extensible_resolve_methods(&(feature_impl->feature.base.
            documentable.base.wsdl_obj.base.wsdl_element.base.attr_extensible), 
            env, NULL, feature_impl->methods);
    return feature;

}

axis2_status_t AXIS2_CALL
axis2_woden_feature_to_attr_extensible_free(
        void *feature,
        const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->feature.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, feature_impl->feature.base.documentable.
                base.wsdl_obj.base.wsdl_element.base.attr_extensible.ops);
        feature_impl->feature.base.documentable.base.wsdl_obj.base.wsdl_element.
                base.attr_extensible.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_to_element_extensible(
        void *feature,
        const axis2_env_t *env)
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
                base.element_extensible.ops = AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_element_extensible_ops_t));
    axis2_woden_element_extensible_resolve_methods(&(feature_impl->feature.base.
            documentable.base.wsdl_obj.base.wsdl_element.base.element_extensible), 
            env, NULL, feature_impl->methods);
    return feature;

}

axis2_status_t AXIS2_CALL
axis2_woden_feature_to_element_extensible_free(
        void *feature,
        const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    if(feature_impl->feature.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, feature_impl->feature.base.documentable.
                base.wsdl_obj.base.wsdl_element.base.element_extensible.ops);
        feature_impl->feature.base.documentable.base.wsdl_obj.base.wsdl_element.
                base.element_extensible.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

/************************End of Woden C Internal Methods***********************/
static axis2_woden_feature_t *
create(const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_woden_feature_impl_t));

    feature_impl->obj_type= AXIS2_WODEN_FEATURE;
    feature_impl->super = NULL;
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
    
    feature_impl->feature.ops = AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_woden_feature_ops_t));

    feature_impl->feature.ops->free = axis2_woden_feature_free;
    feature_impl->feature.ops->super_objs = axis2_woden_feature_super_objs;
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
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
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
    axis2_hash_set(feature_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, axis2_woden_feature_super_objs);
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
    axis2_hash_set(feature_impl->methods, "to_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_feature_to_element);

    return &(feature_impl->feature);
}

AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_create(const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = (axis2_woden_feature_impl_t *) create(env);

    feature_impl->documentable = axis2_woden_documentable_create(env);

    feature_impl->super = axis2_hash_make(env);
    if(!feature_impl->super) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(feature_impl->super, "AXIS2_WODEN_FEATURE", AXIS2_HASH_KEY_STRING, 
            &(feature_impl->feature));
    axis2_hash_set(feature_impl->super, "AXIS2_WODEN_DOCUMENTABLE", AXIS2_HASH_KEY_STRING, 
            feature_impl->documentable);
 
    return &(feature_impl->feature);
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_free(void *feature,
                        const axis2_env_t *env)
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
    
    if(feature_impl->super)
    {
        axis2_hash_free(feature_impl->super, env);
        feature_impl->super = NULL;
    }
    
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
        AXIS2_FREE(env->allocator, feature_impl->feature.base.
                feature_element.ops);
        feature_impl->feature.base.feature_element.ops = NULL;
    }

    if(feature_impl->feature.base.feature_element.base.nested_element.ops)
    {
        AXIS2_FREE(env->allocator, feature_impl->feature.base.
                feature_element.base.nested_element.ops);
        feature_impl->feature.base.feature_element.base.nested_element.ops = 
            NULL;
    }
    
    if(feature_impl->feature.base.feature_element.base.documentable_element.ops)
    {
        AXIS2_FREE(env->allocator, feature_impl->feature.base.
                feature_element.base.documentable_element.ops);
        feature_impl->feature.base.feature_element.base.documentable_element.ops = 
            NULL;
    }
    
    if(feature_impl->feature.base.documentable.ops)
    {
        AXIS2_FREE(env->allocator, feature_impl->feature.base.
                documentable.ops);
        feature_impl->feature.base.documentable.ops = 
            NULL;
    }
    
    if(feature_impl->feature.base.documentable.base.wsdl_obj.ops)
    {
        AXIS2_FREE(env->allocator, feature_impl->feature.base.
                documentable.base.wsdl_obj.ops);
        feature_impl->feature.base.documentable.base.wsdl_obj.ops = 
            NULL;
    }

    if(feature_impl->feature.base.nested_component.ops)
    {
        AXIS2_FREE(env->allocator, feature_impl->feature.base.
                nested_component.ops);
        feature_impl->feature.base.nested_component.ops = NULL;
    }

    if(feature_impl->feature.base.nested_component.wsdl_component.ops)
    {
        AXIS2_FREE(env->allocator, feature_impl->feature.base.
                nested_component.wsdl_component.ops);
        feature_impl->feature.base.nested_component.wsdl_component.ops = NULL;
    }


    if(feature_impl->feature.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, feature_impl->feature.base.documentable.
                base.wsdl_obj.base.wsdl_element.base.attr_extensible.ops);
        feature_impl->feature.base.documentable.base.wsdl_obj.base.wsdl_element.
                base.attr_extensible.ops = NULL;
    }

    if(feature_impl->feature.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, feature_impl->feature.base.documentable.
                base.wsdl_obj.base.wsdl_element.base.element_extensible.ops);
        feature_impl->feature.base.documentable.base.wsdl_obj.base.wsdl_element.
                base.element_extensible.ops = NULL;
    }
    if((&(feature_impl->feature))->ops)
    {
        AXIS2_FREE(env->allocator, (&(feature_impl->feature))->ops);
        (&(feature_impl->feature))->ops = NULL;
    }
    
    if(feature_impl)
    {
        AXIS2_FREE(env->allocator, feature_impl);
        feature_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_woden_feature_super_objs(
        void *feature,
        const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    return feature_impl->super;
}

axis2_woden_obj_types_t AXIS2_CALL
axis2_woden_feature_type(
        void *feature,
        const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    feature_impl = INTF_TO_IMPL(feature);

    return feature_impl->obj_type;
}

axis2_woden_documentable_t *AXIS2_CALL
axis2_woden_feature_get_base_impl(
        void *feature,
        const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    feature_impl = INTF_TO_IMPL(feature);

    return feature_impl->documentable;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_resolve_methods(
        axis2_woden_feature_t *feature,
        const axis2_env_t *env,
        axis2_woden_feature_t *feature_impl,
        axis2_hash_t *methods)
{
    axis2_woden_feature_impl_t *feature_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    feature_impl_l = INTF_TO_IMPL(feature_impl);
    
    feature->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    feature->ops->to_feature_free = axis2_hash_get(methods, "to_feature_free", 
            AXIS2_HASH_KEY_STRING);
    feature->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    feature->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    
    feature->ops->get_ref = axis2_hash_get(methods, 
            "get_ref", AXIS2_HASH_KEY_STRING);
    if(!feature->ops->get_ref && feature_impl_l)
            feature->ops->get_ref = 
            feature_impl_l->feature.ops->get_ref;
    
    feature->ops->is_required = axis2_hash_get(methods, 
            "is_required", AXIS2_HASH_KEY_STRING);
    if(!feature->ops->is_required && feature_impl_l)
            feature->ops->is_required = 
            feature_impl_l->feature.ops->is_required;
    
    feature->ops->get_parent = axis2_hash_get(methods, 
            "get_parent", AXIS2_HASH_KEY_STRING);
    if(!feature->ops->get_parent && feature_impl_l)
            feature->ops->get_parent = 
            feature_impl_l->feature.ops->get_parent;

    feature->ops->to_element = axis2_hash_get(methods, 
            "to_element", AXIS2_HASH_KEY_STRING);
    if(!feature->ops->to_element && feature_impl_l)
            feature->ops->to_element = 
            feature_impl_l->feature.ops->to_element;
   
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_set_ref(
        void *feature,
        const axis2_env_t *env,
        axis2_url_t *ref)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_FEATURE_SUPER_OBJS(feature, env);
    feature_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_FEATURE", AXIS2_HASH_KEY_STRING));

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
        const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_FEATURE_SUPER_OBJS(feature, env);
    feature_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_FEATURE", AXIS2_HASH_KEY_STRING));
    
    return feature_impl->f_ref;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_set_required(
        void *feature,
        const axis2_env_t *env,
        axis2_bool_t required)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_FEATURE_SUPER_OBJS(feature, env);
    feature_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_FEATURE", AXIS2_HASH_KEY_STRING));

    feature_impl->f_required = required;
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_woden_feature_is_required(
        void *feature,
        const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_FEATURE_SUPER_OBJS(feature, env);
    feature_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_FEATURE", AXIS2_HASH_KEY_STRING));

    return feature_impl->f_required;
}

axis2_status_t AXIS2_CALL
axis2_woden_feature_set_parent_element(
        void *feature,
        const axis2_env_t *env,
        axis2_woden_wsdl_element_t *parent)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_FEATURE_SUPER_OBJS(feature, env);
    feature_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_FEATURE", AXIS2_HASH_KEY_STRING));

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
        const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_FEATURE_SUPER_OBJS(feature, env);
    feature_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_FEATURE", AXIS2_HASH_KEY_STRING));

    return feature_impl->f_parent;
}

void *AXIS2_CALL
axis2_woden_feature_get_parent(
        void *feature,
        const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_FEATURE_SUPER_OBJS(feature, env);
    feature_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_FEATURE", AXIS2_HASH_KEY_STRING));

    /* TODO complete this */
    return NULL;
}

void *AXIS2_CALL
axis2_woden_feature_to_element(
        void *feature,
        const axis2_env_t *env)
{
    axis2_woden_feature_impl_t *feature_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_FEATURE_SUPER_OBJS(feature, env);
    feature_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_FEATURE", AXIS2_HASH_KEY_STRING));
    
    return &(feature_impl->feature);
}

