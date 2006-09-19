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

#include <xml_schema_simple_type_content.h>
#include <axis2_utils.h>

typedef struct xml_schema_simple_type_content_impl
            xml_schema_simple_type_content_impl_t;

/**
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension
 */
struct xml_schema_simple_type_content_impl
{
    xml_schema_simple_type_content_t sim_type_cont;

    xml_schema_annotated_t *annotated;

    axis2_hash_t *ht_super;

    xml_schema_types_t obj_type;
};

#define AXIS2_INTF_TO_IMPL(sim_type_cont) \
        ((xml_schema_simple_type_content_impl_t *) sim_type_cont)

/*************** function prototypes *****************************************/

axis2_status_t AXIS2_CALL
xml_schema_simple_type_content_free(void *sim_type_cont,
        const axis2_env_t *env);

xml_schema_annotated_t *AXIS2_CALL
xml_schema_simple_type_content_get_base_impl(void *sim_type_cont,
        const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_simple_type_content_get_type(void *sim_type_cont,
        const axis2_env_t *env);

axis2_hash_t* AXIS2_CALL
xml_schema_simple_type_content_super_objs(void *sim_type_cont,
        const axis2_env_t *env);

/*************** function prototypes *****************************************/

AXIS2_EXTERN xml_schema_simple_type_content_t * AXIS2_CALL
xml_schema_simple_type_content_create(const axis2_env_t *env)
{
    xml_schema_simple_type_content_impl_t *sim_type_cont_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    sim_type_cont_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_simple_type_content_impl_t));

    if (!sim_type_cont_impl)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    sim_type_cont_impl->annotated = NULL;
    sim_type_cont_impl->sim_type_cont.ops = NULL;
    sim_type_cont_impl->sim_type_cont.base.ops = NULL;
    sim_type_cont_impl->ht_super = NULL;
    sim_type_cont_impl->obj_type = XML_SCHEMA_SIMPLE_TYPE_CONTENT;

    sim_type_cont_impl->sim_type_cont.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_simple_type_content_ops_t));

    if (!sim_type_cont_impl->sim_type_cont.ops)
    {
        xml_schema_simple_type_content_free(&(sim_type_cont_impl->sim_type_cont), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    sim_type_cont_impl->sim_type_cont.ops->free =
        xml_schema_simple_type_content_free;
    sim_type_cont_impl->sim_type_cont.ops->get_base_impl =
        xml_schema_simple_type_content_get_base_impl;
    sim_type_cont_impl->sim_type_cont.ops->get_type =
        xml_schema_simple_type_content_get_type;
    sim_type_cont_impl->sim_type_cont.ops->super_objs =
        xml_schema_simple_type_content_super_objs;

    sim_type_cont_impl->ht_super = axis2_hash_make(env);
    if (!sim_type_cont_impl->ht_super)
    {
        xml_schema_simple_type_content_free(&(sim_type_cont_impl->sim_type_cont), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    sim_type_cont_impl->annotated = xml_schema_annotated_create(env);
    if (!sim_type_cont_impl->annotated)
    {
        xml_schema_simple_type_content_free(&(sim_type_cont_impl->sim_type_cont), env);
        return NULL;
    }

    axis2_hash_set(sim_type_cont_impl->ht_super,
            AXIS2_STRDUP("XML_SCHEMA_SIMPLE_TYPE_CONTENT", env),
            AXIS2_HASH_KEY_STRING, &(sim_type_cont_impl->sim_type_cont));

    axis2_hash_set(sim_type_cont_impl->ht_super,
            AXIS2_STRDUP("XML_SCHEMA_ANNOTATED", env),
            AXIS2_HASH_KEY_STRING, sim_type_cont_impl->annotated);

    axis2_hash_set(sim_type_cont_impl->ht_super,
            AXIS2_STRDUP("XML_SCHEMA_OBJ", env),
            AXIS2_HASH_KEY_STRING,
            XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(sim_type_cont_impl->annotated, env));

    status = xml_schema_annotated_resolve_methods(
                &(sim_type_cont_impl->sim_type_cont.base), env,
                sim_type_cont_impl->annotated,
                xml_schema_simple_type_content_super_objs,
                xml_schema_simple_type_content_get_type,
                xml_schema_simple_type_content_free);

    return &(sim_type_cont_impl->sim_type_cont);
}

axis2_status_t AXIS2_CALL
xml_schema_simple_type_content_free(void *sim_type_cont,
        const axis2_env_t *env)
{
    xml_schema_simple_type_content_impl_t *sim_type_cont_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    sim_type_cont_impl = AXIS2_INTF_TO_IMPL(sim_type_cont);


    if (sim_type_cont_impl->ht_super)
    {
        axis2_hash_free(sim_type_cont_impl->ht_super, env);
        sim_type_cont_impl->ht_super = NULL;
    }

    if (sim_type_cont_impl->annotated)
    {
        XML_SCHEMA_ANNOTATED_FREE(sim_type_cont_impl->annotated, env);
        sim_type_cont_impl->annotated = NULL;
    }

    if (sim_type_cont_impl->sim_type_cont.ops)
    {
        AXIS2_FREE(env->allocator, sim_type_cont_impl->sim_type_cont.ops);
        sim_type_cont_impl->sim_type_cont.ops = NULL;
    }
    if (sim_type_cont_impl->sim_type_cont.base.ops)
    {
        AXIS2_FREE(env->allocator, sim_type_cont_impl->sim_type_cont.base.ops);
        sim_type_cont_impl->sim_type_cont.base.ops = NULL;
    }

    if (sim_type_cont_impl)
    {
        AXIS2_FREE(env->allocator, sim_type_cont_impl);
        sim_type_cont_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

xml_schema_annotated_t *AXIS2_CALL
xml_schema_simple_type_content_get_base_impl(void *sim_type_cont,
        const axis2_env_t *env)
{
    xml_schema_simple_type_content_impl_t *sim_type_cont_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    sim_type_cont_impl = AXIS2_INTF_TO_IMPL(sim_type_cont);
    return sim_type_cont_impl->annotated;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
xml_schema_simple_type_content_resolve_methods(
    xml_schema_simple_type_content_t *sim_type_cont,
    const axis2_env_t *env,
    xml_schema_simple_type_content_t *sim_type_cont_impl,
    XML_SCHEMA_SUPER_OBJS_FN super_objs,
    XML_SCHEMA_GET_TYPE_FN get_type,
    XML_SCHEMA_FREE_FN free_fn)
{
    xml_schema_simple_type_content_impl_t *sim_type_cont_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, sim_type_cont_impl, AXIS2_FAILURE);

    sim_type_cont_impl_l =
        (xml_schema_simple_type_content_impl_t *) sim_type_cont_impl;

    sim_type_cont->ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_simple_type_content_ops_t));
    if (sim_type_cont->ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    sim_type_cont->ops->free = free_fn;
    sim_type_cont->ops->get_type = get_type;
    sim_type_cont->ops->super_objs = super_objs;

    return xml_schema_annotated_resolve_methods(&(sim_type_cont->base),
            env, sim_type_cont_impl_l->annotated, super_objs, get_type, free_fn);
}


xml_schema_types_t AXIS2_CALL
xml_schema_simple_type_content_get_type(void *sim_type_cont,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(sim_type_cont)->obj_type;
}

axis2_hash_t* AXIS2_CALL
xml_schema_simple_type_content_super_objs(void *sim_type_cont,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(sim_type_cont)->ht_super;
}

