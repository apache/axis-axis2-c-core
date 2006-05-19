#ifndef _TEST_LOG_H_
#define _TEST_LOG_H_

#include <axis2_env.h>

void run_test_log();
axis2_env_t *create_env_with_error_log();
void test_axis2_log_write(axis2_env_t *env);
void test_axis2_log_debug(axis2_env_t *env);
void test_axis2_log_debug_off(axis2_env_t *env);
void test_axis2_log_info(axis2_env_t *env);
void test_axis2_log_info_off(axis2_env_t *env);
void test_axis2_log_warning(axis2_env_t *env);
void test_axis2_log_warning_off(axis2_env_t *env);
void test_axis2_log_error(axis2_env_t *env);
void test_axis2_log_critical(axis2_env_t *env);

#endif /* _TEST_LOG_H_ */

