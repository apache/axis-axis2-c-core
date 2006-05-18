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

#ifndef GUTHTHILA_STRING_H
#define GUTHTHILA_STRING_H

#include <guththila_defines.h>
#include <guththila_allocator.h>
#include "guththila_environment.h"

#ifdef __cplusplus
extern "C"
{
#endif


GUTHTHILA_DECLARE(void*)
guththila_strdup(guththila_environment_t *environment, const void *ptr);

GUTHTHILA_DECLARE(int)
guththila_strcmp(const guththila_char_t * s1,
                 const guththila_char_t * s2);
                 
GUTHTHILA_DECLARE(guththila_char_t *)
guththila_strndup(guththila_environment_t *environment,                 
                  const guththila_char_t *s1, int n);
                  
GUTHTHILA_DECLARE(int)
guththila_strlen(const guththila_char_t * s);                


#define GUTHTHILA_STRDUP(env, ptr) guththila_strdup(env, ptr)

#define GUTHTHILA_STRCMP(s1, s2) guththila_strcmp( s1, s2)

#define GUTHTHILA_STRNDUP(env, s1, n) guththila_strndup(env, s1, n)

#define GUTHTHILA_STRLEN( s1) guththila_strlen( s1)

#ifdef __cplusplus
}
#endif

#endif                          /* GUTHTHILA_STRING_H */
