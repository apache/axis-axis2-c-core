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

#include <guththila_string.h>
#include <stdlib.h>
#include <string.h>


GUTHTHILA_DECLARE(void*) 
guththila_strdup (guththila_environment_t *environment,
                  const void *ptr)
{
    if (ptr)
    {
        int len = strlen(ptr);
        char* str = (char*) GUTHTHILA_MALLOC(environment->allocator,
                                             sizeof(char) * (len + 1 ));
        if (!str)
            return NULL;
        memcpy(str, ptr, len + 1);
        return (void *) str;
    }
    else
    {
        return NULL;
    }
}

GUTHTHILA_DECLARE(int) 
guththila_strcmp (const guththila_char_t * s1,
                  const guththila_char_t * s2)
{
    if (s1 && s2)
        return strcmp (s1, s2);
    else
        return -1;
}



GUTHTHILA_DECLARE(int)
guththila_strlen (const guththila_char_t * s)
{
    if (s)
    {
#ifdef __UNICODE__FUNCTIONS__

        return guththila_strlen_unicode (x);
#else
        return strlen (s);
#endif
    }
    else
        return -1;
}

GUTHTHILA_DECLARE(guththila_char_t *)
guththila_strndup (guththila_environment_t *environment,
                   const guththila_char_t * s1,
                   int len)
{
    if (s1)
    {
#ifdef __UNICODE__FUNCTIONS__

        return guththila_strdup_unicode (x, y);

#else
        register size_t n;
        register guththila_char_t *dst;

        n = guththila_strlen (s1);
        if (len < n)
            n = len;
        dst = (guththila_char_t *) GUTHTHILA_MALLOC(environment->allocator ,(n + 1));
        if (dst)
        {
            memcpy (dst, s1, n);
            dst[n] = '\0';
        }
        return dst;
#endif
    }
    else
        return NULL;
}
