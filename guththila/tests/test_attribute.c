#include <guththila.h>
#include <check.h>
#include "test.h"

START_TEST (test_attribute)
{
	red = guththila_reader_create_for_file(environment, "resources/om/axis.xml");
    parser = guththila_create(environment, red);
	fail_if (red == NULL, "guththila reader failed");
	fail_if (parser == NULL, "guththila parser failed");
}
END_TEST


Suite *
guththila_attribute_suite (void)
{
  Suite *s = suite_create ("Guththila_attribute");

  /* Core test case */
  TCase *tc_core = tcase_create ("attribute");
  tcase_add_checked_fixture (tc_core, setup, teardown);
  tcase_add_test (tc_core, test_attribute);
  suite_add_tcase (s, tc_core);
  return s;
}
