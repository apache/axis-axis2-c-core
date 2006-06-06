#ifndef UTIL_TEST_H
#define UTIL_TEST_H

#include <CuTest.h>
#include "util_stream_test.h"
#include "util_log_test.h"
#include "util_hash_test.h"
#include "util_array_list_test.h"
#include "util_linked_list_test.h"
#include "util_error_test.h"
#include "util_dir_handler_test.h"
#include "util_class_loader_test.h"
#include "util_url_test.h"
#include "util_string_test.h"

CuSuite* axis2_utilGetSuite();

#endif /* UTIL_TEST_H */
