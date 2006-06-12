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
#include "notify.h"
#include <stdio.h>

void
axis2_notify_notify (const axis2_env_t *env, axiom_node_t *node)
{
    axiom_node_t *text_node = NULL;

    if (!env || !env)
        return;
   
    /* Expected request format is :-
       <m:notify xmlns:m="http://example.org/notify">Message 3</m:notify>
     */
    if (!node) /* 'notify' node */
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SVC_SKEL_INPUT_OM_NODE_NULL, AXIS2_FAILURE);
        printf("Echo client ERROR: input parameter NULL\n");
        return;
    }

    text_node = AXIOM_NODE_GET_FIRST_CHILD(node, env);
    if (!node) /* actual text to notify */
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SVC_SKEL_INVALID_XML_FORMAT_IN_REQUEST, AXIS2_FAILURE);
        printf("Echo client ERROR: invalid XML in request\n");
        return;
    }
    
    if (AXIOM_NODE_GET_NODE_TYPE(text_node, env) == AXIOM_TEXT)
    {
        axiom_text_t *text = (axiom_text_t *)AXIOM_NODE_GET_DATA_ELEMENT(text_node, env);
        if( text && AXIOM_TEXT_GET_VALUE(text , env))
        {
            axis2_char_t *text_str = AXIOM_TEXT_GET_VALUE(text, env);
            printf("Notification received :  %s \n", text_str);
        }
    }
    else
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SVC_SKEL_INVALID_XML_FORMAT_IN_REQUEST, AXIS2_FAILURE);
        printf("Echo client ERROR: invalid XML in request\n");
        return;
    }
   
    return;
}


