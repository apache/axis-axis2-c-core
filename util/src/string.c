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

#include <axis2_string.h>
#include <stdlib.h>
#include <axis2_utils.h>
#include <axis2_utils_defines.h>
#include <stdarg.h> /* NULL */

/** this is used to cache lengths in axis2_strcat */
#define MAX_SAVED_LENGTHS  6

AXIS2_EXTERN void* AXIS2_CALL
axis2_strdup(const void *ptr, const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    if (ptr)
    {
        int len = axis2_strlen(ptr);
        axis2_char_t * str = (axis2_char_t *) AXIS2_MALLOC(env->allocator,
                sizeof(axis2_char_t) * (len + 1));
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

    str = (axis2_char_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axis2_char_t) * (n + 1));
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
axis2_memchr(
    const void *ptr,
    int c,
    size_t n)
{
    const axis2_char_t *cp;

    for (cp = ptr; n > 0; n--, cp++)
    {
        if (*cp == c)
            return (char *) cp; /* Casting away the const here */
    }

    return NULL;
}

AXIS2_EXTERN void* AXIS2_CALL
axis2_strndup(
    const void *ptr,
    int n,
    const axis2_env_t *env)
{
    const axis2_char_t *end;
    axis2_char_t *str;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ptr, NULL);

    end = axis2_memchr(ptr, '\0', n);
    if (end)
        n = end - (axis2_char_t *) ptr;
    str = (axis2_char_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axis2_char_t) * (n + 1));
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
    int str_len = 0;

    /* Pass one --- find length of required string */

    size_t len = 0;
    va_list adummy;

    va_start(adummy, env);

    while ((cp = va_arg(adummy, axis2_char_t *)))
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
    str_len = sizeof(axis2_char_t) * (len + 1);
    str = (axis2_char_t *) AXIS2_MALLOC(env->allocator, str_len);
    if (!str)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    cp = str;

    /* Pass two --- copy the argument strings into the result space */

    va_start(adummy, env);

    nargs = 0;
    while ((argp = va_arg(adummy, axis2_char_t *)))
    {
        if (nargs < MAX_SAVED_LENGTHS)
        {
            len = saved_lengths[nargs++];
        }
        else
        {
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

    if (NULL == s1 && NULL == s2)
    {
        return NULL;
    }
    if (NULL == s1)
    {
        return (axis2_char_t*)AXIS2_STRDUP(s2, env);
    }
    if (NULL == s2)
    {
        return (axis2_char_t*)AXIS2_STRDUP(s1, env);
    }
    alloc_len = axis2_strlen(s1) + axis2_strlen(s2) + 1;
    ret = (axis2_char_t*)AXIS2_MALLOC(env->allocator,
            alloc_len * sizeof(axis2_char_t));
    memcpy(ret, s1, axis2_strlen(s1)*sizeof(axis2_char_t));
    memcpy((ret + axis2_strlen(s1)*sizeof(axis2_char_t)), s2,
            axis2_strlen(s2)*sizeof(axis2_char_t));
    ret[alloc_len*sizeof(axis2_char_t) - sizeof(axis2_char_t)] = '\0';
    return ret;
}

AXIS2_EXTERN int AXIS2_CALL
axis2_strcmp(const axis2_char_t * s1, const axis2_char_t * s2)
{
    if (s1 && s2)
        return strcmp(s1, s2);
    else
        return -1;
}


AXIS2_EXTERN int AXIS2_CALL
axis2_strncmp(const axis2_char_t * s1, const axis2_char_t * s2, int n)
{
    if (s1 && s2)
        return strncmp(s1, s2, n);
    else
        return -1;
}


AXIS2_EXTERN axis2_ssize_t AXIS2_CALL
axis2_strlen(const axis2_char_t * s)
{
    if (s)
        return strlen(s);
    else
        return -1;
}


AXIS2_EXTERN int AXIS2_CALL
axis2_strcasecmp(const axis2_char_t *s1, const axis2_char_t *s2)
{
	while (*s1 != '\0' && *s2 != '\0'){
		if(*s1 >= 'A' && *s1 <= 'Z' && *s2 >= 'a' && *s2 <= 'z'){
			if (*s2 - *s1 - (char)32 != 0){ return 1;} 
		}
		else if(*s1 >= 'a' && *s1 <= 'z' && *s2 >= 'A' && *s2 <= 'Z'){
			if (*s1 - *s2 - 32 != 0) {return 1;}	
		}
		else if (*s1 - *s2 != 0)
			return 1;

		s1++; s2++;
	}
	if (*s1 != *s2)
		return 1;

	return 0;
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
axis2_strstr(const axis2_char_t *heystack,
        const axis2_char_t *needle)
{
    return strstr(heystack, needle);
}


AXIS2_EXTERN axis2_char_t * AXIS2_CALL
axis2_rindex(const axis2_char_t *_s, axis2_char_t _ch)
{
    int i, ilen = axis2_strlen(_s);
    if (ilen < 1)
        return NULL;
    for (i = ilen - 1;i >= 0;i--)
    {
        if (_s[i] == _ch)
            return (axis2_char_t *)(_s + i);
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
    if (!str)
        return NULL;

    newstr = AXIS2_STRDUP(str, env);

    index = strchr(newstr, s1);
    while (index)
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
    axis2_char_t *ret = NULL;

    if (!_s)
        return NULL;
    _p = (axis2_char_t *) _s;
    if (!_trim)
        _trim = " \t\r\n";

    while (*_p)
    {
        if (!strchr(_trim, *_p))
        {
            ret = (axis2_char_t *) AXIS2_STRDUP(_p, env);
            break;
        }
        ++_p;
    }
    return ret;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_strrtrim(
    const axis2_env_t *env,
    const axis2_char_t *_s,
    const axis2_char_t *_trim)
{
    axis2_char_t *__tail;
    axis2_char_t *ret = NULL;

    if (!_s)
        return NULL;
    __tail = ((axis2_char_t *) _s) + axis2_strlen(_s);
    if (!_trim)
        _trim = " \t\n\r";
    while (_s < __tail--)
    {
        if (!strchr(_trim, *__tail))
        {
            ret = (axis2_char_t *) AXIS2_STRDUP(_s, env);
            break;
        }
        *__tail = 0;
    }
    return ret;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_strtrim(
    const axis2_env_t *env,
    const axis2_char_t *_s,
    const axis2_char_t *_trim)
{
    axis2_char_t *_p = NULL;
    axis2_char_t *_q = NULL;

    _p = axis2_strltrim(env, _s, _trim);
    _q = axis2_strrtrim(env, _p, _trim);
    if (_p)
        AXIS2_FREE(env->allocator, _p);
    return _q;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_string_replace(axis2_char_t* str, axis2_char_t old, axis2_char_t new)
{
    axis2_char_t* str_returns = str;
    for (; *str != '\0' ; str ++)
    {
        if (*str == old)
        {
            *str = new;
        }
    }
    return str_returns;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_string_substring_starting_at(axis2_char_t* str, int s)
{
    int len;
    int pos_to_shift;

    len = strlen(str);
    pos_to_shift = len - s + 1;

    if (len <= s)
    {
        return NULL;
    }
    memmove(str , str + s, pos_to_shift);
    return str;
}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_string_substring_ending_at(axis2_char_t* str, int e)
{
    axis2_char_t* ptr = NULL;
    int length = 0;

    length = strlen(str);
    ptr = str;
    if (length <=  e)
    {
        return NULL;
    }
    ptr += e;
    *ptr = '\0';
    return str;
}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_string_tolower(axis2_char_t* str)
{
    axis2_char_t* temp_str = NULL;
    for (temp_str = str; *temp_str != '\0' ; temp_str ++)
    {
        *temp_str = tolower(*temp_str);
    }
    return str;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_string_toupper(axis2_char_t* str)
{
    axis2_char_t* temp_str = NULL;
    for (temp_str = str; *temp_str != '\0' ; temp_str ++)
    {
        *temp_str = toupper(*temp_str);
    }
    return str;
}

AXIS2_EXTERN axis2_char_t * AXIS2_CALL
axis2_strcasestr(const axis2_char_t *heystack, const axis2_char_t *needle)
{
    axis2_char_t start, current;
    size_t len;

    if (!heystack || !needle)
        return NULL;

    if ((start = *needle++))
    {
        len = strlen(needle);
        do
        {
            do
            {
                if (!(current = *heystack++))
                    return NULL;
            } while (toupper(current) != toupper(start));
        } while (axis2_strncasecmp(heystack, needle, len));
        heystack--;
    }
    return (axis2_char_t *)heystack;
}

