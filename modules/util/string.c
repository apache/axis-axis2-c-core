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
#include <string.h>
#include <axis2.h>
#include <axis2_defines.h>
#include <ctype.h>

AXIS2_DECLARE(void*)
axis2_strdup (const void *ptr, axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    if (ptr)
    {
        int len = strlen(ptr);
        axis2_char_t * str = (axis2_char_t *) AXIS2_MALLOC( (*env)->allocator, 
            sizeof(axis2_char_t) * (len + 1 ));
        if (!str)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
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

AXIS2_DECLARE(axis2_char_t*)
axis2_stracat(const axis2_char_t *s1, const axis2_char_t *s2, axis2_env_t **env)
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
    ret = (axis2_char_t*)AXIS2_MALLOC((*env)->allocator,
                    alloc_len*sizeof(axis2_char_t));
    memcpy(ret, s1, axis2_strlen(s1)*sizeof(axis2_char_t));
    memcpy((ret + axis2_strlen(s1)*sizeof(axis2_char_t)), s2,
                    axis2_strlen(s2)*sizeof(axis2_char_t));
    ret[alloc_len*sizeof(axis2_char_t) - sizeof(axis2_char_t)] = '\0';
    return ret;
}

AXIS2_DECLARE(int)
axis2_strcmp (const axis2_char_t * s1, const axis2_char_t * s2)
{
    if (s1 && s2)
        return strcmp (s1, s2);
    else
        return -1;
}


AXIS2_DECLARE(axis2_ssize_t)
axis2_strlen (const axis2_char_t * s)
{
	return strlen(s);
}


AXIS2_DECLARE(int)
axis2_strcasecmp(const axis2_char_t *s1, axis2_char_t *s2)
{
    while (toupper(*s1) == toupper(*s2++))
	if (*s1++ == '\0')
	    return(0);
    return(toupper(*s1) - toupper(*--s2));
}


AXIS2_DECLARE(int)
axis2_strncasecmp(const axis2_char_t *s1, axis2_char_t *s2, int n)
{
    while (--n >= 0 && toupper(*s1) == toupper(*s2++))
	if (toupper(*s1++) == '\0')
	    return(0);
    return(n < 0 ? 0 : toupper(*s1) - toupper(*--s2));
}

AXIS2_DECLARE(axis2_char_t *)
axis2_strstr (const axis2_char_t *heystack,
                const axis2_char_t *needle)
{
	return strstr(heystack, needle);
}


AXIS2_DECLARE(axis2_char_t *)
axis2_rindex(const axis2_char_t *_s, axis2_char_t _ch)
{
        int i,ilen = strlen(_s);
        if (ilen < 1)
                return NULL;
        for (i=ilen-1;i>=0;i--)
        {
                if (_s[i] == _ch)
                        return _s+i;
        }
        return NULL;
}
