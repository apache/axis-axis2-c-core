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

#ifndef AXIS2_ENGINE_H
#define AXIS2_ENGINE_H

/**
 * @defgroup axis2_engine engine 
 * @ingroup axis2_engine
 * @{
 */

/**
  * @file axis2_engine.h
  * @brief axis2 Message Context interface
  */

#include <axis2_defines.h>
#include <axis2_array_list.h>
#include <axis2_env.h>
#include <axis2_conf_ctx.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_engine_ops */
    typedef struct axis2_engine_ops axis2_engine_ops_t;
    /** Type name for struct axis2_engine */
    typedef struct axis2_engine axis2_engine_t;

    struct axiom_soap_fault;


    /**
     * Message Context ops struct
     * Encapsulator struct for ops of axis2_engine
     */
    struct axis2_engine_ops
    {
        /**
         * This methods represents the outflow of the Axis, this could be either at the server side or the client side.
         * Here the <code>ExecutionChain</code> is created using the Phases. The Handlers at the each Phases is ordered in
         * deployment time by the deployment module
         * @param engine pointer to engine
         * @param eng pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                send)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);
        /**
         * This methods represents the inflow of the Axis, this could be either at the server side or the client side.
         * Here the <code>ExecutionChain</code> is created using the Phases. The Handlers at the each Phases is ordered in
         * deployment time by the deployment module
         * @param engine pointer to engine
         * @param eng pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                receive)(   
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);
        /**
         * Sends the SOAP Fault to another SOAP node.
         * @param engine pointer to engine
         * @param eng pointer to environment struct
         * @param msg_ctx pointer to message context
         */
        axis2_status_t (AXIS2_CALL *
                send_fault)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);
        /**
         * This is invoked when a SOAP Fault is received from a Other SOAP Node
         * Receives a SOAP fault from another SOAP node.
         * @param engine pointer to engine
         * @param eng pointer to environment struct
         * @param msg_ctx pointer to message context
         */
        axis2_status_t (AXIS2_CALL *
                receive_fault)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);
        /**
         * This method is called to handle any error that occurs at inflow or outflow. But if the
         * method is called twice, it implies that sending the error handling has failed, in which case
         * the method logs the error and exists.
         * @param engine pointer to engine
         * @param eng pointer to environment struct
         * @param processing_context pointer to message processing context
         */
        axis2_msg_ctx_t *(AXIS2_CALL *
                create_fault_msg_ctx)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *processing_context);
        /**
         * Information to create the SOAPFault can be extracted from different places.
         * 1. Those info may have been put in to the message context by some handler. When someone
         * is putting like that, he must make sure the SOAPElements he is putting must be from the
         * correct SOAP Version.
         * 2. SOAPProcessingException is flexible enough to carry information about the fault. For example
         * it has an attribute to store the fault code. The fault reason can be extracted from the
         * message of the exception. I opted to put the stacktrace under the detail element.
         * eg : <Detail>
         * <Exception> stack trace goes here </Exception>
         * <Detail>
         * <p/>
         * If those information can not be extracted from any of the above places, I default the soap
         * fault values to following.
         * <Fault>
         * <Code>
         * <Value>env:Receiver</Value>
         * </Code>
         * <Reason>
         * <Text>unknown</Text>
         * </Reason>
         * <Role/>
         * <Node/>
         * <Detail/>
         * </Fault>
         * <p/>
         * -- EC
         *
         * @param engine pointer to engine
         * @param eng pointer to environment struct
         * @param msg_ctx pointer to message context
         * @param fault pointer to fault
         */
        axis2_status_t (AXIS2_CALL *
                extract_fault_info_from_msg_ctx)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx,
                    struct axiom_soap_fault *fault);

        /**
         * @param engine pointer to engine
         * @param eng pointer to environment struct
         * @param msg_ctx pointer to message context
         */
        axis2_status_t (AXIS2_CALL *
                verify_ctx_built)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);

        /**
         * @param engine pointer to engine
         * @param eng pointer to environment struct
         * @param phases pointer to phases
         * @param msg_ctx pointer to message context
         */
        axis2_status_t (AXIS2_CALL *
                invoke_phases)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_array_list_t *phases,
                    axis2_msg_ctx_t *msg_ctx);

        /**
         * @param engine pointer to engine
         * @param eng pointer to environment struct
         * @param phases pointer to phases
         * @param msg_ctx pointer to message context
         */
        axis2_status_t (AXIS2_CALL *
                resume_invocation_phases)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_array_list_t *phases,
                    axis2_msg_ctx_t *msg_ctx);

        /**
         * @param engine pointer to engine
         * @param eng pointer to environment struct
         * @param soap_namespace pointer to namespace
         */
        axis2_char_t *(AXIS2_CALL *
                get_sender_fault_code)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_char_t *soap_namespace);


        /**
         * @param engine pointer to engine
         * @param eng pointer to environment struct
         * @param soap_namespace pointer to soap namespace
         */
        const axis2_char_t *(AXIS2_CALL *
                get_receiver_fault_code)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_char_t *soap_namespace);

        /**
         * @param engine pointer to engine
         * @param eng pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env);

        /**
         * @param engine pointer to engine
         * @param eng pointer to environment struct
         * @param msg_ctx pointer to message context
         */
        axis2_status_t (AXIS2_CALL *
                resume_receive)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);

        /**
         * @param engine pointer to engine
         * @param eng pointer to environment struct
         * @param msg_ctx pointer to message context
         */
        axis2_status_t (AXIS2_CALL *
                resume_send)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);

    };

    /**
     * Engine struct
     */
    struct axis2_engine
    {
    
        /** Operations of engine */
        axis2_engine_ops_t *ops;
    };

    AXIS2_EXTERN axis2_engine_t *AXIS2_CALL
    axis2_engine_create(
        const axis2_env_t *env,
        axis2_conf_ctx_t *conf_ctx);

/************************** Start of function macros **************************/

/** Send.
    @sa axis2_engine_ops#send */
#define AXIS2_ENGINE_SEND(engine, env, msg_ctx)\
        ((engine)->ops->send(engine, env, msg_ctx))

/** Receive.
    @sa axis2_engine_ops#receive */
#define AXIS2_ENGINE_RECEIVE(engine, env, msg_ctx) \
        ((engine)->ops->receive(engine, env, msg_ctx))

/** Send fault.
    @sa axis2_engine_ops#send_fault */
#define AXIS2_ENGINE_SEND_FAULT(engine, env, msg_ctx) \
        ((engine)->ops->send_fault(engine, env, msg_ctx))

/** Receive fault.
    @sa axis2_engine_ops#receive_fault */
#define AXIS2_ENGINE_RECEIVE_FAULT(engine, env, msg_ctx) \
        ((engine)->ops->receive_fault(engine, env, msg_ctx))

/** Fault message context.
    @sa axis2_engine_ops#create_fault_msg_ctx */
#define AXIS2_ENGINE_CREATE_FAULT_MSG_CTX(engine, env, msg_ctx) \
        ((engine)->ops->create_fault_msg_ctx(engine, env, msg_ctx))

/** Extraxts  the fault info from messag econtext
    @sa axis2_engine_ops#extract_fault_info_from_msg_ctx */
#define AXIS2_ENGINE_EXTRACT_FAULT_INFO_FROM_MSG_CTX(engine, env, msg_ctx, fault) \
        ((engine)->ops->extract_fault_info_from_msg_ctx(engine, env, msg_ctx, fault))

/** Verify context built.
    @sa axis2_engine_ops#verify_ctx_built */
#define AXIS2_ENGINE_VERIFY_CTX_BUILT(engine, env, msg_ctx) \
        ((engine)->ops->verify_ctx_built(engine, env, msg_ctx))

/** Invokes the phases.
    @sa axis2_engine_ops#invoke_phases */
#define AXIS2_ENGINE_INVOKE_PHASES(engine, env, phases, msg_ctx) \
        ((engine)->ops->invoke_phases(engine, env, phases, msg_ctx))

/** Resumes the invocation phases.
    @sa axis2_engine_ops#resume_invocation_phases */
#define AXIS2_ENGINE_RESUME_INVOCATION_PHASES(engine, env, phases, msg_ctx) \
        ((engine)->ops->resume_invocation_phases(engine, env, phases, msg_ctx))

/** Gets the sender fault code.
    @sa axis2_engine_ops#get_sender_fault_code */
#define AXIS2_ENGINE_GET_SENDER_FAULT_CODE(engine, env, soap_namespace) \
        ((engine)->ops->get_sender_fault_code(engine, env, soap_namespace))

/** Gets the receiver fault code.
    @sa axis2_engine_ops#get_receiver_fault_code */
#define AXIS2_ENGINE_GET_RECEIVER_FAULT_CODE(engine, env, soap_namespace) \
        ((engine)->ops->get_receiver_fault_code(engine, env, soap_namespace))

/** Frees the engine.
    @sa axis2_engine_ops#free */
#define AXIS2_ENGINE_FREE(engine, env) \
        ((engine)->ops->free(engine, env))

/** Resume send.
    @sa axis2_engine_ops#resume_send */
#define AXIS2_ENGINE_RESUME_SEND(engine, env, msg_ctx)\
        ((engine)->ops->resume_send(engine, env, msg_ctx))

/** Resume receive.
    @sa axis2_engine_ops#resume_receive */
#define AXIS2_ENGINE_RESUME_RECEIVE(engine, env, msg_ctx) \
        ((engine)->ops->resume_receive(engine, env, msg_ctx))

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_ENGINE_H */
