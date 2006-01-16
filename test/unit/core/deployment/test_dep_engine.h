#ifndef TEST_DEP_ENGINE_H
#define TEST_DEP_ENGINE_H

#include <string.h>
#include <stdio.h>
#include <CuTest.h>
#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_hash.h>
#include <axis2_string.h>
#include <axis2_dep_engine.h>

void Testaxis2_dep_engine_free(CuTest *tc);
void Testaxis2_dep_engine_create(CuTest *tc);
void Testaxis2_dep_engine_create_with_repos_name(CuTest *tc);
void Testaxis2_dep_engine_load(CuTest *tc);

#endif /* TEST_DEP_ENGINE_H*/
