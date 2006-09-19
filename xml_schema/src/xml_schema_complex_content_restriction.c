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

#include <xml_schema_complex_content_restriction.h>
#include <axis2_utils.h>

typedef struct xml_schema_complex_content_restriction_impl
            xml_schema_complex_content_restriction_impl_t;

/**
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension
 */
struct xml_schema_complex_content_restriction_impl
{
    xml_schema_complex_content_restriction_t cmp_content_res;

    xml_schema_annotated_t *annotated;

    xml_schema_obj_collection_t *attributes;

    xml_schema_any_attribute_t *any_attribute;

    axis2_qname_t *base_type_name;

    xml_schema_particle_t *particle;

    axis2_hash_t *ht_super;

    xml_schema_types_t obj_type;
};

#define AXIS2_INTF_TO_IMPL(cmp_content_res) \
        ((xml_schema_complex_content_restriction_impl_t *) cmp_content_res)

axis2_status_t AXIS2_CALL
xml_schema_complex_content_restriction_free(
    void *cmp_content_res,
    const axis2_env_t *env);

xml_schema_annotated_t *AXIS2_CALL
xml_schema_complex_content_restriction_get_base_impl(
    void *cmp_content_res,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
xml_schema_complex_content_restriction_super_objs(
    void *cmp_content_res,
    const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_complex_content_restriction_get_type(
    void *cmp_content_res,
    const axis2_env_t *env);

xml_schema_any_attribute_t* AXIS2_CALL
xml_schema_complex_content_restriction_get_any_attribute(
    void *cmp_content_res,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_complex_content_restriction_set_any_attribute(
    void *cmp_content_res,
    const axis2_env_t *env,
    xml_schema_any_attribute_t *any_attr);

xml_schema_obj_collection_t* AXIS2_CALL
xml_schema_complex_content_restriction_get_attributes(
    void *cmp_content_res,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_complex_content_restriction_set_base_type_name(
    void *cmp_content_res,
    const axis2_env_t *env,
    axis2_qname_t *base_type_name);

axis2_qname_t* AXIS2_CALL
xml_schema_complex_content_restriction_get_base_type_name(
    void *cmp_content_res,
    const axis2_env_t *env);

axis2_char_t* AXIS2_CALL
xml_schema_complex_content_restriction_to_string(
    void *cmp_cnt_res,
    const axis2_env_t *env,
    axis2_char_t *prefix,
    int tab);

xml_schema_particle_t* AXIS2_CALL
xml_schema_complex_content_restriction_get_particle(
    void *cmp_cnt_res,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_complex_content_restriction_set_particle(
    void *cmp_content,
    const axis2_env_t *env,
    xml_schema_particle_t *particle);


/*****************************************************************************/
AXIS2_EXTERN xml_schema_complex_content_restriction_t * AXIS2_CALL
xml_schema_complex_content_restriction_create(const axis2_env_t *env)
{
    xml_schema_complex_content_restriction_impl_t *cmp_content_res_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    cmp_content_res_impl = (xml_schema_complex_content_restriction_impl_t*)
            AXIS2_MALLOC(env->allocator, sizeof(xml_schema_complex_content_restriction_impl_t));

    if (!cmp_content_res_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    cmp_content_res_impl->annotated = NULL;
    cmp_content_res_impl->cmp_content_res.ops = NULL;
    cmp_content_res_impl->cmp_content_res.base.ops = NULL;
    cmp_content_res_impl->attributes = NULL;
    cmp_content_res_impl->base_type_name = NULL;
    cmp_content_res_impl->any_attribute = NULL;
    cmp_content_res_impl->particle = NULL;
    cmp_content_res_impl->ht_super = NULL;
    cmp_content_res_impl->obj_type = XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION;


    cmp_content_res_impl->cmp_content_res.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_complex_content_restriction_ops_t));
    if (!cmp_content_res_impl->cmp_content_res.ops)
    {
        xml_schema_complex_content_restriction_free(
            &(cmp_content_res_impl->cmp_content_res), env);
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }


    cmp_content_res_impl->cmp_content_res.ops->free =
        xml_schema_complex_content_restriction_free;

    cmp_content_res_impl->cmp_content_res.ops->get_type =
        xml_schema_complex_content_restriction_get_type;

    cmp_content_res_impl->cmp_content_res.ops->super_objs =
        xml_schema_complex_content_restriction_super_objs;

    cmp_content_res_impl->cmp_content_res.ops->get_base_impl =
        xml_schema_complex_content_restriction_get_base_impl;

    cmp_content_res_impl->cmp_content_res.ops->get_any_attribute =
        xml_schema_complex_content_restriction_get_any_attribute;

    cmp_content_res_impl->cmp_content_res.ops->set_any_attribute =
        xml_schema_complex_content_restriction_set_any_attribute;

    cmp_content_res_impl->cmp_content_res.ops->get_attributes =
        xml_schema_complex_content_restriction_get_attributes;

    cmp_content_res_impl->cmp_content_res.ops->get_base_type_name =
        xml_schema_complex_content_restriction_get_base_type_name;

    cmp_content_res_impl->cmp_content_res.ops->set_base_type_name =
        xml_schema_complex_content_restriction_set_base_type_name;

    cmp_content_res_impl->cmp_content_res.ops->get_particle =
        xml_schema_complex_content_restriction_get_particle;

    cmp_content_res_impl->cmp_content_res.ops->set_particle =
        xml_schema_complex_content_restriction_set_particle;

    cmp_content_res_impl->cmp_content_res.ops->to_string =
        xml_schema_complex_content_restriction_to_string;

    cmp_content_res_impl->ht_super = axis2_hash_make(env);
    if (!cmp_content_res_impl->ht_super)
    {
        xml_schema_complex_content_restriction_free(
            &(cmp_content_res_impl->cmp_content_res), env);
        return NULL;
    }

    cmp_content_res_impl->annotated = xml_schema_annotated_create(env);
    if (!cmp_content_res_impl->annotated)
    {
        xml_schema_complex_content_restriction_free(
            &(cmp_content_res_impl->cmp_content_res), env);
        return NULL;
    }
    cmp_content_res_impl->attributes = xml_schema_obj_collection_create(env);
    if (!cmp_content_res_impl->attributes)
    {
        xml_schema_complex_content_restriction_free(
            &(cmp_content_res_impl->cmp_content_res), env);
        return NULL;
    }

    axis2_hash_set(cmp_content_res_impl->ht_super,
            AXIS2_STRDUP("XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION", env),
            AXIS2_HASH_KEY_STRING, &(cmp_content_res_impl->cmp_content_res));
    axis2_hash_set(cmp_content_res_impl->ht_super, AXIS2_STRDUP("XML_SCHEMA_ANNOTATED", env),
            AXIS2_HASH_KEY_STRING, cmp_content_res_impl->annotated);
    axis2_hash_set(cmp_content_res_impl->ht_super, AXIS2_STRDUP("XML_SCHEMA_OBJ", env),
            AXIS2_HASH_KEY_STRING,
            XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(cmp_content_res_impl->annotated, env));
    xml_schema_annotated_resolve_methods(&(cmp_content_res_impl->cmp_content_res.base),
            env, cmp_content_res_impl->annotated,
            xml_schema_complex_content_restriction_super_objs,
            xml_schema_complex_content_restriction_get_type,
            xml_schema_complex_content_restriction_free);

    return &(cmp_content_res_impl->cmp_content_res);
}

axis2_status_t AXIS2_CALL
xml_schema_complex_content_restriction_free(
    void *cmp_content_res,
    const axis2_env_t *env)
{
    xml_schema_complex_content_restriction_impl_t *content_res_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    content_res_impl = AXIS2_INTF_TO_IMPL(cmp_content_res);
    if (content_res_impl->any_attribute)
    {
        XML_SCHEMA_ANY_ATTRIBUTE_FREE(content_res_impl->any_attribute, env);
        content_res_impl->any_attribute = NULL;
    }
    if (content_res_impl->attributes)
    {
        /** TODO free attributes */
    }

    if (content_res_impl->base_type_name)
    {
        AXIS2_QNAME_FREE(content_res_impl->base_type_name, env);
        content_res_impl->base_type_name = NULL;
    }

    if (content_res_impl->annotated)
    {
        XML_SCHEMA_ANNOTATED_FREE(content_res_impl->annotated, env);
        content_res_impl->annotated = NULL;
    }
    if (content_res_impl->cmp_content_res.base.ops)
    {
        AXIS2_FREE(env->allocator, content_res_impl->cmp_content_res.base.ops);
        content_res_impl->cmp_content_res.base.ops = NULL;
    }
    if (content_res_impl->cmp_content_res.ops)
    {
        AXIS2_FREE(env->allocator, content_res_impl->cmp_content_res.ops);
        content_res_impl->cmp_content_res.ops = NULL;
    }

    AXIS2_FREE(env->allocator, content_res_impl);
    content_res_impl = NULL;
    return AXIS2_SUCCESS;
}

xml_schema_annotated_t *AXIS2_CALL
xml_schema_complex_content_restriction_get_base_impl(
    void *cmp_content_res,
    const axis2_env_t *env)
{
    xml_schema_complex_content_restriction_impl_t *content_res_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    content_res_impl = AXIS2_INTF_TO_IMPL(cmp_content_res);
    return content_res_impl->annotated;
}



xml_schema_any_attribute_t* AXIS2_CALL
xml_schema_complex_content_restriction_get_any_attribute(
    void *cmp_content_res,
    const axis2_env_t *env)
{
    xml_schema_complex_content_restriction_impl_t *cnt_res_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    cnt_res_impl = AXIS2_INTF_TO_IMPL(cmp_content_res);
    return cnt_res_impl->any_attribute;
}

axis2_status_t AXIS2_CALL
xml_schema_complex_content_restriction_set_any_attribute(
    void *cmp_content_res,
    const axis2_env_t *env,
    xml_schema_any_attribute_t *any_attr)
{
    xml_schema_complex_content_restriction_impl_t *content_res_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, any_attr, AXIS2_FAILURE);
    if (content_res_impl->any_attribute)
    {
        XML_SCHEMA_ANY_ATTRIBUTE_FREE(content_res_impl->any_attribute, env);
        content_res_impl->any_attribute = NULL;
    }

    content_res_impl->any_attribute = any_attr;
    return AXIS2_SUCCESS;
}

xml_schema_obj_collection_t* AXIS2_CALL
xml_schema_complex_content_restriction_get_attributes(
    void *cmp_content,
    const axis2_env_t *env)
{
    xml_schema_complex_content_restriction_impl_t *cnt_res_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    cnt_res_impl = AXIS2_INTF_TO_IMPL(cmp_content);
    return cnt_res_impl->attributes;
}

axis2_qname_t* AXIS2_CALL
xml_schema_complex_content_restriction_get_base_type_name(
    void *cmp_content,
    const axis2_env_t *env)
{
    xml_schema_complex_content_restriction_impl_t *content_res_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    content_res_impl = AXIS2_INTF_TO_IMPL(cmp_content);
    return content_res_impl->base_type_name;
}

axis2_status_t AXIS2_CALL
xml_schema_complex_content_restriction_set_base_type_name(
    void *cmp_content,
    const axis2_env_t *env,
    axis2_qname_t *base_type_name)
{
    xml_schema_complex_content_restriction_impl_t *content_res_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    content_res_impl = AXIS2_INTF_TO_IMPL(cmp_content);
    if (content_res_impl->base_type_name)
    {
        AXIS2_QNAME_FREE(content_res_impl->base_type_name, env);
        content_res_impl->base_type_name = NULL;
    }
    content_res_impl->base_type_name = base_type_name;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
xml_schema_complex_content_restriction_set_particle(
    void *cmp_content,
    const axis2_env_t *env,
    xml_schema_particle_t *particle)
{
    xml_schema_complex_content_restriction_impl_t *cmp_cnt_res_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, particle, AXIS2_FAILURE);

    if (cmp_cnt_res_impl->particle)
    {
        XML_SCHEMA_PARTICLE_FREE(cmp_cnt_res_impl->particle, env);
        cmp_cnt_res_impl->particle = NULL;
    }

    cmp_cnt_res_impl->particle = particle;
    return AXIS2_SUCCESS;
}

xml_schema_particle_t* AXIS2_CALL
xml_schema_complex_content_restriction_get_particle(
    void *cmp_cnt_res,
    const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(cmp_cnt_res)->particle;
}

axis2_char_t* AXIS2_CALL
xml_schema_complex_content_restriction_to_string(
    void *cmp_cnt_res,
    const axis2_env_t *env,
    axis2_char_t *prefix,
    int tab)
{
    /** TODO */
    return NULL;
}

axis2_hash_t *AXIS2_CALL
xml_schema_complex_content_restriction_super_objs(
    void *cmp_content_res,
    const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(cmp_content_res)->ht_super;
}

xml_schema_types_t AXIS2_CALL
xml_schema_complex_content_restriction_get_type(
    void *cmp_content_res,
    const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(cmp_content_res)->obj_type;
}
