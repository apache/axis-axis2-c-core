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

#ifndef AXIS2_HANDLER_H
#define AXIS2_HANDLER_H

/**
 * @defgroup axis2_handler handler
 * @ingroup axis2_handler
 * handler is the smallest unit of execution in the Axis2 engine's execution flow. 
 * The engine could have two flows, the in-flow and out-flow. A flow is a 
 * collection of phases and a phase in turn is a collection of handlers.
 * handlers are configured in relation to modules. A module is a point of 
 * extension in the Axis2 engine and a module would have one or more handlers 
 * defined in its configuration. The module configuration defines the phases 
 * each handler is attached to. A handler is invoked when the phase within which
 * it lives is invoked. handler is stateless and it is using the message context
 * that the state information is captures across invocations. 
 * @{
 */

/**
 * @file axis2_handler.h
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
     * handler ops struct.
     * Encapsulator struct for ops of axis2_handler.
     */
    struct axis2_handler_ops
    {
        /**
         * Free handler struct.
         * @param handler pointer to handler
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        void (AXIS2_CALL *
                free)(
                    axis2_handler_t *handler,
                    const axutil_env_t *env);


        /**
         * Initializes the handler with the information form handler description.
         * @param handler pointer to handler
         * @param env pointer to environment struct
         * @param handler_desc pointer to handler description related to the handler
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                init)(
                    axis2_handler_t *handler,
                    const axutil_env_t *env,
                    struct axis2_handler_desc *handler_desc);

        /**
         * Invoke is called to do the actual work assigned to the handler.
         * The phase that owns the handler is responsible for calling invoke
         * on top of the handler. Those structs that implement the interface 
         * of the handler should implement the logic for invoke and assign the
         * respective function pointer to invoke operation of the ops struct.
         * @param handler pointer to handler
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                invoke)(
                    axis2_handler_t *handler,
                    const axutil_env_t *env,
                    struct axis2_msg_ctx *msg_ctx);

        /**
         * Gets QName.
         * @param handler pointer to handler
         * @param env pointer to environment struct
         * @return pointer to QName of the handler
         */
        const axis2_string_t *(AXIS2_CALL *
                get_name)(
                    const axis2_handler_t *handler,
                    const axutil_env_t *env);

        /**
         * Gets the named parameter.
         * @param handler pointer to handler
         * @param env pointer to environment struct
         * @param name name of the parameter to be accessed
         */
        axis2_param_t *(AXIS2_CALL *
                get_param)(
                    const axis2_handler_t *handler,
                    const axutil_env_t *env,
                    const axis2_char_t *name);

        /**
          * Gets the handler description related to the handler.
          * @param handler pointer to handler
          * @param env pointer to environment struct
          * @return pointer to handler description struct related to handler         
          */
        struct axis2_handler_desc *(AXIS2_CALL *
                get_handler_desc)(
                    const axis2_handler_t *handler,
                    const axutil_env_t *env);
        
    };

    /**
     * handler struct.
     */
    struct axis2_handler
    {
        /** Operations of handler */
        axis2_handler_ops_t *ops;
    };

    /**
     * Function pointer defining the creates syntax for a handler struct instance.
     * @param env pointer to environment struct
     * @param pointer to qname
     * @return pointer to newly created handler struct
     */
    typedef axis2_handler_t *(AXIS2_CALL *
    AXIS2_HANDLER_CREATE_FUNC)(
        const axutil_env_t *env,
        const axis2_string_t *name);

    /**
     * Creates handler struct instance.
     * @param env pointer to environment struct
     * @return pointer to newly created handler struct
     */
    AXIS2_EXTERN axis2_handler_t *AXIS2_CALL
    axis2_handler_create(
        const axutil_env_t *env);

    /**
     * Creates a handler with invoke method implemented to fill in the service 
     * and operation context information.
     * @param env pointer to environment struct
     * @param qname pointer to qname, this is cloned within create method
     * @return pointer to newly created handler struct
     */
    AXIS2_EXTERN axis2_handler_t *AXIS2_CALL
    axis2_ctx_handler_create(
        const axutil_env_t *env, 
        const axis2_string_t *qname);

/** Frees handler.
    @sa axis2_handler_ops#free */
#define AXIS2_HANDLER_FREE(handler, env) \
       ((handler)->ops->free(handler, env))

/** Initializes handler.
    @sa axis2_handler_ops#init */
#define AXIS2_HANDLER_INIT(handler, env, handler_desc) \
       ((handler)->ops->init(handler, env, handler_desc))

/** Invokes the handler.
    @sa axis2_handler_ops#invoke */
#define AXIS2_HANDLER_INVOKE(handler, env, msg_ctx) \
        ((handler)->ops->invoke(handler, env, msg_ctx))

/** Gets handler QName.
    @sa axis2_handler_ops#get_name */
#define AXIS2_HANDLER_GET_NAME(handler, env) \
        ((handler)->ops->get_name(handler, env))

/** Gets the named parameter.
    @sa axis2_handler_ops#get_param */
#define AXIS2_HANDLER_GET_PARAM(handler, env, name) \
      ((handler)->ops->get_param(handler, env, name))

/** Gets handler description related to the handler.
    @sa axis2_handler_ops#get_handler_desc */
#define AXIS2_HANDLER_GET_HANDLER_DESC(handler, env) \
      ((handler)->ops->get_handler_desc(handler, env))

/** @} */

#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_HANDLER_H */
