#ifndef TEST_LOG_H
#define TEST_LOG_H

#include <axis2_env.h>
#include <axis2_thread.h>


static void init_func(void);
static void thread_init(axis2_env_t *env);
void * AXIS2_CALL test_function(axis2_thread_t *td,void *param);
void test_axis2_thread_create(axis2_env_t *env);
void * AXIS2_CALL test_function2(axis2_thread_t *td,void *param);
void test_axis2_thread_detach(axis2_env_t *env);
void test_axis2_thread_detach2(axis2_env_t *env);
static void check_locks();
/*call this method from main*/
void run_test_thread(axis2_env_t *env);

#endif

