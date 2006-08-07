/*
 *   Copyright 2003-2004 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <stdio.h>
#include <axis2_util.h>
#include <oxs_key.h>
#include <oxs_buffer.h>
#include <oxs_cipher.h>
#include <oxs_error.h>
#include <openssl_cipher_property.h>

AXIS2_EXTERN oxs_key_ptr AXIS2_CALL
oxs_key_create_key(const axis2_env_t *env,
                    axis2_char_t *name,
                    unsigned char *data,
                    int size,
                    int usage
                    )
{
    oxs_key_ptr key = NULL;
    key = (oxs_key_ptr)AXIS2_MALLOC(env->allocator,sizeof(oxs_key)); 
    
    key->name = name;
    key->data = data;
    key->size = size;
    key->usage = usage;
    
    return key;

}



AXIS2_EXTERN oxs_key_ptr AXIS2_CALL
oxs_key_read_from_file(const axis2_env_t *env,
                        axis2_char_t *file_name)
{
    oxs_buffer_ptr buf = NULL;
    oxs_key_ptr key = NULL;
    int ret;
    
    buf = oxs_create_buffer(env, OXS_KEY_DEFAULT_SIZE);
    ret = oxs_buffer_read_file(env, buf, file_name);
    
    key = oxs_key_create_key(env, file_name, buf->data, buf->size, OXS_KEY_USAGE_NONE);
    return key; 
    
}

AXIS2_EXTERN oxs_key_ptr AXIS2_CALL
oxs_key_generate_for_algo(const axis2_env_t *env,
                            axis2_char_t *key_algo)
{
    oxs_key_ptr key = NULL;
    oxs_buffer_ptr key_buf = NULL;
    cipher_prop_ptr cprop = NULL;
    axis2_status_t ret = AXIS2_FAILURE;
    int size;
    axis2_char_t *algo_name = NULL; /*This is not the url name*/

    /*Check algo and then set the key data size))*/
    algo_name = oxs_get_cipher(env, key_algo);
    if(!algo_name){
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
            "oxs_get_cipher failed");
        return NULL;
    }
    cprop = openssl_get_cipher_property(env, algo_name);
    if(!cprop){
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
            "openssl_get_cipher_property failed");
        return NULL;
    }
    size = cprop->key_size;
    

    key_buf = oxs_create_buffer(env, size);    
    ret = generate_random_key(env, key_buf, size);    
    if(ret == AXIS2_FAILURE){
         oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
            "generate_random_key failed");
         return NULL;
    }

    key = oxs_key_create_key(env, NULL, key_buf->data, size, OXS_KEY_USAGE_NONE);

    return key;
}
