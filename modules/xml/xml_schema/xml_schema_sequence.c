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

#include <axis2_xml_schema_sequence.h>
#include <axis2_xml_schema_group_base.h>

typedef struct axis2_xml_schema_sequence_impl axis2_xml_schema_sequence_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_sequence_impl
{
    axis2_xml_schema_sequence_t sequence;
    axis2_xml_schema_group_base_t *base;
    axis2_hash_t *methods;
    axis2_xml_schema_obj_collection_t *items;
};

#define INTF_TO_IMPL(sequence) ((axis2_xml_schema_sequence_impl_t *) sequence)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_sequence_free(void *sequence,
                        axis2_env_t **env);

axis2_xml_schema_group_base_t *AXIS2_CALL
axis2_xml_schema_sequence_get_base_impl(void *sequence,
                                        axis2_env_t **env);

axis2_xml_schema_obj_collection_t *AXIS2_CALL
axis2_xml_schema_sequence_get_items(void *sequence,
                                axis2_env_t **env);


AXIS2_DECLARE(axis2_xml_schema_sequence_t *)
axis2_xml_schema_sequence_create(axis2_env_t **env)
{
    axis2_xml_schema_sequence_impl_t *sequence_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    sequence_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_sequence_impl_t));

    sequence_impl->base = NULL;
    sequence_impl->methods = NULL;
    sequence_impl->items = NULL;
    sequence_impl->sequence.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_sequence_ops_t));

    sequence_impl->sequence.ops->free = axis2_xml_schema_sequence_free;
    sequence_impl->sequence.ops->get_base_impl = 
            axis2_xml_schema_sequence_get_base_impl;
    sequence_impl->sequence.ops->get_items = 
            axis2_xml_schema_sequence_get_items;
   
    sequence_impl->methods = axis2_hash_make(env);
    if(!sequence_impl->methods)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(sequence_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_sequence_free);
    axis2_hash_set(sequence_impl->methods, "get_items", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_sequence_get_items);
    
    sequence_impl->base = axis2_xml_schema_group_base_create(env);
    status = axis2_xml_schema_group_base_resolve_methods(
            &(sequence_impl->sequence.base), env, sequence_impl->base, 
            sequence_impl->methods);
    
    return &(sequence_impl->sequence);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_sequence_free(void *sequence,
                                    axis2_env_t **env)
{
    axis2_xml_schema_sequence_impl_t *sequence_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    sequence_impl = INTF_TO_IMPL(sequence);

    if(sequence_impl->items)
    {
        AXIS2_XML_SCHEMA_OBJ_COLLECTION_FREE(sequence_impl->items, env);
        sequence_impl->items = NULL;
    }
    
    if(sequence_impl->methods)
    {
        axis2_hash_free(sequence_impl->methods, env);
        sequence_impl->methods = NULL;
    }

    if(sequence_impl->base)
    {
        AXIS2_XML_SCHEMA_GROUP_BASE_FREE(sequence_impl->base, env);
        sequence_impl->base = NULL;
    }
    
    if((&(sequence_impl->sequence))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(sequence_impl->sequence))->ops);
        (&(sequence_impl->sequence))->ops = NULL;
    }

    if(sequence_impl)
    {
        AXIS2_FREE((*env)->allocator, sequence_impl);
        sequence_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_group_base_t *AXIS2_CALL
axis2_xml_schema_sequence_get_base_impl(void *sequence,
                                axis2_env_t **env)
{
    axis2_xml_schema_sequence_impl_t *sequence_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    sequence_impl = INTF_TO_IMPL(sequence);

    return sequence_impl->base;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_sequence_resolve_methods(
                                axis2_xml_schema_sequence_t *sequence,
                                axis2_env_t **env,
                                axis2_xml_schema_sequence_t *sequence_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_sequence_impl_t *sequence_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, sequence_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    sequence_impl_l = (axis2_xml_schema_sequence_impl_t *) sequence_impl;
    
    sequence->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_sequence_ops_t));
    sequence->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    sequence->ops->get_base_impl = 
            sequence_impl_l->sequence.ops->get_base_impl;
    sequence->ops->get_items = 
            sequence_impl_l->sequence.ops->get_items;
    
    return axis2_xml_schema_group_base_resolve_methods(&(sequence->base), 
            env, sequence_impl_l->base, methods);
}

axis2_xml_schema_obj_collection_t *AXIS2_CALL
axis2_xml_schema_sequence_get_items(void *sequence,
                                    axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return INTF_TO_IMPL(sequence)->items;
}

