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

#ifndef AXIOM_CACHING_CALLBACK_H 
#define AXIOM_CACHING_CALLBACK_H

/**
  * @file axiom_caching_callback.h
  * @brief Caching callback for mime parser 
  */

/**
* @defgroup caching_callback 
* @ingroup axiom
* @{
*/

#include <axutil_env.h>
#include <axutil_param.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Type name for struct axiom_caching_callback_ops 
     */
    typedef struct axiom_caching_callback_ops axiom_caching_callback_ops_t;

    /**
     * Type name for struct axiom_caching_callback
     */
    typedef struct axiom_caching_callback axiom_caching_callback_t;


    /**
     * init_handler will init the caching storage
     */

    /**
     * cache will write the data to the storage
     */

    /**
     * close_handler will close the storage
     */

    struct axiom_caching_callback_ops
    {
        void* (AXIS2_CALL*
            init_handler)(axiom_caching_callback_t *caching_callback,
            const axutil_env_t* env);

        axis2_status_t (AXIS2_CALL*
            cache)(axiom_caching_callback_t *caching_callback,
            const axutil_env_t* env,
            axis2_char_t *data,
            int length,
            void *handler);

        axis2_status_t (AXIS2_CALL*
            close_handler)(axiom_caching_callback_t *caching_callback,
            const axutil_env_t* env,
            void *handler);

        axis2_status_t (AXIS2_CALL*
            free)(axiom_caching_callback_t *caching_callback,
            const axutil_env_t* env);
    };

    struct axiom_caching_callback
    {
        axiom_caching_callback_ops_t *ops;
		axutil_param_t *param;
    };

    /*************************** Function macros **********************************/
#define AXIOM_CACHING_CALLBACK_INIT_HANDLER(caching_callback, env) \
        ((caching_callback)->ops->init_handler(caching_callback, env))

#define AXIOM_CACHING_CALLBACK_CACHE(caching_callback, env, data, length, handler) \
        ((caching_callback)->ops->cache(caching_callback, env, data, length, handler))

#define AXIOM_CACHING_CALLBACK_CLOSE_HANDLER(caching_callback, env, handler) \
        ((caching_callback)->ops->close_handler(caching_callback, env, handler))

#define AXIOM_CACHING_CALLBACK_FREE(caching_callback, env) \
        ((caching_callback)->ops->free(caching_callback, env))

    /** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIOM_CACHING_CALLBACK */


