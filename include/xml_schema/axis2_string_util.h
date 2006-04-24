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

#ifndef AXIS2_STRING_UTILS_H
#define AXIS2_STRING_UTILS_H

#include <axis2_array_list.h>
#include <axis2_string.h>
#ifdef __cplusplus
extern "C"
{
#endif

AXIS2_DECLARE(axis2_array_list_t *)
axis2_tokenize(axis2_env_t **env,
               axis2_char_t *in, 
               int delim);

AXIS2_DECLARE(axis2_array_list_t *)
axis2_first_token(axis2_env_t **env,
                  axis2_char_t *in,
                  int delim);

AXIS2_DECLARE(axis2_array_list_t *)
axis2_last_token(axis2_env_t **env,
                 axis2_char_t *in,
                 int delim);

#ifdef __cplusplus
}
#endif
#endif /* AXIS2_STRING_UTILS_H */
