#ifndef TEST_RESOLVER_H
#define TEST_RESOLVER_H

#undef NULL
#include <CuTest.h>

struct axis2_flow;
struct axis2_op;
struct axis2_svc;
struct axis2_module_desc;


void axis2_phase_resolver_engage_module_to_svc(CuTest *tc);
void Testaxis2_phase_resolver_free(CuTest *tc);
void Testaxis2_phase_resolver_engage_module_to_op(CuTest *tc);

#endif /* TEST_RESOLVER_H*/
