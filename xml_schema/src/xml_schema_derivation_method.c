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

#include <xml_schema_constants.h>
#include <xml_schema_derivation_method.h>
#include <axis2_utils.h>

typedef struct xml_schema_derivation_method_impl
            xml_schema_derivation_method_impl_t;

/**
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension
 */
struct xml_schema_derivation_method_impl
{
    xml_schema_derivation_method_t derivation_method;

    xml_schema_enum_t *schema_enum;

    xml_schema_types_t type;

    axis2_array_list_t *members;

    axis2_hash_t *ht_super;
};

#define AXIS2_INTF_TO_IMPL(derivation_method) \
        ((xml_schema_derivation_method_impl_t *) derivation_method)

/************************* function prototypes ************************************/
axis2_hash_t* AXIS2_CALL
xml_schema_derivation_method_super_objs(void *derivation_method,
        const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_derivation_method_get_type(void *derivation_method,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_derivation_method_free(void *derivation_method,
        const axis2_env_t *env);

xml_schema_enum_t *AXIS2_CALL
xml_schema_derivation_method_get_base_impl(void *derivation_method,
        const axis2_env_t *env);

axis2_array_list_t * AXIS2_CALL
xml_schema_derivation_method_get_values(void *derivation_method,
        const axis2_env_t *env);


/************************** end function prototypes ********************************/

AXIS2_EXTERN xml_schema_derivation_method_t * AXIS2_CALL
xml_schema_derivation_method_create(const axis2_env_t *env,
        axis2_char_t *value)
{
    xml_schema_derivation_method_impl_t *derivation_method_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    derivation_method_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_derivation_method_impl_t));
    if (!derivation_method_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    derivation_method_impl->schema_enum = NULL;
    derivation_method_impl->members = NULL;
    derivation_method_impl->ht_super = NULL;
    derivation_method_impl->type = XML_SCHEMA_DERIVATION_METHOD;
    derivation_method_impl->derivation_method.base.ops = NULL;
    derivation_method_impl->derivation_method.ops = NULL;

    derivation_method_impl->derivation_method.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_derivation_method_ops_t));
    if (!derivation_method_impl->derivation_method.ops)
    {
        xml_schema_derivation_method_free(
            &(derivation_method_impl->derivation_method), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    derivation_method_impl->derivation_method.ops->free =
        xml_schema_derivation_method_free;
    derivation_method_impl->derivation_method.ops->get_base_impl =
        xml_schema_derivation_method_get_base_impl;
    derivation_method_impl->derivation_method.ops->get_values =
        xml_schema_derivation_method_get_values;
    derivation_method_impl->derivation_method.ops->super_objs =
        xml_schema_derivation_method_super_objs;
    derivation_method_impl->derivation_method.ops->get_type =
        xml_schema_derivation_method_get_type;

    derivation_method_impl->members = axis2_array_list_create(env, 0);

    if (!derivation_method_impl->members)
    {
        xml_schema_derivation_method_free(
            &(derivation_method_impl->derivation_method), env);
        return NULL;
    }
    AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env,
            AXIS2_STRDUP(XML_SCHEMA_CONST_ALL, env));

    AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env,
            AXIS2_STRDUP(XML_SCHEMA_CONST_EMPTY, env));

    AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env,
            AXIS2_STRDUP(XML_SCHEMA_CONST_EXTENSION, env));

    AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env,
            AXIS2_STRDUP(XML_SCHEMA_CONST_LIST, env));

    AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env,
            AXIS2_STRDUP(XML_SCHEMA_CONST_NONE, env));

    AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env,
            AXIS2_STRDUP(XML_SCHEMA_CONST_RESTRICTION, env));

    AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env,
            AXIS2_STRDUP(XML_SCHEMA_CONST_SUBSTITUTION, env));

    AXIS2_ARRAY_LIST_ADD(derivation_method_impl->members, env,
            AXIS2_STRDUP(XML_SCHEMA_CONST_UNION, env));


    derivation_method_impl->schema_enum =
        xml_schema_enum_create(env, value);
    if (!derivation_method_impl->schema_enum)
    {
        xml_schema_derivation_method_free(
            &(derivation_method_impl->derivation_method), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    derivation_method_impl->ht_super = axis2_hash_make(env);
    axis2_hash_set(derivation_method_impl->ht_super, AXIS2_STRDUP("XML_SCHEMA_DERIVATION_METHOD", env),
            AXIS2_HASH_KEY_STRING, &(derivation_method_impl->derivation_method));

    axis2_hash_set(derivation_method_impl->ht_super, AXIS2_STRDUP("XML_SCHEMA_ENUM", env),
            AXIS2_HASH_KEY_STRING, derivation_method_impl->schema_enum);

    status = xml_schema_enum_resolve_methods(
                &(derivation_method_impl->derivation_method.base),
                env, derivation_method_impl->schema_enum,
                xml_schema_derivation_method_super_objs,
                xml_schema_derivation_method_get_type,
                xml_schema_derivation_method_free);

    return &(derivation_method_impl->derivation_method);
}

axis2_status_t AXIS2_CALL
xml_schema_derivation_method_free(void *derivation_method,
        const axis2_env_t *env)
{
    xml_schema_derivation_method_impl_t *derivation_method_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    derivation_method_impl = AXIS2_INTF_TO_IMPL(derivation_method);

    if (derivation_method_impl->members)
    {
        int size = 0;
        int i    = 0;
        size = AXIS2_ARRAY_LIST_SIZE(derivation_method_impl->members, env);
        for (i = 0; i < size ; i++)
        {
            axis2_char_t *value = NULL;
            value = (axis2_char_t*) AXIS2_ARRAY_LIST_GET(derivation_method_impl->members, env, i);
            if (value)
            {
                AXIS2_FREE(env->allocator, value);
                value = NULL;
            }
        }
        AXIS2_ARRAY_LIST_FREE(derivation_method_impl->members, env);
        derivation_method_impl->members = NULL;
    }

    if (derivation_method_impl->schema_enum)
    {
        XML_SCHEMA_ENUM_FREE(derivation_method_impl->schema_enum, env);
        derivation_method_impl->schema_enum = NULL;
    }

    if (derivation_method_impl->derivation_method.ops)
    {
        AXIS2_FREE(env->allocator, derivation_method_impl->derivation_method.ops);
        derivation_method_impl->derivation_method.ops = NULL;
    }
    if (derivation_method_impl->derivation_method.base.ops)
    {
        AXIS2_FREE(env->allocator, derivation_method_impl->derivation_method.base.ops);
        derivation_method_impl->derivation_method.base.ops = NULL;
    }
    if (derivation_method_impl)
    {
        AXIS2_FREE(env->allocator, derivation_method_impl);
        derivation_method_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

xml_schema_enum_t *AXIS2_CALL
xml_schema_derivation_method_get_base_impl(void *derivation_method,
        const axis2_env_t *env)
{
    xml_schema_derivation_method_impl_t *derivation_method_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    derivation_method_impl = AXIS2_INTF_TO_IMPL(derivation_method);

    return derivation_method_impl->schema_enum;
}

axis2_array_list_t *AXIS2_CALL
xml_schema_derivation_method_get_values(void *derivation_method,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(derivation_method)->members;
}

axis2_hash_t* AXIS2_CALL
xml_schema_derivation_method_super_objs(void *derivation_method,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(derivation_method)->ht_super;
}

xml_schema_types_t AXIS2_CALL
xml_schema_derivation_method_get_type(void *derivation_method,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(derivation_method)->type;
}
