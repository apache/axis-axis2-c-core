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
#include "mtom.h"
#include <stdio.h>

axis2_om_node_t *
build_om_programatically(const axis2_env_t *env, axis2_char_t *text);

axis2_om_node_t *
axis2_mtom_mtom (const axis2_env_t *env, axis2_om_node_t *node)
{
    axis2_om_node_t *file_name_node = NULL;
    axis2_om_node_t *file_text_node = NULL;
    axis2_om_node_t *ret_node = NULL;

    AXIS2_ENV_CHECK(env, NULL);
   
    /* Expected request format is :-
     * <ns1:mtomSample xmlns:ns1="http://ws.apache.org/axis2/c/samples">
            <ns1:fileName>test.jpg</ns1:fileName>
            <ns1:image>
                 <xop:Include xmlns:xop="http://www.w3.org/2004/08/xop/include" href="cid:1.dd5183d4-d58a-1da1-2578-001125b4c063@apache.org"></xop:Include>
            </ns1:image>
        </ns1:mtomSample>
     */
    if (!node) /* 'mtomSample' node */
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SVC_SKEL_INPUT_OM_NODE_NULL, AXIS2_FAILURE);
        printf("Echo client ERROR: input parameter NULL\n");
        return NULL;
    }

    file_name_node = AXIS2_OM_NODE_GET_FIRST_CHILD(node, env);
    if (!file_name_node) /* 'text' node */
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SVC_SKEL_INVALID_XML_FORMAT_IN_REQUEST, AXIS2_FAILURE);
        printf("Echo client ERROR: invalid XML in request\n");
        return NULL;
    }
    
    file_text_node = AXIS2_OM_NODE_GET_FIRST_CHILD(file_name_node, env);
    if (!file_text_node) /* actual text to mtom */
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SVC_SKEL_INVALID_XML_FORMAT_IN_REQUEST, AXIS2_FAILURE);
        printf("Echo client ERROR: invalid XML in request\n");
        return NULL;
    }
    
    if (AXIS2_OM_NODE_GET_NODE_TYPE(file_text_node, env) == AXIS2_OM_TEXT)
    {
        axis2_om_text_t *text = (axis2_om_text_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(file_text_node, env);
        if( text && AXIS2_OM_TEXT_GET_VALUE(text , env))
        {
            axis2_om_node_t *image_node = NULL;
            axis2_char_t *text_str = AXIS2_OM_TEXT_GET_VALUE(text, env);
            printf("File Name  %s \n", text_str);
            
            image_node = AXIS2_OM_NODE_GET_NEXT_SIBLING(file_name_node, env);
            if (image_node)
            {
                axis2_om_node_t *inc_node = NULL;
                inc_node = AXIS2_OM_NODE_GET_FIRST_CHILD(image_node, env);
                if (inc_node)
                {
                    axis2_om_node_t *binary_node = NULL;
                    binary_node = AXIS2_OM_NODE_GET_FIRST_CHILD(inc_node, env);
                    if (binary_node)
                    {
                        axis2_data_handler_t *data_handler = NULL;
                        axis2_om_text_t *bin_text = (axis2_om_text_t *)
                            AXIS2_OM_NODE_GET_DATA_ELEMENT(binary_node, env);
                        data_handler = AXIS2_OM_TEXT_SET_GET_DATA_HANDLER(bin_text, env);
                        if (data_handler)
                        {
                            AXIS2_DATA_HANDLER_SET_FILE_NAME(data_handler, env, text_str);
                            AXIS2_DATA_HANDLER_WRITE_TO(data_handler, env);
                            ret_node = build_om_programatically(env, text_str);
                        }
                    }
                }
            }
            
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
axis2_om_node_t *
build_om_programatically(const axis2_env_t *env, axis2_char_t *text)
{
    axis2_om_node_t *mtom_om_node = NULL;
    axis2_om_element_t* mtom_om_ele = NULL;
    axis2_om_node_t* text_om_node = NULL;
    axis2_om_element_t * text_om_ele = NULL;
    axis2_om_namespace_t *ns1 = NULL;
    
    ns1 = axis2_om_namespace_create (env, "http://ws.apache.org/axis2/c/samples", "ns1");

    mtom_om_ele = axis2_om_element_create(env, NULL, "response", ns1, &mtom_om_node);
    
    AXIS2_OM_ELEMENT_SET_TEXT(mtom_om_ele, env, "Image Saved", mtom_om_node);
    
    return mtom_om_node;
}

