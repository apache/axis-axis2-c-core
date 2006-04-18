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
 
#include <axis2_validation_event_handler.h>
#include <axis2_hash.h>

typedef struct axis2_xml_schema_validation_event_handler_impl axis2_xml_schema_validation_event_handler_impl_t;

/** 
 * @brief Xml Schema Validation Event Handler Struct Impl
 *	Axis2 Xml Schema Validation Event Handler  
 */ 
struct axis2_xml_schema_validation_event_handler_impl
{
    axis2_xml_schema_validation_event_handler_t axis2_validation_event_handler;
};

#define INTF_TO_IMPL(axis2_validation_event_handler) ((axis2_xml_schema_validation_event_handler_impl_t *) axis2_validation_event_handler)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_validation_event_handler_free(void *axis2_validation_event_handler,
                axis2_env_t **env);

AXIS2_DECLARE(axis2_xml_schema_validation_event_handler_t *)
axis2_xml_schema_validation_event_handler_create(axis2_env_t **env)
{
    axis2_xml_schema_validation_event_handler_impl_t *axis2_validation_event_handler_impl = NULL;
    
    axis2_validation_event_handler_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_validation_event_handler_impl_t));

    axis2_validation_event_handler_impl->axis2_validation_event_handler.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_validation_event_handler_ops_t)); 


    axis2_validation_event_handler_impl->axis2_validation_event_handler.ops->free = axis2_xml_schema_validation_event_handler_free;

    return &(axis2_validation_event_handler_impl->axis2_validation_event_handler);
}


axis2_status_t AXIS2_CALL
axis2_xml_schema_validation_event_handler_free(void *axis2_validation_event_handler,
                axis2_env_t **env)
{
    axis2_xml_schema_validation_event_handler_impl_t *axis2_validation_event_handler_impl = NULL;

    axis2_validation_event_handler_impl = INTF_TO_IMPL(axis2_validation_event_handler);

    if((&(axis2_validation_event_handler_impl->axis2_validation_event_handler))->ops)
    {
        free((&(axis2_validation_event_handler_impl->axis2_validation_event_handler))->ops);
        (&(axis2_validation_event_handler_impl->axis2_validation_event_handler))->ops = NULL;
    }

    if(axis2_validation_event_handler_impl)
    {
        free(axis2_validation_event_handler_impl);
        axis2_validation_event_handler_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_validation_event_handler_resolve_methods(axis2_xml_schema_validation_event_handler_t *axis2_validation_event_handler,
                                        axis2_env_t **env,
                                        axis2_xml_schema_validation_event_handler_t *axis2_validation_event_handler_impl,
                                        axis2_hash_t *methods)
{    
    axis2_xml_schema_validation_event_handler_impl_t *axis2_validation_event_handler_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, axis2_validation_event_handler_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    axis2_validation_event_handler_impl_l = (axis2_xml_schema_validation_event_handler_impl_t *) axis2_validation_event_handler_impl;
    
    axis2_validation_event_handler->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_validation_event_handler_ops_t));
    axis2_validation_event_handler->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    return AXIS2_SUCCESS;    

}

