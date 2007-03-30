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

#include <xml_schema_any_attribute.h>
#include <axis2_utils.h>


typedef struct xml_schema_any_attribute_impl
            xml_schema_any_attribute_impl_t;

/**
 *   @breaf  xml_schema_any_attribute_impl
 */
struct xml_schema_any_attribute_impl
{
    xml_schema_any_attribute_t any_attr;

    xml_schema_annotated_t *annotated;

    xml_schema_content_processing_t *process_content;
    /**
     * Namespaces containing the elements that can be used.
     */
    xml_schema_types_t obj_type;

    axutil_hash_t* ht_super;

    axis2_char_t *ns;
};

#define AXIS2_INTF_TO_IMPL(any_attr) \
        ((xml_schema_any_attribute_impl_t *) any_attr)

/******************* function prototypes **********************************/
axis2_status_t AXIS2_CALL
xml_schema_any_attribute_free(void *any_attr,
        const axutil_env_t *env);

xml_schema_annotated_t *AXIS2_CALL
xml_schema_any_attribute_get_base_impl(void *any_attr,
        const axutil_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_any_attribute_get_type(void *any_attr,
        const axutil_env_t *env);

axutil_hash_t* AXIS2_CALL
xml_schema_any_attribute_super_objs(void *any_attr,
        const axutil_env_t *env);

axis2_char_t *AXIS2_CALL
xml_schema_any_attribute_get_namespace(void *any_attr,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_any_attribute_set_namespace(void *any_attr,
        const axutil_env_t *env,
        axis2_char_t *ns);

xml_schema_content_processing_t *AXIS2_CALL
xml_schema_any_attribute_get_process_content(void *any_attr,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_any_attribute_set_process_content(void *any_attr,
        const axutil_env_t *env,
        xml_schema_content_processing_t *process_content);

/************************ end function prototypes ******************************/

AXIS2_EXTERN xml_schema_any_attribute_t * AXIS2_CALL
xml_schema_any_attribute_create(const axutil_env_t *env)
{
    xml_schema_any_attribute_impl_t *any_attr_impl = NULL;

    any_attr_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_any_attribute_impl_t));
    if (!any_attr_impl)
    {
        AXIS2_ERROR_SET(env->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    any_attr_impl->any_attr.base.ops = NULL;
    any_attr_impl->any_attr.ops = NULL;
    any_attr_impl->ns = NULL;
    any_attr_impl->annotated = NULL;
    any_attr_impl->process_content = NULL;
    any_attr_impl->ht_super = NULL;
    any_attr_impl->obj_type = XML_SCHEMA_ANY_ATTRIBUTE;
    any_attr_impl->any_attr.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_any_attribute_ops_t));

    if (!any_attr_impl->any_attr.ops)
    {
        xml_schema_any_attribute_free(&(any_attr_impl->any_attr), env);
        AXIS2_ERROR_SET(env->error ,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    any_attr_impl->process_content = xml_schema_content_processing_create(env,
            "none");
    if (!any_attr_impl->process_content)
    {
        xml_schema_any_attribute_free(&(any_attr_impl->any_attr), env);
        return NULL;
    }
    any_attr_impl->any_attr.ops->free =
        xml_schema_any_attribute_free;

    any_attr_impl->any_attr.ops->get_type =
        xml_schema_any_attribute_get_type;

    any_attr_impl->any_attr.ops->super_objs =
        xml_schema_any_attribute_super_objs;

    any_attr_impl->any_attr.ops->get_base_impl =
        xml_schema_any_attribute_get_base_impl;

    any_attr_impl->any_attr.ops->get_namespace =
        xml_schema_any_attribute_get_namespace;

    any_attr_impl->any_attr.ops->set_namespace =
        xml_schema_any_attribute_set_namespace;

    any_attr_impl->any_attr.ops->get_process_content =
        xml_schema_any_attribute_get_process_content;

    any_attr_impl->any_attr.ops->set_process_content =
        xml_schema_any_attribute_set_process_content;

    any_attr_impl->annotated = xml_schema_annotated_create(env);
    if (!any_attr_impl->annotated)
    {
        xml_schema_any_attribute_free(&(any_attr_impl->any_attr),
                env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
    }

    any_attr_impl->ht_super = axutil_hash_make(env);
    if (!any_attr_impl->ht_super)
    {
        xml_schema_any_attribute_free(&(any_attr_impl->any_attr), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axutil_hash_set(any_attr_impl->ht_super,
            axis2_strdup(env, "XML_SCHEMA_ANY_ATTRIBUTE"),
            AXIS2_HASH_KEY_STRING, &(any_attr_impl->any_attr));

    axutil_hash_set(any_attr_impl->ht_super,
            axis2_strdup(env, "XML_SCHEMA_ANNOTATED"),
            AXIS2_HASH_KEY_STRING, any_attr_impl->annotated);

    axutil_hash_set(any_attr_impl->ht_super,
            axis2_strdup(env, "XML_SCHEMA_OBJ"),
            AXIS2_HASH_KEY_STRING,
            XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(any_attr_impl->annotated, env));

    xml_schema_annotated_resolve_methods(
        &(any_attr_impl->any_attr.base), env, any_attr_impl->annotated,
        xml_schema_any_attribute_super_objs,
        xml_schema_any_attribute_get_type,
        xml_schema_any_attribute_free);
    return &(any_attr_impl->any_attr);
}

axis2_status_t AXIS2_CALL
xml_schema_any_attribute_free(void *any_attr,
        const axutil_env_t *env)
{
    xml_schema_any_attribute_impl_t *any_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    any_attr_impl = AXIS2_INTF_TO_IMPL(any_attr);

    if (any_attr_impl->ns)
    {
        AXIS2_FREE(env->allocator, any_attr_impl->ns);
        any_attr_impl->ns = NULL;
    }
    if (any_attr_impl->ht_super)
    {
        axutil_hash_free(any_attr_impl->ht_super, env);
        any_attr_impl->ht_super = NULL;
    }
    if (any_attr_impl->process_content)
    {
        XML_SCHEMA_CONTENT_PROCESSING_FREE(any_attr_impl->process_content, env);
        any_attr_impl->process_content = NULL;
    }

    if (any_attr_impl->annotated)
    {
        XML_SCHEMA_ANNOTATED_FREE(any_attr_impl->annotated, env);
        any_attr_impl->annotated = NULL;
    }

    if (any_attr_impl->any_attr.ops)
    {
        AXIS2_FREE(env->allocator, any_attr_impl->any_attr.ops);
        any_attr_impl->any_attr.ops = NULL;
    }
    if (any_attr_impl->any_attr.base.ops)
    {
        AXIS2_FREE(env->allocator, any_attr_impl->any_attr.base.ops);
        any_attr_impl->any_attr.base.ops = NULL;
    }

    AXIS2_FREE(env->allocator, any_attr_impl);
    any_attr_impl = NULL;
    return AXIS2_SUCCESS;
}

xml_schema_annotated_t *AXIS2_CALL
xml_schema_any_attribute_get_base_impl(void *any_attr,
        const axutil_env_t *env)
{
    xml_schema_any_attribute_impl_t *any_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    any_attr_impl = AXIS2_INTF_TO_IMPL(any_attr);
    return any_attr_impl->annotated;
}

axis2_char_t *AXIS2_CALL
xml_schema_any_attribute_get_namespace(void *any_attr,
        const axutil_env_t *env)
{
    xml_schema_any_attribute_impl_t *any_attr_impl = NULL;
    any_attr_impl = AXIS2_INTF_TO_IMPL(any_attr);

    return any_attr_impl->ns;
}

axis2_status_t AXIS2_CALL
xml_schema_any_attribute_set_namespace(void *any_attr,
        const axutil_env_t *env,
        axis2_char_t *namespc)
{
    xml_schema_any_attribute_impl_t *any_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    any_attr_impl = AXIS2_INTF_TO_IMPL(any_attr);

    if (any_attr_impl->ns)
    {
        AXIS2_FREE(env->allocator, any_attr_impl->ns);
        any_attr_impl->ns = NULL;
    }

    any_attr_impl->ns = axis2_strdup(env, namespc);

    return AXIS2_SUCCESS;
}

xml_schema_content_processing_t *AXIS2_CALL
xml_schema_any_attribute_get_process_content(void *any_attr,
        const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(any_attr)->process_content;
}


axis2_status_t AXIS2_CALL
xml_schema_any_attribute_set_process_content(
    void *any_attr,
    const axutil_env_t *env,
    xml_schema_content_processing_t *process_content)
{
    xml_schema_any_attribute_impl_t *any_attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    any_attr_impl = AXIS2_INTF_TO_IMPL(any_attr);

    if (any_attr_impl->process_content)
    {
        XML_SCHEMA_CONTENT_PROCESSING_FREE(
            any_attr_impl->process_content, env);
        any_attr_impl->process_content = NULL;
    }
    any_attr_impl->process_content = process_content;
    return AXIS2_SUCCESS;
}


xml_schema_types_t AXIS2_CALL
xml_schema_any_attribute_get_type(void *any_attr,
        const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(any_attr)->obj_type;
}

axutil_hash_t* AXIS2_CALL
xml_schema_any_attribute_super_objs(void *any_attr,
        const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(any_attr)->ht_super;
}

