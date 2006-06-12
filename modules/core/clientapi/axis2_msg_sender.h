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

#ifndef AXIS2_MSG_SENDER_H
#define AXIS2_MSG_SENDER_H


/**
  * @file axis2_msg_sender.h
  * @brief axis2 Message Context interface
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_mep_client.h>
#include <axis2_soap_envelope.h>


#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_msg_sender  
  * @ingroup axis2_core_clientapi
  * @{
  */
    
typedef struct axis2_msg_sender_ops axis2_msg_sender_ops_t;
typedef struct axis2_msg_sender axis2_msg_sender_t; 
    
/** 
 * @brief Message Sender ops struct
 * Encapsulator struct for ops of axis2_msg_sender
 */  
struct axis2_msg_sender_ops
{
    /**
     * Send the SOAP Message and forget about it. This is one way
     * @param axisop
     * @param msgctx
     */    
    axis2_status_t (AXIS2_CALL *
    send)(axis2_msg_sender_t *msg_sender, 
        const axis2_env_t *env,
        axis2_op_t *op,
        axis2_msg_ctx_t *msg_ctx);

    /**
     * set the transport to used for sending the SOAP Message
     * @param senderTransport
     */
    axis2_status_t (AXIS2_CALL *
    set_transport_info)(axis2_msg_sender_t *msg_sender, 
                  const axis2_env_t *env,
                  axis2_char_t *sender_transport);

   /**
    * Send an om node
    *
    * @param op - this will be used to identify the operation in the client 
    * side, without dispatching
    * @param om_node_to_send - This should be OM Element (payload)
    * @return
    */
    axis2_status_t (AXIS2_CALL *
    send_with_om)(struct axis2_msg_sender *msg_sender, 
              const axis2_env_t *env,
              axis2_char_t *op_name, 
              axiom_node_t *om_node_to_send);
   
   /**
    * Send the SOAP Message, the actual worker
    *
    */
    axis2_status_t (AXIS2_CALL *
    send_with_soap)(axis2_msg_sender_t *msg_sender, 
                 const axis2_env_t *env,
                 axis2_char_t *op_name, 
               axis2_soap_envelope_t *envelope);
   /**
    * Get the message information header object. All the sets to 
    * msg_info_headers should be done via get_msg_info and a set
    */               
   axis2_msg_info_headers_t* (AXIS2_CALL *
    get_msg_info_headers)(axis2_msg_sender_t *msg_sender, 
                          const axis2_env_t *env);
                  
    axis2_status_t (AXIS2_CALL *
    free)(struct axis2_msg_sender *msg_sender, 
        const axis2_env_t *env);
};

/** 
 * @brief Message Sender struct
  *   Axis2 Message Sender
 */
struct axis2_msg_sender
{
    axis2_msg_sender_ops_t *ops;    
};

AXIS2_EXTERN axis2_msg_sender_t* AXIS2_CALL 
axis2_msg_sender_create(const axis2_env_t *env, 
                  axis2_svc_ctx_t *svc_ctx);

    
/************************** Start of function macros **************************/

#define AXIS2_MSG_SENDER_SEND(msg_sender, env, op, msg_ctx) \
      ((msg_sender)->ops->send(msg_sender, env, op, msg_ctx))
        
#define AXIS2_MSG_SENDER_SET_TRANSPORT_INFO(msg_sender, env, sender_transport)\
      ((msg_sender)->ops->set_transport_info(msg_sender, env,   sender_transport))
        
#define AXIS2_MSG_SENDER_SEND_WITH_OM(msg_sender, env, op_name, om_node_to_send) \
        ((msg_sender)->ops->send_with_om(msg_sender, env, op_name, om_node_to_send))
        
#define AXIS2_MSG_SENDER_SEND_WITH_SOAP(msg_sender, env, op_name, om_node_to_send) \
        ((msg_sender)->ops->send_with_soap(msg_sender, env, op_name, envelope))
        
#define AXIS2_MSG_SENDER_FREE(msg_sender, env) \
        ((msg_sender)->ops->free(msg_sender, env))
        
/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_MSG_SENDER_H */
