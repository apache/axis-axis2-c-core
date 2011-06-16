
/*
 *   Copyright 2004,2005 The Apache Software Foundation.
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
 *
 */

#ifndef AXIS2_NTLM_H
#define AXIS2_NTLM_H

/**
 *@file axis2_ntlm.h
 *@brief this is the ntlm authentication abstraction layer for axis2
 */

#include <axutil_env.h>
#include <axutil_utils.h>

#ifdef __cplusplus
extern "C"
{
#endif

    #define NTLM_NEG_NTLM           0x00000200
    #define NTLM_NEG_UNICODE        0x00000001
    #define NTLM_TARGET_DOMAIN      0x00010000

    typedef struct axis2_ntlm_ops axis2_ntlm_ops_t;
    typedef struct axis2_ntlm axis2_ntlm_t;

    /**
      * @defgroup axis2_ntlm ntlm
      * @ingroup axiom
      * @{
      * @}
      */

    /**
     * @defgroup axis2_ntlm ntlm
     * @ingroup axis2_ntlm
     * @{
     */

    /**
      * \brief AXIS2_NTLM ops
      * Encapsulator struct for ops of axiis2_ntlm
      */

    struct axis2_ntlm_ops
    {
        /**
        * free pull_ntlm
        * @param ntlm axis2_ntlm struct 
        * @param env    axutil_environment MUST NOT be NULL 
        * @returns axis2_status_code
        */

        void (
            AXIS2_CALL
            * free)(
                axis2_ntlm_t * ntlm,
                const axutil_env_t * env);

        /**
         * Create Type1 NTLM message. This will create a type1 message and assign
         * it to the return type. 
         * @param ntlm axis2_ntlm  
         * @param env    axutil_environment, MUST NOT be NULL.
         * @param encoded_message    return type passed by caller.
         * @param encoded_len length of the encoded message
         * @param uname user name
         * @param passwd password
         * @param flags NTLM flags
         * @param domain domain
         * @returns status , AXIS2_FAILURE on error 
         */

        axis2_status_t (
            AXIS2_CALL
            * create_type1_message)(
                axis2_ntlm_t * ntlm,
                const axutil_env_t * env,
                axis2_char_t **encoded_message,
                int *encoded_len,
                const axis2_char_t *uname,
                const axis2_char_t *passwd,
                const int flags,
                const axis2_char_t *domain);

        /**
         * Create Type3 NTLM message. This will create a type3 message and assign
         * it to the return type. 
         * @param ntlm axis2_ntlm  
         * @param env    axutil_environment, MUST NOT be NULL.
         * @param header_value Header value extracted from the type2 message
         * @param encoded_message    return type passed by caller.
         * @param encoded_len length of the encoded message
         * @param uname user name
         * @param passwd password
         * @param domain domain
         * @param workstation workstation
         * @returns status , AXIS2_FAILURE on error 
         */

        axis2_status_t(
            AXIS2_CALL
            * create_type3_message)(
                axis2_ntlm_t * ntlm,
                const axutil_env_t * env,
                axis2_char_t *header_value,
                axis2_char_t **encoded_message,
                int *encoded_len,
                const axis2_char_t *uname,
                const axis2_char_t *passwd,
                const axis2_char_t *domain,
                const axis2_char_t *workstation);


    };

    /**
     * @brief axis2_ntlm struct
      *   Axis2 NTLM
     */
    struct axis2_ntlm
    {
        const axis2_ntlm_ops_t *ops;
    };

    /**
     * Creates an instance of axis2_ntlm that would be used to
     * authorize NTLM
     * @param env environment struct, must not be null
     * @returns a pointer to axis2_ntlm_t struct
     * NULL on error with error code set in the environment's error
     */
    AXIS2_EXTERN axis2_ntlm_t *AXIS2_CALL
    axis2_ntlm_create(
        const axutil_env_t * env);


    /**
     * free method for ntlm
     * @param ntlm pointer to the NTLM struct
     * @param env environment struct, must not be null
     *
     * @return 
     */
    AXIS2_EXTERN void AXIS2_CALL
    axis2_ntlm_free(
        axis2_ntlm_t * ntlm,
        const axutil_env_t * env);

    /**
     * Create Type1 NTLM message. This will create a type1 message and assign
     * it to the return type. 
     * @param ntlm axis2_ntlm  
     * @param env    axutil_environment, MUST NOT be NULL.
     * @param encoded_message    return type passed by caller.
     * @param encoded_len length of the encoded message
     * @param uname user name
     * @param passwd password
     * @param flags NTLM flags
     * @param domain domain
     * @returns status , AXIS2_FAILURE on error 
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_ntlm_auth_create_type1_message(
        axis2_ntlm_t * ntlm,
        const axutil_env_t * env,
        axis2_char_t **encoded_message,
        int *encoded_len,
        const axis2_char_t *uname,
        const axis2_char_t *passwd,
        const int flags,
        const axis2_char_t *domain);

    /**
     * Create Type3 NTLM message. This will create a type3 message and assign
     * it to the return type. 
     * @param ntlm axis2_ntlm  
     * @param env    axutil_environment, MUST NOT be NULL.
     * @param header_value Header value extracted from the type2 message
     * @param encoded_message    return type passed by caller.
     * @param encoded_len length of the encoded message
     * @param uname user name
     * @param passwd password
     * @param domain domain
     * @param workstation workstation
     * @returns status , AXIS2_FAILURE on error 
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_ntlm_auth_create_type3_message(
        axis2_ntlm_t * ntlm,
        const axutil_env_t * env,
        axis2_char_t *header_value,
        axis2_char_t **encoded_message,
        int *encoded_len,
        const axis2_char_t *uname,
        const axis2_char_t *passwd,
        const axis2_char_t *domain,
        const axis2_char_t *workstation);

    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_NTLM_H */














