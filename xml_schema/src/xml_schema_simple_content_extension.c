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

#include <xml_schema_simple_content_extension.h>
#include <axis2_utils.h>

typedef struct xml_schema_simple_content_extension_impl 
                    xml_schema_simple_content_extension_impl_t;

/** 
 * @brief xml_schema_simple_content_extension_impl
 */ 
struct xml_schema_simple_content_extension_impl
{
    xml_schema_simple_content_extension_t sim_content_ext;
    
    xml_schema_annotated_t *annotated;
    
    xml_schema_obj_collection_t *attributes;
    
    xml_schema_any_attribute_t *any_attribute;
    
    axis2_qname_t *base_type_name;
    
    xml_schema_types_t obj_type;
    
    axis2_hash_t *ht_super;
    
    axis2_hash_t *methods;
    
};

#define AXIS2_INTF_TO_IMPL(sim_content_ext) \
        ((xml_schema_simple_content_extension_impl_t *) sim_content_ext)

axis2_status_t AXIS2_CALL 
xml_schema_simple_content_extension_free(
        void *sim_content_ext,
        const axis2_env_t *env);

xml_schema_annotated_t *AXIS2_CALL
xml_schema_simple_content_extension_get_base_impl(
        void *sim_content_ext,
        const axis2_env_t *env);
        
xml_schema_types_t AXIS2_CALL
xml_schema_simple_content_extension_get_type(
        void *sim_content_ext,
        const axis2_env_t *env);
        
axis2_hash_t *AXIS2_CALL
xml_schema_simple_content_extension_super_objs(
        void *sim_content_ext,
        const axis2_env_t *env);
                        

xml_schema_any_attribute_t* AXIS2_CALL
xml_schema_simple_content_extension_get_any_attribute(
        void *sim_content_ext,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_simple_content_extension_set_any_attribute(
        void *sim_content_ext,
        const axis2_env_t *env,
        xml_schema_any_attribute_t *any_attr);

xml_schema_obj_collection_t* AXIS2_CALL
xml_schema_simple_content_extension_get_attributes(
        void *sim_content_ext,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_simple_content_extension_set_base_type_name(
        void *sim_content_ext,
        const axis2_env_t *env,
        axis2_qname_t *base_type_name);
        
                                          
axis2_qname_t* AXIS2_CALL
xml_schema_simple_content_extension_get_base_type_name(
        void *sim_content_ext,
        const axis2_env_t *env);
                                          
/*************************** end function protos **********************************/                
                          
AXIS2_EXTERN xml_schema_simple_content_extension_t * AXIS2_CALL
xml_schema_simple_content_extension_create(const axis2_env_t *env)
{
    xml_schema_simple_content_extension_impl_t *sim_content_ext_impl = NULL;

    sim_content_ext_impl = (xml_schema_simple_content_extension_impl_t*)
        AXIS2_MALLOC(env->allocator, sizeof(xml_schema_simple_content_extension_impl_t));
        
    if(!sim_content_ext_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    sim_content_ext_impl->annotated = NULL;
    sim_content_ext_impl->sim_content_ext.ops = NULL;
    sim_content_ext_impl->sim_content_ext.base.ops = NULL;
    sim_content_ext_impl->attributes = NULL;
    sim_content_ext_impl->base_type_name = NULL;
    sim_content_ext_impl->any_attribute = NULL;
    sim_content_ext_impl->obj_type = XML_SCHEMA_SIMPLE_CONTENT_EXTENSION;
    sim_content_ext_impl->ht_super = NULL;
    
    sim_content_ext_impl->sim_content_ext.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(xml_schema_simple_content_extension_ops_t));
    if(!sim_content_ext_impl->sim_content_ext.ops)
    {
        xml_schema_simple_content_extension_free(
            &(sim_content_ext_impl->sim_content_ext), env);
        AXIS2_ERROR_SET(env->error, 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    sim_content_ext_impl->sim_content_ext.ops->free = 
            xml_schema_simple_content_extension_free;
    sim_content_ext_impl->sim_content_ext.ops->get_base_impl = 
            xml_schema_simple_content_extension_get_base_impl;
    sim_content_ext_impl->sim_content_ext.ops->get_type = 
            xml_schema_simple_content_extension_get_type;            
    sim_content_ext_impl->sim_content_ext.ops->super_objs = 
            xml_schema_simple_content_extension_super_objs;            
            
    sim_content_ext_impl->sim_content_ext.ops->get_any_attribute = 
            xml_schema_simple_content_extension_get_any_attribute;
    sim_content_ext_impl->sim_content_ext.ops->set_any_attribute = 
            xml_schema_simple_content_extension_set_any_attribute;
    sim_content_ext_impl->sim_content_ext.ops->get_attributes = 
            xml_schema_simple_content_extension_get_attributes;
    sim_content_ext_impl->sim_content_ext.ops->get_base_type_name = 
            xml_schema_simple_content_extension_get_base_type_name;
    sim_content_ext_impl->sim_content_ext.ops->set_base_type_name =
            xml_schema_simple_content_extension_set_base_type_name; 
    
    sim_content_ext_impl->ht_super = axis2_hash_make(env);
    sim_content_ext_impl->methods = axis2_hash_make(env);  
    if(!sim_content_ext_impl->methods || !sim_content_ext_impl->ht_super)
    {
        xml_schema_simple_content_extension_free(
                &(sim_content_ext_impl->sim_content_ext), env);
        return NULL;
    }  
    
    axis2_hash_set(sim_content_ext_impl->methods, "free", AXIS2_HASH_KEY_STRING,
            xml_schema_simple_content_extension_free);
     axis2_hash_set(sim_content_ext_impl->methods, "get_type", AXIS2_HASH_KEY_STRING,
            xml_schema_simple_content_extension_get_type);            
      axis2_hash_set(sim_content_ext_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING,
            xml_schema_simple_content_extension_super_objs);            
            
    axis2_hash_set(sim_content_ext_impl->methods, "get_any_attribute", AXIS2_HASH_KEY_STRING,
            xml_schema_simple_content_extension_get_any_attribute);
            
    axis2_hash_set(sim_content_ext_impl->methods, "set_any_attribute", AXIS2_HASH_KEY_STRING,
            xml_schema_simple_content_extension_set_any_attribute);
    
    axis2_hash_set(sim_content_ext_impl->methods, "get_attributes", AXIS2_HASH_KEY_STRING,
            xml_schema_simple_content_extension_get_attributes);
    axis2_hash_set(sim_content_ext_impl->methods, "get_base_type_name", AXIS2_HASH_KEY_STRING,
            xml_schema_simple_content_extension_get_base_type_name);
    axis2_hash_set(sim_content_ext_impl->methods, "set_base_type_name", AXIS2_HASH_KEY_STRING,
            xml_schema_simple_content_extension_set_base_type_name);
    
    sim_content_ext_impl->annotated = xml_schema_annotated_create(env);
    if(!sim_content_ext_impl->annotated)
    {
        xml_schema_simple_content_extension_free(
                &(sim_content_ext_impl->sim_content_ext), env);
        return NULL;
    }
    sim_content_ext_impl->attributes = xml_schema_obj_collection_create(env);
    if(!sim_content_ext_impl->attributes)
    {
        xml_schema_simple_content_extension_free(&(sim_content_ext_impl->sim_content_ext),
             env);
        return NULL;
    }
     axis2_hash_set(sim_content_ext_impl->ht_super, "XML_SCHEMA_SIMPLE_CONTENT_EXTENSION",
        AXIS2_HASH_KEY_STRING,&(sim_content_ext_impl->sim_content_ext));
    axis2_hash_set(sim_content_ext_impl->ht_super, "XML_SCHEMA_ANNOTATED",
        AXIS2_HASH_KEY_STRING,(sim_content_ext_impl->annotated));
    axis2_hash_set(sim_content_ext_impl->ht_super, "XML_SCHEMA_OBJ",
        AXIS2_HASH_KEY_STRING,
        XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(sim_content_ext_impl->annotated, env));
    
    xml_schema_annotated_resolve_methods(&(sim_content_ext_impl->sim_content_ext.base),
        env, sim_content_ext_impl->annotated, sim_content_ext_impl->methods);
    
    return &(sim_content_ext_impl->sim_content_ext);
}

axis2_status_t AXIS2_CALL
xml_schema_simple_content_extension_free(
        void *sim_content_ext_ext,
        const axis2_env_t *env)
{
    xml_schema_simple_content_extension_impl_t *content_ext_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    content_ext_impl = AXIS2_INTF_TO_IMPL(sim_content_ext_ext);
    if(NULL != content_ext_impl->any_attribute)
    {
        XML_SCHEMA_ANY_ATTRIBUTE_FREE(content_ext_impl->any_attribute, env);
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
        XML_SCHEMA_ANNOTATED_FREE(content_ext_impl->annotated, env);
        content_ext_impl->annotated = NULL;
    }
    if(NULL != content_ext_impl->sim_content_ext.base.ops)
    {
        AXIS2_FREE(env->allocator, content_ext_impl->sim_content_ext.base.ops);
        content_ext_impl->sim_content_ext.base.ops = NULL;
    }
    if(NULL != content_ext_impl->sim_content_ext.ops)
    {
        AXIS2_FREE(env->allocator, content_ext_impl->sim_content_ext.ops);
        content_ext_impl->sim_content_ext.ops = NULL;
    }
 
    AXIS2_FREE(env->allocator, content_ext_impl);
    content_ext_impl = NULL;
    return AXIS2_SUCCESS;
}

xml_schema_annotated_t *AXIS2_CALL
xml_schema_simple_content_extension_get_base_impl(
        void *sim_content_ext,
        const axis2_env_t *env)
{
    xml_schema_simple_content_extension_impl_t *content_ext_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    content_ext_impl = AXIS2_INTF_TO_IMPL(sim_content_ext);
    return content_ext_impl->annotated;
}



xml_schema_any_attribute_t* AXIS2_CALL
xml_schema_simple_content_extension_get_any_attribute(
        void *sim_content_ext,
        const axis2_env_t *env)
{
    xml_schema_simple_content_extension_impl_t *cnt_ext_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    cnt_ext_impl = AXIS2_INTF_TO_IMPL(sim_content_ext);
    return cnt_ext_impl->any_attribute;    
}

axis2_status_t AXIS2_CALL
xml_schema_simple_content_extension_set_any_attribute(
        void *sim_content_ext,
        const axis2_env_t *env,
        xml_schema_any_attribute_t *any_attr)
{
    xml_schema_simple_content_extension_impl_t *content_ext_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, any_attr, AXIS2_FAILURE);
    if(NULL != content_ext_impl->any_attribute)
    {
        XML_SCHEMA_ANY_ATTRIBUTE_FREE(content_ext_impl->any_attribute, env);
        content_ext_impl->any_attribute = NULL;
    }
        content_ext_impl->any_attribute = any_attr;
    return AXIS2_SUCCESS;
}

xml_schema_obj_collection_t* AXIS2_CALL
xml_schema_simple_content_extension_get_attributes(
        void *sim_content,
        const axis2_env_t *env)
{
    xml_schema_simple_content_extension_impl_t *cnt_ext_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    cnt_ext_impl = AXIS2_INTF_TO_IMPL(sim_content);
    return cnt_ext_impl->attributes;
}

axis2_qname_t* AXIS2_CALL
xml_schema_simple_content_extension_get_base_type_name(
        void *sim_content,
        const axis2_env_t *env)
{
    xml_schema_simple_content_extension_impl_t *content_ext_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    content_ext_impl = AXIS2_INTF_TO_IMPL(sim_content);
    return content_ext_impl->base_type_name;
}

axis2_status_t AXIS2_CALL
xml_schema_simple_content_extension_set_base_type_name(
        void *sim_content,
        const axis2_env_t *env,
        axis2_qname_t *base_type_name)
{
    xml_schema_simple_content_extension_impl_t *content_ext_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    content_ext_impl = AXIS2_INTF_TO_IMPL(sim_content);
    if(NULL != content_ext_impl->base_type_name)
    {
        AXIS2_QNAME_FREE(content_ext_impl->base_type_name, env);
        content_ext_impl->base_type_name = NULL;
    }
    content_ext_impl->base_type_name = base_type_name;
    return AXIS2_SUCCESS;
}

xml_schema_types_t AXIS2_CALL
xml_schema_simple_content_extension_get_type(
        void *sim_content_ext,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(sim_content_ext)->obj_type;
}        
        
axis2_hash_t *AXIS2_CALL
xml_schema_simple_content_extension_super_objs(
        void *sim_content_ext,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(sim_content_ext)->ht_super;
}        
                                       
