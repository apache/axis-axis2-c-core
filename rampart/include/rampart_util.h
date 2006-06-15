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


#include <axis2_utils_defines.h>
#include <axis2_defines.h>
#include <axis2_env.h>

/**
  * @file rampart_util.h
  * @brief RAMPART-UTIL 
  */
#ifndef RAMPART_UTIL_H
#define RAMPART_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @defgroup Rampart_Util
  * @ingroup Rampart_Util
  */

                                                                            
 /**
    * Generates the nonce. Nonce is a base64 encoded random string.
    * @return generated nonce
    */    

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
   rampart_generate_nonce(const axis2_env_t *env);

  /**
    * Generates the nonce. Nonce is a base64 encoded random string.
    * @param ttl Time to live. The time difference between created and expired in mili seconds.
    * @return generated nonce
    **/    
AXIS2_EXTERN axis2_char_t* AXIS2_CALL 
rampart_generate_time(const axis2_env_t *env, int ttl);

AXIS2_EXTERN int AXIS2_CALL 
rampart_format_date_zulu_to_mili(const axis2_env_t *env, axis2_char_t *str);


AXIS2_EXTERN axis2_status_t AXIS2_CALL 
rampart_print_info(const axis2_env_t *env, axis2_char_t* info);

/* @} */
#ifdef __cplusplus
}
#endif

#endif    /* !RAMPART_UTIL_H */
