#ifndef TEST_RESOLVER_H
#define TEST_RESOLVER_H

#include <string.h>
#include <stdio.h>
#include <CuTest.h>
#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_hash.h>
#include <axis2_string.h>
#include <axis2_phase_resolver.h>
#include <axis2_module_desc.h>
#include <axis2_op.h>
#include <axis2_flow.h>

struct axis2_flow;
struct axis2_op;
struct axis2_svc;
struct axis2_module_desc;


void axis2_phase_resolver_engage_module_to_svc(CuTest *tc);
void Testaxis2_phase_resolver_engage_module_to_op(CuTest *tc);

#endif /* TEST_RESOLVER_H*/
