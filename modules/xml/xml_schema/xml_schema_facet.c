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

#include <axis2_xml_schema_facet.h>
#include <axis2_om_element.h>
#include <axis2_om_node.h>

typedef struct axis2_xml_schema_facet_impl 
                axis2_xml_schema_facet_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_facet_impl
{
    axis2_xml_schema_facet_t facet;
    
    axis2_xml_schema_annotated_t *annotated;
    
    axis2_hash_t *methods;
    
    axis2_bool_t fixed;
    
    void *value;
};

#define AXIS2_INTF_TO_IMPL(facet) \
        ((axis2_xml_schema_facet_impl_t *) facet)

/****************** function prototypes ***************************************/

axis2_status_t AXIS2_CALL 
axis2_xml_schema_facet_free(void *facet,
                        axis2_env_t **env);

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_facet_get_base_impl(void *facet,
                                        axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_xml_schema_facet_is_fixed(void *facet,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_facet_set_fixed(void *facet,
                                 axis2_env_t **env,
                                 axis2_bool_t fixed);

void *AXIS2_CALL
axis2_xml_schema_facet_get_value(void *facet,
                                 axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_facet_set_value(void *facet,
                                 axis2_env_t **env,
                                 void *value);

/********************** end function prototypes *******************************/

AXIS2_DECLARE(axis2_xml_schema_facet_t *)
axis2_xml_schema_facet_create(axis2_env_t **env,
                                void *value,
                                axis2_bool_t fixed)
{
    axis2_xml_schema_facet_impl_t *facet_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    facet_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_facet_impl_t));
    if(!facet_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }                    

    facet_impl->annotated = NULL;
    facet_impl->facet.ops = NULL;
    facet_impl->facet.base.ops = NULL;
    facet_impl->methods = NULL;
    facet_impl->fixed = fixed;
    facet_impl->value = value;
    
    facet_impl->facet.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_facet_ops_t));
    if(!facet_impl->facet.ops)
    {
        axis2_xml_schema_facet_free(&(facet_impl->facet), env);
        return NULL;
    }
    
    facet_impl->facet.ops->free = 
        axis2_xml_schema_facet_free;
    facet_impl->facet.ops->get_base_impl = 
        axis2_xml_schema_facet_get_base_impl;
    facet_impl->facet.ops->is_fixed = 
        axis2_xml_schema_facet_is_fixed;
    facet_impl->facet.ops->set_fixed = 
        axis2_xml_schema_facet_set_fixed;
    facet_impl->facet.ops->get_value = 
        axis2_xml_schema_facet_get_value;
    facet_impl->facet.ops->set_value = 
        axis2_xml_schema_facet_set_value;
   
    facet_impl->methods = axis2_hash_make(env);
    if(!facet_impl->methods)
    {
        axis2_xml_schema_facet_free(&(facet_impl->facet), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(facet_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_facet_free);
    axis2_hash_set(facet_impl->methods, "is_fixed", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_facet_is_fixed);
    axis2_hash_set(facet_impl->methods, "set_fixed", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_facet_set_fixed);
    axis2_hash_set(facet_impl->methods, "get_value", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_facet_get_value);
    axis2_hash_set(facet_impl->methods, "set_value", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_facet_set_value);

    
    facet_impl->annotated = axis2_xml_schema_annotated_create(env);
    if(!facet_impl->annotated)
    {
        axis2_xml_schema_facet_free(&(facet_impl->facet), env);
        return NULL;
    }
    status = axis2_xml_schema_annotated_resolve_methods(
            &(facet_impl->facet.base), env, facet_impl->annotated, 
            facet_impl->methods);
    
    return &(facet_impl->facet);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_facet_free(void *facet,
                                    axis2_env_t **env)
{
    axis2_xml_schema_facet_impl_t *facet_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    facet_impl = AXIS2_INTF_TO_IMPL(facet);
    if(NULL != facet_impl->methods)
    {
        axis2_hash_free(facet_impl->methods, env);
        facet_impl->methods = NULL;
    }
    if(NULL != facet_impl->annotated)
    {
        AXIS2_XML_SCHEMA_ANNOTATED_FREE(facet_impl->annotated, env);
        facet_impl->annotated = NULL;
    }
    
    if(NULL != facet_impl->facet.ops)
    {
        AXIS2_FREE((*env)->allocator, facet_impl->facet.ops);
        facet_impl->facet.ops = NULL;
    }
    if(NULL != facet_impl->facet.base.ops)
    {
        AXIS2_FREE((*env)->allocator, facet_impl->facet.base.ops);
        facet_impl->facet.base.ops = NULL;        
    }
    if(NULL != facet_impl)
    {
        AXIS2_FREE((*env)->allocator, facet_impl);
        facet_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_facet_get_base_impl(void *facet,
                                axis2_env_t **env)
{
    axis2_xml_schema_facet_impl_t *facet_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    facet_impl = AXIS2_INTF_TO_IMPL(facet);

    return facet_impl->annotated;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_facet_resolve_methods(
                                axis2_xml_schema_facet_t *facet,
                                axis2_env_t **env,
                                axis2_xml_schema_facet_t *facet_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_facet_impl_t *facet_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, facet_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    facet_impl_l = (axis2_xml_schema_facet_impl_t *) facet_impl;
    
    facet->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_facet_ops_t));
    if(!facet->ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;     
    }            
            
    facet->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    facet->ops->get_base_impl = 
            facet_impl_l->facet.ops->get_base_impl;
    facet->ops->is_fixed = 
            facet_impl_l->facet.ops->is_fixed;
    facet->ops->set_fixed = 
            facet_impl_l->facet.ops->set_fixed;
    facet->ops->get_value = 
            facet_impl_l->facet.ops->get_value;
    facet->ops->set_value = 
            facet_impl_l->facet.ops->set_value;
    return axis2_xml_schema_annotated_resolve_methods(&(facet->base), 
            env, facet_impl_l->annotated, methods);
}

axis2_bool_t AXIS2_CALL
axis2_xml_schema_facet_is_fixed(void *facet,
                                            axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(facet)->fixed;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_facet_set_fixed(void *facet,
                                  axis2_env_t **env,
                                  axis2_bool_t fixed)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(facet)->fixed = fixed;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_xml_schema_facet_get_value(void *facet,
                                 axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(facet)->value;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_facet_set_value(void *facet,
                                 axis2_env_t **env,
                                 void *value)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    /* TODO if value already exist free it */
    
    
    
    AXIS2_INTF_TO_IMPL(facet)->value = value;
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_xml_schema_facet_t*)
axis2_xml_schema_facet_construct (axis2_env_t **env,
                                  axis2_om_node_t *node)
{
    axis2_om_element_t *om_ele = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, node, NULL);
    
    
    return NULL;
}
