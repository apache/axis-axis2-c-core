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
  
#ifndef W2C_MESSAGES_H
#define W2C_MESSAGES_H

/**
 * @file w2c_messages.h
 * @brief handles Axis2/C w2c messages
 */

#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_properties.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define W2C_MESSAGES_INTER_PATH "/conf/"
    
/**
 * @defgroup w2c_messages handles file writing
 * @ingroup w2c_utils
 * @{
 */    

/**
 * creates/ returns a hash of messages.
 * @param env double pointer to environment struct. MUST NOT be NULL
 * @param file_name     name of the file
 * @return the hash including the messages
 */
AXIS2_EXTERN axis2_properties_t* AXIS2_CALL
w2c_messages_get_message_properties( 
    const axis2_env_t *env );

/**
 * return the message for specified key.
 * @param env double pointer to environment struct. MUST NOT be NULL
 * @param key key for the message
 * @return the message
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
w2c_messages_get_message( const axis2_env_t *env, axis2_char_t *key );


/**
 * print and log the message for specified key.
 * @param env double pointer to environment struct. MUST NOT be NULL
 * @param key key for the message
 */
AXIS2_EXTERN void AXIS2_CALL
w2c_messages_print_n_log_error( const axis2_env_t *env,
                                 axis2_char_t *key );

/**
 * print and log the message for specified key from given properties.
 * @param env double pointer to environment struct. MUST NOT be NULL
 * @param properties properties to find the key
 * @param key key for the message
 */
AXIS2_EXTERN void AXIS2_CALL
w2c_messages_print_n_log_error_from_properties( const axis2_env_t *env,
                                          axis2_properties_t *props,
                                          axis2_char_t *key );

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_MESSAGES_H */
