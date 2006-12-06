/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
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
  * @file oxs_cipher.h
  * @brief 
  */

#include <axis2_defines.h>
#include <oxs_constants.h>
#include <openssl_cipher_property.h>
#include <axis2_env.h>
#ifdef __cplusplus
extern "C"
{
#endif

AXIS2_EXTERN openssl_cipher_property_t *AXIS2_CALL
oxs_get_cipher_property_for_url(const axis2_env_t *env,
                     axis2_char_t *url);

/*Get the cipher name for the given url*/ 
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_get_cipher_name_for_url(const axis2_env_t *env,
                     axis2_char_t *url);

/*Get the cipher url for the given name*/
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_get_cipher_url_for_name(const axis2_env_t *env,
                     axis2_char_t *name);

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_CIPHER_H */
