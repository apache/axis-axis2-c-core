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

#include <axis2_string.h>
#include <stdlib.h>
#include <axis2_utils.h>
#include <axis2_utils_defines.h>
#include <stdarg.h> /* NULL */

/** this is used to cache lengths in axis2_strcat */
#define MAX_SAVED_LENGTHS  6

AXIS2_EXTERN void* AXIS2_CALL
axis2_strdup (const void *ptr, const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    if (ptr)
    {
        int len = axis2_strlen(ptr);
        axis2_char_t * str = (axis2_char_t *) AXIS2_MALLOC( env->allocator, 
            sizeof(axis2_char_t) * (len + 1 ));
        if (!str)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
        memcpy(str, ptr, len + 1);
        return (void *) str;
    }
    else
    {
        return NULL;
    }
}

AXIS2_EXTERN void * AXIS2_CALL
axis2_strmemdup(const void *ptr, 
        size_t n, 
        const axis2_env_t *env)
{
    axis2_char_t *str;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ptr, NULL);

    str = (axis2_char_t *) AXIS2_MALLOC( env->allocator, 
        sizeof(axis2_char_t) * (n + 1 ));
    if (!str)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    memcpy(str, ptr, n);
    str[n] = '\0';

    return (void *) str;
}

AXIS2_EXTERN void * AXIS2_CALL
axis2_memchr (
        const void *ptr, 
        int c, 
        size_t n)
{
    const axis2_char_t *cp;
    
    for (cp = ptr; n > 0; n--, cp++) {
        if (*cp == c)
            return (char *) cp; /* Casting away the const here */
    }

    return NULL;
}

AXIS2_EXTERN void* AXIS2_CALL
axis2_strndup (
        const void *ptr, 
        int n, 
        const axis2_env_t *env )
{
    const axis2_char_t *end;
    axis2_char_t *str;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ptr, NULL);

    end = axis2_memchr(ptr, '\0', n);
    if(NULL != end)
        n = end - (axis2_char_t *) ptr;
    str = (axis2_char_t *) AXIS2_MALLOC( env->allocator, 
        sizeof(axis2_char_t) * (n + 1 ));
    if (!str)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    memcpy(str, ptr, n);
    str[n] = '\0';

    return (void *) str;
}

AXIS2_EXTERN axis2_char_t * AXIS2_CALL
axis2_strcat(
        const axis2_env_t *env, ...)
{
    axis2_char_t *cp, *argp, *str;
    size_t saved_lengths[MAX_SAVED_LENGTHS];
    int nargs = 0;

    /* Pass one --- find length of required string */

    size_t len = 0;
    va_list adummy;

    va_start(adummy, env);

    while ((cp = va_arg(adummy, axis2_char_t *)) != NULL) 
    {
        size_t cplen = strlen(cp);
        if (nargs < MAX_SAVED_LENGTHS) 
        {
            saved_lengths[nargs++] = cplen;
        }
        len += cplen;
    }

    va_end(adummy);

    /* Allocate the required string */

    str = (axis2_char_t *) AXIS2_MALLOC( env->allocator, 
        sizeof(axis2_char_t) * (len + 1 ));
    if (!str)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    cp = str;

    /* Pass two --- copy the argument strings into the result space */

    va_start(adummy, env);

    nargs = 0;
    while ((argp = va_arg(adummy, axis2_char_t *)) != NULL) {
        if (nargs < MAX_SAVED_LENGTHS) {
            len = saved_lengths[nargs++];
        }
        else {
            len = strlen(argp);
        }
 
        memcpy(cp, argp, len);
        cp += len;
    }

    va_end(adummy);

    /* Return the result string */

    *cp = '\0';

    return str;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_stracat(const axis2_char_t *s1, const axis2_char_t *s2, const axis2_env_t *env)
{
    axis2_char_t *ret = NULL;
    int alloc_len = -1;

    if(NULL == s1 && NULL == s2)
    {
        return NULL;
    }
    if(NULL == s1)
    {
        return (axis2_char_t*)AXIS2_STRDUP(s2, env);
    }
    if(NULL == s2)
    {
        return (axis2_char_t*)AXIS2_STRDUP(s1, env);
    }
    alloc_len = axis2_strlen(s1) + axis2_strlen(s2) + 1;
    ret = (axis2_char_t*)AXIS2_MALLOC(env->allocator,
                    alloc_len*sizeof(axis2_char_t));
    memcpy(ret, s1, axis2_strlen(s1)*sizeof(axis2_char_t));
    memcpy((ret + axis2_strlen(s1)*sizeof(axis2_char_t)), s2,
                    axis2_strlen(s2)*sizeof(axis2_char_t));
    ret[alloc_len*sizeof(axis2_char_t) - sizeof(axis2_char_t)] = '\0';
    return ret;
}

AXIS2_EXTERN int AXIS2_CALL
axis2_strcmp (const axis2_char_t * s1, const axis2_char_t * s2)
{
    if (s1 && s2)
        return strcmp (s1, s2);
    else
        return -1;
}


AXIS2_EXTERN axis2_ssize_t AXIS2_CALL
axis2_strlen (const axis2_char_t * s)
{
   if (s)   
      return strlen(s);
   else
      return -1;
}


AXIS2_EXTERN int AXIS2_CALL
axis2_strcasecmp(const axis2_char_t *s1, const axis2_char_t *s2)
{
    while (toupper(*s1) == toupper(*s2++))
      if (*s1++ == '\0')
       return(0);
    return(toupper(*s1) - toupper(*--s2));
}


AXIS2_EXTERN int AXIS2_CALL
axis2_strncasecmp(const axis2_char_t *s1, const axis2_char_t *s2, const int n)
{
    axis2_char_t *str1 = (axis2_char_t *)s1, *str2 = (axis2_char_t *)s2;
    int i = (int)n;
    
    while (--i >= 0 && toupper(*str1) == toupper(*str2++))
    if (toupper(*str1++) == '\0')
       return(0);
    return(i < 0 ? 0 : toupper(*str1) - toupper(*--str2));
}

AXIS2_EXTERN axis2_char_t * AXIS2_CALL
axis2_strstr (const axis2_char_t *heystack,
                const axis2_char_t *needle)
{
   return strstr(heystack, needle);
}


AXIS2_EXTERN axis2_char_t * AXIS2_CALL
axis2_rindex(const axis2_char_t *_s, axis2_char_t _ch)
{
    int i,ilen = axis2_strlen(_s);
    if (ilen < 1)
       return NULL;
    for (i=ilen-1;i>=0;i--)
    {
       if (_s[i] == _ch)
           return (axis2_char_t *)(_s+i);
    }
    return NULL;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_replace(const axis2_env_t *env,
              axis2_char_t *str,
              int s1,
              int s2)
{
    axis2_char_t *newstr = NULL;
    axis2_char_t *index = NULL;
    if(!str)
        return NULL;
    
    newstr = AXIS2_STRDUP(str, env);
    
    index = strchr(newstr, s1);
    while(NULL != index)
    {
        newstr[index - newstr] = s2;
        index = strchr(newstr, s1);
    }
    return newstr;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL 
axis2_strltrim(
        const axis2_env_t *env,
        const axis2_char_t *_s, 
        const axis2_char_t *_trim)
{
    axis2_char_t *_p = NULL;

    if(!_s)
        return NULL;
    _p = (axis2_char_t *) _s;
    if(!_trim)
        _trim = " \t\r\n";

    while(*_p)
    {
        if(!strchr(_trim, *_p))
            return _p;
        ++_p;
    }
    return AXIS2_STRDUP(_p, env);
}
           
AXIS2_EXTERN axis2_char_t* AXIS2_CALL 
axis2_strrtrim(
        const axis2_env_t *env,
        const axis2_char_t *_s, 
        const axis2_char_t *_trim)
{
    axis2_char_t *__tail;
    if(!_s)
        return NULL;
    __tail = (axis2_char_t *) _s + axis2_strlen(_s);
    if(!_trim)
        _trim = " \t\n\r";
    while(_s < __tail--)
    {
        if(!strchr(_trim, *__tail))
            return (axis2_char_t *) _s;
        *__tail = 0;
    }
    return AXIS2_STRDUP(_s, env);
}    

AXIS2_EXTERN axis2_char_t* AXIS2_CALL 
axis2_strtrim(
        const axis2_env_t *env,
        const axis2_char_t *_s, 
        const axis2_char_t *_trim)
{
    axis2_char_t *_p = NULL;
    axis2_char_t *_q = NULL;

    _p = axis2_strrtrim(env, _s, _trim);
    _q = axis2_strltrim(env, _p, _trim);
    AXIS2_FREE(env->allocator,_p);
    return _q;
}

