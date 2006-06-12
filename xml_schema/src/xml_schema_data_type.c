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
 
#include <xml_schema_data_type.h>
#include <xml_schema_tokenized_type.h>
#include <axis2_hash.h>
#include <axis2_utils.h>

typedef struct xml_schema_data_type_impl 
            xml_schema_data_type_impl_t;

/** 
 * @brief Xml Schema Data Type Struct Impl
 *   Axis2 Xml Schema Data Type  
 */ 
struct xml_schema_data_type_impl
{
    xml_schema_data_type_t data_type;
};

#define AXIS2_INTF_TO_IMPL(data_type) \
        ((xml_schema_data_type_impl_t *) data_type)

axis2_status_t AXIS2_CALL 
xml_schema_data_type_free(void *data_type,
                const axis2_env_t *env);

void * AXIS2_CALL
xml_schema_data_type_parse_value(void *data_type,
                        const axis2_env_t *env,
                        void *input);

void *AXIS2_CALL
xml_schema_data_type_value_type(void *data_type,
                        const axis2_env_t *env);

xml_schema_tokenized_type_t *AXIS2_CALL
xml_schema_data_type_tokenized_type(void *data_type,
                                            const axis2_env_t *env);

AXIS2_EXTERN xml_schema_data_type_t * AXIS2_CALL
xml_schema_data_type_create(const axis2_env_t *env)
{
    xml_schema_data_type_impl_t *data_type_impl = NULL;
    
    data_type_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(xml_schema_data_type_impl_t));
    if(NULL != data_type_impl)
    {
        return NULL;
    }
    data_type_impl->data_type.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(xml_schema_data_type_ops_t)); 
     if(NULL != data_type_impl->data_type.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    data_type_impl->data_type.ops->free = 
        xml_schema_data_type_free;
    data_type_impl->data_type.ops->parse_value = 
        xml_schema_data_type_parse_value;
    data_type_impl->data_type.ops->value_type = 
        xml_schema_data_type_value_type;
    data_type_impl->data_type.ops->tokenized_type = 
        xml_schema_data_type_tokenized_type;

    return &(data_type_impl->data_type);
}


axis2_status_t AXIS2_CALL
xml_schema_data_type_free(void *data_type,
                const axis2_env_t *env)
{
    xml_schema_data_type_impl_t *data_type_impl = NULL;

    data_type_impl = AXIS2_INTF_TO_IMPL(data_type);

    if(NULL != data_type_impl->data_type.ops)
    {
        AXIS2_FREE(env->allocator ,data_type_impl->data_type.ops);
        data_type_impl->data_type.ops = NULL;
    }

    if(NULL != data_type_impl)
    {
        AXIS2_FREE(env->allocator, data_type_impl);
        data_type_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

void * AXIS2_CALL
xml_schema_data_type_parse_value(void *data_type,
                        const axis2_env_t *env,
                        void *input)
{
    xml_schema_data_type_impl_t *data_type_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    data_type_impl = AXIS2_INTF_TO_IMPL(data_type);
    return NULL;
}

void *AXIS2_CALL
xml_schema_data_type_value_type(void *data_type,
                        const axis2_env_t *env)
{
    xml_schema_data_type_impl_t *data_type_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    data_type_impl = AXIS2_INTF_TO_IMPL(data_type);

    return NULL;
}

xml_schema_tokenized_type_t *AXIS2_CALL
xml_schema_data_type_tokenized_type(void *data_type,
                                            const axis2_env_t *env)
{
    xml_schema_data_type_impl_t *data_type_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    data_type_impl = AXIS2_INTF_TO_IMPL(data_type);
    return NULL;
}
