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
#include "math.h"

axis2_om_node_t *
axis2_math_svc_add (axis2_om_node_t *node)
{
    axis2_allocator_t *allocator = NULL;
    axis2_env_t *env = NULL;
    axis2_error_t *error = NULL;
    axis2_log_t *axis_log     = NULL;

    axis2_om_node_t *param1_node = NULL;
    axis2_om_node_t *param1_text_node = NULL;
    axis2_char_t *param1_str = NULL;
    long int param1 = 0;
    axis2_om_node_t *param2_node = NULL;
    axis2_om_node_t *param2_text_node = NULL;
    axis2_char_t *param2_str = NULL;
    long int param2 = 0;

    allocator = axis2_allocator_init (NULL);
    axis_log = axis2_log_create(allocator, NULL);
    error = axis2_error_create(allocator);
    env = axis2_env_create_with_error_log(allocator, error,  axis_log);
                
    
    if (!node)
    {
        printf("Math client request ERROR: input parameter NULL\n");
        return NULL;
    }

    param1_node = AXIS2_OM_NODE_GET_FIRST_CHILD(node, &env);
    if (!param1_node)
    {
        printf("Math service  ERROR: invalid XML in request\n");
        /** TODO: return a SOAP Fault here*/
        return node;
    }
    
    param1_text_node = AXIS2_OM_NODE_GET_FIRST_CHILD(param1_node, &env);
    if (!param1_text_node)
    {
        printf("Math service ERROR: invalid XML in request\n");
        /** TODO: return a SOAP Fault here*/
        return node;
    }
    
    if (AXIS2_OM_NODE_GET_NODE_TYPE(param1_text_node, &env) == AXIS2_OM_TEXT)
    {
        axis2_om_text_t *text = (axis2_om_text_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(param1_text_node, &env);
        if( text && AXIS2_OM_TEXT_GET_VALUE(text ,&env))
        {
            param1_str = AXIS2_OM_TEXT_GET_VALUE(text,&env);
        }
    }
    else
    {
        printf("Math service ERROR: invalid XML in request\n");
    }
	
    param2_node = AXIS2_OM_NODE_GET_NEXT_SIBLING(param1_node, &env);
    if (!param2_node)
    {
        printf("Math service  ERROR: invalid XML in request\n");
        /** TODO: return a SOAP Fault here*/
        return node;
    }
    
    param2_text_node = AXIS2_OM_NODE_GET_FIRST_CHILD(param2_node, &env);
    if (!param2_text_node)
    {
        printf("Math service ERROR: invalid XML in request\n");
        /** TODO: return a SOAP Fault here*/
        return node;
    }
    
    if (AXIS2_OM_NODE_GET_NODE_TYPE(param2_text_node, &env) == AXIS2_OM_TEXT)
    {
        axis2_om_text_t *text = (axis2_om_text_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(param2_text_node, &env);
        if( text && AXIS2_OM_TEXT_GET_VALUE(text ,&env))
        {
            param2_str = AXIS2_OM_TEXT_GET_VALUE(text,&env);
        }
    }
    else
    {
        printf("Math service ERROR: invalid XML in request\n");
    }
	
    if (param1_str && param2_str)
    {
        long int result = 0;
        axis2_char_t result_str[255];

        axis2_om_element_t *ele1 = NULL;
        axis2_om_node_t *node1 = NULL, *node2 = NULL;
        axis2_om_namespace_t *ns1 = NULL;
        axis2_om_text_t *text1 = NULL;
        
        param1 = strtol(param1_str, NULL, 10);
        param2 = strtol(param2_str, NULL, 10);
        result = param1 + param2;
        sprintf(result_str, "%ld", result);

        

        ns1 = axis2_om_namespace_create (&env,
                                   "http://axis2/test/namespace1",
                                   "ns1");
        ele1 = axis2_om_element_create (&env, NULL, "result", ns1, &node1);
        text1 = axis2_om_text_create (&env, node1, result_str, &node2);

        return node1;
    }
    
    printf("Math service ERROR: invalid parameters\n");
    /** TODO: return a SOAP Fault here*/
    return node;
}

axis2_om_node_t *
axis2_math_svc_sub (axis2_om_node_t *node)
{
    axis2_allocator_t *allocator = NULL;
    axis2_env_t *env = NULL;
    axis2_error_t *error = NULL;
    axis2_log_t *axis_log     = NULL;

    axis2_om_node_t *param1_node = NULL;
    axis2_om_node_t *param1_text_node = NULL;
    axis2_char_t *param1_str = NULL;
    long int param1 = 0;
    axis2_om_node_t *param2_node = NULL;
    axis2_om_node_t *param2_text_node = NULL;
    axis2_char_t *param2_str = NULL;
    long int param2 = 0;

    allocator = axis2_allocator_init (NULL);
    axis_log = axis2_log_create(allocator, NULL);
    error = axis2_error_create(allocator);
    env = axis2_env_create_with_error_log(allocator, error,  axis_log);
                
    
    if (!node)
    {
        printf("Math client request ERROR: input parameter NULL\n");
        return NULL;
    }

    param1_node = AXIS2_OM_NODE_GET_FIRST_CHILD(node, &env);
    if (!param1_node)
    {
        printf("Math service  ERROR: invalid XML in request\n");
        /** TODO: return a SOAP Fault here*/
        return node;
    }
    
    param1_text_node = AXIS2_OM_NODE_GET_FIRST_CHILD(param1_node, &env);
    if (!param1_text_node)
    {
        printf("Math service ERROR: invalid XML in request\n");
        /** TODO: return a SOAP Fault here*/
        return node;
    }
    
    if (AXIS2_OM_NODE_GET_NODE_TYPE(param1_text_node, &env) == AXIS2_OM_TEXT)
    {
        axis2_om_text_t *text = (axis2_om_text_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(param1_text_node, &env);
        if( text && AXIS2_OM_TEXT_GET_VALUE(text ,&env))
        {
            param1_str = AXIS2_OM_TEXT_GET_VALUE(text,&env);
        }
    }
    else
    {
        printf("Math service ERROR: invalid XML in request\n");
    }
	
    param2_node = AXIS2_OM_NODE_GET_NEXT_SIBLING(param1_node, &env);
    if (!param2_node)
    {
        printf("Math service  ERROR: invalid XML in request\n");
        /** TODO: return a SOAP Fault here*/
        return node;
    }
    
    param2_text_node = AXIS2_OM_NODE_GET_FIRST_CHILD(param2_node, &env);
    if (!param2_text_node)
    {
        printf("Math service ERROR: invalid XML in request\n");
        /** TODO: return a SOAP Fault here*/
        return node;
    }
    
    if (AXIS2_OM_NODE_GET_NODE_TYPE(param2_text_node, &env) == AXIS2_OM_TEXT)
    {
        axis2_om_text_t *text = (axis2_om_text_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(param2_text_node, &env);
        if( text && AXIS2_OM_TEXT_GET_VALUE(text ,&env))
        {
            param2_str = AXIS2_OM_TEXT_GET_VALUE(text,&env);
        }
    }
    else
    {
        printf("Math service ERROR: invalid XML in request\n");
    }
	
    if (param1_str && param2_str)
    {
        long int result = 0;
        axis2_char_t result_str[255];

        axis2_om_element_t *ele1 = NULL;
        axis2_om_node_t *node1 = NULL, *node2 = NULL;
        axis2_om_namespace_t *ns1 = NULL;
        axis2_om_text_t *text1 = NULL;
        
        param1 = strtol(param1_str, NULL, 10);
        param2 = strtol(param2_str, NULL, 10);
        result = param1 - param2;
        sprintf(result_str, "%ld", result);

        

        ns1 = axis2_om_namespace_create (&env,
                                   "http://axis2/test/namespace1",
                                   "ns1");
        ele1 = axis2_om_element_create (&env, NULL, "result", ns1, &node1);
        text1 = axis2_om_text_create (&env, node1, result_str, &node2);

        return node1;
    }
    
    printf("Math service ERROR: invalid parameters\n");
    /** TODO: return a SOAP Fault here*/
    return node;
}


axis2_om_node_t *
axis2_math_svc_mul (axis2_om_node_t *node)
{
    axis2_allocator_t *allocator = NULL;
    axis2_env_t *env = NULL;
    axis2_error_t *error = NULL;
    axis2_log_t *axis_log     = NULL;

    axis2_om_node_t *param1_node = NULL;
    axis2_om_node_t *param1_text_node = NULL;
    axis2_char_t *param1_str = NULL;
    long int param1 = 0;
    axis2_om_node_t *param2_node = NULL;
    axis2_om_node_t *param2_text_node = NULL;
    axis2_char_t *param2_str = NULL;
    long int param2 = 0;

    allocator = axis2_allocator_init (NULL);
    axis_log = axis2_log_create(allocator, NULL);
    error = axis2_error_create(allocator);
    env = axis2_env_create_with_error_log(allocator, error,  axis_log);
                
    
    if (!node)
    {
        printf("Math client request ERROR: input parameter NULL\n");
        return NULL;
    }

    param1_node = AXIS2_OM_NODE_GET_FIRST_CHILD(node, &env);
    if (!param1_node)
    {
        printf("Math service  ERROR: invalid XML in request\n");
        /** TODO: return a SOAP Fault here*/
        return node;
    }
    
    param1_text_node = AXIS2_OM_NODE_GET_FIRST_CHILD(param1_node, &env);
    if (!param1_text_node)
    {
        printf("Math service ERROR: invalid XML in request\n");
        /** TODO: return a SOAP Fault here*/
        return node;
    }
    
    if (AXIS2_OM_NODE_GET_NODE_TYPE(param1_text_node, &env) == AXIS2_OM_TEXT)
    {
        axis2_om_text_t *text = (axis2_om_text_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(param1_text_node, &env);
        if( text && AXIS2_OM_TEXT_GET_VALUE(text ,&env))
        {
            param1_str = AXIS2_OM_TEXT_GET_VALUE(text,&env);
        }
    }
    else
    {
        printf("Math service ERROR: invalid XML in request\n");
    }
	
    param2_node = AXIS2_OM_NODE_GET_NEXT_SIBLING(param1_node, &env);
    if (!param2_node)
    {
        printf("Math service  ERROR: invalid XML in request\n");
        /** TODO: return a SOAP Fault here*/
        return node;
    }
    
    param2_text_node = AXIS2_OM_NODE_GET_FIRST_CHILD(param2_node, &env);
    if (!param2_text_node)
    {
        printf("Math service ERROR: invalid XML in request\n");
        /** TODO: return a SOAP Fault here*/
        return node;
    }
    
    if (AXIS2_OM_NODE_GET_NODE_TYPE(param2_text_node, &env) == AXIS2_OM_TEXT)
    {
        axis2_om_text_t *text = (axis2_om_text_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(param2_text_node, &env);
        if( text && AXIS2_OM_TEXT_GET_VALUE(text ,&env))
        {
            param2_str = AXIS2_OM_TEXT_GET_VALUE(text,&env);
        }
    }
    else
    {
        printf("Math service ERROR: invalid XML in request\n");
    }
	
    if (param1_str && param2_str)
    {
        long int result = 0;
        axis2_char_t result_str[255];

        axis2_om_element_t *ele1 = NULL;
        axis2_om_node_t *node1 = NULL, *node2 = NULL;
        axis2_om_namespace_t *ns1 = NULL;
        axis2_om_text_t *text1 = NULL;
        
        param1 = strtol(param1_str, NULL, 10);
        param2 = strtol(param2_str, NULL, 10);
        result = param1 * param2;
        sprintf(result_str, "%ld", result);

        

        ns1 = axis2_om_namespace_create (&env,
                                   "http://axis2/test/namespace1",
                                   "ns1");
        ele1 = axis2_om_element_create (&env, NULL, "result", ns1, &node1);
        text1 = axis2_om_text_create (&env, node1, result_str, &node2);

        return node1;
    }
    
    printf("Math service ERROR: invalid parameters\n");
    /** TODO: return a SOAP Fault here*/
    return node;
}


axis2_om_node_t *
axis2_math_svc_div (axis2_om_node_t *node)
{
    axis2_allocator_t *allocator = NULL;
    axis2_env_t *env = NULL;
    axis2_error_t *error = NULL;
    axis2_log_t *axis_log     = NULL;

    axis2_om_node_t *param1_node = NULL;
    axis2_om_node_t *param1_text_node = NULL;
    axis2_char_t *param1_str = NULL;
    long int param1 = 0;
    axis2_om_node_t *param2_node = NULL;
    axis2_om_node_t *param2_text_node = NULL;
    axis2_char_t *param2_str = NULL;
    long int param2 = 0;

    allocator = axis2_allocator_init (NULL);
    axis_log = axis2_log_create(allocator, NULL);
    error = axis2_error_create(allocator);
    env = axis2_env_create_with_error_log(allocator, error,  axis_log);
                
    
    if (!node)
    {
        printf("Math client request ERROR: input parameter NULL\n");
        return NULL;
    }

    param1_node = AXIS2_OM_NODE_GET_FIRST_CHILD(node, &env);
    if (!param1_node)
    {
        printf("Math service  ERROR: invalid XML in request\n");
        /** TODO: return a SOAP Fault here*/
        return node;
    }
    
    param1_text_node = AXIS2_OM_NODE_GET_FIRST_CHILD(param1_node, &env);
    if (!param1_text_node)
    {
        printf("Math service ERROR: invalid XML in request\n");
        /** TODO: return a SOAP Fault here*/
        return node;
    }
    
    if (AXIS2_OM_NODE_GET_NODE_TYPE(param1_text_node, &env) == AXIS2_OM_TEXT)
    {
        axis2_om_text_t *text = (axis2_om_text_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(param1_text_node, &env);
        if( text && AXIS2_OM_TEXT_GET_VALUE(text ,&env))
        {
            param1_str = AXIS2_OM_TEXT_GET_VALUE(text,&env);
        }
    }
    else
    {
        printf("Math service ERROR: invalid XML in request\n");
    }
	
    param2_node = AXIS2_OM_NODE_GET_NEXT_SIBLING(param1_node, &env);
    if (!param2_node)
    {
        printf("Math service  ERROR: invalid XML in request\n");
        /** TODO: return a SOAP Fault here*/
        return node;
    }
    
    param2_text_node = AXIS2_OM_NODE_GET_FIRST_CHILD(param2_node, &env);
    if (!param2_text_node)
    {
        printf("Math service ERROR: invalid XML in request\n");
        /** TODO: return a SOAP Fault here*/
        return node;
    }
    
    if (AXIS2_OM_NODE_GET_NODE_TYPE(param2_text_node, &env) == AXIS2_OM_TEXT)
    {
        axis2_om_text_t *text = (axis2_om_text_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(param2_text_node, &env);
        if( text && AXIS2_OM_TEXT_GET_VALUE(text ,&env))
        {
            param2_str = AXIS2_OM_TEXT_GET_VALUE(text,&env);
        }
    }
    else
    {
        printf("Math service ERROR: invalid XML in request\n");
    }
	
    if (param1_str && param2_str)
    {
        long int result = 0;
        axis2_char_t result_str[255];

        axis2_om_element_t *ele1 = NULL;
        axis2_om_node_t *node1 = NULL, *node2 = NULL;
        axis2_om_namespace_t *ns1 = NULL;
        axis2_om_text_t *text1 = NULL;
        
        param1 = strtol(param1_str, NULL, 10);
        param2 = strtol(param2_str, NULL, 10);
        result = param1 / param2;
        sprintf(result_str, "%ld", result);

        

        ns1 = axis2_om_namespace_create (&env,
                                   "http://axis2/test/namespace1",
                                   "ns1");
        ele1 = axis2_om_element_create (&env, NULL, "result", ns1, &node1);
        text1 = axis2_om_text_create (&env, node1, result_str, &node2);

        return node1;
    }
    
    printf("Math service ERROR: invalid parameters\n");
    /** TODO: return a SOAP Fault here*/
    return node;
}
