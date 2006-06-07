#include "util_test.h"
#include <string.h>
#include <stdio.h>
#include <axis2_allocator.h>
#include <axis2_env.h>

CuSuite* axis2_utilGetSuite() 
{
    CuSuite* suite = CuSuiteNew();
   
    /* Samisa - need to remove this as we no longer have samples in the main build
    SUITE_ADD_TEST(suite, Testaxis2_class_loader_create_dll); */
    SUITE_ADD_TEST(suite, Testaxis2_stream_write);
    SUITE_ADD_TEST(suite, Testaxis2_log_write);
    SUITE_ADD_TEST(suite, Testaxis2_hash_get);
    SUITE_ADD_TEST(suite, Testaxis2_hash_while);
    SUITE_ADD_TEST(suite, Testaxis2_array_list_get);
    SUITE_ADD_TEST(suite, Testaxis2_linked_list_get);
    SUITE_ADD_TEST(suite, Testaxis2_strndup);
    SUITE_ADD_TEST(suite, Testaxis2_strcat);
    SUITE_ADD_TEST(suite, Testaxis2_url_parse_string);
    SUITE_ADD_TEST(suite, Testaxis2_uri_parse_string);
    SUITE_ADD_TEST(suite, Testaxis2_uri_parse_relative);
    /* Samisa - need to remove this as we run make check before make install
    SUITE_ADD_TEST(suite, Testaxis2_dir_handler_list_dir); */
    return suite;
}
