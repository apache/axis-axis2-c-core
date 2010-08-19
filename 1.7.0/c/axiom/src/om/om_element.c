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

#include "axiom_element_internal.h"
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
    axutil_string_t *localname;

    /** List of attributes */
    axutil_hash_t *attributes;

    /** List of other namespaces */
    axutil_hash_t *namespaces;

    /* denotes whether current element is an empty element. i.e. <element/>
     * Used only when writing the output */
    axis2_bool_t is_empty;

    /* Following members are kept as a result of some operations. Reason for keeping them is,
     * (1) we can free them without memory leak
     * (2) Improve the performance, so that we don't need to re-do the calculation again
     */
    axutil_qname_t *qname;                          /* result of axiom_element_get_qname */
    axiom_child_element_iterator_t *child_ele_iter; /* result of axiom_element_get_child_elements*/
    axiom_children_iterator_t *children_iter;       /* result of axiom_element_get_children */
    axiom_children_qname_iterator_t *children_qname_iter; /*axiom_element_get_children_with_qname */
    axis2_char_t *text_value;                       /* result of axiom_element_get_text */

};

/**
 * Creates an AXIOM element with given local name
 * @param env Environment. MUST NOT be NULL.
 * @param parent parent of the element node to be created. can be NULL.
 * @param localname local name of the element. cannot be NULL.
 * @param ns namespace of the element.  can be NULL.
 *                       If the value of the namespace has not already been declared
 *                       then the namespace structure ns will be cloned and declared and will be
 *                       freed when the tree is freed.
 *                       Caller has to delete the original ns object passed to the method.
 * @param node This is an out parameter. cannot be NULL.
 *                       Returns the node corresponding to the comment created.
 *                       Node type will be set to AXIOM_ELEMENT
 * @return a pointer to the newly created element struct
 */
AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_element_create(
    const axutil_env_t * env,
    axiom_node_t * parent,
    const axis2_char_t * localname,
    axiom_namespace_t * ns,
    axiom_node_t ** node)
{
    axiom_element_t *element;
    AXIS2_ASSERT(localname != NULL);
    AXIS2_ASSERT(node != NULL);
    AXIS2_ASSERT(env != NULL);

    (*node) = axiom_node_create(env);
    if(!(*node))
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to create axiom node needed by element");
        return NULL;
    }

    element = (axiom_element_t *)AXIS2_MALLOC(env->allocator, sizeof(axiom_element_t));
    if(!element)
    {
        axiom_node_free_tree(*node, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Insufficient memory to create axiom element");
        return NULL;
    }
    memset(element, 0, sizeof(axiom_element_t));

    element->localname = axutil_string_create(env, localname);
    if(!element->localname)
    {
        AXIS2_FREE(env->allocator, element);/* Still we haven't set the data element. so, we have */
        axiom_node_free_tree(*node, env);   /* to free node and element separately */
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to create string to store local name");
        return NULL;
    }

    if(parent)
    {
        axiom_node_add_child(parent, env, (*node));
    }
    axiom_node_set_node_type((*node), env, AXIOM_ELEMENT);
    axiom_node_set_data_element((*node), env, element);

    if(ns)
    {
        if(axiom_element_set_namespace(element, env, ns, *node) != AXIS2_SUCCESS)
        {
            axiom_node_free_tree(*node, env); /* this will internally free axiom element */
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to set namespace of element");
            return NULL;
        }
    }

    return element;
}

/**
 * Creates an AXIOM element with given qname
 * @param env Environment. MUST NOT be NULL.
 * @param parent parent of the element node to be created. can be NULL.
 * @param qname qname of the elment.cannot be NULL.
 * @param node This is an out parameter. cannot be NULL.
 *                       Returns the node corresponding to the comment created.
 *                       Node type will be set to AXIOM_ELEMENT
 * @return a pointer to the newly created element struct
 */
AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_element_create_with_qname(
    const axutil_env_t * env,
    axiom_node_t * parent,
    const axutil_qname_t * qname,
    axiom_node_t ** node)
{
    axiom_element_t *element;
    axis2_char_t *localpart;
    axis2_char_t *temp_nsuri;
    axis2_char_t *temp_prefix;

    AXIS2_ASSERT(qname != NULL);
    AXIS2_ASSERT(node != NULL);
    AXIS2_ASSERT(env != NULL);

    localpart = axutil_qname_get_localpart(qname, env);
    AXIS2_ASSERT(localpart != NULL);

    element = axiom_element_create(env, parent, localpart, NULL, node);
    if(!element)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to create element with qname");
        return NULL;
    }

    AXIS2_ASSERT(*node != NULL);

    temp_nsuri = axutil_qname_get_uri(qname, env);
    temp_prefix = axutil_qname_get_prefix(qname, env);

    if((!temp_nsuri) || (axutil_strcmp(temp_nsuri, "") == 0))
    {
        /** no namespace uri is available in given qname no need to bother about it */
        return element;
    }

    element->ns = axiom_element_find_namespace(element, env, (*node), temp_nsuri, temp_prefix);
    if(!element->ns)
    {
        /** could not find a namespace so declare namespace */
        axiom_namespace_t *ns = axiom_namespace_create(env, temp_nsuri, temp_prefix);
        if(!ns)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to create namespace needed by element");
            axiom_node_free_tree(*node, env);
            *node = NULL;
            return NULL;
        }

        if(axiom_element_declare_namespace(element, env, *node, ns) != AXIS2_SUCCESS)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to declare namespace needed by element");
            axiom_node_free_tree(*node, env);
            *node = NULL;
            axiom_namespace_free(ns, env);
            return NULL;
        }

        element->ns = ns;
    }
    else
    {
        /* namespace is declared somewhere, but since we are going to keep it, we should
         * increment the reference
         */
        axiom_namespace_increment_ref(element->ns, env);
    }
    return element;
}

/**
 * Frees given element
 * @param element AXIOM element to be freed.
 * @param env Environment. MUST NOT be NULL.
 * @return status of the operation. AXIS2_SUCCESS on success ,AXIS2_FAILURE on error.
 */
AXIS2_EXTERN void AXIS2_CALL
axiom_element_free(
    axiom_element_t * om_element,
    const axutil_env_t * env)
{
    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(env != NULL);
    AXIS2_ASSERT(om_element->localname != NULL);

    axutil_string_free(om_element->localname, env);
    if(om_element->ns)
    {
        axiom_namespace_free(om_element->ns, env);
    }

    if(om_element->attributes)
    {
        axutil_hash_index_t *hi;
        void *val;
        for(hi = axutil_hash_first(om_element->attributes, env); hi; hi = axutil_hash_next(env, hi))
        {
            axutil_hash_this(hi, NULL, NULL, &val);
            AXIS2_ASSERT(val != NULL);
            axiom_attribute_free((axiom_attribute_t *)val, env);
        }
        axutil_hash_free(om_element->attributes, env);
    }

    if(om_element->namespaces)
    {
        axutil_hash_index_t *hi;
        void *val;
        for(hi = axutil_hash_first(om_element->namespaces, env); hi; hi = axutil_hash_next(env, hi))
        {
            axutil_hash_this(hi, NULL, NULL, &val);
            AXIS2_ASSERT(val != NULL);
            axiom_namespace_free((axiom_namespace_t *)val, env);
        }
        axutil_hash_free(om_element->namespaces, env);
    }

    if(om_element->qname)
    {
        axutil_qname_free(om_element->qname, env);
    }
    if(om_element->children_iter)
    {
        axiom_children_iterator_free(om_element->children_iter, env);
    }
    if(om_element->child_ele_iter)
    {
        AXIOM_CHILD_ELEMENT_ITERATOR_FREE(om_element->child_ele_iter, env);
    }
    if(om_element->children_qname_iter)
    {
        axiom_children_qname_iterator_free(om_element->children_qname_iter, env);
    }
    if(om_element->text_value)
    {
        AXIS2_FREE(env->allocator, om_element->text_value);
    }
    AXIS2_FREE(env->allocator, om_element);
}

/**
 * finds a namespace in current element's scope, by uri or prefix or both. Will not check in the
 * parents, so even it is defined in parent nodes, this method will return NULL if it is not defined
 * in element's scope
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @param uri namespace uri, may be null
 * @param prefix prefix
 * @return axiom_namespace_t if found, else return NULL
 */
AXIS2_EXTERN axiom_namespace_t *AXIS2_CALL
axiom_element_find_declared_namespace(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    const axis2_char_t * uri,
    const axis2_char_t * prefix)
{
    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(env != NULL);

    if(om_element->namespaces)
    {
        if(uri && (!prefix || axutil_strcmp(prefix, "") == 0))
        {
            /** prefix is null , so iterate the namespaces hash to find the namespace */
            axutil_hash_index_t *hashindex;
            for(hashindex = axutil_hash_first(om_element->namespaces, env); hashindex;
                hashindex = axutil_hash_next(env, hashindex))
            {
                void *ns = NULL;
                axutil_hash_this(hashindex, NULL, NULL, &ns);
                if(ns)
                {
                    axiom_namespace_t *temp_ns = (axiom_namespace_t *)ns;
                    axis2_char_t *temp_nsuri = axiom_namespace_get_uri(temp_ns, env);
                    if(axutil_strcmp(temp_nsuri, uri) == 0)
                    {
                        /** namespace uri matches, so free hash index and return ns*/
                        AXIS2_FREE(env->allocator, hashindex);
                        return temp_ns;
                    }
                }
            }
        }
        else if(prefix)
        {
            /** prefix is not null get namespace directly if exist */
            axiom_namespace_t *ns = (axiom_namespace_t *)axutil_hash_get(
                om_element->namespaces, prefix, AXIS2_HASH_KEY_STRING);
            if(ns)
            {
                /* if uri provided, return found ns only if uri matches */
                if((uri) && (axutil_strcmp(axiom_namespace_get_uri(ns, env), uri) != 0))
                {
                    ns = NULL;
                }
                return ns;
            }
        }
    }
    return NULL;
}

/**
 * Find a namespace in the scope of the document.
 * Start to find from the given node and go up the hierarchy.
 * @param om_element pointer to om_element_struct contained in node ,
 * @param env Environment. MUST NOT be NULL.
 * @param node node containing an instance of an AXIOM element,cannot be NULL.
 * @param uri namespace uri..
 * @param prefix namespace prefix. can be NULL.
 * @return pointer to the namespace, if found, else NULL. On error, returns
 *           NULL and sets error code in environment,s error
 */
AXIS2_EXTERN axiom_namespace_t *AXIS2_CALL
axiom_element_find_namespace(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_node_t * element_node,
    const axis2_char_t * uri,
    const axis2_char_t * prefix)
{
    axiom_node_t *parent;
    axiom_namespace_t *ns;

    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(env != NULL);
    AXIS2_ASSERT(element_node != NULL);

    /* check whether we can find the namespace in current element scope */
    ns = axiom_element_find_declared_namespace(om_element, env, uri, prefix);
    if(ns)
    {
        return ns;
    }

    /* could not find the namespace in current element scope look in the parent */
    parent = axiom_node_get_parent(element_node, env);
    if((parent) && (axiom_node_get_node_type(parent, env) == AXIOM_ELEMENT))
    {
        axiom_element_t *om_element;
        om_element = (axiom_element_t *)axiom_node_get_data_element(parent, env);
        if(om_element)
        {
            /** parent exist, parent is om element so find in parent*/
            return axiom_element_find_namespace(om_element, env, parent, uri, prefix);
        }
    }
    return NULL;
}

/**
 * Finds a namespace using qname. Start to find from the given node and go up the hierarchy.
 * @param om_element om_element contained in node
 * @param env Environment. MUST NOT be NULL.
 * @param node node containing an instance of an AXIOM element, cannot be NULL.
 * @param qname qname of the namespace to be found. cannot be NULL.
 * @return pointer to the namespace, if found, else NULL. On error, returns
 *           NULL and sets the error code in environment's error struct.
 */
AXIS2_EXTERN axiom_namespace_t *AXIS2_CALL
axiom_element_find_namespace_with_qname(
    axiom_element_t * element,
    const axutil_env_t * env,
    axiom_node_t * node,
    axutil_qname_t * qname)
{
    AXIS2_ASSERT(qname != NULL);
    AXIS2_ASSERT(axutil_qname_get_uri(qname, env) != NULL);

    return axiom_element_find_namespace(element, env, node, axutil_qname_get_uri(qname, env),
            axutil_qname_get_prefix(qname, env));
}

/**
 * Declare a namespace in current element (in the scope of this element ).
 * It checks to see if it is already declared at this level or in its ancestors
 * @param om_element contained in the om node struct
 * @param env Environment. MUST NOT be NULL.
 * @param node node containing an instance of an AXIOM element.
 * @param ns pointer to the namespace struct to be declared. Should not be null
 * @return status of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE.
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_declare_namespace(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_node_t * node,
    axiom_namespace_t * ns)
{
    axiom_namespace_t *declared_ns;
    axis2_char_t *prefix;
    axis2_char_t *uri;

    AXIS2_ASSERT(node != NULL);
    AXIS2_ASSERT(ns != NULL);
    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(env != NULL);

    uri = axiom_namespace_get_uri(ns, env);
    prefix = axiom_namespace_get_prefix(ns, env);
    declared_ns = axiom_element_find_namespace(om_element, env, node, uri, prefix);
    if(declared_ns)
    {
        /*Namespace already declared, so return */
        return AXIS2_SUCCESS;
    }

    if(!om_element->namespaces)
    {
        om_element->namespaces = axutil_hash_make(env);
        if(!om_element->namespaces)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to create namespaces hash map");
            return AXIS2_FAILURE;
        }
    }

    if(prefix)
    {
        axutil_hash_set(om_element->namespaces, prefix, AXIS2_HASH_KEY_STRING, ns);
    }
    else
    {
        /* create a key with empty string */
        axis2_char_t *key;
        key = AXIS2_MALLOC(env->allocator, sizeof(char) * 1);
        if(!key)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Insufficient memory to create key to store namespace");
        }
        key[0] = '\0';
        axutil_hash_set(om_element->namespaces, key, AXIS2_HASH_KEY_STRING, ns);
    }
    axiom_namespace_increment_ref(ns, env);
    return AXIS2_SUCCESS;
}

/**
 * retrieves the default namespace of this element
 * @param om_element pointer to om element
 * @param env axutil_environment MUST Not be NULL
 * @param element_node corresponding om element node of this om element
 * @returns pointer to default namespace if available , NULL otherwise
 */
axiom_namespace_t *AXIS2_CALL
axiom_element_get_default_namespace(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_node_t * element_node)
{
    axiom_node_t *parent_node;
    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(env != NULL);
    AXIS2_ASSERT(element_node != NULL);

    if(om_element->namespaces)
    {
        axiom_namespace_t *default_ns;
        default_ns = axutil_hash_get(om_element->namespaces, "", AXIS2_HASH_KEY_STRING);
        if(default_ns)
        {
            return default_ns;
        }
    }

    parent_node = axiom_node_get_parent(element_node, env);
    if((parent_node) && (axiom_node_get_node_type(parent_node, env) == AXIOM_ELEMENT))
    {
        axiom_element_t *parent_ele;
        parent_ele = (axiom_element_t *)axiom_node_get_data_element(parent_node, env);
        return axiom_element_get_default_namespace(parent_ele, env, parent_node);
    }
    return NULL;
}

/**
 * get the namespace  of om_element
 * @param om_element om_element struct
 * @param env environment, MUST NOT be NULL.
 * @returns pointer to axiom_namespace_t struct
 *          NULL if there is no namespace associated with the element,
 *          NULL on error with error code set to environment's error
 */
AXIS2_EXTERN axiom_namespace_t *AXIS2_CALL
axiom_element_get_namespace(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_node_t * ele_node)
{
    axiom_namespace_t *ns;
    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(env != NULL);
    AXIS2_ASSERT(ele_node != NULL);

    if(om_element->ns)
    {
        ns = om_element->ns;
    }
    else
    {
        ns = axiom_element_get_default_namespace(om_element, env, ele_node);

    }
    return ns;
}

/**
 * set the namespace of the element
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @param ns pointer to namespace. Must not be NULL
 *                       If the value of the namespace has not already been declared
 *                       then the namespace structure ns will be declared and will be
 *                       freed when the tree is freed.
 * @returns status code of the op, with error code
 *                  set to environment's error
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_set_namespace(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_namespace_t * ns,
    axiom_node_t * node)
{
    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(env != NULL);
    AXIS2_ASSERT(ns != NULL);
    AXIS2_ASSERT(node != NULL);

    if(axiom_element_declare_namespace(om_element, env, node, ns) != AXIS2_SUCCESS)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to declare namespace given");
        return AXIS2_FAILURE;
    }
    om_element->ns = ns;
    axiom_namespace_increment_ref(ns, env);
    return AXIS2_SUCCESS;
}

/**
 * get the namespace list of the element
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @returns axutil_hash pointer to namespaces hash
 * this hash table is read only
 */
AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
axiom_element_get_namespaces(
    axiom_element_t * om_element,
    const axutil_env_t * env)
{
    return om_element->namespaces;
}

/**
 * Adds an attribute to current element. The current element takes responsibility of the
 * assigned attribute
 * @param om_element element to which the attribute is to be added.cannot be NULL.
 * @param env Environment. MUST NOT be NULL.
 * @param attribute attribute to be added.
 * @param node axiom_node_t node that om_element is contained in
 * @return status of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE.
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_add_attribute(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_attribute_t * attribute,
    axiom_node_t * element_node)
{
    axutil_qname_t *qname;
    axiom_namespace_t *om_namespace;

    AXIS2_ASSERT(attribute != NULL);
    AXIS2_ASSERT(element_node != NULL);
    AXIS2_ASSERT(env != NULL);
    AXIS2_ASSERT(om_element != NULL);

    om_namespace = axiom_attribute_get_namespace(attribute, env);
    if(om_namespace)
    {
        /* Declare the namespace in element */
        if(axiom_element_declare_namespace(om_element, env, element_node, om_namespace)
            != AXIS2_SUCCESS)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to declare attribute namespace");
            return AXIS2_FAILURE;
        }
    }

    if(!om_element->attributes)
    {
        om_element->attributes = axutil_hash_make(env);
        if(!om_element->attributes)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to create hash map to store attributes");
            return AXIS2_FAILURE;
        }
    }

    qname = axiom_attribute_get_qname(attribute, env);
    if(qname)
    {
        axis2_char_t *name = axutil_qname_to_string(qname, env);
        axutil_hash_set(om_element->attributes, name, AXIS2_HASH_KEY_STRING, attribute);
        axiom_attribute_increment_ref(attribute, env);
    }
    else
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to create qname to store attribute");
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

/**
 * Gets (finds) the attribute with the given qname
 * @param element element whose attribute is to be found.
 * @param env Environment. MUST NOT be NULL.
 * @qname qname qname of the attribute to be found. should not be NULL.
 * @return a pointer to the attribute with given qname if found, else NULL.
 *           On error, returns NULL and sets the error code in environment's error struct.
 */
AXIS2_EXTERN axiom_attribute_t *AXIS2_CALL
axiom_element_get_attribute(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axutil_qname_t * qname)
{
    axis2_char_t *name;
    void *attr;

    AXIS2_ASSERT(env != NULL);
    AXIS2_ASSERT(qname != NULL);
    AXIS2_ASSERT(om_element != NULL);

    /* if there are no attributes, then return NULL */
    if(!om_element->attributes)
    {
        return NULL;
    }

    name = axutil_qname_to_string(qname, env);
    if(!name)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to get string representation of qname");
        return NULL;
    }

    attr = axutil_hash_get(om_element->attributes, name, AXIS2_HASH_KEY_STRING);
    return (axiom_attribute_t *)attr;
}

/**
 * get  the attribute list of the element
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @returns axutil_hash pointer to attributes hash
 * This hash table is read only
 */
AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
axiom_element_get_all_attributes(
    axiom_element_t * om_element,
    const axutil_env_t * env)
{

    return om_element->attributes;
}

/**
 * Gets (finds) the attribute value with the given qname
 * @param element element whose attribute is to be found.
 * @param env Environment. MUST NOT be NULL.
 * @qname qname qname of the attribute to be found. should not be NULL.
 * @return the attribute value with given qname if found, else NULL.
 *  On error, returns NULL and sets the error code in environment's error struct.
 */
AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axiom_element_get_attribute_value(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axutil_qname_t * qname)
{
    axiom_attribute_t *attr = axiom_element_get_attribute(om_element, env, qname);
    if(!attr)
    {
        /* cannot find the attribute with given name. But this might not be an error, and a valid
         * case */
        return NULL;
    }

    return axiom_attribute_get_value(attr, env);
}

/**
 *  Extract attributes , returns a clones hash table of attributes,
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @param om_node pointer to this element node
 */
AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
axiom_element_extract_attributes(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_node_t * ele_node)
{
    axutil_hash_index_t *hi;
    axutil_hash_t *ht_cloned;

    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(env != NULL);
    AXIS2_ASSERT(ele_node != NULL);

    if(!om_element->attributes)
    {
        /* no attributes defined */
        return NULL;
    }

    ht_cloned = axutil_hash_make(env);
    if(!ht_cloned)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to create hashmap to extract attributes");
        return NULL;
    }

    for(hi = axutil_hash_first(om_element->attributes, env); hi; hi = axutil_hash_next(env, hi))
    {
        void *val;
        axiom_attribute_t *cloned_attr;
        axis2_char_t *key = NULL;

        axutil_hash_this(hi, NULL, NULL, &val);
        AXIS2_ASSERT(val != NULL);

        cloned_attr = axiom_attribute_clone((axiom_attribute_t*)val, env);
        if(cloned_attr)
        {
            axutil_qname_t *qn = axiom_attribute_get_qname(cloned_attr, env);
            if(qn)
            {
                key = axutil_qname_to_string(qn, env);
            }
        }

        if(key)
        {
            axutil_hash_set(ht_cloned, key, AXIS2_HASH_KEY_STRING, cloned_attr);
        }
        else
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to clone attribute");
            return NULL;
        }
    }
    return ht_cloned;
}

/**
 * Returns the attribute value as a string for the given element
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @param attr_name the attribute name
 * @return the attribute value as a string
 */
AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axiom_element_get_attribute_value_by_name(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axis2_char_t * attr_name)
{
    axutil_hash_index_t *hi;

    AXIS2_ASSERT(attr_name != NULL);
    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(env != NULL);

    if(!om_element->attributes)
    {
        /* no attributes are defined. */
        return NULL;
    }

    for(hi = axutil_hash_first(om_element->attributes, env); hi; hi = axutil_hash_next(env, hi))
    {
        void *attr;
        axis2_char_t *this_attr_name;
        axiom_namespace_t *attr_ns;
        axis2_char_t *prefix;

        axutil_hash_this(hi, NULL, NULL, &attr);
        AXIS2_ASSERT(attr != NULL);

        this_attr_name = axiom_attribute_get_localname((axiom_attribute_t*)attr, env);
        attr_ns = axiom_attribute_get_namespace((axiom_attribute_t*)attr, env);
        if(attr_ns && (prefix = axiom_namespace_get_prefix(attr_ns, env)) &&
            (axutil_strcmp(prefix, "") != 0))
        {
            /* namespace is defined and prefix is not empty. So, prefix:localname should match
             * with given name
             */
            axis2_char_t *attr_qn_str = axutil_strcat(env, prefix, ":", this_attr_name, NULL);
            if(axutil_strcmp(attr_qn_str, attr_name) != 0)
            {
                /* not the attribute we are looking for */
                AXIS2_FREE(env->allocator, attr_qn_str);
                continue;
            }
            AXIS2_FREE(env->allocator, attr_qn_str);
        }
        else
        {
            /* no namespace or no prefix. so compare only local name */
            if(axutil_strcmp(this_attr_name, attr_name) != 0)
            {
                /* not the attribute we are looking for */
                continue;
            }
        }

        /* we found the attribute */
        AXIS2_FREE(env->allocator, hi);
        return axiom_attribute_get_value((axiom_attribute_t*)attr, env);
    }
    return NULL;
}

/**
 * Select all the text children and concatenate them to a single string. The string
 * returned by this method call will be free by axiom when this method is called again.
 * So it is recommended to have a copy of the return value if this method is going to
 * be called more that once and the return values of the earlier calls are important.
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @param element node , the container node of this om element
 * @return the concatenated text of all text children text values
 *         return null if no text children is available or on error
 */
AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axiom_element_get_text(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_node_t * element_node)
{
    axiom_node_t *temp_node;
    axis2_char_t *dest = NULL;

    AXIS2_ASSERT(env != NULL);
    AXIS2_ASSERT(element_node != NULL);
    AXIS2_ASSERT(om_element != NULL);

    if(om_element->text_value)
    {
        AXIS2_FREE(env->allocator, om_element->text_value);
        om_element->text_value = NULL;
    }

    temp_node = axiom_node_get_first_child(element_node, env);
    while(temp_node)
    {
        if(axiom_node_get_node_type(temp_node, env) == AXIOM_TEXT)
        {
            const axis2_char_t *temp_text;
            axiom_text_t *text_ele;

            text_ele = (axiom_text_t *)axiom_node_get_data_element(temp_node, env);
            AXIS2_ASSERT(text_ele != NULL);
            temp_text = axiom_text_get_value(text_ele, env);
            if(dest && temp_text && axutil_strcmp(temp_text, "") != 0)
            {
                axis2_char_t *temp_dest = axutil_stracat(env, dest, temp_text);
                AXIS2_FREE(env->allocator, dest);
                dest = temp_dest;
            }
            else if(!dest && temp_text && axutil_strcmp(temp_text, "") != 0)
            {
                dest = axutil_strdup(env, temp_text);
            }
        }
        temp_node = axiom_node_get_next_sibling(temp_node, env);
    }

    om_element->text_value = dest;
    return om_element->text_value;
}

/**
 * Sets the text of the given element.
 * caution - This method will wipe out all the text elements (and hence any mixed content)
 * before setting the text
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @param text text to set.
 * @param element_node node of element.
 * @return AXIS2_SUCCESS if attribute was found and removed, else
 *           AXIS2_FAILURE
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_set_text(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    const axis2_char_t * text,
    axiom_node_t * element_node)
{
    axiom_node_t *temp_node, *next_node;

    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(env != NULL);
    AXIS2_ASSERT(text != NULL);
    AXIS2_ASSERT(element_node != NULL);

    next_node = axiom_node_get_first_child(element_node, env);
    while(next_node)
    {
        temp_node = next_node;
        next_node = axiom_node_get_next_sibling(temp_node, env);
        if(axiom_node_get_node_type(temp_node, env) == AXIOM_TEXT)
        {
            axiom_node_free_tree(temp_node, env);
        }
    }

    if(!axiom_text_create(env, element_node, text, &temp_node))
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to set text to element");
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

/**
 * returns the localname of this element
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @returns localname of element, returns NULL on error.
 */
AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axiom_element_get_localname(
    axiom_element_t * om_element,
    const axutil_env_t * env)
{
    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(om_element->localname != NULL);

    return (axis2_char_t *)axutil_string_get_buffer(om_element->localname, env);
}

/**
 * set the localname of this element
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @localname text value to be set as localname
 * @returns status code of operation, AXIS2_SUCCESS on success, AXIS2_FAILURE on error.
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_set_localname(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    const axis2_char_t * localname)
{
    axutil_string_t *new_name;

    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(om_element->localname != NULL);
    AXIS2_ASSERT(localname != NULL);
    AXIS2_ASSERT(env != NULL);

    new_name = axutil_string_create(env, localname);
    if(!new_name)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to set local name of element");
        return AXIS2_FAILURE;
    }

    axutil_string_free(om_element->localname, env);
    om_element->localname = new_name;
    return AXIS2_SUCCESS;
}

/**
 * return qname of this element. The returned qname should not be freed by the caller.
 * It will be freed when om_element struct is freed
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @param ele_node pointer to this element node
 * @returns axutil_qname_t struct , NULL on failure
 */
AXIS2_EXTERN axutil_qname_t *AXIS2_CALL
axiom_element_get_qname(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_node_t * ele_node)
{
    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(env != NULL);
    AXIS2_ASSERT(ele_node != NULL);

    if(!om_element->qname)
    {
        axiom_namespace_t *ns = axiom_element_get_namespace(om_element, env, ele_node);
        const axis2_char_t *localname = axutil_string_get_buffer(om_element->localname, env);
        axis2_char_t *prefix = NULL;
        axis2_char_t *uri = NULL;

        if(ns)
        {
            prefix = axiom_namespace_get_prefix(ns, env);
            uri = axiom_namespace_get_uri(ns, env);
        }

        om_element->qname = axutil_qname_create(env, localname, uri, prefix);
    }
    return om_element->qname;
}

/**
 * returns a list of children iterator. Returned iterator is freed when om_element struct
 * is freed
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @param element_node pointer to this element node
 */
AXIS2_EXTERN axiom_children_iterator_t *AXIS2_CALL
axiom_element_get_children(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_node_t * element_node)
{
    AXIS2_ASSERT(element_node != NULL);
    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(env != NULL);

    if(!om_element->children_iter)
    {
        om_element->children_iter = axiom_children_iterator_create(env,
            axiom_node_get_first_child(element_node, env));
    }
    else
    {
        axiom_children_iterator_reset(om_element->children_iter, env);
    }
    return om_element->children_iter;
}

/**
 * returns a list of children iterator with qname. Returned iterator is freed when om element
 * struct is freed
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @param element_node pointer to this element node
 * @returns children qname iterator struct
 */
AXIS2_EXTERN axiom_children_qname_iterator_t *AXIS2_CALL
axiom_element_get_children_with_qname(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axutil_qname_t * element_qname,
    axiom_node_t * element_node)
{
    AXIS2_ASSERT(element_node != NULL);
    AXIS2_ASSERT(element_qname != NULL);
    AXIS2_ASSERT(om_element != NULL);

    if(om_element->children_qname_iter)
    {
        axiom_children_qname_iterator_free(om_element->children_qname_iter, env);
    }
    om_element->children_qname_iter = axiom_children_qname_iterator_create(env,
        axiom_node_get_first_child(element_node, env), element_qname);
    return om_element->children_qname_iter;
}

/**
 * Returns the first om_element corresponding to element_qname
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @param element_qname qname of the element
 * @param om_node pointer to this element node
 * @param element_node
 * @param child_node
 * @returns children qname iterator struct
 */
AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_element_get_first_child_with_qname(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axutil_qname_t * qname,
    axiom_node_t * element_node,
    axiom_node_t ** child_node)
{
    axiom_children_qname_iterator_t *children_iterator;
    children_iterator = axiom_element_get_children_with_qname(om_element, env, qname, element_node);
    if(!children_iterator)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Could not get children qname iterator");
        return NULL;
    }

    if(axiom_children_qname_iterator_has_next(children_iterator, env))
    {
        axiom_node_t *om_node = axiom_children_qname_iterator_next(children_iterator, env);
        AXIS2_ASSERT(om_node != NULL);
        AXIS2_ASSERT(axiom_node_get_node_type(om_node, env) == AXIOM_ELEMENT);

        if(child_node)
        {
            *child_node = om_node;
        }
        return (axiom_element_t *)axiom_node_get_data_element(om_node, env);
    }

    return NULL;
}

/**
 * returns the first child om element of this om element node
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @param om_node pointer to this element node
 * @return om_element if one is available otherwise return NULL
 */
AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_element_get_first_element(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_node_t * element_node,
    axiom_node_t ** first_ele_node)
{
    axiom_node_t *temp_node;

    AXIS2_ASSERT(env != NULL);
    AXIS2_ASSERT(element_node != NULL);

    temp_node = axiom_node_get_first_child(element_node, env);
    while(temp_node)
    {
        if(axiom_node_get_node_type(temp_node, env) == AXIOM_ELEMENT)
        {
            if(first_ele_node)
            {
                *first_ele_node = temp_node;
            }
            return (axiom_element_t *)axiom_node_get_data_element(temp_node, env);
        }
        else
        {
            temp_node = axiom_node_get_next_sibling(temp_node, env);
        }
    }
    return NULL;
}

/**
 * returns an iterator with child elements of type AXIOM_ELEMENT
 * iterator is freed when om_element node is freed
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @param element_node
 * @returns axiom_child_element_iterator_t , NULL on error
 */
AXIS2_EXTERN axiom_child_element_iterator_t *AXIS2_CALL
axiom_element_get_child_elements(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_node_t * element_node)
{
    AXIS2_ASSERT(env != NULL);
    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(element_node != NULL);

    if(om_element->child_ele_iter)
    {
        return om_element->child_ele_iter;
    }
    else
    {
        axiom_node_t *first_node;
        axiom_element_t *ele;
        ele = axiom_element_get_first_element(om_element, env, element_node, &first_node);
        if(ele)
        {
            AXIS2_ASSERT(first_node != NULL);
            om_element->child_ele_iter = axiom_child_element_iterator_create(env, first_node);
            return om_element->child_ele_iter;
        }
    }
    return NULL;
}



/**
 * Collect all the namespaces with distinct prefixes in the parents of the given element.
 * Effectively this is the set of namespaces declared above this element and might be used by it
 * or its children. Output of this will be used later by axiom_element_redeclare_parent_namespaces
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @param om_node pointer to this element node
 * @returns pointer to hash of relevant namespaces
 */
axutil_hash_t * AXIS2_CALL
axiom_element_gather_parent_namespaces(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_node_t * om_node)
{
    axutil_hash_t *inscope_namespaces;
    axiom_node_t *parent_node = om_node;

    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(env != NULL);
    AXIS2_ASSERT(om_node != NULL);

    inscope_namespaces = axutil_hash_make(env);
    if(!inscope_namespaces)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Unable to create hashmap needed to gather parent namespace");
        return NULL;
    }

    while((parent_node = axiom_node_get_parent(parent_node, env)) &&
        (axiom_node_get_node_type(parent_node, env) == AXIOM_ELEMENT))
    {
        axiom_element_t *parent_element;
        axutil_hash_t *parent_namespaces;
        axutil_hash_index_t *hi;

        parent_element = (axiom_element_t *)axiom_node_get_data_element(parent_node, env);
        parent_namespaces = axiom_element_get_namespaces(parent_element, env);
        if(!parent_namespaces)
        {
            /* no namespaces are declared. So, continue without processing */
            continue;
        }

        for(hi = axutil_hash_first(parent_namespaces, env); hi; hi = axutil_hash_next(env, hi))
        {
            axis2_char_t *key;
            void *val;
            axutil_hash_this(hi, NULL, NULL, &val);
            AXIS2_ASSERT(val != NULL);

            key = axiom_namespace_get_prefix((axiom_namespace_t *)val, env);
            if(!key)
            {
                key = "";
            }

            /* Check if prefix already associated with some namespace in a parent node */
            if(!axutil_hash_get(inscope_namespaces, key, AXIS2_HASH_KEY_STRING))
            {
                /* Remember this namespace as needing to be declared, if used */
                axutil_hash_set(inscope_namespaces, key, AXIS2_HASH_KEY_STRING, val);
            }
        }
    }

    return inscope_namespaces;
}

/**
 * If the provided namespace used by the provided element is one of the namespaces from the
 * parent of the detached node, redeclares that namespace at the element level and removes it
 * from the hash of parent namespaces
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @param om_node pointer to this element node
 * @param ns pointer to namespace to redeclare
 * @param inscope_namespaces pointer to hash of parent namespaces
 */
void AXIS2_CALL
axiom_element_use_parent_namespace(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_node_t * om_node,
    axiom_namespace_t *ns,
    axutil_hash_t *inscope_namespaces)
{
    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(env != NULL);
    AXIS2_ASSERT(om_node != NULL);

    if(ns && inscope_namespaces)
    {
        axiom_namespace_t *parent_ns;
        axis2_char_t *key = axiom_namespace_get_prefix(ns, env);
        if(!key)
        {
            key = "";
        }

        parent_ns = axutil_hash_get(inscope_namespaces, key, AXIS2_HASH_KEY_STRING);
        /* Check if namespace is a namespace declared in a parent and not also declared at an
         * intermediate level */
        if(parent_ns)
        {
            /* declare the namespace. If it is already declared in intermediate level,
             * axiom_elment_declare_namespace will handle it
             */
            axiom_element_declare_namespace(om_element, env, om_node, parent_ns);
            /* Remove the namespace from the inscope parent namespaces now that it has
             been redeclared. */
            axutil_hash_set(inscope_namespaces, key, AXIS2_HASH_KEY_STRING, NULL);
        }
    }
}

/**
 * Examines the subtree beginning at the provided element for each element or attribute,
 * if it refers to a namespace declared in a parent of the subtree root element, if not already
 * declared, redeclares that namespace at the level of the subtree root and removes
 * it from the set of parent inscope_namespaces. inscope_namespaces contains all the parent
 * namespaces which should be redeclared at some point.
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @param om_node pointer to this element node
 * @param inscope_namespaces pointer to hash of parent namespaces
 */
void AXIS2_CALL
axiom_element_redeclare_parent_namespaces(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_node_t * om_node,
    axutil_hash_t *inscope_namespaces)
{
    axiom_node_t *child_node;
    axutil_hash_t * attributes;
    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(env != NULL);
    AXIS2_ASSERT(om_node != NULL);

    /* ensure the element's namespace is declared */
    axiom_element_use_parent_namespace(om_element, env, om_node, om_element->ns, inscope_namespaces);

    /* for each attribute, ensure the attribute's namespace is declared */
    attributes = om_element->attributes;
    if(attributes)
    {
        axutil_hash_index_t *hi;
        for(hi = axutil_hash_first(attributes, env); hi; hi = axutil_hash_next(env, hi))
        {
            void *val;
            axiom_namespace_t* ns;

            axutil_hash_this(hi, NULL, NULL, &val);
            AXIS2_ASSERT(val != NULL);

            ns = axiom_attribute_get_namespace((axiom_attribute_t*)val, env);
            axiom_element_use_parent_namespace(om_element, env, om_node,ns, inscope_namespaces);
        }
    }

    /* ensure the namespaces in all the children are declared */
    child_node = axiom_node_get_first_child(om_node, env);
    while(child_node && (axutil_hash_count(inscope_namespaces) > 0))
    {
        if(axiom_node_get_node_type(child_node, env) == AXIOM_ELEMENT)
        {
            axiom_element_redeclare_parent_namespaces(axiom_node_get_data_element(child_node, env),
                env, child_node, inscope_namespaces);
        }
        child_node = axiom_node_get_next_sibling(child_node, env);
    }
}

/**
 * Serializes the start part of the given element
 * @param element element to be serialized.
 * @param env Environment. MUST NOT be NULL.
 * @param om_output AXIOM output handler to be used in serializing
 * @return status of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
 */
axis2_status_t AXIS2_CALL
axiom_element_serialize_start_part(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_output_t * om_output,
    axiom_node_t * ele_node)
{
    axis2_status_t status = AXIS2_SUCCESS;

    AXIS2_ASSERT(env != NULL);
    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(om_output != NULL);
    AXIS2_ASSERT(ele_node != NULL);

    if(om_element->is_empty)
    {
        if(om_element->ns)
        {
            axis2_char_t *uri = axiom_namespace_get_uri(om_element->ns, env);
            axis2_char_t *prefix = axiom_namespace_get_prefix(om_element->ns, env);
            AXIS2_ASSERT(uri != NULL);

            if(prefix && (axutil_strcmp(prefix, "") != 0))
            {
                status = axiom_output_write(om_output, env, AXIOM_ELEMENT, 4,
                    axutil_string_get_buffer(om_element-> localname, env), uri, prefix, NULL);
            }
            else
            {
                status = axiom_output_write(om_output, env, AXIOM_ELEMENT, 4,
                    axutil_string_get_buffer(om_element-> localname, env), uri, NULL, NULL);
            }
        }
        else
        {
            status = axiom_output_write(om_output, env, AXIOM_ELEMENT, 4,
                axutil_string_get_buffer(om_element-> localname, env), NULL, NULL, NULL);
        }
    }
    else
    {
        if(om_element->ns)
        {
            axis2_char_t *uri = axiom_namespace_get_uri(om_element->ns, env);
            axis2_char_t *prefix = axiom_namespace_get_prefix(om_element->ns, env);
            AXIS2_ASSERT(uri != NULL);

            if(prefix && (axutil_strcmp(prefix, "") != 0))
            {
                status = axiom_output_write(om_output, env, AXIOM_ELEMENT, 3,
                    axutil_string_get_buffer(om_element-> localname, env), uri, prefix);
            }
            else
            {
                status = axiom_output_write(om_output, env, AXIOM_ELEMENT, 2,
                    axutil_string_get_buffer(om_element-> localname, env), uri);
            }
        }
        else
        {
            status = axiom_output_write(om_output, env, AXIOM_ELEMENT, 1,
                axutil_string_get_buffer(om_element-> localname, env));
        }
    }

    if(status != AXIS2_SUCCESS)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "element serialized failed");
        return AXIS2_FAILURE;
    }

    if(om_element->attributes)
    {
        axutil_hash_index_t *hi;
        void *val;
        for(hi = axutil_hash_first(om_element->attributes, env); hi; hi = axutil_hash_next(env, hi))
        {
            axutil_hash_this(hi, NULL, NULL, &val);
            AXIS2_ASSERT(val != NULL);

            if(axiom_attribute_serialize((axiom_attribute_t *)val, env, om_output) != AXIS2_SUCCESS)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "element attribute serialize failed");
                AXIS2_FREE(env->allocator, hi);
                return AXIS2_FAILURE;
            }
        }
    }

    if(om_element->namespaces)
    {
        axutil_hash_index_t *hi;
        void *val;
        for(hi = axutil_hash_first(om_element->namespaces, env); hi; hi = axutil_hash_next(env, hi))
        {
            axutil_hash_this(hi, NULL, NULL, &val);
            AXIS2_ASSERT(val != NULL);

            if(axiom_namespace_serialize((axiom_namespace_t *)val, env, om_output) != AXIS2_SUCCESS)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "element namespace serialize failed");
                AXIS2_FREE(env->allocator, hi);
                return AXIS2_FAILURE;
            }
        }
    }

    return AXIS2_SUCCESS;
}

/**
 * Serializes the end part of the given element. serialize_start_part must
 *     have been called before calling this method.
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @param om_node pointer to this element node
 * @param om_output AXIOM output handler to be used in serializing
 * @return status of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
 */
axis2_status_t AXIS2_CALL
axiom_element_serialize_end_part(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_output_t * om_output)
{
    AXIS2_ASSERT(env != NULL);
    AXIS2_ASSERT(om_element != NULL);
    AXIS2_ASSERT(om_output != NULL);

    return axiom_output_write(om_output, env, AXIOM_ELEMENT, 0);
}

/**
 * Set whether the element is empty or not
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @param is_empty AXIS2_TRUE if empty AXIS2_FALSE if not empty
 * @return VOID
 */
void AXIS2_CALL
axiom_element_set_is_empty(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axis2_bool_t is_empty)
{
    om_element->is_empty = is_empty;
}

/**
 * This method will declare the namespace without checking whether it is already declared. 
 * (This method is only used by codegen. We have to remove this method in future)
 * @param om_element pointer to om_element
 * @param env environment MUST not be NULL
 * @param om_node pointer to this element node
 * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE.
 *
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_declare_namespace_assume_param_ownership(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_namespace_t * ns)
{
    axis2_char_t *prefix = NULL;

    if(!ns || !om_element)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "namespace or om_element is NULL");
        return AXIS2_FAILURE;
    }

    if(!(om_element->namespaces))
    {
        om_element->namespaces = axutil_hash_make(env);
        if(!(om_element->namespaces))
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to create namespaces hash map");
            return AXIS2_FAILURE;
        }
    }
	
    prefix = axiom_namespace_get_prefix(ns, env);
    if(prefix)
    {
        axutil_hash_set(om_element->namespaces, prefix, AXIS2_HASH_KEY_STRING, ns);
    }
    else
    {
        /* create a key with empty string */
        axis2_char_t *key;
        key = AXIS2_MALLOC(env->allocator, sizeof(char) * 1);
        if(!key)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Insufficient memory to create key to store namespace");
        }
        key[0] = '\0';
        axutil_hash_set(om_element->namespaces, key, AXIS2_HASH_KEY_STRING, ns);
    }
    axiom_namespace_increment_ref(ns, env);
    return AXIS2_SUCCESS;
}

#if 0
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_build(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_node_t * om_ele_node)
{
    axiom_stax_builder_t *builder = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    AXIS2_PARAM_CHECK(env->error, om_ele_node, AXIS2_FAILURE);
    if(axiom_node_get_node_type(om_ele_node, env) != AXIOM_ELEMENT)
    {
        return AXIS2_FAILURE;
    }

    builder = axiom_node_get_builder(om_ele_node, env);
    if(!builder)
    {
        return AXIS2_FAILURE;
    }
    while(!axiom_node_is_complete(om_ele_node, env)
        && !axiom_stax_builder_is_complete(builder, env))
    {
        void *value = NULL;
        value = axiom_stax_builder_next(builder, env);
        if(!value)
        {
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_SUCCESS;
}

/**
 * checks for the namespace in the context of this element
 * with the given prefix
 */

AXIS2_EXTERN axiom_namespace_t *AXIS2_CALL
axiom_element_find_namespace_uri(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    const axis2_char_t * prefix,
    axiom_node_t * element_node)
{
    axiom_node_t *parent_node = NULL;
    axiom_namespace_t *ns = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, element_node, NULL);
    AXIS2_PARAM_CHECK(env->error, prefix, NULL);

    if(om_element->namespaces)
    {
        ns = axutil_hash_get(om_element->namespaces, prefix, AXIS2_HASH_KEY_STRING);
        if(ns)
        {
            return ns;
        }
    }

    parent_node = axiom_node_get_parent(element_node, env);
    if((parent_node) && (axiom_node_get_node_type(parent_node, env) == AXIOM_ELEMENT))
    {
        axiom_element_t *parent_ele = NULL;
        parent_ele = (axiom_element_t *)axiom_node_get_data_element(parent_node, env);
        if(parent_ele)
        {
            return axiom_element_find_namespace_uri(parent_ele, env, prefix, parent_node);
        }
    }
    return NULL;
}

AXIS2_EXTERN axutil_string_t *AXIS2_CALL
axiom_element_get_localname_str(
    axiom_element_t * om_element,
    const axutil_env_t * env)
{
    return om_element->localname;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_set_localname_str(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axutil_string_t * localname)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, localname, AXIS2_FAILURE);

    if(om_element->localname)
    {
        axutil_string_free(om_element->localname, env);
        om_element->localname = NULL;
    }

    om_element->localname = axutil_string_clone(localname, env);

    if(!(om_element->localname))
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_set_namespace_with_no_find_in_current_scope(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_namespace_t * om_ns)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_ns, AXIS2_FAILURE);
    om_element->ns = om_ns;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_set_namespace_assume_param_ownership(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_namespace_t * ns)
{
    om_element->ns = ns;
    return AXIS2_SUCCESS;
}

/**
 * declared a default namespace explicitly
 */
AXIS2_EXTERN axiom_namespace_t *AXIS2_CALL
axiom_element_declare_default_namespace(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axis2_char_t * uri)
{
    axiom_namespace_t *default_ns = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, uri, NULL);

    if(axutil_strcmp(uri, "") == 0)
    {
        return NULL;
    }

    default_ns = axiom_namespace_create(env, uri, "");
    if(!default_ns)
    {
        return NULL;
    }
    if(!om_element->namespaces)
    {
        om_element->namespaces = axutil_hash_make(env);
        if(!(om_element->namespaces))
        {
            axiom_namespace_free(default_ns, env);
            return NULL;
        }
    }

    axutil_hash_set(om_element->namespaces, "", AXIS2_HASH_KEY_STRING, default_ns);
    axiom_namespace_increment_ref(default_ns, env);
    return default_ns;
}

AXIS2_EXTERN axiom_element_t *AXIS2_CALL
axiom_element_create_str(
    const axutil_env_t * env,
    axiom_node_t * parent,
    axutil_string_t * localname,
    axiom_namespace_t * ns,
    axiom_node_t ** node)
{
    axiom_element_t *element;

    if(!localname || !node)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "localname or node is NULL");
        return NULL;
    }

    (*node) = axiom_node_create(env);
    if(!(*node))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unable to create axiom node");
        return NULL;
    }

    element = (axiom_element_t *)AXIS2_MALLOC(env->allocator, sizeof(axiom_element_t));
    if(!element)
    {
        AXIS2_FREE(env->allocator, (*node));
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Insufficient memory to create axiom element");
        return NULL;
    }

    memset(element, 0, sizeof(axiom_element_t));
    element->localname = axutil_string_clone(localname, env);
    /* clone can't be null so, no need to check for null validity*/

    if(parent)
    {
        axiom_node_add_child(parent, env, (*node));
    }
    axiom_node_set_node_type((*node), env, AXIOM_ELEMENT);
    axiom_node_set_data_element((*node), env, element);

    if(ns)
    {
        axis2_char_t *uri = NULL;
        axis2_char_t *prefix = NULL;

        uri = axiom_namespace_get_uri(ns, env);
        prefix = axiom_namespace_get_prefix(ns, env);

        element->ns = axiom_element_find_namespace(element, env, *node, uri, prefix);
        if(!(element->ns))
        {
            if(axiom_element_declare_namespace(element, env, *node, ns) == AXIS2_SUCCESS)
            {
                element->ns = ns;
            }
        }
        if(prefix && axutil_strcmp(prefix, "") == 0)
        {
            element->ns = NULL;
        }
    }

    return element;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axiom_element_get_is_empty(
    axiom_element_t * om_element,
    const axutil_env_t * env)
{
    return om_element->is_empty;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_element_remove_attribute(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_attribute_t * om_attribute)
{
    axutil_qname_t *qname = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_attribute, AXIS2_FAILURE);

    qname = axiom_attribute_get_qname(om_attribute, env);
    if(qname && (om_element->attributes))
    {
        axis2_char_t *name = NULL;
        name = axutil_qname_to_string(qname, env);
        if(name)
        {
            axutil_hash_set(om_element->attributes, name, AXIS2_HASH_KEY_STRING, NULL);
            return AXIS2_SUCCESS;
        }
    }
    return AXIS2_FAILURE;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axiom_element_to_string(
    axiom_element_t * om_element,
    const axutil_env_t * env,
    axiom_node_t * element_node)
{
    return axiom_node_to_string(element_node, env);
}
#endif
