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

#include <xml_schema_group.h>
#include <xml_schema_group_base.h>
#include <axutil_utils.h>

typedef struct xml_schema_group_impl
            xml_schema_group_impl_t;

/**
 * @brief xml_schema_group_impl
 */
struct xml_schema_group_impl
{
    xml_schema_group_t group;
    xml_schema_annotated_t *annotated;
    xml_schema_types_t obj_type;
    axutil_hash_t *ht_super;
    axis2_char_t *name;
    xml_schema_group_base_t *particle;
};

#define AXIS2_INTF_TO_IMPL(group) ((xml_schema_group_impl_t *) group)

axis2_status_t AXIS2_CALL
xml_schema_group_free(
    void *group,
    const axutil_env_t *env);

axutil_hash_t *AXIS2_CALL
xml_schema_group_super_objs(
    void *group,
    const axutil_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_group_get_type(
    void *group,
    const axutil_env_t *env);

xml_schema_annotated_t *AXIS2_CALL
xml_schema_group_get_base_impl(void *group,
        const axutil_env_t *env);

axis2_char_t *AXIS2_CALL
xml_schema_group_get_name(void *group,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_group_set_name(void *group,
        const axutil_env_t *env,
        axis2_char_t *name);

xml_schema_group_base_t *AXIS2_CALL
xml_schema_group_get_particle(void *group,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_group_set_particle(void *group,
        const axutil_env_t *env,
        xml_schema_group_base_t *particle);

AXIS2_EXTERN xml_schema_group_t * AXIS2_CALL
xml_schema_group_create(const axutil_env_t *env)
{
    xml_schema_group_impl_t *group_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    group_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_group_impl_t));
    if (!group_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    group_impl->annotated = NULL;
    group_impl->obj_type = XML_SCHEMA_GROUP;
    group_impl->ht_super = NULL;
    group_impl->name = NULL;
    group_impl->particle = NULL;
    group_impl->group.base.ops = NULL;
    group_impl->group.ops = NULL;

    group_impl->group.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_group_ops_t));
    if (!group_impl->group.ops)
    {
        xml_schema_group_free(&(group_impl->group), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }


    group_impl->group.ops->free =
        xml_schema_group_free;
    group_impl->group.ops->super_objs =
        xml_schema_group_super_objs;
    group_impl->group.ops->get_type =
        xml_schema_group_get_type;
    group_impl->group.ops->get_base_impl =
        xml_schema_group_get_base_impl;
    group_impl->group.ops->get_name =
        xml_schema_group_get_name;
    group_impl->group.ops->set_name =
        xml_schema_group_set_name;
    group_impl->group.ops->get_particle =
        xml_schema_group_get_particle;
    group_impl->group.ops->set_particle =
        xml_schema_group_set_particle;

    group_impl->annotated = xml_schema_annotated_create(env);
    if (!group_impl->annotated)
    {
        xml_schema_group_free(&(group_impl->group), env);
        return NULL;
    }
    group_impl->ht_super = axutil_hash_make(env);
    if (!group_impl->ht_super)
    {
        xml_schema_group_free(&(group_impl->group), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axutil_hash_set(group_impl->ht_super, axis2_strdup(env, "XML_SCHEMA_GROUP"), AXIS2_HASH_KEY_STRING,
            &(group_impl->group));
    axutil_hash_set(group_impl->ht_super, axis2_strdup(env, "XML_SCHEMA_ANNOTATED"), AXIS2_HASH_KEY_STRING,
            group_impl->annotated);
    axutil_hash_set(group_impl->ht_super, axis2_strdup(env, "XML_SCHEMA_OBJ"), AXIS2_HASH_KEY_STRING,
            XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(group_impl->annotated, env));


    status = xml_schema_annotated_resolve_methods(
                &(group_impl->group.base), env, group_impl->annotated,
                xml_schema_group_super_objs,
                xml_schema_group_get_type,
                xml_schema_group_free);
    return &(group_impl->group);
}

axis2_status_t AXIS2_CALL
xml_schema_group_free(void *group,
        const axutil_env_t *env)
{
    xml_schema_group_impl_t *group_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    group_impl = AXIS2_INTF_TO_IMPL(group);

    if (group_impl->name)
    {
        AXIS2_FREE(env->allocator, group_impl->name);
        group_impl->name = NULL;
    }

    if (group_impl->particle)
    {
        XML_SCHEMA_PARTICLE_FREE(group_impl->particle, env);
        group_impl->particle = NULL;
    }

    if (group_impl->ht_super)
    {
        axutil_hash_free(group_impl->ht_super, env);
        group_impl->ht_super = NULL;
    }


    if (group_impl->annotated)
    {
        XML_SCHEMA_ANNOTATED_FREE(group_impl->annotated, env);
        group_impl->annotated = NULL;
    }

    if (group_impl->group.ops)
    {
        AXIS2_FREE(env->allocator, group_impl->group.ops);
        group_impl->group.ops = NULL;
    }
    if (group_impl->group.base.ops)
    {
        AXIS2_FREE(env->allocator, group_impl->group.base.ops);
        group_impl->group.base.ops = NULL;
    }
    AXIS2_FREE(env->allocator, group_impl);
    group_impl = NULL;
    return AXIS2_SUCCESS;
}

axutil_hash_t *AXIS2_CALL
xml_schema_group_super_objs(void *group,
        const axutil_env_t *env)
{
    xml_schema_group_impl_t *group_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    group_impl = AXIS2_INTF_TO_IMPL(group);
    return group_impl->ht_super;
}

xml_schema_types_t AXIS2_CALL
xml_schema_group_get_type(void *group,
        const axutil_env_t *env)
{
    xml_schema_group_impl_t *group_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    group_impl = AXIS2_INTF_TO_IMPL(group);
    return group_impl->obj_type;
}

xml_schema_annotated_t *AXIS2_CALL
xml_schema_group_get_base_impl(void *group,
        const axutil_env_t *env)
{
    xml_schema_group_impl_t *group_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    group_impl = AXIS2_INTF_TO_IMPL(group);
    return group_impl->annotated;
}

axis2_char_t *AXIS2_CALL
xml_schema_group_get_name(void *group,
        const axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(group)->name;
}

axis2_status_t AXIS2_CALL
xml_schema_group_set_name(void *group,
        const axutil_env_t *env,
        axis2_char_t *name)
{
    xml_schema_group_impl_t *group_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    group_impl = AXIS2_INTF_TO_IMPL(group);
    if (group_impl->name)
    {
        AXIS2_FREE(env->allocator, group_impl->name);
        group_impl->name = NULL;
    }
    group_impl->name = axis2_strdup(env, name);
    if (!group_impl->name)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

xml_schema_group_base_t *AXIS2_CALL
xml_schema_group_get_particle(void *group,
        const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(group)->particle;
}

axis2_status_t AXIS2_CALL
xml_schema_group_set_particle(void *group,
        const axutil_env_t *env,
        xml_schema_group_base_t *particle)
{
    xml_schema_group_impl_t *group_impl = NULL;
    group_impl = AXIS2_INTF_TO_IMPL(group);
    if (group_impl->particle)
    {
        group_impl->particle = NULL;
    }
    group_impl->particle = particle;
    return AXIS2_SUCCESS;
}
