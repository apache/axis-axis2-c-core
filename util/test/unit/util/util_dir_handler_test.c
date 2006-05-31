#include <util_dir_handler_test.h>
#include <axis2_file.h>
#include <axis2_string.h>

void Testaxis2_dir_handler_list_dir(CuTest *tc)
{
	axis2_char_t *pathname = NULL;
    axis2_array_list_t *file_list = NULL;
    int size = 0;
    axis2_char_t *expected = NULL;
    axis2_char_t *actual = NULL;
    int i = 0;
    axis2_char_t *axis2c_home = NULL;
    
    printf("******************************************************************\n");
    printf("testing axis2_dir_handler_list_services_or_modules_in_dir  method \n");
    printf("******************************************************************\n");

    axis2_allocator_t *allocator = axis2_allocator_init (NULL);
    const axis2_env_t *env = axis2_env_create(allocator);

    expected = AXIS2_STRDUP("libaxis2_util.so", env);
    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    pathname = AXIS2_STRACAT (axis2c_home, "/lib", env);
    file_list = AXIS2_DIR_HANDLER_LIST_SERVICES_OR_MODULES_IN_DIR(env, pathname);
    size = AXIS2_ARRAY_LIST_SIZE(file_list, env);
    for (i = 0; i < size; i++) 
    {
        axis2_file_t *arch_file = NULL;
        arch_file = (axis2_file_t *) AXIS2_ARRAY_LIST_GET(file_list, env, i);
        if(!arch_file)
        {
            printf("arch file is null\n");
            continue;
        }
        if(0 == AXIS2_STRCMP(AXIS2_FILE_GET_NAME(arch_file, env), expected))
        {
            actual = AXIS2_FILE_GET_NAME(arch_file, env);
        }   
    }
    
    CuAssertStrEquals(tc, expected, actual);
    AXIS2_FREE(env->allocator, expected);
    AXIS2_FREE(env->allocator, pathname);
}
