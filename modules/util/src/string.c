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

AXIS2_DECLARE(void*)
axis2_strdup (const void *ptr, axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(ptr, env, NULL);
    if (ptr)
    {
        int len = strlen(ptr);
        char* str = (char*) AXIS2_MALLOC( (*env)->allocator, sizeof(char) * (len + 1 ));
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
        return (axis2_char_t)AXIS2_STRDUP(s1, env);
    }
    alloc_len = axis2_strlen(s1) + axis2_strlen(s2) + 1;
    ret = (axis2_char_t*)AXIS2_MALLOC((*env)->allocator,
                    alloc_len*sizeof(axis2_char_t));
    memcpy(ret, s1, axis2_strlen(s1)*sizeof(axis2_char_t));
    memcpy((ret + axis2_strlen(s1)*sizeof(axis2_char_t)), s2,
                    axis2_strlen(s2)*sizeof(axis2_char_t));
    ret[alloc_len*sizeof(axis2_char_t)] = '\0';
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

AXIS2_DECLARE(int)
axis2_strlen(const axis2_char_t *s)
{
    if(s)
        return strlen(s);
    else
        return -1;
}
