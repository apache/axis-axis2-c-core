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

#include <xml_schema/axis2_xml_schema_complex_content_extension.h>
#include <axis2_utils.h>

typedef struct axis2_xml_schema_complex_content_extension_impl 
                    axis2_xml_schema_complex_content_extension_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_complex_content_extension_impl
{
    axis2_xml_schema_complex_content_extension_t cmp_cnt_ext;
    
    axis2_xml_schema_annotated_t *annotated;
    
    axis2_xml_schema_obj_collection_t *attributes;
    
    axis2_xml_schema_any_attribute_t *any_attribute;
    
    axis2_qname_t *base_type_name;
    
    axis2_xml_schema_particle_t *particle;
    
    axis2_hash_t *methods;
    
    axis2_hash_t *ht_super;
    
    axis2_xml_schema_types_t obj_type;
};

#define AXIS2_INTF_TO_IMPL(cmp_cnt_ext) \
        ((axis2_xml_schema_complex_content_extension_impl_t *) cmp_cnt_ext)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_complex_content_extension_free(
        void *cmp_cnt_ext,
        axis2_env_t **env);

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_complex_content_extension_get_base_impl(
        void *cmp_cnt_ext,
        axis2_env_t **env);
        
axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_complex_content_extension_type(
        void *cmp_cnt_ext,
        axis2_env_t **env);
        
axis2_hash_t *AXIS2_CALL
axis2_xml_schema_complex_content_extension_super_objs(
        void *cmp_cnt_ext,
        axis2_env_t **env);                

axis2_xml_schema_any_attribute_t* AXIS2_CALL
axis2_xml_schema_complex_content_extension_get_any_attribute(
        void *cmp_cnt_ext,
        axis2_env_t **env);
        
axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_content_extension_set_any_attribute(
        void *cmp_cnt_ext,
        axis2_env_t **env,
        axis2_xml_schema_any_attribute_t *any_attr);

axis2_xml_schema_obj_collection_t* AXIS2_CALL
axis2_xml_schema_complex_content_extension_get_attributes(
        void *cmp_cnt_ext,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_content_extension_set_base_type_name(
        void *cmp_cnt_ext,
        axis2_env_t **env,
        axis2_qname_t *base_type_name);
                                          
axis2_qname_t* AXIS2_CALL
axis2_xml_schema_complex_content_extension_get_base_type_name(
        void *cmp_cnt_ext,
        axis2_env_t **env);
        
                                          
axis2_xml_schema_particle_t* AXIS2_CALL
axis2_xml_schema_complex_content_extension_get_particle( 
        void *cmp_cnt_ext,
        axis2_env_t **env);                                       
                              
axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_content_extension_set_particle( 
        void *cmp_cnt_ext,
        axis2_env_t **env,
        axis2_xml_schema_particle_t *particle);
        
axis2_char_t *AXIS2_CALL
axis2_xml_schema_complex_content_extension_to_string(
        void *cmp_cnt_ext,
        axis2_env_t **env,
        axis2_char_t *prefix,
        int tab);                                      
                                          
/*************************** end function protos **********************************/                                          
AXIS2_DECLARE(axis2_xml_schema_complex_content_extension_t *)
axis2_xml_schema_complex_content_extension_create(axis2_env_t **env)
{
    axis2_xml_schema_complex_content_extension_impl_t *cmp_cnt_ext_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    cmp_cnt_ext_impl = (axis2_xml_schema_complex_content_extension_impl_t*)
        AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_xml_schema_complex_content_extension_impl_t));
        
    if(!cmp_cnt_ext_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    cmp_cnt_ext_impl->annotated = NULL;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops = NULL;
    cmp_cnt_ext_impl->attributes = NULL;
    cmp_cnt_ext_impl->base_type_name = NULL;
    cmp_cnt_ext_impl->any_attribute = NULL;
    cmp_cnt_ext_impl->particle = NULL;
    cmp_cnt_ext_impl->ht_super = NULL;
    cmp_cnt_ext_impl->methods = NULL;
    cmp_cnt_ext_impl->obj_type = AXIS2_XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION;
    
    cmp_cnt_ext_impl->cmp_cnt_ext.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_complex_content_extension_ops_t));
                    
    if(!cmp_cnt_ext_impl->cmp_cnt_ext.ops)
    {
        axis2_xml_schema_complex_content_extension_free(
            &(cmp_cnt_ext_impl->cmp_cnt_ext), env);
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    cmp_cnt_ext_impl->cmp_cnt_ext.ops->free = 
            axis2_xml_schema_complex_content_extension_free;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->type = 
            axis2_xml_schema_complex_content_extension_type;            
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->super_objs = 
            axis2_xml_schema_complex_content_extension_super_objs;            
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->get_base_impl = 
            axis2_xml_schema_complex_content_extension_get_base_impl;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->get_any_attribute = 
            axis2_xml_schema_complex_content_extension_get_any_attribute;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->set_any_attribute = 
            axis2_xml_schema_complex_content_extension_set_any_attribute;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->get_attributes = 
            axis2_xml_schema_complex_content_extension_get_attributes;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->get_base_type_name = 
            axis2_xml_schema_complex_content_extension_get_base_type_name;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->set_base_type_name =
            axis2_xml_schema_complex_content_extension_set_base_type_name; 
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->get_particle =
            axis2_xml_schema_complex_content_extension_get_particle;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->set_particle =
            axis2_xml_schema_complex_content_extension_set_particle;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->to_string =
            axis2_xml_schema_complex_content_extension_to_string;            

    cmp_cnt_ext_impl->methods = axis2_hash_make(env);
    cmp_cnt_ext_impl->ht_super = axis2_hash_make(env);
    if(!cmp_cnt_ext_impl->ht_super || !cmp_cnt_ext_impl->methods)
    {
        axis2_xml_schema_complex_content_extension_free(
                &(cmp_cnt_ext_impl->cmp_cnt_ext), env);
        return NULL;
    }

    axis2_hash_set(cmp_cnt_ext_impl->methods,"free", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_complex_content_extension_free);

    axis2_hash_set(cmp_cnt_ext_impl->methods,"type", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_complex_content_extension_type);            
    
    axis2_hash_set(cmp_cnt_ext_impl->methods,"super_objs", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_complex_content_extension_super_objs);            
    
    axis2_hash_set(cmp_cnt_ext_impl->methods,"get_any_attribute", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_complex_content_extension_get_any_attribute);
    axis2_hash_set(cmp_cnt_ext_impl->methods,"set_any_attribute", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_complex_content_extension_set_any_attribute);
    axis2_hash_set(cmp_cnt_ext_impl->methods,"get_attributes", AXIS2_HASH_KEY_STRING,                
            axis2_xml_schema_complex_content_extension_get_attributes);
    axis2_hash_set(cmp_cnt_ext_impl->methods,"get_base_type_name", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_complex_content_extension_get_base_type_name);
    axis2_hash_set(cmp_cnt_ext_impl->methods,"set_base_type_name", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_complex_content_extension_set_base_type_name); 
    axis2_hash_set(cmp_cnt_ext_impl->methods,"get_particle", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_complex_content_extension_get_particle);
    axis2_hash_set(cmp_cnt_ext_impl->methods,"set_particle", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_complex_content_extension_set_particle);
    axis2_hash_set(cmp_cnt_ext_impl->methods,"free", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_complex_content_extension_to_string);            
                
    if(!cmp_cnt_ext_impl->annotated)
    {
        axis2_xml_schema_complex_content_extension_free(
                &(cmp_cnt_ext_impl->cmp_cnt_ext), env);
        return NULL;
    }
    cmp_cnt_ext_impl->attributes = axis2_xml_schema_obj_collection_create(env);
    if(!cmp_cnt_ext_impl->attributes)
    {
        axis2_xml_schema_complex_content_extension_free(&(cmp_cnt_ext_impl->cmp_cnt_ext), env);
        return NULL;
    }
    axis2_hash_set(cmp_cnt_ext_impl->methods,"AXIS2_XML_SCHEMA_COMPLEX_CONTENT_EXTENSION",
        AXIS2_HASH_KEY_STRING, &(cmp_cnt_ext_impl->cmp_cnt_ext));   
    axis2_hash_set(cmp_cnt_ext_impl->methods,"AXIS2_XML_SCHEMA_ANNOTATED",
        AXIS2_HASH_KEY_STRING, cmp_cnt_ext_impl->annotated);   
    axis2_hash_set(cmp_cnt_ext_impl->methods,"AXIS2_XML_SCHEMA_OBJ",
        AXIS2_HASH_KEY_STRING, 
        AXIS2_XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(cmp_cnt_ext_impl->annotated, env));   
            
    axis2_xml_schema_annotated_resolve_methods(&(cmp_cnt_ext_impl->cmp_cnt_ext.base), env, 
        cmp_cnt_ext_impl->annotated, cmp_cnt_ext_impl->methods);
    return &(cmp_cnt_ext_impl->cmp_cnt_ext);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_content_extension_free(void *cmp_cnt_ext_ext_ext,
                                    axis2_env_t **env)
{
    axis2_xml_schema_complex_content_extension_impl_t *content_ext_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    content_ext_impl = AXIS2_INTF_TO_IMPL(cmp_cnt_ext_ext_ext);
    if(NULL != content_ext_impl->any_attribute)
    {
        AXIS2_XML_SCHEMA_ANY_ATTRIBUTE_FREE(content_ext_impl->any_attribute, env);
        content_ext_impl->any_attribute = NULL;
    }
    if(NULL != content_ext_impl->attributes)
    {
        /** TODO free attributes */
    }    
    if(NULL != content_ext_impl->base_type_name)
    {
        AXIS2_QNAME_FREE(content_ext_impl->base_type_name, env);
        content_ext_impl->base_type_name = NULL;
    }    
    if(NULL != content_ext_impl->annotated)
    {
        AXIS2_XML_SCHEMA_ANNOTATED_FREE(content_ext_impl->annotated, env);
        content_ext_impl->annotated = NULL;
    }
    if(NULL != content_ext_impl->cmp_cnt_ext.ops)
    {
        AXIS2_FREE((*env)->allocator, content_ext_impl->cmp_cnt_ext.ops);
        content_ext_impl->cmp_cnt_ext.ops = NULL;
    }
    if(NULL != content_ext_impl->cmp_cnt_ext.base.ops)
    {
        AXIS2_FREE((*env)->allocator, content_ext_impl->cmp_cnt_ext.base.ops);
        content_ext_impl->cmp_cnt_ext.base.ops = NULL;
    }
 
    AXIS2_FREE((*env)->allocator, content_ext_impl);
    content_ext_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_complex_content_extension_get_base_impl(void *cmp_cnt_ext,
                                axis2_env_t **env)
{
    axis2_xml_schema_complex_content_extension_impl_t *content_ext_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    content_ext_impl = AXIS2_INTF_TO_IMPL(cmp_cnt_ext);
    return content_ext_impl->annotated;
}



axis2_xml_schema_any_attribute_t* AXIS2_CALL
axis2_xml_schema_complex_content_extension_get_any_attribute(void *cmp_cnt_ext,
                                                            axis2_env_t **env)
{
    axis2_xml_schema_complex_content_extension_impl_t *cnt_ext_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    cnt_ext_impl = AXIS2_INTF_TO_IMPL(cmp_cnt_ext);
    return cnt_ext_impl->any_attribute;    
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_content_extension_set_any_attribute(void *cmp_cnt_ext,
                                axis2_env_t **env,
                                axis2_xml_schema_any_attribute_t *any_attr)
{
    axis2_xml_schema_complex_content_extension_impl_t *content_ext_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, any_attr, AXIS2_FAILURE);
    if(NULL != content_ext_impl->any_attribute)
    {
        AXIS2_XML_SCHEMA_ANY_ATTRIBUTE_FREE(content_ext_impl->any_attribute, env);
        content_ext_impl->any_attribute = NULL;
    }
        content_ext_impl->any_attribute = any_attr;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_obj_collection_t* AXIS2_CALL
axis2_xml_schema_complex_content_extension_get_attributes(void *cmp_cnt_ext,
                                                        axis2_env_t **env)
{
    axis2_xml_schema_complex_content_extension_impl_t *cnt_ext_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    cnt_ext_impl = AXIS2_INTF_TO_IMPL(cmp_cnt_ext);
    return cnt_ext_impl->attributes;
}

axis2_qname_t* AXIS2_CALL
axis2_xml_schema_complex_content_extension_get_base_type_name(
                                          void *cmp_cnt_ext,
                                          axis2_env_t **env)
{
    axis2_xml_schema_complex_content_extension_impl_t *content_ext_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    content_ext_impl = AXIS2_INTF_TO_IMPL(cmp_cnt_ext);
    return content_ext_impl->base_type_name;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_content_extension_set_base_type_name(
                                          void *cmp_cnt_ext,
                                          axis2_env_t **env,
                                          axis2_qname_t *base_type_name)
{
    axis2_xml_schema_complex_content_extension_impl_t *content_ext_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    content_ext_impl = AXIS2_INTF_TO_IMPL(cmp_cnt_ext);
    if(NULL != content_ext_impl->base_type_name)
    {
        AXIS2_QNAME_FREE(content_ext_impl->base_type_name, env);
        content_ext_impl->base_type_name = NULL;
    }
    content_ext_impl->base_type_name = base_type_name;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_particle_t* AXIS2_CALL
axis2_xml_schema_complex_content_extension_get_particle( 
        void *cmp_cnt_ext,
        axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(cmp_cnt_ext)->particle;
}                                             
                              
axis2_status_t AXIS2_CALL
axis2_xml_schema_complex_content_extension_set_particle( 
        void *cmp_cnt_ext,
        axis2_env_t **env,
        axis2_xml_schema_particle_t *particle)
{
    axis2_xml_schema_complex_content_extension_impl_t *cmp_cnt_ext_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, particle, AXIS2_FAILURE);
    cmp_cnt_ext_impl = AXIS2_INTF_TO_IMPL(cmp_cnt_ext);
    if(NULL != cmp_cnt_ext_impl->particle)
    {
        AXIS2_XML_SCHEMA_PARTICLE_FREE(cmp_cnt_ext_impl->particle, env);
        cmp_cnt_ext_impl->particle = NULL;
    }
    cmp_cnt_ext_impl->particle = particle;
    return AXIS2_SUCCESS;
}        
        
axis2_char_t *AXIS2_CALL
axis2_xml_schema_complex_content_extension_to_string(
        void *cmp_cnt_ext,
        axis2_env_t **env,
        axis2_char_t *prefix,
        int tab)
{
    return NULL;
} 

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_complex_content_extension_type(
        void *cmp_cnt_ext,
        axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(cmp_cnt_ext)->obj_type;
}        
        
axis2_hash_t *AXIS2_CALL
axis2_xml_schema_complex_content_extension_super_objs(
        void *cmp_cnt_ext,
        axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(cmp_cnt_ext)->ht_super;
}        