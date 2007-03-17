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

#include <xml_schema_group_base.h>
#include <axis2_utils.h>

typedef struct xml_schema_group_base_impl
            xml_schema_group_base_impl_t;

/**
 * @brief group base Struct Impl
 *   Axis2 Other Extension
 */
struct xml_schema_group_base_impl
{
    xml_schema_group_base_t group_base;

    xml_schema_particle_t *particle;

    xml_schema_types_t obj_type;

    axis2_hash_t *ht_super;

    xml_schema_obj_collection_t *items;
};

#define AXIS2_INTF_TO_IMPL(group_base) \
        ((xml_schema_group_base_impl_t *) group_base)

/****************** *****************************************************/
axis2_status_t AXIS2_CALL
xml_schema_group_base_free(
    void *group_base,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
xml_schema_group_base_super_objs(
    void *group_base,
    const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_group_base_get_type(
    void *group_base,
    const axis2_env_t *env);

xml_schema_particle_t *AXIS2_CALL
xml_schema_group_base_get_base_impl(void *group_base,
        const axis2_env_t *env);

xml_schema_obj_collection_t *AXIS2_CALL
xml_schema_group_base_get_items(void *group_base,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_group_base_to_string(void *group_base,
        const axis2_env_t *env,
        axis2_char_t *prefix,
        int tab);

/***********************************************************************/

AXIS2_EXTERN xml_schema_group_base_t * AXIS2_CALL
xml_schema_group_base_create(const axis2_env_t *env)
{
    xml_schema_group_base_impl_t *group_base_impl = NULL;
    xml_schema_annotated_t *annotated = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    group_base_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_group_base_impl_t));
    if (!group_base_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    group_base_impl->particle = NULL;
    group_base_impl->obj_type = XML_SCHEMA_GROUP_BASE;
    group_base_impl->ht_super = NULL;
    group_base_impl->items = NULL;
    group_base_impl->group_base.ops = NULL;
    group_base_impl->group_base.base.ops = NULL;

    group_base_impl->group_base.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_group_base_ops_t));
    if (!group_base_impl->group_base.ops)
    {
        xml_schema_group_base_free(&(group_base_impl->group_base), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    group_base_impl->group_base.ops->free =
        xml_schema_group_base_free;

    group_base_impl->group_base.ops->super_objs =
        xml_schema_group_base_super_objs;

    group_base_impl->group_base.ops->get_type =
        xml_schema_group_base_get_type;

    group_base_impl->group_base.ops->get_base_impl =
        xml_schema_group_base_get_base_impl;

    group_base_impl->group_base.ops->get_items =
        xml_schema_group_base_get_items;

    group_base_impl->group_base.ops->to_string =
        xml_schema_group_base_to_string;

    group_base_impl->particle = xml_schema_particle_create(env);
    if (!group_base_impl->particle)
    {
        xml_schema_group_base_free(&(group_base_impl->group_base), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    group_base_impl->ht_super = axis2_hash_make(env);
    if (!group_base_impl->ht_super)
    {
        xml_schema_group_base_free(&(group_base_impl->group_base), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(group_base_impl->ht_super, axis2_strdup("XML_SCHEMA_GROUP_BASE", env),
            AXIS2_HASH_KEY_STRING, &(group_base_impl->group_base));

    axis2_hash_set(group_base_impl->ht_super, axis2_strdup("XML_SCHEMA_PARTICLE", env),
            AXIS2_HASH_KEY_STRING, group_base_impl->particle);

    annotated = XML_SCHEMA_PARTICLE_GET_BASE_IMPL(group_base_impl->particle, env);
    if (annotated)
    {
        axis2_hash_set(group_base_impl->ht_super, axis2_strdup("XML_SCHEMA_ANNOTATED", env),
                AXIS2_HASH_KEY_STRING, annotated);
        axis2_hash_set(group_base_impl->ht_super, axis2_strdup("XML_SCHEMA_OBJ", env),
                AXIS2_HASH_KEY_STRING, XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(annotated, env));
    }

    group_base_impl->items = xml_schema_obj_collection_create(env);

    status = xml_schema_particle_resolve_methods(
                &(group_base_impl->group_base.base), env, group_base_impl->particle,
                xml_schema_group_base_super_objs,
                xml_schema_group_base_get_type,
                xml_schema_group_base_free);
    return &(group_base_impl->group_base);
}


AXIS2_EXTERN xml_schema_group_base_t * AXIS2_CALL
xml_schema_all_create(const axis2_env_t *env)
{
    xml_schema_group_base_t *grp_base = NULL;
    grp_base = xml_schema_group_base_create(env);
    AXIS2_INTF_TO_IMPL(grp_base)->obj_type = XML_SCHEMA_ALL;
    return grp_base;
}

AXIS2_EXTERN xml_schema_group_base_t * AXIS2_CALL
xml_schema_sequence_create(const axis2_env_t *env)
{
    xml_schema_group_base_t *grp_base = NULL;
    grp_base = xml_schema_group_base_create(env);
    AXIS2_INTF_TO_IMPL(grp_base)->obj_type = XML_SCHEMA_SEQUENCE;
    return grp_base;
}

AXIS2_EXTERN xml_schema_group_base_t * AXIS2_CALL
xml_schema_choice_create(const axis2_env_t *env)
{
    xml_schema_group_base_t *grp_base = NULL;
    grp_base = xml_schema_group_base_create(env);
    AXIS2_INTF_TO_IMPL(grp_base)->obj_type = XML_SCHEMA_CHOICE;
    return grp_base;
}


axis2_status_t AXIS2_CALL
xml_schema_group_base_free(void *group_base,
        const axis2_env_t *env)
{
    xml_schema_group_base_impl_t *group_base_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    group_base_impl = AXIS2_INTF_TO_IMPL(group_base);

    if (group_base_impl->items)
    {
        XML_SCHEMA_OBJ_COLLECTION_FREE(group_base_impl->items, env);
        group_base_impl->items = NULL;
    }

    if (group_base_impl->ht_super)
    {
        axis2_hash_free(group_base_impl->ht_super, env);
        group_base_impl->ht_super = NULL;
    }

    if (group_base_impl->particle)
    {
        XML_SCHEMA_PARTICLE_FREE(group_base_impl->particle, env);
        group_base_impl->particle = NULL;
    }

    if (group_base_impl->group_base.ops)
    {
        AXIS2_FREE(env->allocator, group_base_impl->group_base.ops);
        group_base_impl->group_base.ops = NULL;
    }
    if (group_base_impl->group_base.base.ops)
    {
        AXIS2_FREE(env->allocator, group_base_impl->group_base.base.ops);
        group_base_impl->group_base.base.ops = NULL;
    }
    if (group_base_impl)
    {
        AXIS2_FREE(env->allocator, group_base_impl);
        group_base_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
xml_schema_group_base_super_objs(
    void *group_base,
    const axis2_env_t *env)
{
    xml_schema_group_base_impl_t *group_base_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    group_base_impl = AXIS2_INTF_TO_IMPL(group_base);

    return group_base_impl->ht_super;
}

xml_schema_types_t AXIS2_CALL
xml_schema_group_base_get_type(
    void *group_base,
    const axis2_env_t *env)
{
    xml_schema_group_base_impl_t *group_base_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    group_base_impl = AXIS2_INTF_TO_IMPL(group_base);
    return group_base_impl->obj_type;
}

xml_schema_particle_t *AXIS2_CALL
xml_schema_group_base_get_base_impl(
    void *group_base,
    const axis2_env_t *env)
{
    xml_schema_group_base_impl_t *group_base_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    group_base_impl = AXIS2_INTF_TO_IMPL(group_base);
    return group_base_impl->particle;
}

xml_schema_obj_collection_t* AXIS2_CALL
xml_schema_group_base_get_items(void *group_base,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(group_base)->items;
}

axis2_status_t AXIS2_CALL
xml_schema_group_base_to_string(void *group_base,
        const axis2_env_t *env,
        axis2_char_t *prefix,
        int tab)
{
    /** TODO */
    return AXIS2_SUCCESS;
}

