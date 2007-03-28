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

#include <xml_schema_obj.h>
#include <axis2_hash.h>
#include <axis2_utils.h>

/**
 * @brief xml_schema_obj struct impl
 */
typedef struct xml_schema_obj_impl
{
    xml_schema_obj_t obj;

    xml_schema_types_t obj_type;

    axis2_hash_t *ht_super;

    int line_num;

    int line_pos;

    axis2_hash_t *meta_info_map;

    axis2_char_t *source_uri;

}
xml_schema_obj_impl_t;

#define AXIS2_INTF_TO_IMPL(obj) \
        ((xml_schema_obj_impl_t *) obj)

/***************** function pointers ******************************************/

axis2_status_t AXIS2_CALL
xml_schema_obj_free(
    void *obj,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
xml_schema_obj_super_objs(
    void *obj,
    const axis2_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_obj_get_type(
    void *obj,
    const axis2_env_t *env);

axis2_char_t * AXIS2_CALL
xml_schema_obj_get_source_uri(
    void *obj,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_obj_set_source_uri(
    void *obj,
    const axis2_env_t *env,
    axis2_char_t *source_uri);

axis2_bool_t AXIS2_CALL
xml_schema_obj_equals(
    void *obj,
    const axis2_env_t *env,
    void *obj_comp);

int AXIS2_CALL
xml_schema_obj_get_line_num(
    void *obj,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_obj_set_line_num(
    void *obj,
    const axis2_env_t *env,
    int line_num);

int AXIS2_CALL
xml_schema_obj_get_line_pos(
    void *obj,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_obj_set_line_pos(
    void *obj,
    const axis2_env_t *env,
    int line_pos);

axis2_status_t AXIS2_CALL
xml_schema_obj_add_meta_info(
    void *obj,
    const axis2_env_t *env,
    axis2_char_t *key,
    void *value);

axis2_hash_t* AXIS2_CALL
xml_schema_obj_get_meta_info(
    void *obj,
    const axis2_env_t *env);

AXIS2_EXTERN xml_schema_obj_t * AXIS2_CALL
xml_schema_obj_create(const axis2_env_t *env)
{
    xml_schema_obj_impl_t *obj_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    obj_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_obj_impl_t));
    if (!obj_impl)
    {
        AXIS2_ERROR_SET(env->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    obj_impl->obj_type = XML_SCHEMA_OBJ;
    obj_impl->ht_super = NULL;
    obj_impl->line_num = -1;
    obj_impl->line_pos = -1;
    obj_impl->source_uri = NULL;
    obj_impl->meta_info_map = NULL;
    obj_impl->obj.ops   = NULL;

    obj_impl->obj.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_obj_ops_t));
    if (!obj_impl->obj.ops)
    {
        xml_schema_obj_free(&(obj_impl->obj), env);
        AXIS2_ERROR_SET(env->error ,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    obj_impl->obj.ops->free =
        xml_schema_obj_free;
    obj_impl->obj.ops->super_objs =
        xml_schema_obj_super_objs;
    obj_impl->obj.ops->get_type =
        xml_schema_obj_get_type;
    obj_impl->obj.ops->get_line_num =
        xml_schema_obj_get_line_num;
    obj_impl->obj.ops->set_line_num =
        xml_schema_obj_set_line_num;
    obj_impl->obj.ops->get_line_pos =
        xml_schema_obj_get_line_pos;
    obj_impl->obj.ops->set_line_pos =
        xml_schema_obj_set_line_pos;
    obj_impl->obj.ops->get_source_uri =
        xml_schema_obj_get_source_uri;
    obj_impl->obj.ops->set_source_uri =
        xml_schema_obj_set_source_uri;
    obj_impl->obj.ops->equals =
        xml_schema_obj_equals;
    obj_impl->obj.ops->get_meta_info =
        xml_schema_obj_get_meta_info;
    obj_impl->obj.ops->add_meta_info =
        xml_schema_obj_add_meta_info;

    obj_impl->ht_super = axis2_hash_make(env);

    if (!obj_impl->ht_super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(obj_impl->ht_super, "XML_SCHEMA_OBJ", AXIS2_HASH_KEY_STRING,
            &(obj_impl->obj));

    return &(obj_impl->obj);
}


axis2_status_t AXIS2_CALL
xml_schema_obj_free(void *obj,
        const axis2_env_t *env)
{
    xml_schema_obj_impl_t *obj_impl = NULL;

    obj_impl = AXIS2_INTF_TO_IMPL(obj);

    if (obj_impl->source_uri)
    {
        AXIS2_FREE(env->allocator, obj_impl->source_uri);
        obj_impl->source_uri = NULL;
    }

    if (obj_impl->ht_super)
    {
        axis2_hash_free(obj_impl->ht_super, env);
        obj_impl->ht_super = NULL;
    }

    if (obj_impl->obj.ops)
    {
        AXIS2_FREE(env->allocator, obj_impl->obj.ops);
        obj_impl->obj.ops = NULL;
    }

    if (obj_impl)
    {
        AXIS2_FREE(env->allocator, obj_impl);
        obj_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
xml_schema_obj_super_objs(void *obj,
        const axis2_env_t *env)
{
    xml_schema_obj_impl_t *obj_impl = NULL;
    obj_impl = AXIS2_INTF_TO_IMPL(obj);
    return obj_impl->ht_super;
}

xml_schema_types_t AXIS2_CALL
xml_schema_obj_get_type(
    void *obj,
    const axis2_env_t *env)
{
    xml_schema_obj_impl_t *obj_impl = NULL;
    obj_impl = AXIS2_INTF_TO_IMPL(obj);
    return obj_impl->obj_type;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
xml_schema_obj_resolve_methods(
    xml_schema_obj_t *obj,
    const axis2_env_t *env,
    xml_schema_obj_t *obj_impl,
    XML_SCHEMA_SUPER_OBJS_FN super_objs,
    XML_SCHEMA_GET_TYPE_FN get_type,
    XML_SCHEMA_FREE_FN free_fn)
{
    xml_schema_obj_impl_t *obj_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, obj_impl, AXIS2_FAILURE);

    obj_impl_l = (xml_schema_obj_impl_t *) obj_impl;

    obj->ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_obj_ops_t));
    if (!obj->ops)
    {
        AXIS2_ERROR_SET(env->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    obj->ops->free = free_fn;

    obj->ops->super_objs = super_objs;

    obj->ops->get_type = get_type;

    obj->ops->get_line_num =
        obj_impl_l->obj.ops->get_line_num;

    obj->ops->set_line_num =
        obj_impl_l->obj.ops->set_line_num;

    obj->ops->get_line_pos =
        obj_impl_l->obj.ops->get_line_pos;

    obj->ops->set_line_pos =
        obj_impl_l->obj.ops->set_line_pos;

    obj->ops->get_source_uri =
        obj_impl_l->obj.ops->get_source_uri;

    obj->ops->set_source_uri =
        obj_impl_l->obj.ops->set_source_uri;

    obj->ops->equals = obj_impl_l->obj.ops->equals;

    return AXIS2_SUCCESS;
}
int AXIS2_CALL
xml_schema_obj_get_line_num(void *obj,
        const axis2_env_t *env)
{
    xml_schema_obj_impl_t *obj_impl = NULL;
    axis2_hash_t *super = NULL;

    super = XML_SCHEMA_OBJ_SUPER_OBJS(obj, env);
    if (super)
    {
        obj_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super,
                "XML_SCHEMA_OBJ", AXIS2_HASH_KEY_STRING));
        if (obj_impl)
            return obj_impl->line_num;
    }
    return -1;
}

axis2_status_t AXIS2_CALL
xml_schema_obj_set_line_num(void *obj,
        const axis2_env_t *env,
        int line_num)
{
    xml_schema_obj_impl_t *obj_impl = NULL;
    axis2_hash_t *super = NULL;

    super = XML_SCHEMA_OBJ_SUPER_OBJS(obj, env);
    if (super)
    {
        obj_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, "XML_SCHEMA_OBJ",
                AXIS2_HASH_KEY_STRING));
    }
    else
    {
        return AXIS2_FAILURE;
    }
    obj_impl->line_num = line_num;

    return AXIS2_SUCCESS;
}

int AXIS2_CALL
xml_schema_obj_get_line_pos(void *obj,
        const axis2_env_t *env)
{
    xml_schema_obj_impl_t *obj_impl = NULL;
    axis2_hash_t *super = NULL;

    super = XML_SCHEMA_OBJ_SUPER_OBJS(obj, env);
    if (super)
    {
        obj_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, "XML_SCHEMA_OBJ",
                AXIS2_HASH_KEY_STRING));
    }
    return obj_impl->line_pos;
}

axis2_status_t AXIS2_CALL
xml_schema_obj_set_line_pos(void *obj,
        const axis2_env_t *env,
        int line_pos)
{
    xml_schema_obj_impl_t *obj_impl = NULL;
    axis2_hash_t *super = NULL;

    super = XML_SCHEMA_OBJ_SUPER_OBJS(obj, env);
    if (super)
    {
        obj_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, "XML_SCHEMA_OBJ",
                AXIS2_HASH_KEY_STRING));
        if (obj_impl)
            obj_impl->line_pos = line_pos;
    }
    return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL
xml_schema_obj_get_source_uri(void *obj,
        const axis2_env_t *env)
{
    xml_schema_obj_impl_t *obj_impl = NULL;
    axis2_hash_t *super = NULL;
    super = XML_SCHEMA_OBJ_SUPER_OBJS(obj, env);
    obj_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, "XML_SCHEMA_OBJ",
            AXIS2_HASH_KEY_STRING));
    return obj_impl->source_uri;
}

axis2_status_t AXIS2_CALL
xml_schema_obj_set_source_uri(void *obj,
        const axis2_env_t *env,
        axis2_char_t *source_uri)
{
    xml_schema_obj_impl_t *obj_impl = NULL;
    axis2_hash_t *super = NULL;

    super = XML_SCHEMA_OBJ_SUPER_OBJS(obj, env);
    obj_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, "XML_SCHEMA_OBJ",
            AXIS2_HASH_KEY_STRING));

    if (obj_impl->source_uri)
    {
        AXIS2_FREE(env->allocator, obj_impl->source_uri);
        obj_impl->source_uri = NULL;
    }
    obj_impl->source_uri = axis2_strdup(env, source_uri);

    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
xml_schema_obj_equals(void *obj,
        const axis2_env_t *env,
        void *obj_comp)
{
    xml_schema_obj_impl_t *obj_impl = NULL;
    axis2_hash_t *super = NULL;

    super = XML_SCHEMA_OBJ_SUPER_OBJS(obj, env);
    obj_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, "XML_SCHEMA_OBJ",
            AXIS2_HASH_KEY_STRING));
    /****
        TODO

    ***/
    return AXIS2_TRUE;
}

axis2_status_t AXIS2_CALL
xml_schema_obj_add_meta_info(void *obj,
        const axis2_env_t *env,
        axis2_char_t *key,
        void *value)
{
    xml_schema_obj_impl_t *obj_impl = NULL;
    axis2_hash_t *ht_super = NULL;

    ht_super = XML_SCHEMA_OBJ_SUPER_OBJS(obj, env);
    if (ht_super)
    {
        obj_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, "XML_SCHEMA_OBJ",
                AXIS2_HASH_KEY_STRING));
        if (!obj_impl->meta_info_map)
        {
            obj_impl->meta_info_map = axis2_hash_make(env);
            if (!obj_impl->meta_info_map)
            {
                AXIS2_ERROR_SET((env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            axis2_hash_set(obj_impl->meta_info_map, key, AXIS2_HASH_KEY_STRING,
                    value);
        }
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}

axis2_hash_t* AXIS2_CALL
xml_schema_obj_get_meta_info(void *obj,
        const axis2_env_t *env)
{
    xml_schema_obj_impl_t *obj_impl = NULL;
    axis2_hash_t *ht_super = NULL;
    ht_super = XML_SCHEMA_OBJ_SUPER_OBJS(obj, env);
    if (ht_super)
    {
        obj_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(ht_super, "XML_SCHEMA_OBJ",
                AXIS2_HASH_KEY_STRING));
        return obj_impl->meta_info_map;
    }
    return NULL;
}
