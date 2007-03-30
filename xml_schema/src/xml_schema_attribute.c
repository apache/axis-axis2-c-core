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

#include <xml_schema_attribute.h>
#include <axis2_utils.h>

typedef struct xml_schema_attribute_impl
            xml_schema_attribute_impl_t;

/**
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension
 */
struct xml_schema_attribute_impl
{
    xml_schema_attribute_t attr;

    xml_schema_annotated_t *annotated;

    xml_schema_form_t *form;

    xml_schema_use_t *use;

    void *attribute_type;

    axis2_char_t *default_value;

    axis2_char_t *fixed_value;

    axis2_char_t *name;

    xml_schema_simple_type_t *schema_type;

    axis2_qname_t *schema_type_name;

    axis2_qname_t *qualified_name;

    axis2_qname_t *ref_name;

    xml_schema_types_t obj_type;

    axis2_hash_t* ht_super;
};

#define AXIS2_INTF_TO_IMPL(attr) \
        ((xml_schema_attribute_impl_t *) attr)

/******************* function prototypes **********************************/
axis2_status_t AXIS2_CALL
xml_schema_attribute_free(void *attr,
        const axutil_env_t *env);

xml_schema_annotated_t *AXIS2_CALL
xml_schema_attribute_get_base_impl(void *attr,
        const axutil_env_t *env);

xml_schema_types_t AXIS2_CALL
xml_schema_attribute_get_type(void *attr,
        const axutil_env_t *env);

axis2_hash_t* AXIS2_CALL
xml_schema_attribute_super_objs(void *attr,
        const axutil_env_t *env);

void * AXIS2_CALL
xml_schema_attribute_get_attr_type(
    void *attr,
    const axutil_env_t *env);

axis2_char_t * AXIS2_CALL
xml_schema_attribute_get_default_value(
    void *attr,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
xml_schema_attribute_set_default_value(
    void *attr,
    const axutil_env_t *env,
    axis2_char_t *default_value);

axis2_char_t*  AXIS2_CALL
xml_schema_attribute_get_fixed_value(void *attr,
        const axutil_env_t *env);

axis2_status_t  AXIS2_CALL
xml_schema_attribute_set_fixed_value(void *attr,
        const axutil_env_t *env,
        axis2_char_t *fixed_value);

xml_schema_form_t* AXIS2_CALL
xml_schema_attribute_get_schema_form(void *attr,
        const axutil_env_t *env);

axis2_status_t  AXIS2_CALL
xml_schema_attribute_set_schema_form(void *attr,
        const axutil_env_t *env,
        xml_schema_form_t *schema_form);

axis2_qname_t*  AXIS2_CALL
xml_schema_attribute_get_qname(void *attr,
        const axutil_env_t *env);

axis2_status_t  AXIS2_CALL
xml_schema_attribute_set_qname(void *attr,
        const axutil_env_t *env,
        axis2_qname_t *qualified_name);

axis2_char_t*  AXIS2_CALL
xml_schema_attribute_get_name(void *attr,
        const axutil_env_t *env);

axis2_status_t  AXIS2_CALL
xml_schema_attribute_set_name(void *attr,
        const axutil_env_t *env,
        axis2_char_t *name);

axis2_qname_t*  AXIS2_CALL
xml_schema_attribute_get_ref_name(void *attr,
        const axutil_env_t *env);

axis2_status_t  AXIS2_CALL
xml_schema_attribute_set_ref_name(void *attr,
        const axutil_env_t *env,
        axis2_qname_t *ref_name);

xml_schema_simple_type_t*  AXIS2_CALL
xml_schema_attribute_get_schema_type(void *attr,
        const axutil_env_t *env);

axis2_status_t  AXIS2_CALL
xml_schema_attribute_set_schema_type(void *attr,
        const axutil_env_t *env,
        xml_schema_simple_type_t* sch_type);

axis2_qname_t*  AXIS2_CALL
xml_schema_attribute_get_schema_type_name(void *attr,
        const axutil_env_t *env);


axis2_status_t  AXIS2_CALL
xml_schema_attribute_set_schema_type_name(void *attr,
        const axutil_env_t *env,
        axis2_qname_t *sch_type_name);

xml_schema_use_t*  AXIS2_CALL
xml_schema_attribute_get_use(void *attr,
        const axutil_env_t *env);

axis2_status_t  AXIS2_CALL
xml_schema_attribute_set_use(void *attr,
        const axutil_env_t *env,
        xml_schema_use_t *use);

axis2_char_t*  AXIS2_CALL
xml_schema_attribute_to_string(void *attr,
        const axutil_env_t *env,
        axis2_char_t *prefix,
        int tab);
/************************ end function prototypes ******************************/

AXIS2_EXTERN xml_schema_attribute_t * AXIS2_CALL
xml_schema_attribute_create(const axutil_env_t *env)
{
    xml_schema_attribute_impl_t *attr_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    attr_impl = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_attribute_impl_t));
    if (!attr_impl)
    {
        AXIS2_ERROR_SET(env->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    attr_impl->attr.base.ops = NULL;
    attr_impl->attr.ops = NULL;
    attr_impl->annotated = NULL;
    attr_impl->ht_super = NULL;
    attr_impl->attribute_type = NULL;
    attr_impl->default_value = NULL;
    attr_impl->fixed_value = NULL;
    attr_impl->form = NULL;
    attr_impl->use = NULL;
    attr_impl->obj_type = XML_SCHEMA_ATTRIBUTE;
    attr_impl->schema_type_name = NULL;
    attr_impl->schema_type = NULL;
    attr_impl->qualified_name = NULL;
    attr_impl->name = NULL;
    attr_impl->ref_name = NULL;

    attr_impl->attr.ops = AXIS2_MALLOC(env->allocator,
            sizeof(xml_schema_attribute_ops_t));

    if (!attr_impl->attr.ops)
    {
        xml_schema_attribute_free(&(attr_impl->attr), env);
        AXIS2_ERROR_SET(env->error ,
                AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    attr_impl->use = xml_schema_use_create(env, "none");
    attr_impl->form = xml_schema_form_create(env, "none");

    if (!attr_impl->use || !attr_impl->form)
    {
        xml_schema_attribute_free(&(attr_impl->attr), env);
        return NULL;
    }
    attr_impl->attr.ops->free =
        xml_schema_attribute_free;
    attr_impl->attr.ops->get_type =
        xml_schema_attribute_get_type;
    attr_impl->attr.ops->super_objs =
        xml_schema_attribute_super_objs;
    attr_impl->attr.ops->get_base_impl =
        xml_schema_attribute_get_base_impl;
    attr_impl->attr.ops->get_attr_type =
        xml_schema_attribute_get_attr_type;
    attr_impl->attr.ops->get_default_value =
        xml_schema_attribute_get_default_value;
    attr_impl->attr.ops->set_default_value =
        xml_schema_attribute_set_default_value;
    attr_impl->attr.ops->get_fixed_value =
        xml_schema_attribute_get_fixed_value;
    attr_impl->attr.ops->set_fixed_value =
        xml_schema_attribute_set_fixed_value;
    attr_impl->attr.ops->get_schema_form =
        xml_schema_attribute_get_schema_form;
    attr_impl->attr.ops->set_schema_form =
        xml_schema_attribute_set_schema_form;
    attr_impl->attr.ops->get_qname =
        xml_schema_attribute_get_qname;
    attr_impl->attr.ops->set_qname =
        xml_schema_attribute_set_qname;
    attr_impl->attr.ops->get_name =
        xml_schema_attribute_get_name;
    attr_impl->attr.ops->set_name =
        xml_schema_attribute_set_name;
    attr_impl->attr.ops->set_ref_name =
        xml_schema_attribute_set_ref_name;
    attr_impl->attr.ops->get_schema_type =
        xml_schema_attribute_get_schema_type;
    attr_impl->attr.ops->set_schema_type =
        xml_schema_attribute_set_schema_type;
    attr_impl->attr.ops->get_schema_type_name =
        xml_schema_attribute_get_schema_type_name;
    attr_impl->attr.ops->set_schema_type_name =
        xml_schema_attribute_set_schema_type_name;
    attr_impl->attr.ops->get_use =
        xml_schema_attribute_get_use;
    attr_impl->attr.ops->set_use =
        xml_schema_attribute_set_use;
    attr_impl->attr.ops->to_string =
        xml_schema_attribute_to_string;

    attr_impl->ht_super = axis2_hash_make(env);
    if (!attr_impl->ht_super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    attr_impl->annotated = xml_schema_annotated_create(env);
    if (!attr_impl->annotated)
    {
        xml_schema_attribute_free(&(attr_impl->attr), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    axis2_hash_set(attr_impl->ht_super,
            axis2_strdup(env, "XML_SCHEMA_ATTRIBUTE"),
            AXIS2_HASH_KEY_STRING, &(attr_impl->attr));

    axis2_hash_set(attr_impl->ht_super,
            axis2_strdup(env, "XML_SCHEMA_ANNOTATED"),
            AXIS2_HASH_KEY_STRING, attr_impl->annotated);

    axis2_hash_set(attr_impl->ht_super,
            axis2_strdup(env, "XML_SCHEMA_OBJ"),
            AXIS2_HASH_KEY_STRING,
            XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(attr_impl->annotated, env));

    status = xml_schema_annotated_resolve_methods(
                &(attr_impl->attr.base), env, attr_impl->annotated,
                xml_schema_attribute_super_objs,
                xml_schema_attribute_get_type,
                xml_schema_attribute_free);
    return &(attr_impl->attr);
}

axis2_status_t AXIS2_CALL
xml_schema_attribute_free(void *attr,
        const axutil_env_t *env)
{
    xml_schema_attribute_impl_t *attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);

    if (attr_impl->attribute_type)
    {
        /* TODO */
    }
    if (attr_impl->ht_super)
    {
        axis2_hash_free(attr_impl->ht_super, env);
        attr_impl->ht_super = NULL;
    }
    if (attr_impl->default_value)
    {
        AXIS2_FREE(env->allocator, attr_impl->default_value);
        attr_impl->default_value = NULL;
    }
    if (attr_impl->default_value)
    {
        AXIS2_FREE(env->allocator, attr_impl->fixed_value);
        attr_impl->fixed_value = NULL;
    }
    if (attr_impl->name)
    {
        AXIS2_FREE(env->allocator, attr_impl->name);
        attr_impl->name = NULL;
    }

    if (attr_impl->form)
    {
        XML_SCHEMA_FORM_FREE(attr_impl->form, env);
        attr_impl->form = NULL;
    }
    if (attr_impl->use)
    {
        XML_SCHEMA_USE_FREE(attr_impl->use, env);
        attr_impl->use = NULL;
    }

    if (attr_impl->annotated)
    {
        XML_SCHEMA_ANNOTATED_FREE(attr_impl->annotated, env);
        attr_impl->annotated = NULL;
    }

    if (attr_impl->attr.ops)
    {
        AXIS2_FREE(env->allocator, attr_impl->attr.ops);
        attr_impl->attr.ops = NULL;
    }
    if (attr_impl->attr.base.ops)
    {
        AXIS2_FREE(env->allocator, attr_impl->attr.base.ops);
        attr_impl->attr.base.ops = NULL;
    }

    AXIS2_FREE(env->allocator, attr_impl);
    attr_impl = NULL;
    return AXIS2_SUCCESS;
}

xml_schema_annotated_t *AXIS2_CALL
xml_schema_attribute_get_base_impl(void *attr,
        const axutil_env_t *env)
{
    xml_schema_attribute_impl_t *attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    return attr_impl->annotated;
}

xml_schema_types_t AXIS2_CALL
xml_schema_attribute_get_type(void *attr,
        const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->obj_type;
}

axis2_hash_t* AXIS2_CALL
xml_schema_attribute_super_objs(void *attr,
        const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->ht_super;
}

void * AXIS2_CALL
xml_schema_attribute_get_attr_type(
    void *attr,
    const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->attribute_type;
}

axis2_char_t * AXIS2_CALL
xml_schema_attribute_get_default_value(
    void *attr,
    const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->default_value;
}

axis2_status_t AXIS2_CALL
xml_schema_attribute_set_default_value(
    void *attr,
    const axutil_env_t *env,
    axis2_char_t *default_value)
{
    xml_schema_attribute_impl_t *attr_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, default_value, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    if (attr_impl->default_value)
    {
        AXIS2_FREE(env->allocator, attr_impl->default_value);
        attr_impl->default_value = NULL;
    }
    attr_impl->default_value = axis2_strdup(env, default_value);
    return AXIS2_SUCCESS;
}

axis2_char_t*  AXIS2_CALL
xml_schema_attribute_get_fixed_value(void *attr,
        const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->fixed_value;
}

axis2_status_t  AXIS2_CALL
xml_schema_attribute_set_fixed_value(void *attr,
        const axutil_env_t *env,
        axis2_char_t *fixed_value)
{
    xml_schema_attribute_impl_t *attr_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, fixed_value, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    if (attr_impl->fixed_value)
    {
        AXIS2_FREE(env->allocator, attr_impl->fixed_value);
        attr_impl->fixed_value = NULL;
    }
    attr_impl->fixed_value = axis2_strdup(env, fixed_value);
    return AXIS2_SUCCESS;
}

xml_schema_form_t* AXIS2_CALL
xml_schema_attribute_get_schema_form(void *attr,
        const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->form;
}

axis2_status_t  AXIS2_CALL
xml_schema_attribute_set_schema_form(void *attr,
        const axutil_env_t *env,
        xml_schema_form_t *schema_form)
{
    xml_schema_attribute_impl_t *attr_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, schema_form, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    if (attr_impl->form)
    {
        /* TODO */
    }
    attr_impl->form = schema_form;
    return AXIS2_SUCCESS;

}

axis2_qname_t*  AXIS2_CALL
xml_schema_attribute_get_qname(void *attr,
        const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->qualified_name;
}

axis2_status_t  AXIS2_CALL
xml_schema_attribute_set_qname(void *attr,
        const axutil_env_t *env,
        axis2_qname_t *qualified_name)
{
    xml_schema_attribute_impl_t *attr_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, qualified_name, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    if (attr_impl->qualified_name)
    {
        /* TODO */
    }
    attr_impl->qualified_name = qualified_name;
    return AXIS2_SUCCESS;



}

axis2_char_t*  AXIS2_CALL
xml_schema_attribute_get_name(void *attr,
        const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->name;
}

axis2_status_t  AXIS2_CALL
xml_schema_attribute_set_name(void *attr,
        const axutil_env_t *env,
        axis2_char_t *name)
{
    xml_schema_attribute_impl_t *attr_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, name, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    if (attr_impl->name)
    {
        AXIS2_FREE(env->allocator, attr_impl->name);
        attr_impl->name = NULL;
    }
    attr_impl->name = axis2_strdup(env, name);
    return AXIS2_SUCCESS;
}

axis2_qname_t*  AXIS2_CALL
xml_schema_attribute_get_ref_name(void *attr,
        const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->ref_name;
}

axis2_status_t  AXIS2_CALL
xml_schema_attribute_set_ref_name(void *attr,
        const axutil_env_t *env,
        axis2_qname_t *ref_name)
{
    xml_schema_attribute_impl_t *attr_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, ref_name, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    if (attr_impl->ref_name)
    {
        /* TODO */
    }
    attr_impl->ref_name = ref_name;
    return AXIS2_SUCCESS;
}

xml_schema_simple_type_t*  AXIS2_CALL
xml_schema_attribute_get_schema_type(void *attr,
        const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->schema_type;
}

axis2_status_t  AXIS2_CALL
xml_schema_attribute_set_schema_type(void *attr,
        const axutil_env_t *env,
        xml_schema_simple_type_t* sch_type)
{
    xml_schema_attribute_impl_t *attr_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, sch_type, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    if (attr_impl->schema_type)
    {
        /* TODO */
    }
    attr_impl->schema_type = sch_type;
    return AXIS2_SUCCESS;

}

axis2_qname_t*  AXIS2_CALL
xml_schema_attribute_get_schema_type_name(void *attr,
        const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->schema_type_name;
}


axis2_status_t  AXIS2_CALL
xml_schema_attribute_set_schema_type_name(void *attr,
        const axutil_env_t *env,
        axis2_qname_t *sch_type_name)
{
    xml_schema_attribute_impl_t *attr_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, sch_type_name, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    if (attr_impl->schema_type_name)
    {
        /* TODO */
    }
    attr_impl->schema_type_name = sch_type_name;
    return AXIS2_SUCCESS;

}

xml_schema_use_t*  AXIS2_CALL
xml_schema_attribute_get_use(void *attr,
        const axutil_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->use;
}

axis2_status_t  AXIS2_CALL
xml_schema_attribute_set_use(void *attr,
        const axutil_env_t *env,
        xml_schema_use_t *use)
{
    xml_schema_attribute_impl_t *attr_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, use, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    if (attr_impl->use)
    {
        /* TODO */
    }
    attr_impl->use = use;
    return AXIS2_SUCCESS;

}

axis2_char_t*  AXIS2_CALL
xml_schema_attribute_to_string(void *attr,
        const axutil_env_t *env,
        axis2_char_t *prefix,
        int tab)
{
    /** TODO */
    return NULL;
}

