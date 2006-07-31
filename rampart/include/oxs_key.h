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

#ifndef OXS_KEY_H
#define OXS_KEY_H


/**
  * @file oxs_key.h
  * @brief 
  */

#include <axis2_defines.h>
#include <oxs_constants.h>
#include <axis2_env.h>
#ifdef __cplusplus
extern "C"
{
#endif


#define OXS_KEY_USAGE_SIGN      0
#define OXS_KEY_USAGE_VERIFY    1
#define OXS_KEY_USAGE_ENCRYPT   2
#define OXS_KEY_USAGE_DECRYPT   3
#define OXS_KEY_USAGE_NONE      4

#define OXS_KEY_DEFAULT_SIZE    64

typedef struct _oxs_key oxs_key, *oxs_key_ptr;

struct _oxs_key{
    axis2_char_t *name;
    unsigned char *data;
    int            size;
    int            usage;
};

AXIS2_EXTERN oxs_key_ptr AXIS2_CALL
oxs_key_create_key(const axis2_env_t *env,
                    axis2_char_t *name,
                    unsigned char *data,
                    int size,
                    int usage
                    );

AXIS2_EXTERN oxs_key_ptr AXIS2_CALL
oxs_key_read_from_file(const axis2_env_t *env,
                        axis2_char_t *file_name);




/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_KEY_H */
