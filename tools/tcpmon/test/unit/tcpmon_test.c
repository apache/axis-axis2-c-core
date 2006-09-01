#include <CuTest.h>
#include <axis2_utils.h>
#include "tcpmon_test.h"
#include <stdlib.h>
#include <axis2_string.h>

#include <tcpmon_util.h>

void test_format_xml(CuTest *tc)
{
    axis2_env_t *env;
    axis2_allocator_t *allocator;
    axis2_char_t* input;
    axis2_char_t* actual;
    axis2_char_t* expected;

    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);

    input = (char*)AXIS2_STRDUP(
             "<input>check for one step</input>",
                                              env);
    actual = 
      (char*)tcpmon_util_format_as_xml(env, input);
    expected = "<input>\n"
               "\tcheck for one step\n" 
               "</input>\n";
    CuAssertStrEquals(tc, expected, actual);
    free (actual);
    free (input);


    input = (char*)AXIS2_STRDUP(
             "<input><tag2><another_tag with='attriutes'>check for one step</another_tag></tag2></input>",
                                              env);
    actual = 
      (char*)tcpmon_util_format_as_xml(env, input);
    expected = 
               "<input>\n"
               "\t<tag2>\n"
               "\t\t<another_tag with='attriutes'>\n"
               "\t\t\tcheck for one step\n"
               "\t\t</another_tag>\n"
               "\t</tag2>\n"
               "</input>\n";

    CuAssertStrEquals(tc, expected, actual);
    free (actual);
    free (input);

    input = (char*)AXIS2_STRDUP(
             "<?processing inc?><input><tag2><another_tag with='attriutes'>check for one step</another_tag></tag2></input>",
                                              env);
    actual = 
      (char*)tcpmon_util_format_as_xml(env, input);
    expected = "<?processing inc?>\n"
               "<input>\n"
               "\t<tag2>\n"
               "\t\t<another_tag with='attriutes'>\n"
               "\t\t\tcheck for one step\n"
               "\t\t</another_tag>\n"
               "\t</tag2>\n"
               "</input>\n";

    CuAssertStrEquals(tc, expected, actual);
    free (actual);
    free (input);

}

CuSuite* tcpmon_GetSuite()
{
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_format_xml);
    return suite;
}

