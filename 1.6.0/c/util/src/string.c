
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

#include <axutil_string.h>
#include <stdlib.h>
#include <ctype.h>
#include <axutil_utils.h>
#include <axutil_utils_defines.h>
#include <stdarg.h>             /* NULL */

struct axutil_string
{
    axis2_char_t *buffer;
    unsigned int length;
    unsigned int ref_count;
    axis2_bool_t owns_buffer;
};

AXIS2_EXTERN axutil_string_t *AXIS2_CALL
axutil_string_create(
    const axutil_env_t *env,
    const axis2_char_t *str)
{
    axutil_string_t *string = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    /* str can't be null */
    if (!str)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "NULL parameter was passed when a non NULL parameter was expected");
        return NULL;
    }

    string =
        (axutil_string_t *) AXIS2_MALLOC(env->allocator,
                                         sizeof(axutil_string_t));
    if (!string)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    /* set properties */
    string->buffer = NULL;
    string->ref_count = 1;
    string->owns_buffer = AXIS2_TRUE;

    string->length = axutil_strlen(str);

    if (string->length < 0)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "NULL parameter was passed when a non NULL parameter was expected");
        axutil_string_free(string, env);
        return NULL;
    }

    string->buffer =
        (axis2_char_t *) AXIS2_MALLOC(env->allocator,
                                      sizeof(axis2_char_t) * (string->length +
                                                              1));
    if (!(string->buffer))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        axutil_string_free(string, env);
        return NULL;
    }
    memcpy(string->buffer, str, string->length + 1);

    return string;
}

AXIS2_EXTERN axutil_string_t *AXIS2_CALL
axutil_string_create_assume_ownership(
    const axutil_env_t *env,
    axis2_char_t **str)
{
    axutil_string_t *string = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    /* str can't be null */
    if (!str || !(*str))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "NULL parameter was passed when a non NULL parameter was expected");
        return NULL;
    }

    string =
        (axutil_string_t *) AXIS2_MALLOC(env->allocator,
                                         sizeof(axutil_string_t));
    if (!string)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    /* set properties */
    string->buffer = *str;
    string->length = axutil_strlen(*str);
    string->ref_count = 1;
    string->owns_buffer = AXIS2_TRUE;

    if (string->length < 0)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "NULL parameter was passed when a non NULL parameter was expected");
        axutil_string_free(string, env);
        return NULL;
    }

    return string;
}

AXIS2_EXTERN axutil_string_t *AXIS2_CALL
axutil_string_create_const(
    const axutil_env_t *env,
    axis2_char_t **str)
{
    axutil_string_t *string = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    /* str can't be null */
    if (!str)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "NULL parameter was passed when a non NULL parameter was expected");

        return NULL;
    }

    string =
        (axutil_string_t *) AXIS2_MALLOC(env->allocator,
                                         sizeof(axutil_string_t));
    if (!string)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    /* set properties */
    string->buffer = *str;
    string->length = axutil_strlen(*str);
    string->ref_count = 1;
    string->owns_buffer = AXIS2_FALSE;

    if (string->length < 0)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "NULL parameter was passed when a non NULL parameter was expected");
        axutil_string_free(string, env);
        return NULL;
    }

    return string;
}

AXIS2_EXTERN void AXIS2_CALL
axutil_string_free(
    struct axutil_string *string,
    const axutil_env_t *env)
{
    if (!string)
    {
        return;
    }

    string->ref_count--;

    if (string->ref_count > 0)
    {
        return;
    }

    if (string->owns_buffer && string->buffer)
    {
        AXIS2_FREE(env->allocator, string->buffer);
    }

    AXIS2_FREE(env->allocator, string);
    return;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axutil_string_equals(
    const struct axutil_string *string,
    const axutil_env_t * env,
    const struct axutil_string *string1)
{
    if (!string || !string1)
    {
        return AXIS2_FALSE;
    }

    return (string->buffer == string1->buffer);
}

AXIS2_EXTERN struct axutil_string *AXIS2_CALL
axutil_string_clone(
    struct axutil_string *string,
    const axutil_env_t *env)
{
    if (!string)
    {
        return NULL;
    }

    string->ref_count++;

    return string;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axutil_string_get_buffer(
    const struct axutil_string *string,
    const axutil_env_t *env)
{
    if (!string)
    {
        return NULL;
    }

    return string->buffer;
}

AXIS2_EXTERN unsigned int AXIS2_CALL
axutil_string_get_length(
    const struct axutil_string *string,
    const axutil_env_t *env)
{
    int error_return = -1;
    if (!string)
    {
        return error_return;
    }

    return string->length;
}

/* END of string struct implementation */

/** this is used to cache lengths in axutil_strcat */
#define MAX_SAVED_LENGTHS  6

AXIS2_EXTERN void *AXIS2_CALL
axutil_strdup(
    const axutil_env_t *env,
    const void *ptr)
{
    if (ptr)
    {
        int len = axutil_strlen(ptr);
        axis2_char_t *str = (axis2_char_t *) AXIS2_MALLOC(env->allocator,
                                                          sizeof(axis2_char_t) *
                                                          (len + 1));
        if (!str)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
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

AXIS2_EXTERN void *AXIS2_CALL
axutil_strmemdup(
    const void *ptr,
    size_t n,
    const axutil_env_t *env)
{
    axis2_char_t *str;

    AXIS2_PARAM_CHECK(env->error, ptr, NULL);

    str =
        (axis2_char_t *) AXIS2_MALLOC(env->allocator,
                                      sizeof(axis2_char_t) * (n + 1));
    if (!str)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    memcpy(str, ptr, n);
    str[n] = '\0';

    return (void *) str;
}

AXIS2_EXTERN void *AXIS2_CALL
axutil_memchr(
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

AXIS2_EXTERN void *AXIS2_CALL
axutil_strndup(
    const axutil_env_t *env,
    const void *ptr,
    int n)
{
    const axis2_char_t *end;
    axis2_char_t *str;

    AXIS2_PARAM_CHECK(env->error, ptr, NULL);

    end = axutil_memchr(ptr, '\0', n);
    if (end)
        n = (int)(end - (axis2_char_t *) ptr);
        /* We are sure that the difference lies within the int range */
    str =
        (axis2_char_t *) AXIS2_MALLOC(env->allocator,
                                      sizeof(axis2_char_t) * (n + 1));
    if (!str)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    memcpy(str, ptr, n);
    str[n] = '\0';

    return (void *) str;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_strcat(
    const axutil_env_t *env,
    ...)
{
    axis2_char_t *cp,
    *argp,
    *str;
    size_t saved_lengths[MAX_SAVED_LENGTHS];
    int nargs = 0;
    int str_len = 0;

    /* Pass one --- find length of required string */

    size_t len = 0;
    va_list adummy;

    va_start(adummy, env);

    cp = va_arg(adummy, axis2_char_t *);
    while (cp)
    {
        size_t cplen = strlen(cp);
        if (nargs < MAX_SAVED_LENGTHS)
        {
            saved_lengths[nargs++] = cplen;
        }
        len += cplen;
        cp = va_arg(adummy, axis2_char_t *);
    }

    va_end(adummy);

    /* Allocate the required string */
    str_len = (int)(sizeof(axis2_char_t) * (len + 1));
    /* We are sure that the difference lies within the int range */
    str = (axis2_char_t *) AXIS2_MALLOC(env->allocator, str_len);
    if (!str)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    cp = str;

    /* Pass two --- copy the argument strings into the result space */

    va_start(adummy, env);

    nargs = 0;
    argp = va_arg(adummy, axis2_char_t *);
    while (argp)
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
        argp = va_arg(adummy, axis2_char_t *);
    }

    va_end(adummy);

    /* Return the result string */

    *cp = '\0';
    return str;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_stracat(
    const axutil_env_t *env,
    const axis2_char_t *s1,
    const axis2_char_t *s2)
{
    axis2_char_t *ret = NULL;
    int alloc_len = -1;
    int len1 = 0;
    int len2 = 0;

    if (!s1 && !s2)
    {
        return NULL;
    }
    if (!s1)
    {
        return (axis2_char_t *) axutil_strdup(env, s2);
    }
    if (!s2)
    {
        return (axis2_char_t *) axutil_strdup(env, s1);
    }

    len1 = axutil_strlen(s1);
    len2 = axutil_strlen(s2);
    alloc_len = len1 + len2 + 1;
    ret =
        (axis2_char_t *) AXIS2_MALLOC(env->allocator,
                                      alloc_len * sizeof(axis2_char_t));
    memcpy(ret, s1, len1 * sizeof(axis2_char_t));
    memcpy((ret + len1 * sizeof(axis2_char_t)), s2,
           len2 * sizeof(axis2_char_t));
    ret[alloc_len * sizeof(axis2_char_t) - sizeof(axis2_char_t)] = '\0';
    return ret;
}

AXIS2_EXTERN int AXIS2_CALL
axutil_strcmp(
    const axis2_char_t *s1,
    const axis2_char_t *s2)
{
    if (s1 && s2)
    {
        return strcmp(s1, s2);
    }
    else
    {
        return -1;
    }
}

AXIS2_EXTERN int AXIS2_CALL
axutil_strncmp(
    const axis2_char_t *s1,
    const axis2_char_t *s2,
    int n)
{
    if (s1 && s2)
    {
        return strncmp(s1, s2, n);
    }
    else
    {
        return -1;
    }
}

AXIS2_EXTERN axis2_ssize_t AXIS2_CALL
axutil_strlen(
    const axis2_char_t *s)
{
    int error_return = -1;
    if (s)
    {
        return (axis2_ssize_t)strlen(s);
        /* We are sure that the difference lies within the axis2_ssize_t range */
    }
    return error_return;
}

AXIS2_EXTERN int AXIS2_CALL
axutil_strcasecmp(
    const axis2_char_t *s1,
    const axis2_char_t *s2)
{
    while (*s1 != '\0' && *s2 != '\0')
    {
        if (*s1 >= 'A' && *s1 <= 'Z' && *s2 >= 'a' && *s2 <= 'z')
        {
            if (*s2 - *s1 - (char) 32 != 0)
            {
                return 1;
            }
        }
        else if (*s1 >= 'a' && *s1 <= 'z' && *s2 >= 'A' && *s2 <= 'Z')
        {
            if (*s1 - *s2 - 32 != 0)
            {
                return 1;
            }
        }
        else if (*s1 - *s2 != 0)
        {
            return 1;
        }
        s1++;
        s2++;
    }
    if (*s1 != *s2)
    {
        return 1;
    }

    return 0;
}

AXIS2_EXTERN int AXIS2_CALL
axutil_strncasecmp(
    const axis2_char_t *s1,
    const axis2_char_t *s2,
    const int n)
{
    axis2_char_t *str1 = (axis2_char_t *) s1,
        *str2 = (axis2_char_t *) s2;
    int i = (int) n;

    while (--i >= 0 && toupper((int)*str1) == toupper((int)*str2++))
    {
        if (toupper((int)*str1++) == '\0')
        {
            return (0);
        }
    }
    return (i < 0 ? 0 : toupper((int)*str1) - toupper((int)*--str2));
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_strstr(
    const axis2_char_t *haystack,
    const axis2_char_t *needle)
{
    return strstr(haystack, needle);
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_strchr(
    const axis2_char_t *s,
    axis2_char_t ch)
{
    return (axis2_char_t *) strchr(s, ch);
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_rindex(
    const axis2_char_t *_s,
    axis2_char_t _ch)
{
    int i,
     ilen = axutil_strlen(_s);
    if (ilen < 1)
    {
        return NULL;
    }
    for (i = ilen - 1; i >= 0; i--)
    {
        if (_s[i] == _ch)
        {
            return (axis2_char_t *) (_s + i);
        }
    }
    return NULL;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_replace(
    const axutil_env_t *env,
    axis2_char_t *str,
    int s1,
    int s2)
{
    axis2_char_t *newstr = NULL;
    axis2_char_t *index = NULL;
    if (!str)
    {
        return NULL;
    }

    newstr = axutil_strdup(env, str);
    index = strchr(newstr, s1);
    while (index)
    {
        newstr[index - newstr] = (axis2_char_t)s2;
        /* We are sure that the conversion is safe */
        index = strchr(newstr, s1);
    }
    return newstr;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_strltrim(
    const axutil_env_t *env,
    const axis2_char_t *_s,
    const axis2_char_t *_trim)
{
    axis2_char_t *_p = NULL;
    axis2_char_t *ret = NULL;

    if (!_s)
    {
        return NULL;
    }
    _p = (axis2_char_t *) _s;
    if (!_trim)
    {
        _trim = " \t\r\n";
    }

    while (*_p)
    {
        if (!strchr(_trim, *_p))
        {
            ret = (axis2_char_t *) axutil_strdup(env, _p);
            break;
        }
        ++_p;
    }
    return ret;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_strrtrim(
    const axutil_env_t *env,
    const axis2_char_t *_in,
    const axis2_char_t *_trim)
{
    axis2_char_t *__tail;
    axis2_char_t *_s = NULL;
    axis2_char_t *ret = NULL;

    if (_in)
    {
        _s = axutil_strdup(env, _in);
    }
    if (!_s)
    {
        return NULL;
    }
    __tail = _s + axutil_strlen(_s);
    if (!_trim)
    {
        _trim = " \t\n\r";
    }
    while (_s < __tail--)
    {
        if (!strchr(_trim, *__tail))
        {
            ret = _s;
            break;
        }
        *__tail = 0;
    }
    if (!ret && _s)
    {
        AXIS2_FREE(env->allocator, _s);
    }
    return ret;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_strtrim(
    const axutil_env_t *env,
    const axis2_char_t *_s,
    const axis2_char_t *_trim)
{
    axis2_char_t *_p = NULL;
    axis2_char_t *_q = NULL;

    _p = axutil_strltrim(env, _s, _trim);
    _q = axutil_strrtrim(env, _p, _trim);
    if (_p)
    {
        AXIS2_FREE(env->allocator, _p);
    }
    return _q;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_string_replace(
    axis2_char_t *str,
    axis2_char_t old,
    axis2_char_t new)
{
    axis2_char_t *str_returns = str;
    for (; *str != '\0'; str++)
    {
        if (*str == old)
        {
            *str = new;
        }
    }
    return str_returns;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_string_substring_starting_at(
    axis2_char_t *str,
    int s)
{
    int len;
    int pos_to_shift;

    len = (int)strlen(str);
    /* We are sure that the difference lies within the int range */
    pos_to_shift = len - s + 1;

    if (len <= s)
    {
        return NULL;
    }
    memmove(str, str + s, pos_to_shift);
    return str;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_string_substring_ending_at(
    axis2_char_t *str,
    int e)
{
    axis2_char_t *ptr = NULL;
    int length = 0;

    length = (int)strlen(str);
    /* We are sure that the difference lies within the int range */
    ptr = str;
    if (length <= e)
    {
        return NULL;
    }
    ptr += e;
    *ptr = '\0';
    return str;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_string_tolower(
    axis2_char_t *str)
{
    axis2_char_t *temp_str = NULL;
    for (temp_str = str; *temp_str != '\0'; temp_str++)
    {
        *temp_str = (axis2_char_t)tolower((int)*temp_str);
        /* We are sure that the conversion is safe */
    }
    return str;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_string_toupper(
    axis2_char_t *str)
{
    axis2_char_t *temp_str = NULL;
    for (temp_str = str; *temp_str != '\0'; temp_str++)
    {
        *temp_str = (axis2_char_t)toupper((int)*temp_str);
        /* We are sure that the conversion is safe */
    }
    return str;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_strcasestr(
    const axis2_char_t *haystack,
    const axis2_char_t *needle)
{
    axis2_char_t start,
     current;
    size_t len;

    if (!haystack || !needle)
    {
        return NULL;
    }

    if ((start = *needle++))
    {
        len = strlen(needle);
        do
        {
            do
            {
                if (!(current = *haystack++))
                {
                    return NULL;
                }
            }
            while (toupper((int)current) != toupper((int)start));
        }
        while (axutil_strncasecmp(haystack, needle, (int)len));
        /* We are sure that the difference lies within the int range */
        haystack--;
    }
    return (axis2_char_t *) haystack;
}
