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

#ifndef AXIOM_MTOM_SENDING_CALLBACK_H 
#define AXIOM_MTOM_SENDING_CALLBACK_H

/**
  * @file axiom_mtom_sending_callback.h
  * @brief sending callback for attachment sending 
  */

/**
* @defgroup mtom_sending_callback
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
     * Type name for struct axiom_mtom_sending_callback_ops 
     */
    typedef struct axiom_mtom_sending_callback_ops axiom_mtom_sending_callback_ops_t;

    /**
     * Type name for struct axiom_mtom_sending_callback
     */
    typedef struct axiom_mtom_sending_callback axiom_mtom_sending_callback_t;


    /**
     * init_handler will init the attachment storage
     */

    /**
     * load will read the attachemnt by part from the storage
     */

    /**
     * close_handler will close the storage
     */

    struct axiom_mtom_sending_callback_ops
    {
        void* (AXIS2_CALL*
            init_handler)(axiom_mtom_sending_callback_t *mtom_sending_callback,
            const axutil_env_t* env,
            void *user_param);

        int (AXIS2_CALL*
            load_data)(axiom_mtom_sending_callback_t *mtom_sending_callback,
            const axutil_env_t* env,
            void *handler,
            axis2_char_t **buffer);

        axis2_status_t (AXIS2_CALL*
            close_handler)(axiom_mtom_sending_callback_t *mtom_sending_callback,
            const axutil_env_t* env,
            void *handler);

        axis2_status_t (AXIS2_CALL*
            free)(axiom_mtom_sending_callback_t *mtom_sending_callback,
            const axutil_env_t* env);
    };

    struct axiom_mtom_sending_callback
    {
        axiom_mtom_sending_callback_ops_t *ops;
		axutil_param_t *param;
    };

    /*************************** Function macros **********************************/
#define AXIOM_MTOM_SENDING_CALLBACK_INIT_HANDLER(mtom_sending_callback, env, user_param) \
        ((mtom_sending_callback)->ops->init_handler(mtom_sending_callback, env, user_param))

#define AXIOM_MTOM_SENDING_CALLBACK_LOAD_DATA(mtom_sending_callback, env, handler, buffer) \
        ((mtom_sending_callback)->ops->load_data(mtom_sending_callback, env, handler, buffer))

#define AXIOM_MTOM_SENDING_CALLBACK_CLOSE_HANDLER(mtom_sending_callback, env, handler) \
        ((mtom_sending_callback)->ops->close_handler(mtom_sending_callback, env, handler))

#define AXIOM_MTOM_SENDING_CALLBACK_FREE(mtom_sending_callback, env) \
        ((mtom_sending_callback)->ops->free(mtom_sending_callback, env))

    /** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIOM_MTOM_SENDING_CALLBACK */


