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

#ifndef OXS_IV_H
#define OXS_IV_H


/**
  * @file oxs_key.h
  * @brief 
  */

#include <axis2_defines.h>
#include <oxs_constants.h>
#include <openssl_constants.h>
#include <axis2_env.h>

#ifdef __cplusplus
extern "C"
{
#endif


#define OXS_IV_DEFAULT    OPENSSL_DEFAULT_IV16


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_iv_generate_for_algo(const axis2_env_t *env, 
                            axis2_char_t *key_algo);



/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_IV_H */
