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
#include <oxs_iv.h>
#include <oxs_buffer.h>
#include <oxs_cipher.h>
#include <oxs_error.h>
#include <openssl_cipher_property.h>


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_iv_generate_for_algo(const axis2_env_t *env,
                            axis2_char_t *key_algo)
{
    axis2_char_t* iv = NULL;
    openssl_cipher_property_t *cprop = NULL;
    int size;
    axis2_char_t *algo_name = NULL; /*This is not the url name*/

    /*Check algo and then set the key data size))*/
    algo_name = oxs_get_cipher(env, key_algo);
    if(!algo_name){
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
            "oxs_get_cipher failed");
        return NULL;
    }
    cprop = (openssl_cipher_property_t *)openssl_get_cipher_property(env, algo_name);
    if(!cprop){
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
            "openssl_get_cipher_property failed");
        return NULL;
    }
    size = OPENSSL_CIPHER_PROPERTY_GET_IV_SIZE(cprop, env);
    
    /*Here we have predefined IVs in the openssl_constants.
      Get the correct one using the size*/ 
    
    if(size == 8){
        iv = OPENSSL_DEFAULT_IV8;
    }else if(size == 16){
        iv = OPENSSL_DEFAULT_IV16;
    }else if(size == 24){
        iv = OPENSSL_DEFAULT_IV24;
    }else{
        iv = OXS_IV_DEFAULT;/* i.e. OPENSSL_DEFAULT_IV16. Many ciphers have 16... mmm.. need a better way.. a TODO*/
    }
    printf("IVIVIVIV size=%d value = %s", size, iv);
    
    return iv;
}
