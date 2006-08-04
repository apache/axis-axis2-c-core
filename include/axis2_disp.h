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

#ifndef AXIS2_DISP_H
#define AXIS2_DISP_H

/**
 * @defgroup axis2_disp dispatcher
 * @ingroup axis2_engine
 * Description.
 * @{
 */

/**
 * @file axis2_disp.h
 * @brief Axis2 Dispatcher interface
 */

#include <axis2_defines.h>
#include <axis2_qname.h>
#include <axis2_handler.h>
#include <axis2_svc.h>

#ifdef __cplusplus
extern "C"
{
#endif
    struct axis2_op;
    struct axis2_svc;

    /** Type name for struct axis2_disp */
    typedef struct axis2_disp axis2_disp_t;
    /** Type name for struct axis2_disp_ops */
    typedef struct axis2_disp_ops axis2_disp_ops_t;

    /**
     * Dispatcher ops struct
     * Encapsulator struct for operations of axis2_dispatcher
     *
     */
    AXIS2_DECLARE_DATA struct axis2_disp_ops
    {
        /**
         * @param disp pointer to dispatcher
         * @param env pointer to environment struct
         */
        axis2_handler_t* (AXIS2_CALL 
                *get_base)(
                  const axis2_disp_t *disp,
                    const axis2_env_t *env);

        /**
         * @param disp pointer to dispatcher
         * @param env pointer to environment struct
         */
        axis2_qname_t* (AXIS2_CALL 
                *get_qname)(
                    const axis2_disp_t *disp,
                    const axis2_env_t *env);

        /**
         * @param disp pointer to dispatcher
         * @param env pointer to environment struct
         * @param qname pointer to qname
         */
        axis2_status_t (AXIS2_CALL 
                *set_qname)(
                    struct axis2_disp *disp,
                    const axis2_env_t *env, 
                  axis2_qname_t *qname);

        /**
         * @param disp pointer to dispatcher
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL 
                *free)(
                    struct axis2_disp *disp,
                    const axis2_env_t *env);
        /**
         * Finds the service
         * @param msg_ctx pointer to message context 
         * @param env pointer to environment struct
         */
        struct axis2_svc* (AXIS2_CALL 
                *find_svc)(
                    axis2_msg_ctx_t *msg_ctx,
                    const axis2_env_t *env);
        /**
         * Finds the op
         * @param msg_ctx pointer to message context
         * @param env pointer to environment struct
         * @param svc pointer to service
         */
        struct axis2_op *(AXIS2_CALL 
                *find_op)(
                    axis2_msg_ctx_t *msg_ctx,
                    const axis2_env_t *env,
                    struct axis2_svc *svc);

    };

    /**
     * dispatcher struct
     */
    struct axis2_disp
    {
        /** operations of dispatcher struct */
        axis2_disp_ops_t *ops;
    };


    /**
     * Creates disp struct.
     * @param env pointer to environment struct
     * @param qname pointer to qname, it can be NULL
     */
    AXIS2_EXTERN axis2_disp_t *AXIS2_CALL 
    axis2_disp_create(
        const axis2_env_t *env, 
        axis2_qname_t *qname);

    /**
     * Invokes the dispatcher.
     * @param handler pointer to handler
     * @param env pointer to environment struct
     * @param msg_ctx pointer to message context
     */
    axis2_status_t AXIS2_CALL 
    axis2_disp_invoke(
        struct axis2_handler *handler,
        const axis2_env_t *env,
        struct axis2_msg_ctx *msg_ctx);

    /**
     * Creates the addressing dispatcher.
     * @param env pointer to environment struct
     */
    axis2_disp_t* AXIS2_CALL 
    axis2_addr_disp_create(
        const axis2_env_t *env);

    /**
     * Creates the request uri dispatcher.
     * @param env pointer to environment struct
     */
    axis2_disp_t* AXIS2_CALL 
    axis2_req_uri_disp_create(
        const axis2_env_t *env);

    /**
     * Creates the soap body dispatcher.
     * @param env pointer to environment struct
     */
    axis2_disp_t* AXIS2_CALL 
    axiom_soap_body_disp_create(
        const axis2_env_t *env);

    /**
     * Creates the soap action dispatcher.
     * @param env pointer to environment struct
     */
    axis2_disp_t* AXIS2_CALL 
    axiom_soap_action_disp_create(
        const axis2_env_t *env);


/** Gets the  base. 
    @sa axis2_disp_ops#get_base */
#define AXIS2_DISP_GET_BASE(disp, env) \
        ((disp)->ops->get_base(disp, env))

/** Gets the qname.
    @sa axis2_disp_ops#get_qname */
#define AXIS2_DISP_GET_QNAME(disp, env) \
        ((disp)->ops->get_qname(disp, env))

/** Sets the qname.
    @sa axis2_disp_ops#set_qname */
#define AXIS2_DISP_SET_QNAME(disp, env, name) \
        ((disp)->ops->set_qname(disp, env, name))

/** Frees the dispatcher.
    @sa axis2_disp_ops#free */
#define AXIS2_DISP_FREE(disp, env) \
        ((disp)->ops->free(disp, env))

/** Finds the service.
    @sa axis2_disp_ops#find_svc */
#define AXIS2_DISP_FIND_SVC(msg_ctx, env) \
        ((msg_ctx)->ops->find_svc(msg_ctx, env))

/** Finds the operation.
    @sa axis2_disp_ops#find_op */
#define AXIS2_DISP_FIND_OP(msg_ctx, env, svc) \
        ((msg_ctx)->ops->find_op(msg_ctx, env, svc))

/** @} */

#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_DISP_H */
