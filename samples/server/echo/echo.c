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
#include <axis2_om_element.h>

axis2_om_node_t *
build_om_programatically(axis2_env_t **env, axis2_char_t *text);

axis2_om_node_t *
axis2_echo_echo (axis2_env_t **env, axis2_om_node_t *node)
{
    axis2_om_node_t *text_parent_node = NULL;
    axis2_om_node_t *text_node = NULL;
    axis2_om_node_t *ret_node = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    
    if (!node)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SVC_SKEL_INPUT_OM_NODE_NULL, AXIS2_FAILURE);
        printf("Echo client ERROR: input parameter NULL\n");
        return NULL;
    }

    text_parent_node = AXIS2_OM_NODE_GET_FIRST_CHILD(node, env);
    if (!text_parent_node)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SVC_SKEL_INVALID_XML_FORMAT_IN_REQUEST, AXIS2_FAILURE);
        printf("Echo client ERROR: invalid XML in request\n");
        return NULL;
    }
    
    text_node = AXIS2_OM_NODE_GET_FIRST_CHILD(text_parent_node, env);
    if (!text_node)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SVC_SKEL_INVALID_XML_FORMAT_IN_REQUEST, AXIS2_FAILURE);
        printf("Echo client ERROR: invalid XML in request\n");
        return NULL;
    }
    
    if (AXIS2_OM_NODE_GET_NODE_TYPE(text_node, env) == AXIS2_OM_TEXT)
    {
        axis2_om_text_t *text = (axis2_om_text_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(text_node, env);
        if( text && AXIS2_OM_TEXT_GET_VALUE(text , env))
        {
            axis2_char_t *text_str = AXIS2_OM_TEXT_GET_VALUE(text, env);
            printf("Echoing text value  %s \n", text_str);
            ret_node = build_om_programatically(env, text_str);
        }
    }
    else
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SVC_SKEL_INVALID_XML_FORMAT_IN_REQUEST, AXIS2_FAILURE);
        printf("Echo client ERROR: invalid XML in request\n");
        return NULL;
    }
	
    return ret_node;
}

axis2_om_node_t *
build_om_programatically(axis2_env_t **env, axis2_char_t *text)
{
    axis2_om_node_t *echo_om_node = NULL;
    axis2_om_element_t* echo_om_ele = NULL;
    axis2_om_node_t* text_om_node = NULL;
    axis2_om_element_t * text_om_ele = NULL;
    axis2_om_namespace_t *ns1 = NULL;
    

    axis2_xml_writer_t *xml_writer = NULL;
    axis2_om_output_t *om_output = NULL;
    axis2_char_t *buffer = NULL;

    ns1 = axis2_om_namespace_create (env, "http://localhost:9090/axis2/services/echo", "ns1");

    echo_om_ele = axis2_om_element_create(env, NULL, "echoString", ns1, &echo_om_node);
    
    text_om_ele = axis2_om_element_create(env, echo_om_node, "text", NULL, &text_om_node);

    AXIS2_OM_ELEMENT_SET_TEXT(text_om_ele, env, text, text_om_node);
    
    
    xml_writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_FALSE, AXIS2_FALSE);
    om_output = axis2_om_output_create( env, xml_writer);
    
    AXIS2_OM_NODE_SERIALIZE(echo_om_node, env, om_output);
    buffer = AXIS2_XML_WRITER_GET_XML(xml_writer, env);         
    printf("\nSending OM node in XML : %s \n",  buffer); 

    AXIS2_OM_OUTPUT_FREE(om_output, env);
    AXIS2_FREE((*env)->allocator, buffer);

    return echo_om_node;
}

