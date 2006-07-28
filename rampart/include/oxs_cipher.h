/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OXS_CIPHER_H
#define OXS_CIPHER_H


/**
  * @file oxs_utils.h
  * @brief 
  */

#include <axis2_defines.h>
#include <oxs_constants.h>
#include <axis2_env.h>
#ifdef __cplusplus
extern "C"
{
#endif

/*
typedef struct _oxs_cipher oxs_cipher, *oxs_cipher_ptr;

struct _oxs_cipher
{
    axis2_char_t *name;
    int key_size;
    int iv_size;
};
*/
  
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_get_cipher(const axis2_env_t *env,
                     axis2_char_t *url);


AXIS2_EXTERN int AXIS2_CALL
oxs_get_cipher_key_size(const axis2_env_t *env,
                     axis2_char_t *url);


AXIS2_EXTERN int AXIS2_CALL
oxs_get_cipher_iv_size(const axis2_env_t *env,
                     axis2_char_t *url);
/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_CIPHER_H */
