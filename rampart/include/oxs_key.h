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

    /** Type name for struct  oxs_key_ops */
    typedef struct oxs_key_ops oxs_key_ops_t;

    /** Type name for struct  oxs_key */
    typedef struct oxs_key oxs_key_t;

    struct oxs_key_ops
    {
        axis2_char_t *(AXIS2_CALL *
                get_data)(
                    const oxs_key_t *key,
                    const axis2_env_t *env);
        axis2_char_t *(AXIS2_CALL *
                get_name)(
                    const oxs_key_t *key,
                    const axis2_env_t *env);
        int (AXIS2_CALL *
                get_size)(
                    const oxs_key_t *key,
                    const axis2_env_t *env);
        int (AXIS2_CALL *
                get_usage)(
                    const oxs_key_t *key,
                    const axis2_env_t *env);
        
        axis2_status_t (AXIS2_CALL *
                set_data)(
                    oxs_key_t *key,
                    const axis2_env_t *env,
                    axis2_char_t *data);

        axis2_status_t (AXIS2_CALL *
                set_name)(
                    oxs_key_t *key,
                    const axis2_env_t *env,
                    axis2_char_t *name);

        axis2_status_t (AXIS2_CALL *
                set_size)(
                    oxs_key_t *key,
                    const axis2_env_t *env,
                    int size);

        axis2_status_t (AXIS2_CALL *
                set_usage)(
                    oxs_key_t *key,
                    const axis2_env_t *env,
                    int usage);

        axis2_status_t (AXIS2_CALL *
                free)(
                    oxs_key_t *key,
                    const axis2_env_t *env
                );

        axis2_status_t (AXIS2_CALL *
                populate)(
                    oxs_key_t *key,
                    const axis2_env_t *env,
                    axis2_char_t *data,
                    axis2_char_t *name,
                    int size,
                    int usage
                );
        
        axis2_status_t (AXIS2_CALL *
                read_from_file)(
                    oxs_key_t *key,
                    const axis2_env_t *env,
                    axis2_char_t *file_name
                );
        
        axis2_status_t (AXIS2_CALL *
                for_algo)(
                    oxs_key_t *key,
                    const axis2_env_t *env,
                    axis2_char_t *key_algo
                );
    };

    struct oxs_key
    {
        oxs_key_ops_t *ops;
    };

AXIS2_EXTERN oxs_key_t *AXIS2_CALL
oxs_key_create_key(const axis2_env_t *env);

/*Macros*/


#define OXS_KEY_GET_DATA(key,env)\
    ((key)->ops->get_data(key,env))

#define OXS_KEY_GET_NAME(key,env)\
    ((key)->ops->get_name(key,env))

#define OXS_KEY_GET_SIZE(key,env)\
    ((key)->ops->get_size(key,env))

#define OXS_KEY_GET_USAGE(key,env)\
    ((key)->ops->get_usage(key,env))

#define OXS_KEY_SET_DATA(key,env, data)\
    ((key)->ops->set_data(key, env, data))

#define OXS_KEY_SET_NAME(key,env, name)\
    ((key)->ops->set_name(key, env, name))

#define OXS_KEY_SET_SIZE(key,env, size)\
    ((key)->ops->set_size(key, env, size))

#define OXS_KEY_SET_USAGE(key,env, usage)\
    ((key)->ops->set_usage(key, env, usage))

#define OXS_KEY_FREE(key,env)\
    ((key)->ops->free(key, env))

#define OXS_KEY_POPULATE(key,env, data, name, size, usage)\
    ((key)->ops->populate(key, env, data, name, size, usage))

#define OXS_KEY_READ_FROM_FILE(key,env, file_name)\
    ((key)->ops->read_from_file(key, env, file_name))

#define OXS_KEY_FOR_ALGO(key,env, key_algo)\
    ((key)->ops->for_algo(key, env, key_algo))

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_KEY_H */
