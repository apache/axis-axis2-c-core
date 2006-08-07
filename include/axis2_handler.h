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

#ifndef AXIS2_HANDLER_H
#define AXIS2_HANDLER_H

/**
 * @defgroup axis2_handler handler
 * @ingroup axis2_desc
 * Description.
 * @{
 */

/**
 * @file axis2_handler.h
 * @brief Axis2 handler interface
 */

#include <axis2_defines.h>
#include <axis2_qname.h>
#include <axis2_param.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_handler */
    typedef struct axis2_handler axis2_handler_t;
    /** Type name for struct axis2_handler_ops */
    typedef struct axis2_handler_ops axis2_handler_ops_t;
    struct axis2_handler_desc;
    struct axis2_msg_ctx;


    /**
     * Handler ops struct
     */
    struct axis2_handler_ops
    {
        /**
         * Free an axis2_handler struct
         * @param env Environment. MUST NOT be NULL, if NULL behaviour is undefined.
         * @param axis2_handler pointer to axis2_handler struct to be freed
         * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    struct axis2_handler *handler,
                    const axis2_env_t *env);


        /**
         * @param handler pointer to handler
         * @param env pointer to environment struct
         * @param handler_desc pointer to handler description
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                init)(
                    struct axis2_handler *handler,
                    const axis2_env_t *env,
                    struct axis2_handler_desc *handler_desc);

        /**
         * Invoke is called to do the actual work of the Handler object.
         * If there is a fault during the processing of this method it is
         * invoke's job to report the error and undo any partial work
         * that has been completed. 
         * @param handler pointer to handler
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                invoke)(
                    struct axis2_handler *handler,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *msg_ctx);

        /**
         * @param handler pointer to handler
         * @param env pointer to environment struct
         */
        axis2_qname_t *(AXIS2_CALL *
                get_name)(
                    const axis2_handler_t *handler,
                    const axis2_env_t *env);

        /**
         * @param handler pointer to handler
         * @param env pointer to environment struct
         * @param name pointer to name
         */
        axis2_param_t *(AXIS2_CALL *
                get_param)(
                    const axis2_handler_t *handler,
                    const axis2_env_t *env,
                    axis2_char_t *name);

        /**
          * To get the phaseRule of a handler it is required to get the HnadlerDescription of the handler
          * so the argumnet pass when it call return as HnadlerDescription
          * @param handler pointer to handler
          * @param env pointer to environment struct
          */
        struct axis2_handler_desc *(AXIS2_CALL *
                get_handler_desc)(
                    const axis2_handler_t *handler,
                    const axis2_env_t *env);
        /**
         * derived struct accessor
         */
         /*void* (AXIS2_CALL *get_derived)(struct axis2_handler * handler,
                                                          const axis2_env_t *env);
         */
        /**
          * derived struct mutator
          */
         /*axis2_status_t (AXIS2_CALL *set_derived)(struct axis2_handler *handler,
                                                          const axis2_env_t *env, void *derived);
         */
    };

    /**
     * Handler struct
     */
    struct axis2_handler
    {
        /** Operations of handler */
        axis2_handler_ops_t *ops;
    };

    /**
     * @param env pointer to environment struct
     * @param pointer to qname
     */
    typedef axis2_handler_t *(AXIS2_CALL *
    AXIS2_HANDLER_CREATE_FUNC)(
        const axis2_env_t *env,
        axis2_qname_t *qname);

    /**
     * creates handler struct
     * @param env pointer to environment struct
     */
    AXIS2_EXTERN axis2_handler_t *AXIS2_CALL
    axis2_handler_create(
        const axis2_env_t *env);

    /**
     * @param env pointer to environment struct
     * @param qname pointer to qname
     */
    AXIS2_EXTERN axis2_handler_t *AXIS2_CALL
    axis2_ctx_handler_create(
        const axis2_env_t *env, 
        axis2_qname_t *qname);

/** Frees the handler.
    @sa axis2_handler_ops#free */
#define AXIS2_HANDLER_FREE(handler, env) \
       ((handler)->ops->free(handler, env))

/** Initialize the handler.
    @sa axis2_handler_ops#init */
#define AXIS2_HANDLER_INIT(handler, env, handler_desc) \
       ((handler)->ops->init(handler, env, handler_desc))

/** Invoke.
    @sa axis2_handler_ops#invoke */
#define AXIS2_HANDLER_INVOKE(handler, env, msg_ctx) \
        ((handler)->ops->invoke(handler, env, msg_ctx))

/** Set invoke.
    @sa axis2_handler_ops#set_invoke */
#define AXIS2_HANDLER_SET_INVOKE(handler, env, invoke) \
        ((handler)->ops->set_invoke(handler, env, invoke))

/** Gets the name.
    @sa axis2_handler_ops#get_name */
#define AXIS2_HANDLER_GET_NAME(handler, env) \
        ((handler)->ops->get_name(handler, env))

/** Gets the param.
    @sa axis2_handler_ops#get_param */
#define AXIS2_HANDLER_GET_PARAM(handler, env, name) \
      ((handler)->ops->get_param(handler, env, name))

/** Gets the handler description.
    @sa axis2_handler_ops#get_handler_desc */
#define AXIS2_HANDLER_GET_HANDLER_DESC(handler, env) \
      ((handler)->ops->get_handler_desc(handler, env))

/*#define AXIS2_HANDLER_GET_DERIVED(handler, env) ((handler)->ops->get_derived(handler, env))
#define AXIS2_HANDLER_SET_DERIVED(handler, env, derived) ((handler)->ops->set_derived(handler, env, derived))
*/
/** @} */

#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_HANDLER_H */
