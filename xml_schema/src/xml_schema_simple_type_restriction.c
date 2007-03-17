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

#include <xml_schema_simple_type_restriction.h>
#include <xml_schema_obj_collection.h>
#include <axis2_utils.h>

typedef struct xml_schema_simple_type_restriction_impl
            xml_schema_simple_type_restriction_impl_t;

/**
 * @brief xml_schema_simple_type_restriction_impl
 *
 */
struct xml_schema_simple_type_restriction_impl
{
    xml_schema_simple_type_restriction_t simple_type_restriction;

    xml_schema_simple_type_content_t *sim_type_content;

    axis2_hash_t *ht_super;

    xml_schema_types_t obj_type;

    xml_schema_simple_type_t *base_type;

    axis2_qname_t *base_type_name;

    xml_schema_obj_collection_t *facets;
};

#define AXIS2_INTF_TO_IMPL(simple_type_restriction) \
        ((xml_schema_simple_type_restriction_impl_t *) simple_type_restriction)

/*************** function prototypes *****************************************/

axis2_status_t AXIS2_CALL
xml_schema_simple_type_restriction_free(void *simple_type_restriction,
        const axis2_env_t *env);

xml_schema_simple_type_content_t *AXIS2_CALL
xml_schema_simple_type_restriction_get_base_impl(void *simple_type_restriction,
        const axis2_env_t *env);

xml_schema_simple_type_t* AXIS2_CALL
xml_schema_simple_type_restriction_get_base_type(void *simple_type_restriction,
        const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_simple_type_restriction_get_type(void *simple_type_restriction,
        const axis2_env_t *env);

axis2_hash_t* AXIS2_CALL
xml_schema_simple_type_restriction_super_objs(void *simple_type_restriction,
        const axis2_env_t *env);


axis2_status_t AXIS2_CALL
xml_schema_simple_type_restriction_set_base_type(void *simple_type_restriction,
        const axis2_env_t *env,
        xml_schema_simple_type_t *base_type);

axis2_qname_t* AXIS2_CALL
xml_schema_simple_type_restriction_get_base_type_name(void *simple_type_restriction,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_simple_type_restriction_set_base_type_name(void *simple_type_restriction,
        const axis2_env_t *env,
        axis2_qname_t *base_type_name);

xml_schema_obj_collection_t* AXIS2_CALL
xml_schema_simple_type_restriction_get_facets(void *simple_type_restriction,
        const axis2_env_t *env);
/*************** function prototypes *****************************************/

AXIS2_EXTERN xml_schema_simple_type_restriction_t * AXIS2_CALL
xml_schema_simple_type_restriction_create(const axis2_env_t *env)
{
    xml_schema_simple_type_restriction_impl_t *simple_type_restriction_impl = NULL;
    xml_schema_annotated_t *annotated = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    simple_type_restriction_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_simple_type_restriction_impl_t));

    if (!simple_type_restriction_impl)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    simple_type_restriction_impl->sim_type_content = NULL;
    simple_type_restriction_impl->simple_type_restriction.ops = NULL;
    simple_type_restriction_impl->simple_type_restriction.base.ops = NULL;
    simple_type_restriction_impl->base_type = NULL;
    simple_type_restriction_impl->base_type_name = NULL;
    simple_type_restriction_impl->facets = NULL;
    simple_type_restriction_impl->ht_super = NULL;
    simple_type_restriction_impl->obj_type = XML_SCHEMA_SIMPLE_TYPE_RESTRICTION;


    simple_type_restriction_impl->simple_type_restriction.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(xml_schema_simple_type_restriction_ops_t));

    if (!simple_type_restriction_impl->simple_type_restriction.ops)
    {
        xml_schema_simple_type_restriction_free(
            &(simple_type_restriction_impl->simple_type_restriction), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    simple_type_restriction_impl->simple_type_restriction.ops->free =
        xml_schema_simple_type_restriction_free;
    simple_type_restriction_impl->simple_type_restriction.ops->get_base_impl =
        xml_schema_simple_type_restriction_get_base_impl;
    simple_type_restriction_impl->simple_type_restriction.ops->get_type =
        xml_schema_simple_type_restriction_get_type;
    simple_type_restriction_impl->simple_type_restriction.ops->super_objs =
        xml_schema_simple_type_restriction_super_objs;
    simple_type_restriction_impl->simple_type_restriction.ops->get_base_type =
        xml_schema_simple_type_restriction_get_base_type;
    simple_type_restriction_impl->simple_type_restriction.ops->set_base_type =
        xml_schema_simple_type_restriction_set_base_type;
    simple_type_restriction_impl->simple_type_restriction.ops->get_base_type_name =
        xml_schema_simple_type_restriction_get_base_type_name;
    simple_type_restriction_impl->simple_type_restriction.ops->set_base_type_name =
        xml_schema_simple_type_restriction_set_base_type_name;
    simple_type_restriction_impl->simple_type_restriction.ops->get_facets =
        xml_schema_simple_type_restriction_get_facets;


    simple_type_restriction_impl->ht_super = axis2_hash_make(env);
    if (!simple_type_restriction_impl->ht_super)
    {
        xml_schema_simple_type_restriction_free(&(simple_type_restriction_impl->simple_type_restriction), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    simple_type_restriction_impl->sim_type_content =
        xml_schema_simple_type_content_create(env);

    simple_type_restriction_impl->facets = xml_schema_obj_collection_create(env);

    if (!simple_type_restriction_impl->sim_type_content ||
            !simple_type_restriction_impl->facets)
    {
        xml_schema_simple_type_restriction_free(
            &(simple_type_restriction_impl->simple_type_restriction), env);
        return NULL;
    }

    axis2_hash_set(simple_type_restriction_impl->ht_super,
            axis2_strdup("XML_SCHEMA_SIMPLE_TYPE_RESTRICTION", env),
            AXIS2_HASH_KEY_STRING, &(simple_type_restriction_impl->simple_type_restriction));


    axis2_hash_set(simple_type_restriction_impl->ht_super,
            axis2_strdup("XML_SCHEMA_SIMPLE_TYPE_CONTENT", env),
            AXIS2_HASH_KEY_STRING, simple_type_restriction_impl->sim_type_content);

    annotated = XML_SCHEMA_SIMPLE_TYPE_CONTENT_GET_BASE_IMPL(
                simple_type_restriction_impl->sim_type_content, env);

    if (annotated)
    {
        axis2_hash_set(simple_type_restriction_impl->ht_super,
                axis2_strdup("XML_SCHEMA_ANNOTATED", env),
                AXIS2_HASH_KEY_STRING, annotated);

        axis2_hash_set(simple_type_restriction_impl->ht_super,
                axis2_strdup("XML_SCHEMA_OBJ", env),
                AXIS2_HASH_KEY_STRING,
                XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(annotated, env));
    }

    status = xml_schema_simple_type_content_resolve_methods(
                &(simple_type_restriction_impl->simple_type_restriction.base), env,                                  simple_type_restriction_impl->sim_type_content,
                xml_schema_simple_type_restriction_super_objs,
                xml_schema_simple_type_restriction_get_type,
                xml_schema_simple_type_restriction_free);

    return &(simple_type_restriction_impl->simple_type_restriction);
}

axis2_status_t AXIS2_CALL
xml_schema_simple_type_restriction_free(void *simple_type_restriction,
        const axis2_env_t *env)
{
    xml_schema_simple_type_restriction_impl_t *simple_type_restriction_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    simple_type_restriction_impl = AXIS2_INTF_TO_IMPL(simple_type_restriction);


    if (simple_type_restriction_impl->ht_super)
    {
        axis2_hash_free(simple_type_restriction_impl->ht_super, env);
        simple_type_restriction_impl->ht_super = NULL;
    }


    if (simple_type_restriction_impl->sim_type_content)
    {
        XML_SCHEMA_SIMPLE_TYPE_CONTENT_FREE(
            simple_type_restriction_impl->sim_type_content , env);
        simple_type_restriction_impl->sim_type_content = NULL;
    }

    if (simple_type_restriction_impl->simple_type_restriction.ops)
    {
        AXIS2_FREE(env->allocator,
                simple_type_restriction_impl->simple_type_restriction.ops);
        simple_type_restriction_impl->simple_type_restriction.ops = NULL;
    }
    if (simple_type_restriction_impl->simple_type_restriction.base.ops)
    {
        AXIS2_FREE(env->allocator,
                simple_type_restriction_impl->simple_type_restriction.base.ops);
        simple_type_restriction_impl->simple_type_restriction.base.ops = NULL;
    }

    if (simple_type_restriction_impl)
    {
        AXIS2_FREE(env->allocator, simple_type_restriction_impl);
        simple_type_restriction_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

xml_schema_simple_type_content_t *AXIS2_CALL
xml_schema_simple_type_restriction_get_base_impl(void *simple_type_restriction,
        const axis2_env_t *env)
{
    xml_schema_simple_type_restriction_impl_t *simple_type_restriction_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    simple_type_restriction_impl = AXIS2_INTF_TO_IMPL(simple_type_restriction);

    return simple_type_restriction_impl->sim_type_content;
}

/*
AXIS2_EXTERN axis2_status_t AXIS2_CALL
xml_schema_simple_type_restriction_resolve_methods(
                    xml_schema_simple_type_restriction_t *simple_type_restriction,
                    const axis2_env_t *env,
                    xml_schema_simple_type_restriction_t *simple_type_restriction_impl,
                    axis2_hash_t *methods)
{
    xml_schema_simple_type_restriction_impl_t *simple_type_restriction_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, simple_type_restriction_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);

    simple_type_restriction_impl_l =
        (xml_schema_simple_type_restriction_impl_t *) simple_type_restriction_impl;

    simple_type_restriction->ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_simple_type_restriction_ops_t));
    if( simple_type_restriction->ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    simple_type_restriction->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    simple_type_restriction->ops->get_base_impl =
            simple_type_restriction_impl_l->simple_type_restriction.ops->get_base_impl;

    simple_type_restriction->ops->get_base_type =
            simple_type_restriction_impl_l->simple_type_restriction.ops->get_base_type;

    simple_type_restriction->ops->set_base_type =
            simple_type_restriction_impl_l->simple_type_restriction.ops->set_base_type;

    simple_type_restriction->ops->get_base_type_name =
            simple_type_restriction_impl_l->simple_type_restriction.ops->get_base_type_name;
    simple_type_restriction->ops->set_base_type_name =
            simple_type_restriction_impl_l->simple_type_restriction.ops->set_base_type_name;

    simple_type_restriction->ops->get_facets =
            simple_type_restriction_impl_l->simple_type_restriction.ops->get_facets;

    return xml_schema_simple_type_content_resolve_methods(
                &(simple_type_restriction->base), env,
                simple_type_restriction_impl_l->sim_type_content, methods);
}
*/
xml_schema_simple_type_t* AXIS2_CALL
xml_schema_simple_type_restriction_get_base_type(void *simple_type_restriction,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return
        AXIS2_INTF_TO_IMPL(simple_type_restriction)->base_type;
}

axis2_status_t AXIS2_CALL
xml_schema_simple_type_restriction_set_base_type(void *simple_type_restriction,
        const axis2_env_t *env,
        xml_schema_simple_type_t* base_type)
{
    xml_schema_simple_type_restriction_impl_t *sim_type_res_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    sim_type_res_impl = AXIS2_INTF_TO_IMPL(simple_type_restriction);
    if (sim_type_res_impl->base_type_name)
    {
        XML_SCHEMA_SIMPLE_TYPE_CONTENT_FREE(sim_type_res_impl->base_type, env);
        sim_type_res_impl->base_type_name = NULL;
    }

    sim_type_res_impl->base_type = base_type;

    return AXIS2_SUCCESS;
}

axis2_qname_t* AXIS2_CALL
xml_schema_simple_type_restriction_get_base_type_name(void *simple_type_restriction,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(simple_type_restriction)->base_type_name;
}

axis2_status_t AXIS2_CALL
xml_schema_simple_type_restriction_set_base_type_name(void *simple_type_restriction,
        const axis2_env_t *env,
        axis2_qname_t *base_type_name)
{
    xml_schema_simple_type_restriction_impl_t *sim_type_res_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, base_type_name, AXIS2_FAILURE);

    sim_type_res_impl = AXIS2_INTF_TO_IMPL(simple_type_restriction);
    if (sim_type_res_impl->base_type_name)
    {
        axis2_qname_free(sim_type_res_impl->base_type_name, env);
        sim_type_res_impl->base_type_name = NULL;
    }
    sim_type_res_impl->base_type_name = base_type_name;
    return AXIS2_SUCCESS;
}

xml_schema_obj_collection_t* AXIS2_CALL
xml_schema_simple_type_restriction_get_facets(void *simple_type_restriction,
        const axis2_env_t *env)
{
    xml_schema_simple_type_restriction_impl_t *sim_type_res_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    sim_type_res_impl = AXIS2_INTF_TO_IMPL(simple_type_restriction);
    return sim_type_res_impl->facets;
}

xml_schema_types_t AXIS2_CALL
xml_schema_simple_type_restriction_get_type(void *simple_type_restriction,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(simple_type_restriction)->obj_type;
}

axis2_hash_t* AXIS2_CALL
xml_schema_simple_type_restriction_super_objs(void *simple_type_restriction,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(simple_type_restriction)->ht_super;
}
