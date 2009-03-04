#include <axutil_log_default.h>
#include <axutil_error_default.h>
#include <axiom_node.h>
#include <stdio.h>
#include "../util/create_env.h"

int plain_binary_len;
unsigned char *plain_binary ;

/** @brief read binary
 *  read the binary file 
 */

int read_binary()
{
      unsigned char buffer[1024];
      FILE *in = fopen("test","rb");
      FILE *out = fopen("test.doc","w");
      int fwrite_result = 0;

      if (!(in && out))
      {
          fprintf (stderr, "unable to open streams\n");
          return -1;
      }

      while((plain_binary_len = fread(buffer,1,sizeof(buffer),in)) > 0)
      {
          fwrite_result = fwrite(buffer,1,plain_binary_len,out);
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
axis2_status_t test_base64(axutil_env_t *env)
{  
    axis2_status_t status = AXIS2_FAILURE;
    axutil_base64_binary_t *base64_binary;
    axutil_base64_binary_t *plain_base64_binary;
    const char *encoded_binary;
    char * get_binary = NULL;
    int binary_len;
    unsigned char * plain_binary = NULL;
    read_binary(); 

    base64_binary = axutil_base64_binary_create(env);
    if(!base64_binary)
    {
        printf("The test axutil_base64_binary_create is failed\n");
        axutil_base64_binary_free(base64_binary,env);
        return AXIS2_FAILURE;
    }
    else 
        printf("The test axutil_base64_binary_create is successfull\n");

    plain_base64_binary = axutil_base64_binary_create_with_plain_binary(env,
                                                                        plain_binary,
                                                                        plain_binary_len); 
    if(!plain_base64_binary)
    {
        printf("The test axutil_base64_binary_create_with_plain_binary is failed\n");
        axutil_base64_binary_free(plain_base64_binary,env);
    }
    else
        printf("The test axutil_base64_binary_create_with_plain_binary is successfull\n");

    encoded_binary = axutil_base64_binary_get_encoded_binary(base64_binary,env);
    if(!encoded_binary)
    {
        printf("The test axutil_base64_binary_get_encoded_binary is failed\n");
        axutil_base64_binary_free(base64_binary,env);
    }
    else
        printf("The test axutil_base64_binary_get_encoded_binary is successfull\n");

    status = axutil_base64_binary_set_plain_binary(base64_binary,env,plain_binary,
                                                   plain_binary_len);
    if (status == AXIS2_SUCCESS)
        printf("The test axutil_base64_binary_set_plain_binary is successful\n");
    else
        printf("The test axutil_base64_binary_set_plain_binary failed\n") ;

    plain_binary = axutil_base64_binary_get_plain_binary(base64_binary,env,&plain_binary_len);
    if(!plain_binary)
    {   
        printf("The test axutil_base64_binary_get_plain_binary is failed\n");
        axutil_base64_binary_free(base64_binary,env);
    }
    else
        printf("The test axutil_base64_binary_get_plain_binary is successfull\n" );

    status = axutil_base64_binary_set_encoded_binary(base64_binary,env,encoded_binary);
    if (status == AXIS2_SUCCESS)
        printf("The test axutil_base64_binary_set_encoded_binary is successful\n");
    else
        printf("The test axutil_base64_binary_set_encoded_binary failed\n");

    get_binary = axutil_base64_binary_get_encoded_binary(base64_binary,env);
    if(!get_binary)
    {
        printf("The test axutil_base64_binary_get_encoded_binary is failed\n");
        axutil_base64_binary_free(base64_binary,env);
    }
    else
        printf("The test axutil_base64_binary_get_encoded_binary is successfull\n");

    binary_len = axutil_base64_binary_get_encoded_binary_len(base64_binary,env);
    if(!binary_len)
    {
        printf("The test axutil_base64_binary_get_encoded_binary_len is failed\n");
        axutil_base64_binary_free(base64_binary,env);
    }
    else
        printf("The test  axutil_base64_binary_get_encoded_binary_len is successfull\n");

    return AXIS2_SUCCESS;
}

int main()
{
    int status = AXIS2_SUCCESS;
    axutil_env_t *env = NULL;
    env = create_environment();
    status = test_base64(env);

    if(status == AXIS2_FAILURE)
    {
        printf("build  failed");
    }
    axutil_env_free(env);
    return 0;
}








