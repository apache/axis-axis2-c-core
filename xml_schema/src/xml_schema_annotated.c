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

#include <xml_schema/axis2_xml_schema_annotated.h>
#include <xml_schema/axis2_xml_schema_annotation.h>
#include <axis2_utils.h>

typedef struct axis2_xml_schema_annotated_impl 
                    axis2_xml_schema_annotated_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_annotated_impl
{
    axis2_xml_schema_annotated_t annotated;
    
    axis2_xml_schema_obj_t *schema_obj;
   
    axis2_xml_schema_types_t obj_type;

    axis2_hash_t *ht_super;

    axis2_hash_t *methods;
    
    axis2_xml_schema_annotation_t *annotation;
    
    axis2_char_t *id;
    
    axis2_array_list_t *unhandled_attrs;
};

#define AXIS2_INTF_TO_IMPL(annotated) \
        ((axis2_xml_schema_annotated_impl_t *) annotated)

/*************** function prototypes *****************************************/

axis2_status_t AXIS2_CALL 
axis2_xml_schema_annotated_free(
        void *annotated,
        axis2_env_t **env);

axis2_hash_t *AXIS2_CALL 
axis2_xml_schema_annotated_super_objs(
        void *annotated,
        axis2_env_t **env);

axis2_xml_schema_types_t AXIS2_CALL 
axis2_xml_schema_annotated_type(
        void *annotated,
        axis2_env_t **env);

axis2_xml_schema_obj_t *AXIS2_CALL
axis2_xml_schema_annotated_get_base_impl(void *annotated,
                                            axis2_env_t **env);

axis2_char_t *AXIS2_CALL
axis2_xml_schema_annotated_get_id(void *annotated,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_annotated_set_id(void *annotated,
                                            axis2_env_t **env,
                                            axis2_char_t *id);

axis2_xml_schema_annotation_t *AXIS2_CALL
axis2_xml_schema_annotated_get_annotation(void *annotated,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_annotated_set_annotation(void *annotated,
                                            axis2_env_t **env,
                                            axis2_xml_schema_annotation_t *
                                                annotation);

axis2_array_list_t *AXIS2_CALL
axis2_xml_schema_annotated_get_unhandled_attrs(void *annotated,
                                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_annotated_set_unhandled_attrs(void *annotated,
                                                axis2_env_t **env,
                                                axis2_array_list_t *
                                                    unhandled_attrs);

/******************* implementation functions ********************************/

AXIS2_DECLARE(axis2_xml_schema_annotated_t *)
axis2_xml_schema_annotated_create(axis2_env_t **env)
{
    axis2_xml_schema_annotated_impl_t *annotated_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    AXIS2_ENV_CHECK(env, NULL);

    annotated_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_annotated_impl_t));
    if(!annotated_impl)
    {
        AXIS2_ERROR_SET((*env)->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    annotated_impl->schema_obj = NULL;
    annotated_impl->obj_type = AXIS2_XML_SCHEMA_ANNOTATED;
    annotated_impl->ht_super = NULL;
    annotated_impl->methods = NULL;
    annotated_impl->id = NULL;
    annotated_impl->annotation = NULL;
    annotated_impl->unhandled_attrs = NULL;
    annotated_impl->annotated.ops = NULL;
    annotated_impl->annotated.base.ops = NULL;
    
    annotated_impl->annotated.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_annotated_ops_t));
    if(!annotated_impl->annotated.ops)
    {
        axis2_xml_schema_annotated_free(&(annotated_impl->annotated), env);
        AXIS2_ERROR_SET((*env)->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    annotated_impl->annotated.ops->free = 
            axis2_xml_schema_annotated_free;
    annotated_impl->annotated.ops->super_objs = 
            axis2_xml_schema_annotated_super_objs;
    annotated_impl->annotated.ops->type = 
            axis2_xml_schema_annotated_type;
    annotated_impl->annotated.ops->get_base_impl = 
            axis2_xml_schema_annotated_get_base_impl;
    annotated_impl->annotated.ops->get_id = 
            axis2_xml_schema_annotated_get_id;
    annotated_impl->annotated.ops->set_id = 
            axis2_xml_schema_annotated_set_id;
    annotated_impl->annotated.ops->get_annotation = 
            axis2_xml_schema_annotated_get_annotation;
    annotated_impl->annotated.ops->set_annotation = 
            axis2_xml_schema_annotated_set_annotation;
    annotated_impl->annotated.ops->get_unhandled_attrs = 
            axis2_xml_schema_annotated_get_unhandled_attrs;
    annotated_impl->annotated.ops->set_unhandled_attrs = 
            axis2_xml_schema_annotated_set_unhandled_attrs;
   
    annotated_impl->methods = axis2_hash_make(env);
    if(!annotated_impl->methods)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(annotated_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_annotated_free);
    axis2_hash_set(annotated_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_annotated_super_objs);
    axis2_hash_set(annotated_impl->methods, "type", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_annotated_type);
    axis2_hash_set(annotated_impl->methods, "get_id", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_annotated_get_id);
    axis2_hash_set(annotated_impl->methods, "set_id", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_annotated_set_id);
    axis2_hash_set(annotated_impl->methods, "get_annotation", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_annotated_get_annotation);
    axis2_hash_set(annotated_impl->methods, "set_annotation", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_annotated_set_annotation);
    axis2_hash_set(annotated_impl->methods, "get_unhandled_attrs", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_annotated_get_unhandled_attrs);
    axis2_hash_set(annotated_impl->methods, "set_unhandled_attrs", 
       AXIS2_HASH_KEY_STRING, axis2_xml_schema_annotated_set_unhandled_attrs);

    annotated_impl->schema_obj = axis2_xml_schema_obj_create(env);
    if(!annotated_impl->schema_obj)
    {
        axis2_xml_schema_annotated_free(&(annotated_impl->annotated), env);
        return NULL;
    }

    annotated_impl->ht_super = axis2_hash_make(env);
    if(!annotated_impl->ht_super)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(annotated_impl->ht_super, "AXIS2_XML_SCHEMA_ANNOTATED", AXIS2_HASH_KEY_STRING, 
           &(annotated_impl->annotated) );
    axis2_hash_set(annotated_impl->ht_super, "AXIS2_XML_SCHEMA_OBJ", AXIS2_HASH_KEY_STRING, 
           annotated_impl->schema_obj );

    status = axis2_xml_schema_obj_resolve_methods(
            &(annotated_impl->annotated.base), env, annotated_impl->schema_obj, 
            annotated_impl->methods);
            
    return &(annotated_impl->annotated);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_annotated_free(void *annotated,
                                axis2_env_t **env)
{
    axis2_xml_schema_annotated_impl_t *annotated_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    annotated_impl = AXIS2_INTF_TO_IMPL(annotated);

    if(NULL != annotated_impl->id)
    {
        AXIS2_FREE((*env)->allocator, annotated_impl->id);
        annotated_impl->id = NULL;
    }

    if(NULL != annotated_impl->annotation)
    {
        AXIS2_XML_SCHEMA_ANNOTATION_FREE(annotated_impl->annotation, env);
        annotated_impl->annotation = NULL;
    }
 
    if(NULL != annotated_impl->unhandled_attrs)
    {
        /* TODO need to iterate and free attributes */
        AXIS2_ARRAY_LIST_FREE(annotated_impl->unhandled_attrs, env);
        annotated_impl->unhandled_attrs = NULL;
    }  

    if(NULL != annotated_impl->ht_super)
    {
        axis2_hash_free(annotated_impl->ht_super, env);
        annotated_impl->ht_super = NULL;
    }

    if(NULL != annotated_impl->methods)
    {
        axis2_hash_free(annotated_impl->methods, env);
        annotated_impl->methods = NULL;
    }
    
    if(NULL != annotated_impl->schema_obj)
    {
        AXIS2_XML_SCHEMA_OBJ_FREE(annotated_impl->schema_obj, env);
        annotated_impl->schema_obj = NULL;
    }
    
    if(NULL != annotated_impl->annotated.ops)
    {
        AXIS2_FREE((*env)->allocator, annotated_impl->annotated.ops);
        annotated_impl->annotated.ops = NULL;
    }
    if(NULL != annotated_impl->annotated.base.ops)
    {
        AXIS2_FREE((*env)->allocator, annotated_impl->annotated.base.ops);
        annotated_impl->annotated.base.ops = NULL;
    }

    AXIS2_FREE((*env)->allocator, annotated_impl);
    annotated_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_xml_schema_annotated_super_objs(
        void *annotated,
        axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(annotated)->ht_super;
}

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_annotated_type(
        void *annotated,
        axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(annotated)->obj_type;
}

axis2_xml_schema_obj_t *AXIS2_CALL
axis2_xml_schema_annotated_get_base_impl(void *annotated,
                                axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(annotated)->schema_obj;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_annotated_resolve_methods(
                                axis2_xml_schema_annotated_t *annotated,
                                axis2_env_t **env,
                                axis2_xml_schema_annotated_t *annotated_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_annotated_impl_t *sch_annotated_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, annotated_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    sch_annotated_impl = (axis2_xml_schema_annotated_impl_t *) annotated_impl;

    annotated->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_annotated_ops_t));
    if(!annotated->ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    annotated->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);

    annotated->ops->super_objs = axis2_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);

    annotated->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);

    annotated->ops->get_id = axis2_hash_get(methods, "get_id", 
            AXIS2_HASH_KEY_STRING);
    if(!annotated->ops->get_id)
            annotated->ops->get_id = sch_annotated_impl->annotated.ops->get_id;
    
    annotated->ops->set_id = axis2_hash_get(methods, "set_id", 
            AXIS2_HASH_KEY_STRING);
    if(!annotated->ops->set_id)
            annotated->ops->set_id = 
            sch_annotated_impl->annotated.ops->set_id;

    annotated->ops->get_annotation = axis2_hash_get(methods, "get_annotation", 
            AXIS2_HASH_KEY_STRING);
    if(!annotated->ops->get_annotation)
            annotated->ops->get_annotation = sch_annotated_impl->annotated.ops->get_annotation;
    
    annotated->ops->set_annotation = axis2_hash_get(methods, "set_annotation", 
            AXIS2_HASH_KEY_STRING);
    if(!annotated->ops->set_annotation)
            annotated->ops->set_annotation = 
            sch_annotated_impl->annotated.ops->set_annotation;
    
    annotated->ops->get_unhandled_attrs = axis2_hash_get(methods, 
            "get_unhandled_attrs", AXIS2_HASH_KEY_STRING);
    if(!annotated->ops->get_unhandled_attrs)
            annotated->ops->get_unhandled_attrs = 
            sch_annotated_impl->annotated.ops->get_unhandled_attrs;
    
    annotated->ops->set_unhandled_attrs = axis2_hash_get(methods, 
            "set_unhandled_attrs", AXIS2_HASH_KEY_STRING);
    if(!annotated->ops->set_unhandled_attrs)
            annotated->ops->set_unhandled_attrs = 
            sch_annotated_impl->annotated.ops->set_unhandled_attrs;

    return axis2_xml_schema_obj_resolve_methods(&(annotated->base), 
            env, sch_annotated_impl->schema_obj, methods);
}

axis2_char_t *AXIS2_CALL
axis2_xml_schema_annotated_get_id(void *annotated,
                                        axis2_env_t **env)
{
    axis2_xml_schema_annotated_impl_t *annotated_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_ANNOTATED_SUPER_OBJS(annotated, env);
    if(NULL != ht_super)
    {
        annotated_impl = (axis2_xml_schema_annotated_impl_t*)
            axis2_hash_get(ht_super, "AXIS2_XML_SCHEMA_ANNOTATED",
                AXIS2_HASH_KEY_STRING);
        if(!annotated_impl)
           return NULL;
    }
    
    return annotated_impl->id;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_annotated_set_id(void *annotated,
                                            axis2_env_t **env,
                                            axis2_char_t *id)
{
    axis2_xml_schema_annotated_impl_t *annotated_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, id,AXIS2_FAILURE );
    
    ht_super = AXIS2_XML_SCHEMA_ANNOTATED_SUPER_OBJS(annotated, env);
    if(NULL != ht_super)
    {
        annotated_impl = (axis2_xml_schema_annotated_impl_t*)
            axis2_hash_get(ht_super, "AXIS2_XML_SCHEMA_ANNOTATED",
                AXIS2_HASH_KEY_STRING);
        if(!annotated_impl)
            return AXIS2_FAILURE;
    }
    if(annotated_impl->id)
    {
        AXIS2_FREE((*env)->allocator, annotated_impl->id);
        annotated_impl->id = NULL;
    }
    annotated_impl->id = AXIS2_STRDUP(id, env);
    return AXIS2_SUCCESS;
        
}

axis2_xml_schema_annotation_t *AXIS2_CALL
axis2_xml_schema_annotated_get_annotation(void *annotated,
                                        axis2_env_t **env)
{
    axis2_xml_schema_annotated_impl_t *annotated_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_ANNOTATED_SUPER_OBJS(annotated, env);
    if(NULL != ht_super)
    {
        annotated_impl = (axis2_xml_schema_annotated_impl_t*)
            axis2_hash_get(ht_super, "AXIS2_XML_SCHEMA_ANNOTATED",
                AXIS2_HASH_KEY_STRING);
        if(!annotated_impl)
            return NULL;
    } return annotated_impl->annotation;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_annotated_set_annotation(void *annotated,
                                            axis2_env_t **env,
                                            axis2_xml_schema_annotation_t *
                                                annotation)
{
    axis2_xml_schema_annotated_impl_t *annotated_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = AXIS2_XML_SCHEMA_ANNOTATED_SUPER_OBJS(annotated, env);
    if(NULL != ht_super)
    {
        annotated_impl = (axis2_xml_schema_annotated_impl_t*)
            axis2_hash_get(ht_super, "AXIS2_XML_SCHEMA_ANNOTATED",
                AXIS2_HASH_KEY_STRING);
        if(!annotated_impl)
            return AXIS2_FAILURE;
    }
    if(NULL != annotated_impl->annotation)
    {
        /* TODO Free annotation 
        AXIS2_XML_SCHEMA_ANNOTATED_FREE(annotated_impl->annotation, env);
        */
    }
    annotated_impl->annotation = annotation;
    return AXIS2_SUCCESS;
        
}

axis2_array_list_t *AXIS2_CALL
axis2_xml_schema_annotated_get_unhandled_attrs(void *annotated,
                                                axis2_env_t **env)
{
    axis2_xml_schema_annotated_impl_t *annotated_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = AXIS2_XML_SCHEMA_ANNOTATED_SUPER_OBJS(annotated, env);
    if(NULL != ht_super)
    {
        annotated_impl = (axis2_xml_schema_annotated_impl_t*)
            axis2_hash_get(ht_super, "AXIS2_XML_SCHEMA_ANNOTATED",
                AXIS2_HASH_KEY_STRING);
        if(!annotated_impl)
            return NULL;
    }
    return annotated_impl->unhandled_attrs;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_annotated_set_unhandled_attrs(void *annotated,
                                                axis2_env_t **env,
                                                axis2_array_list_t *
                                                    unhandled_attrs)
{
    axis2_xml_schema_annotated_impl_t *annotated_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = AXIS2_XML_SCHEMA_ANNOTATED_SUPER_OBJS(annotated, env);
    if(NULL != ht_super)
    {
        annotated_impl = (axis2_xml_schema_annotated_impl_t*)
            axis2_hash_get(ht_super, "AXIS2_XML_SCHEMA_ANNOTATED",
                AXIS2_HASH_KEY_STRING);
        if(!annotated_impl)
            return AXIS2_FAILURE;
    }
    if(annotated_impl->unhandled_attrs)
    {
        /* TODO Free unhandled_attrs */
    }
    annotated_impl->unhandled_attrs = unhandled_attrs;
    return AXIS2_SUCCESS;
        
}

