#include <stdio.h>
#include <rampart_util.h>
#include <rampart_crypto_util.h>
#include <axis2_util.h>
#include <axis2_base64.h>
#include <axis2_date_time.h>

axis2_env_t *test_init()
{
   axis2_allocator_t *allocator = axis2_allocator_init (NULL);
   axis2_error_t *error = (axis2_error_t*)axis2_error_create (allocator);
   axis2_env_t *env = axis2_env_create_with_error(allocator, error);
   return env;
}

int main()
{
   axis2_char_t* date_str1="2007-08-21T12:14:42Z";
   axis2_char_t* date_str2="2006-08-21T12:15:42Z";
   axis2_env_t *env = NULL;
   axis2_status_t status = AXIS2_FAILURE;

   env = test_init();

   printf("--Testing started--------------------------------------------\n");

     
   status  =  rampart_compare_date_time(env, date_str1, date_str2);
   printf("Status = %d", status);
 
   printf("\n----End of testings----------------------------------------\n");
   return 0;
}


