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

#include <xml_schema_external.h>
#include <xml_schema.h>

/**
 * @brief xml_schema_external_impl
 */
typedef struct xml_schema_external_impl
{
    xml_schema_external_t external;

    xml_schema_annotated_t *annotated;

    xml_schema_t *schema;

    xml_schema_types_t obj_type;

    axis2_hash_t *ht_super;

    axis2_char_t *schema_location;

}
xml_schema_external_impl_t;

#define AXIS2_INTF_TO_IMPL(external) \
        ((xml_schema_external_impl_t *) external)

/********************* function prototypes ***********************************/

axis2_status_t AXIS2_CALL
xml_schema_external_free(void *external,
        const axis2_env_t *env);

xml_schema_annotated_t *AXIS2_CALL
xml_schema_external_get_base_impl(void *external,
        const axis2_env_t *env);

axis2_hash_t* AXIS2_CALL
xml_schema_external_super_objs(void *external,
        const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_external_get_type(void *external,
        const axis2_env_t *env);


xml_schema_t* AXIS2_CALL
xml_schema_external_get_schema(void *external,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_external_set_schema(void *external,
        const axis2_env_t *env,
        xml_schema_t *schema);

axis2_char_t* AXIS2_CALL
xml_schema_external_get_schema_location(void *external,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_external_set_schema_location(void *external,
        const axis2_env_t *env,
        axis2_char_t* location);


/********************* end function prototypes ********************************/

AXIS2_EXTERN xml_schema_external_t * AXIS2_CALL
xml_schema_external_create(const axis2_env_t *env)
{
    xml_schema_external_impl_t *external_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    external_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_external_impl_t));
    if (!external_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    external_impl->schema = NULL;
    external_impl->annotated = NULL;
    external_impl->external.ops = NULL;
    external_impl->external.base.ops = NULL;
    external_impl->ht_super = NULL;
    external_impl->schema_location = NULL;
    external_impl->obj_type = XML_SCHEMA_EXTERNAL;

    external_impl->external.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_external_ops_t));

    if (!external_impl->external.ops)
    {
        xml_schema_external_free(&(external_impl->external), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    external_impl->external.ops->free =
        xml_schema_external_free;
    external_impl->external.ops->get_base_impl =
        xml_schema_external_get_base_impl;
    external_impl->external.ops->get_type =
        xml_schema_external_get_type;
    external_impl->external.ops->super_objs =
        xml_schema_external_super_objs;
    external_impl->external.ops->get_schema =
        xml_schema_external_get_schema;
    external_impl->external.ops->set_schema =
        xml_schema_external_set_schema;
    external_impl->external.ops->get_schema_location =
        xml_schema_external_get_schema_location;
    external_impl->external.ops->set_schema_location =
        xml_schema_external_set_schema_location;

    external_impl->ht_super = axis2_hash_make(env);

    if (!external_impl->ht_super)
    {
        xml_schema_external_free(&(external_impl->external), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    external_impl->annotated = xml_schema_annotated_create(env);

    if (!external_impl->annotated)
    {
        xml_schema_external_free(&(external_impl->external), env);
        return NULL;
    }

    axis2_hash_set(external_impl->ht_super, AXIS2_STRDUP("XML_SCHEMA_EXTERNAL", env),
            AXIS2_HASH_KEY_STRING, &(external_impl->external));
    axis2_hash_set(external_impl->ht_super, AXIS2_STRDUP("XML_SCHEMA_ANNOTATED", env),
            AXIS2_HASH_KEY_STRING, external_impl->annotated);
    axis2_hash_set(external_impl->ht_super, AXIS2_STRDUP("XML_SCHEMA_OBJ", env),
            AXIS2_HASH_KEY_STRING,
            XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(external_impl->annotated, env));

    status = xml_schema_annotated_resolve_methods(
                &(external_impl->external.base), env, external_impl->annotated,
                xml_schema_external_super_objs,
                xml_schema_external_get_type,
                xml_schema_external_free);

    return &(external_impl->external);
}

AXIS2_EXTERN xml_schema_external_t * AXIS2_CALL
xml_schema_include_create(const axis2_env_t *env)
{
    xml_schema_external_t *include = NULL;
    include = xml_schema_external_create(env);
    AXIS2_INTF_TO_IMPL(include)->obj_type = XML_SCHEMA_INCLUDE;
    return include;
}

axis2_status_t AXIS2_CALL
xml_schema_external_free(void *external,
        const axis2_env_t *env)
{
    xml_schema_external_impl_t *external_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    external_impl = AXIS2_INTF_TO_IMPL(external);

    if (external_impl->ht_super)
    {
        axis2_hash_free(external_impl->ht_super, env);
        external_impl->ht_super = NULL;
    }
    if (external_impl->annotated)
    {
        XML_SCHEMA_ANNOTATED_FREE(external_impl->annotated, env);
        external_impl->annotated = NULL;
    }

    if (external_impl->external.ops)
    {
        AXIS2_FREE(env->allocator, external_impl->external.ops);
        external_impl->external.ops = NULL;
    }
    if (external_impl->external.base.ops)
    {
        AXIS2_FREE(env->allocator, external_impl->external.base.ops);
        external_impl->external.base.ops = NULL;
    }
    AXIS2_FREE(env->allocator, external_impl);
    external_impl = NULL;
    return AXIS2_SUCCESS;
}

xml_schema_annotated_t *AXIS2_CALL
xml_schema_external_get_base_impl(void *external,
        const axis2_env_t *env)
{
    xml_schema_external_impl_t *external_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    external_impl = AXIS2_INTF_TO_IMPL(external);

    return external_impl->annotated;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
xml_schema_external_resolve_methods(
    xml_schema_external_t *external,
    const axis2_env_t *env,
    xml_schema_external_t *external_impl,
    XML_SCHEMA_SUPER_OBJS_FN super_objs,
    XML_SCHEMA_GET_TYPE_FN get_type,
    XML_SCHEMA_FREE_FN free_fn)
{
    xml_schema_external_impl_t *sch_ext_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, external_impl, AXIS2_FAILURE);

    sch_ext_impl = (xml_schema_external_impl_t *) external_impl;

    external->ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_external_ops_t));

    external->ops->free = free_fn;
    external->ops->super_objs = super_objs;
    external->ops->get_type = get_type;

    external->ops->set_schema =
        sch_ext_impl->external.ops->set_schema;
    external->ops->get_schema =
        sch_ext_impl->external.ops->get_schema;
    external->ops->set_schema_location =
        sch_ext_impl->external.ops->set_schema_location;
    external->ops->get_schema_location =
        sch_ext_impl->external.ops->get_schema_location;
    return xml_schema_annotated_resolve_methods(&(external->base),
            env, sch_ext_impl->annotated, super_objs, get_type, free_fn);
}

xml_schema_t * AXIS2_CALL
xml_schema_external_get_schema(void *external,
        const axis2_env_t *env)
{
    xml_schema_external_impl_t *ext_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_EXTERNAL_SUPER_OBJS(external, env);
    if (ht_super)
    {

        ext_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super,
                "XML_SCHEMA_EXTERNAL", AXIS2_HASH_KEY_STRING));
        if (!ext_impl)
            return NULL;
    }
    return ext_impl->schema;
}

axis2_status_t AXIS2_CALL
xml_schema_external_set_schema(void *external,
        const axis2_env_t *env,
        xml_schema_t *schema)
{
    xml_schema_external_impl_t *ext_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = XML_SCHEMA_EXTERNAL_SUPER_OBJS(external, env);
    if (ht_super)
    {

        ext_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super,
                "XML_SCHEMA_EXTERNAL", AXIS2_HASH_KEY_STRING));
        if (!ext_impl)
            return AXIS2_FAILURE;
    }
    if (ext_impl->schema)
    {
        /*
        XML_SCHEMA_FREE(ext_impl->schema, env);
        ext_impl->schema = NULL;
        */
    }
    ext_impl->schema = schema;
    return AXIS2_SUCCESS;
}

axis2_hash_t* AXIS2_CALL
xml_schema_external_super_objs(void *external,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(external)->ht_super;
}

xml_schema_types_t AXIS2_CALL
xml_schema_external_get_type(void *external,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(external)->obj_type;
}

axis2_char_t* AXIS2_CALL
xml_schema_external_get_schema_location(void *external,
        const axis2_env_t *env)
{
    xml_schema_external_impl_t *ext_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_EXTERNAL_SUPER_OBJS(external, env);
    if (ht_super)
    {

        ext_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super,
                "XML_SCHEMA_EXTERNAL", AXIS2_HASH_KEY_STRING));
        if (!ext_impl)
            return NULL;
    }
    return ext_impl->schema_location;
}

axis2_status_t AXIS2_CALL
xml_schema_external_set_schema_location(void *external,
        const axis2_env_t *env,
        axis2_char_t* location)
{
    xml_schema_external_impl_t *ext_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, location, AXIS2_FAILURE);
    ht_super = XML_SCHEMA_EXTERNAL_SUPER_OBJS(external, env);
    if (ht_super)
    {

        ext_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super,
                "XML_SCHEMA_EXTERNAL", AXIS2_HASH_KEY_STRING));
        if (!ext_impl)
            return AXIS2_FAILURE;
    }
    if (ext_impl->schema_location)
    {
        AXIS2_FREE(env->allocator, ext_impl->schema_location);
        ext_impl->schema_location = NULL;
    }
    ext_impl->schema_location = AXIS2_STRDUP(location, env);
    return AXIS2_SUCCESS;
}

