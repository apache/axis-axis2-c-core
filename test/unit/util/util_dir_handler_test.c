#include <util_dir_handler_test.h>
#include <axis2_string.h>

void Testaxis2_dir_handler_list_dir(CuTest *tc)
{
    puts("testing axis2_dir_handler_list_dir\n");

    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    axis2_env_t *env = axis2_env_create(allocator);

	axis2_char_t *pathname = NULL;
    axis2_array_list_t *file_list = NULL;
    int size = 0;
    axis2_char_t *expected = NULL;
    axis2_char_t *actual = NULL;
    int i = 0;
    axis2_char_t *axis2c_home = NULL;
    
    expected = AXIS2_STRDUP("libaxis2_util.so", &env);
    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    pathname = AXIS2_STRACAT (axis2c_home, "/lib", &env);
    file_list = axis2_dir_handler_list_dir(&env, pathname);
    size = AXIS2_ARRAY_LIST_SIZE(file_list, &env);
    
    for (i = 0; i < size; i++) 
    {
        axis2_file_t *arch_file = NULL;
        arch_file = (axis2_file_t *) AXIS2_ARRAY_LIST_GET(file_list, &env, i);
        if(0 == AXIS2_STRCMP(arch_file->name, expected))
        {
            actual = arch_file->name;
        }   
    }
    
    CuAssertStrEquals(tc, expected, actual);
    AXIS2_FREE(env->allocator, expected);
    AXIS2_FREE(env->allocator, pathname);
    AXIS2_FREE(env->allocator, axis2c_home);
}
