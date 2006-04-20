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
 
#include <xml_schema/axis2_xml_schema_type_receiver.h>
#include <xml_schema/axis2_xml_schema_type.h>
#include <axis2_hash.h>

typedef struct axis2_xml_schema_type_receiver_impl axis2_xml_schema_type_receiver_impl_t;

/** 
 * @brief Xml Schema Type Receiver Struct Impl
 *	Axis2 Xml Schema Type Receiver  
 */ 
struct axis2_xml_schema_type_receiver_impl
{
    axis2_xml_schema_type_receiver_t type_receiver;
};

#define INTF_TO_IMPL(type_receiver) ((axis2_xml_schema_type_receiver_impl_t *) type_receiver)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_type_receiver_free(void *type_receiver,
                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_type_receiver_set_type(void *type_receiver,
                        axis2_env_t **env,
                        axis2_xml_schema_type_t *type);

AXIS2_DECLARE(axis2_xml_schema_type_receiver_t *)
axis2_xml_schema_type_receiver_create(axis2_env_t **env)
{
    axis2_xml_schema_type_receiver_impl_t *type_receiver_impl = NULL;
    
    type_receiver_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_type_receiver_impl_t));

    type_receiver_impl->type_receiver.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_type_receiver_ops_t)); 


    type_receiver_impl->type_receiver.ops->free = axis2_xml_schema_type_receiver_free;
    type_receiver_impl->type_receiver.ops->set_type = axis2_xml_schema_type_receiver_set_type;

    return &(type_receiver_impl->type_receiver);
}


axis2_status_t AXIS2_CALL
axis2_xml_schema_type_receiver_free(void *type_receiver,
                axis2_env_t **env)
{
    axis2_xml_schema_type_receiver_impl_t *type_receiver_impl = NULL;

    type_receiver_impl = INTF_TO_IMPL(type_receiver);

    if((&(type_receiver_impl->type_receiver))->ops)
    {
        AXIS2_FREE((*env)->allocator,((&(type_receiver_impl->type_receiver))->ops));
        (&(type_receiver_impl->type_receiver))->ops = NULL;
    }

    if(type_receiver_impl)
    {
        AXIS2_FREE((*env)->allocator,(type_receiver_impl));
        type_receiver_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_type_receiver_resolve_methods(axis2_xml_schema_type_receiver_t *type_receiver,
                                        axis2_env_t **env,
                                        axis2_xml_schema_type_receiver_t *type_receiver_impl,
                                        axis2_hash_t *methods)
{    
    axis2_xml_schema_type_receiver_impl_t *type_receiver_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, type_receiver_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    type_receiver_impl_l = (axis2_xml_schema_type_receiver_impl_t *) type_receiver_impl;
    
    type_receiver->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_type_receiver_ops_t));
    type_receiver->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    type_receiver->ops->free = axis2_hash_get(methods, "set_type", 
            AXIS2_HASH_KEY_STRING);
    return AXIS2_SUCCESS;    

}

axis2_status_t AXIS2_CALL
axis2_xml_schema_type_receiver_set_type(void *type_receiver,
                        axis2_env_t **env,
                        axis2_xml_schema_type_t *type)
{
    axis2_xml_schema_type_receiver_impl_t *type_receiver_impl = NULL;

    type_receiver_impl = INTF_TO_IMPL(type_receiver);

    return AXIS2_SUCCESS;
}

