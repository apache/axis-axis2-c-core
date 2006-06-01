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

#include <xml_schema/axis2_xml_schema_element.h>
#include <xml_schema/axis2_xml_schema_annotation.h>
#include <xml_schema/axis2_xml_schema_particle.h>
#include <xml_schema/axis2_xml_schema_derivation_method.h>
#include <xml_schema/axis2_xml_schema_obj_collection.h>
#include <xml_schema/axis2_xml_schema_type.h>
#include <xml_schema/axis2_xml_schema_form.h>
#include <axis2_utils.h>


typedef struct axis2_xml_schema_element_impl axis2_xml_schema_element_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_element_impl
{
    axis2_xml_schema_element_t element;
    
    axis2_xml_schema_annotated_t *type_recv;
    
    axis2_xml_schema_particle_t *particle;
    
    axis2_hash_t *methods;
    /**
     * Attribute used to block a type derivation.
     */
    axis2_xml_schema_derivation_method_t *block;

    /**
     * The value after an element has been compiled to post-schema infoset.
     * This value is either from the type itself or, if not defined on the 
     * type, taken from the schema element.
     */
    axis2_xml_schema_derivation_method_t *block_resolved;
    
    axis2_xml_schema_obj_collection_t *constraints;

    /**
     * Provides the default value of the element if its content
     * is a simple type or the element's content is textOnly.
     */
    axis2_char_t *default_value;
    
    axis2_char_t *fixed_value;

    /**
     * Returns the correct common runtime library
     * object based upon the Schema Type for the element.
     */
    void *element_type;

    axis2_xml_schema_derivation_method_t *final_derivation;
    
    axis2_xml_schema_derivation_method_t *final_derivation_resolved;

    /**
     * The default value is the value of the Element Form Default attribute for 
     * the schema element containing the attribute. The default is Unqualified.
     */
    axis2_xml_schema_form_t *form;
    
    axis2_bool_t is_abstract;
    
    axis2_bool_t is_nillable;
    
    axis2_char_t *name;
    
    axis2_qname_t *qualified_qname;
    
    axis2_qname_t *ref_qname;

    /**
     * Returns the type of the element.
     * This can either be a complex type or a simple type.
     */
    axis2_xml_schema_type_t *schema_type;

    /**
     * QName of a built-in data type defined in this schema or another
     * schema indicated by the specified namespace.
     */
    axis2_qname_t *schema_type_qname;

    /**
     * QName of an element that can be a substitute for this element.
     */
    axis2_qname_t *substitution_group;
    
    axis2_hash_t *ht_super;
    
    axis2_xml_schema_types_t obj_type;

};

#define AXIS2_INTF_TO_IMPL(element) ((axis2_xml_schema_element_impl_t *) element)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_free(void *element,
                        axis2_env_t **env);

axis2_xml_schema_particle_t *AXIS2_CALL
axis2_xml_schema_element_get_base_impl(void *element,
                                            axis2_env_t **env);

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_element_type(void *element,
                            axis2_env_t **env);
                            
axis2_hash_t *AXIS2_CALL
axis2_xml_schema_element_super_objs(void *element,
                                    axis2_env_t **env);                            
/**
 * Returns a collection of constraints on the element.
 */
axis2_xml_schema_obj_collection_t *AXIS2_CALL
axis2_xml_schema_element_get_constraints(void *element,
                                            axis2_env_t **env); 

axis2_char_t *AXIS2_CALL 
axis2_xml_schema_element_get_default_value(void *element,
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_default_value(void *element,
                                            axis2_env_t **env,
                                            axis2_char_t *default_value);

axis2_xml_schema_derivation_method_t *AXIS2_CALL
axis2_xml_schema_element_get_block(void *element,
                                    axis2_env_t **env); 

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_block(void *element,
                                    axis2_env_t **env,
                                    axis2_xml_schema_derivation_method_t * block);

axis2_xml_schema_derivation_method_t *AXIS2_CALL
axis2_xml_schema_element_get_final(void *element,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_element_set_final(void *element,
                                    axis2_env_t **env,
                                    axis2_xml_schema_derivation_method_t *final_derivation);

axis2_xml_schema_derivation_method_t *AXIS2_CALL 
axis2_xml_schema_element_get_block_resolved(void *element,
                                            axis2_env_t **env);

axis2_char_t *AXIS2_CALL 
axis2_xml_schema_element_get_fixed_value(void *element,
                                            axis2_env_t **env); 

axis2_status_t AXIS2_CALL
axis2_xml_schema_element_set_fixed_value(void *element,
                                            axis2_env_t **env,
                                            axis2_char_t *fixed_value); 
void *AXIS2_CALL
axis2_xml_schema_element_get_element_type(void *element,
                                            axis2_env_t **env); 

axis2_xml_schema_form_t *AXIS2_CALL
axis2_xml_schema_element_get_form(void *element,
                                    axis2_env_t **env); 

axis2_status_t AXIS2_CALL
axis2_xml_schema_element_set_form(void *element,
                                    axis2_env_t **env,
                                    axis2_xml_schema_form_t *form); 

axis2_bool_t AXIS2_CALL
axis2_xml_schema_element_is_abstract(void *element,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_abstract(void *element,
                                        axis2_env_t **env,
                                        axis2_bool_t is_abstract);

axis2_bool_t AXIS2_CALL
axis2_xml_schema_element_is_nillable(void *element,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_nillable(void *element,
                                        axis2_env_t **env,
                                        axis2_bool_t  is_nillable);

axis2_char_t *AXIS2_CALL 
axis2_xml_schema_element_get_name(void *element,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_name(void *element,
                                    axis2_env_t **env,
                                    axis2_char_t *name);

axis2_qname_t *AXIS2_CALL 
axis2_xml_schema_element_get_ref_qname(void *element,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_ref_qname(void *element,
                                        axis2_env_t **env,
                                        axis2_qname_t *ref_qname);

axis2_qname_t *AXIS2_CALL 
axis2_xml_schema_element_get_qname(void *element,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_qname(void *element,
                                    axis2_env_t **env,
                                    axis2_qname_t *qualified_name); 

axis2_xml_schema_type_t *AXIS2_CALL
axis2_xml_schema_element_get_schema_type(void *element,
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_schema_type(void *element,
                                            axis2_env_t **env,
                                            axis2_xml_schema_type_t *schema_type);

axis2_qname_t *AXIS2_CALL 
axis2_xml_schema_element_get_schema_type_qname(void *element,
                                                axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_schema_type_qname(void *element,
                                                axis2_env_t **env,
                                                axis2_qname_t *schema_type_qname);

axis2_qname_t *AXIS2_CALL 
axis2_xml_schema_element_get_substitution_group(void *element,
                                                axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_substitution_group(void *element,
                                                axis2_env_t **env,
                                                axis2_qname_t *substitution_group);

axis2_char_t *AXIS2_CALL 
axis2_xml_schema_element_to_string(void *element,
                                    axis2_env_t **env,
                                    axis2_char_t *prefix, 
                                    int tab);

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_type(void *element,
                                    axis2_env_t **env,
                                    axis2_xml_schema_type_t *type);

AXIS2_EXTERN axis2_xml_schema_element_t * AXIS2_CALL
axis2_xml_schema_element_create(axis2_env_t **env)
{
    axis2_xml_schema_element_impl_t *element_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_xml_schema_annotated_t *annotated = NULL;
    element_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_element_impl_t));

    element_impl->type_recv = NULL;
    element_impl->methods = NULL;
    element_impl->block = NULL;
    element_impl->block_resolved = NULL;
    element_impl->constraints = NULL;
    element_impl->default_value = NULL;
    element_impl->element_type = NULL;
    element_impl->final_derivation = NULL;
    element_impl->final_derivation_resolved = NULL;
    element_impl->fixed_value = NULL;
    element_impl->form = NULL;
    element_impl->is_abstract = AXIS2_FALSE;
    element_impl->is_nillable = AXIS2_FALSE;
    element_impl->methods = NULL;
    element_impl->schema_type = NULL;
    element_impl->schema_type_qname = NULL;
    element_impl->element.ops = NULL;
    element_impl->element.base.ops = NULL;
    element_impl->name = NULL;
    element_impl->particle = NULL;
    element_impl->ref_qname = NULL;
    element_impl->substitution_group = NULL;
    element_impl->type_recv = NULL;
    element_impl->schema_type_qname = NULL;
    element_impl->substitution_group = NULL;
    element_impl->qualified_qname = NULL;
    element_impl->ht_super = NULL;
    element_impl->obj_type = AXIS2_XML_SCHEMA_ELEMENT;
    

    element_impl->element.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_element_ops_t));
    
    if(!element_impl->element.ops)
    {
        AXIS2_FREE((*env)->allocator, element_impl);
        return NULL;
    }                    

    element_impl->element.ops->free = axis2_xml_schema_element_free;
    element_impl->element.ops->get_base_impl = 
            axis2_xml_schema_element_get_base_impl;
    element_impl->element.ops->super_objs =
            axis2_xml_schema_element_super_objs;
    element_impl->element.ops->type =
            axis2_xml_schema_element_type;
                        
    element_impl->element.ops->get_constraints = 
            axis2_xml_schema_element_get_constraints;
    element_impl->element.ops->get_default_value = 
            axis2_xml_schema_element_get_default_value;
    element_impl->element.ops->set_default_value = 
            axis2_xml_schema_element_set_default_value;
    element_impl->element.ops->get_block = 
            axis2_xml_schema_element_get_block;
    element_impl->element.ops->set_block = 
            axis2_xml_schema_element_set_block;
    element_impl->element.ops->get_final = 
            axis2_xml_schema_element_get_final;
    element_impl->element.ops->set_final = 
            axis2_xml_schema_element_set_final;
    element_impl->element.ops->get_block_resolved = 
            axis2_xml_schema_element_get_block_resolved;
    element_impl->element.ops->get_fixed_value = 
            axis2_xml_schema_element_get_fixed_value;
    element_impl->element.ops->set_fixed_value = 
            axis2_xml_schema_element_set_fixed_value;
    element_impl->element.ops->get_element_type = 
            axis2_xml_schema_element_get_element_type;
    element_impl->element.ops->get_form = 
            axis2_xml_schema_element_get_form;
    element_impl->element.ops->set_form = 
            axis2_xml_schema_element_set_form;
    element_impl->element.ops->is_abstract = 
            axis2_xml_schema_element_is_abstract;
    element_impl->element.ops->set_abstract = 
            axis2_xml_schema_element_set_abstract;
    element_impl->element.ops->is_nillable = 
            axis2_xml_schema_element_is_nillable;
    element_impl->element.ops->set_nillable = 
            axis2_xml_schema_element_set_nillable;
    element_impl->element.ops->get_name = 
            axis2_xml_schema_element_get_name;
    element_impl->element.ops->set_name = 
            axis2_xml_schema_element_set_name;
    element_impl->element.ops->get_ref_qname = 
            axis2_xml_schema_element_get_ref_qname;
    element_impl->element.ops->set_ref_qname = 
            axis2_xml_schema_element_set_ref_qname;
    element_impl->element.ops->get_qname = 
            axis2_xml_schema_element_get_qname;
    element_impl->element.ops->set_qname = 
            axis2_xml_schema_element_set_qname;
    element_impl->element.ops->get_schema_type = 
            axis2_xml_schema_element_get_schema_type;
    element_impl->element.ops->set_schema_type = 
            axis2_xml_schema_element_set_schema_type;
    element_impl->element.ops->get_schema_type_qname = 
            axis2_xml_schema_element_get_schema_type_qname;
    element_impl->element.ops->set_schema_type_qname = 
            axis2_xml_schema_element_set_schema_type_qname;
    element_impl->element.ops->get_substitution_group = 
            axis2_xml_schema_element_get_substitution_group;
    element_impl->element.ops->set_substitution_group = 
            axis2_xml_schema_element_set_substitution_group;
    element_impl->element.ops->to_string = 
            axis2_xml_schema_element_to_string;
    element_impl->element.ops->set_type = 
            axis2_xml_schema_element_set_type;
   
    element_impl->methods = axis2_hash_make(env);
    if(!element_impl->methods)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(element_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_free);
    axis2_hash_set(element_impl->methods, "get_constraints", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_get_constraints);
    axis2_hash_set(element_impl->methods, "type", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_element_type);
    axis2_hash_set(element_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_element_super_objs);            
    axis2_hash_set(element_impl->methods, "get_default_value", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_get_default_value);
    axis2_hash_set(element_impl->methods, "set_default_value", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_set_default_value);
    axis2_hash_set(element_impl->methods, "get_block", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_get_block);
    axis2_hash_set(element_impl->methods, "set_block", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_set_block);
    axis2_hash_set(element_impl->methods, "get_final", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_get_final);
    axis2_hash_set(element_impl->methods, "set_final", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_set_final);
    axis2_hash_set(element_impl->methods, "get_block_resolved", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_get_block_resolved);
    axis2_hash_set(element_impl->methods, "get_fixed_value", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_get_fixed_value);
    axis2_hash_set(element_impl->methods, "set_fixed_value", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_set_fixed_value);
    axis2_hash_set(element_impl->methods, "get_element_type", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_get_element_type);
    axis2_hash_set(element_impl->methods, "get_form", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_get_form);
    axis2_hash_set(element_impl->methods, "set_form", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_set_form);
    axis2_hash_set(element_impl->methods, "is_abstract", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_is_abstract);
    axis2_hash_set(element_impl->methods, "set_abstract", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_set_abstract);
    axis2_hash_set(element_impl->methods, "is_nillable", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_is_nillable);
    axis2_hash_set(element_impl->methods, "set_nillable", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_set_nillable);
    axis2_hash_set(element_impl->methods, "get_name", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_get_name);
    axis2_hash_set(element_impl->methods, "set_name", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_set_name);
    axis2_hash_set(element_impl->methods, "get_ref_qname", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_get_ref_qname);
    axis2_hash_set(element_impl->methods, "set_ref_qname", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_set_ref_qname);
    axis2_hash_set(element_impl->methods, "get_qname", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_get_qname);
    axis2_hash_set(element_impl->methods, "get_qname", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_get_qname);
    axis2_hash_set(element_impl->methods, "set_qname", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_set_qname);
    axis2_hash_set(element_impl->methods, "get_schema_type", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_get_schema_type);
    axis2_hash_set(element_impl->methods, "set_schema_type", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_set_schema_type);
    axis2_hash_set(element_impl->methods, "get_schema_type_qname", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_get_schema_type_qname);
    axis2_hash_set(element_impl->methods, "set_schema-type_qname", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_set_schema_type_qname);
    axis2_hash_set(element_impl->methods, "get_substitution_group", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_get_substitution_group);
    axis2_hash_set(element_impl->methods, "set_substitution_group", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_set_substitution_group);
    axis2_hash_set(element_impl->methods, "to_string", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_to_string);
    axis2_hash_set(element_impl->methods, "set_type", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_element_set_type);

    /*element_impl->type_recv = axis2_xml_schema_type_receiver_create(env); */
    element_impl->particle = axis2_xml_schema_particle_create(env);
    if(!element_impl->particle)
    {
        axis2_xml_schema_element_free(&(element_impl->element), env);
        return NULL;
    }
    element_impl->ht_super = axis2_hash_make(env);
    if(!element_impl->ht_super)
    {
        axis2_xml_schema_element_free(&(element_impl->element), env);
        return NULL;
    }
    
    axis2_hash_set(element_impl->ht_super, "AXIS2_XML_SCHEMA_ELEMENT",
        AXIS2_HASH_KEY_STRING, &(element_impl->element));
    axis2_hash_set(element_impl->ht_super, "AXIS2_XML_SCHEMA_PARTICLE",
        AXIS2_HASH_KEY_STRING, element_impl->particle);
    
    annotated = AXIS2_XML_SCHEMA_PARTICLE_GET_BASE_IMPL(element_impl->particle, env);
    if(NULL != annotated)
    {
        axis2_hash_set(element_impl->ht_super, "AXIS2_XML_SCHEMA_ANNOTATED", 
            AXIS2_HASH_KEY_STRING, &(element_impl->element));
        axis2_hash_set(element_impl->ht_super, "AXIS2_XML_SCHEMA_OBJ",
            AXIS2_HASH_KEY_STRING, AXIS2_XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(annotated, env));            
    }
    status = axis2_xml_schema_particle_resolve_methods(
            &(element_impl->element.base), env, element_impl->particle, 
            element_impl->methods);

    return &(element_impl->element);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_element_free(void *element,
                                    axis2_env_t **env)
{
    axis2_xml_schema_element_impl_t *element_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    element_impl = AXIS2_INTF_TO_IMPL(element);

    if(element_impl->particle)
    {
        AXIS2_XML_SCHEMA_PARTICLE_FREE(element_impl->particle, env);
        element_impl->particle = NULL;
    }
 
    if(element_impl->methods)
    {
        axis2_hash_free(element_impl->methods, env);
        element_impl->methods = NULL;
    }
    
    if(element_impl->type_recv)
    {
        AXIS2_XML_SCHEMA_ANNOTATED_FREE(element_impl->type_recv, env);
        element_impl->type_recv = NULL;
    }
    
    if((&(element_impl->element))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(element_impl->element))->ops);
        (&(element_impl->element))->ops = NULL;
    }

    if(element_impl)
    {
        AXIS2_FREE((*env)->allocator, element_impl);
        element_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_particle_t *AXIS2_CALL
axis2_xml_schema_element_get_base_impl(void *element,
                                axis2_env_t **env)
{
    axis2_xml_schema_element_impl_t *element_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    element_impl = AXIS2_INTF_TO_IMPL(element);

    return element_impl->particle;
}

/*

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_xml_schema_element_resolve_methods(
                                axis2_xml_schema_element_t *element,
                                axis2_env_t **env,
                                axis2_xml_schema_element_t *element_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_element_impl_t *element_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, element_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    element_impl_l = (axis2_xml_schema_element_impl_t *) element_impl;
    
    element->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_element_ops_t));
    element->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    element->ops->get_base_impl = 
            element_impl_l->element.ops->get_base_impl;
    element->ops->get_constraints = 
            element_impl_l->element.ops->get_constraints;
    element->ops->get_default_value = 
            element_impl_l->element.ops->get_default_value;
    element->ops->set_default_value = 
            element_impl_l->element.ops->set_default_value;
    element->ops->get_block = 
            element_impl_l->element.ops->get_block;
    element->ops->set_block = 
            element_impl_l->element.ops->set_block;
    element->ops->get_final = 
            element_impl_l->element.ops->get_final;
    element->ops->set_final = 
            element_impl_l->element.ops->set_final;
    element->ops->get_block_resolved = 
            element_impl_l->element.ops->get_block_resolved;
    element->ops->get_fixed_value = 
            element_impl_l->element.ops->get_fixed_value;
    element->ops->set_fixed_value = 
            element_impl_l->element.ops->set_fixed_value;
    element->ops->get_element_type = 
            element_impl_l->element.ops->get_element_type;
    element->ops->get_form = 
            element_impl_l->element.ops->get_form;
    element->ops->set_form = 
            element_impl_l->element.ops->set_form;
    element->ops->is_abstract = 
            element_impl_l->element.ops->is_abstract;
    element->ops->set_abstract = 
            element_impl_l->element.ops->set_abstract;
    element->ops->is_nillable = 
            element_impl_l->element.ops->is_nillable;
    element->ops->set_nillable = 
            element_impl_l->element.ops->set_nillable;
    element->ops->get_name = 
            element_impl_l->element.ops->get_name;
    element->ops->set_name = 
            element_impl_l->element.ops->set_name;
    element->ops->get_ref_qname = 
            element_impl_l->element.ops->get_ref_qname;
    element->ops->set_ref_qname = 
            element_impl_l->element.ops->set_ref_qname;
    element->ops->get_qname = 
            element_impl_l->element.ops->get_qname;
    element->ops->set_qname = 
            element_impl_l->element.ops->set_qname;
    element->ops->get_schema_type = 
            element_impl_l->element.ops->get_schema_type;
    element->ops->set_schema_type = 
            element_impl_l->element.ops->set_schema_type;
    element->ops->get_schema_type_qname = 
            element_impl_l->element.ops->get_schema_type_qname;
    element->ops->set_schema_type_qname = 
            element_impl_l->element.ops->set_schema_type_qname;
    element->ops->get_substitution_group = 
            element_impl_l->element.ops->get_substitution_group;
    element->ops->set_substitution_group = 
            element_impl_l->element.ops->set_substitution_group;
    element->ops->to_string = 
            element_impl_l->element.ops->to_string;
    element->ops->set_type = 
            element_impl_l->element.ops->set_type;
    
    return axis2_xml_schema_type_receiver_resolve_methods(&(element->base), 
            env, element_impl_l->type_recv, methods);
}
*/

/**
 * Returns a collection of constraints on the element.
 */
axis2_xml_schema_obj_collection_t *AXIS2_CALL
axis2_xml_schema_element_get_constraints(void *element,
                                            axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(element)->constraints;
}

axis2_char_t *AXIS2_CALL 
axis2_xml_schema_element_get_default_value(void *element,
                                            axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(element)->default_value;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_default_value(void *element,
                                            axis2_env_t **env,
                                            axis2_char_t *default_value) 
{
   axis2_xml_schema_element_impl_t *element_impl = NULL;
   AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK((*env)->error, default_value, AXIS2_FAILURE);
   element_impl = AXIS2_INTF_TO_IMPL(element);
   if(NULL != element_impl->default_value)
   {
        AXIS2_FREE((*env)->allocator, element_impl->default_value);
        element_impl->default_value = NULL;
   }
   element_impl->default_value = default_value;
   return AXIS2_SUCCESS;
}

axis2_xml_schema_derivation_method_t *AXIS2_CALL
axis2_xml_schema_element_get_block(void *element,
                                    axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(element)->block;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_block(void *element,
                                    axis2_env_t **env,
                                    axis2_xml_schema_derivation_method_t * block)
{
    axis2_xml_schema_element_impl_t *element_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, block, AXIS2_FAILURE);
    
    element_impl = AXIS2_INTF_TO_IMPL(element);
    if(NULL != element_impl->block)
    {
        /** TODO Free block */
    
    }
    element_impl->block = block;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_derivation_method_t *AXIS2_CALL
axis2_xml_schema_element_get_final(void *element,
                                    axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(element)->final_derivation;    
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_element_set_final(
    void *element,
    axis2_env_t **env,
    axis2_xml_schema_derivation_method_t *final_derivation)
{
    axis2_xml_schema_element_impl_t *element_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    element_impl = AXIS2_INTF_TO_IMPL(element);
    if(NULL != element_impl->final_derivation)
    {
        /** TODO Free */
    
    }
    element_impl->final_derivation = final_derivation;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_derivation_method_t *AXIS2_CALL 
axis2_xml_schema_element_get_block_resolved(void *element,
                                            axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(element)->final_derivation_resolved;    
}

axis2_char_t *AXIS2_CALL 
axis2_xml_schema_element_get_fixed_value(void *element,
                                            axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(element)->fixed_value;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_element_set_fixed_value(void *element,
                                            axis2_env_t **env,
                                            axis2_char_t *fixed_value) 
{
    axis2_xml_schema_element_impl_t *element_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    element_impl = AXIS2_INTF_TO_IMPL(element);
    if(NULL != element_impl->fixed_value)
    {
        AXIS2_FREE((*env)->allocator, element_impl->fixed_value);
        element_impl->fixed_value = NULL;
    }
    element_impl->fixed_value = AXIS2_STRDUP(fixed_value, env);
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_xml_schema_element_get_element_type(void *element,
                                            axis2_env_t **env) 
{
    return NULL;
}

axis2_xml_schema_form_t *AXIS2_CALL
axis2_xml_schema_element_get_form(void *element,
                                    axis2_env_t **env) 
{
    axis2_xml_schema_element_impl_t *element_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    element_impl = AXIS2_INTF_TO_IMPL(element);
    if(NULL != element_impl->form)
    {
        AXIS2_XML_SCHEMA_FORM_FREE(element_impl->form, env);
        element_impl->form = NULL;
    }
    return element_impl->form;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_element_set_form(void *element,
                                    axis2_env_t **env,
                                    axis2_xml_schema_form_t *form) 
{
    axis2_xml_schema_element_impl_t *element_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    element_impl = AXIS2_INTF_TO_IMPL(element);
    if(NULL != element_impl->form)
    {
        AXIS2_XML_SCHEMA_FORM_FREE(element_impl->form, env);
        element_impl->block = NULL;
    }
    element_impl->form = element_impl->form;
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_xml_schema_element_is_abstract(void *element,
                                        axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(element)->is_abstract;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_abstract(void *element,
                                        axis2_env_t **env,
                                        axis2_bool_t is_abstract) 
{
    axis2_xml_schema_element_impl_t *element_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    element_impl = AXIS2_INTF_TO_IMPL(element);
    element_impl->is_abstract = is_abstract;
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_xml_schema_element_is_nillable(void *element,
                                        axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(element)->is_nillable;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_nillable(void *element,
                                        axis2_env_t **env,
                                        axis2_bool_t  nillable) 
{
    axis2_xml_schema_element_impl_t *element_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    element_impl = AXIS2_INTF_TO_IMPL(element);
    element_impl->is_nillable = nillable;
    return AXIS2_SUCCESS; 
}

axis2_char_t *AXIS2_CALL 
axis2_xml_schema_element_get_name(void *element,
                                    axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(element)->name;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_name(void *element,
                                    axis2_env_t **env,
                                    axis2_char_t *name) 
{
    axis2_xml_schema_element_impl_t *element_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    element_impl = AXIS2_INTF_TO_IMPL(element);
    if(NULL != element_impl->name)
    {
        AXIS2_FREE((*env)->allocator, element_impl->name);
        element_impl->name = NULL;
    }
    element_impl->name = AXIS2_STRDUP(name, env);
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL 
axis2_xml_schema_element_get_ref_qname(void *element,
                                        axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(element)->ref_qname;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_ref_qname(void *element,
                                        axis2_env_t **env,
                                        axis2_qname_t *ref_qname) 
{
    axis2_xml_schema_element_impl_t *element_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    element_impl = AXIS2_INTF_TO_IMPL(element);
    if(NULL != element_impl->ref_qname)
    {
        AXIS2_QNAME_FREE(element_impl->ref_qname, env);
        element_impl->ref_qname = NULL;
    }
    element_impl->ref_qname = ref_qname;
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL 
axis2_xml_schema_element_get_qname(void *element,
                                    axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(element)->qualified_qname;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_qname(void *element,
                                    axis2_env_t **env,
                                    axis2_qname_t *qualified_name) 
{
    axis2_xml_schema_element_impl_t *element_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    element_impl = AXIS2_INTF_TO_IMPL(element);
    if(NULL != element_impl->qualified_qname)
    {
        AXIS2_QNAME_FREE(element_impl->qualified_qname, env);
        element_impl->qualified_qname = NULL;
    }
    element_impl->qualified_qname = qualified_name;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_type_t *AXIS2_CALL
axis2_xml_schema_element_get_schema_type(void *element,
                                            axis2_env_t **env) 
{
    axis2_xml_schema_element_impl_t *element_impl = NULL;
    element_impl = AXIS2_INTF_TO_IMPL(element);
    return element_impl->schema_type;    
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_schema_type(void *element,
                                        axis2_env_t **env,
                                        axis2_xml_schema_type_t *schema_type) 
{
    axis2_xml_schema_element_impl_t *element_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    element_impl = AXIS2_INTF_TO_IMPL(element);
    if(NULL != element_impl->schema_type)
    {
        /** TODO free */
    }
    element_impl->schema_type = schema_type;
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL 
axis2_xml_schema_element_get_schema_type_qname(void *element,
                                                axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(element)->schema_type_qname;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_schema_type_qname(
        void *element,
        axis2_env_t **env,
        axis2_qname_t *schema_type_qname) 
{
    axis2_xml_schema_element_impl_t *element_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    element_impl = AXIS2_INTF_TO_IMPL(element);
    if(NULL != element_impl->schema_type_qname)
    {
        /** TODO free */
    }
    element_impl->schema_type_qname = schema_type_qname;
    return AXIS2_SUCCESS;       
}

axis2_qname_t *AXIS2_CALL 
axis2_xml_schema_element_get_substitution_group(void *element,
                                                axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(element)->substitution_group;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_substitution_group(void *element,
                                                axis2_env_t **env,
                                                axis2_qname_t *substitution_group) 
{
    axis2_xml_schema_element_impl_t *element_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    element_impl = AXIS2_INTF_TO_IMPL(element);
    if(NULL != element_impl->substitution_group)
    {
        /** TODO free */
    }
    element_impl->substitution_group = substitution_group;
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL 
axis2_xml_schema_element_to_string(void *element,
                                    axis2_env_t **env,
                                    axis2_char_t *prefix, 
                                    int tab) 
{
    /** TODO */
    return NULL;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_element_set_type(void *element,
                                    axis2_env_t **env,
                                    axis2_xml_schema_type_t *type)
{
    axis2_xml_schema_element_impl_t *element_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    element_impl = AXIS2_INTF_TO_IMPL(element);
    if(NULL != element_impl->schema_type)
    {
        /** TODO */
    }
    element_impl->schema_type = type;
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_xml_schema_element_super_objs(void *element,
                                            axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(element)->ht_super;
}

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_element_type(void *element,
                                            axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(element)->obj_type;
}                                             