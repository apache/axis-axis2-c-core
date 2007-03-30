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
  
#ifndef W2C_STRING_H
#define W2C_STRING_H

/**
 * @file w2c_string.h
 * @brief handles Axis2/C w2c handles string processing
 */

#include <stdio.h>
#include <axis2_utils.h>
#include <axis2_string.h>
#include <axis2_qname.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_string handles string processing
 * @ingroup w2c_utils
 * @{
 */    

/**
 * gives the index of the given axis2_char_tacter in its first occurrence.
 * @param str		string operation apply
 * @param c		character
 * @return 		index of the first occurrence
 */
AXIS2_EXTERN int AXIS2_CALL
w2c_string_indexof( axis2_char_t *str, axis2_char_t c );
/**
 * gives the index of the given axis2_char_tacters in its first occurrence.
 * @param str		string operation apply
 * @param cs		characters
 * @return 		index of the first occurrence
 */
AXIS2_EXTERN int AXIS2_CALL
w2c_string_indexof_cs( axis2_char_t *str, axis2_char_t *cs );

/**
 * if string1 is NULL new string created
 * otherwise add string2 to string1.
 * @param env pointer to the environment. MUST NOT be NULL
 * @param string1	string
 * @param string2	string
 * @return		added string
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
w2c_string_add_string ( axis2_char_t *string1, axis2_char_t *string2,
                   const axutil_env_t *env );

/**
 * convert given name to an axis2/c convension
 * ie whenver the case change '_' is placed between
 * all letters convert to simple.
 * @param name name to convert
 * @param env pointer to the environment. MUST NOT be NULL
 * @return	c simple name	
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
w2c_string_make_c_simple_name( axis2_char_t *name,
                               const axutil_env_t *env);

/**
 * convert given name to an axis2/c convension macro name
 * ie whenver the case change '_' is placed between
 * all letters convert to capital.
 * @param name name to convert
 * @param env pointer to the environment. MUST NOT be NULL
 * @return	c macro name	
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
w2c_string_make_c_macro_name(axis2_char_t *name,
                               const axutil_env_t *env);

/**
 * get a hash key from the qname.
 * @param qname qname to make the key
 * @param env pointer to the environment. MUST NOT be NULL
 * @return	newly create hash key
 */
AXIS2_EXTERN axis2_char_t*
w2c_string_make_key_from_qname(axis2_qname_t *qname,
                               const axutil_env_t *env);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_STRING_H */
