/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <xml_schema_form.h>
#include <axis2_utils.h>
typedef struct xml_schema_form_impl
            xml_schema_form_impl_t;

/**
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension
 */
struct xml_schema_form_impl
{
    xml_schema_form_t form;

    xml_schema_enum_t *schema_enum;

    xml_schema_types_t obj_type;

    axis2_hash_t *ht_super;


    axis2_array_list_t *members;
};

#define AXIS2_INTF_TO_IMPL(form) \
        ((xml_schema_form_impl_t *) form)

axis2_status_t AXIS2_CALL
xml_schema_form_free(
    void *form,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
xml_schema_form_super_objs(
    void *form,
    const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_form_get_type(
    void *form,
    const axis2_env_t *env);

xml_schema_enum_t *AXIS2_CALL
xml_schema_form_get_base_impl(void *form,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
xml_schema_form_resolve_methods(
    xml_schema_form_t *form,
    const axis2_env_t *env,
    xml_schema_form_t *form_impl,
    axis2_hash_t *methods);

axis2_array_list_t *AXIS2_CALL
xml_schema_form_get_values(void *form,
        const axis2_env_t *env);


AXIS2_EXTERN xml_schema_form_t * AXIS2_CALL
xml_schema_form_create(const axis2_env_t *env,
        axis2_char_t *value)
{
    xml_schema_form_impl_t *form_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    form_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_form_impl_t));
    if (!form_impl)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    form_impl->schema_enum = NULL;
    form_impl->obj_type = XML_SCHEMA_FORM;
    form_impl->ht_super = NULL;

    form_impl->members = NULL;
    form_impl->form.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_form_ops_t));
    if (!form_impl->form.ops)
    {
        xml_schema_form_free(&(form_impl->form), env);
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }


    form_impl->form.ops->free =
        xml_schema_form_free;

    form_impl->form.ops->super_objs =
        xml_schema_form_super_objs;

    form_impl->form.ops->get_type =
        xml_schema_form_get_type;

    form_impl->form.ops->get_base_impl =
        xml_schema_form_get_base_impl;

    form_impl->form.ops->get_values =
        xml_schema_form_get_values;

    form_impl->members = axis2_array_list_create(env, 0);
    if (!form_impl->members)
    {
        xml_schema_form_free(&(form_impl->form), env);
        return NULL;
    }

    axis2_array_list_add(form_impl->members, env,
            AXIS2_STRDUP(XML_SCHEMA_FORM_QUALIFIED, env));

    axis2_array_list_add(form_impl->members, env,
            AXIS2_STRDUP(XML_SCHEMA_FORM_UNQUALIFIED, env));

    axis2_array_list_add(form_impl->members, env,
            AXIS2_STRDUP(XML_SCHEMA_FORM_NONE, env));

    form_impl->schema_enum = xml_schema_enum_create(env, value);
    if (!form_impl->schema_enum)
    {
        xml_schema_form_free(&(form_impl->form), env);
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    form_impl->ht_super = axis2_hash_make(env);

    if (!form_impl->ht_super)
    {
        xml_schema_form_free(&(form_impl->form), env);
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    axis2_hash_set(form_impl->ht_super, AXIS2_STRDUP("XML_SCHEMA_FORM", env),
            AXIS2_HASH_KEY_STRING, &(form_impl->form));

    axis2_hash_set(form_impl->ht_super, AXIS2_STRDUP("XML_SCHEMA_ENUM", env),
            AXIS2_HASH_KEY_STRING, form_impl->schema_enum);

    status = xml_schema_enum_resolve_methods(
                &(form_impl->form.base), env, form_impl->schema_enum,
                xml_schema_form_super_objs,
                xml_schema_form_get_type,
                xml_schema_form_free);

    return &(form_impl->form);
}

axis2_status_t AXIS2_CALL
xml_schema_form_free(void *form,
        const axis2_env_t *env)
{
    xml_schema_form_impl_t *form_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    form_impl = AXIS2_INTF_TO_IMPL(form);

    if (form_impl->members)
    {
        int size = 0;
        int i    = 0;
        size = axis2_array_list_size(form_impl->members, env);
        for (i = 0; i < size ; i++)
        {
            axis2_char_t *value = NULL;
            value = (axis2_char_t*) axis2_array_list_get(form_impl->members, env, i);
            if (value)
            {
                AXIS2_FREE(env->allocator, value);
                value = NULL;
            }
        }
        axis2_array_list_free(form_impl->members, env);
        form_impl->members = NULL;
    }

    if (form_impl->ht_super)
    {
        axis2_hash_free(form_impl->ht_super, env);
        form_impl->ht_super = NULL;
    }


    if (form_impl->schema_enum)
    {
        XML_SCHEMA_ENUM_FREE(form_impl->schema_enum, env);
        form_impl->schema_enum = NULL;
    }
    if (form_impl->form.base.ops)
    {
        AXIS2_FREE(env->allocator, form_impl->form.base.ops);
        form_impl->form.base.ops = NULL;

    }
    if (form_impl->form.ops)
    {
        AXIS2_FREE(env->allocator, form_impl->form.ops);
        form_impl->form.ops = NULL;
    }

    if (form_impl)
    {
        AXIS2_FREE(env->allocator, form_impl);
        form_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
xml_schema_form_super_objs(void *form,
        const axis2_env_t *env)
{
    xml_schema_form_impl_t *form_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    form_impl = AXIS2_INTF_TO_IMPL(form);
    return form_impl->ht_super;
}

xml_schema_types_t AXIS2_CALL
xml_schema_form_get_type(void *form,
        const axis2_env_t *env)
{
    xml_schema_form_impl_t *form_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    form_impl = AXIS2_INTF_TO_IMPL(form);
    return form_impl->obj_type;
}

xml_schema_enum_t *AXIS2_CALL
xml_schema_form_get_base_impl(void *form,
        const axis2_env_t *env)
{
    xml_schema_form_impl_t *form_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    form_impl = AXIS2_INTF_TO_IMPL(form);
    return form_impl->schema_enum;
}


axis2_array_list_t* AXIS2_CALL
xml_schema_form_get_values(void *form,
        const axis2_env_t *env)
{
    xml_schema_form_impl_t *form_impl = NULL;
    axis2_hash_t *super = NULL;

    super = XML_SCHEMA_FORM_SUPER_OBJS(form, env);
    form_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super,
            "XML_SCHEMA_FORM", AXIS2_HASH_KEY_STRING));
    return form_impl->members;
}

