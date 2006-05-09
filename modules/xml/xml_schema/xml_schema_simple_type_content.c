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

#include <xml_schema/axis2_xml_schema_simple_type_content.h>

typedef struct axis2_xml_schema_simple_type_content_impl 
                    axis2_xml_schema_simple_type_content_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_simple_type_content_impl
{
    axis2_xml_schema_simple_type_content_t sim_type_cont;
    
    axis2_xml_schema_annotated_t *annotated;

    int type;
    
    axis2_hash_t *methods;
    
};

#define AXIS2_INTF_TO_IMPL(sim_type_cont) \
        ((axis2_xml_schema_simple_type_content_impl_t *) sim_type_cont)
        
/*************** function prototypes *****************************************/

axis2_status_t AXIS2_CALL 
axis2_xml_schema_simple_type_content_free(void *sim_type_cont,
                        axis2_env_t **env);

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_simple_type_content_get_base_impl(void *sim_type_cont,
                                        axis2_env_t **env);

int AXIS2_CALL
axis2_xml_schema_simple_type_content_get_type(void *sim_type_cont,
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_simple_type_content_set_type(void *sim_type_cont,
                                            axis2_env_t **env,
                                            int type);

/*************** function prototypes *****************************************/

AXIS2_DECLARE(axis2_xml_schema_simple_type_content_t *)
axis2_xml_schema_simple_type_content_create(axis2_env_t **env)
{
    axis2_xml_schema_simple_type_content_impl_t *sim_type_cont_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    sim_type_cont_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_simple_type_content_impl_t));
    
    if(!sim_type_cont_impl)
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    sim_type_cont_impl->annotated = NULL;
    sim_type_cont_impl->sim_type_cont.ops = NULL;
    sim_type_cont_impl->sim_type_cont.base.ops = NULL;
    sim_type_cont_impl->methods = NULL;
    sim_type_cont_impl->type = AXIS2_XML_SCHEMA_SIMPLE_CONTENT_TYPE;
    
    
    sim_type_cont_impl->sim_type_cont.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_simple_type_content_ops_t));

    if(!sim_type_cont_impl->sim_type_cont.ops)
    {
        axis2_xml_schema_simple_type_content_free(&(sim_type_cont_impl->sim_type_cont), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    sim_type_cont_impl->sim_type_cont.ops->free = 
            axis2_xml_schema_simple_type_content_free;
    sim_type_cont_impl->sim_type_cont.ops->get_base_impl = 
            axis2_xml_schema_simple_type_content_get_base_impl;
    sim_type_cont_impl->sim_type_cont.ops->get_type = 
            axis2_xml_schema_simple_type_content_get_type;
    sim_type_cont_impl->sim_type_cont.ops->set_type = 
            axis2_xml_schema_simple_type_content_set_type;
   
    sim_type_cont_impl->methods = axis2_hash_make(env);
    if(!sim_type_cont_impl->methods)
    {
        axis2_xml_schema_simple_type_content_free(&(sim_type_cont_impl->sim_type_cont), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(sim_type_cont_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_simple_type_content_free);
    axis2_hash_set(sim_type_cont_impl->methods, "get_type", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_simple_type_content_get_type);
    axis2_hash_set(sim_type_cont_impl->methods, "set_type", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_simple_type_content_set_type);

    sim_type_cont_impl->annotated = axis2_xml_schema_annotated_create(env);
    if(!sim_type_cont_impl->annotated)
    {
        axis2_xml_schema_simple_type_content_free(&(sim_type_cont_impl->sim_type_cont), env);
        return NULL;        
    }
    status = axis2_xml_schema_annotated_resolve_methods(
            &(sim_type_cont_impl->sim_type_cont.base), env, sim_type_cont_impl->annotated, 
            sim_type_cont_impl->methods);
    
    return &(sim_type_cont_impl->sim_type_cont);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_simple_type_content_free(void *sim_type_cont,
                                    axis2_env_t **env)
{
    axis2_xml_schema_simple_type_content_impl_t *sim_type_cont_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    sim_type_cont_impl = AXIS2_INTF_TO_IMPL(sim_type_cont);

    if(NULL != sim_type_cont_impl->methods)
    {
        axis2_hash_free(sim_type_cont_impl->methods, env);
        sim_type_cont_impl->methods = NULL;
    }

    if(NULL != sim_type_cont_impl->annotated)
    {
        AXIS2_XML_SCHEMA_ANNOTATED_FREE(sim_type_cont_impl->annotated, env);
        sim_type_cont_impl->annotated = NULL;
    }
    
    if(NULL != sim_type_cont_impl->sim_type_cont.ops)
    {
        AXIS2_FREE((*env)->allocator, sim_type_cont_impl->sim_type_cont.ops);
        sim_type_cont_impl->sim_type_cont.ops = NULL;
    }
    if(NULL != sim_type_cont_impl->sim_type_cont.base.ops)
    {
        AXIS2_FREE((*env)->allocator, sim_type_cont_impl->sim_type_cont.base.ops);
        sim_type_cont_impl->sim_type_cont.base.ops = NULL;
    }
    
    if(NULL != sim_type_cont_impl)
    {
        AXIS2_FREE((*env)->allocator, sim_type_cont_impl);
        sim_type_cont_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_simple_type_content_get_base_impl(void *sim_type_cont,
                                axis2_env_t **env)
{
    axis2_xml_schema_simple_type_content_impl_t *sim_type_cont_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    sim_type_cont_impl = AXIS2_INTF_TO_IMPL(sim_type_cont);
    return sim_type_cont_impl->annotated;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_simple_type_content_resolve_methods(
                                axis2_xml_schema_simple_type_content_t *sim_type_cont,
                                axis2_env_t **env,
                                axis2_xml_schema_simple_type_content_t *sim_type_cont_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_simple_type_content_impl_t *sim_type_cont_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, sim_type_cont_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    sim_type_cont_impl_l = 
        (axis2_xml_schema_simple_type_content_impl_t *) sim_type_cont_impl;
    
    sim_type_cont->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_simple_type_content_ops_t));
    if(NULL != sim_type_cont->ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }            
    
    sim_type_cont->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    sim_type_cont->ops->get_base_impl = 
            sim_type_cont_impl_l->sim_type_cont.ops->get_base_impl;
    sim_type_cont->ops->get_type = 
            sim_type_cont_impl_l->sim_type_cont.ops->get_type;
    sim_type_cont->ops->set_type = 
            sim_type_cont_impl_l->sim_type_cont.ops->set_type;

    return axis2_xml_schema_annotated_resolve_methods(&(sim_type_cont->base), 
            env, sim_type_cont_impl_l->annotated, methods);
}

int AXIS2_CALL
axis2_xml_schema_simple_type_content_get_type(void *sim_type_cont,
                                              axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(sim_type_cont)->type;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_simple_type_content_set_type(void *sim_type_cont,
                                              axis2_env_t **env,
                                              int type)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(sim_type_cont)->type = type;
    return AXIS2_SUCCESS;
}

