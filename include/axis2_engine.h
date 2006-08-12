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
 * engine has the send and receieve functions that is the heart when providing 
 * and consuming services. In Axis2 SOAP engine architecture, all the others 
 * parts are build around the concept of the engine. There is only one engine 
 * for both the server side and the client side, and the engine is not aware of
 * if it is invoked as an client or a service. engine supports both synchronous 
 * and asynchronous messageing modes based on send and receive functions.
 * @{
 */

/**
  * @file axis2_engine.h
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
     * engine ops struct.
     * Encapsulator struct for ops of axis2_engine.
     */
    struct axis2_engine_ops
    {
        /**
         * This methods represents the out flow of the Axis engine both at the 
         * server side as well as the client side. In this function, the 
         * execution chain is created using the phases of the out flow. 
         * All handlers at each out flow phase, which are ordered in the
         * deployment time are invoked in sequence here.
         * @param engine pointer to engine
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context representing current state
         * that is used when sending message
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                send)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);
        
        /**
         * This methods represents the in flow of the Axis engine,  both at the 
         * server side as well as the client side. In this function, the 
         * execution chain is created using the phases of the in flow. 
         * All handlers at each in flow phase, which are ordered in the
         * deployment time are invoked in sequence here.
         * @param engine pointer to engine
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context representing current state
         * that is used in receieving message
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                receive)(   
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);
        
        /**
         * Sends a SOAP fault.
         * @param engine pointer to engine
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context that contains details of 
         * fault state
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                send_fault)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);
                    
        /**
         * This is invoked when a SOAP fault is received.
         * @param engine pointer to engine
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context representing that contains
         * the details of receieve state
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                receive_fault)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);
                    
        /**
         * Creates a message context that represents the fault state based on 
         * current processing state.
         * @param engine pointer to engine
         * @param env pointer to environment struct
         * @param processing_context pointer to message context representing 
         * current processing context
         * @return pointer to message context representing the fault state
         */
        axis2_msg_ctx_t *(AXIS2_CALL *
                create_fault_msg_ctx)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *processing_context);
         
        /**
         * Extraxts fault information form message context.          
         * @param engine pointer to engine
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context containing fault state
         * @param fault pointer to SOAP fault struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                extract_fault_info_from_msg_ctx)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx,
                    struct axiom_soap_fault *fault);

        /**
         * Verifies that the context hierarchy consisting of service group 
         * context, service context and operation context is built.
         * @param engine pointer to engine
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context containing the context 
         * related information
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                verify_ctx_built)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);

        /**
         * Invokes the phases in the given array list of phases. The list of 
         * phases could be representing one of the flows. The two possible 
         * flows are in flow and out flow. Both of those flows can also have 
         * fault related representations, in fault flow and out fault flow.
         * Invoking a phase triggers the invokation of handlers the phase 
         * contain. 
         * @param engine pointer to engine
         * @param env pointer to environment struct
         * @param phases pointer to phases
         * @param msg_ctx pointer to message context containing current state
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                invoke_phases)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_array_list_t *phases,
                    axis2_msg_ctx_t *msg_ctx);

        /**
         * Resumes phase invocation. While invoking the phases, one of the 
         * handlers in any phase could determine to pause the invocation. 
         * Often pausing happens to wait till some state is reached or some
         * task is complete. Once paused, the invocation has to be resumed 
         * using this function, which will resume the invocation from the paused 
         * handler in the paused phase and will continute till it is paused 
         * again or it completes invoking all the remaining handlers in the
         * remaining phases.
         * @param engine pointer to engine
         * @param env pointer to environment struct
         * @param phases pointer to phases
         * @param msg_ctx pointer to message context containing current paused 
         * state
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                resume_invocation_phases)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_array_list_t *phases,
                    axis2_msg_ctx_t *msg_ctx);

        /**
         * Gets sender's SOAP fault code.
         * @param engine pointer to engine
         * @param env pointer to environment struct
         * @param soap_namespace pointer to SOAP namespace
         * @return pointer to SOAP fault code string
         */
        const axis2_char_t *(AXIS2_CALL *
                get_sender_fault_code)(
                    const axis2_engine_t *engine,
                    const axis2_env_t *env,
                    const axis2_char_t *soap_namespace);


        /**
         * Gets receiver's SOAP fault code.
         * @param engine pointer to engine
         * @param env pointer to environment struct
         * @param soap_namespace pointer to soap namespace
         */
        const axis2_char_t *(AXIS2_CALL *
                get_receiver_fault_code)(
                    const axis2_engine_t *engine,
                    const axis2_env_t *env,
                    const axis2_char_t *soap_namespace);

        /**
         * Frees engine struct.
         * @param engine pointer to engine
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env);

        /**
         * Resumes receieve operation. It could be the case that receive was 
         * paused by one of the in flow handlers. In such a situation, this 
         * method could be used to resume the receieve operation.
         * @param engine pointer to engine
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                resume_receive)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);

        /**
         * Resumes send operation. It could be the case that send was 
         * paused by one of the out flow handlers. In such a situation, this 
         * method could be used to resume the send operation.
         * @param engine pointer to engine
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                resume_send)(
                    axis2_engine_t *engine,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);

    };

    /**
     * engine struct.
     */
    struct axis2_engine
    {
    
        /** Operations of engine */
        axis2_engine_ops_t *ops;
    };

    /**
     * Creates en engine struct instance.
     * @param env pointer to environment struct
     * @param conf_ctx pointer to configuration context struct
     * @return pointer to newly created engine struct
     */
    AXIS2_EXTERN axis2_engine_t *AXIS2_CALL
    axis2_engine_create(
        const axis2_env_t *env,
        axis2_conf_ctx_t *conf_ctx);

/** Sends SOAP message invoking the out flow.
    @sa axis2_engine_ops#send */
#define AXIS2_ENGINE_SEND(engine, env, msg_ctx)\
        ((engine)->ops->send(engine, env, msg_ctx))

/** Receives SOAP message invoking the in flow.
    @sa axis2_engine_ops#receive */
#define AXIS2_ENGINE_RECEIVE(engine, env, msg_ctx) \
        ((engine)->ops->receive(engine, env, msg_ctx))

/** Sends a SOAP fault.
    @sa axis2_engine_ops#send_fault */
#define AXIS2_ENGINE_SEND_FAULT(engine, env, msg_ctx) \
        ((engine)->ops->send_fault(engine, env, msg_ctx))

/** Receive a SOAP fault.
    @sa axis2_engine_ops#receive_fault */
#define AXIS2_ENGINE_RECEIVE_FAULT(engine, env, msg_ctx) \
        ((engine)->ops->receive_fault(engine, env, msg_ctx))

/** Creates fault message context.
    @sa axis2_engine_ops#create_fault_msg_ctx */
#define AXIS2_ENGINE_CREATE_FAULT_MSG_CTX(engine, env, msg_ctx) \
        ((engine)->ops->create_fault_msg_ctx(engine, env, msg_ctx))

/** Extraxts the fault info from messag econtext,
    @sa axis2_engine_ops#extract_fault_info_from_msg_ctx */
#define AXIS2_ENGINE_EXTRACT_FAULT_INFO_FROM_MSG_CTX(engine, env, msg_ctx, fault) \
        ((engine)->ops->extract_fault_info_from_msg_ctx(engine, env, msg_ctx, fault))

/** Verifies that the context hierarchy is built.
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

/** Gets sender fault code.
    @sa axis2_engine_ops#get_sender_fault_code */
#define AXIS2_ENGINE_GET_SENDER_FAULT_CODE(engine, env, soap_namespace) \
        ((engine)->ops->get_sender_fault_code(engine, env, soap_namespace))

/** Gets receiver fault code.
    @sa axis2_engine_ops#get_receiver_fault_code */
#define AXIS2_ENGINE_GET_RECEIVER_FAULT_CODE(engine, env, soap_namespace) \
        ((engine)->ops->get_receiver_fault_code(engine, env, soap_namespace))

/** Frees engine.
    @sa axis2_engine_ops#free */
#define AXIS2_ENGINE_FREE(engine, env) \
        ((engine)->ops->free(engine, env))

/** Resumes send operation.
    @sa axis2_engine_ops#resume_send */
#define AXIS2_ENGINE_RESUME_SEND(engine, env, msg_ctx)\
        ((engine)->ops->resume_send(engine, env, msg_ctx))

/** Resumes  receive operation.
    @sa axis2_engine_ops#resume_receive */
#define AXIS2_ENGINE_RESUME_RECEIVE(engine, env, msg_ctx) \
        ((engine)->ops->resume_receive(engine, env, msg_ctx))

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_ENGINE_H */
