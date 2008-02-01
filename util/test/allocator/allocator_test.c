#include <axutil_log_default.h>
#include <axutil_error_default.h>
#include <axiom_node.h>
#include <stdio.h>
#include "../util/create_env.h"

FILE *f = NULL;
unsigned char *plain_binary ;
const char *encoded_binary;
unsigned char buffer[1024];
int plain_binary_len;

axutil_base64_binary_t *base64_binary;
axutil_env_t *env = NULL;
axutil_base64_binary_t *base64_binary;

/** @brief read binary
 *  read the binary file 
 */
int read_binary()
{
      FILE *in = fopen("test","rb");
      FILE *out = fopen("test.doc","w");

      while((plain_binary_len = fread(buffer,1,sizeof(buffer),in)) > 0)
      {
         fwrite(buffer,1,plain_binary_len,out);
      }
      fclose(in);
      fclose(out);
      plain_binary = buffer;
      printf("%s",buffer);
      return plain_binary_len;
}

/** @brief test base64
 *  create duration from values and retrieve values
 */
axis2_status_t test_base64()
{  
    env = create_environment();
    read_binary(); 
    axutil_base64_binary_create(env);
    printf(" base64 binary created\n");
    base64_binary = axutil_base64_binary_create_with_plain_binary(env,plain_binary,plain_binary_len);
    printf(" base64 binary created with plain binary\n"); 
    encoded_binary = axutil_base64_binary_get_encoded_binary(base64_binary,env);
    printf(" get encorded binary \n");
    axutil_base64_binary_create_with_encoded_binary(env,encoded_binary);
    printf(" base64 binary created with encorded binary \n");
    axutil_base64_binary_set_plain_binary(base64_binary,env,plain_binary,plain_binary_len);
    axutil_base64_binary_get_plain_binary(base64_binary,env,&plain_binary_len);
    axutil_base64_binary_set_encoded_binary(base64_binary,env,encoded_binary);
    axutil_base64_binary_get_encoded_binary(base64_binary,env);
    axutil_base64_binary_get_encoded_binary_len(base64_binary,env);
    axutil_base64_binary_free(base64_binary,env);
    return AXIS2_SUCCESS;
}

int main()
{
    int status = AXIS2_SUCCESS;

    axutil_env_t *env = NULL;
    env = create_environment();

    status = test_base64();

    if(status == AXIS2_FAILURE)
    {
        printf(" build  failed");
    }
    axutil_env_free(env);
    return 0;
}








