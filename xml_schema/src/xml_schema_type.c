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


#include <xml_schema_constants.h>
#include <xml_schema_type.h>
#include <xml_schema_data_type.h>
#include <xml_schema_derivation_method.h>
#include <xml_schema.h>
#include <axis2_qname.h>

typedef struct xml_schema_type_impl
            xml_schema_type_impl_t;

/**
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension
 */
struct xml_schema_type_impl
{
    xml_schema_type_t type;
    /** parent type */
    void *base_schema_type;

    xml_schema_annotated_t *annotated;

    xml_schema_data_type_t *data_type;

    xml_schema_derivation_method_t *derive_by;

    xml_schema_derivation_method_t *final_derivation;

    xml_schema_derivation_method_t *final_resolved;

    axis2_bool_t is_mixed;
    /* Name of the type */

    axis2_char_t *name;

    xml_schema_t *schema;

    axis2_qname_t *qname;

    axutil_hash_t *ht_super;

    xml_schema_types_t obj_type;
};

#define AXIS2_INTF_TO_IMPL(type) \
        ((xml_schema_type_impl_t *) type)

/************************** function prototypes ******************************/

axis2_status_t AXIS2_CALL
xml_schema_type_free(void *type,
        const axutil_env_t *env);

xml_schema_annotated_t *AXIS2_CALL
xml_schema_type_get_base_impl(void *type,
        const axutil_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_type_get_type(void *type,
        const axutil_env_t *env);

axutil_hash_t*  AXIS2_CALL
xml_schema_type_super_objs(void *type,
        const axutil_env_t *env);

void *AXIS2_CALL
xml_schema_type_get_base_schema_type(void *type,
        const axutil_env_t *env);

xml_schema_data_type_t *AXIS2_CALL
xml_schema_type_get_data_type(void *type,
        const axutil_env_t *env);

xml_schema_derivation_method_t *AXIS2_CALL
xml_schema_type_get_derive_by(void *type,
        const axutil_env_t *env);

xml_schema_derivation_method_t *AXIS2_CALL
xml_schema_type_get_final(void *type,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_type_set_final(void *type,
        const axutil_env_t *env,
        xml_schema_derivation_method_t *final_derivation);

xml_schema_derivation_method_t *AXIS2_CALL
xml_schema_type_get_final_resolved(void *type,
        const axutil_env_t *env);

axis2_bool_t AXIS2_CALL
xml_schema_type_is_mixed(void *type,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_type_set_mixed(void *type,
        const axutil_env_t *env,
        axis2_bool_t is_mixed);

axis2_char_t *AXIS2_CALL
xml_schema_type_get_name(void *type,
        const axutil_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_type_set_name(void *type,
        const axutil_env_t *env,
        axis2_char_t *name);

axis2_qname_t *AXIS2_CALL
xml_schema_type_get_qname(void *type,
        const axutil_env_t *env);

/************************** end       ****************************************/

AXIS2_EXTERN xml_schema_type_t * AXIS2_CALL
xml_schema_type_create(const axutil_env_t *env,
        xml_schema_t *schema)
{
    xml_schema_type_impl_t *type_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    type_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_type_impl_t));
    if (!type_impl)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    type_impl->type.base.ops = NULL;
    type_impl->name = NULL;
    type_impl->derive_by = NULL;
    type_impl->final_derivation = NULL;
    type_impl->final_resolved = NULL;
    type_impl->schema = NULL;
    type_impl->data_type = NULL;
    type_impl->annotated = NULL;
    type_impl->base_schema_type = NULL;
    type_impl->qname = NULL;
    type_impl->ht_super = NULL;
    type_impl->obj_type = XML_SCHEMA_TYPE;
    type_impl->schema = schema;

    type_impl->type.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_type_ops_t));


    if (!type_impl->type.ops)
    {
        xml_schema_type_free(&(type_impl->type), env);
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    type_impl->type.ops->free =
        xml_schema_type_free;
    type_impl->type.ops->get_base_impl =
        xml_schema_type_get_base_impl;
    type_impl->type.ops->get_type =
        xml_schema_type_get_type;
    type_impl->type.ops->super_objs =
        xml_schema_type_super_objs;
    type_impl->type.ops->get_base_schema_type =
        xml_schema_type_get_base_schema_type;
    type_impl->type.ops->get_data_type =
        xml_schema_type_get_data_type;
    type_impl->type.ops->get_derive_by =
        xml_schema_type_get_derive_by;
    type_impl->type.ops->get_final =
        xml_schema_type_get_final;
    type_impl->type.ops->set_final =
        xml_schema_type_set_final;
    type_impl->type.ops->get_final_resolved =
        xml_schema_type_get_final_resolved;
    type_impl->type.ops->is_mixed =
        xml_schema_type_is_mixed;
    type_impl->type.ops->set_mixed =
        xml_schema_type_set_mixed;
    type_impl->type.ops->get_name =
        xml_schema_type_get_name;
    type_impl->type.ops->set_name =
        xml_schema_type_set_name;
    type_impl->type.ops->get_qname =
        xml_schema_type_get_qname;



    type_impl->final_derivation = xml_schema_derivation_method_create(env,
            "none");
    if (!(type_impl->final_derivation))
    {
        xml_schema_type_free(&(type_impl->type), env);
        return NULL;
    }


    type_impl->annotated = xml_schema_annotated_create(env);
    if (!type_impl->annotated)
    {
        xml_schema_type_free(&(type_impl->annotated), env);
        return NULL;
    }
    type_impl->ht_super = axutil_hash_make(env);
    if (!type_impl->ht_super)
    {
        xml_schema_type_free(&(type_impl->annotated), env);
        return NULL;
    }
    axutil_hash_set(type_impl->ht_super,
            axis2_strdup(env, "XML_SCHEMA_TYPE"),
            AXIS2_HASH_KEY_STRING, &(type_impl->type));

    axutil_hash_set(type_impl->ht_super,
            axis2_strdup(env, "XML_SCHEMA_ANNOTATED"),
            AXIS2_HASH_KEY_STRING, type_impl->annotated);

    axutil_hash_set(type_impl->ht_super,
            axis2_strdup(env, "XML_SCHEMA_OBJ"),
            AXIS2_HASH_KEY_STRING,
            XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(type_impl->annotated, env));


    status = xml_schema_annotated_resolve_methods(
                &(type_impl->type.base), env, type_impl->annotated,
                xml_schema_type_super_objs,
                xml_schema_type_get_type,
                xml_schema_type_free);

    return &(type_impl->type);
}

axis2_status_t AXIS2_CALL
xml_schema_type_free(void *type,
        const axutil_env_t *env)
{
    xml_schema_type_impl_t *type_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    type_impl = AXIS2_INTF_TO_IMPL(type);
    if (type_impl->qname)
    {
        axis2_qname_free(type_impl->qname, env);
        type_impl->qname = NULL;

    }
    if (type_impl->annotated)
    {
        XML_SCHEMA_ANNOTATED_FREE(type_impl->annotated, env);
        type_impl->annotated = NULL;
    }

    if (type_impl->type.ops)
    {
        AXIS2_FREE(env->allocator, type_impl->type.ops);
        type_impl->type.ops = NULL;
    }
    if (type_impl->type.base.ops)
    {
        AXIS2_FREE(env->allocator, type_impl->type.base.ops);
        type_impl->type.base.ops = NULL;
    }
    AXIS2_FREE(env->allocator, type_impl);
    type_impl = NULL;
    return AXIS2_SUCCESS;
}

xml_schema_annotated_t *AXIS2_CALL
xml_schema_type_get_base_impl(void *type,
        const axutil_env_t *env)
{
    xml_schema_type_impl_t *type_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    type_impl = AXIS2_INTF_TO_IMPL(type);
    return type_impl->annotated;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
xml_schema_type_resolve_methods(
    xml_schema_type_t *type,
    const axutil_env_t *env,
    xml_schema_type_t *type_impl,
    XML_SCHEMA_SUPER_OBJS_FN super_objs,
    XML_SCHEMA_GET_TYPE_FN get_type,
    XML_SCHEMA_FREE_FN free_fn)
{
    xml_schema_type_impl_t *sch_type_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, type_impl, AXIS2_FAILURE);

    sch_type_impl = (xml_schema_type_impl_t *) type_impl;

    type->ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_type_ops_t));
    if (!type->ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    type->ops->free = free_fn;
    type->ops->get_type = get_type;
    type->ops->super_objs = super_objs;

    type->ops->get_base_schema_type =
        sch_type_impl->type.ops->get_base_schema_type;

    type->ops->get_data_type =
        sch_type_impl->type.ops->get_data_type;

    type->ops->get_derive_by =
        sch_type_impl->type.ops->get_derive_by;

    type->ops->get_final =
        sch_type_impl->type.ops->get_final;

    type->ops->set_final =
        sch_type_impl->type.ops->set_final;
    type->ops->get_final_resolved =
        sch_type_impl->type.ops->get_final_resolved;

    type->ops->is_mixed =
        sch_type_impl->type.ops->is_mixed;

    type->ops->set_mixed =
        sch_type_impl->type.ops->set_mixed;

    type->ops->get_name =
        sch_type_impl->type.ops->get_name;

    type->ops->set_name =
        sch_type_impl->type.ops->set_name;

    type->ops->get_qname =
        sch_type_impl->type.ops->get_qname;

    return xml_schema_annotated_resolve_methods(&(type->base),
            env, sch_type_impl->annotated, super_objs, get_type, free_fn);
}

void *AXIS2_CALL
xml_schema_type_get_base_schema_type(void *type,
        const axutil_env_t *env)
{
    xml_schema_type_impl_t *type_impl = NULL;
    axutil_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if (ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axutil_hash_get(ht_super,
                "XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if (!type_impl)
            return NULL;
    }
    return type_impl->base_schema_type;
}

xml_schema_data_type_t *AXIS2_CALL
xml_schema_type_get_data_type(void *type,
        const axutil_env_t *env)
{
    xml_schema_type_impl_t *type_impl = NULL;
    axutil_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if (ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axutil_hash_get(ht_super,
                "XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if (!type_impl)
            return NULL;
    }
    return type_impl->data_type;
}

xml_schema_derivation_method_t *AXIS2_CALL
xml_schema_type_get_derive_by(void *type,
        const axutil_env_t *env)
{
    xml_schema_type_impl_t *type_impl = NULL;
    axutil_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if (ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axutil_hash_get(ht_super,
                "XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if (!type_impl)
            return NULL;
    }
    return type_impl->derive_by;
}

xml_schema_derivation_method_t *AXIS2_CALL
xml_schema_type_get_final(void *type,
        const axutil_env_t *env)
{
    xml_schema_type_impl_t *type_impl = NULL;
    axutil_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if (ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axutil_hash_get(ht_super,
                "XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if (!type_impl)
            return NULL;
    }
    return type_impl->final_derivation;
}

axis2_status_t AXIS2_CALL
xml_schema_type_set_final(void *type,
        const axutil_env_t *env,
        xml_schema_derivation_method_t *final_derivation)
{
    xml_schema_type_impl_t *type_impl = NULL;
    axutil_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, final_derivation, AXIS2_FAILURE);
    ht_super = XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if (ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axutil_hash_get(ht_super,
                "XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if (!type_impl)
            return AXIS2_FAILURE;
    }
    if (type_impl->final_derivation)
    {
        XML_SCHEMA_DERIVATION_METHOD_FREE(type_impl->final_derivation, env);
        type_impl->final_derivation = NULL;
    }
    type_impl->final_derivation = final_derivation;
    return AXIS2_SUCCESS;
}

xml_schema_derivation_method_t *AXIS2_CALL
xml_schema_type_get_final_resolved(void *type,
        const axutil_env_t *env)
{
    xml_schema_type_impl_t *type_impl = NULL;
    axutil_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if (ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axutil_hash_get(ht_super,
                "XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if (!type_impl)
            return NULL;
    }
    return type_impl->final_resolved;

}

axis2_bool_t AXIS2_CALL
xml_schema_type_is_mixed(void *type,
        const axutil_env_t *env)
{
    xml_schema_type_impl_t *type_impl = NULL;
    axutil_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if (ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axutil_hash_get(ht_super,
                "XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if (!type_impl)
            return AXIS2_FAILURE;
    }
    return type_impl->is_mixed;
}

axis2_status_t AXIS2_CALL
xml_schema_type_set_mixed(void *type,
        const axutil_env_t *env,
        axis2_bool_t is_mixed)
{
    xml_schema_type_impl_t *type_impl = NULL;
    axutil_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ht_super = XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if (ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axutil_hash_get(ht_super,
                "XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if (!type_impl)
            return AXIS2_FAILURE;
    }
    return type_impl->is_mixed = is_mixed;
}

axis2_char_t *AXIS2_CALL
xml_schema_type_get_name(void *type,
        const axutil_env_t *env)
{
    xml_schema_type_impl_t *type_impl = NULL;
    axutil_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if (ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axutil_hash_get(ht_super,
                "XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if (!type_impl)
            return NULL;
    }
    return type_impl->name;
}

axis2_status_t AXIS2_CALL
xml_schema_type_set_name(void *type,
        const axutil_env_t *env,
        axis2_char_t *name)
{
    xml_schema_type_impl_t *type_impl = NULL;
    axutil_hash_t *ht_super = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, name, AXIS2_FAILURE);
    ht_super = XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if (ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axutil_hash_get(ht_super,
                "XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if (!type_impl)
            return AXIS2_FAILURE;
    }
    if (type_impl->name)
    {
        AXIS2_FREE(env->allocator, type_impl->name);
        type_impl->name = NULL;
    }
    type_impl->name = axis2_strdup(env, name);
    if (!type_impl->name)
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
xml_schema_type_get_qname(void *type,
        const axutil_env_t *env)
{
    xml_schema_type_impl_t *type_impl = NULL;
    axutil_hash_t *ht_super = NULL;
    axis2_char_t *target_ns = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ht_super = XML_SCHEMA_TYPE_SUPER_OBJS(type, env);
    if (ht_super)
    {
        type_impl = AXIS2_INTF_TO_IMPL(axutil_hash_get(ht_super,
                "XML_SCHEMA_TYPE", AXIS2_HASH_KEY_STRING));
        if (!type_impl)
            return NULL;
    }

    if (!(type_impl->name))
    {
        return NULL;
    }
    if (type_impl->qname)
    {
        return type_impl->qname;
    }

    if (type_impl->schema)
    {
        target_ns =
            XML_SCHEMA_GET_TARGET_NAMESPACE(type_impl->schema, env);
    }

    type_impl->qname = axis2_qname_create(env, type_impl->name, target_ns, NULL);
    return type_impl->qname;
}

xml_schema_types_t AXIS2_CALL
xml_schema_type_get_type(void *type,
        const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(type)->obj_type;
}

axutil_hash_t*  AXIS2_CALL
xml_schema_type_super_objs(void *type,
        const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(type)->ht_super;
}

