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
#include <woden/wsdl20/axis2_woden_property.h>
#include <woden/wsdl20/extensions/axis2_woden_ext_element.h>
#include <woden/wsdl20/extensions/axis2_woden_component_exts.h>
#include <woden/xml/axis2_woden_xml_attr.h>
#include <woden/wsdl20/xml/axis2_woden_documentation_element.h>
#include <woden/wsdl20/axis2_woden_type_def.h>
#include <woden/util/axis2_woden_generic_obj.h>
#include <xml_schema/axis2_xml_schema_type.h>

typedef struct axis2_woden_property_impl axis2_woden_property_impl_t;

/** 
 * @brief Feature Struct Impl
 *	Axis2 Feature  
 */ 
struct axis2_woden_property_impl
{
    axis2_woden_property_t property;
    axis2_woden_obj_types_t obj_type;
    axis2_woden_documentable_t *documentable;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    /* WSDL Component model data */
    axis2_url_t *f_ref;
    axis2_generic_obj_t *f_value;
    axis2_woden_type_def_t *f_value_constraint;
    void *f_parent;
    /* XML Element data */
    axis2_qname_t *f_constraint_qname;
    axis2_bool_t f_has_value_token;
    void *f_types;
};

#define INTF_TO_IMPL(property) ((axis2_woden_property_impl_t *) property)

axis2_status_t AXIS2_CALL 
axis2_woden_property_free(
        void *property,
        axis2_env_t **env);

axis2_hash_t *AXIS2_CALL 
axis2_woden_property_super_objs(
        void *property,
        axis2_env_t **env);

axis2_woden_obj_types_t AXIS2_CALL 
axis2_woden_property_type(
        void *property,
        axis2_env_t **env);

axis2_woden_documentable_t *AXIS2_CALL
axis2_woden_property_get_base_impl(
        void *property,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_property_set_ref(
        void *property,
        axis2_env_t **env,
        axis2_url_t *ref);

axis2_url_t *AXIS2_CALL
axis2_woden_property_get_ref(
        void *property,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_property_set_parent_element(
        void *property,
        axis2_env_t **env,
        axis2_woden_wsdl_element_t *parent);

void *AXIS2_CALL
axis2_woden_property_get_parent_element(
        void *property,
        axis2_env_t **env);

void *AXIS2_CALL
axis2_woden_property_get_parent(
        void *property,
        axis2_env_t **env);

void *AXIS2_CALL
axis2_woden_property_to_element(
        void *property,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_property_set_value(
        void *property,
        axis2_env_t **env,
        void *value);

void *AXIS2_CALL
axis2_woden_property_get_value(
        void *property,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_property_set_value_constraint(
        void *property,
        axis2_env_t **env,
        void *type_def);

void *AXIS2_CALL
axis2_woden_property_get_value_constraint(
        void *property,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_property_set_constraint_qname(
        void *property,
        axis2_env_t **env,
        axis2_qname_t *type_def);

axis2_qname_t *AXIS2_CALL
axis2_woden_property_get_constraint_qname(
        void *property,
        axis2_env_t **env);

void *AXIS2_CALL
axis2_woden_property_get_constraint(
        void *property,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_property_set_types(
        void *property,
        axis2_env_t **env,
        void *types);

axis2_status_t AXIS2_CALL
axis2_woden_property_set_has_value_token(
        void *property,
        axis2_env_t **env,
        axis2_bool_t b);

axis2_bool_t AXIS2_CALL
axis2_woden_property_has_value_token(
        void *property,
        axis2_env_t **env);

static axis2_woden_property_t *
create(axis2_env_t **env);

static axis2_status_t
axis2_woden_property_free_ops(
        void *property,
        axis2_env_t **env);

/************************Woden C Internal Methods******************************/
AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_to_property_element(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!property)
    {
        property_impl = (axis2_woden_property_impl_t *) create(env);
    }
    else
        property_impl = (axis2_woden_property_impl_t *) property;

    axis2_woden_property_free_ops(property, env);

    property_impl->property.base.property_element.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_property_element_ops_t));
    axis2_woden_property_element_resolve_methods(&(property_impl->property.base.
            property_element), env, property_impl->methods);
    return property;
}

AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_to_nested_element(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!property)
    {
        property_impl = (axis2_woden_property_impl_t *) create(env);
    }
    else
        property_impl = (axis2_woden_property_impl_t *) property;
    axis2_woden_property_free_ops(property, env);

    property_impl->property.base.property_element.base.nested_element.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_nested_element_ops_t));
    axis2_woden_nested_element_resolve_methods(&(property_impl->property.base.
            property_element.base.nested_element), env, property_impl->methods);
    return property;
}

AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_to_documentable_element(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!property)
    {
        property_impl = (axis2_woden_property_impl_t *) create(env);
    }
    else
        property_impl = (axis2_woden_property_impl_t *) property;
    axis2_woden_property_free_ops(property, env);

    property_impl->property.base.property_element.base.documentable_element.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_documentable_element_ops_t));
    axis2_woden_documentable_element_resolve_methods(&(property_impl->property.base.
            property_element.base.documentable_element), env, 
            property_impl->methods);
    return property;
}

AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_to_documentable(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!property)
    {
        property_impl = (axis2_woden_property_impl_t *) create(env);
    }
    else
        property_impl = (axis2_woden_property_impl_t *) property;
    axis2_woden_property_free_ops(property, env);

    property_impl->property.base.documentable.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_documentable_ops_t));
    axis2_woden_documentable_resolve_methods(&(property_impl->property.base.
            documentable), env, property_impl->documentable, property_impl->methods);
    return property;
}

AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_to_wsdl_obj(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;
    axis2_woden_wsdl_obj_t *wsdl_obj = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!property)
    {
        property_impl = (axis2_woden_property_impl_t *) create(env);
    }
    else
        property_impl = (axis2_woden_property_impl_t *) property;
    axis2_woden_property_free_ops(property, env);

    property_impl->property.base.documentable.base.wsdl_obj.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_wsdl_obj_ops_t));
    wsdl_obj = AXIS2_WODEN_DOCUMENTABLE_GET_BASE_IMPL(
            property_impl->documentable, env);
    axis2_woden_wsdl_obj_resolve_methods(&(property_impl->property.base.
            documentable.base.wsdl_obj), env, wsdl_obj, property_impl->methods);
    return property;
}

AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_to_nested_component(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!property)
    {
        property_impl = (axis2_woden_property_impl_t *) create(env);
    }
    else
        property_impl = (axis2_woden_property_impl_t *) property;
    axis2_woden_property_free_ops(property, env);
  
    property_impl->property.base.nested_component.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_nested_component_ops_t));
    axis2_woden_nested_component_resolve_methods(&(property_impl->property.base.
            nested_component), env, property_impl->methods);
    return property;
}

AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_to_wsdl_component(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!property)
    {
        property_impl = (axis2_woden_property_impl_t *) create(env);
    }
    else
        property_impl = (axis2_woden_property_impl_t *) property;
    axis2_woden_property_free_ops(property, env);
    property_impl->property.base.nested_component.wsdl_component.ops = 
        AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_woden_wsdl_component_ops_t));
    axis2_woden_wsdl_component_resolve_methods(&(property_impl->property.base.
            nested_component.wsdl_component), env, property_impl->methods);
    return property;
}

AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_to_attr_extensible(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!property)
    {
        property_impl = (axis2_woden_property_impl_t *) create(env);
    }
    else
        property_impl = (axis2_woden_property_impl_t *) property;
    axis2_woden_property_free_ops(property, env);

    property_impl->property.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.attr_extensible.ops =
            AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_woden_attr_extensible_ops_t));
    axis2_woden_attr_extensible_resolve_methods(&(property_impl->property.base.
            documentable.base.wsdl_obj.base.wsdl_element.base.attr_extensible), 
            env, NULL, property_impl->methods);
    return property;

}

AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_to_element_extensible(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!property)
    {
        property_impl = (axis2_woden_property_impl_t *) create(env);
    }
    else
        property_impl = (axis2_woden_property_impl_t *) property;
    axis2_woden_property_free_ops(property, env);
    property_impl->property.base.documentable.base.wsdl_obj.base.wsdl_element.
                base.element_extensible.ops = AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_element_extensible_ops_t));
    axis2_woden_element_extensible_resolve_methods(&(property_impl->property.base.
            documentable.base.wsdl_obj.base.wsdl_element.base.element_extensible), 
            env, NULL, property_impl->methods);
    return property;

}

/************************End of Woden C Internal Methods***********************/
static axis2_woden_property_t *
create(axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    property_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_property_impl_t));

    property_impl->obj_type= AXIS2_WODEN_PROPERTY;
    property_impl->super = NULL;
    property_impl->methods = NULL;
    property_impl->f_ref = NULL;
    property_impl->f_value = NULL;
    property_impl->f_value_constraint = NULL;
    property_impl->f_parent = NULL;
    property_impl->f_constraint_qname = NULL;
    property_impl->f_has_value_token = AXIS2_FALSE;
    property_impl->f_types = NULL;
    
    property_impl->property.base.property_element.ops = NULL;
    property_impl->property.base.property_element.base.nested_element.ops = NULL;
    property_impl->property.base.property_element.base.documentable_element.ops = 
            NULL;
    property_impl->property.base.documentable.ops = NULL;
    property_impl->property.base.nested_component.ops = NULL; 
    property_impl->property.base.nested_component.wsdl_component.ops = NULL;
    property_impl->property.base.documentable.base.wsdl_obj.ops = NULL;
    property_impl->property.base.documentable.base.wsdl_obj.base.wsdl_element.
            base.attr_extensible.ops = NULL;
    property_impl->property.base.documentable.base.wsdl_obj.base.wsdl_element.
            base.element_extensible.ops = NULL;
    
    property_impl->property.ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_woden_property_ops_t));

    property_impl->property.ops->free = axis2_woden_property_free;
    property_impl->property.ops->super_objs = axis2_woden_property_super_objs;
    property_impl->property.ops->type = axis2_woden_property_type;
    property_impl->property.ops->get_base_impl = axis2_woden_property_get_base_impl;

    property_impl->property.ops->get_ref = axis2_woden_property_get_ref;
    property_impl->property.ops->get_value_constraint = 
        axis2_woden_property_get_value_constraint;
    property_impl->property.ops->get_value = axis2_woden_property_get_value;
    property_impl->property.ops->get_parent = axis2_woden_property_get_parent;
    property_impl->property.ops->to_element = axis2_woden_property_to_element;
 
    property_impl->methods = axis2_hash_make(env);
    if(!property_impl->methods) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(property_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_woden_property_free);
    axis2_hash_set(property_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING, 
            axis2_woden_property_super_objs);
    axis2_hash_set(property_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_woden_property_type);

    axis2_hash_set(property_impl->methods, "set_ref", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_property_set_ref);
    axis2_hash_set(property_impl->methods, "get_ref", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_property_get_ref);
    axis2_hash_set(property_impl->methods, "set_parent_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_property_set_parent_element);
    axis2_hash_set(property_impl->methods, "get_parent_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_property_get_parent_element);
    axis2_hash_set(property_impl->methods, "get_parent", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_property_get_parent);
    axis2_hash_set(property_impl->methods, "to_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_property_to_element);
    axis2_hash_set(property_impl->methods, "set_value", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_property_set_value);
    axis2_hash_set(property_impl->methods, "get_value", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_property_get_value);
    axis2_hash_set(property_impl->methods, "set_value_constraint", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_property_set_value_constraint);
    axis2_hash_set(property_impl->methods, "get_value_constraint", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_property_get_value_constraint);
    axis2_hash_set(property_impl->methods, "set_constraint_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_property_set_constraint_qname);
    axis2_hash_set(property_impl->methods, "get_constraint_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_property_get_constraint_qname);
    axis2_hash_set(property_impl->methods, "get_constraint", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_property_get_constraint);
    axis2_hash_set(property_impl->methods, "set_types", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_property_set_types);
    axis2_hash_set(property_impl->methods, "set_has_value_token", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_property_set_has_value_token);
    axis2_hash_set(property_impl->methods, "has_value_token", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_property_has_value_token);

    return &(property_impl->property);
}

AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_create(axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    property_impl = (axis2_woden_property_impl_t *) create(env);

    property_impl->documentable = axis2_woden_documentable_create(env);

    property_impl->super = axis2_hash_make(env);
    if(!property_impl->super) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(property_impl->super, "AXIS2_WODEN_PROPERTY", 
            AXIS2_HASH_KEY_STRING, &(property_impl->property));
    axis2_hash_set(property_impl->super, "AXIS2_WODEN_DOCUMENTABLE", 
            AXIS2_HASH_KEY_STRING, property_impl->documentable);
    
    return &(property_impl->property);
}

static axis2_status_t
axis2_woden_property_free_ops(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    property_impl = INTF_TO_IMPL(property);

    if(property_impl->property.base.property_element.ops)
    {
        AXIS2_FREE((*env)->allocator, property_impl->property.base.
                property_element.ops);
        property_impl->property.base.property_element.ops = NULL;
    }

    if(property_impl->property.base.property_element.base.nested_element.ops)
    {
        AXIS2_FREE((*env)->allocator, property_impl->property.base.
                property_element.base.nested_element.ops);
        property_impl->property.base.property_element.base.nested_element.ops = 
            NULL;
    }
    
    if(property_impl->property.base.property_element.base.documentable_element.ops)
    {
        AXIS2_FREE((*env)->allocator, property_impl->property.base.
                property_element.base.documentable_element.ops);
        property_impl->property.base.property_element.base.documentable_element.ops = 
            NULL;
    }
    
    if(property_impl->property.base.documentable.ops)
    {
        AXIS2_FREE((*env)->allocator, property_impl->property.base.
                documentable.ops);
        property_impl->property.base.documentable.ops = 
            NULL;
    }
    
    if(property_impl->property.base.documentable.base.wsdl_obj.ops)
    {
        AXIS2_FREE((*env)->allocator, property_impl->property.base.
                documentable.base.wsdl_obj.ops);
        property_impl->property.base.documentable.base.wsdl_obj.ops = 
            NULL;
    }

    if(property_impl->property.base.nested_component.ops)
    {
        AXIS2_FREE((*env)->allocator, property_impl->property.base.
                nested_component.ops);
        property_impl->property.base.nested_component.ops = NULL;
    }

    if(property_impl->property.base.nested_component.wsdl_component.ops)
    {
        AXIS2_FREE((*env)->allocator, property_impl->property.base.
                nested_component.wsdl_component.ops);
        property_impl->property.base.nested_component.wsdl_component.ops = NULL;
    }


    if(property_impl->property.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE((*env)->allocator, property_impl->property.base.documentable.
                base.wsdl_obj.base.wsdl_element.base.attr_extensible.ops);
        property_impl->property.base.documentable.base.wsdl_obj.base.wsdl_element.
                base.attr_extensible.ops = NULL;
    }

    if(property_impl->property.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE((*env)->allocator, property_impl->property.base.documentable.
                base.wsdl_obj.base.wsdl_element.base.element_extensible.ops);
        property_impl->property.base.documentable.base.wsdl_obj.base.wsdl_element.
                base.element_extensible.ops = NULL;
    }
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_woden_property_free(void *property,
                        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    property_impl = INTF_TO_IMPL(property);

    if(property_impl->f_ref)
    {
        AXIS2_URL_FREE(property_impl->f_ref, env);
        property_impl->f_ref = NULL;
    }
   
    /* TODO free f_parent */
    
    if(property_impl->super)
    {
        axis2_hash_free(property_impl->super, env);
        property_impl->super = NULL;
    }
    
    if(property_impl->methods)
    {
        axis2_hash_free(property_impl->methods, env);
        property_impl->methods = NULL;
    }

    if(property_impl->documentable)
    {
        AXIS2_WODEN_DOCUMENTABLE_FREE(property_impl->documentable, env);
        property_impl->documentable = NULL;
    }

    axis2_woden_property_free_ops(property, env);

    if((&(property_impl->property))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(property_impl->property))->ops);
        (&(property_impl->property))->ops = NULL;
    }
    
    if(property_impl)
    {
        AXIS2_FREE((*env)->allocator, property_impl);
        property_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_woden_property_super_objs(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    property_impl = INTF_TO_IMPL(property);

    return property_impl->super;
}

axis2_woden_obj_types_t AXIS2_CALL
axis2_woden_property_type(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    property_impl = INTF_TO_IMPL(property);

    return property_impl->obj_type;
}

axis2_woden_documentable_t *AXIS2_CALL
axis2_woden_property_get_base_impl(void *property,
                                axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    property_impl = INTF_TO_IMPL(property);

    return property_impl->documentable;
}

axis2_status_t AXIS2_CALL
axis2_woden_property_resolve_methods(
        axis2_woden_property_t *property,
        axis2_env_t **env,
        axis2_woden_property_t *property_impl,
        axis2_hash_t *methods)
{
    axis2_woden_property_impl_t *property_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    property_impl_l = INTF_TO_IMPL(property_impl);
    
    property->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    property->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    property->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);

    property->ops->get_ref = axis2_hash_get(methods, 
            "get_ref", AXIS2_HASH_KEY_STRING);
    if(!property->ops->get_ref && property_impl_l)
            property->ops->get_ref = 
            property_impl_l->property.ops->get_ref;
    
    property->ops->get_value_constraint = axis2_hash_get(methods, 
            "get_value_constraint", AXIS2_HASH_KEY_STRING);
    if(!property->ops->get_value_constraint && property_impl_l)
            property->ops->get_value_constraint = 
            property_impl_l->property.ops->get_value_constraint;
    
    property->ops->get_value = axis2_hash_get(methods, 
            "get_value", AXIS2_HASH_KEY_STRING);
    if(!property->ops->get_value && property_impl_l)
            property->ops->get_value = 
            property_impl_l->property.ops->get_value;
    
    property->ops->get_parent = axis2_hash_get(methods, 
            "get_parent", AXIS2_HASH_KEY_STRING);
    if(!property->ops->get_parent && property_impl_l)
            property->ops->get_parent = 
            property_impl_l->property.ops->get_parent;
    
    property->ops->to_element = axis2_hash_get(methods, 
            "to_element", AXIS2_HASH_KEY_STRING);
    if(!property->ops->to_element && property_impl_l)
            property->ops->to_element = 
            property_impl_l->property.ops->to_element;
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_property_set_ref(
        void *property,
        axis2_env_t **env,
        axis2_url_t *ref)
{
    axis2_woden_property_impl_t *property_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    if(property_impl->f_ref)
    {
        AXIS2_URL_FREE(property_impl->f_ref, env);
    }
    property_impl->f_ref = ref;
    return AXIS2_SUCCESS;
}

axis2_url_t *AXIS2_CALL
axis2_woden_property_get_ref(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));
    
    return property_impl->f_ref;
}

axis2_status_t AXIS2_CALL
axis2_woden_property_set_parent_element(
        void *property,
        axis2_env_t **env,
        axis2_woden_wsdl_element_t *parent)
{
    axis2_woden_property_impl_t *property_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    /*if(property_impl->f_parent)
    {
        AXIS2_WODEN_WSDL_ELEMENT_FREE(parent, env);
    }*/
    property_impl->f_parent = parent;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_woden_property_get_parent_element(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    return property_impl->f_parent;
}

void *AXIS2_CALL
axis2_woden_property_get_parent(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    return property_impl->f_parent;
}

void *AXIS2_CALL
axis2_woden_property_to_element(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));
    
    return &(property_impl->property);
}

axis2_status_t AXIS2_CALL
axis2_woden_property_set_value(
        void *property,
        axis2_env_t **env,
        void *value)
{
    axis2_woden_property_impl_t *property_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    if(property_impl->f_value)
    {
        AXIS2_WODEN_GENERIC_OBJ_FREE(property_impl->f_value, env);
        property_impl->f_value = NULL;
    }
    property_impl->f_value = value;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_woden_property_get_value(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    return property_impl->f_value;
}

axis2_status_t AXIS2_CALL
axis2_woden_property_set_value_constraint(
        void *property,
        axis2_env_t **env,
        void *type_def)
{
    axis2_woden_property_impl_t *property_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    /*if(property_impl->f_value_constraint)
    {
        AXIS2_WODEN_TYPE_DEFINITION_FREE(property_impl->f_value_constraint, 
                env);
        property_impl->f_value_constraint = NULL;
    }*/
    property_impl->f_value_constraint = type_def;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_woden_property_get_value_constraint(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    return property_impl->f_value_constraint;
}

axis2_status_t AXIS2_CALL
axis2_woden_property_set_constraint_qname(
        void *property,
        axis2_env_t **env,
        axis2_qname_t *constraint)
{
    axis2_woden_property_impl_t *property_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    if(property_impl->f_constraint_qname)
    {
        AXIS2_QNAME_FREE(property_impl->f_constraint_qname, env);
        property_impl->f_constraint_qname = NULL;
    }
    property_impl->f_constraint_qname = AXIS2_QNAME_CLONE(constraint, env);
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
axis2_woden_property_get_constraint_qname(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    return property_impl->f_constraint_qname;
}

void *AXIS2_CALL
axis2_woden_property_get_constraint(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;
    axis2_xml_schema_type_t *xst = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));
   
    if(property_impl->f_constraint_qname && property_impl->f_types)
    {
        xst = (axis2_xml_schema_type_t *) 
            AXIS2_WODEN_TYPES_GET_TYPE_DEFINITION(property_impl->f_types, env, 
                property_impl->f_constraint_qname);
    }
    return xst;
}

axis2_status_t AXIS2_CALL
axis2_woden_property_set_types(
        void *property,
        axis2_env_t **env,
        void *types)
{
    axis2_woden_property_impl_t *property_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));
    
    /*if(property_impl->f_types)
    {
        AXIS2_WODEN_TYPES_ELEMENT_FREE(property_impl->f_types, env);
        property_impl->f_types = NULL;
    }*/
    property_impl->f_types = types;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_property_set_has_value_token(
        void *property,
        axis2_env_t **env,
        axis2_bool_t b)
{
    axis2_woden_property_impl_t *property_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    property_impl->f_has_value_token = b;
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_woden_property_has_value_token(
        void *property,
        axis2_env_t **env)
{
    axis2_woden_property_impl_t *property_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    return property_impl->f_has_value_token;
}


