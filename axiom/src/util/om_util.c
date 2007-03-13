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

#include <axiom_node.h>
#include <axiom_element.h>
#include <axiom_document.h>
#include <axis2_array_list.h>
#include <axis2_uri.h>
#include <axiom_util.h>

AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_util_get_first_child_element_with_uri(axiom_node_t *ele_node,
    const axis2_env_t *env,
    axis2_char_t *uri,
    axiom_node_t **child)
{
    axiom_node_t *child_node = NULL;

    if (!ele_node || !uri)
    {
        AXIS2_ERROR_SET(env->error,
            AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }

    child_node = AXIOM_NODE_GET_FIRST_CHILD(ele_node, env);
    while (child_node)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(child_node, env) == AXIOM_ELEMENT)
        {
            axiom_element_t *child_ele = NULL;
            axiom_namespace_t *ns = NULL;

            child_ele = (axiom_element_t*)
                AXIOM_NODE_GET_DATA_ELEMENT(child_node, env);
            ns = AXIOM_ELEMENT_GET_NAMESPACE(child_ele, env, child_node);
            if (ns)
            {
                axis2_char_t *child_uri = NULL;
                child_uri = AXIOM_NAMESPACE_GET_URI(ns, env);
                if (child_uri && AXIS2_STRCMP(child_uri, uri) == 0)
                {
                    (*child) = child_node;
                    return child_ele;
                }
            }
        }
        child_node = AXIOM_NODE_GET_NEXT_SIBLING(child_node, env);
    }
    return NULL;
}

AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_util_get_next_sibling_element_with_uri(axiom_node_t *ele_node,
    const axis2_env_t *env,
    axis2_char_t *uri,
    axiom_node_t **next_node)
{
    axiom_node_t *next_sib_node = NULL;
    if (!ele_node || !uri)
    {
        AXIS2_ERROR_SET(env->error,
            AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }

    next_sib_node = AXIOM_NODE_GET_NEXT_SIBLING(ele_node, env);
    while (next_sib_node)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(next_sib_node, env) == AXIOM_ELEMENT)
        {
            axiom_element_t *sib_ele = NULL;
            axiom_namespace_t *ns = NULL;

            sib_ele = (axiom_element_t*)
                AXIOM_NODE_GET_DATA_ELEMENT(next_sib_node, env);
            ns = AXIOM_ELEMENT_GET_NAMESPACE(sib_ele, env, next_sib_node);
            if (ns)
            {
                axis2_char_t *sib_uri = NULL;
                sib_uri = AXIOM_NAMESPACE_GET_URI(ns, env);
                if (sib_uri && AXIS2_STRCMP(sib_uri, uri) == 0)
                {
                    (*next_node) = next_sib_node;
                    return sib_ele;
                }
            }
        }
        next_sib_node = AXIOM_NODE_GET_NEXT_SIBLING(next_sib_node, env);
    }
    return NULL;
}

AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_util_get_first_child_element(axiom_element_t *ele,
        const axis2_env_t *env,
        axiom_node_t *ele_node,
        axiom_node_t **child_node)
{
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);

    return AXIOM_ELEMENT_GET_FIRST_ELEMENT(ele, env, ele_node, child_node);
}

AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_util_get_last_child_element(axiom_element_t *ele,
        const axis2_env_t *env,
        axiom_node_t *ele_node,
        axiom_node_t **child_node)
{
    axiom_node_t *last_node = NULL;
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);

    last_node = AXIOM_NODE_GET_LAST_CHILD(ele_node, env);
    while (last_node)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(last_node, env) == AXIOM_ELEMENT)
        {
            *child_node = last_node;
            return (axiom_element_t*)
                AXIOM_NODE_GET_DATA_ELEMENT(last_node, env);

        }
        last_node = AXIOM_NODE_GET_PREVIOUS_SIBLING(ele_node, env);
    }
    return NULL;
}

AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_util_get_next_sibling_element(axiom_element_t *ele,
        const axis2_env_t *env,
        axiom_node_t *ele_node,
        axiom_node_t **next_node)
{
    axiom_node_t *next_sibling = NULL;
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);

    next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(ele_node, env);
    while (next_sibling)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(next_sibling, env) == AXIOM_ELEMENT)
        {
            *next_node = next_sibling;
            return (axiom_element_t *)
                AXIOM_NODE_GET_DATA_ELEMENT(next_sibling, env);
        }
        next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(next_sibling, env);
    }
    return NULL;
}

AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_util_get_first_child_element_with_localname(axiom_element_t *ele,
        const axis2_env_t *env,
        axiom_node_t *ele_node,
        axis2_char_t *localname,
        axiom_node_t **child_node)
{
    axiom_node_t *child = NULL;
    axiom_node_t *next_sibling = NULL;
    axis2_char_t *child_localname = NULL;
    axiom_element_t *om_ele  = NULL;
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);
    AXIS2_PARAM_CHECK(env->error, localname, NULL);

    child = AXIOM_NODE_GET_FIRST_CHILD(ele_node, env);
    if (child)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(child, env) == AXIOM_ELEMENT)
        {
            om_ele = (axiom_element_t *)
                AXIOM_NODE_GET_DATA_ELEMENT(child, env);
            if (om_ele)
            {
                child_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if (child_localname && AXIS2_STRCMP(child_localname, localname) == 0)
                {
                    *child_node = child;
                    return om_ele;
                }
            }
        }
        om_ele = NULL;
        child_localname = NULL;
    }

    next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(child, env);
    while (next_sibling)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(next_sibling, env) == AXIOM_ELEMENT)
        {
            om_ele = (axiom_element_t*)
                AXIOM_NODE_GET_DATA_ELEMENT(next_sibling, env);
            if (om_ele)
            {
                child_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if (child_localname && AXIS2_STRCMP(child_localname, localname) == 0)
                {
                    *child_node = next_sibling;
                    return om_ele;
                }

            }
            om_ele = NULL;
            child_localname = NULL;
        }
        next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(next_sibling, env);
    }
    return NULL;
}


AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_util_get_last_child_element_with_localname(axiom_element_t *ele,
        const axis2_env_t *env,
        axiom_node_t *ele_node,
        axis2_char_t *localname,
        axiom_node_t **child_node)
{
    axiom_node_t *child = NULL;
    axiom_element_t *om_ele = NULL;
    axis2_char_t *child_localname = NULL;

    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, localname, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);

    child = AXIOM_NODE_GET_LAST_CHILD(ele_node, env);
    while (child)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(child, env) == AXIOM_ELEMENT)
        {
            om_ele = (axiom_element_t*)
                AXIOM_NODE_GET_DATA_ELEMENT(ele_node, env);
            if (om_ele)
            {
                child_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if (child_localname &&
                    AXIS2_STRCMP(child_localname, localname) == 0)
                {
                    *child_node = child;
                    return om_ele;
                }

            }
            om_ele = NULL;
            child_localname = NULL;
        }
        child = AXIOM_NODE_GET_PREVIOUS_SIBLING(child, env);
    }
    return NULL;
}

AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_util_get_next_siblng_element_with_localname(axiom_element_t *ele,
        const axis2_env_t  *env,
        axiom_node_t *ele_node,
        axis2_char_t *localname,
        axiom_node_t **next_node)
{
    axiom_node_t *next_sibling = NULL;
    axiom_element_t *om_ele    = NULL;
    axis2_char_t *ele_localname   = NULL;

    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);

    next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(ele_node, env);
    while (next_sibling)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(next_sibling, env) == AXIOM_ELEMENT)
        {
            om_ele = (axiom_element_t *)
                AXIOM_NODE_GET_DATA_ELEMENT(next_sibling, env);
            if (om_ele)
            {
                ele_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if (ele_localname && AXIS2_STRCMP(localname, ele_localname) == 0)
                {
                    *next_node = next_sibling;
                    return om_ele;
                }
            }
            om_ele = NULL;
            ele_localname = NULL;
        }
        next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(next_sibling, env);
    }
    return NULL;
}

AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_util_get_first_child_element_with_uri_localname(axiom_element_t *ele,
        const axis2_env_t  *env,
        axiom_node_t *ele_node,
        axis2_char_t *localname,
        axis2_char_t *uri,
        axiom_node_t **child_node)
{
    axiom_node_t *child = NULL;
    axiom_node_t *next_sibling = NULL;
    axis2_char_t *child_localname = NULL;
    axiom_element_t *om_ele  = NULL;
    axiom_namespace_t *ns    = NULL;
    axis2_char_t *ns_uri        = NULL;

    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);
    AXIS2_PARAM_CHECK(env->error, localname, NULL);
    AXIS2_PARAM_CHECK(env->error, uri, NULL);

    child = AXIOM_NODE_GET_FIRST_CHILD(ele_node, env);
    if (!child)
    {
        return NULL;

    }
    if (AXIOM_NODE_GET_NODE_TYPE(ele_node, env) == AXIOM_ELEMENT)
    {
        om_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(child , env);
        if (om_ele)
        {
            child_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
            ns = AXIOM_ELEMENT_GET_NAMESPACE(om_ele, env, child);
            if (ns)
            {
                ns_uri = AXIOM_NAMESPACE_GET_URI(ns, env);
            }

            if ((child_localname) &&
                (AXIS2_STRCMP(child_localname, localname) == 0) && (ns_uri)
                && (AXIS2_STRCMP(ns_uri, uri) == 0))
            {
                *child_node = child;
                return om_ele;
            }
        }
    }
    om_ele = NULL;
    child_localname = NULL;
    ns = NULL;
    ns_uri = NULL;

    next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(child, env);
    while (next_sibling)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(next_sibling, env) == AXIOM_ELEMENT)
        {
            om_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(next_sibling, env);
            if (om_ele)
            {
                child_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
                ns = AXIOM_ELEMENT_GET_NAMESPACE(om_ele, env, next_sibling);
                if (ns)
                {
                    ns_uri = AXIOM_NAMESPACE_GET_URI(ns, env);
                }

                if ((child_localname) &&
                    (AXIS2_STRCMP(child_localname, localname) == 0) && (ns_uri) &&
                    (AXIS2_STRCMP(ns_uri, uri) == 0))
                {
                    *child_node = next_sibling;
                    return om_ele;
                }

            }
            om_ele = NULL;
            child_localname = NULL;
            ns = NULL;
            ns_uri = NULL;
        }
        next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(next_sibling, env);
    }
    return NULL;
}
AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_util_get_last_child_element_with_uri_localname(axiom_element_t *ele,
        const axis2_env_t  *env,
        axiom_node_t *ele_node,
        axis2_char_t *localname,
        axis2_char_t *uri,
        axiom_node_t **child_node)
{
    axiom_node_t *child          = NULL;
    axiom_element_t *om_ele      = NULL;
    axis2_char_t *child_localname   = NULL;
    axiom_namespace_t *ns        = NULL;
    axis2_char_t *ns_uri            = NULL;

    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, localname, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);
    AXIS2_PARAM_CHECK(env->error, uri, NULL);

    child = AXIOM_NODE_GET_LAST_CHILD(ele_node, env);
    while (child)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(child, env) == AXIOM_ELEMENT)
        {
            om_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(ele_node, env);
            if (om_ele)
            {
                ns = AXIOM_ELEMENT_GET_NAMESPACE(om_ele, env, ele_node);
                if (ns)
                {
                    ns_uri = AXIOM_NAMESPACE_GET_URI(ns, env);
                }
                child_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if (child_localname &&
                    (AXIS2_STRCMP(child_localname, localname) == 0) && (ns_uri)
                    && (AXIS2_STRCMP(ns_uri, uri) == 0))
                {
                    *child_node = child;
                    return om_ele;
                }
            }
            om_ele = NULL;
            child_localname = NULL;
            ns = NULL;
            ns_uri = NULL;
        }
        child = AXIOM_NODE_GET_PREVIOUS_SIBLING(child, env);
    }
    return NULL;
}

AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_util_get_next_sibling_element_with_uri_localname(axiom_element_t *ele,
        const axis2_env_t  *env,
        axiom_node_t *ele_node,
        axis2_char_t *localname,
        axis2_char_t *uri,
        axiom_node_t **next_node)
{
    axiom_node_t *next_sibling = NULL;
    axiom_element_t *om_ele    = NULL;
    axis2_char_t *ele_localname   = NULL;
    axiom_namespace_t *ns      = NULL;
    axis2_char_t *ns_uri          = NULL;
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);

    next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(ele_node, env);
    while (next_sibling)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(next_sibling, env) == AXIOM_ELEMENT)
        {
            om_ele = (axiom_element_t *)
                AXIOM_NODE_GET_DATA_ELEMENT(next_sibling, env);
            if (om_ele)
            {
                ns = AXIOM_ELEMENT_GET_NAMESPACE(om_ele, env, next_sibling);
                if (ns)
                {
                    ns_uri = AXIOM_NAMESPACE_GET_URI(ns, env);
                }
                ele_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if (ele_localname &&
                    (AXIS2_STRCMP(localname, ele_localname) == 0) && (ns)
                    && (AXIS2_STRCMP(ns_uri, uri) == 0))
                {
                    *next_node = next_sibling;
                    return om_ele;
                }
            }
            om_ele = NULL;
            ele_localname = NULL;
            ns_uri = NULL;
            ns = NULL;
        }
        next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(next_sibling, env);
    }
    return NULL;
}

AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_util_get_first_child_element_with_localnames(axiom_element_t *ele,
        const axis2_env_t  *env,
        axiom_node_t *ele_node,
        axis2_array_list_t *names,
        axiom_node_t **child_node)
{
    axiom_node_t *child = NULL;
    axiom_node_t *next_sibling = NULL;
    axis2_char_t *child_localname = NULL;
    axis2_char_t *given_localname = NULL;
    axiom_element_t *om_ele  = NULL;
    int size = 0;
    int i    = 0;
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);
    AXIS2_PARAM_CHECK(env->error, names, NULL);

    child = AXIOM_NODE_GET_FIRST_CHILD(ele_node, env);
    if (child)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(child, env) == AXIOM_ELEMENT)
        {
            om_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(child, env);
            if (om_ele)
            {
                size = axis2_array_list_size(names, env);
                child_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);

                for (i = 0; i < size; i++)
                {
                    given_localname = (axis2_char_t *)axis2_array_list_get(names, env, i);

                    if ((child_localname) && (NULL != given_localname) &&
                        AXIS2_STRCMP(child_localname, given_localname) == 0)
                    {
                        *child_node = child;
                        return om_ele;
                    }
                    given_localname = NULL;
                }
            }
        }
        om_ele = NULL;
        child_localname = NULL;
        given_localname = NULL;
        size = 0;
    }

    next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(child, env);
    while (next_sibling)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(next_sibling, env) == AXIOM_ELEMENT)
        {
            om_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(next_sibling, env);
            if (om_ele)
            {
                size = axis2_array_list_size(names, env);
                child_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
                for (i = 0; i < size; i++)
                {
                    given_localname = (axis2_char_t *)axis2_array_list_get(names, env, i);
                    if ((child_localname) && (NULL != given_localname) &&
                        (AXIS2_STRCMP(child_localname, given_localname) == 0))
                    {
                        *child_node = next_sibling;
                        return om_ele;
                    }
                    given_localname = NULL;
                }
            }
            om_ele = NULL;
            child_localname = NULL;
            given_localname = NULL;
        }
        next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(next_sibling, env);
    }
    return NULL;

}


AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_util_get_last_child_element_with_localnames(axiom_element_t *ele,
        const axis2_env_t  *env,
        axiom_node_t *ele_node,
        axis2_array_list_t *names,
        axiom_node_t **child_node)
{
    axiom_node_t *child = NULL;
    axiom_element_t *om_ele = NULL;
    axis2_char_t *child_localname = NULL;
    axis2_char_t *given_localname = NULL;
    int size = 0;
    int i = 0;

    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, names, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);

    child = AXIOM_NODE_GET_LAST_CHILD(ele_node, env);
    while (child)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(child, env) == AXIOM_ELEMENT)
        {
            om_ele = (axiom_element_t*)
                AXIOM_NODE_GET_DATA_ELEMENT(ele_node, env);
            if (om_ele)
            {
                size = axis2_array_list_size(names, env);
                for (i = 0; i < size; i++)
                {
                    given_localname = (axis2_char_t *)axis2_array_list_get(names, env, i);
                    child_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
                    if (child_localname && (NULL != given_localname) &&
                        (AXIS2_STRCMP(child_localname, given_localname) == 0))
                    {
                        *child_node = child;
                        return om_ele;
                    }
                }
            }
            om_ele = NULL;
            child_localname = NULL;
            given_localname = NULL;
        }
        child = AXIOM_NODE_GET_PREVIOUS_SIBLING(child, env);
    }
    return NULL;
}

AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_util_get_next_siblng_element_with_localnames(axiom_element_t *ele,
        const axis2_env_t  *env,
        axiom_node_t *ele_node,
        axis2_array_list_t *names,
        axiom_node_t **next_node)
{
    axiom_node_t *next_sibling = NULL;
    axiom_element_t *om_ele    = NULL;
    axis2_char_t *ele_localname   = NULL;
    axis2_char_t *given_localname = NULL;
    int size = 0;
    int i    = 0;
    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, names, NULL);
    AXIS2_PARAM_CHECK(env->error, next_node, NULL);


    next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(ele_node, env);
    while (next_sibling)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(next_sibling, env) == AXIOM_ELEMENT)
        {
            om_ele = (axiom_element_t *)
                AXIOM_NODE_GET_DATA_ELEMENT(next_sibling, env);
            if (om_ele)
            {
                size = axis2_array_list_size(names, env);
                for (i = 0; i < size; i++)
                {
                    given_localname = (axis2_char_t *)axis2_array_list_get(names, env, i);
                    ele_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
                    if ((ele_localname) && (NULL != given_localname) &&
                        (AXIS2_STRCMP(given_localname, ele_localname) == 0))
                    {
                        *next_node = next_sibling;
                        return om_ele;
                    }

                }
            }
            om_ele = NULL;
            ele_localname = NULL;
        }
        next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(next_sibling, env);
    }
    return NULL;
}

AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_util_get_first_child_element_with_localname_attr(axiom_element_t *ele,
        const axis2_env_t  *env,
        axiom_node_t *ele_node,
        axis2_char_t *localname,
        axis2_char_t *attr_name,
        axis2_char_t *attr_value,
        axiom_node_t **child_node)
{
    axiom_node_t *child          = NULL;
    axiom_node_t *next_sibling   = NULL;
    axis2_char_t *child_localname   = NULL;
    axiom_element_t *om_ele      = NULL;
    axiom_attribute_t *om_attr   = NULL;
    axis2_hash_t *attr_ht           = NULL;
    axis2_hash_index_t *hi          = NULL;
    axis2_char_t *om_attr_value     = NULL;
    axis2_char_t *om_attr_name      = NULL;

    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);
    AXIS2_PARAM_CHECK(env->error, localname, NULL);
    AXIS2_PARAM_CHECK(env->error, attr_name, NULL);
    AXIS2_PARAM_CHECK(env->error, attr_value, NULL);

    child = AXIOM_NODE_GET_FIRST_CHILD(ele_node, env);

    if (child)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(child, env) == AXIOM_ELEMENT)
        {
            om_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(child, env);
            if (om_ele)
            {

                child_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if (child_localname && AXIS2_STRCMP(child_localname, localname) == 0)
                {
                    attr_ht = AXIOM_ELEMENT_GET_ALL_ATTRIBUTES(om_ele, env);
                    if (attr_ht)
                    {
                        for (hi = axis2_hash_first(attr_ht, env);  hi;
                            hi = axis2_hash_next(env, hi))
                        {
                            void *val = NULL;
                            axis2_hash_this(hi, NULL, NULL, &val);
                            if (val)
                            {
                                om_attr = (axiom_attribute_t *)val;
                                om_attr_name =
                                    axiom_attribute_get_localname(om_attr, env);

                                om_attr_value =
                                    axiom_attribute_get_value(om_attr, env);
                                if (om_attr_name && NULL != om_attr_value &&
                                    AXIS2_STRCMP(om_attr_name, attr_name) == 0 &&
                                    AXIS2_STRCMP(om_attr_value, attr_value) == 0)
                                {
                                    AXIS2_FREE(env->allocator, hi);
                                    *child_node = child;
                                    return om_ele;
                                }
                                om_attr = NULL;
                                om_attr_name = NULL;
                                om_attr_value = NULL;
                            }
                        }
                    }
                }
            }
        }
        om_ele = NULL;
        child_localname = NULL;
        hi = NULL;
    }

    next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(child, env);
    while (next_sibling)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(next_sibling, env) == AXIOM_ELEMENT)
        {
            om_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(next_sibling, env);
            if (om_ele)
            {
                child_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if (child_localname && AXIS2_STRCMP(child_localname, localname) == 0)
                {
                    attr_ht = AXIOM_ELEMENT_GET_ALL_ATTRIBUTES(om_ele, env);
                    if (attr_ht)
                    {

                        for (hi = axis2_hash_first(attr_ht, env);  hi;
                            hi = axis2_hash_next(env, hi))
                        {
                            void *val = NULL;
                            axis2_hash_this(hi, NULL, NULL, &val);
                            if (val)
                            {
                                om_attr = (axiom_attribute_t *)val;
                                om_attr_name =
                                    axiom_attribute_get_localname(om_attr, env);

                                om_attr_value =
                                    axiom_attribute_get_value(om_attr, env);

                                if (om_attr_name && NULL != om_attr_value &&
                                    AXIS2_STRCMP(om_attr_name, attr_name) == 0 &&
                                    AXIS2_STRCMP(om_attr_value, attr_value) == 0)
                                {
                                    *child_node = child;
                                    return om_ele;
                                }
                                om_attr = NULL;
                                om_attr_name = NULL;
                                om_attr_value = NULL;
                            }
                        }
                    }
                }

            }
            om_ele = NULL;
            child_localname = NULL;
        }
        next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(next_sibling, env);
    }
    return NULL;
}


AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_util_get_last_child_element_with_localname_attr(axiom_element_t *ele,
        const axis2_env_t  *env,
        axiom_node_t *ele_node,
        axis2_char_t *localname,
        axis2_char_t *attr_name,
        axis2_char_t *attr_value,
        axiom_node_t **child_node)
{
    axiom_node_t *child = NULL;
    axiom_element_t *om_ele = NULL;
    axis2_char_t *child_localname = NULL;
    axiom_attribute_t *om_attr = NULL;
    axis2_char_t *om_attr_name = NULL;
    axis2_char_t *om_attr_value = NULL;
    axis2_hash_index_t *hi      = NULL;
    axis2_hash_t *attr_ht       = NULL;

    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    AXIS2_PARAM_CHECK(env->error, localname, NULL);
    AXIS2_PARAM_CHECK(env->error, child_node, NULL);

    child = AXIOM_NODE_GET_LAST_CHILD(ele_node, env);
    while (child)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(child, env) == AXIOM_ELEMENT)
        {
            om_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(ele_node, env);
            if (om_ele)
            {
                child_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if (child_localname &&
                    AXIS2_STRCMP(child_localname, localname) == 0)
                {
                    attr_ht = AXIOM_ELEMENT_GET_ALL_ATTRIBUTES(om_ele, env);
                    if (attr_ht)
                    {
                        for (hi = axis2_hash_first(attr_ht, env);  hi;
                            hi = axis2_hash_next(env, hi))
                        {
                            void *val = NULL;
                            axis2_hash_this(hi, NULL, NULL, &val);
                            if (val)
                            {
                                om_attr = (axiom_attribute_t *)val;
                                om_attr_name =
                                    axiom_attribute_get_localname(om_attr, env);

                                om_attr_value =
                                    axiom_attribute_get_value(om_attr, env);

                                if (om_attr_name && NULL != om_attr_value &&
                                    AXIS2_STRCMP(om_attr_name, attr_name) == 0 &&
                                    AXIS2_STRCMP(om_attr_value, attr_value) == 0)
                                {
                                    AXIS2_FREE(env->allocator, hi);
                                    *child_node = child;
                                    return om_ele;
                                }
                                om_attr = NULL;
                                om_attr_name = NULL;
                                om_attr_value = NULL;
                            }
                        }
                    }
                }

            }
            om_ele = NULL;
            child_localname = NULL;
        }
        child = AXIOM_NODE_GET_PREVIOUS_SIBLING(child, env);
    }
    return NULL;
}

AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_util_get_next_siblng_element_with_localname_attr(axiom_element_t *ele,
        const axis2_env_t  *env,
        axiom_node_t *ele_node,
        axis2_char_t *localname,
        axis2_char_t *attr_name,
        axis2_char_t *attr_value,
        axiom_node_t **child_node)
{
    axiom_node_t *next_sibling = NULL;
    axiom_element_t *om_ele    = NULL;
    axis2_char_t *ele_localname   = NULL;
    axiom_attribute_t *om_attr = NULL;
    axis2_char_t *om_attr_value   = NULL;
    axis2_char_t *om_attr_name    = NULL;
    axis2_hash_t *attr_ht         = NULL;
    axis2_hash_index_t *hi        = NULL;

    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);


    next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(ele_node, env);
    while (next_sibling)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(next_sibling, env) == AXIOM_ELEMENT)
        {
            om_ele = (axiom_element_t *)
                AXIOM_NODE_GET_DATA_ELEMENT(next_sibling, env);
            if (om_ele)
            {
                ele_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
                if (ele_localname && AXIS2_STRCMP(localname, ele_localname) == 0)
                {
                    attr_ht = AXIOM_ELEMENT_GET_ALL_ATTRIBUTES(om_ele, env);
                    if (attr_ht)
                    {
                        for (hi = axis2_hash_first(attr_ht, env);  hi;
                            hi = axis2_hash_next(env, hi))
                        {
                            void *val = NULL;
                            axis2_hash_this(hi, NULL, NULL, &val);
                            if (val)
                            {
                                om_attr = (axiom_attribute_t *)val;
                                om_attr_name =
                                    axiom_attribute_get_localname(om_attr, env);

                                om_attr_value =
                                    axiom_attribute_get_value(om_attr, env);

                                if (om_attr_name && NULL != om_attr_value &&
                                        AXIS2_STRCMP(om_attr_name, attr_name) == 0 &&
                                        AXIS2_STRCMP(om_attr_value, attr_value) == 0)
                                {
                                    AXIS2_FREE(env->allocator, hi);
                                    *child_node = next_sibling;
                                    return om_ele;
                                }
                                om_attr = NULL;
                                om_attr_name = NULL;
                                om_attr_value = NULL;
                            }
                        }
                    }
                }
            }
            om_ele = NULL;
            ele_localname = NULL;
        }
        next_sibling = AXIOM_NODE_GET_NEXT_SIBLING(next_sibling, env);
    }
    return NULL;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axiom_util_get_child_node_text(axiom_node_t *om_node,
        const axis2_env_t *env)
{
    axiom_element_t *om_ele = NULL;
    if (!om_node)
    {
        return NULL;
    }
    if (AXIOM_NODE_GET_NODE_TYPE(om_node, env) != AXIOM_ELEMENT)
    {
        return NULL;
    }

    om_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(om_node, env);

    if (om_ele)
    {
        return AXIOM_ELEMENT_GET_TEXT(om_ele, env, om_node);
    }
    return NULL;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axiom_util_get_localname(axiom_node_t *node,
        const axis2_env_t *env)
{
    axiom_element_t *om_ele = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!node)
    {
        return NULL;
    }
    if (AXIOM_NODE_GET_NODE_TYPE(node, env) != AXIOM_ELEMENT)
    {
        return NULL;
    }
    om_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(node, env);
    if (om_ele)
    {
        return AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
    }
    return NULL;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axiom_util_get_node_namespace_uri(axiom_node_t *om_node,
        const axis2_env_t *env)
{
    axiom_element_t *om_ele = NULL;
    axiom_namespace_t *om_ns   = NULL;
    if (!om_node)
    {
        return NULL;
    }
    if (AXIOM_NODE_GET_NODE_TYPE(om_node, env) == AXIOM_ELEMENT)
    {
        om_ele = AXIOM_NODE_GET_DATA_ELEMENT(om_node, env);
        if (NULL == om_ele)
        {
            return NULL;
        }
        om_ns = AXIOM_ELEMENT_GET_NAMESPACE(om_ele, env, om_node);
        if (om_ns)
        {
            return AXIOM_NAMESPACE_GET_URI(om_ns, env);
        }
    }
    return NULL;
}

AXIS2_EXTERN axiom_child_element_iterator_t *AXIS2_CALL
axiom_util_get_child_elements(axiom_element_t *om_ele,
    const axis2_env_t *env,
    axiom_node_t *om_node)
{
    axiom_element_t *first_ele = NULL;
    axiom_node_t *first_node   = NULL;
    AXIS2_PARAM_CHECK(env->error, om_node, NULL);
    AXIS2_PARAM_CHECK(env->error, om_ele, NULL);
    first_ele =
        AXIOM_ELEMENT_GET_FIRST_ELEMENT(om_ele, env, om_node, &first_node);
    if (first_ele)
    {
        return axiom_child_element_iterator_create(env, first_node);
    }
    return NULL;
}

AXIS2_EXTERN axiom_document_t *AXIS2_CALL
axiom_util_new_document(const axis2_env_t *env,
    const axis2_uri_t *uri)
{
    axis2_char_t *path = NULL;
    axiom_xml_reader_t *reader = NULL;
    axiom_stax_builder_t *om_builder = NULL;
    axiom_document_t *doc   = NULL;

    AXIS2_PARAM_CHECK(env->error, uri, NULL);
    /* This is temporary code. Later complete the code to read from uri and build
     * the document
     */
    if (uri)
    {
        path = AXIS2_URI_GET_PATH((axis2_uri_t *) uri, env);
    }
    else
    {
        return NULL;
    }
    if (path)
    {
        reader = axiom_xml_reader_create_for_file(env, path, NULL);
    }
    else
    {
        return NULL;
    }
    if(reader)
    {
        om_builder = axiom_stax_builder_create(env, reader);
    }
    else
    {
        return NULL;
    }
    if (om_builder)
    {
        doc = axiom_stax_builder_get_document(om_builder, env);
    }
    else
    {
        return NULL;
    }
    if (doc)
    {
        AXIOM_DOCUMENT_BUILD_ALL(doc, env);
    }
    else
    {
        return NULL;
    }

    return doc;
}



