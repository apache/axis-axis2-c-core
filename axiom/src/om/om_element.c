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

#include <axiom_element.h>
#include "axiom_node_internal.h"
#include <axiom_attribute.h>
#include <axiom_namespace.h>
#include <axiom_xml_writer.h>
#include <axiom_stax_builder.h>
#include <string.h>
#include <stdio.h>

struct axiom_element
{
    /** Element's namespace */
    axiom_namespace_t *ns;
    /** Element's local name */
    axis2_char_t *localname;
    /** List of attributes */
    axis2_hash_t *attributes;
    /** List of namespaces */
    axis2_hash_t *namespaces;

    axis2_qname_t *qname;

    axiom_child_element_iterator_t *child_ele_iter;

    axiom_children_iterator_t* children_iter;

    axiom_children_qname_iterator_t *children_qname_iter;

    axis2_char_t *text_value;

    int next_ns_prefix_number;

};

AXIS2_EXTERN axiom_element_t * AXIS2_CALL
axiom_element_create(const axis2_env_t *env,
        axiom_node_t *parent,
        const axis2_char_t *localname,
        axiom_namespace_t *ns,
        axiom_node_t **node)
{
    axiom_element_t *element;
    AXIS2_ENV_CHECK(env, NULL);

    if (!localname || !node)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }

    (*node) = axiom_node_create(env);
    if (!(*node))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    element = (axiom_element_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axiom_element_t));

    if (!element)
    {
        AXIS2_FREE(env->allocator, (*node));
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    element->ns = NULL;
    element->localname = NULL;
    element->attributes = NULL;
    element->namespaces = NULL;
    element->qname = NULL;
    element->child_ele_iter = NULL;
    element->children_iter = NULL;
    element->children_qname_iter = NULL;
    element->text_value = NULL;
    element->next_ns_prefix_number = 0;

    element->localname = (axis2_char_t *) AXIS2_STRDUP(localname, env);
    if (!element->localname)
    {
        AXIS2_FREE(env->allocator, element);
        AXIS2_FREE(env->allocator, (*node));
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    if (parent)
    {
        AXIOM_NODE_ADD_CHILD(parent, env, (*node));

    }
    axiom_node_set_complete((*node), env, AXIS2_FALSE);
    axiom_node_set_node_type((*node), env, AXIOM_ELEMENT);
    axiom_node_set_data_element((*node), env, element);

    if (ns)
    {
        axis2_char_t *uri = NULL;
        axis2_char_t *prefix = NULL;

        uri = AXIOM_NAMESPACE_GET_URI(ns, env);
        prefix = AXIOM_NAMESPACE_GET_PREFIX(ns, env);

        element->ns = axiom_element_find_namespace(element,
                env, *node, uri, prefix);
        if (!(element->ns))
        {
            if (axiom_element_declare_namespace(element,
                    env, *node, ns) == AXIS2_SUCCESS)
                element->ns = ns;
        }
        if (prefix && AXIS2_STRCMP(prefix, "") == 0)
        {
            element->ns = NULL;
        }
    }

    return element;
}

AXIS2_EXTERN axiom_element_t * AXIS2_CALL
axiom_element_create_with_qname(const axis2_env_t *env,
        axiom_node_t *parent,
        const axis2_qname_t *qname,
        axiom_node_t ** node)
{
    axiom_element_t *element = NULL;
    axis2_char_t *localpart = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!qname || !node)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }
    localpart = AXIS2_QNAME_GET_LOCALPART(qname, env);
    if (!localpart)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }
    element = axiom_element_create(env, parent, localpart, NULL, node);
    if (!element)
        return NULL;

    if (*node)
    {
        axiom_element_t *om_element = NULL;
        axis2_char_t *temp_nsuri = NULL;
        axis2_char_t *temp_prefix = NULL;
        axiom_namespace_t *ns  = NULL;

        om_element = ((axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT((*node), env));
        temp_nsuri = AXIS2_QNAME_GET_URI(qname, env);
        temp_prefix = AXIS2_QNAME_GET_PREFIX(qname, env);
        if (!om_element)
            return NULL;

        if ((NULL == temp_nsuri) || (AXIS2_STRCMP(temp_nsuri, "") == 0))
        {
            /** no namespace uri is available in given qname
                no need to bother about it 
             */
            return om_element;
        }

        om_element->ns = axiom_element_find_namespace(om_element, env,
                (*node), temp_nsuri, temp_prefix);

        if (!(element->ns))
        {
            /** could not find a namespace so declare namespace */
            ns = axiom_namespace_create(env, temp_nsuri, temp_prefix);
            if (axiom_element_declare_namespace(om_element , env, *node, ns) == AXIS2_SUCCESS)
            {
                (element->ns) = ns;
                return om_element;
            }
            else
            {
                AXIOM_NAMESPACE_FREE(ns, env);
                AXIOM_ELEMENT_FREE(om_element, env);
                AXIS2_FREE(env->allocator, *node);
                return NULL;
            }
        }
    }
    return element;
}

AXIS2_EXTERN axiom_namespace_t * AXIS2_CALL
axiom_element_find_namespace(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_node_t * element_node,
        const axis2_char_t * uri,
        const axis2_char_t * prefix)
{
    axiom_node_t *parent = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    if (!element_node || !om_element)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }
    if (!AXIOM_NODE_GET_DATA_ELEMENT(element_node, env) ||
            AXIOM_NODE_GET_NODE_TYPE(element_node, env) != AXIOM_ELEMENT)
    {
        /* wrong element type or null node */
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }

    if (om_element->namespaces)
    {
        void *ns = NULL;

        if (!prefix || AXIS2_STRCMP(prefix, "") == 0)
        {
            /** check for a default namepsace */
            axiom_namespace_t *default_ns = NULL;
            axis2_hash_index_t *hashindex;

            default_ns = axiom_element_get_default_namespace(om_element ,
                    env, element_node);
            if (default_ns && NULL != uri)
            {
                axis2_char_t *default_uri = NULL;
                default_uri = AXIOM_NAMESPACE_GET_URI(default_ns, env);

                if (default_uri && AXIS2_STRCMP(uri, default_uri) == 0)
                {
                    return default_ns;
                }
            }
            /** prefix is null , so iterate the namespaces hash to find the namespace */
            for (hashindex = axis2_hash_first(om_element->namespaces, env);
                    hashindex; hashindex = axis2_hash_next(env, hashindex))
            {
                axis2_hash_this(hashindex, NULL, NULL, &ns);
                if (ns)
                {
                    axiom_namespace_t *temp_ns = NULL;
                    axis2_char_t *temp_nsuri = NULL;
                    temp_ns = (axiom_namespace_t *)ns;
                    temp_nsuri = AXIOM_NAMESPACE_GET_URI(temp_ns, env);

                    if (temp_nsuri && (AXIS2_STRCMP(temp_nsuri, uri) == 0))
                    {
                        /** namespace uri matches , so free hashindex and return ns*/
                        AXIS2_FREE(env->allocator, hashindex);
                        return (axiom_namespace_t *)(ns);
                    }
                    temp_ns = NULL;
                    temp_nsuri = NULL;
                    ns = NULL;
                }
            }
            ns = NULL;
        }
        else if (prefix)
        {
            /** prefix is not null get namespace directly if exist */
            ns = axis2_hash_get(om_element->namespaces, prefix,
                    AXIS2_HASH_KEY_STRING);
            if (ns)
            {
                axiom_namespace_t *found_ns  = NULL;
                axis2_char_t *found_uri         = NULL;
                found_ns = (axiom_namespace_t*)ns;

                found_uri = AXIOM_NAMESPACE_GET_URI(found_ns, env);

                /* TODO fix this
                if( ( found_uri) && AXIS2_STRCMP(found_uri, uri) == 0)
                {
                    printf("\n  furi   %s    real uri %s  %s\n ", found_uri, uri, prefix);
                    return found_ns;
                }
                */
                return found_ns;
            }
        }
    }
    /** could not find the namespace in current element scope
        look in the parent */

    parent = AXIOM_NODE_GET_PARENT(element_node, env);

    if (parent)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(parent, env) == AXIOM_ELEMENT)
        {
            axiom_element_t *om_element = NULL;
            om_element = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(parent, env);
            if (om_element)
            {   /** parent exist, parent is om element so find in parent*/
                return axiom_element_find_namespace(om_element, env,
                        parent, uri, prefix);
            }
        }
    }
    return NULL;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_declare_namespace_shallow(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_namespace_t *ns)
{
    axis2_char_t *prefix = NULL;
    axis2_char_t *uri    = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (!ns || !om_element)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);

        return AXIS2_FAILURE;
    }

    uri = AXIOM_NAMESPACE_GET_URI(ns, env);
    prefix = AXIOM_NAMESPACE_GET_PREFIX(ns, env);

    if (!(om_element->namespaces))
    {
        om_element->namespaces = axis2_hash_make(env);
        if (!(om_element->namespaces))
            return AXIS2_FAILURE;
    }
    if (prefix)
    {
        axis2_hash_set(om_element->namespaces,
                prefix, AXIS2_HASH_KEY_STRING, ns);
    }
    else
    {
        axis2_char_t *key = NULL;
        key = AXIS2_MALLOC(env->allocator, sizeof(char) * 10);
        memset(key, 0, sizeof(char)*10);
        om_element->next_ns_prefix_number++;
        sprintf(key, "axis2ns%d", om_element->next_ns_prefix_number);
        axis2_hash_set(om_element->namespaces, key,
                AXIS2_HASH_KEY_STRING,  ns);
    }
    axiom_namespace_increment_ref(ns, env);

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_declare_namespace(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_node_t *node,
        axiom_namespace_t *ns)
{
    axiom_namespace_t *declared_ns = NULL;
    axis2_char_t *prefix = NULL;
    axis2_char_t *uri    = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (!node || !ns || !om_element)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);

        return AXIS2_FAILURE;
    }

    uri = AXIOM_NAMESPACE_GET_URI(ns, env);
    prefix = AXIOM_NAMESPACE_GET_PREFIX(ns, env);

    declared_ns = axiom_element_find_namespace(om_element,
            env, node, uri, prefix);

    if (declared_ns)
    {
        if (AXIOM_NAMESPACE_EQUALS(ns, env, declared_ns) == AXIS2_TRUE)
        {/*Namespace already declared, so return */
            return AXIS2_SUCCESS;
        }
    }

    if (!(om_element->namespaces))
    {
        om_element->namespaces = axis2_hash_make(env);
        if (!(om_element->namespaces))
            return AXIS2_FAILURE;
    }
    if (prefix)
    {
        axis2_hash_set(om_element->namespaces,
                prefix, AXIS2_HASH_KEY_STRING, ns);
    }
    else
    {
        axis2_char_t *key = NULL;
        key = AXIS2_MALLOC(env->allocator, sizeof(char) * 10);
        memset(key, 0, sizeof(char)*10);
        om_element->next_ns_prefix_number++;
        sprintf(key, "axis2ns%d", om_element->next_ns_prefix_number);
        axis2_hash_set(om_element->namespaces, key,
                AXIS2_HASH_KEY_STRING,  ns);
    }
    axiom_namespace_increment_ref(ns, env);

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axiom_namespace_t * AXIS2_CALL
axiom_element_find_declared_namespace(axiom_element_t *om_element,
        const axis2_env_t *env,
        const axis2_char_t * uri,
        const axis2_char_t * prefix)
{
    axis2_hash_index_t *hash_index = NULL;
    void *ns = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, uri, NULL);


    if (!(om_element->namespaces))
    {
        return NULL;
    }
    if (!prefix || AXIS2_STRCMP(prefix, "") == 0)
    {
        /** prefix null iterate the namespace hash for matching uri */
        for (hash_index = axis2_hash_first(om_element->namespaces, env);
                hash_index; hash_index = axis2_hash_next(env, hash_index))
        {
            axis2_hash_this(hash_index, NULL, NULL, &ns);
            if (ns)
            {
                axiom_namespace_t *temp_ns = NULL;
                axis2_char_t *temp_nsuri = NULL;
                temp_ns = (axiom_namespace_t *)(ns);
                temp_nsuri = AXIOM_NAMESPACE_GET_URI(temp_ns, env);

                if (temp_nsuri && AXIS2_STRCMP(temp_nsuri, uri) == 0)
                {
                    AXIS2_FREE(env->allocator, hash_index);
                    return temp_ns;
                }
                temp_ns = NULL;
                temp_nsuri = NULL;
            }
        }
        ns = NULL;
        return NULL;
    }
    else if (prefix)
    {
        axiom_namespace_t *found_ns = NULL;
        ns = axis2_hash_get(om_element->namespaces, prefix, AXIS2_HASH_KEY_STRING);
        if (ns)
        {
            axis2_char_t *found_uri = NULL;
            found_ns = (axiom_namespace_t *) ns;
            found_uri = AXIOM_NAMESPACE_GET_URI(found_ns, env);
            if (found_uri && AXIS2_STRCMP(found_uri, uri) == 0)
            {
                return found_ns;
            }
        }
    }
    return NULL;
}


AXIS2_EXTERN axiom_namespace_t * AXIS2_CALL
axiom_element_find_namespace_with_qname(axiom_element_t *element,
        const axis2_env_t *env,
        axiom_node_t *node,
        axis2_qname_t *qname)
{
    AXIS2_ENV_CHECK(env, NULL);
    if (!element || !node || !qname)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }

    if (AXIS2_QNAME_GET_URI(qname, env))
    {
        return axiom_element_find_namespace(element, env, node,
                AXIS2_QNAME_GET_URI(qname, env),
                AXIS2_QNAME_GET_PREFIX(qname, env));
    }
    else
    {
        return NULL;
    }
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_add_attribute(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_attribute_t *attribute,
        axiom_node_t *element_node)
{

    axis2_qname_t *qname = NULL;
    axiom_namespace_t *om_namespace = NULL;
    axiom_namespace_t *temp_ns = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, attribute, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, element_node, AXIS2_FAILURE);

    om_namespace = AXIOM_ATTRIBUTE_GET_NAMESPACE(attribute, env);

    if (om_namespace)
    {
        temp_ns = AXIOM_ELEMENT_FIND_NAMESPACE
                (om_element, env,
                        element_node,
                        AXIOM_NAMESPACE_GET_URI(om_namespace, env),
                        AXIOM_NAMESPACE_GET_PREFIX(om_namespace, env));
        if (!temp_ns)
        {
            axis2_status_t status = AXIS2_SUCCESS;
            status = AXIOM_ELEMENT_DECLARE_NAMESPACE(om_element, env,
                    element_node, om_namespace);
            if (status == AXIS2_FAILURE)
                return AXIS2_FAILURE;
        }
    }

    if (!(om_element->attributes))
    {
        om_element->attributes = axis2_hash_make(env);
        if (!(om_element->attributes))
            return AXIS2_FAILURE;
    }

    qname = AXIOM_ATTRIBUTE_GET_QNAME(attribute, env);
    if (qname)
    {
        axis2_char_t *name = AXIS2_QNAME_TO_STRING(qname, env);
        axis2_hash_set(om_element->attributes,
                name, AXIS2_HASH_KEY_STRING,
                attribute);
    }
    axiom_attribute_increment_ref(attribute, env);
    return ((qname) ? AXIS2_SUCCESS : AXIS2_FAILURE);
}

AXIS2_EXTERN axiom_attribute_t * AXIS2_CALL
axiom_element_get_attribute(axiom_element_t *om_element,
        const axis2_env_t *env,
        axis2_qname_t *qname)
{
    axis2_char_t *name = NULL;
    axiom_attribute_t *attr = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, qname, NULL);

    name = AXIS2_QNAME_TO_STRING(qname, env);

    if ((om_element->attributes) && name)
    {
        attr = (axiom_attribute_t*)(axis2_hash_get(om_element->attributes, name, AXIS2_HASH_KEY_STRING));
    }
    return attr;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_free(axiom_element_t *om_element,
        const axis2_env_t *env)
{
    axis2_status_t status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (!om_element)
        return AXIS2_FAILURE;

    if (om_element->localname)
    {
        AXIS2_FREE(env->allocator, om_element->localname);
        om_element->localname = NULL;
    }
    if (om_element->ns)
    {
        /* it is the responsibility of the element where the namespace is declared to free it */
    }
    if (om_element->attributes)
    {
        axis2_hash_index_t *hi;
        void *val = NULL;

        for (hi = axis2_hash_first(om_element->attributes, env); hi;
                hi = axis2_hash_next(env, hi))
        {
            axis2_hash_this(hi, NULL, NULL, &val);

            if (val)
            {
                AXIOM_ATTRIBUTE_FREE((axiom_attribute_t *)val, env);
            }
            val = NULL;
        }
        axis2_hash_free(om_element->attributes, env);
        om_element->attributes = NULL;
    }

    if (om_element->namespaces)
    {
        axis2_hash_index_t *hi;
        void *val = NULL;
        for (hi = axis2_hash_first(om_element->namespaces, env); hi;
                hi = axis2_hash_next(env, hi))
        {
            axis2_hash_this(hi, NULL, NULL, &val);
            if (val)
            {
                AXIOM_NAMESPACE_FREE((axiom_namespace_t *)val, env);
                val = NULL;
            }
        }
        axis2_hash_free(om_element->namespaces, env);
        om_element->namespaces = NULL;
    }
    if (om_element->qname)
    {
        AXIS2_QNAME_FREE(om_element->qname, env);
        om_element->qname = NULL;
    }
    if (om_element->children_iter)
    {
        AXIOM_CHILDREN_ITERATOR_FREE(om_element->children_iter, env);
        om_element->children_iter = NULL;
    }
    if (om_element->child_ele_iter)
    {
        AXIOM_CHILD_ELEMENT_ITERATOR_FREE(om_element->child_ele_iter, env);
        om_element->child_ele_iter = NULL;
    }
    if (om_element->children_qname_iter)
    {
        AXIOM_CHILDREN_QNAME_ITERATOR_FREE(om_element->children_qname_iter, env);
        om_element->children_qname_iter = NULL;
    }
    if (om_element->text_value)
    {
        AXIS2_FREE(env->allocator, om_element->text_value);
        om_element->text_value = NULL;
    }

    AXIS2_FREE(env->allocator, om_element);

    return status;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_serialize_start_part(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_output_t *om_output,
        axiom_node_t *ele_node)
{
    int status = AXIS2_SUCCESS;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_output, AXIS2_FAILURE);

    if (om_element->ns)
    {
        axis2_char_t *uri = NULL;
        axis2_char_t *prefix = NULL;

        uri = AXIOM_NAMESPACE_GET_URI(om_element->ns, env);
        prefix = AXIOM_NAMESPACE_GET_PREFIX(om_element->ns, env);

        if ((uri) && (NULL != prefix) && (AXIS2_STRCMP(prefix, "") != 0))
        {
            status = axiom_output_write(om_output, env,
                    AXIOM_ELEMENT, 3, om_element->localname,
                    uri, prefix);

        }
        else if (uri)
        {
            status = axiom_output_write(om_output, env,
                    AXIOM_ELEMENT, 2, om_element->localname, uri);
        }
    }
    else
    {
        status = axiom_output_write(om_output, env,
                AXIOM_ELEMENT, 1, om_element->localname);

    }
    if (om_element->attributes)
    {
        axis2_hash_index_t *hi;
        void *val;
        for (hi = axis2_hash_first(om_element->attributes, env); hi;
                hi = axis2_hash_next(env, hi))
        {
            axis2_hash_this(hi, NULL, NULL, &val);

            if (val)
                status =
                    AXIOM_ATTRIBUTE_SERIALIZE((axiom_attribute_t *)val,
                            env, om_output);
            else
            {
                status = AXIS2_FAILURE;
            }
        }
    }


    if (om_element->namespaces)
    {
        axis2_hash_index_t *hi;
        void *val;
        for (hi = axis2_hash_first(om_element->namespaces, env); hi;
                hi = axis2_hash_next(env, hi))
        {
            axis2_hash_this(hi, NULL, NULL, &val);

            if (val)
                status =
                    AXIOM_NAMESPACE_SERIALIZE((axiom_namespace_t *)val,
                            env, om_output);
            else
            {
                status = AXIS2_FAILURE;
            }
        }
    }
    if ((NULL == om_element->namespaces) && (NULL == om_element->ns))
    {
        axiom_namespace_t *default_ns = NULL;
        default_ns = AXIOM_ELEMENT_GET_DEFAULT_NAMESPACE(om_element,
                env, ele_node);
        if (default_ns)
        {
            AXIOM_NAMESPACE_SERIALIZE(default_ns, env, om_output);
        }
    }
    return status;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_serialize_end_part(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_output_t * om_output)
{
    int status = AXIS2_SUCCESS;
    AXIS2_PARAM_CHECK(env->error, om_output, AXIS2_FAILURE);
    status = axiom_output_write(om_output, env, AXIOM_ELEMENT, 0);
    return status;
}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axiom_element_get_localname(axiom_element_t *om_element,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return om_element->localname;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_set_localname(axiom_element_t *om_element,
        const axis2_env_t *env,
        const axis2_char_t *localname)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, localname, AXIS2_FAILURE);
    if (om_element->localname)
    {
        AXIS2_FREE(env->allocator, om_element->localname);
        om_element->localname = NULL;
    }
    om_element->localname = (axis2_char_t*)AXIS2_STRDUP(localname, env);
    if (!(om_element->localname))
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axiom_namespace_t *AXIS2_CALL
axiom_element_get_namespace(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_node_t *ele_node)

{
    axiom_namespace_t *ns = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    if (om_element->ns)
    {
        ns = om_element->ns;
    }
    else
    {   /* TODO need to change get_namespace and get_qname methods so
                                            that default namespace is handled properly */
        ns = axiom_element_get_default_namespace(
                    om_element, env, ele_node);

    }
    return ns;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_set_namespace(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_namespace_t *ns,
        axiom_node_t *node)
{
    axiom_namespace_t *om_ns = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ns , AXIS2_FAILURE);
    om_ns = axiom_element_find_namespace(om_element, env, node,
            AXIOM_NAMESPACE_GET_URI(ns, env),
            AXIOM_NAMESPACE_GET_PREFIX(ns , env));
    if (!om_ns)
    {
        status = axiom_element_declare_namespace(om_element, env, node, ns);
        if (status == AXIS2_FAILURE)
            return AXIS2_FAILURE;
        om_element->ns = ns;
    }
    else
    {
        om_element->ns = om_ns;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_set_namespace_shallow(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_namespace_t *ns)
{
    om_element->ns = ns;
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_hash_t * AXIS2_CALL
axiom_element_get_all_attributes(axiom_element_t *om_element,
        const axis2_env_t *env)
{

    AXIS2_ENV_CHECK(env, NULL);
    return om_element->attributes;
}

AXIS2_EXTERN axis2_hash_t* AXIS2_CALL
axiom_element_get_namespaces
(axiom_element_t *om_element,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return om_element->namespaces;
}

AXIS2_EXTERN axis2_qname_t* AXIS2_CALL
axiom_element_get_qname(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_node_t *ele_node)
{
    axiom_namespace_t *ns =  NULL;
    AXIS2_ENV_CHECK(env, NULL);

    if (om_element->qname)
    {
        return om_element->qname;
    }
    else
    {
        ns = AXIOM_ELEMENT_GET_NAMESPACE(om_element, env, ele_node);
        if (ns)
        {
            if (AXIOM_NAMESPACE_GET_PREFIX(ns, env))
            {
                om_element->qname = axis2_qname_create(env ,
                        om_element->localname,
                        AXIOM_NAMESPACE_GET_URI(ns, env),
                        AXIOM_NAMESPACE_GET_PREFIX(ns, env));
            }
            else
            {
                om_element->qname = axis2_qname_create(env,
                        om_element->localname,
                        AXIOM_NAMESPACE_GET_URI(ns, env),
                        NULL);
            }

        }
        else
        {
            om_element->qname = axis2_qname_create(env,
                    om_element->localname,
                    NULL,  NULL);
        }
    }
    return om_element->qname;
}

AXIS2_EXTERN axiom_children_iterator_t* AXIS2_CALL
axiom_element_get_children(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_node_t *element_node)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, element_node, NULL);
    if (om_element->children_iter)
    {
        return om_element->children_iter;
    }
    else
    {
        om_element->children_iter = axiom_children_iterator_create(env,
                AXIOM_NODE_GET_FIRST_CHILD(element_node, env));
        return om_element->children_iter;
    }
    return NULL;
}

AXIS2_EXTERN axiom_children_qname_iterator_t* AXIS2_CALL
axiom_element_get_children_with_qname(axiom_element_t *om_element,
        const axis2_env_t *env,
        axis2_qname_t *element_qname,
        axiom_node_t *element_node)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, element_node, NULL);
    if (om_element->children_qname_iter)
    {
        AXIOM_CHILDREN_QNAME_ITERATOR_FREE(om_element->children_qname_iter, env);
        om_element->children_qname_iter = NULL;
    }
    om_element->children_qname_iter =  axiom_children_qname_iterator_create(env,
            AXIOM_NODE_GET_FIRST_CHILD(element_node, env),
            element_qname);
    return om_element->children_qname_iter;
}

AXIS2_EXTERN axiom_element_t* AXIS2_CALL
axiom_element_get_first_child_with_qname(axiom_element_t *om_element,
        const axis2_env_t *env,
        axis2_qname_t *element_qname,
        axiom_node_t *element_node,
        axiom_node_t **child_node)
{
    axiom_node_t *om_node = NULL;
    axiom_children_qname_iterator_t *children_iterator = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, element_qname, NULL);
    AXIS2_PARAM_CHECK(env->error, element_node, NULL);

    children_iterator = axiom_children_qname_iterator_create(env,
            AXIOM_NODE_GET_FIRST_CHILD(element_node, env),
            element_qname);
    if (!children_iterator)
        return NULL;

    if (AXIOM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(children_iterator, env))
    {
        om_node =   AXIOM_CHILDREN_QNAME_ITERATOR_NEXT(children_iterator, env);

    }
    if (om_node && (AXIOM_NODE_GET_NODE_TYPE(om_node, env) == AXIOM_ELEMENT))
    {
        AXIOM_CHILDREN_QNAME_ITERATOR_FREE(children_iterator, env);

        if (child_node)
        {
            *child_node = om_node;
        }
        return (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(om_node, env);
    }
    else
    {
        AXIOM_CHILDREN_QNAME_ITERATOR_FREE(children_iterator, env);
        return NULL;
    }
    return NULL;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_remove_attribute(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_attribute_t *om_attribute)
{
    axis2_qname_t *qname = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_attribute, AXIS2_FAILURE);

    qname = AXIOM_ATTRIBUTE_GET_QNAME(om_attribute, env);
    if (qname && (om_element->attributes))
    {
        axis2_char_t *name = NULL;
        name = AXIS2_QNAME_TO_STRING(qname, env);
        if (name)
        {
            axis2_hash_set(om_element->attributes, name,
                    AXIS2_HASH_KEY_STRING, NULL);
            return AXIS2_SUCCESS;
        }
    }
    return AXIS2_FAILURE;
}

AXIS2_EXTERN axiom_element_t* AXIS2_CALL
axiom_element_get_first_element(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_node_t *element_node,
        axiom_node_t **first_ele_node)
{
    axiom_node_t *temp_node = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, element_node, NULL);

    temp_node = AXIOM_NODE_GET_FIRST_CHILD(element_node, env);
    while (temp_node)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(temp_node, env) == AXIOM_ELEMENT)
        {
            if (first_ele_node)
            {
                *first_ele_node = temp_node;
            }
            return (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(temp_node, env);
        }
        else
        {
            temp_node = AXIOM_NODE_GET_NEXT_SIBLING(temp_node, env);
        }
    }
    return NULL;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axiom_element_get_text(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_node_t *element_node)
{
    axis2_char_t *dest = NULL;
    axis2_char_t* temp_text = NULL;
    axiom_text_t* text_node = NULL;
    axiom_node_t* temp_node = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, element_node, NULL);

    if (om_element->text_value)
    {
        AXIS2_FREE(env->allocator, om_element->text_value);
        om_element->text_value = NULL;
    }

    temp_node = AXIOM_NODE_GET_FIRST_CHILD(element_node, env);

    while (temp_node)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(temp_node, env) == AXIOM_TEXT)
        {
            int dest_len = 0;
            int curr_len = 0;
            axis2_char_t *temp_dest = NULL;

            text_node = (axiom_text_t *)AXIOM_NODE_GET_DATA_ELEMENT(temp_node, env);
            if (text_node)
            {
                temp_text = AXIOM_TEXT_GET_VALUE(text_node, env);
                if (dest && temp_text && AXIS2_STRCMP(temp_text, "") != 0)
                {
                    dest_len = AXIS2_STRLEN(dest);
                    curr_len = dest_len + AXIS2_STRLEN(temp_text);
                    temp_dest = AXIS2_MALLOC(env->allocator,
                            (curr_len + 1) * sizeof(axis2_char_t));

                    memcpy(temp_dest, dest, dest_len*sizeof(axis2_char_t));
                    memcpy((temp_dest + dest_len * sizeof(axis2_char_t)),
                            temp_text, curr_len - dest_len);

                    temp_dest[curr_len] = '\0';

                    AXIS2_FREE(env->allocator, dest);
                    dest = NULL;
                    dest = temp_dest;
                }
                else if (!dest && temp_text && AXIS2_STRCMP(temp_text, "") != 0)
                {
                    dest = AXIS2_STRDUP(temp_text, env);
                }
            }
        }
        temp_node = AXIOM_NODE_GET_NEXT_SIBLING(temp_node, env);
    }

    om_element->text_value = dest;
    return om_element->text_value;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_set_text(axiom_element_t *om_element,
        const axis2_env_t *env,
        const axis2_char_t *text,
        axiom_node_t *element_node)
{
    axiom_node_t* temp_node = NULL;
    axiom_text_t* om_text = NULL;
    axiom_node_t* node_to_free = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, text, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, element_node, AXIS2_FAILURE);

    temp_node = AXIOM_NODE_GET_FIRST_CHILD(element_node, env);
    while (temp_node)
    {
        if (AXIOM_NODE_GET_NODE_TYPE(temp_node, env) == AXIOM_TEXT)
        {
            node_to_free = AXIOM_NODE_DETACH(temp_node, env);
            AXIOM_NODE_FREE_TREE(node_to_free, env);
        }
        temp_node = AXIOM_NODE_GET_NEXT_SIBLING(temp_node, env);
    }
    temp_node = NULL;

    om_text = axiom_text_create(env, NULL, text, &temp_node);
    AXIOM_NODE_ADD_CHILD(element_node, env, temp_node);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axiom_element_to_string(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_node_t *element_node)
{
    int status = AXIS2_SUCCESS;
    axiom_output_t *om_output = NULL;
    axiom_xml_writer_t *xml_writer = NULL;
    axis2_char_t *xml = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, element_node, NULL);

    xml_writer = axiom_xml_writer_create_for_memory(env, NULL, AXIS2_TRUE, 0,
            AXIS2_XML_PARSER_TYPE_BUFFER);
    if (!xml_writer)
        return NULL;

    om_output = axiom_output_create(env, xml_writer);
    if (!om_output)
        return NULL;

    status = AXIOM_NODE_SERIALIZE(element_node, env, om_output);
    if (status == AXIS2_SUCCESS)
    {
        xml = (axis2_char_t*)AXIOM_XML_WRITER_GET_XML(xml_writer, env);
    }
    AXIOM_OUTPUT_FREE(om_output, env);
    return xml;
}

AXIS2_EXTERN axiom_child_element_iterator_t * AXIS2_CALL
axiom_element_get_child_elements(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_node_t *element_node)
{
    axiom_node_t *first_node = NULL;
    axiom_element_t *ele = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, element_node, NULL);
    ele = AXIOM_ELEMENT_GET_FIRST_ELEMENT(om_element, env, element_node, &first_node);
    if (om_element->child_ele_iter)
    {
        return om_element->child_ele_iter;
    }
    else if (ele && first_node)
    {
        om_element->child_ele_iter = axiom_child_element_iterator_create(env, first_node);
        return om_element->child_ele_iter;
    }
    return NULL;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_build(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_node_t *om_ele_node)
{
    axiom_stax_builder_t *builder = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    AXIS2_PARAM_CHECK(env->error, om_ele_node, AXIS2_FAILURE);
    if (AXIOM_NODE_GET_NODE_TYPE(om_ele_node, env) != AXIOM_ELEMENT)
        return AXIS2_FAILURE;

    builder = axiom_node_get_builder(om_ele_node, env);
    if (!builder)
        return AXIS2_FAILURE;
    while (!AXIOM_NODE_IS_COMPLETE(om_ele_node, env) &&
            !AXIOM_STAX_BUILDER_IS_COMPLETE(builder, env))
    {
        void *value  = NULL;
        value   = AXIOM_STAX_BUILDER_NEXT(builder, env);
        if (!value)
            return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axiom_namespace_t* AXIS2_CALL
axiom_element_get_default_namespace(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_node_t *element_node)
{
    axiom_node_t *parent_node = NULL;
    axiom_namespace_t *default_ns = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, element_node, NULL);

    if (om_element->namespaces)
    {
        default_ns = axis2_hash_get(om_element->namespaces, "",
                AXIS2_HASH_KEY_STRING);
        if (default_ns)
        {
            return default_ns;
        }
    }

    parent_node = AXIOM_NODE_GET_PARENT(element_node, env);
    if ((parent_node) &&
            (AXIOM_NODE_GET_NODE_TYPE(parent_node, env) == AXIOM_ELEMENT))
    {
        axiom_element_t *parent_ele = NULL;
        parent_ele = (axiom_element_t *)
                AXIOM_NODE_GET_DATA_ELEMENT(parent_node, env);
        if (parent_ele)
        {
            return axiom_element_get_default_namespace(parent_ele, env, parent_node);
        }
    }
    return NULL;
}

/**
* declared a default namespace explicitly
*/
AXIS2_EXTERN axiom_namespace_t* AXIS2_CALL
axiom_element_declare_default_namespace(axiom_element_t *om_element,
        const axis2_env_t *env,
        axis2_char_t *uri)
{
    axiom_namespace_t *default_ns = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, uri, NULL);

    if (AXIS2_STRCMP(uri, "") == 0)
        return NULL;

    default_ns = axiom_namespace_create(env, uri, "");
    if (!default_ns)
    {
        return NULL;
    }
    if (om_element->namespaces)
    {
        om_element->namespaces = axis2_hash_make(env);
        if (!(om_element->namespaces))
        {
            return NULL;
        }
    }

    axis2_hash_set(om_element->namespaces, "",
            AXIS2_HASH_KEY_STRING, default_ns);
    axiom_namespace_increment_ref(default_ns, env);
    return default_ns;
}

/**
* checks for the namespace in the context of this element
* with the given prefix
*/

AXIS2_EXTERN axiom_namespace_t* AXIS2_CALL
axiom_element_find_namespace_uri(axiom_element_t *om_element,
        const axis2_env_t *env,
        axis2_char_t *prefix,
        axiom_node_t *element_node)
{
    axiom_node_t *parent_node = NULL;
    axiom_namespace_t *ns = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, element_node, NULL);
    AXIS2_PARAM_CHECK(env->error, prefix, NULL);

    if (om_element->namespaces)
    {
        ns = axis2_hash_get(om_element->namespaces, prefix,
                AXIS2_HASH_KEY_STRING);
        if (ns)
        {
            return ns;
        }
    }

    parent_node = AXIOM_NODE_GET_PARENT(element_node, env);
    if ((parent_node) &&
            (AXIOM_NODE_GET_NODE_TYPE(parent_node, env) == AXIOM_ELEMENT))
    {
        axiom_element_t *parent_ele = NULL;
        parent_ele = (axiom_element_t *)
                AXIOM_NODE_GET_DATA_ELEMENT(parent_node, env);
        if (parent_ele)
        {
            return axiom_element_find_namespace_uri(parent_ele, env, prefix, parent_node);
        }
    }
    return NULL;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axiom_element_get_attribute_value(axiom_element_t *om_element,
        const axis2_env_t *env,
        axis2_qname_t *qname)
{
    axis2_char_t *name = NULL;
    axiom_attribute_t *attr = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, qname, NULL);

    name = AXIS2_QNAME_TO_STRING(qname, env);

    if ((om_element->attributes) && (NULL != name))
    {
        attr = (axiom_attribute_t*) axis2_hash_get(om_element->attributes,
                name, AXIS2_HASH_KEY_STRING);
        if (attr)
        {
            return AXIOM_ATTRIBUTE_GET_VALUE(attr, env);
        }
    }
    return NULL;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_set_namespace_with_no_find_in_current_scope(
    axiom_element_t *om_element,
    const axis2_env_t *env,
    axiom_namespace_t  *om_ns)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_ns, AXIS2_FAILURE);
    om_element->ns = om_ns;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_hash_t* AXIS2_CALL
axiom_element_extract_attributes(axiom_element_t *om_element,
        const axis2_env_t *env,
        axiom_node_t *ele_node)
{
    axis2_hash_index_t *hi = NULL;
    axis2_hash_t *ht_cloned = NULL;

    axiom_attribute_t *om_attr = NULL;
    axiom_attribute_t *cloned_attr = NULL;

    axiom_namespace_t *om_ns = NULL;
    axiom_namespace_t *cloned_ns = NULL;

    axis2_char_t *key = NULL;
    axis2_qname_t *qn = NULL;

    AXIS2_PARAM_CHECK(env->error, ele_node, NULL);
    if (!om_element->attributes)
        return NULL;
    ht_cloned = axis2_hash_make(env);
    if (!ht_cloned)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    for (hi = axis2_hash_first(om_element->attributes, env);
            hi; hi = axis2_hash_next(env, hi))
    {
        void *val = NULL;
        axis2_hash_this(hi, NULL, NULL, &val);
        if (val)
        {
            om_attr = (axiom_attribute_t *)val;
            cloned_attr = AXIOM_ATTRIBUTE_CLONE(om_attr, env);

            om_ns = AXIOM_ATTRIBUTE_GET_NAMESPACE(om_attr, env);
            if (om_ns)
            {
                cloned_ns = AXIOM_NAMESPACE_CLONE(om_ns, env);
                AXIOM_ATTRIBUTE_SET_NAMESPACE(cloned_attr, env, cloned_ns);
            }
            qn = AXIOM_ATTRIBUTE_GET_QNAME(cloned_attr, env);
            key = AXIS2_QNAME_TO_STRING(qn, env);
            axis2_hash_set(ht_cloned, key, AXIS2_HASH_KEY_STRING, cloned_attr);
        }
        val = NULL;
        key = NULL;
        qn  = NULL;
        om_attr = NULL;
        cloned_attr = NULL;
        om_ns = NULL;
        cloned_ns = NULL;
    }
    return ht_cloned;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axiom_element_get_attribute_value_by_name(
    axiom_element_t *om_element,
    const axis2_env_t *env,
    axis2_char_t *attr_name)
{
    axis2_hash_index_t *hi = NULL;

    AXIS2_PARAM_CHECK(env->error, attr_name, NULL);
    if (!om_element->attributes)
        return NULL;
    for (hi = axis2_hash_first(om_element->attributes, env); hi;
            hi = axis2_hash_next(env, hi))
    {
        void *attr = NULL;
        axiom_attribute_t *om_attr = NULL;
        axis2_hash_this(hi, NULL, NULL, &attr);
        if (attr)
        {
            axis2_char_t *this_attr_name;
            axis2_char_t *this_attr_value;
            axis2_char_t *attr_qn_str = NULL;
            axiom_namespace_t *attr_ns = NULL;
            axis2_char_t *prefix          = NULL;

            om_attr = (axiom_attribute_t*)attr;
            this_attr_name = AXIOM_ATTRIBUTE_GET_LOCALNAME(om_attr, env);
            this_attr_value = AXIOM_ATTRIBUTE_GET_VALUE(om_attr, env);
            attr_ns = AXIOM_ATTRIBUTE_GET_NAMESPACE(om_attr, env);
            if (attr_ns)
            {
                prefix = AXIOM_NAMESPACE_GET_PREFIX(attr_ns, env);
                if (prefix)
                {
                    axis2_char_t *tmp_val = NULL;
                    tmp_val = AXIS2_STRACAT(prefix, ":", env);
                    attr_qn_str = AXIS2_STRACAT(tmp_val, this_attr_name, env);
                    if (tmp_val)
                    {
                        AXIS2_FREE(env->allocator, tmp_val);
                        tmp_val = NULL;
                    }
                }
            }
            else
            {
                attr_qn_str = AXIS2_STRDUP(this_attr_name, env);
            }

            if (attr_qn_str && AXIS2_STRCMP(attr_qn_str, attr_name) == 0)
            {
                AXIS2_FREE(env->allocator, attr_qn_str);
                attr_qn_str = NULL;
                AXIS2_FREE(env->allocator, hi);
                return this_attr_value;
            }

            AXIS2_FREE(env->allocator, attr_qn_str);
            attr_qn_str = NULL;
        }
    }
    return NULL;
}



