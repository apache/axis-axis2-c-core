#include <stdio.h>
#include <rampart_util.h>
#include <rampart_crypto_util.h>
#include <axis2_util.h>
#include <axis2_base64.h>

axis2_env_t *test_init()
{
    axis2_allocator_t *allocator = axis2_allocator_init(NULL);
    axis2_error_t *error = (axis2_error_t*)axis2_error_create(allocator);
    axis2_env_t *env = axis2_env_create_with_error(allocator, error);
    return env;
}

int main()
{
    axis2_char_t* hash = NULL;
    axis2_env_t *env = NULL;
    axis2_char_t* nonce = NULL;
    axis2_char_t* created = NULL;

    env = test_init();

    printf("--Testing started--------------------------------------------\n");
    printf("\n--Testing hash-----------------------------------------------\n");

    hash = rampart_crypto_sha1(env, "WScqanjCEAC4mQoBE07sAQ==", "2003-07-16T01:24:32Z", "IloveDogs");
    if (!hash)
    {
        printf("No hash generated :( ...\n");
    }
    printf("Hash = %s", hash);
#if 0
    /*encode Base64*/
    axis2_char_t* encodedStr = NULL;


    encodedStr = AXIS2_MALLOC(env->allocator, 20);
    axis2_base64_encode_binary(encodedStr, hash, 20);

    printf("Hash = %s \n", encodedStr);
#endif
    printf("\n----Testing nonce------------------------------------------\n");
    nonce = rampart_generate_nonce(env);
    printf("NONCE: %s", nonce);

    printf("\n----Testing created----------------------------------------\n");
    created = rampart_generate_time(env, 300);
    printf("CREATED: %s", created);
    printf("\n----End of testings----------------------------------------\n");
    return 0;
}


