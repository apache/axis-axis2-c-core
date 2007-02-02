#include <check.h>
#include <guththila.h>
#include "guththila_defines.h"

axis2_allocator_t *allocator;
guththila_reader_t *red;
axis2_env_t *environment;
guththila_t *parser;

void
setup (void)
{
allocator = axis2_allocator_init(NULL);
environment = axis2_env_create(allocator);
}

void
teardown (void)
{
 guththila_reader_free(environment, red);
 guththila_free(environment, parser);
 axis2_env_free(environment);
}

START_TEST (test_guththila)
{
	red = guththila_reader_create_for_file(environment, "resources/om/axis.xml");
    parser = guththila_create(environment, red);
	fail_if (red == NULL, "guththila reader failed");
	fail_if (parser == NULL, "guththila parser failed");
}
END_TEST

Suite *
guththila_suite (void)
{
  Suite *s = suite_create ("Guththila");

  /* Core test case */
  TCase *tc_core = tcase_create ("Core");
  tcase_add_checked_fixture (tc_core, setup, teardown);
  tcase_add_test (tc_core, test_guththila);
  suite_add_tcase (s, tc_core);
  return s;
}


int
main (void)
{
  int number_failed;
  Suite *s = guththila_suite ();
  SRunner *sr = srunner_create (s);
  srunner_run_all (sr, CK_NORMAL);
  number_failed = srunner_ntests_failed (sr);
  srunner_free (sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
