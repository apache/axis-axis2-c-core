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

#ifndef AXIS2_DISP_H
#define AXIS2_DISP_H

/**
 * @defgroup axis2_disp dispatcher
 * @ingroup axis2_engine
 * dispatcher is responsible for finding the service and operation for a given
 * invocation. A Web service request would contain information that help 
 * locate the service and the operation serving the request. This information
 * could be in various formats, and hence the mechanism to find the requested
 * service and operation based on the available information could too vary.
 * Hence there can be various types on dispatches involved in a dispatching 
 * phase of the engine, that implements the API given in this header.
 * @{
 */

/**
 * @file axis2_disp.h
 */

#include <axis2_defines.h>
#include <axis2_string.h>
#include <axis2_handler.h>
#include <axis2_svc.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define AXIS2_DISP_NAMESPACE "http://axis.ws.apache.org"

    /** Type name for struct axis2_disp */
    typedef struct axis2_disp axis2_disp_t;
    /** Type name for struct axis2_disp_ops */
    typedef struct axis2_disp_ops axis2_disp_ops_t;

    /**
     * dispatcher ops struct.
     * Encapsulator struct for operations of axis2_dispatcher.
     */
     struct axis2_disp_ops
    {
        /**
         * Gets the base struct which is of type handler.
         * @param disp pointer to dispatcher
         * @param env pointer to environment struct
         * @return pointer to base handler struct. Returns a reference, not a 
         * cloned copy
         */
        axis2_handler_t *(AXIS2_CALL *
                get_base)(
                    const axis2_disp_t *disp,
                    const axis2_env_t *env);

        /**
         * Gets the name of the dispatcher.
         * @param disp pointer to dispatcher
         * @param env pointer to environment struct
         * @return pointer to name. Returns a reference, not a 
         * cloned copy
         */
        axis2_string_t *(AXIS2_CALL *
                get_name)(
                    const axis2_disp_t *disp,
                    const axis2_env_t *env);

        /**
         * Sets the name of the dispatcher.
         * @param disp pointer to dispatcher
         * @param env pointer to environment struct
         * @param name pointer to name, dispatcher assumes ownership of the 
         * name struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_name)(
                    axis2_disp_t *disp,
                    const axis2_env_t *env, 
                    axis2_string_t *name);

        /**
         * Frees dispatcher struct.
         * @param disp pointer to dispatcher
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE                    
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_disp_t *disp,
                    const axis2_env_t *env);
        /**
         * Finds the service that corresponds to the given message context. 
         * The various dispatcher implementations that inherit from dispatcher 
         * struct should implement this find_svc method and assign the 
         * respective function pointers to point to this method. 
         * @param msg_ctx pointer to message context 
         * @param env pointer to environment struct
         * @return pointer to service that should be servicing the request
         */
        axis2_svc_t *(AXIS2_CALL *
                find_svc)(
                    axis2_msg_ctx_t *msg_ctx,
                    const axis2_env_t *env);
        /**
         * Finds the operation that corresponds to the given message context
         * and service. The various dispatcher implementations that inherit from 
         * dispatcher struct should implement this find_op method and assign the 
         * respective function pointers to point to this method. 
         * Finds the op
         * @param msg_ctx pointer to message context
         * @param env pointer to environment struct
         * @param svc pointer to service, usually this is found through find_svc
         * method
         * @return pointer to operation that the request is targeted to
         */
        axis2_op_t *(AXIS2_CALL *
                find_op)(
                    axis2_msg_ctx_t *msg_ctx,
                    const axis2_env_t *env,
                    const axis2_svc_t *svc);

    };

    /**
     * dispatcher struct.
     */
    struct axis2_disp
    {
        /** operations of dispatcher struct */
        axis2_disp_ops_t *ops;
    };


    /**
     * Creates a dispatcher struct instance.
     * @param env pointer to environment struct
     * @param name pointer to QName. QName is cloned by create method.
     * @return pointer to newly created dispatcher
     */
    AXIS2_EXTERN axis2_disp_t *AXIS2_CALL 
    axis2_disp_create(
        const axis2_env_t *env, 
        const axis2_string_t *name);

    /**
     * Invokes the dispatcher.
     * @param handler pointer to handler that is the base of the dispatcher 
     * to be invoked
     * @param env pointer to environment struct
     * @param msg_ctx pointer to message context
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    axis2_status_t AXIS2_CALL 
    axis2_disp_invoke(
        axis2_handler_t *handler,
        const axis2_env_t *env,
        axis2_msg_ctx_t *msg_ctx);

    /**
     * Creates a WS-Addressing based dispatcher.
     * @param env pointer to environment struct
     * @return pointer to the newly created dispatcher with find_svc and find_op
     * methods implemented based on WS-Addressing
     */
    axis2_disp_t *AXIS2_CALL 
    axis2_addr_disp_create(
        const axis2_env_t *env);

    /**
     * Creates a request URI based dispatcher.
     * @param env pointer to environment struct
     * @return pointer to the newly created dispatcher with find_svc and find_op
     * methods implemented based on request URI processing.
     */
    AXIS2_EXTERN axis2_disp_t *AXIS2_CALL 
    axis2_req_uri_disp_create(
        const axis2_env_t *env);

    /**
     * Creates a SOAP body based dispatcher.
     * @param env pointer to environment struct
     * @return pointer to the newly created dispatcher with find_svc and find_op
     * methods implemented based on SOAP body processing.
     */
    axis2_disp_t *AXIS2_CALL 
    axiom_soap_body_disp_create(
        const axis2_env_t *env);

    /**
     * Creates a SOAP action based dispatcher.
     * @param env pointer to environment struct
     * @return pointer to the newly created dispatcher with find_svc and find_op
     * methods implemented based on SOAP action processing
     */
    axis2_disp_t *AXIS2_CALL 
    axiom_soap_action_disp_create(
        const axis2_env_t *env);


/** Gets the base handler. 
    @sa axis2_disp_ops#get_base */
#define AXIS2_DISP_GET_BASE(disp, env) \
        ((disp)->ops->get_base(disp, env))

/** Gets QName.
    @sa axis2_disp_ops#get_name */
#define AXIS2_DISP_GET_NAME(disp, env) \
        ((disp)->ops->get_name(disp, env))

/** Sets QName.
    @sa axis2_disp_ops#set_name */
#define AXIS2_DISP_SET_QNAME(disp, env, name) \
        ((disp)->ops->set_name(disp, env, name))

/** Frees dispatcher.
    @sa axis2_disp_ops#free */
#define AXIS2_DISP_FREE(disp, env) \
        ((disp)->ops->free(disp, env))

/** Finds service that should service the request.
    @sa axis2_disp_ops#find_svc */
#define AXIS2_DISP_FIND_SVC(msg_ctx, env) \
        ((msg_ctx)->ops->find_svc(msg_ctx, env))

/** Finds the operation in the give service that the request is targeted to.
    @sa axis2_disp_ops#find_op */
#define AXIS2_DISP_FIND_OP(msg_ctx, env, svc) \
        ((msg_ctx)->ops->find_op(msg_ctx, env, svc))

/** @} */

#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_DISP_H */
