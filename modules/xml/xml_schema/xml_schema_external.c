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

#include <axis2_xml_schema_external.h>
#include <axis2_xml_schema.h>

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
typedef struct axis2_xml_schema_external_impl
{
    axis2_xml_schema_external_t external;
    
    axis2_xml_schema_annotated_t *annotated;
    
    axis2_xml_schema_t *schema;
    
    axis2_hash_t *methods;

}axis2_xml_schema_external_impl_t;

#define AXIS2_INTF_TO_IMPL(external) \
        ((axis2_xml_schema_external_impl_t *) external)

/********************* function prototypes ***********************************/

axis2_status_t AXIS2_CALL 
axis2_xml_schema_external_free(void *external,
                        axis2_env_t **env);

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_external_get_base_impl(void *external,
                                        axis2_env_t **env);

axis2_xml_schema_t* AXIS2_CALL
axis2_xml_schema_external_get_schema(void *external,
                                     axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_external_set_schema(void *external,
                                     axis2_env_t **env,
                                     axis2_xml_schema_t *schema);

/********************* end function prototypes ********************************/

AXIS2_DECLARE(axis2_xml_schema_external_t *)
axis2_xml_schema_external_create(axis2_env_t **env)
{
    axis2_xml_schema_external_impl_t *external_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    external_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_external_impl_t));
    if(!external_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    external_impl->schema = NULL;
    external_impl->annotated = NULL;
    external_impl->methods = NULL;
    external_impl->external.ops = NULL;
    external_impl->external.base.ops = NULL;
    
    external_impl->external.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_external_ops_t));

    if(!external_impl->external.ops)
    {
        axis2_xml_schema_external_free(&(external_impl->external), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    external_impl->external.ops->free = 
        axis2_xml_schema_external_free;
    external_impl->external.ops->get_base_impl = 
        axis2_xml_schema_external_get_base_impl;
        
    external_impl->external.ops->get_schema = 
            axis2_xml_schema_external_get_schema;
            
    external_impl->external.ops->set_schema = 
            axis2_xml_schema_external_set_schema;

    external_impl->methods = axis2_hash_make(env);
    if(!external_impl->methods)
    {
        axis2_xml_schema_external_free(&(external_impl->external), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    axis2_hash_set(external_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_external_free);

    axis2_hash_set(external_impl->methods, "get_schema", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_external_get_schema);
    axis2_hash_set(external_impl->methods, "set_schema", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_external_set_schema);

    external_impl->annotated = axis2_xml_schema_annotated_create(env);
     if(!external_impl->annotated)
    {
        axis2_xml_schema_external_free(&(external_impl->external), env);
        return NULL;
    }
    status = axis2_xml_schema_annotated_resolve_methods(
            &(external_impl->external.base), env, external_impl->annotated, 
            external_impl->methods);

    return &(external_impl->external);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_external_free(void *external,
                                    axis2_env_t **env)
{
    axis2_xml_schema_external_impl_t *external_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    external_impl = AXIS2_INTF_TO_IMPL(external);

    if(NULL != external_impl->methods)
    {
        axis2_hash_free(external_impl->methods, env);
        external_impl->methods = NULL;
    }

    if(NULL != external_impl->annotated)
    {
        AXIS2_XML_SCHEMA_ANNOTATED_FREE(external_impl->annotated, env);
        external_impl->annotated = NULL;
    }
    
    if(NULL != external_impl->external.ops)
    {
        AXIS2_FREE((*env)->allocator, external_impl->external.ops);
        external_impl->external.ops = NULL;
    }
    if(NULL != external_impl->external.base.ops)
    {
        AXIS2_FREE((*env)->allocator, external_impl->external.base.ops);
        external_impl->external.base.ops = NULL;        
    }
    AXIS2_FREE((*env)->allocator, external_impl);
    external_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_external_get_base_impl(void *external,
                                axis2_env_t **env)
{
    axis2_xml_schema_external_impl_t *external_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    external_impl = AXIS2_INTF_TO_IMPL(external);

    return external_impl->annotated;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_external_resolve_methods(
                                axis2_xml_schema_external_t *external,
                                axis2_env_t **env,
                                axis2_xml_schema_external_t *external_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_external_impl_t *external_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, external_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    external_impl_l = (axis2_xml_schema_external_impl_t *) external_impl;
    
    external->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_external_ops_t));
            
    external->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
            
    external->ops->get_base_impl = 
        external_impl_l->external.ops->get_base_impl;
    external->ops->set_schema = 
        external_impl_l->external.ops->set_schema;
    external->ops->get_schema = 
        external_impl_l->external.ops->get_schema;
        
    return axis2_xml_schema_annotated_resolve_methods(&(external->base), 
            env, external_impl_l->annotated, methods);
}

axis2_xml_schema_t * AXIS2_CALL
axis2_xml_schema_external_get_schema(void *external,
                                            axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(external)->schema;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_external_set_schema(void *external,
                                     axis2_env_t **env,
                                     axis2_xml_schema_t *schema)
{
    axis2_xml_schema_external_impl_t *ext_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ext_impl = AXIS2_INTF_TO_IMPL(external);
    if(NULL != ext_impl->schema)
    {
        AXIS2_XML_SCHEMA_FREE(ext_impl->schema, env);
        ext_impl->schema = NULL;
    }
    ext_impl->schema = schema;
    return AXIS2_SUCCESS;
}
