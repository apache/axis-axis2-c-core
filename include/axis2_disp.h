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
    struct axis2_disp;
    struct axis2_disp_ops;
    
/**
 * @defgroup axis2_disp dispatcher
 * @ingroup axis2_engine
 * @{
 */

/**
 *   \brief Dispatcher ops struct
 */
 AXIS2_DECLARE_DATA   typedef struct axis2_disp_ops
    { 
        axis2_handler_t* (AXIS2_CALL *get_base) (struct axis2_disp *disp, 
                                               const axis2_env_t *env);
        axis2_qname_t* (AXIS2_CALL *get_qname) (struct axis2_disp *disp, 
                                               const axis2_env_t *env);
        axis2_status_t (AXIS2_CALL *set_qname) (struct axis2_disp *disp, 
                                               const axis2_env_t *env, axis2_qname_t *qname);
        axis2_status_t (AXIS2_CALL *free) (struct axis2_disp *disp, 
                                               const axis2_env_t *env);
        /**
        * finds the service
        *
        * @param messageContext
        * @return
        */
        struct axis2_svc* (AXIS2_CALL *find_svc)(axis2_msg_ctx_t *msg_ctx, 
                        const axis2_env_t *env);
        /**
        * finds the op
        *
        * @param service
        * @param msg_ctx
        * @return
        */
        struct axis2_op *(AXIS2_CALL *find_op)(axis2_msg_ctx_t *msg_ctx,
                                    const axis2_env_t *env,
                                    struct axis2_svc *svc);
        
    } axis2_disp_ops_t;
   
   /** 
    * \brief Dispatcher struct
    */
    typedef struct axis2_disp
    {
        /** Dispatcher related ops */
        axis2_disp_ops_t *ops;
    } axis2_disp_t;


/**
 * creates disp struct
 * @param qname qname, can be NULL
 */
AXIS2_EXTERN axis2_disp_t *AXIS2_CALL axis2_disp_create(const axis2_env_t *env, axis2_qname_t *qname);

#define AXIS2_DISP_GET_BASE(disp, env) ((disp)->ops->get_base(disp, env))
#define AXIS2_DISP_GET_QNAME(disp, env) ((disp)->ops->get_qname(disp, env))
#define AXIS2_DISP_SET_QNAME(disp, env, name) ((disp)->ops->set_qname(disp, env, name))
#define AXIS2_DISP_FREE(disp, env) ((disp)->ops->free(disp, env))
#define AXIS2_DISP_FIND_SVC(msg_ctx, env) ((msg_ctx)->ops->find_svc(msg_ctx, env))
#define AXIS2_DISP_FIND_OP(msg_ctx, env, svc) ((msg_ctx)->ops->find_op(msg_ctx, env, svc))

axis2_status_t AXIS2_CALL axis2_disp_invoke(struct axis2_handler * handler, 
                                                const axis2_env_t *env,
                                                struct axis2_msg_ctx *msg_ctx);
                                                    
axis2_disp_t* AXIS2_CALL axis2_addr_disp_create(const axis2_env_t *env);
axis2_disp_t* AXIS2_CALL axis2_req_uri_disp_create(const axis2_env_t *env);
axis2_disp_t* AXIS2_CALL axiom_soap_body_disp_create(const axis2_env_t *env);
axis2_disp_t* AXIS2_CALL axiom_soap_action_disp_create(const axis2_env_t *env);
                                                
/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_DISP_H */
