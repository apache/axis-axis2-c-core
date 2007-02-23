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


#include <stdio.h>
#include <axis2_util.h>
#include <oxs_constants.h>
#include <oxs_error.h>
#include <oxs_axiom.h>
#include <axiom_node.h>
#include <axiom_namespace.h>
#include <axiom_attribute.h>
#include <axiom_element.h>
#include <axiom_document.h>
#include <axiom_stax_builder.h>
#include <axiom_util.h>

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_axiom_add_attribute(const axis2_env_t *env,
    axiom_node_t* node,
    axis2_char_t* attribute_ns,
    axis2_char_t* attribute_ns_uri,
    axis2_char_t* attribute,
    axis2_char_t* value)
{
    axiom_attribute_t *attr = NULL;
    axiom_element_t *ele = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axiom_namespace_t *ns = NULL;

    ns =  axiom_namespace_create(env, attribute_ns_uri, attribute_ns);

    ele =  AXIOM_NODE_GET_DATA_ELEMENT(node, env);
    attr =  axiom_attribute_create(env, attribute , value, ns);
    status = AXIOM_ELEMENT_ADD_ATTRIBUTE(ele, env, attr, node);
    
    return status;
}

AXIS2_EXTERN int AXIS2_CALL
oxs_axiom_get_number_of_children_with_qname(const axis2_env_t *env,
        axiom_node_t* parent,
        axis2_char_t* local_name,
        axis2_char_t* ns_uri,
        axis2_char_t* prefix)
{

    axis2_qname_t *qname = NULL;
    axiom_element_t *parent_ele = NULL;
    axiom_children_qname_iterator_t *qname_iter = NULL;
    axiom_node_t *temp_node = NULL;
    int counter = 0;

    qname = axis2_qname_create(env, local_name, ns_uri, prefix);
    parent_ele = AXIOM_NODE_GET_DATA_ELEMENT(parent, env);
    if (!parent_ele)
    {
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "Cannot find %s element", local_name);
        return -1;
    }

    qname_iter = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(parent_ele, env, qname, parent);
    while (AXIS2_TRUE == AXIOM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(qname_iter , env))
    {

        counter++;
        temp_node = AXIOM_CHILDREN_QNAME_ITERATOR_NEXT(qname_iter, env);
    }

    return counter;
}

/**
 * Traverse thru the node and its children. Check if the localname is equal to the given name
 * */
AXIS2_EXTERN axiom_node_t* AXIS2_CALL
oxs_axiom_get_node_by_local_name(const axis2_env_t *env,
    axiom_node_t *node,
    axis2_char_t *local_name)
{
    axis2_char_t *temp_name = NULL;
    
    if(!node){return NULL;}
    
    if(AXIOM_NODE_GET_NODE_TYPE(node, env) != AXIOM_ELEMENT){return NULL;}

    temp_name = axiom_util_get_localname(node, env);
    AXIS2_LOG_INFO(env->log, "[rampart][axiom] Checking node %s for %s", temp_name, local_name );

    if(0 == AXIS2_STRCMP(temp_name, local_name) ){
        /*Gottcha.. return this node*/
        return node;
    }else{
        /*Doesn't match? Get the first child*/    
        axiom_node_t *temp_node = NULL;

        temp_node = AXIOM_NODE_GET_FIRST_CHILD(node, env);
        while (temp_node)
        {
            axiom_node_t *res_node = NULL;       
            res_node = oxs_axiom_get_node_by_local_name(env, temp_node, local_name);
            if(res_node){
                return res_node;
            }
            temp_node = AXIOM_NODE_GET_NEXT_SIBLING(temp_node, env);
        } 
    
    }
    return NULL;
}

/**
 * Traverse thru the node and its children. Check if the id attribute is equal to the given value
 * */
AXIS2_EXTERN axiom_node_t* AXIS2_CALL
oxs_axiom_get_node_by_id(const axis2_env_t *env,
    axiom_node_t *node,
    axis2_char_t *attr,
    axis2_char_t *val)
{
    axis2_char_t *attribute_value = NULL;
    axis2_char_t *localname = NULL;
    
    if(!node){return NULL;}
    
    if(AXIOM_NODE_GET_NODE_TYPE(node, env) != AXIOM_ELEMENT){return NULL;}

    localname = axiom_util_get_localname(node, env);
    /*AXIS2_LOG_INFO(env->log, "[rampart][axiom] Checking node %s for the attribute %s with value = %s", localname, attr, val);*/

    attribute_value = oxs_axiom_get_attribute_value_of_node_by_name(env, node, attr);
    if(0 == AXIS2_STRCMP(val, attribute_value) ){
        /*Gottcha.. return this node*/
        return node;
    }else{
        /*Doesn't match? Get the first child*/    
        axiom_node_t *temp_node = NULL;

        temp_node = AXIOM_NODE_GET_FIRST_CHILD(node, env);
        while (temp_node)
        {
            axiom_node_t *res_node = NULL;       
            res_node = oxs_axiom_get_node_by_id(env, temp_node, attr, val);
            if(res_node){
                return res_node;
            }
            temp_node = AXIOM_NODE_GET_NEXT_SIBLING(temp_node, env);
        } 
    
    }
    return NULL;
}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_axiom_get_attribute_value_of_node_by_name(const axis2_env_t *env,
    axiom_node_t *node,
    axis2_char_t *attribute_name)
{
    axis2_char_t *attribute_value = NULL;
    axiom_element_t *ele = NULL;
    
    ele = AXIOM_NODE_GET_DATA_ELEMENT(node, env);
    attribute_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele, env, attribute_name);
    
    return attribute_value;
}

AXIS2_EXTERN axiom_node_t* AXIS2_CALL
oxs_axiom_get_first_child_node_by_name(const axis2_env_t *env,
        axiom_node_t* parent,
        axis2_char_t* local_name,
        axis2_char_t* ns_uri,
        axis2_char_t* prefix)
{
    axis2_qname_t *qname = NULL;
    axiom_node_t *node = NULL;
    axiom_element_t *parent_ele = NULL;
    axiom_element_t *ele = NULL;
    axis2_char_t *parent_name = NULL;

    qname = axis2_qname_create(env, local_name, ns_uri, prefix);
    parent_ele = AXIOM_NODE_GET_DATA_ELEMENT(parent, env);
    if (!parent_ele)
    {
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "Cannot find %s element", local_name);
        return NULL;
    }
    /*Get the child*/
    ele = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(parent_ele, env, qname, parent, &node);

    AXIS2_QNAME_FREE(qname, env);
    qname = NULL;

    parent_name = AXIOM_NODE_TO_STRING(parent, env);
    if (!node)
    {
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "Cannot find child %s of %s", local_name, parent_name);
        return NULL;
    }
    return node;
}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_axiom_get_node_content(const axis2_env_t *env, axiom_node_t* node)
{
    axiom_element_t *ele = NULL;
    axis2_char_t *content = NULL;

    ele = AXIOM_NODE_GET_DATA_ELEMENT(node, env);
    if (!ele) return NULL;

    content = AXIOM_ELEMENT_GET_TEXT(ele, env, node);
    if (!content) return NULL;

    return content;
}

AXIS2_EXTERN axiom_node_t *AXIS2_CALL
oxs_axiom_deserialize_node(const axis2_env_t *env,  axis2_char_t* buffer)
{

    axiom_document_t *doc = NULL;
    axiom_stax_builder_t *builder = NULL;
    axiom_xml_reader_t *reader = NULL;
    axiom_node_t *node = NULL;

    if (!buffer)
    {
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "buffer is NULL");
        return NULL;
    }
    reader = axiom_xml_reader_create_for_memory(env,
            (void*)buffer, AXIS2_STRLEN(buffer), "utf-8", AXIS2_XML_PARSER_TYPE_BUFFER);

    if (!reader)
    {
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "axiom_xml_reader is NULL");
        return NULL;
    }

    builder = axiom_stax_builder_create(env, reader);
    if (!builder)
    {
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "axiom_stax_builder is NULL");
        return NULL;
    }

    doc = axiom_document_create(env, NULL, builder);
    if (!doc)
    {
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "axiom_document is NULL");
        return NULL;
    }
    node = AXIOM_DOCUMENT_BUILD_ALL(doc, env);
    if (!node)
    {
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "Building node failed");
        return NULL;
    }

    return node;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_axiom_check_node_name(const axis2_env_t *env, axiom_node_t* node, axis2_char_t* name, axis2_char_t* ns)
{
    int ret_name, ret_ns;
    axiom_element_t * ele = NULL;
    axis2_char_t* namestr = NULL;
    axis2_char_t* ns_str = NULL;
    axis2_qname_t* qname = NULL;

    ele = AXIOM_NODE_GET_DATA_ELEMENT(node, env);
    qname = AXIOM_ELEMENT_GET_QNAME(ele, env, node);

    namestr = AXIS2_QNAME_GET_LOCALPART(qname, env);
    ret_name =  AXIS2_STRCMP(namestr, name) ;


    if (ret_name < 0) return 0;

    if (ns)
    {
        ns_str = AXIS2_QNAME_GET_URI(qname, env);
        ret_ns =  AXIS2_STRCMP(ns_str, ns) ;
        if (ret_ns < 0) return AXIS2_FAILURE;
        else   return AXIS2_SUCCESS;

    }
    else
    {
        return AXIS2_SUCCESS;
    }


}

