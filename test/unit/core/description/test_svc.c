#include "test_svc.h"

void Testaxis2_svc_add_module_ops(CuTest *tc)
{
    

}

struct axis2_module_desc *create_module_desc(axis2_env_t **env) 
{
    struct axis2_op *op = NULL;
    struct axis2_wsdl_op *wsdl_op = NULL;
    axis2_qname_t *op_qname = NULL;
    
    wsdl_op = axis2_wsdl_op_create(env);
    op_qname = axis2_qname_create(env, "op_name", NULL, NULL);
    op = axis2_op_create_with_name(env, op_qname);
}
