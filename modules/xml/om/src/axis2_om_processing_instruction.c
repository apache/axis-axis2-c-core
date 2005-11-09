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

#include <axis2_om_processing_instruction.h>
#include <axis2_string.h>

axis2_status_t AXIS2_CALL
axis2_om_processing_instruction_free (axis2_om_processing_instruction_t *om_pi,
                                      axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_om_processing_instruction_set_value(axis2_om_processing_instruction_t *om_pi,
                                          axis2_env_t **env,
                                          const axis2_char_t *value);
axis2_status_t AXIS2_CALL
axis2_om_processing_instruction_set_target(axis2_om_processing_instruction_t *om_pi,
                                           axis2_env_t **env,
                                           const axis2_char_t *target);                                                                                 

axis2_char_t* AXIS2_CALL
axis2_om_processing_instruction_get_value(axis2_om_processing_instruction_t *om_pi,
                                          axis2_env_t **env);
axis2_char_t*  AXIS2_CALL
axis2_om_processing_instruction_get_target(axis2_om_processing_instruction_t *om_pi,
                                           axis2_env_t **env);

/*********************** axis2_om_processing_instruction_impl_t struct ********/
typedef struct axis2_om_processing_instruction_impl
{

    axis2_om_processing_instruction_t om_pi;    
    /** processing instruction  target */
    axis2_char_t *target;
    /** processing instruction  value */
    axis2_char_t *value;
}axis2_om_processing_instruction_impl_t;

/******************************************************************************/

#define AXIS2_INTF_TO_IMPL(om_processing_ins) ((axis2_om_processing_instruction_impl_t*)om_processing_ins)


/******************************************************************************/
axis2_om_processing_instruction_t *AXIS2_CALL 
axis2_om_processing_instruction_create (axis2_env_t **env,
                                        axis2_om_node_t * parent,
                                        const axis2_char_t * target,
                                        const axis2_char_t * value,
                                        axis2_om_node_t ** node)
{
    axis2_om_processing_instruction_impl_t *processing_instruction = NULL;
    AXIS2_ENV_CHECK(env,NULL);
    
    if (!node || !target || !value)
        AXIS2_ERROR_SET((*env)->error,AXIS2_ERROR_INVALID_NULL_PARAM,NULL);

    *node = axis2_om_node_create (env);
    
    if (!*node)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);

    processing_instruction = (axis2_om_processing_instruction_impl_t *) AXIS2_MALLOC (
                              (*env)->allocator,sizeof(axis2_om_processing_instruction_impl_t));
                              
    if (!processing_instruction)
    {
        AXIS2_OM_NODE_FREE (*node, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    processing_instruction->value = NULL;

    if (value)
    {
        processing_instruction->value = (axis2_char_t*)AXIS2_STRDUP(value,env);
        if (!processing_instruction->value)
        {
            AXIS2_FREE ((*env)->allocator, processing_instruction);
            AXIS2_FREE ((*env)->allocator , *node);
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        }
    }

    processing_instruction->target = NULL;

    if (target)
    {
        processing_instruction->target = (axis2_char_t*) AXIS2_STRDUP(target,env);
        if (!processing_instruction->target)
        {
            AXIS2_FREE ((*env)->allocator, processing_instruction->value);
            AXIS2_FREE ((*env)->allocator, processing_instruction);
            AXIS2_FREE ((*env)->allocator, *node);
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        }
    }
    AXIS2_OM_NODE_SET_DATA_ELEMENT(*node, env, processing_instruction);
    AXIS2_OM_NODE_SET_NODE_TYPE(*node, env, AXIS2_OM_PROCESSING_INSTRUCTION);
    if (parent)
    {
        AXIS2_OM_NODE_SET_PARENT((*node), env, parent);
        AXIS2_OM_NODE_ADD_CHILD((*node), env, parent); 
    }

    /* operations */
    processing_instruction->om_pi.ops = NULL;
    processing_instruction->om_pi.ops =
        (axis2_om_processing_instruction_ops_t *) AXIS2_MALLOC (
        (*env)->allocator, sizeof(axis2_om_processing_instruction_ops_t));
        
    if (!processing_instruction->om_pi.ops)
    {
        AXIS2_FREE ((*env)->allocator,  processing_instruction->value);
        AXIS2_FREE ((*env)->allocator,  processing_instruction->target);
        AXIS2_FREE ((*env)->allocator,  processing_instruction);
        AXIS2_FREE ((*env)->allocator,  *node);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }

    processing_instruction->om_pi.ops->free = 
            axis2_om_processing_instruction_free;
    processing_instruction->om_pi.ops->set_target = 
            axis2_om_processing_instruction_set_target;
    processing_instruction->om_pi.ops->get_target =
            axis2_om_processing_instruction_get_target;
    processing_instruction->om_pi.ops->get_value =
            axis2_om_processing_instruction_get_value;
    processing_instruction->om_pi.ops->set_value = 
            axis2_om_processing_instruction_set_value;                     
    
    return &(processing_instruction->om_pi);
}

axis2_status_t AXIS2_CALL
axis2_om_processing_instruction_free (axis2_om_processing_instruction_t *om_pi,
                                      axis2_env_t **env)
{
        AXIS2_FUNC_PARAM_CHECK(om_pi, env, AXIS2_FAILURE);
        if (AXIS2_INTF_TO_IMPL(om_pi)->value)
        {
            AXIS2_FREE ((*env)->allocator, AXIS2_INTF_TO_IMPL(om_pi)->value);
        }

        if (AXIS2_INTF_TO_IMPL(om_pi)->target)
        {
            AXIS2_FREE ((*env)->allocator, AXIS2_INTF_TO_IMPL(om_pi)->target);
        }

        AXIS2_FREE ((*env)->allocator,om_pi);
        return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_om_processing_instruction_set_value(axis2_om_processing_instruction_t *om_pi,
                                          axis2_env_t **env,
                                          const axis2_char_t *value)
{
    AXIS2_FUNC_PARAM_CHECK(om_pi, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_pi)->value = (axis2_char_t*)AXIS2_STRDUP(value,env);
    return AXIS2_SUCCESS;
}

                                          
axis2_status_t AXIS2_CALL
axis2_om_processing_instruction_set_target(axis2_om_processing_instruction_t *om_pi,
                                           axis2_env_t **env,
                                           const axis2_char_t *target)
{
    AXIS2_FUNC_PARAM_CHECK(om_pi, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, target, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_pi)->target = (axis2_char_t*)AXIS2_STRDUP(target,env);
    return AXIS2_SUCCESS;

}                                                                                                                            

axis2_char_t* AXIS2_CALL
axis2_om_processing_instruction_get_value(axis2_om_processing_instruction_t *om_pi,
                                          axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(om_pi, env, NULL);
    return AXIS2_INTF_TO_IMPL(om_pi)->value;
}                                          
axis2_char_t*  AXIS2_CALL
axis2_om_processing_instruction_get_target(axis2_om_processing_instruction_t *om_pi,
                                           axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(om_pi, env, NULL);
    return AXIS2_INTF_TO_IMPL(om_pi)->target;
}                                           