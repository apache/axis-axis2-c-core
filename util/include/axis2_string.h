/**
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
 * @defgroup axis2_string String
 * @ingroup axis2_util 
 * @{
 */

    AXIS2_EXTERN void* AXIS2_CALL
    axis2_strdup (const void *ptr, 
                  const axis2_env_t *env);

    AXIS2_EXTERN int AXIS2_CALL
    axis2_strcmp (const axis2_char_t * s1, 
                  const axis2_char_t * s2);

    AXIS2_EXTERN axis2_ssize_t AXIS2_CALL
    axis2_strlen(const axis2_char_t * s);

    AXIS2_EXTERN int AXIS2_CALL
    axis2_strcasecmp(const axis2_char_t *s1, 
                     const axis2_char_t *s2);

    AXIS2_EXTERN int AXIS2_CALL
    axis2_strncasecmp(const axis2_char_t *s1, 
                      const axis2_char_t *s2, int n);
    
    /* much similar to the strcat behaviour. But the difference is
     * this allocates new memory to put the conatenated string rather than
     * modifying the first argument. The user should free the allocated
     * memory for the return value
     */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_stracat(const axis2_char_t *s1, 
                  const axis2_char_t *s2, 
                  const axis2_env_t *env);
    
    AXIS2_EXTERN axis2_char_t * AXIS2_CALL
    axis2_strstr (const axis2_char_t *heystack,
                  const axis2_char_t *needle);

    AXIS2_EXTERN axis2_char_t * AXIS2_CALL
    axis2_rindex(const axis2_char_t *s, 
                 axis2_char_t c);
                 
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_replace(axis2_env_t *env,
                  axis2_char_t *str,
                  int s1,
                  int s2);
                                   

#define AXIS2_STRDUP(pts, env) \
        axis2_strdup(pts, env)
        
#define AXIS2_STRCMP(s1, s2) \
        axis2_strcmp(s1, s2)
        
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

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_STRING_H */
