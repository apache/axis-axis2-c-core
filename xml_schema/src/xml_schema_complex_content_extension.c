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

#include <xml_schema_complex_content_extension.h>
#include <axis2_utils.h>

typedef struct xml_schema_complex_content_extension_impl
            xml_schema_complex_content_extension_impl_t;

/**
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension
 */
struct xml_schema_complex_content_extension_impl
{
    xml_schema_complex_content_extension_t cmp_cnt_ext;

    xml_schema_annotated_t *annotated;

    xml_schema_obj_collection_t *attributes;

    xml_schema_any_attribute_t *any_attribute;

    axutil_qname_t *base_type_name;

    xml_schema_particle_t *particle;

    axutil_hash_t *ht_super;

    xml_schema_types_t obj_type;
};

#define AXIS2_INTF_TO_IMPL(cmp_cnt_ext) \
        ((xml_schema_complex_content_extension_impl_t *) cmp_cnt_ext)

axis2_status_t AXIS2_CALL
xml_schema_complex_content_extension_free(
    void *cmp_cnt_ext,
    const axutil_env_t *env);

xml_schema_annotated_t *AXIS2_CALL
xml_schema_complex_content_extension_get_base_impl(
    void *cmp_cnt_ext,
    const axutil_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_complex_content_extension_get_type(
    void *cmp_cnt_ext,
    const axutil_env_t *env);

axutil_hash_t *AXIS2_CALL
xml_schema_complex_content_extension_super_objs(
    void *cmp_cnt_ext,
    const axutil_env_t *env);

xml_schema_any_attribute_t* AXIS2_CALL
xml_schema_complex_content_extension_get_any_attribute(
    void *cmp_cnt_ext,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_complex_content_extension_set_any_attribute(
    void *cmp_cnt_ext,
    const axutil_env_t *env,
    xml_schema_any_attribute_t *any_attr);

xml_schema_obj_collection_t* AXIS2_CALL
xml_schema_complex_content_extension_get_attributes(
    void *cmp_cnt_ext,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_complex_content_extension_set_base_type_name(
    void *cmp_cnt_ext,
    const axutil_env_t *env,
    axutil_qname_t *base_type_name);

axutil_qname_t* AXIS2_CALL
xml_schema_complex_content_extension_get_base_type_name(
    void *cmp_cnt_ext,
    const axutil_env_t *env);


xml_schema_particle_t* AXIS2_CALL
xml_schema_complex_content_extension_get_particle(
    void *cmp_cnt_ext,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_complex_content_extension_set_particle(
    void *cmp_cnt_ext,
    const axutil_env_t *env,
    xml_schema_particle_t *particle);

axis2_char_t *AXIS2_CALL
xml_schema_complex_content_extension_to_string(
    void *cmp_cnt_ext,
    const axutil_env_t *env,
    axis2_char_t *prefix,
    int tab);

/*************************** end function protos **********************************/
AXIS2_EXTERN xml_schema_complex_content_extension_t * AXIS2_CALL
xml_schema_complex_content_extension_create(const axutil_env_t *env)
{
    xml_schema_complex_content_extension_impl_t *cmp_cnt_ext_impl = NULL;
    cmp_cnt_ext_impl = (xml_schema_complex_content_extension_impl_t*)
            AXIS2_MALLOC(env->allocator,
                    sizeof(xml_schema_complex_content_extension_impl_t));

    if (!cmp_cnt_ext_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    cmp_cnt_ext_impl->annotated = NULL;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops = NULL;
    cmp_cnt_ext_impl->cmp_cnt_ext.base.ops = NULL;
    cmp_cnt_ext_impl->attributes = NULL;
    cmp_cnt_ext_impl->base_type_name = NULL;
    cmp_cnt_ext_impl->any_attribute = NULL;
    cmp_cnt_ext_impl->particle = NULL;
    cmp_cnt_ext_impl->ht_super = NULL;
    cmp_cnt_ext_impl->obj_type = XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION;

    cmp_cnt_ext_impl->cmp_cnt_ext.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_complex_content_extension_ops_t));

    if (!cmp_cnt_ext_impl->cmp_cnt_ext.ops)
    {
        xml_schema_complex_content_extension_free(
            &(cmp_cnt_ext_impl->cmp_cnt_ext), env);
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    cmp_cnt_ext_impl->cmp_cnt_ext.ops->free =
        xml_schema_complex_content_extension_free;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->get_type =
        xml_schema_complex_content_extension_get_type;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->super_objs =
        xml_schema_complex_content_extension_super_objs;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->get_base_impl =
        xml_schema_complex_content_extension_get_base_impl;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->get_any_attribute =
        xml_schema_complex_content_extension_get_any_attribute;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->set_any_attribute =
        xml_schema_complex_content_extension_set_any_attribute;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->get_attributes =
        xml_schema_complex_content_extension_get_attributes;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->get_base_type_name =
        xml_schema_complex_content_extension_get_base_type_name;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->set_base_type_name =
        xml_schema_complex_content_extension_set_base_type_name;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->get_particle =
        xml_schema_complex_content_extension_get_particle;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->set_particle =
        xml_schema_complex_content_extension_set_particle;
    cmp_cnt_ext_impl->cmp_cnt_ext.ops->to_string =
        xml_schema_complex_content_extension_to_string;

    cmp_cnt_ext_impl->ht_super = axutil_hash_make(env);
    if (!cmp_cnt_ext_impl->ht_super)
    {
        xml_schema_complex_content_extension_free(
            &(cmp_cnt_ext_impl->cmp_cnt_ext), env);
        return NULL;
    }


    cmp_cnt_ext_impl->annotated = xml_schema_annotated_create(env);

    if (!cmp_cnt_ext_impl->annotated)
    {
        xml_schema_complex_content_extension_free(
            &(cmp_cnt_ext_impl->cmp_cnt_ext), env);
        return NULL;
    }
    cmp_cnt_ext_impl->attributes = xml_schema_obj_collection_create(env);
    if (!cmp_cnt_ext_impl->attributes)
    {
        xml_schema_complex_content_extension_free(&(cmp_cnt_ext_impl->cmp_cnt_ext), env);
        return NULL;
    }
    axutil_hash_set(cmp_cnt_ext_impl->ht_super, axis2_strdup(env, "XML_SCHEMA_COMPLEX_CONTENT_EXTENSION"),
            AXIS2_HASH_KEY_STRING, &(cmp_cnt_ext_impl->cmp_cnt_ext));
    axutil_hash_set(cmp_cnt_ext_impl->ht_super, axis2_strdup(env, "XML_SCHEMA_ANNOTATED"),
            AXIS2_HASH_KEY_STRING, cmp_cnt_ext_impl->annotated);
    axutil_hash_set(cmp_cnt_ext_impl->ht_super, axis2_strdup(env, "XML_SCHEMA_OBJ"),
            AXIS2_HASH_KEY_STRING,
            XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(cmp_cnt_ext_impl->annotated, env));

    xml_schema_annotated_resolve_methods(&(cmp_cnt_ext_impl->cmp_cnt_ext.base), env,
            cmp_cnt_ext_impl->annotated,
            xml_schema_complex_content_extension_super_objs,
            xml_schema_complex_content_extension_get_type,
            xml_schema_complex_content_extension_free);
    return &(cmp_cnt_ext_impl->cmp_cnt_ext);
}

axis2_status_t AXIS2_CALL
xml_schema_complex_content_extension_free(void *cmp_cnt_ext_ext_ext,
        const axutil_env_t *env)
{
    xml_schema_complex_content_extension_impl_t *content_ext_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    content_ext_impl = AXIS2_INTF_TO_IMPL(cmp_cnt_ext_ext_ext);
    if (content_ext_impl->any_attribute)
    {
        XML_SCHEMA_ANY_ATTRIBUTE_FREE(content_ext_impl->any_attribute, env);
        content_ext_impl->any_attribute = NULL;
    }
    if (content_ext_impl->attributes)
    {
        /** TODO free attributes */
    }
    if (content_ext_impl->base_type_name)
    {
        axutil_qname_free(content_ext_impl->base_type_name, env);
        content_ext_impl->base_type_name = NULL;
    }
    if (content_ext_impl->annotated)
    {
        XML_SCHEMA_ANNOTATED_FREE(content_ext_impl->annotated, env);
        content_ext_impl->annotated = NULL;
    }
    if (content_ext_impl->cmp_cnt_ext.ops)
    {
        AXIS2_FREE(env->allocator, content_ext_impl->cmp_cnt_ext.ops);
        content_ext_impl->cmp_cnt_ext.ops = NULL;
    }
    if (content_ext_impl->cmp_cnt_ext.base.ops)
    {
        AXIS2_FREE(env->allocator, content_ext_impl->cmp_cnt_ext.base.ops);
        content_ext_impl->cmp_cnt_ext.base.ops = NULL;
    }

    AXIS2_FREE(env->allocator, content_ext_impl);
    content_ext_impl = NULL;
    return AXIS2_SUCCESS;
}

xml_schema_annotated_t *AXIS2_CALL
xml_schema_complex_content_extension_get_base_impl(void *cmp_cnt_ext,
        const axutil_env_t *env)
{
    xml_schema_complex_content_extension_impl_t *content_ext_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    content_ext_impl = AXIS2_INTF_TO_IMPL(cmp_cnt_ext);
    return content_ext_impl->annotated;
}



xml_schema_any_attribute_t* AXIS2_CALL
xml_schema_complex_content_extension_get_any_attribute(void *cmp_cnt_ext,
        const axutil_env_t *env)
{
    xml_schema_complex_content_extension_impl_t *cnt_ext_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    cnt_ext_impl = AXIS2_INTF_TO_IMPL(cmp_cnt_ext);
    return cnt_ext_impl->any_attribute;
}

axis2_status_t AXIS2_CALL
xml_schema_complex_content_extension_set_any_attribute(void *cmp_cnt_ext,
        const axutil_env_t *env,
        xml_schema_any_attribute_t *any_attr)
{
    xml_schema_complex_content_extension_impl_t *content_ext_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, any_attr, AXIS2_FAILURE);
    if (content_ext_impl->any_attribute)
    {
        XML_SCHEMA_ANY_ATTRIBUTE_FREE(content_ext_impl->any_attribute, env);
        content_ext_impl->any_attribute = NULL;
    }
    content_ext_impl->any_attribute = any_attr;
    return AXIS2_SUCCESS;
}

xml_schema_obj_collection_t* AXIS2_CALL
xml_schema_complex_content_extension_get_attributes(void *cmp_cnt_ext,
        const axutil_env_t *env)
{
    xml_schema_complex_content_extension_impl_t *cnt_ext_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    cnt_ext_impl = AXIS2_INTF_TO_IMPL(cmp_cnt_ext);
    return cnt_ext_impl->attributes;
}

axutil_qname_t* AXIS2_CALL
xml_schema_complex_content_extension_get_base_type_name(
    void *cmp_cnt_ext,
    const axutil_env_t *env)
{
    xml_schema_complex_content_extension_impl_t *content_ext_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    content_ext_impl = AXIS2_INTF_TO_IMPL(cmp_cnt_ext);
    return content_ext_impl->base_type_name;
}

axis2_status_t AXIS2_CALL
xml_schema_complex_content_extension_set_base_type_name(
    void *cmp_cnt_ext,
    const axutil_env_t *env,
    axutil_qname_t *base_type_name)
{
    xml_schema_complex_content_extension_impl_t *content_ext_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    content_ext_impl = AXIS2_INTF_TO_IMPL(cmp_cnt_ext);
    if (content_ext_impl->base_type_name)
    {
        axutil_qname_free(content_ext_impl->base_type_name, env);
        content_ext_impl->base_type_name = NULL;
    }
    content_ext_impl->base_type_name = base_type_name;
    return AXIS2_SUCCESS;
}

xml_schema_particle_t* AXIS2_CALL
xml_schema_complex_content_extension_get_particle(
    void *cmp_cnt_ext,
    const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(cmp_cnt_ext)->particle;
}

axis2_status_t AXIS2_CALL
xml_schema_complex_content_extension_set_particle(
    void *cmp_cnt_ext,
    const axutil_env_t *env,
    xml_schema_particle_t *particle)
{
    xml_schema_complex_content_extension_impl_t *cmp_cnt_ext_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, particle, AXIS2_FAILURE);
    cmp_cnt_ext_impl = AXIS2_INTF_TO_IMPL(cmp_cnt_ext);
    if (cmp_cnt_ext_impl->particle)
    {
        XML_SCHEMA_PARTICLE_FREE(cmp_cnt_ext_impl->particle, env);
        cmp_cnt_ext_impl->particle = NULL;
    }
    cmp_cnt_ext_impl->particle = particle;
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
xml_schema_complex_content_extension_to_string(
    void *cmp_cnt_ext,
    const axutil_env_t *env,
    axis2_char_t *prefix,
    int tab)
{
    return NULL;
}

xml_schema_types_t AXIS2_CALL
xml_schema_complex_content_extension_get_type(
    void *cmp_cnt_ext,
    const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(cmp_cnt_ext)->obj_type;
}

axutil_hash_t *AXIS2_CALL
xml_schema_complex_content_extension_super_objs(
    void *cmp_cnt_ext,
    const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(cmp_cnt_ext)->ht_super;
}
