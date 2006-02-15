#include "test_resolver.h"
#include <axis2_phase_resolver.h>
#include <axis2_module_desc.h>
#include <axis2_op.h>
#include <axis2_flow.h>

void add_handlers_to_flow(struct axis2_flow *flow, axis2_env_t **env);
axis2_array_list_t *get_op_in_phases(axis2_env_t **env);

void axis2_phase_resolver_engage_module_to_svc(CuTest *tc)
{
}

void Testaxis2_phase_resolver_free(CuTest *tc)
{
    axis2_status_t actual = AXIS2_FAILURE;
    axis2_status_t expected = AXIS2_TRUE;
    axis2_phase_resolver_t *resolver = NULL;
    axis2_allocator_t *allocator = NULL;
    axis2_log_t *log = NULL;
    axis2_env_t *env = NULL;
    axis2_error_t *error = NULL;

    printf("******************************************\n");
    printf("testing axis2_phase_resolver_free  method \n");
    printf("******************************************\n");

    allocator = axis2_allocator_init (NULL);
    error = axis2_error_create(allocator);
    log = axis2_log_create(allocator, NULL, "/dev/stderr");
    env = axis2_env_create_with_error_log(allocator, error, log);
    env->log->level = AXIS2_LOG_LEVEL_TRACE;
    axis2_error_init();

    resolver = axis2_phase_resolver_create(&env); 
    actual = axis2_phase_resolver_free(resolver,  &env);
    
    CuAssertIntEquals(tc, expected, actual);

}

void Testaxis2_phase_resolver_engage_module_to_op(CuTest *tc)
{
    struct axis2_phase_resolver *resolver = NULL;
    axis2_array_list_t *op_in_phases = NULL;
    struct axis2_module_desc *module_desc = NULL;
    struct axis2_flow *flow = NULL;
    axis2_status_t expected = AXIS2_SUCCESS;
    axis2_status_t actual = AXIS2_FAILURE;
    axis2_allocator_t *allocator = NULL;
    axis2_log_t *log = NULL;
    axis2_env_t *env = NULL;
    axis2_error_t *error = NULL;
    
    printf("*********************************************************\n");
    printf("testing axis2_phase_resolver_engage_module_to_op  method \n");
    printf("*********************************************************\n");

    allocator = axis2_allocator_init (NULL);
    error = axis2_error_create(allocator);
    log = axis2_log_create(allocator, NULL, "/dev/stderr");
    env = axis2_env_create_with_error_log(allocator, error, log);
    env->log->level = AXIS2_LOG_LEVEL_TRACE;
    axis2_error_init();

    axis2_op_t *optr = axis2_op_create(&env);
    op_in_phases = get_op_in_phases(&env);  
    AXIS2_OP_SET_REMAINING_PHASES_INFLOW(optr, &env, op_in_phases);
    flow = axis2_flow_create(&env); 
    add_handlers_to_flow(flow, &env);
    module_desc = axis2_module_desc_create(&env);
    AXIS2_MODULE_DESC_SET_INFLOW(module_desc, &env, flow);
    resolver = axis2_phase_resolver_create(&env);
    actual = AXIS2_PHASE_RESOLVER_ENGAGE_MODULE_TO_OP(resolver, &env, optr,
        module_desc);

    AXIS2_FLOW_FREE(flow, &env);
    AXIS2_OP_FREE(optr, &env);
    AXIS2_MODULE_DESC_FREE(module_desc, &env);
    AXIS2_PHASE_RESOLVER_FREE(resolver, &env);
    CuAssertIntEquals(tc, expected, actual);
    
}

void add_handlers_to_flow(struct axis2_flow *flow, axis2_env_t **env)
{
    struct axis2_handler_desc *handler_desc = NULL;
    struct axis2_handler *handler = NULL;
    axis2_qname_t *qname = NULL;
    struct axis2_phase_rule *rule = NULL;
        
    rule = axis2_phase_rule_create(env, AXIS2_PHASE_POLICY_DETERMINATION);
    AXIS2_PHASE_RULE_SET_BEFORE(rule, env, "before");
    AXIS2_PHASE_RULE_SET_AFTER(rule, env, "after");
    qname = axis2_qname_create(env, "handler1", NULL, NULL);
    handler_desc = axis2_handler_desc_create_with_qname(env, qname);
    AXIS2_HANDLER_DESC_SET_RULES(handler_desc, env, rule);
    handler = axis2_handler_create(env);
    AXIS2_HANDLER_INIT(handler, env, handler_desc);
    AXIS2_HANDLER_DESC_SET_HANDLER(handler_desc, env, handler);
    AXIS2_FLOW_ADD_HANDLER(flow, env, handler_desc);
    
    rule = axis2_phase_rule_create(env, AXIS2_PHASE_POLICY_DETERMINATION); 
    axis2_phase_rule_set_before(rule, env, "before");
    axis2_phase_rule_set_after(rule, env, "after");
    qname = axis2_qname_create(env, "handler2", NULL, NULL);
    handler_desc = axis2_handler_desc_create_with_qname(env, qname);
    AXIS2_HANDLER_DESC_SET_RULES(handler_desc, env, rule);
    handler = axis2_handler_create(env);
    AXIS2_HANDLER_INIT(handler, env, handler_desc);
    AXIS2_HANDLER_DESC_SET_HANDLER(handler_desc, env, handler);
    AXIS2_FLOW_ADD_HANDLER(flow, env, handler_desc);
    
    rule = axis2_phase_rule_create(env, AXIS2_PHASE_POLICY_DETERMINATION);
    axis2_phase_rule_set_before(rule, env, "before");
    axis2_phase_rule_set_after(rule, env, "after");
    qname = axis2_qname_create(env, "handler3", NULL, NULL);
    handler_desc = axis2_handler_desc_create_with_qname(env, qname);
    AXIS2_HANDLER_DESC_SET_RULES(handler_desc, env, rule);
    handler = axis2_handler_create(env);
    AXIS2_HANDLER_INIT(handler, env, handler_desc);
    AXIS2_HANDLER_DESC_SET_HANDLER(handler_desc, env, handler);
    AXIS2_FLOW_ADD_HANDLER(flow, env, handler_desc);    
}    
           
/** helper method. This is the requirement method from phases_info's 
  * axis2_get_op_in_phases method
  */
axis2_array_list_t *get_op_in_phases(axis2_env_t **env)
{
    struct axis2_phase *phase = NULL; 
    axis2_array_list_t *op_in_phases = NULL; 
    
    op_in_phases = axis2_array_list_create(env, 10);
    
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
