#include "test_phase_holder.h"
#include <axis2_array_list.h>
#include <axis2_phase_holder.h>
#include <axis2_phase_resolver.h>

/** helper method
  */
axis2_array_list_t *get_phases(axis2_env_t **env);

void Testaxis2_phase_holder_free(CuTest *tc)
{
    axis2_status_t actual = AXIS2_FAILURE;
    axis2_status_t expected = AXIS2_TRUE;
    axis2_phase_holder_t *phase_holder = NULL;
    axis2_array_list_t *phases = NULL;

    printf("****************************************\n");
    printf("testing axis2_phase_holder_free  method \n");
    printf("****************************************\n");

    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_env_t *env = axis2_env_create (allocator);

    phases = get_phases(&env);
    phase_holder = axis2_phase_holder_create_with_phases(&env, phases); 
    actual = axis2_phase_holder_free(phase_holder, &env);
    
    CuAssertIntEquals(tc, expected, actual);

}

/** helper method
  */
axis2_array_list_t *get_phases(axis2_env_t **env)
{
    struct axis2_phase *phase = NULL; 
    axis2_array_list_t *phases = NULL; 
    
    phases = axis2_array_list_create(env, 10);
    
    phase = axis2_phase_create(env, AXIS2_PHASE_POLICY_DETERMINATION);
    AXIS2_ARRAY_LIST_ADD(phases, env, phase);
    
    phase = axis2_phase_create(env, AXIS2_PHASE_TRANSPORTIN);   
    AXIS2_ARRAY_LIST_ADD(phases, env, phase);
    
    phase = axis2_phase_create(env, AXIS2_PHASE_PRE_DISPATCH);   
    AXIS2_ARRAY_LIST_ADD(phases, env, phase);
    
    phase = axis2_phase_create(env, AXIS2_PHASE_DISPATCH);  
    AXIS2_ARRAY_LIST_ADD(phases, env, phase);
    
    return phases;

}
