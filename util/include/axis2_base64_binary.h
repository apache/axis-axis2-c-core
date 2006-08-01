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

#ifndef AXIS2_BASE64_BINARY_H
#define AXIS2_BASE64_BINARY_H

#include <axis2_base64.h>
#include <axis2_utils_defines.h>
#include <axis2_env.h>

/**
 * @defgroup axis2_base64_binary encoding holder
 * @ingroup axis2_util
 * @{
 */

/**
 * @file axis2_base64_binary.h
 * @brief axis2-util base64 encoding holder
 */

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_base64_binary_ops */
    typedef struct axis2_base64_binary_ops axis2_base64_binary_ops_t;
    /** Type name for struct axis2_base64_binary */
    typedef struct axis2_base64_binary axis2_base64_binary_t;

    AXIS2_DECLARE_DATA struct axis2_base64_binary_ops
    {
        /**
         * free the axis2_base64_binary.
         * @param base64_binary represet the type object
         * @param env pointer to environment struct. MUST NOT be NULL
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_base64_binary_t *base64_binary,
                    const axis2_env_t *env);

        /**
         * store the value from plain binary.
         * @param base64_binary represet the type object
         * @param env pointer to environment struct. MUST NOT be NULL
         * @param plain_binary binary buffer to store
         * @param plain_binary_len length of the plain_binary binary buffer
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_plain_binary)(
                    axis2_base64_binary_t *base64_binary,
                    const axis2_env_t *env,
                    const unsigned char* plain_binary,
                    int plain_binary_len);

        /**
         * retrieve the value from plain binary.
         * @param base64_binary represet the type object
         * @param env pointer to environment struct. MUST NOT be NULL
         * @param plain_binary_len length of the plain_binary binary buffer
         * @return the plain binary
         */

        unsigned char* (AXIS2_CALL *
                get_plain_binary)(
                    axis2_base64_binary_t *base64_binary,
                    const axis2_env_t *env,
                    int* plain_binary_len );

        /**
         * store the value from encoded binary.
         * @param base64_binary represet the type object
         * @param env pointer to environment struct. MUST NOT be NULL
         * @param encoded_binary encoded binary buffer to store
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_encoded_binary)(
                   axis2_base64_binary_t *base64_binary,
                   const axis2_env_t *env,
                   const char* encoded_binary );

        /**
         * retrieve the value from encoded binary.
         * @param base64_binary represet the type object
         * @param env pointer to environment struct. MUST NOT be NULL
         * @return the encoded binary
         */
        char* (AXIS2_CALL *
                get_encoded_binary)(
                    axis2_base64_binary_t *base64_binary,
                    const axis2_env_t *env );

        /**
         * retrieve the value from encoded binary length.
         * @param base64_binary represet the type object
         * @param env pointer to environment struct. MUST NOT be NULL
         * @return the encoded binary length
         */
        int (AXIS2_CALL *
                get_encoded_binary_len)(
                    axis2_base64_binary_t *base64_binary,
                    const axis2_env_t *env );

        /**
         * retrieve the value from decoded binary length.
         * @param base64_binary represet the type object
         * @param env pointer to environment struct. MUST NOT be NULL
         * @return the decoded binary length
         */
        int (AXIS2_CALL *
                get_decoded_binary_len)(
                    axis2_base64_binary_t *base64_binary,
                    const axis2_env_t *env );


    };

    AXIS2_DECLARE_DATA struct axis2_base64_binary
    {
        axis2_base64_binary_ops_t *ops;
    };

    /**
     * Creates axis2_base64_binary struct
     * @param env double pointer to environment struct. MUST NOT be NULL
     * @return pointer to newly created axis2_base64_binary struct
     */
    AXIS2_EXTERN axis2_base64_binary_t* AXIS2_CALL
    axis2_base64_binary_create(
        const axis2_env_t *env );

    /**
     * Creates axis2_base64_binary struct
     * @param env double pointer to environment struct. MUST NOT be NULL
     * @param plain_binary binary buffer to initialize
     * @return pointer to newly created axis2_base64_binary struct
     */
    AXIS2_EXTERN axis2_base64_binary_t* AXIS2_CALL
    axis2_base64_binary_create_with_plain_binary(
            const axis2_env_t *env,
            const unsigned char *plain_binary,
            int plain_binary_len);

    /**
     * Creates axis2_base64_binary struct.
     * @param env double pointer to environment struct. MUST NOT be NULL
     * @param encoded_binary binary buffer to initialize 
     * @return pointer to newly created axis2_base64_binary struct
     */
    AXIS2_EXTERN axis2_base64_binary_t* AXIS2_CALL
    axis2_base64_binary_create_with_encoded_binary(
            const axis2_env_t *env,
            const char *encoded_binary );

/*************************** Function macros **********************************/

/** Frees the base64 binary.
    @sa axis2_base64_binary_ops#free */
#define AXIS2_BASE64_BINARY_FREE(base64_binary, env) \
     ((base64_binary)->ops->free(base64_binary, env))

/** Sets the plain binary.
    @sa axis2_base64_binary_ops#set_plain_binary */
#define AXIS2_BASE64_BINARY_SET_PLAIN_BINARY(base64_binary, env, plain_binary) \
     ((base64_binary)->ops->set_plain_binary(base64_binary, env, plain_binary))

/** Gets the plain binary.
    @sa axis2_base64_binary_ops#get_plain_binary */
#define AXIS2_BASE64_BINARY_GET_PLAIN_BINARY(base64_binary, env ) \
     ((base64_binary)->ops->get_plain_binary(base64_binary, env))

/** Sets the encoded binary.
    @sa axis2_base64_binary_ops#set_encoded_binary */
#define AXIS2_BASE64_BINARY_SET_ENCODED_BINARY(base64_binary, env, encoded_binary) \
     ((base64_binary)->ops->set_encoded_binary(base64_binary, env, encoded_binary))

/** Gets the encoded binary.
    @sa axis2_base64_binary_ops#get_encoded_binary */
#define AXIS2_BASE64_BINARY_GET_ENCODED_BINARY(base64_binary, env ) \
     ((base64_binary)->ops->get_encoded_binary(base64_binary, env))

/** Gets the encoded binary length.
    @sa axis2_base64_binary_ops#get_encoded_binary_len */
#define AXIS2_BASE64_BINARY_GET_ENCODED_BINARY_LEN(base64_binary, env ) \
     ((base64_binary)->ops->get_encoded_binary_len(base64_binary, env))

/** Gets the decoded binary length.
    @sa axis2_base64_binary_ops#get_decoded_binary_len */
#define AXIS2_BASE64_BINARY_GET_DECODED_BINARY_LEN(base64_binary, env ) \
     ((base64_binary)->ops->get_decoded_binary_len(base64_binary, env))

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_BASE64_BINARY_H */
