#ifndef TEST_SVC_H
#define TEST_SVC_H

#undef NULL
#include <CuTest.h>
#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_hash.h>
#include <axis2_string.h>
#include <axis2_op.h>
#include <axis2_module_desc.h>
#include <axis2_op.h>
#include <axis2_flow.h>
#include <axis2_wsdl_op.h>
#include <axis2_phase.h>
#include <axis2_svc.h>

struct axis2_svc;
struct axis2_op;
struct axis2_module_desc;
struct axis2_wsdl_op;
struct axis2_conf;
struct axis2_flow;
struct axis2_phase;

void Testaxis2_svc_add_module_ops(CuTest *tc);
void Testaxis2_svc_create_with_qname(CuTest *tc);

#endif /* TEST_SVC_H*/
