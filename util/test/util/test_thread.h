#ifndef TEST_LOG_H
#define TEST_LOG_H

#include <axis2_env.h>
#include <axis2_thread.h>


void init_func(void);
void thread_init(const axis2_env_t *env);
void * AXIS2_CALL test_function(axis2_thread_t *td,void *param);
void test_axis2_thread_create(const axis2_env_t *env);
void * AXIS2_CALL test_function2(axis2_thread_t *td,void *param);
void test_axis2_thread_detach(const axis2_env_t *env);
void test_axis2_thread_detach2(const axis2_env_t *env);
void check_locks();
/*call this method from main*/
void run_test_thread(const axis2_env_t *env);

#endif

