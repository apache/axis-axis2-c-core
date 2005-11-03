#ifndef UTIL_STREAM_TEST_H
#define UTIL_STREAM_TEST_H

#include <string.h>
#include <stdlib.h>
#include <CuTest.h>
#include <axis2_environment.h>
#include <axis2_allocator.h>
#include <axis2_error_default.h>
#include <axis2_stream_default.h>

void Testaxis2_stream_read(CuTest *tc);

void Testaxis2_stream_write(CuTest *tc);

#endif /* UTIL_STREAM_TEST_H*/
