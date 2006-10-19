/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "echo.h"
#include <axiom_xml_writer.h>
#include <stdio.h>

axiom_node_t *
build_om_programatically(const axis2_env_t *env, axis2_char_t *text);

axiom_node_t *
axis2_echo_echo(const axis2_env_t *env, axiom_node_t *node)
{
    axiom_node_t *text_parent_node = NULL;
    axiom_node_t *text_node = NULL;
    axiom_node_t *ret_node = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    /* Expected request format is :-
     * <ns1:echoString xmlns:ns1="http://ws.apache.org/axis2/c/samples">
     *      <text>echo5</text>
     * </ns1:echoString>
     */
    if (!node) /* 'echoString' node */
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SVC_SKEL_INPUT_OM_NODE_NULL, AXIS2_FAILURE);
        printf("Echo client ERROR: input parameter NULL\n");
        return NULL;
    }
    else
    {
        axiom_xml_writer_t *writer = NULL;
        axiom_output_t *om_output = NULL;
     /*   axis2_char_t *buffer = NULL;*/

        writer = axiom_xml_writer_create_for_memory(env, NULL, AXIS2_TRUE, 0,
                AXIS2_XML_PARSER_TYPE_BUFFER);
        om_output = axiom_output_create(env, writer);

        /*AXIOM_NODE_SERIALIZE (node, env, om_output);
        buffer = AXIOM_XML_WRITER_GET_XML(writer, env);
        printf("buffer = %s \n", buffer);*/
    }

    text_parent_node = AXIOM_NODE_GET_FIRST_CHILD(node, env);
    if (!text_parent_node) /* 'text' node */
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SVC_SKEL_INVALID_XML_FORMAT_IN_REQUEST, AXIS2_FAILURE);
        printf("Echo client ERROR: invalid XML in request\n");
        return NULL;
    }

    text_node = AXIOM_NODE_GET_FIRST_CHILD(text_parent_node, env);
    if (!text_node) /* actual text to echo */
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SVC_SKEL_INVALID_XML_FORMAT_IN_REQUEST, AXIS2_FAILURE);
        printf("Echo client ERROR: invalid XML in request\n");
        return NULL;
    }

    if (AXIOM_NODE_GET_NODE_TYPE(text_node, env) == AXIOM_TEXT)
    {
        axiom_text_t *text = (axiom_text_t *)AXIOM_NODE_GET_DATA_ELEMENT(text_node, env);
        if (text && AXIOM_TEXT_GET_VALUE(text , env))
        {
            axis2_char_t *text_str = AXIOM_TEXT_GET_VALUE(text, env);
            /*printf("Echoing text value  %s \n", text_str);*/
            ret_node = build_om_programatically(env, text_str);
        }
    }
    else
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SVC_SKEL_INVALID_XML_FORMAT_IN_REQUEST, AXIS2_FAILURE);
        printf("Echo client ERROR: invalid XML in request\n");
        return NULL;
    }

    return ret_node;
}

/* Builds the response content */
axiom_node_t *
build_om_programatically(const axis2_env_t *env, axis2_char_t *text)
{
    axiom_node_t *echo_om_node = NULL;
    axiom_element_t* echo_om_ele = NULL;
    axiom_node_t* text_om_node = NULL;
    axiom_element_t * text_om_ele = NULL;
    axiom_namespace_t *ns1 = NULL;

    ns1 = axiom_namespace_create(env, "http://ws.apache.org/axis2/c/samples", "ns1");

    echo_om_ele = axiom_element_create(env, NULL, "echoString", ns1, &echo_om_node);

    text_om_ele = axiom_element_create(env, echo_om_node, "text", NULL, &text_om_node);

    AXIOM_ELEMENT_SET_TEXT(text_om_ele, env, text, text_om_node);

    return echo_om_node;
}

