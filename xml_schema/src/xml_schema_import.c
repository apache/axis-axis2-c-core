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

#include  <xml_schema_import.h>
#include  <xml_schema.h>
#include <axis2_utils.h>
/**
 * @brief xml_schema_import_impl
 */
typedef struct xml_schema_import_impl
{
    xml_schema_import_t import;

    xml_schema_external_t *external;

    axis2_char_t *ns;

    axis2_hash_t *ht_super;

    xml_schema_types_t obj_type;

}
xml_schema_import_impl_t;

#define AXIS2_INTF_TO_IMPL(import) \
        ((xml_schema_import_impl_t *) import)

/********************* function prototypes ***********************************/

axis2_status_t AXIS2_CALL
xml_schema_import_free(void *import,
        const axutil_env_t *env);

xml_schema_external_t *AXIS2_CALL
xml_schema_import_get_base_impl(void *import,
        const axutil_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_import_get_type(void *import,
        const axutil_env_t *env);

axis2_hash_t *AXIS2_CALL
xml_schema_import_super_objs(void *import,
        const axutil_env_t *env);
axis2_char_t* AXIS2_CALL
xml_schema_import_get_namespace(void *import,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_import_set_namespace(void *import,
        const axutil_env_t *env,
        axis2_char_t *ns);

/********************* end function prototypes ********************************/

AXIS2_EXTERN xml_schema_import_t * AXIS2_CALL
xml_schema_import_create(const axutil_env_t *env)
{
    xml_schema_import_impl_t *import_impl = NULL;
    xml_schema_annotated_t *annotated = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    import_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_import_impl_t));
    if (!import_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    import_impl->ns = NULL;
    import_impl->external = NULL;
    import_impl->import.ops = NULL;
    import_impl->import.base.ops = NULL;
    import_impl->ht_super = NULL;
    import_impl->obj_type = XML_SCHEMA_IMPORT;

    import_impl->import.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_import_ops_t));

    if (!import_impl->import.ops)
    {
        xml_schema_import_free(&(import_impl->import), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    import_impl->import.ops->free =
        xml_schema_import_free;
    import_impl->import.ops->get_base_impl =
        xml_schema_import_get_base_impl;
    import_impl->import.ops->get_type =
        xml_schema_import_get_type;
    import_impl->import.ops->super_objs =
        xml_schema_import_super_objs;
    import_impl->import.ops->get_namespace =
        xml_schema_import_get_namespace;
    import_impl->import.ops->set_namespace =
        xml_schema_import_set_namespace;

    import_impl->ht_super = axis2_hash_make(env);
    if (!import_impl->ht_super)
    {
        xml_schema_import_free(&(import_impl->import), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }


    import_impl->external = xml_schema_external_create(env);
    if (!import_impl->external)
    {
        xml_schema_import_free(&(import_impl->import), env);
        return NULL;
    }

    axis2_hash_set(import_impl->ht_super, axis2_strdup(env, "XML_SCHEMA_IMPORT"),
            AXIS2_HASH_KEY_STRING, &(import_impl->import));
    axis2_hash_set(import_impl->ht_super, axis2_strdup(env, "XML_SCHEMA_EXTERNAL"),
            AXIS2_HASH_KEY_STRING, import_impl->external);

    annotated = XML_SCHEMA_EXTERNAL_GET_BASE_IMPL(import_impl->external, env);
    if (annotated)
    {

        axis2_hash_set(import_impl->ht_super, axis2_strdup(env, "XML_SCHEMA_ANNOTATED"),
                AXIS2_HASH_KEY_STRING, annotated);
        axis2_hash_set(import_impl->ht_super, axis2_strdup(env, "XML_SCHEMA_OBJ"),
                AXIS2_HASH_KEY_STRING,
                XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(annotated, env));
    }

    status = xml_schema_external_resolve_methods(
                &(import_impl->import.base), env, import_impl->external,
                xml_schema_import_super_objs,
                xml_schema_import_get_type,
                xml_schema_import_free);
    return &(import_impl->import);
}

axis2_status_t AXIS2_CALL
xml_schema_import_free(void *import,
        const axutil_env_t *env)
{
    xml_schema_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    import_impl = AXIS2_INTF_TO_IMPL(import);

    if (import_impl->ht_super)
    {
        axis2_hash_free(import_impl->ht_super, env);
        import_impl->ht_super = NULL;
    }
    if (import_impl->external)
    {
        XML_SCHEMA_EXTERNAL_FREE(import_impl->external, env);
        import_impl->external = NULL;
    }

    if (import_impl->import.ops)
    {
        AXIS2_FREE(env->allocator, import_impl->import.ops);
        import_impl->import.ops = NULL;
    }
    if (import_impl->import.base.ops)
    {
        AXIS2_FREE(env->allocator, import_impl->import.base.ops);
        import_impl->import.base.ops = NULL;
    }
    AXIS2_FREE(env->allocator, import_impl);
    import_impl = NULL;
    return AXIS2_SUCCESS;
}

xml_schema_external_t *AXIS2_CALL
xml_schema_import_get_base_impl(void *import,
        const axutil_env_t *env)
{
    xml_schema_import_impl_t *import_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    import_impl = AXIS2_INTF_TO_IMPL(import);

    return import_impl->external;
}

axis2_char_t * AXIS2_CALL
xml_schema_import_get_namespace(void *import,
        const axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(import)->ns;
}

axis2_status_t AXIS2_CALL
xml_schema_import_set_namespace(void *import,
        const axutil_env_t *env,
        axis2_char_t *ns)
{
    xml_schema_import_impl_t *import_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    import_impl = AXIS2_INTF_TO_IMPL(import);
    if (import_impl->ns)
    {
        AXIS2_FREE(env->allocator, ns);
        import_impl->ns = NULL;
    }
    import_impl->ns = ns;
    return AXIS2_SUCCESS;
}

xml_schema_types_t AXIS2_CALL
xml_schema_import_get_type(void *import,
        const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(import)->obj_type;
}

axis2_hash_t *AXIS2_CALL
xml_schema_import_super_objs(void *import,
        const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(import)->ht_super;
}
