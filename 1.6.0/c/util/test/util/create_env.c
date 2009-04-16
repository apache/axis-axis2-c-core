#include "create_env.h"

axutil_env_t * create_environment()
{
    axutil_allocator_t *allocator = NULL;
    axutil_log_t *log = NULL;
    axutil_error_t *error = NULL;
    axutil_env_t *env = NULL;
    allocator = axutil_allocator_init(NULL);
    log = axutil_log_create(allocator, NULL, NULL);

    error = axutil_error_create(allocator);
    env = axutil_env_create_with_error_log(allocator, error, log);
    return env;
}


