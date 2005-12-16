#include "test_op.h"

void add_handlers_to_flow(struct axis2_flow *flow, axis2_env_t **env);
axis2_array_list_t *get_op_op_in_phases(axis2_env_t **env);

void Testaxis2_op_set_remaining_phases_inflow(CuTest *tc)
{
    axis2_array_list_t *op_in_phases = NULL;
    axis2_status_t expected = AXIS2_SUCCESS;
    axis2_status_t actual = AXIS2_FAILURE;
    
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_env_t *env = axis2_env_create (allocator);

    struct axis2_op *op = axis2_op_create(&env);
    op_in_phases = get_op_op_in_phases(&env);  
    actual = AXIS2_OP_SET_REMAINING_PHASES_INFLOW(op, &env, op_in_phases);
    
    CuAssertIntEquals(tc, expected, actual);
}

/** helper method. This is the requirement method from phases_info's 
  * axis2_get_op_in_phases method
  */
axis2_array_list_t *get_op_op_in_phases(axis2_env_t **env)
{
    struct axis2_phase *phase = NULL; 
    axis2_array_list_t *op_in_phases = NULL; 
    
    op_in_phases = axis2_array_list_create(env, 0);
    
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
