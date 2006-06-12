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
 
#include <axis2_xml_schema.h>
#include <axis2_xml_schema_constants.h>
#include <axis2_xml_schema_complex_type.h>
#include <axis2_xml_schema_data_type.h>
#include <axis2_xml_schema_derivation_method.h>
#include <axis2_qname.h>

typedef struct axis2_xml_schema_complex_type_impl 
                axis2_xml_schema_complex_type_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension  
 */ 
struct axis2_xml_schema_complex_type_impl
{
    axis2_xml_schema_complex_type_t complex_type;

    /** parent type */
    axis2_xml_schema_type_t *schema_type;    
    
    axis2_xml_schema_any_attribute_t *any_attr, *attr_wildcard;
    
    axis2_xml_schema_obj_collection_t *attributes;
    
    axis2_xml_schema_obj_table_t *attr_uses;
    
    axis2_xml_schema_derivation_method_t *block, *block_resolved;
    
    /** type stores the type of model stored in content_model */    
    void *content_model;        
    
    axis2_xml_schema_content_type_t *content_type;
    
    void *particle_type;
    void *particle;
    
    axis2_bool_t is_abstract, is_mixed;
    
    axis2_xml_schema_types_t obj_type;
    
    axis2_hash_t *ht_super;
    
    axis2_hash_t *methods;
    
};

#define AXIS2_INTF_TO_IMPL(complex_type) \
        ((axis2_xml_schema_complex_type_impl_t *) complex_type)

/************************** function prototypes ******************************/

axis2_status_t AXIS2_CALL 
axis2_xml_schema_complex_type_free(
        void *complex_type,
        const axis2_env_t *env);
        

axis2_xml_schema_type_t* AXIS2_CALL
axis2_xml_schema_complex_type_get_base_impl(
        void *complex_type,
        const axis2_env_t *env);
        
axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_complex_type_get_type(
        void *complex_type,
        const axis2_env_t *env);
        
axis2_hash_t* AXIS2_CALL
axis2_xml_schema_complex_type_super_objs(
        void *complex_type,
        const axis2_env_t *env);                
        
axis2_xml_schema_any_attribute_t* AXIS2_CALL 
axis2_xml_schema_complex_type_get_any_attribute(
        void *cmp_type,
        const axis2_env_t *env);
        
axis2_status_t  AXIS2_CALL 
axis2_xml_schema_complex_type_set_any_attribute(
        void *cmp_type,
        const axis2_env_t *env,
        axis2_xml_schema_any_attribute_t *any_attr);
        
axis2_xml_schema_obj_collection_t* AXIS2_CALL
axis2_xml_schema_complex_type_get_attributes(
        void *cmp_type,
        const axis2_env_t *env);
        
axis2_xml_schema_obj_table_t* AXIS2_CALL
axis2_xml_schema_complex_type_get_attribute_use(
        void *cmp_type,
        const axis2_env_t *env);

axis2_xml_schema_any_attribute_t* AXIS2_CALL
axis2_xml_schema_complex_type_get_attribute_wildcard(
        void *cmp_type,
        const axis2_env_t *env);
        
axis2_xml_schema_derivation_method_t* AXIS2_CALL
axis2_xml_schema_complex_type_get_block(
        void *cmp_type,
        const axis2_env_t *env);
        
axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_type_set_block(
        void *cmp_type,
        const axis2_env_t *env,
        axis2_xml_schema_derivation_method_t *block);
        
axis2_xml_schema_derivation_method_t* AXIS2_CALL
axis2_xml_schema_complex_type_get_block_resolved(
        void *cmp_type,
        const axis2_env_t *env);
        
void* AXIS2_CALL
axis2_xml_schema_complex_type_get_content_model(
        void *cmp_type,
        const axis2_env_t *env);
        
axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_type_set_content_model(
        void *cmp_type,
        const axis2_env_t *env,
        void *content_model);
        
axis2_xml_schema_content_type_t* AXIS2_CALL 
axis2_xml_schema_complex_type_get_content_type(
        void *cmp_type,
        const axis2_env_t *env);
        
axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_type_set_content_type(
        void *cmp_type,
        const axis2_env_t *env,
        axis2_xml_schema_content_type_t *content_type);    
        
axis2_xml_schema_particle_t * AXIS2_CALL
axis2_xml_schema_complex_type_get_content_type_particle(
        void *cmp_type,
        const axis2_env_t *env);                    
                                            
axis2_bool_t AXIS2_CALL
axis2_xml_schema_complex_type_is_abstract(
        void *cmp_type,
        const axis2_env_t *env);
        
axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_type_set_abstract(
        void *cmp_type,
        const axis2_env_t *env,
        axis2_bool_t b);
        
axis2_bool_t AXIS2_CALL
axis2_xml_schema_complex_type_is_mixed(
        void *cmp_type,
        const axis2_env_t *env);
        
axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_type_set_mixed(
        void *cmp_type,
        const axis2_env_t *env,
        axis2_bool_t b);
        
void* AXIS2_CALL
axis2_xml_schema_complex_type_get_particle(
        void *cmp_type,
        const axis2_env_t *env);
        
axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_type_set_particle(
        void *cmp_type,
        const axis2_env_t *env,
        void *particle);
                    
axis2_char_t* AXIS2_CALL
axis2_xml_schema_complex_type_to_string(
        void *cmp_type,
        const axis2_env_t *env,
        axis2_char_t *prefix,
        int tab);        
        
                                
/************************** end ***********************************************/                                

AXIS2_EXTERN axis2_xml_schema_complex_type_t * AXIS2_CALL
axis2_xml_schema_complex_type_create(
        const axis2_env_t *env,
        axis2_xml_schema_t *schema)
{
    axis2_xml_schema_complex_type_impl_t *complex_type_impl = NULL;
    axis2_xml_schema_annotated_t *annotated = NULL;

    complex_type_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_complex_type_impl_t));
    if(!complex_type_impl)
    {
        AXIS2_ERROR_SET(env->error,
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    complex_type_impl->any_attr = NULL;
    complex_type_impl->attr_uses = NULL;
    complex_type_impl->attr_wildcard = NULL;
    complex_type_impl->attributes = NULL;
    complex_type_impl->block = NULL;
    complex_type_impl->block_resolved = NULL;
    complex_type_impl->complex_type.ops = NULL;
    complex_type_impl->complex_type.base.ops = NULL;
    complex_type_impl->content_model = NULL;
    complex_type_impl->content_type = NULL;
    complex_type_impl->attr_uses = NULL;
    complex_type_impl->is_mixed = AXIS2_FALSE;
    complex_type_impl->is_abstract = AXIS2_FALSE;
    complex_type_impl->ht_super = NULL;
    complex_type_impl->obj_type = AXIS2_XML_SCHEMA_COMPLEX_TYPE;
    complex_type_impl->methods = NULL;
    
    complex_type_impl->complex_type.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_complex_type_ops_t));
    if(!complex_type_impl->complex_type.ops)
    {
        axis2_xml_schema_complex_type_free(&(complex_type_impl->complex_type), env);
        AXIS2_ERROR_SET(env->error,
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    complex_type_impl->schema_type = axis2_xml_schema_type_create(env, schema);
    if(!complex_type_impl->schema_type)
    {
        axis2_xml_schema_complex_type_free(&(complex_type_impl->complex_type), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    complex_type_impl->attributes = axis2_xml_schema_obj_collection_create(env);
    
    if(!complex_type_impl->attributes)
    {
        axis2_xml_schema_complex_type_free(&(complex_type_impl->complex_type), env);
        return NULL;
    }
    
    complex_type_impl->block = axis2_xml_schema_derivation_method_create(env, "none");
    if(!complex_type_impl->block)
    {
        axis2_xml_schema_complex_type_free(&(complex_type_impl->complex_type), env);
        return NULL;
    }    
    
    complex_type_impl->complex_type.ops->free = 
        axis2_xml_schema_complex_type_free;
    complex_type_impl->complex_type.ops->get_type =
        axis2_xml_schema_complex_type_get_type;
    complex_type_impl->complex_type.ops->super_objs =
        axis2_xml_schema_complex_type_super_objs;            
    complex_type_impl->complex_type.ops->get_base_impl = 
        axis2_xml_schema_complex_type_get_base_impl;
    complex_type_impl->complex_type.ops->get_any_attribute =
        axis2_xml_schema_complex_type_get_any_attribute;
    complex_type_impl->complex_type.ops->set_any_attribute =
        axis2_xml_schema_complex_type_set_any_attribute;
    complex_type_impl->complex_type.ops->get_attributes =
        axis2_xml_schema_complex_type_get_attributes;
    complex_type_impl->complex_type.ops->get_attribute_use =
        axis2_xml_schema_complex_type_get_attribute_use;
    complex_type_impl->complex_type.ops->get_attribute_wildcard =
        axis2_xml_schema_complex_type_get_attribute_wildcard;
    complex_type_impl->complex_type.ops->get_block =
        axis2_xml_schema_complex_type_get_block;
    complex_type_impl->complex_type.ops->set_block =
        axis2_xml_schema_complex_type_set_block;
    complex_type_impl->complex_type.ops->get_block_resolved =
        axis2_xml_schema_complex_type_get_block_resolved;
    complex_type_impl->complex_type.ops->get_content_model =
        axis2_xml_schema_complex_type_get_content_model;
    complex_type_impl->complex_type.ops->set_content_model =
        axis2_xml_schema_complex_type_set_content_model;
    complex_type_impl->complex_type.ops->get_content_type =
        axis2_xml_schema_complex_type_get_content_type;
    complex_type_impl->complex_type.ops->set_content_type =
        axis2_xml_schema_complex_type_set_content_type;                          
    complex_type_impl->complex_type.ops->get_content_type_particle =
        axis2_xml_schema_complex_type_get_content_type_particle;
    complex_type_impl->complex_type.ops->is_abstract =
        axis2_xml_schema_complex_type_is_abstract;
    complex_type_impl->complex_type.ops->set_abstract =
        axis2_xml_schema_complex_type_set_abstract;
    complex_type_impl->complex_type.ops->is_mixed =
        axis2_xml_schema_complex_type_is_mixed;         
    complex_type_impl->complex_type.ops->set_mixed =
        axis2_xml_schema_complex_type_set_mixed;
    complex_type_impl->complex_type.ops->get_particle =
        axis2_xml_schema_complex_type_get_particle;
    complex_type_impl->complex_type.ops->set_particle =
        axis2_xml_schema_complex_type_set_particle;
    complex_type_impl->complex_type.ops->to_string =
        axis2_xml_schema_complex_type_to_string;          
    
    complex_type_impl->methods = axis2_hash_make(env);
    complex_type_impl->ht_super = axis2_hash_make(env);
    
    if(!complex_type_impl->methods || !complex_type_impl->ht_super)
    {
        axis2_xml_schema_complex_type_free(&(complex_type_impl->complex_type), env);    
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(complex_type_impl->methods, "free", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_free);
  axis2_hash_set(complex_type_impl->methods, "get_type", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_get_type);
    axis2_hash_set(complex_type_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_super_objs);            
    axis2_hash_set(complex_type_impl->methods, "get_any_attribute", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_get_any_attribute);
    axis2_hash_set(complex_type_impl->methods, "set_any_attribute", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_set_any_attribute);
    axis2_hash_set(complex_type_impl->methods, "get_attributes", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_get_attributes);
    axis2_hash_set(complex_type_impl->methods, "get_attribute_use", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_get_attribute_use);
    axis2_hash_set(complex_type_impl->methods, "get_attribute_wildcare", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_get_attribute_wildcard);
    axis2_hash_set(complex_type_impl->methods, "get_block", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_get_block);
    axis2_hash_set(complex_type_impl->methods, "set_block", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_set_block);
    axis2_hash_set(complex_type_impl->methods, "get_block_resolved", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_get_block_resolved);
    axis2_hash_set(complex_type_impl->methods, "get_content_model", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_get_content_model);
    axis2_hash_set(complex_type_impl->methods, "set_content_model", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_set_content_model);
    axis2_hash_set(complex_type_impl->methods, "get_content_type", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_get_content_type);
    axis2_hash_set(complex_type_impl->methods, "set_content_type", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_set_content_type);                          
    axis2_hash_set(complex_type_impl->methods, "get_content_type_particle",
        AXIS2_HASH_KEY_STRING, axis2_xml_schema_complex_type_get_content_type_particle);
    axis2_hash_set(complex_type_impl->methods, "is_abstract", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_is_abstract);
    axis2_hash_set(complex_type_impl->methods, "set_abstract", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_set_abstract);
    axis2_hash_set(complex_type_impl->methods, "is_mixed", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_is_mixed);         
    axis2_hash_set(complex_type_impl->methods, "set_mixed", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_set_mixed);
    axis2_hash_set(complex_type_impl->methods, "get_particle", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_get_particle);
    axis2_hash_set(complex_type_impl->methods, "set_particle", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_set_particle);
    axis2_hash_set(complex_type_impl->methods, "to_string", AXIS2_HASH_KEY_STRING,
        axis2_xml_schema_complex_type_to_string);
                
    axis2_hash_set(complex_type_impl->ht_super, "AXIS2_XML_SCHEMA_COMPLEX_TYPE",
        AXIS2_HASH_KEY_STRING, &(complex_type_impl->complex_type));
    axis2_hash_set(complex_type_impl->ht_super, "AXIS2_XML_SCHEMA_TYPE",
        AXIS2_HASH_KEY_STRING, complex_type_impl->schema_type);
        
    annotated = 
        AXIS2_XML_SCHEMA_TYPE_GET_BASE_IMPL(complex_type_impl->schema_type, env);         
    if(NULL != annotated)
    {
        axis2_hash_set(complex_type_impl->ht_super, "AXIS2_XML_SCHEMA_ANNOTATED",
           AXIS2_HASH_KEY_STRING, annotated);
        axis2_hash_set(complex_type_impl->ht_super, "AXIS2_XML_SCHEMA_OBJ",
           AXIS2_HASH_KEY_STRING, 
           AXIS2_XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(annotated, env));
    }        
        
    axis2_hash_set(complex_type_impl->ht_super, "AXIS2_XML_SCHEMA_COMPLEX_TYPE",
        AXIS2_HASH_KEY_STRING, &(complex_type_impl->complex_type));
                
    axis2_xml_schema_type_resolve_methods(&(complex_type_impl->complex_type.base), env,
        complex_type_impl->schema_type, complex_type_impl->methods);
            
    return &(complex_type_impl->complex_type);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_type_free(void *complex_type,
                                    const axis2_env_t *env)
{
    axis2_xml_schema_complex_type_impl_t *complex_type_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    complex_type_impl = AXIS2_INTF_TO_IMPL(complex_type);

    if(NULL != complex_type_impl->schema_type)
    {
        AXIS2_XML_SCHEMA_TYPE_FREE(complex_type_impl->schema_type, env);
        complex_type_impl->schema_type = NULL;
    }
    if(NULL != complex_type_impl->complex_type.base.ops)
    {
        AXIS2_FREE(env->allocator, complex_type_impl->complex_type.base.ops);
        complex_type_impl->complex_type.base.ops = NULL;        
    }
    
    if(NULL != complex_type_impl->complex_type.ops)
    {
        AXIS2_FREE(env->allocator, complex_type_impl->complex_type.ops);
        complex_type_impl->complex_type.ops = NULL;
    }
    AXIS2_FREE(env->allocator, complex_type_impl);
    complex_type_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_type_t *AXIS2_CALL
axis2_xml_schema_complex_type_get_base_impl(
        void *complex_type,
        const axis2_env_t *env)
{
    axis2_xml_schema_complex_type_impl_t *complex_type_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    complex_type_impl = AXIS2_INTF_TO_IMPL(complex_type);
    return complex_type_impl->schema_type;
    
}

axis2_xml_schema_any_attribute_t* AXIS2_CALL 
axis2_xml_schema_complex_type_get_any_attribute(
        void *cmp_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(cmp_type)->any_attr; 
}
        
axis2_status_t  AXIS2_CALL 
axis2_xml_schema_complex_type_set_any_attribute(
        void *cmp_type,
        const axis2_env_t *env,
        axis2_xml_schema_any_attribute_t *any_attr)
{
    axis2_xml_schema_complex_type_impl_t *cmp_type_impl = NULL;

    cmp_type_impl = AXIS2_INTF_TO_IMPL(cmp_type);
    if(NULL != cmp_type_impl->any_attr)
    {
        /** TODO free attribute */
    }
    cmp_type_impl->any_attr= any_attr;
    return AXIS2_SUCCESS;
}
        
axis2_xml_schema_obj_collection_t* AXIS2_CALL
axis2_xml_schema_complex_type_get_attributes(
        void *cmp_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(cmp_type)->attributes;
}
        
axis2_xml_schema_obj_table_t* AXIS2_CALL
axis2_xml_schema_complex_type_get_attribute_use(
        void *cmp_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(cmp_type)->attr_uses;
}

axis2_xml_schema_any_attribute_t* AXIS2_CALL
axis2_xml_schema_complex_type_get_attribute_wildcard(
        void *cmp_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(cmp_type)->attr_wildcard;
}
        
axis2_xml_schema_derivation_method_t* AXIS2_CALL
axis2_xml_schema_complex_type_get_block(
        void *cmp_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(cmp_type)->block;
}
        
axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_type_set_block(
        void *cmp_type,
        const axis2_env_t *env,
        axis2_xml_schema_derivation_method_t *block)
{
    axis2_xml_schema_complex_type_impl_t *cmp_type_impl = NULL;
    cmp_type_impl = AXIS2_INTF_TO_IMPL(cmp_type);
    if(NULL != cmp_type_impl->block)
    {
        /** TODO free block */
    }
    
    cmp_type_impl->block = block;
    return AXIS2_SUCCESS;
}
        
axis2_xml_schema_derivation_method_t* AXIS2_CALL
axis2_xml_schema_complex_type_get_block_resolved(
        void *cmp_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(cmp_type)->block_resolved;

}
        
void* AXIS2_CALL
axis2_xml_schema_complex_type_get_content_model(
        void *cmp_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(cmp_type)->content_model;

}
        
axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_type_set_content_model(
        void *cmp_type,
        const axis2_env_t *env,
        void *content_model)
{
    axis2_xml_schema_complex_type_impl_t *cmp_type_impl = NULL;
    cmp_type_impl = AXIS2_INTF_TO_IMPL(cmp_type);
    
    if(NULL != cmp_type_impl->content_model)
    {
        /** TODO free content_model_type */
    }
    AXIS2_INTF_TO_IMPL(cmp_type)->content_model = content_model;
    return AXIS2_SUCCESS;
}
        
axis2_xml_schema_content_type_t* AXIS2_CALL 
axis2_xml_schema_complex_type_get_content_type(
        void *cmp_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(cmp_type)->content_type;
}
        
axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_type_set_content_type(
        void *cmp_type,
        const axis2_env_t *env,
        axis2_xml_schema_content_type_t *content_type)
{
    axis2_xml_schema_complex_type_impl_t *cmp_type_impl = NULL;
    
    cmp_type_impl = AXIS2_INTF_TO_IMPL(cmp_type);
    if(NULL != cmp_type_impl->content_type)
    {
        /** TODO free */
    }
    cmp_type_impl->content_type = content_type;
    return AXIS2_SUCCESS;
}    
        
axis2_xml_schema_particle_t * AXIS2_CALL
axis2_xml_schema_complex_type_get_content_type_particle(
        void *cmp_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(cmp_type)->particle_type;
}                    
                                            
axis2_bool_t AXIS2_CALL
axis2_xml_schema_complex_type_is_abstract(
        void *cmp_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(cmp_type)->is_abstract;  
}
        
axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_type_set_abstract(
        void *cmp_type,
        const axis2_env_t *env,
        axis2_bool_t b)
{
    AXIS2_INTF_TO_IMPL(cmp_type)->is_abstract = b;
    return AXIS2_SUCCESS;
}
        
axis2_bool_t AXIS2_CALL
axis2_xml_schema_complex_type_is_mixed(
        void *cmp_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(cmp_type)->is_mixed;
}
        
axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_type_set_mixed(
        void *cmp_type,
        const axis2_env_t *env,
        axis2_bool_t b)
{
    AXIS2_INTF_TO_IMPL(cmp_type)->is_mixed = b;
    return AXIS2_SUCCESS;
}
        
void* AXIS2_CALL
axis2_xml_schema_complex_type_get_particle(
        void *cmp_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(cmp_type)->particle;
}
        
axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_type_set_particle(
        void *cmp_type,
        const axis2_env_t *env,
        void *particle)
{
    axis2_xml_schema_complex_type_impl_t *cmp_type_impl = NULL;
    cmp_type_impl = AXIS2_INTF_TO_IMPL(cmp_type);
    if(NULL != cmp_type_impl->particle)
    {
        /** TODO free particle */
    }
    cmp_type_impl->particle = particle;
    return AXIS2_SUCCESS;
}
                    


axis2_char_t* AXIS2_CALL
axis2_xml_schema_complex_type_to_string(
        void *complex_type,
        const axis2_env_t *env,
        axis2_char_t *prefix,
        int tab)
{
    axis2_xml_schema_complex_type_impl_t *complex_type_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    complex_type_impl = AXIS2_INTF_TO_IMPL(complex_type);
    /** TODO implement the method */
    return NULL;
}

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_complex_type_get_type(
        void *complex_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(complex_type)->obj_type;
}        
        
axis2_hash_t* AXIS2_CALL
axis2_xml_schema_complex_type_super_objs(
        void *complex_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(complex_type)->ht_super;
}        
