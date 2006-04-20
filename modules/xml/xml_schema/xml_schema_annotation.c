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

#include <xml_schema/axis2_xml_schema_annotation.h>

typedef struct axis2_xml_schema_annotation_impl axis2_xml_schema_annotation_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_annotation_impl
{
    axis2_xml_schema_annotation_t annotation;
    axis2_xml_schema_obj_t *schema_obj;
    axis2_hash_t *items;
    axis2_hash_t *methods;
};

#define INTF_TO_IMPL(annotation) ((axis2_xml_schema_annotation_impl_t *) annotation)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_annotation_free(void *annotation,
                        axis2_env_t **env);

axis2_xml_schema_obj_t *AXIS2_CALL
axis2_xml_schema_annotation_get_base_impl(void *annotation,
                                axis2_env_t **env);

axis2_hash_t * AXIS2_CALL
axis2_xml_schema_annotation_get_items(void *annotation,
                        axis2_env_t **env);

AXIS2_DECLARE(axis2_xml_schema_annotation_t *)
axis2_xml_schema_annotation_create(axis2_env_t **env)
{
    axis2_xml_schema_annotation_impl_t *annotation_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    annotation_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_annotation_impl_t));

    annotation_impl->schema_obj = NULL;
    annotation_impl->methods = NULL;
    annotation_impl->items = NULL;
    annotation_impl->annotation.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_annotation_ops_t));

    annotation_impl->annotation.ops->free = axis2_xml_schema_annotation_free;
    annotation_impl->annotation.ops->get_base_impl = 
            axis2_xml_schema_annotation_get_base_impl;
    annotation_impl->annotation.ops->get_items = 
            axis2_xml_schema_annotation_get_items;
   
    if(!annotation_impl->items)
    {
        annotation_impl->items = axis2_hash_make(env);
        if(!annotation_impl->items)
            return NULL;
    }
    
    annotation_impl->methods = axis2_hash_make(env);
    if(!annotation_impl->methods) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(annotation_impl->methods, "free", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_annotation_free);
    axis2_hash_set(annotation_impl->methods, "get_items", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_annotation_get_items);

    annotation_impl->schema_obj = axis2_xml_schema_obj_create(env);
    status = 
        axis2_xml_schema_obj_resolve_methods(&(annotation_impl->annotation.base), 
            env, annotation_impl->schema_obj, annotation_impl->methods);
    return &(annotation_impl->annotation);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_annotation_free(void *annotation,
                                    axis2_env_t **env)
{
    axis2_xml_schema_annotation_impl_t *annotation_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    annotation_impl = INTF_TO_IMPL(annotation);

    if(annotation_impl->items)
    {
        axis2_hash_free(annotation_impl->items, env);
        annotation_impl->items = NULL;
    }
    
    if(annotation_impl->methods)
    {
        axis2_hash_free(annotation_impl->methods, env);
        annotation_impl->methods = NULL;
    }

    if(annotation_impl->schema_obj)
    {
        AXIS2_XML_SCHEMA_OBJ_FREE(annotation_impl->schema_obj, env);
        annotation_impl->schema_obj = NULL;
    }
    
    if((&(annotation_impl->annotation))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(annotation_impl->annotation))->ops);
        (&(annotation_impl->annotation))->ops = NULL;
    }

    if(annotation_impl)
    {
        AXIS2_FREE((*env)->allocator, annotation_impl);
        annotation_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_obj_t *AXIS2_CALL
axis2_xml_schema_annotation_get_base_impl(void *annotation,
                                axis2_env_t **env)
{
    axis2_xml_schema_annotation_impl_t *annotation_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    annotation_impl = INTF_TO_IMPL(annotation);

    return annotation_impl->schema_obj;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_annotation_resolve_methods(
                                axis2_xml_schema_annotation_t *annotation,
                                axis2_env_t **env,
                                axis2_xml_schema_annotation_t *annotation_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_annotation_impl_t *annotation_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, annotation_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    annotation_impl_l = (axis2_xml_schema_annotation_impl_t *) annotation_impl;
    
    annotation->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_annotation_ops_t));
    annotation->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    annotation->ops->get_base_impl = 
            annotation_impl_l->annotation.ops->get_base_impl;
    annotation->ops->get_items = 
            annotation_impl_l->annotation.ops->get_items;
    
    return axis2_xml_schema_obj_resolve_methods(&(annotation->base), 
            env, annotation_impl_l->schema_obj, methods);
}

axis2_hash_t *AXIS2_CALL
axis2_xml_schema_annotation_get_items(void *annotation,
                                        axis2_env_t **env)
{
    return INTF_TO_IMPL(annotation)->items;
}

