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

#include <xml_schema_simple_type_union.h>
#include <axis2_utils.h>

typedef struct xml_schema_simple_type_union_impl
            xml_schema_simple_type_union_impl_t;

/**
 * @brief xml_schema_simple_type_union_impl
 *
 */
struct xml_schema_simple_type_union_impl
{
    xml_schema_simple_type_union_t simple_type_union;

    xml_schema_simple_type_content_t *sim_type_content;

    axis2_hash_t *ht_super;

    xml_schema_types_t obj_type;

    axis2_char_t *member_types_source;

    axutil_array_list_t *member_types_qnames;

    xml_schema_obj_collection_t *base_types;
};

#define AXIS2_INTF_TO_IMPL(simple_type_union) \
        ((xml_schema_simple_type_union_impl_t *) simple_type_union)

/*************** function prototypes *****************************************/

axis2_status_t AXIS2_CALL
xml_schema_simple_type_union_free(void *simple_type_union,
        const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_simple_type_union_get_type(void *simple_type_union,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
xml_schema_simple_type_union_super_objs(void *simple_type_union,
        const axis2_env_t *env);


xml_schema_simple_type_content_t *AXIS2_CALL
xml_schema_simple_type_union_get_base_impl(void *simple_type_union,
        const axis2_env_t *env);


xml_schema_obj_collection_t* AXIS2_CALL
xml_schema_simple_type_union_get_base_types(void *simple_type_union,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_simple_type_union_set_member_types_source(void *simple_type_union,
        const axis2_env_t *env,
        axis2_char_t *member_types_source);

axis2_char_t* AXIS2_CALL
xml_schema_simple_type_union_get_member_types_source
(void *simple_type_union,
        const axis2_env_t *env);

axutil_array_list_t* AXIS2_CALL
xml_schema_simple_type_union_get_member_types_qnames(
    void *simple_type_union,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_simple_type_union_set_member_types_qnames(
    void *simple_type_union,
    const axis2_env_t *env,
    axutil_array_list_t *qns);

/*************** function prototypes *****************************************/

AXIS2_EXTERN xml_schema_simple_type_union_t * AXIS2_CALL
xml_schema_simple_type_union_create(const axis2_env_t *env)
{
    xml_schema_simple_type_union_impl_t *simple_type_union_impl = NULL;
    xml_schema_annotated_t *annotated = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    simple_type_union_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_simple_type_union_impl_t));

    if (!simple_type_union_impl)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    simple_type_union_impl->sim_type_content = NULL;
    simple_type_union_impl->simple_type_union.ops = NULL;
    simple_type_union_impl->simple_type_union.base.ops = NULL;
    simple_type_union_impl->member_types_qnames = NULL;
    simple_type_union_impl->member_types_source = NULL;
    simple_type_union_impl->ht_super = NULL;
    simple_type_union_impl->obj_type = XML_SCHEMA_SIMPLE_TYPE_UNION;


    simple_type_union_impl->simple_type_union.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(xml_schema_simple_type_union_ops_t));

    if (!simple_type_union_impl->simple_type_union.ops)
    {
        xml_schema_simple_type_union_free(
            &(simple_type_union_impl->simple_type_union), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    simple_type_union_impl->simple_type_union.ops->free =
        xml_schema_simple_type_union_free;
    simple_type_union_impl->simple_type_union.ops->get_base_impl =
        xml_schema_simple_type_union_get_base_impl;
    simple_type_union_impl->simple_type_union.ops->get_type =
        xml_schema_simple_type_union_get_type;
    simple_type_union_impl->simple_type_union.ops->super_objs =
        xml_schema_simple_type_union_super_objs;
    simple_type_union_impl->simple_type_union.ops->get_base_types =
        xml_schema_simple_type_union_get_base_types;
    simple_type_union_impl->simple_type_union.ops->set_member_types_source =
        xml_schema_simple_type_union_set_member_types_source;
    simple_type_union_impl->simple_type_union.ops->get_member_types_source =
        xml_schema_simple_type_union_get_member_types_source;
    simple_type_union_impl->simple_type_union.ops->get_member_types_qnames =
        xml_schema_simple_type_union_get_member_types_qnames;
    simple_type_union_impl->simple_type_union.ops->set_member_types_qnames =
        xml_schema_simple_type_union_set_member_types_qnames;

    simple_type_union_impl->ht_super = axis2_hash_make(env);

    if (!simple_type_union_impl->ht_super)
    {
        xml_schema_simple_type_union_free(
            &(simple_type_union_impl->simple_type_union), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    simple_type_union_impl->sim_type_content =
        xml_schema_simple_type_content_create(env);

    if (!simple_type_union_impl->sim_type_content)
    {
        xml_schema_simple_type_union_free(
            &(simple_type_union_impl->simple_type_union), env);
        return NULL;
    }

    simple_type_union_impl->base_types =
        xml_schema_obj_collection_create(env);

    if (!simple_type_union_impl->base_types)
    {
        xml_schema_simple_type_union_free(
            &(simple_type_union_impl->simple_type_union), env);
        return NULL;
    }


    axis2_hash_set(simple_type_union_impl->ht_super,
            axis2_strdup(env, "XML_SCHEMA_SIMPLE_TYPE_UNION"),
            AXIS2_HASH_KEY_STRING, &(simple_type_union_impl->simple_type_union));

    axis2_hash_set(simple_type_union_impl->ht_super,
            axis2_strdup(env, "XML_SCHEMA_SIMPLE_TYPE_CONTENT"),
            AXIS2_HASH_KEY_STRING,
            simple_type_union_impl->sim_type_content);

    annotated = XML_SCHEMA_SIMPLE_TYPE_CONTENT_GET_BASE_IMPL(
                simple_type_union_impl->sim_type_content, env);

    if (annotated)
    {
        axis2_hash_set(simple_type_union_impl->ht_super,
                axis2_strdup(env, "XML_SCHEMA_ANNOTATED"),
                AXIS2_HASH_KEY_STRING, annotated);

        axis2_hash_set(simple_type_union_impl->ht_super,
                axis2_strdup(env, "XML_SCHEMA_OBJ"),
                AXIS2_HASH_KEY_STRING,
                XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(annotated, env));
    }

    status = xml_schema_simple_type_content_resolve_methods(
                &(simple_type_union_impl->simple_type_union.base), env,                                  simple_type_union_impl->sim_type_content,
                xml_schema_simple_type_union_super_objs,
                xml_schema_simple_type_union_get_type,
                xml_schema_simple_type_union_free);

    return &(simple_type_union_impl->simple_type_union);
}

axis2_status_t AXIS2_CALL
xml_schema_simple_type_union_free(void *simple_type_union,
        const axis2_env_t *env)
{
    xml_schema_simple_type_union_impl_t *simple_type_union_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    simple_type_union_impl = AXIS2_INTF_TO_IMPL(simple_type_union);


    if (simple_type_union_impl->ht_super)
    {
        axis2_hash_free(simple_type_union_impl->ht_super, env);
        simple_type_union_impl->ht_super = NULL;
    }
    if (simple_type_union_impl->sim_type_content)
    {
        XML_SCHEMA_SIMPLE_TYPE_CONTENT_FREE(
            simple_type_union_impl->sim_type_content , env);
        simple_type_union_impl->sim_type_content = NULL;
    }

    if (simple_type_union_impl->simple_type_union.ops)
    {
        AXIS2_FREE(env->allocator,
                simple_type_union_impl->simple_type_union.ops);
        simple_type_union_impl->simple_type_union.ops = NULL;
    }
    if (simple_type_union_impl->simple_type_union.base.ops)
    {
        AXIS2_FREE(env->allocator,
                simple_type_union_impl->simple_type_union.base.ops);
        simple_type_union_impl->simple_type_union.base.ops = NULL;
    }

    if (simple_type_union_impl)
    {
        AXIS2_FREE(env->allocator, simple_type_union_impl);
        simple_type_union_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

xml_schema_simple_type_content_t *AXIS2_CALL
xml_schema_simple_type_union_get_base_impl(void *simple_type_union,
        const axis2_env_t *env)
{
    xml_schema_simple_type_union_impl_t *simple_type_union_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    simple_type_union_impl = AXIS2_INTF_TO_IMPL(simple_type_union);

    return simple_type_union_impl->sim_type_content;
}

/*
AXIS2_EXTERN axis2_status_t AXIS2_CALL
xml_schema_simple_type_union_resolve_methods(
                    xml_schema_simple_type_union_t *simple_type_union,
                    const axis2_env_t *env,
                    xml_schema_simple_type_union_t *simple_type_union_impl,
                    axis2_hash_t *methods)
{
    xml_schema_simple_type_union_impl_t *simple_type_union_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, simple_type_union_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);

    simple_type_union_impl_l =
        (xml_schema_simple_type_union_impl_t *) simple_type_union_impl;

    simple_type_union->ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_simple_type_union_ops_t));
    if( simple_type_union->ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    simple_type_union->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    simple_type_union->ops->get_base_impl =
            simple_type_union_impl_l->simple_type_union.ops->get_base_impl;

    simple_type_union->ops->get_base_types =
            simple_type_union_impl_l->simple_type_union.ops->get_base_types;

    simple_type_union->ops->get_member_types_qnames =
            simple_type_union_impl_l->simple_type_union.ops->get_member_types_qnames;

    simple_type_union->ops->get_member_types_source =
            simple_type_union_impl_l->simple_type_union.ops->get_member_types_source;
    simple_type_union->ops->set_member_types_source =
            simple_type_union_impl_l->simple_type_union.ops->set_member_types_source;

    return xml_schema_simple_type_content_resolve_methods(
                &(simple_type_union->base), env,
                simple_type_union_impl_l->sim_type_content, methods);
}
*/

xml_schema_obj_collection_t* AXIS2_CALL
xml_schema_simple_type_union_get_base_types(void *simple_type_union,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return
        AXIS2_INTF_TO_IMPL(simple_type_union)->base_types;
}

axis2_status_t AXIS2_CALL
xml_schema_simple_type_union_set_member_types_source(void *simple_type_union,
        const axis2_env_t *env,
        axis2_char_t *member_types_source)

{
    xml_schema_simple_type_union_impl_t *sim_type_res_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    sim_type_res_impl = AXIS2_INTF_TO_IMPL(simple_type_union);
    if (sim_type_res_impl->member_types_source)
    {
        XML_SCHEMA_SIMPLE_TYPE_CONTENT_FREE(sim_type_res_impl->member_types_source, env);
        sim_type_res_impl->member_types_source = NULL;
    }

    sim_type_res_impl->member_types_source = axis2_strdup(env, member_types_source);

    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL
xml_schema_simple_type_union_get_member_types_source
(void *simple_type_union,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(simple_type_union)->member_types_source;
}


axutil_array_list_t* AXIS2_CALL
xml_schema_simple_type_union_get_member_types_qnames(void *simple_type_union,
        const axis2_env_t *env)
{
    xml_schema_simple_type_union_impl_t *sim_type_res_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    return sim_type_res_impl->member_types_qnames;
}

xml_schema_types_t AXIS2_CALL
xml_schema_simple_type_union_get_type(void *simple_type_union,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(simple_type_union)->obj_type;
}

axis2_hash_t *AXIS2_CALL
xml_schema_simple_type_union_super_objs(void *simple_type_union,
        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(simple_type_union)->ht_super;
}

axis2_status_t AXIS2_CALL
xml_schema_simple_type_union_set_member_types_qnames(
    void *simple_type_union,
    const axis2_env_t *env,
    axutil_array_list_t *qns)
{
    xml_schema_simple_type_union_impl_t *sim_type_union_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, qns, AXIS2_FAILURE);
    sim_type_union_impl = AXIS2_INTF_TO_IMPL(simple_type_union);
    sim_type_union_impl->member_types_qnames = qns;
    return AXIS2_FAILURE;
}

