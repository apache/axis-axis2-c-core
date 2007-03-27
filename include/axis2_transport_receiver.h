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

#ifndef AXIS2_TRANSPORT_RECEIVER_H
#define AXIS2_TRANSPORT_RECEIVER_H

/**
 * @defgroup axis2_transport transport
 * @ingroup axis2
 * @{
 * @}
 */

/** @defgroup axis2_transport_receiver transport receiver
 * @ingroup axis2_transport
 * Description.
 * @{
 */

/**
 * @file axis2_transport_receiver.h
 * @brief Axis2 description transport receiver interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_endpoint_ref.h>
#include <axis2_ctx.h>
#include <axis2_conf_ctx.h>



#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_conf_ctx;
    struct axis2_transport_in_desc;
    /** Type name for axis2_transport_receiver */
    typedef struct axis2_transport_receiver axis2_transport_receiver_t;
    /** Type name for axis2_transport_receiver_ops */
    typedef struct axis2_transport_receiver_ops axis2_transport_receiver_ops_t;

    /**
     * @brief Description Transport Receiver ops struct
     * Encapsulator struct for ops of axis2_transport_receiver
     */
     struct axis2_transport_receiver_ops
    {
        /**
    * De-allocate memory
    * @param transport_receiver pointer to transport receiver
    * @param env pointer to environment struct
        */
        void (AXIS2_CALL *
                free)(
                    axis2_transport_receiver_t *transport_receiver,
                    const axis2_env_t *env);

   /**
    * @param transport_receiver
    * @param env pointer to environmnet struct
    */
        axis2_status_t (AXIS2_CALL *
                start)(
                    axis2_transport_receiver_t *transport_receiver,
                    const axis2_env_t *env);

   /**
    * @param transport_receiver pointer to transport receiver
    * @param env pointer to environment struct
    */
        axis2_status_t (AXIS2_CALL *
                stop)(
                    axis2_transport_receiver_t *transport_receiver,
                    const axis2_env_t *env);

   /**
    * @param tranport_receiver pointer to transport receiver
    * @param env pointer to environmnet struct
    * @param svc_name pointer to service name
    */
        axis2_endpoint_ref_t* (AXIS2_CALL *
                get_reply_to_epr)(
                    axis2_transport_receiver_t *transport_receiver,
                    const axis2_env_t *env,
                    const axis2_char_t *svc_name);

   /**
    * @param transport_receiver pointer to transport receiver
    * @param env pointer to environment struct
    * @param conf_ctx pointer to configuratoin context
    * @param intrasport_in pointer to transport_in
    */
        axis2_status_t (AXIS2_CALL *
                init) (
                    axis2_transport_receiver_t *transport_receiver,
                    const axis2_env_t *env,
                    struct axis2_conf_ctx *conf_ctx,
                    struct axis2_transport_in_desc *transport_in);


   /**
    * @param server pointer to server
    * @param env pointer to environment struct
    */
        struct axis2_conf_ctx* (AXIS2_CALL *
                get_conf_ctx)(
                    axis2_transport_receiver_t *server,
                    const axis2_env_t *env);


   /**
    * @param server pointer to server
    * @param env pointer to environment struct
    */
        axis2_bool_t (AXIS2_CALL *
                is_running)(
                    axis2_transport_receiver_t *server,
                    const axis2_env_t *env);

    };

    /**
     * @brief Transport Reciever  struct  
     */
     struct axis2_transport_receiver
    {
        axis2_transport_receiver_ops_t *ops;
    };


/*************************** Function macros **********************************/

/** Frees the transport receiver.
    @sa axis2_transport_receiver#free */
#define AXIS2_TRANSPORT_RECEIVER_FREE(transport_receiver, env) \
      ((transport_receiver->ops)->free (transport_receiver, env))

/** Initialize the transport receiver.
    @sa axis2_transport_receiver#init */
#define AXIS2_TRANSPORT_RECEIVER_INIT(transport_receiver, env, axisconf, transport_in) \
      ((transport_receiver->ops)->init (transport_receiver, env, axisconf, transport_in))

/** Start
    @sa axis2_transport_receiver#start */
#define AXIS2_TRANSPORT_RECEIVER_START(transport_receiver, env) \
      ((transport_receiver->ops)->start (transport_receiver, env))

/** Stop.
    @sa axis2_transport_receiver#stop */
#define AXIS2_TRANSPORT_RECEIVER_STOP(transport_receiver, env) \
      ((transport_receiver->ops)->stop (transport_receiver, env))

/** Get reply to epr.
    @sa axis2_transport_receiver#get_reply_to_epr */
#define AXIS2_TRANSPORT_RECEIVER_GET_REPLY_TO_EPR(transport_receiver, env, svc_name) \
      ((transport_receiver->ops)->get_reply_to_epr (transport_receiver, env, svc_name))

/** Get conf ctx. 
    @sa axis2_transport_receiver#get_conf_ctx */
#define AXIS2_TRANSPORT_RECEIVER_GET_CONF_CTX(transport_receiver, env) \
                        ((transport_receiver)->ops->get_conf_ctx(transport_receiver, env))

/** Is running.
    @sa axis2_transport_receiver#is_running */
#define AXIS2_TRANSPORT_RECEIVER_IS_RUNNING(transport_receiver, env) \
                                ((transport_receiver)->ops->is_running(transport_receiver, env))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_TRANSPORT_RECEIVER_H */
