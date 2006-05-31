#ifndef _TEST_LOG_H_
#define _TEST_LOG_H_

#include <axis2_env.h>

void run_test_log();
const axis2_env_t *create_env_with_error_log();
void test_axis2_log_write(const axis2_env_t *env);
void test_axis2_log_debug(const axis2_env_t *env);
void test_axis2_log_debug_off(const axis2_env_t *env);
void test_axis2_log_info(const axis2_env_t *env);
void test_axis2_log_info_off(const axis2_env_t *env);
void test_axis2_log_warning(const axis2_env_t *env);
void test_axis2_log_warning_off(const axis2_env_t *env);
void test_axis2_log_error(const axis2_env_t *env);
void test_axis2_log_critical(const axis2_env_t *env);

#endif /* _TEST_LOG_H_ */

