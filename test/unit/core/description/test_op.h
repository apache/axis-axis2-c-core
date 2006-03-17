#ifndef TEST_OP_H
#define TEST_OP_H

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

struct axis2_op;


void Testaxis2_op_set_remaining_phases_inflow(CuTest *tc);

#endif /* TEST_OP_H*/
