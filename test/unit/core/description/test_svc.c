#include "test_svc.h"

struct axis2_module_desc *create_module_desc(axis2_env_t **env);
void add_handlers_to_flow(struct axis2_flow *flow, axis2_env_t **env);    
    
void Testaxis2_svc_add_module_ops(CuTest *tc)
{
    struct axis2_module_desc *module_desc = NULL;
    struct axis2_conf *conf = NULL;
    struct axis2_svc *svc = NULL;
    axis2_status_t expected = AXIS2_SUCCESS;
    axis2_status_t actual = AXIS2_FAILURE;
    struct axis2_flow *inflow = NULL;
         
    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_env_t *env = axis2_env_create (allocator);    

    conf = axis2_conf_create(&env);
    module_desc = create_module_desc(&env);
      
    inflow = axis2_flow_create(&env);
    add_handlers_to_flow(inflow, &env);
    svc = axis2_svc_create(&env);
    axis2_svc_set_inflow(svc, &env, inflow);
    actual = axis2_svc_add_module_ops(svc, &env, module_desc, conf);
    
    CuAssertIntEquals(tc, expected, actual);
    
}

struct axis2_module_desc *create_module_desc(axis2_env_t **env) 
{
    struct axis2_op *op = NULL;
    axis2_qname_t *op_qname = NULL;
    struct axis2_module_desc *module_desc = NULL;
    
    module_desc = axis2_module_desc_create(env);
    
    op_qname = axis2_qname_create(env, "op_name1", NULL, NULL);
    op = axis2_op_create_with_name(env, op_qname);
    int status = AXIS2_MODULE_DESC_ADD_OP(module_desc, env, op);
    
    op_qname = axis2_qname_create(env, "op_name2", NULL, NULL);
    op = axis2_op_create_with_name(env, op_qname);
    AXIS2_MODULE_DESC_ADD_OP(module_desc, env, op);
    
    return module_desc;
    
}

void add_handlers_to_flow(struct axis2_flow *flow, axis2_env_t **env)
{
    struct axis2_handler_desc *handler_desc = NULL;
    struct axis2_handler *handler = NULL;
    axis2_qname_t *qname = NULL;
    struct axis2_phase_rule *rule = NULL;
        
    rule = axis2_phase_rule_create(env, AXIS2_PHASE_POLICY_DETERMINATION);
    axis2_phase_rule_set_before(rule, env, "before");
    axis2_phase_rule_set_after(rule, env, "after");
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
