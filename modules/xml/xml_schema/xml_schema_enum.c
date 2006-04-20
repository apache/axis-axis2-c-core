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
 
#include <xml_schema/axis2_xml_schema_enum.h>
#include <axis2_hash.h>

typedef struct axis2_xml_schema_enum_impl axis2_xml_schema_enum_impl_t;

/** 
 * @brief Xml Schema Enum Impl
 *	Axis2 Xml Schema Enum 
 */ 
struct axis2_xml_schema_enum_impl
{
    axis2_xml_schema_enum_t schema_enum;
    axis2_array_list_t *values;
    axis2_char_t *value;
};

#define INTF_TO_IMPL(schema_enum) ((axis2_xml_schema_enum_impl_t *) schema_enum)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_enum_free(void *schema_enum,
                axis2_env_t **envv);

axis2_char_t * AXIS2_CALL
axis2_xml_schema_enum_get_value(void *schema_enum,
                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_enum_set_value(void *schema_enum,
                        axis2_env_t **env,
                        axis2_char_t *value);

axis2_bool_t AXIS2_CALL
axis2_xml_schema_enum_equals(void *schema_enum,
                                axis2_env_t **env,
                                void *obj);

axis2_array_list_t *AXIS2_CALL
axis2_xml_schema_enum_get_values(void *schema_enum,
                                axis2_env_t **env);

AXIS2_DECLARE(axis2_xml_schema_enum_t *)
axis2_xml_schema_enum_create(axis2_env_t **env,
                                axis2_char_t *value)
{
    axis2_xml_schema_enum_impl_t *schema_enum_impl = NULL;
    
    schema_enum_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_enum_impl_t));

    schema_enum_impl->values = NULL;
    schema_enum_impl->value = NULL;

    schema_enum_impl->schema_enum.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_enum_ops_t)); 

    axis2_xml_schema_enum_set_value(&(schema_enum_impl->schema_enum), 
            env, value);

    schema_enum_impl->schema_enum.ops->free = axis2_xml_schema_enum_free;
    schema_enum_impl->schema_enum.ops->get_value = 
            axis2_xml_schema_enum_get_value;
    schema_enum_impl->schema_enum.ops->set_value = 
            axis2_xml_schema_enum_set_value;
    schema_enum_impl->schema_enum.ops->equals = axis2_xml_schema_enum_equals;
    schema_enum_impl->schema_enum.ops->get_values = 
            axis2_xml_schema_enum_get_values;

    return &(schema_enum_impl->schema_enum);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_enum_free(void *schema_enum,
                axis2_env_t **env)
{
    axis2_xml_schema_enum_impl_t *schema_enum_impl = NULL;

    schema_enum_impl = INTF_TO_IMPL(schema_enum);

    if(schema_enum_impl->value)
    {
        AXIS2_FREE((*env)->allocator, schema_enum_impl->value);
        schema_enum_impl->value = NULL;
    }

    if(schema_enum_impl->values)
    {
        AXIS2_ARRAY_LIST_FREE(schema_enum_impl->values, env);
        schema_enum_impl->values = NULL;
    }

    if((&(schema_enum_impl->schema_enum))->ops)
    {
        free((&(schema_enum_impl->schema_enum))->ops);
        (&(schema_enum_impl->schema_enum))->ops = NULL;
    }

    if(schema_enum_impl)
    {
        free(schema_enum_impl);
        schema_enum_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_enum_resolve_methods(axis2_xml_schema_enum_t *schema_enum,
                                        axis2_env_t **env,
                                        axis2_xml_schema_enum_t *schema_enum_impl,
                                        axis2_hash_t *methods)
{    
    axis2_xml_schema_enum_impl_t *schema_enum_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, schema_enum_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    schema_enum_impl_l = (axis2_xml_schema_enum_impl_t *) schema_enum_impl;
    
    schema_enum->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_enum_ops_t));
    schema_enum->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    schema_enum->ops->get_value = 
            schema_enum_impl_l->schema_enum.ops->get_value;
    schema_enum->ops->set_value = 
            schema_enum_impl_l->schema_enum.ops->set_value; 
    schema_enum->ops->equals = 
            schema_enum_impl_l->schema_enum.ops->equals;
    schema_enum->ops->get_values = 
            schema_enum_impl_l->schema_enum.ops->get_values;
    return AXIS2_SUCCESS;    

}


axis2_char_t * AXIS2_CALL
axis2_xml_schema_enum_get_value(void *schema_enum,
                        axis2_env_t **env)
{
    axis2_xml_schema_enum_impl_t *schema_enum_impl = NULL;
    
    schema_enum_impl = INTF_TO_IMPL(schema_enum);
    return schema_enum_impl->value;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_enum_set_value(void *schema_enum,
                        axis2_env_t **env,
                        axis2_char_t *value)
{
    axis2_xml_schema_enum_impl_t *schema_enum_impl = NULL;

    schema_enum_impl = INTF_TO_IMPL(schema_enum);

    if(schema_enum_impl->value)
    {
        free(schema_enum_impl->value);
        schema_enum_impl->value = NULL;
    }
    schema_enum_impl->value = AXIS2_STRDUP(value, env);

    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_xml_schema_enum_equals(void *schema_enum,
                                axis2_env_t **env,
                                void *obj)
{
    axis2_xml_schema_enum_impl_t *schema_enum_impl = NULL;

    schema_enum_impl = INTF_TO_IMPL(schema_enum);

    return AXIS2_TRUE;
}

axis2_array_list_t *AXIS2_CALL
axis2_xml_schema_enum_get_values(void *schema_enum, 
                                axis2_env_t **env)
{
    axis2_xml_schema_enum_impl_t *schema_enum_impl = NULL;

    schema_enum_impl = INTF_TO_IMPL(schema_enum);

    return schema_enum_impl->values; 
}

int AXIS2_CALL
axis2_xml_schema_enum_index(axis2_char_t *value,
                            axis2_array_list_t *values,
                            axis2_env_t **env)
{
   return -1; 
}

