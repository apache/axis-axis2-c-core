#ifndef TEST_FLOW_H
#define TEST_FLOW_H

#include <string.h>
#include <stdio.h>
#include <CuTest.h>
#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_hash.h>
#include <axis2_string.h>

void Testaxis2_flow_add_handler(CuTest *tc);
void Testaxis2_flow_free(CuTest *tc);

#endif /* TEST_FLOW_H*/
