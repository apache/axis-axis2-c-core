/**
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

#ifndef AXIS2_STRING_H
#define AXIS2_STRING_H

#include <axis2_utils_defines.h>
#include <axis2_error.h>
#include <axis2_env.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif
    
    /**
     * @defgroup axis2_string string
     * @ingroup axis2_util
     * @{
     */

    typedef struct axis2_string axis2_string_t;

    /**
     * Creates a string struct.
     * @param str pointer to string. string struct would create a duplicate of 
     * this    
     * @param env pointer to environment struct
     * @return a pointer to newly created string struct
     */
    AXIS2_EXTERN axis2_string_t * AXIS2_CALL
    axis2_string_create(const axis2_env_t *env,
        const axis2_char_t *str);
    
    /**
     * Creates a string struct.
     * @param str pointer to string. string struct would not create a duplicate 
     * of this, but would assume ownership
     * @param env pointer to environment struct
     * @return a pointer to newly created string struct
     */
    AXIS2_EXTERN axis2_string_t * AXIS2_CALL
    axis2_string_create_assume_ownership(const axis2_env_t *env,
        axis2_char_t **str);

    /**
     * Creates a string struct.
     * @param str pointer to string. string struct would not create a duplicate 
     * of this and assumes the str would have longer life than that of itself
     * @param env pointer to environment struct
     * @return a pointer to newly created string struct     
     */
    AXIS2_EXTERN axis2_string_t* AXIS2_CALL
    axis2_string_create_const(const axis2_env_t *env,
        axis2_char_t **str);

    /**
     * Frees string struct.
     * @param string pointer to string struct
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_string_free(struct axis2_string *string,
        const axis2_env_t *env);

    /**
     * Compares two strings. Checks if the two strings point to the same buffer.
     * Do not cmpare the buffer contents.
     * @param string pointer to string struct
     * @param env pointer to environment struct
     * @param string1 pointer to string struct to be compared
     * @return AXIS2_TRUE if string equals string1, AXIS2_FALSE otherwise 
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_string_equals(const struct axis2_string *string,
        const axis2_env_t *env,
        const struct axis2_string *string1);

    /**
     * Clones a given string. Does not duplicate the buffer, rather 
     * increments the reference count. Each call to clone needs to have a 
     * matching free, when the clone is done with. 
     * @param string pointer to string struct
     * @param env pointer to environment struct
     * @returns pointer to cloned string struct instance
     */
    AXIS2_EXTERN struct axis2_string *AXIS2_CALL
    axis2_string_clone(struct axis2_string *string,
        const axis2_env_t *env);

    /**
     * Gets string buffer.
     * @param string pointer to string struct
     * @param env pointer to environment struct
     * @returns pointer to string buffer
     */
    AXIS2_EXTERN const axis2_char_t* AXIS2_CALL
    axis2_string_get_buffer(const struct axis2_string *string,
        const axis2_env_t *env);

    /**
     * Gets string length.
     * @param string pointer to string struct
     * @param env pointer to environment struct
     * @returns buffer length
     */
    AXIS2_EXTERN unsigned int AXIS2_CALL
    axis2_string_get_length(const struct axis2_string *string,
        const axis2_env_t *env);
    
    /** @} */

    /**
     * @defgroup axis2_string_utils string_utils
     * @ingroup axis2_util
     * @{
     */

    AXIS2_EXTERN void* AXIS2_CALL
    axis2_strdup(const void *ptr,
        const axis2_env_t *env);

    /**
     * duplicate the first n characters of a string into memory allocated 
     * the new string will be null-terminated
     * @param ptr The string to duplicate
     * @param n The number of characters to duplicate
     * @return The new string
     */
    AXIS2_EXTERN void* AXIS2_CALL
    axis2_strndup(const void *ptr,
        int n,
        const axis2_env_t *env);

    /**
     * Create a null-terminated string by making a copy of a sequence
     * of characters and appending a null byte
     * @param ptr The block of characters to duplicate
     * @param n The number of characters to duplicate
     * @return The new string
     * @remark This is a faster alternative to axis2_strndup, for use
     *         when you know that the string being duplicated really
     *         has 'n' or more characters.  If the string might contain
     *         fewer characters, use axis2_strndup.
     */
    AXIS2_EXTERN void * AXIS2_CALL
    axis2_strmemdup(const void *ptr,
        size_t n,
        const axis2_env_t *env);

    AXIS2_EXTERN void * AXIS2_CALL
    axis2_memchr(const void *ptr,
        int c,
        size_t n);

    AXIS2_EXTERN int AXIS2_CALL
    axis2_strcmp(const axis2_char_t * s1,
        const axis2_char_t * s2);

    AXIS2_EXTERN int AXIS2_CALL
    axis2_strncmp(const axis2_char_t * s1,
        const axis2_char_t * s2,
        int n);

    AXIS2_EXTERN axis2_ssize_t AXIS2_CALL
    axis2_strlen(const axis2_char_t * s);

    AXIS2_EXTERN int AXIS2_CALL
    axis2_strcasecmp(const axis2_char_t *s1,
        const axis2_char_t *s2);

    AXIS2_EXTERN int AXIS2_CALL
    axis2_strncasecmp(const axis2_char_t *s1,
        const axis2_char_t *s2, 
        int n);

    /* much similar to the strcat behaviour. But the difference is
     * this allocates new memory to put the conatenated string rather than
     * modifying the first argument. The user should free the allocated
     * memory for the return value
     */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_stracat(const axis2_char_t *s1,
        const axis2_char_t *s2,
        const axis2_env_t *env);

    /**
     * Concatenate multiple strings, allocating memory
     * @param ... The strings to concatenate.  The final string must be NULL
     * @return The new string
     */
    AXIS2_EXTERN axis2_char_t * AXIS2_CALL
    axis2_strcat(const axis2_env_t *env, ...);

    AXIS2_EXTERN axis2_char_t * AXIS2_CALL
    axis2_strstr(const axis2_char_t *heystack,
        const axis2_char_t *needle);

    AXIS2_EXTERN axis2_char_t * AXIS2_CALL
    axis2_rindex(const axis2_char_t *s,
        axis2_char_t c);

    /* replaces s1 with s2 */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_replace(const axis2_env_t *env,
        axis2_char_t *str,
        int s1,
        int s2);

    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_strltrim(const axis2_env_t *env,
        const axis2_char_t *_s,
        const axis2_char_t *_trim);

    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_strrtrim(const axis2_env_t *env,
        const axis2_char_t *_s,
        const axis2_char_t *_trim);

    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_strtrim(const axis2_env_t *env,
        const axis2_char_t *_s,
        const axis2_char_t *_trim);

    /**
     * replace given axis2_character with a new one.
     * @param str       string operation apply
     * @param old_char  the old axis2_character which would be replaced
     * @param new_char  new axis2_char_tacter
     * @return      replaced string
     */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_string_replace(axis2_char_t *str,
        axis2_char_t old_char,
        axis2_char_t new_char);

    /**
     * gives a sub string starting with given index.
     * @param str       string operation apply
     * @param c     starting index
     * @return      substring
     */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_string_substring_starting_at(axis2_char_t *str,
        int s);

    /**
     * gives a sub string ending with given index.
     * @param str       string operation apply
     * @param c     ending index
     * @return      substring
     */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_string_substring_ending_at(axis2_char_t *str,
        int e);

    /**
     * set a string to lowercase.
     * @param str   string
     * @return string with lowercase
     */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_string_tolower(axis2_char_t *str);

    /**
     * set a string to uppercase.
     * @param str   string
     * @return string with uppercase
     */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_string_toupper(axis2_char_t *str);

    /**
     * Finds the first occurrence of the substring needle in the string 
     * haystack, ignores the case of both arguments. 
     * @param haystack string in which the given string is to be found
     * @param needle string to be found in haystack
     * @return pointer to the beginning of the substring, 
     * or NULL  if  the  substring  is  not found
     */
    AXIS2_EXTERN axis2_char_t * AXIS2_CALL
    axis2_strcasestr(const axis2_char_t *heystack,
        const axis2_char_t *needle);


#define AXIS2_STRDUP(pts, env) \
        axis2_strdup(pts, env)

#define AXIS2_STRNDUP(pts, n, env) \
        axis2_strndup(pts, n, env)

#define AXIS2_STRMEMDUP(pts, n, env) \
        axis2_strmemdup(pts, n, env)

#define AXIS2_MEMCHR(pts, c, n) \
        axis2_memchr(pts, c, n)

#define AXIS2_STRCMP(s1, s2) \
        axis2_strcmp(s1, s2)

#define AXIS2_STRNCMP(s1, s2, n) \
        axis2_strncmp(s1, s2, n)

#define AXIS2_STRLEN(s) \
        axis2_strlen(s)

#define AXIS2_STRCASECMP(s1,s2) \
        axis2_strcasecmp(s1,s2)

#define AXIS2_STRNCASECMP(s1,s2,n) \
        axis2_strncasecmp(s1,s2,n)

#define AXIS2_STRACAT(s1, s2, env) \
        axis2_stracat(s1, s2, env)

#define AXIS2_STRSTR(s1, s2) \
        axis2_strstr(s1, s2)

#define AXIS2_RINDEX(s, c) \
        axis2_rindex(s, c)

#define AXIS2_REPLACE(env, str, s1, s2) \
        axis2_replace(env, str, s1, s2)

#define AXIS2_STRLTRIM(env, s, trim) \
      axis2_strltrim(env, s, trim)

#define AXIS2_STRRTRIM(env, s, trim) \
      axis2_strrtrim(env, s, trim)

#define AXIS2_STRTRIM(env, s, trim) \
      axis2_strtrim(env, s, trim)

    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_STRING_H */
