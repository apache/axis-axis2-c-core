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

#include <xml_schema.h>
#include <xml_schema_constants.h>
#include <xml_schema_simple_type.h>
#include <xml_schema_data_type.h>
#include <xml_schema_derivation_method.h>

#include <axis2_qname.h>

typedef struct xml_schema_simple_type_impl
            xml_schema_simple_type_impl_t;

/**
 * @brief xml_schema_simple_type_impl
 */
struct xml_schema_simple_type_impl
{
    xml_schema_simple_type_t simple_type;
    /** parent type */
    xml_schema_type_t *schema_type;

    axis2_hash_t *ht_super;

    xml_schema_types_t obj_type;

    xml_schema_simple_type_content_t *content;
};

#define AXIS2_INTF_TO_IMPL(simple_type) ((xml_schema_simple_type_impl_t *) simple_type)

/************************** function prototypes ******************************/

axis2_status_t AXIS2_CALL
xml_schema_simple_type_free(void *simple_type,
        const axis2_env_t *env);

xml_schema_type_t *AXIS2_CALL
xml_schema_simple_type_get_base_impl(void *simple_type,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
xml_schema_simple_type_super_objs(void *simple_type,
        const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_simple_type_get_type(void *simple_type,
        const axis2_env_t *env);


xml_schema_simple_type_content_t* AXIS2_CALL
xml_schema_simple_type_get_content(void *simple_type,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_simple_type_set_content(void *simple_type,
        const axis2_env_t *env,
        xml_schema_simple_type_content_t *simple_content);

axis2_char_t* AXIS2_CALL
xml_schema_simple_type_to_string(void *simple_type,
        const axis2_env_t *env,
        axis2_char_t *prefix,
        int tab);

/************************** end       ****************************************/

AXIS2_EXTERN xml_schema_simple_type_t * AXIS2_CALL
xml_schema_simple_type_create(const axis2_env_t *env,
        xml_schema_t *schema)
{
    xml_schema_simple_type_impl_t *simple_type = NULL;
    xml_schema_annotated_t *annotated =  NULL;
    axis2_status_t status = AXIS2_FAILURE;

    simple_type = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_simple_type_impl_t));
    if (!simple_type)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    simple_type->content = NULL;
    simple_type->schema_type = NULL;
    simple_type->simple_type.ops = NULL;
    simple_type->simple_type.base.ops = NULL;
    simple_type->ht_super = NULL;
    simple_type->obj_type = XML_SCHEMA_SIMPLE_TYPE;

    simple_type->simple_type.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_simple_type_ops_t));
    if (!simple_type->simple_type.ops)
    {
        xml_schema_simple_type_free(&(simple_type->simple_type), env);
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    simple_type->schema_type = xml_schema_type_create(env, schema);
    if (!simple_type->schema_type)
    {
        xml_schema_simple_type_free(&(simple_type->schema_type), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    simple_type->simple_type.ops->free =
        xml_schema_simple_type_free;

    simple_type->simple_type.ops->get_base_impl =
        xml_schema_simple_type_get_base_impl;

    simple_type->simple_type.ops->get_content =
        xml_schema_simple_type_get_content;

    simple_type->simple_type.ops->set_content =
        xml_schema_simple_type_set_content;

    simple_type->simple_type.ops->get_type =
        xml_schema_simple_type_get_type;

    simple_type->simple_type.ops->super_objs =
        xml_schema_simple_type_super_objs;

    simple_type->ht_super = axis2_hash_make(env);
    if (!simple_type->ht_super)
    {
        xml_schema_simple_type_free(&(simple_type->simple_type), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    axis2_hash_set(simple_type->ht_super,
            AXIS2_STRDUP("XML_SCHEMA_SIMPLE_TYPE", env),
            AXIS2_HASH_KEY_STRING, &(simple_type->simple_type));

    axis2_hash_set(simple_type->ht_super,
            AXIS2_STRDUP("XML_SCHEMA_TYPE", env),
            AXIS2_HASH_KEY_STRING, simple_type->schema_type);

    annotated = XML_SCHEMA_TYPE_GET_BASE_IMPL(simple_type->schema_type, env);
    if (annotated)
    {
        axis2_hash_set(simple_type->ht_super,
                AXIS2_STRDUP("XML_SCHEMA_ANNOTATED", env),
                AXIS2_HASH_KEY_STRING, annotated);
        axis2_hash_set(simple_type->ht_super,
                AXIS2_STRDUP("XML_SCHEMA_OBJ", env),
                AXIS2_HASH_KEY_STRING,
                XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(annotated, env));
    }

    status = xml_schema_type_resolve_methods(&(simple_type->simple_type.base),
            env, simple_type->schema_type,
            xml_schema_simple_type_super_objs,
            xml_schema_simple_type_get_type,
            xml_schema_simple_type_free);

    return &(simple_type->simple_type);
}

axis2_status_t AXIS2_CALL
xml_schema_simple_type_free(void *simple_type,
        const axis2_env_t *env)
{
    xml_schema_simple_type_impl_t *simple_type_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    simple_type_impl = AXIS2_INTF_TO_IMPL(simple_type);

    if (simple_type_impl->ht_super)
    {
        axis2_hash_free(simple_type_impl->ht_super, env);
        simple_type_impl->ht_super = NULL;
    }
    if (simple_type_impl->schema_type)
    {
        XML_SCHEMA_TYPE_FREE(simple_type_impl->schema_type, env);
        simple_type_impl->schema_type = NULL;
    }

    if (simple_type_impl->simple_type.ops)
    {
        AXIS2_FREE(env->allocator, simple_type_impl->simple_type.ops);
        simple_type_impl->simple_type.ops = NULL;
    }
    if (simple_type_impl->simple_type.base.ops)
    {
        AXIS2_FREE(env->allocator, simple_type_impl->simple_type.base.ops);
        simple_type_impl->simple_type.base.ops = NULL;
    }
    AXIS2_FREE(env->allocator, simple_type_impl);
    simple_type_impl = NULL;
    return AXIS2_SUCCESS;
}

xml_schema_type_t *AXIS2_CALL
xml_schema_simple_type_get_base_impl(void *simple_type,
        const axis2_env_t *env)
{
    xml_schema_simple_type_impl_t *simple_type_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    simple_type_impl = AXIS2_INTF_TO_IMPL(simple_type);
    return simple_type_impl->schema_type;

}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
xml_schema_simple_type_resolve_methods(
    xml_schema_simple_type_t *simple_type,
    const axis2_env_t *env,
    xml_schema_simple_type_t *simple_type_impl,
    axis2_hash_t *methods)
{
    xml_schema_simple_type_impl_t *type_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, simple_type, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);

    type_impl_l = (xml_schema_simple_type_impl_t *) simple_type_impl;

    simple_type->ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_simple_type_ops_t));
    if (!simple_type->ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    simple_type->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    simple_type->ops->get_base_impl =
        type_impl_l->simple_type.ops->get_base_impl;
    simple_type->ops->get_content =
        type_impl_l->simple_type.ops->get_content;
    simple_type->ops->set_content =
        type_impl_l->simple_type.ops->set_content;

    return xml_schema_type_resolve_methods(&(simple_type->base),
            env, type_impl_l->schema_type ,
            xml_schema_simple_type_super_objs,
            xml_schema_simple_type_get_type,
            xml_schema_simple_type_free);
}

xml_schema_simple_type_content_t* AXIS2_CALL
xml_schema_simple_type_get_content(void *simple_type,
        const axis2_env_t *env)
{
    xml_schema_simple_type_impl_t *simple_type_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    simple_type_impl = AXIS2_INTF_TO_IMPL(simple_type);
    return simple_type_impl->content;
}

axis2_status_t AXIS2_CALL
xml_schema_simple_type_set_content(void *simple_type,
        const axis2_env_t *env,
        xml_schema_simple_type_content_t *simple_content)
{
    xml_schema_simple_type_impl_t *simple_type_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    simple_type_impl = AXIS2_INTF_TO_IMPL(simple_type);
    if (simple_type_impl->content)
    {
        XML_SCHEMA_SIMPLE_TYPE_CONTENT_FREE(simple_type_impl->content, env);
        simple_type_impl->content   = NULL;
    }
    simple_type_impl->content = simple_content;
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL
xml_schema_simple_type_to_string(void *simple_type,
        const axis2_env_t *env,
        axis2_char_t *prefix,
        int tab)
{
    xml_schema_simple_type_impl_t *simple_type_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    simple_type_impl = AXIS2_INTF_TO_IMPL(simple_type);
    /** TODO implement the method */
    return NULL;
}

axis2_hash_t *AXIS2_CALL
xml_schema_simple_type_super_objs(void *simple_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(simple_type)->ht_super;
}

xml_schema_types_t AXIS2_CALL
xml_schema_simple_type_get_type(void *simple_type,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(simple_type)->obj_type;
}

