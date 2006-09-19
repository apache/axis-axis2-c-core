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

#include <xml_schema_group_ref.h>
#include <xml_schema_group_base.h>
#include <axis2_utils.h>

typedef struct xml_schema_group_ref_impl
            xml_schema_group_ref_impl_t;

/**
 * @brief xml_schema_group_ref_impl
 *
 */
struct xml_schema_group_ref_impl
{
    xml_schema_group_ref_t group_ref;

    xml_schema_particle_t *base;

    xml_schema_types_t obj_type;

    axis2_hash_t *ht_super;

    axis2_qname_t *ref_qname;

    void *particle;
};

#define AXIS2_INTF_TO_IMPL(group_ref) \
        ((xml_schema_group_ref_impl_t *) group_ref)

axis2_status_t AXIS2_CALL
xml_schema_group_ref_free(
    void *group_ref,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
xml_schema_group_ref_super_objs(
    void *group_ref,
    const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_group_ref_get_type(
    void *group_ref,
    const axis2_env_t *env);

xml_schema_particle_t *AXIS2_CALL
xml_schema_group_ref_get_base_impl(
    void *group_ref,
    const axis2_env_t *env);

axis2_qname_t *AXIS2_CALL
xml_schema_group_ref_get_ref_qname(
    void *group_ref,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_group_ref_set_ref_qname(
    void *group_ref,
    const axis2_env_t *env,
    axis2_qname_t *ref_qname);

void *AXIS2_CALL
xml_schema_group_ref_get_particle(
    void *group_ref,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_group_ref_set_particle(
    void *group_ref,
    const axis2_env_t *env,
    void *particle);


AXIS2_EXTERN xml_schema_group_ref_t * AXIS2_CALL
xml_schema_group_ref_create(const axis2_env_t *env)
{
    xml_schema_group_ref_impl_t *group_ref_impl = NULL;
    xml_schema_annotated_t *annotated = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    group_ref_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_group_ref_impl_t));
    if (!group_ref_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    group_ref_impl->group_ref.base.ops = NULL;
    group_ref_impl->base = NULL;
    group_ref_impl->obj_type = XML_SCHEMA_GROUP_REF;
    group_ref_impl->ht_super = NULL;
    group_ref_impl->ref_qname = NULL;
    group_ref_impl->particle = NULL;
    group_ref_impl->group_ref.ops = NULL;

    group_ref_impl->group_ref.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_group_ref_ops_t));

    if (!group_ref_impl->group_ref.ops)
    {
        xml_schema_group_ref_free(&(group_ref_impl->group_ref), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    group_ref_impl->group_ref.ops->free =
        xml_schema_group_ref_free;
    group_ref_impl->group_ref.ops->super_objs =
        xml_schema_group_ref_super_objs;
    group_ref_impl->group_ref.ops->get_type =
        xml_schema_group_ref_get_type;

    group_ref_impl->group_ref.ops->get_base_impl =
        xml_schema_group_ref_get_base_impl;

    group_ref_impl->group_ref.ops->get_ref_qname =
        xml_schema_group_ref_get_ref_qname;

    group_ref_impl->group_ref.ops->set_ref_qname =
        xml_schema_group_ref_set_ref_qname;

    group_ref_impl->group_ref.ops->get_particle =
        xml_schema_group_ref_get_particle;

    group_ref_impl->group_ref.ops->set_particle =
        xml_schema_group_ref_set_particle;

    group_ref_impl->base = xml_schema_particle_create(env);
    if (!group_ref_impl->base)
    {
        xml_schema_group_ref_free(&(group_ref_impl->group_ref), env);
        return NULL;
    }
    group_ref_impl->ht_super = axis2_hash_make(env);
    if (!group_ref_impl->ht_super)
    {
        xml_schema_group_ref_free(&(group_ref_impl->group_ref), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    axis2_hash_set(group_ref_impl->ht_super,
            AXIS2_STRDUP("XML_SCHEMA_GROUP_REF", env),
            AXIS2_HASH_KEY_STRING, &(group_ref_impl->group_ref));

    axis2_hash_set(group_ref_impl->ht_super, AXIS2_STRDUP("XML_SCHEMA_PARTICLE", env),
            AXIS2_HASH_KEY_STRING, group_ref_impl->particle);

    annotated = XML_SCHEMA_PARTICLE_GET_BASE_IMPL(group_ref_impl->particle, env);
    if (annotated)
    {
        axis2_hash_set(group_ref_impl->ht_super, AXIS2_STRDUP("XML_SCHEMA_ANNOTATED", env),
                AXIS2_HASH_KEY_STRING, annotated);
        axis2_hash_set(group_ref_impl->ht_super, AXIS2_STRDUP("XML_SCHEMA_OBJ", env),
                AXIS2_HASH_KEY_STRING, XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(annotated, env));
    }

    status = xml_schema_particle_resolve_methods(
                &(group_ref_impl->group_ref.base), env, group_ref_impl->base,
                xml_schema_group_ref_super_objs,
                xml_schema_group_ref_get_type,
                xml_schema_group_ref_free);

    return &(group_ref_impl->group_ref);
}

axis2_status_t AXIS2_CALL
xml_schema_group_ref_free(void *group_ref,
        const axis2_env_t *env)
{
    xml_schema_group_ref_impl_t *group_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    group_ref_impl = AXIS2_INTF_TO_IMPL(group_ref);

    if (group_ref_impl->ref_qname)
    {
        AXIS2_FREE(env->allocator, group_ref_impl->ref_qname);
        group_ref_impl->ref_qname = NULL;
    }

    if (group_ref_impl->particle)
    {
        XML_SCHEMA_GROUP_BASE_FREE(group_ref_impl->particle, env);
        group_ref_impl->particle = NULL;
    }

    if (group_ref_impl->ht_super)
    {
        axis2_hash_free(group_ref_impl->ht_super, env);
        group_ref_impl->ht_super = NULL;
    }

    if (group_ref_impl->base)
    {
        XML_SCHEMA_PARTICLE_FREE(group_ref_impl->base, env);
        group_ref_impl->base = NULL;
    }

    if (group_ref_impl->group_ref.base.ops)
    {
        AXIS2_FREE(env->allocator, group_ref_impl->group_ref.base.ops);
        group_ref_impl->group_ref.base.ops = NULL;
    }
    if (group_ref_impl->group_ref.ops)
    {
        AXIS2_FREE(env->allocator, group_ref_impl->group_ref.ops);
        group_ref_impl->group_ref.ops = NULL;
    }
    AXIS2_FREE(env->allocator, group_ref_impl);
    group_ref_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
xml_schema_group_ref_super_objs(
    void *group_ref,
    const axis2_env_t *env)
{
    xml_schema_group_ref_impl_t *group_ref_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    group_ref_impl = AXIS2_INTF_TO_IMPL(group_ref);
    return group_ref_impl->ht_super;
}

xml_schema_types_t AXIS2_CALL
xml_schema_group_ref_get_type(
    void *group_ref,
    const axis2_env_t *env)
{
    xml_schema_group_ref_impl_t *group_ref_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    group_ref_impl = AXIS2_INTF_TO_IMPL(group_ref);
    return group_ref_impl->obj_type;
}

xml_schema_particle_t *AXIS2_CALL
xml_schema_group_ref_get_base_impl(void *group_ref,
        const axis2_env_t *env)
{
    xml_schema_group_ref_impl_t *group_ref_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    group_ref_impl = AXIS2_INTF_TO_IMPL(group_ref);
    return group_ref_impl->base;
}

axis2_qname_t *AXIS2_CALL
xml_schema_group_ref_get_ref_qname(void *group_ref,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(group_ref)->ref_qname;
}

axis2_status_t AXIS2_CALL
xml_schema_group_ref_set_ref_qname(void *group_ref,
        const axis2_env_t *env,
        axis2_qname_t *ref_qname)
{
    xml_schema_group_ref_impl_t *group_ref_impl = NULL;
    if (group_ref_impl->ref_qname)
    {
        AXIS2_QNAME_FREE(group_ref_impl->ref_qname, env);
        group_ref_impl->ref_qname = NULL;
    }
    group_ref_impl->ref_qname = AXIS2_QNAME_CLONE(ref_qname, env);
    if (!group_ref_impl->ref_qname)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
xml_schema_group_ref_get_particle(
    void *group_ref,
    const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(group_ref)->particle;
}

axis2_status_t AXIS2_CALL
xml_schema_group_ref_set_particle(
    void *group_ref,
    const axis2_env_t *env,
    void *particle)
{
    xml_schema_group_ref_impl_t *grp_ref_impl = NULL;

    grp_ref_impl = AXIS2_INTF_TO_IMPL(group_ref);

    if (grp_ref_impl->particle)
    {
        /** TODO free */
    }
    grp_ref_impl->particle = particle;
    return AXIS2_SUCCESS;
}

