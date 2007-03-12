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

#include <axiom_attribute.h>
#include <string.h>
#include <axis2_utils_defines.h>

struct axiom_attribute
{
    /** localname of this attribute  */
    axis2_char_t *localname;
    /** value of this attribute */
    axis2_char_t *value;
    /** attribute namespace */
    axiom_namespace_t *ns;
    /** store qname here */
    axis2_qname_t *qname;
    int ref;
};

AXIS2_EXTERN axiom_attribute_t* AXIS2_CALL
axiom_attribute_create(const axis2_env_t *env,
    const axis2_char_t * localname,
    const axis2_char_t * value,
    axiom_namespace_t * ns)
{
    axiom_attribute_t *attribute = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    /* localname is mandatory */
    AXIS2_PARAM_CHECK(env->error, localname, NULL);

    attribute = (axiom_attribute_t *) AXIS2_MALLOC(env->allocator,
        sizeof(axiom_attribute_t));
    if (!attribute)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    /**  initialize fields */
    attribute->localname = NULL;
    attribute->value     = NULL;
    attribute->ns        = NULL;
    attribute->qname = NULL;

    attribute->localname = (axis2_char_t*) AXIS2_STRDUP(localname, env);
    if (!(attribute->localname))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE(env->allocator, attribute);
        return NULL;
    }
    if (value)
    {
        attribute->value = (axis2_char_t*) AXIS2_STRDUP(value, env);
        if (!(attribute->value))
        {
            AXIS2_ERROR_SET(env->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            AXIS2_FREE(env->allocator, attribute->localname);
            AXIS2_FREE(env->allocator, attribute);
            return NULL;
        }
    }
    attribute->ns = ns;

    attribute->ref = 0;

    return attribute;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_attribute_free(axiom_attribute_t *attribute,
    const axis2_env_t *env)
{
    if (--attribute->ref > 0)
    {
        return AXIS2_SUCCESS;
    }

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (attribute->localname)
    {
        AXIS2_FREE(env->allocator, attribute->localname);
    }
    if (attribute->value)
    {
        AXIS2_FREE(env->allocator, attribute->value);
    }
    if (attribute->qname)
    {
        AXIS2_QNAME_FREE(attribute->qname, env);
    }
    
    AXIS2_FREE(env->allocator, attribute);

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_attribute_free_void_arg(void *attribute,
    const axis2_env_t *env)
{
    axiom_attribute_t *om_attribute_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_attribute_l = (axiom_attribute_t *) attribute;
    return axiom_attribute_free(om_attribute_l, env);
}

AXIS2_EXTERN axis2_qname_t *AXIS2_CALL
axiom_attribute_get_qname(axiom_attribute_t *attribute,
    const axis2_env_t *env)
{
    axis2_qname_t *qname = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    if (!(attribute->qname))
    {
        if (attribute->ns)
        {
            qname = axis2_qname_create(env,
                attribute->localname,
                AXIOM_NAMESPACE_GET_URI(attribute->ns, env),
                AXIOM_NAMESPACE_GET_PREFIX(attribute->ns, env));
        }
        else
        {
            qname = axis2_qname_create(env, attribute->localname,
                NULL,  NULL);
        }
        attribute->qname = qname;
        return qname;
    }
    return attribute->qname;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_attribute_serialize(axiom_attribute_t *attribute,
    const axis2_env_t *env,
    axiom_output_t *om_output)
{
    int status = AXIS2_SUCCESS;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_output, AXIS2_FAILURE);

    if (attribute->ns)
    {
        axis2_char_t *uri = NULL;
        axis2_char_t *prefix = NULL;

        uri = AXIOM_NAMESPACE_GET_URI(attribute->ns, env);
        prefix = AXIOM_NAMESPACE_GET_PREFIX(attribute->ns, env);

        if ((uri) && (NULL != prefix) && (AXIS2_STRCMP(prefix, "") != 0))
        {
            status = axiom_output_write(om_output, env, AXIOM_ATTRIBUTE, 4,
                attribute->localname,
                attribute->value,
                uri , prefix);
        }
        else if (uri)
        {
            status = axiom_output_write(om_output, env, AXIOM_ATTRIBUTE, 3,
                attribute->localname, attribute->value, uri);
        }
    }
    else
    {
        status = axiom_output_write(om_output, env, AXIOM_ATTRIBUTE, 2,
            attribute->localname, attribute->value);
    }
    return status;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axiom_attribute_get_localname(axiom_attribute_t *attribute,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return attribute->localname;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axiom_attribute_get_value(axiom_attribute_t *attribute,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return attribute->value;
}

AXIS2_EXTERN axiom_namespace_t* AXIS2_CALL
axiom_attribute_get_namespace(axiom_attribute_t *attribute,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return attribute->ns;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_attribute_set_localname(axiom_attribute_t *attribute,
    const axis2_env_t *env,
    const axis2_char_t *localname)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, localname, AXIS2_FAILURE);

    if ((attribute->localname))
    {
        AXIS2_FREE(env->allocator, attribute->localname);
    }

    attribute->localname = (axis2_char_t*)AXIS2_STRDUP(localname, env);

    if (!(attribute->localname))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_attribute_set_value(axiom_attribute_t *attribute,
    const axis2_env_t *env,
    const axis2_char_t *value)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, value, AXIS2_FAILURE);

    if (attribute->value)
    {
        AXIS2_FREE(env->allocator, attribute->value);
    }

    attribute->value = (axis2_char_t*)AXIS2_STRDUP(value, env);
    if (!(attribute->value))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_attribute_set_namespace(axiom_attribute_t *attribute,
    const axis2_env_t *env,
    axiom_namespace_t *om_namespace)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_FUNC_PARAM_CHECK(om_namespace, env, AXIS2_FAILURE);
    attribute->ns = om_namespace;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axiom_attribute_t* AXIS2_CALL
axiom_attribute_clone(axiom_attribute_t *attribute,
    const axis2_env_t *env)
{
    axiom_attribute_t *cloned_attr    = NULL;
    if (!attribute) return NULL;
    AXIS2_ENV_CHECK(env, NULL);

    /** namespace is not cloned since it is a shollow copy*/
    cloned_attr = axiom_attribute_create(env,
        attribute->localname,
        attribute->value,
        attribute->ns);
    if (cloned_attr)
    {
        return cloned_attr;
    }
    return NULL;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
axiom_attribute_increment_ref(struct axiom_attribute *om_attribute,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_attribute->ref++;
    return AXIS2_SUCCESS;
}


