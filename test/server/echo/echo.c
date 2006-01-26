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

axis2_om_node_t *
echo (axis2_om_node_t *node)
{
    axis2_allocator_t *allocator = NULL;
    axis2_env_t *env = NULL;
    axis2_error_t *error = NULL;
    axis2_log_t *axis_log     = NULL;

    axis2_om_node_t *text_parent_node = NULL;
    axis2_om_node_t *text_node = NULL;

    allocator = axis2_allocator_init (NULL);
    axis_log = axis2_log_create(allocator, NULL);
    error = axis2_error_create(allocator);
    env = axis2_env_create_with_error_log(allocator, error,  axis_log);
                
    
    if (!node)
    {
        printf("Echo client ERROR: input parameter NULL\n");
        return NULL;
    }

    text_parent_node = AXIS2_OM_NODE_GET_FIRST_CHILD(node, &env);
    if (!text_parent_node)
    {
        printf("Echo client ERROR: invalid XML in request\n");
        return node;
    }
    
    text_node = AXIS2_OM_NODE_GET_FIRST_CHILD(text_parent_node, &env);
    if (!text_node)
    {
        printf("Echo client ERROR: invalid XML in request\n");
        return node;
    }
    
    if (AXIS2_OM_NODE_GET_NODE_TYPE(text_node, &env) == AXIS2_OM_TEXT)
    {
        axis2_om_text_t *text = (axis2_om_text_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(text_node, &env);
        if( text && AXIS2_OM_TEXT_GET_VALUE(text ,&env))
            printf("Eching text value  %s \n", AXIS2_OM_TEXT_GET_VALUE(text,&env));
    }
    else
    {
        printf("Echo client ERROR: invalid XML in request\n");
    }
	
    return node;
}

