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

#include <stdio.h>
#include "test_op.h"

void add_handlers_to_flow(struct axis2_flow *flow, const axis2_env_t *env);
axis2_array_list_t *get_op_op_in_phases(const axis2_env_t *env);

void Testaxis2_op_set_remaining_phases_inflow(CuTest *tc)
{
    axis2_array_list_t *op_in_phases = NULL;
    axis2_status_t expected = AXIS2_SUCCESS;
    axis2_status_t actual = AXIS2_FAILURE;
    axis2_op_t *op = NULL;
    
    printf("*****************************************************\n");
    printf("testing axis2_op_set_remaining_phases_inflow  method \n");
    printf("*****************************************************\n");

    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    const axis2_env_t *env = axis2_env_create (allocator);

    op = axis2_op_create(env);
    op_in_phases = get_op_op_in_phases(env);  
    actual = AXIS2_OP_SET_REMAINING_PHASES_INFLOW(op, env, op_in_phases);
    AXIS2_OP_FREE(op, env);
    
    CuAssertIntEquals(tc, expected, actual);
}

/** helper method. This is the requirement method from phases_info's 
  * axis2_get_op_in_phases method
  */
axis2_array_list_t *get_op_op_in_phases(const axis2_env_t *env)
{
    struct axis2_phase *phase = NULL; 
    axis2_array_list_t *op_in_phases = NULL; 
    
    op_in_phases = axis2_array_list_create(env, 20);
    
    phase = axis2_phase_create(env, AXIS2_PHASE_POLICY_DETERMINATION);
    AXIS2_ARRAY_LIST_ADD(op_in_phases, env, phase);
    
    phase = axis2_phase_create(env, AXIS2_PHASE_TRANSPORTIN);   
    AXIS2_ARRAY_LIST_ADD(op_in_phases, env, phase);
    
    phase = axis2_phase_create(env, AXIS2_PHASE_PRE_DISPATCH);   
    AXIS2_ARRAY_LIST_ADD(op_in_phases, env, phase);
    
    phase = axis2_phase_create(env, AXIS2_PHASE_DISPATCH);  
    AXIS2_ARRAY_LIST_ADD(op_in_phases, env, phase);
    
    return op_in_phases;

}
