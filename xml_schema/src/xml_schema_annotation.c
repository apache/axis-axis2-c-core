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

#include <xml_schema_annotation.h>
#include <axis2_utils.h>

typedef struct xml_schema_annotation_impl 
                xml_schema_annotation_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension  
 */ 
struct xml_schema_annotation_impl
{
    xml_schema_annotation_t annotation;
    
    xml_schema_types_t obj_type;
    
    axis2_hash_t *ht_super;
    
    xml_schema_obj_t *schema_obj;
    
    xml_schema_obj_collection_t *items;
};


#define AXIS2_INTF_TO_IMPL(annotation) \
        ((xml_schema_annotation_impl_t *) annotation)

/*************** function prototypes *****************************************/

axis2_status_t AXIS2_CALL 
xml_schema_annotation_free(
        void *annotation,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
xml_schema_annotation_super_objs(
        void *annotation,
        const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL 
xml_schema_annotation_get_type(
        void *annotation,
        const axis2_env_t *env);

xml_schema_obj_t *AXIS2_CALL
xml_schema_annotation_get_base_impl(void *annotation,
                                const axis2_env_t *env);

xml_schema_obj_collection_t * AXIS2_CALL
xml_schema_annotation_get_items(void *annotation,
                        const axis2_env_t *env);

/********************** end **************************************************/

AXIS2_EXTERN xml_schema_annotation_t * AXIS2_CALL
xml_schema_annotation_create(const axis2_env_t *env)
{
    xml_schema_annotation_impl_t *annotation_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    annotation_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(xml_schema_annotation_impl_t));
    if(!annotation_impl)
    {
        AXIS2_ERROR_SET(env->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    annotation_impl->schema_obj = NULL;
    annotation_impl->obj_type = XML_SCHEMA_ANNOTATION;
    annotation_impl->ht_super = NULL;
    annotation_impl->items = NULL;
    annotation_impl->annotation.base.ops = NULL;
    annotation_impl->annotation.ops      = NULL;
    
    annotation_impl->annotation.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(xml_schema_annotation_ops_t));
     if(!annotation_impl->annotation.ops)
    {
        xml_schema_annotation_free(&(annotation_impl->annotation), env);
        AXIS2_ERROR_SET(env->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    annotation_impl->annotation.ops->free = 
        xml_schema_annotation_free;
        
    annotation_impl->annotation.ops->super_objs = 
        xml_schema_annotation_super_objs;
    
    annotation_impl->annotation.ops->get_type = 
        xml_schema_annotation_get_type;
    
    annotation_impl->annotation.ops->get_base_impl = 
        xml_schema_annotation_get_base_impl;
    
    annotation_impl->annotation.ops->get_items = 
        xml_schema_annotation_get_items;
   
    annotation_impl->items = xml_schema_obj_collection_create(env);
    if(!annotation_impl->items)
    {
        xml_schema_annotation_free(&(annotation_impl->annotation), env);
        AXIS2_ERROR_SET(env->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    } 
       
    annotation_impl->schema_obj = xml_schema_obj_create(env);
    
    if(!annotation_impl->schema_obj) 
    {
        xml_schema_annotation_free(&(annotation_impl->annotation), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    annotation_impl->ht_super = axis2_hash_make(env);
    if(!annotation_impl->ht_super) 
    {
        xml_schema_annotation_free(&(annotation_impl->annotation), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(annotation_impl->ht_super, 
        AXIS2_STRDUP("XML_SCHEMA_ANNOTATION", env), 
        AXIS2_HASH_KEY_STRING, &(annotation_impl->annotation));
    
    axis2_hash_set(annotation_impl->ht_super, 
        AXIS2_STRDUP("XML_SCHEMA_OBJ", env), 
        AXIS2_HASH_KEY_STRING, annotation_impl->schema_obj);
    
    status =  xml_schema_obj_resolve_methods(&(annotation_impl->annotation.base), 
            env, annotation_impl->schema_obj, 
            xml_schema_annotation_super_objs,
            xml_schema_annotation_get_type,
            xml_schema_annotation_free);
    return &(annotation_impl->annotation);
}


axis2_status_t AXIS2_CALL
xml_schema_annotation_free(void *annotation,
                                 const axis2_env_t *env)
{
    xml_schema_annotation_impl_t *annotation_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    annotation_impl = AXIS2_INTF_TO_IMPL(annotation);

    if(NULL != annotation_impl->items)
    {
        XML_SCHEMA_OBJ_COLLECTION_FREE(annotation_impl->items, env);
        annotation_impl->items = NULL;
    }
    
    if(NULL != annotation_impl->ht_super)
    {
        axis2_hash_free(annotation_impl->ht_super, env);
        annotation_impl->ht_super = NULL;
    }
    
    if(NULL != annotation_impl->schema_obj)
    {
        XML_SCHEMA_OBJ_FREE(annotation_impl->schema_obj, env);
        annotation_impl->schema_obj = NULL;
    }
    
    if(NULL != annotation_impl->annotation.ops)
    {
        AXIS2_FREE(env->allocator, annotation_impl->annotation.ops);
        annotation_impl->annotation.ops = NULL;
    }
    if(NULL != annotation_impl->annotation.base.ops)
    {
        AXIS2_FREE(env->allocator, annotation_impl->annotation.base.ops);
        annotation_impl->annotation.base.ops = NULL;
    }
    if(annotation_impl)
    {
        AXIS2_FREE(env->allocator, annotation_impl);
        annotation_impl = NULL;
    }
    return AXIS2_SUCCESS;
}


xml_schema_obj_t *AXIS2_CALL
xml_schema_annotation_get_base_impl(void *annotation,
                                const axis2_env_t *env)
{
    xml_schema_annotation_impl_t *annotation_impl = NULL;
    annotation_impl = AXIS2_INTF_TO_IMPL(annotation);
    return annotation_impl->schema_obj;
}

xml_schema_obj_collection_t *AXIS2_CALL
xml_schema_annotation_get_items(void *annotation,
                                        const axis2_env_t *env)
{
    /*
    xml_schema_annotation_impl_t *annotation_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    
    ht_super = XML_SCHEMA_ANNOTATION_SUPER_OBJS(annotation, env);
    if(NULL != ht_super)
    {
        annotation_impl = (xml_schema_annotation_impl_t*)
            axis2_hash_get(ht_super , "XML_SCHEMA_ANNOTATION", 
                AXIS2_HASH_KEY_STRING);
        return annotation_impl->items;
    }
    */
    return AXIS2_INTF_TO_IMPL(annotation)->items;
}

axis2_hash_t *AXIS2_CALL 
xml_schema_annotation_super_objs(
        void *annotation,
        const axis2_env_t *env)
{
    xml_schema_annotation_impl_t *annotation_impl = NULL;
    annotation_impl = AXIS2_INTF_TO_IMPL(annotation);
    return annotation_impl->ht_super;
}

xml_schema_types_t AXIS2_CALL 
xml_schema_annotation_get_type(
        void *annotation,
        const axis2_env_t *env)
{
    xml_schema_annotation_impl_t *annotation_impl = NULL;    
    annotation_impl = AXIS2_INTF_TO_IMPL(annotation);
    return annotation_impl->obj_type;
}
