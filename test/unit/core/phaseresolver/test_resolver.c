#include "test_resolver.h"

void Testaxis2_phase_resolver_engage_module_to_operation(CuTest *tc)
{
    struct axis2_phase_resolver *resolver = NULL;
    axis2_array_list_t *operation_inphases = NULL;
    struct axis2_module_desc module_desc = NULL;
    struct axis2_flow *flow = NULL;
    
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_env_t *environment = axis2_env_create (allocator);

    struct axis2_operation *optr = axis2_operation_create(env);
    operation_inphases = get_operation_inphases(env);  
    AXIS2_OPERATION_SET_REMAINING_PHASES_INFLOW(optr, env, operation_inphases);
    
    flow = axis2_flow_create(env); 
    add_handlers_to_flow(flow, env);
    module_desc = axis2_module_desc_create(env);
    AXIS2_MODULE_DESC_SET_INFLOW(module_desc, env, flow);
    resolver = axis2_phase_resolver_create(env);
    
    /*expected = (a *) axis2_hash_get (ht, key1, AXIS2_HASH_KEY_STRING);
    CuAssertStrEquals(tc, expected->value, actual->value);*/
}

void add_handlers_to_flow(struct axis2_flow *flow, env)
{
    struct axis2_handler *handler = NULL;
        
    handler = axis2_handler_create_with_name(env, "handler1");
    AXIS2_FLOW_ADD_HANDLER(flow, env, handler);
    
    handler = axis2_handler_create_with_name(env, "handler2");
    AXIS2_FLOW_ADD_HANDLER(flow, env, handler);
    
    handler = axis2_handler_create_with_name(env, "handler3");
    AXIS2_FLOW_ADD_HANDLER(flow, env, handler);    
}    
           
/** helper method. This is the requirement method from phases_info's 
  * axis2_get_operation_inphases method
  */
axis2_array_list get_operation_inphases(axis2_env_t **env)
{
    struct axis2_phase *phase = NULL;
    int i = 0; 
    axis2_array_list_t *operation_inphases = NULL; 
    
    operation_inphases = axis2_array_list_create(env, 0);
    
    phase = axis2_phase_create(env, AXIS2_PHASE_POLICY_DETERMINATION);
    AXIS2_ARRAY_LIST_ADD(operation_inphases, env, phase);
    
    phase = axis2_phase_create(env, AXIS2_PHASE_TRANSPORTIN);   
    AXIS2_ARRAY_LIST_ADD(operation_inphases, env, phase);
    
    phase = axis2_phase_create(env, AXIS2_PHASE_PRE_DISPATCH);   
    AXIS2_ARRAY_LIST_ADD(operation_inphases, env, phase);
    
    phase = axis2_phase_create(env, AXIS2_PHASE_DISPATCH);  
    AXIS2_ARRAY_LIST_ADD(operation_inphases, env, phase);
    
    phase = axis2_phase_create(env, AXIS2_PHASE_POLICY_DETERMINATIONzzzz);   
    AXIS2_ARRAY_LIST_ADD(operation_inphases, env, phase);
    
    return operation_inphases;

}
