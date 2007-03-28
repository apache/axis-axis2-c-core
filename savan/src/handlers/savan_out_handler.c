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

/*
#include <axis2_addr_mod.h>
#include <axis2_addr.h>
*/
#include <axis2_handler_desc.h>
#include <axis2_array_list.h>
#include <axiom_soap_const.h>
#include <axiom_soap_envelope.h>
#include <axiom_soap_header.h>
#include <axiom_soap_header_block.h>
#include <axis2_op.h>
#include <axis2_msg_ctx.h>
#include <axis2_conf_ctx.h>
#include <axis2_msg_info_headers.h>
#include <axis2_property.h>

#include <savan_constants.h>
#include <savan_error.h>
#include <savan_sub_processor.h>
#include <savan_util.h>
#include <savan_msg_recv.h>
#include <savan_subscriber.h>

/* Function Prototypes ********************************************************/

axis2_status_t AXIS2_CALL
savan_out_handler_invoke(
    struct axis2_handler *handler, 
    const axis2_env_t *env,
    struct axis2_msg_ctx *msg_ctx);

/* End of Function Prototypes *************************************************/
                         
AXIS2_EXTERN axis2_handler_t* AXIS2_CALL
savan_out_handler_create(
    const axis2_env_t *env, 
    axis2_qname_t *qname) 
{
    axis2_handler_t *handler = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    handler = axis2_handler_create(env);
    if (!handler)
    {
        return NULL;
    }
   
    /* handler init is handled by conf loading, so no need to do it here */
    
    /* set the base struct's invoke op */
    if (handler->ops) 
        handler->ops->invoke = savan_out_handler_invoke;

    return handler;
}

/******************************************************************************/

axis2_status_t AXIS2_CALL
savan_out_handler_invoke(
    struct axis2_handler *handler, 
    const axis2_env_t *env,
    struct axis2_msg_ctx *msg_ctx)
{
    savan_message_types_t msg_type = SAVAN_MSG_TYPE_UNKNOWN;
    /*axis2_svc_t *svc = NULL;*/
    axis2_param_t *param = NULL;
    axis2_hash_t *store = NULL;
    const axis2_svc_t *svc = NULL;
    const axis2_char_t *svc_name = NULL;
    axis2_hash_index_t *hi = NULL;
    void *val = NULL;
    
    AXIS2_ENV_CHECK( env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
    
    svc =  axis2_msg_ctx_get_svc(msg_ctx, env);
    if (svc)
        svc_name = axis2_svc_get_name (svc, env);
    
    printf("[%s][savan][out handler] invoke...\n", svc_name);
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[%s][savan][out handler] invoke...",
        svc_name);
    
    /* Determine the eventing msg type */
    msg_type = savan_util_get_message_type(msg_ctx, env);
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[%s][savan][out handler] msg type:"
        " %d", svc_name, msg_type);
    if (msg_type == SAVAN_MSG_TYPE_UNKNOWN)
    {
        /* Treat unknown msgs as msgs for publishing */
        
        svc =  axis2_msg_ctx_get_svc(msg_ctx, env);
        if (!svc)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[savan][out handler] "
                "Service not found");
            return AXIS2_SUCCESS; /* returning FAILURE will break handler chain */
        }
        
        param = axis2_svc_get_param(svc, env, SUBSCRIBER_STORE);
        if (!param)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[savan][out handler] "
                "Subscribe store not found");
            return AXIS2_SUCCESS; /* returning FAILURE will break handler chain */
        }
        
        store = (axis2_hash_t*)axis2_param_get_value(param, env);
        if (!store)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[savan][out handler] "
                "Subscribe store is null");
            return AXIS2_SUCCESS; /* returning FAILURE will break handler chain */
        }
        
        /* Iterate the subscribe store and send the msg to each one */
        
        for (hi = axis2_hash_first(store, env); hi; hi = axis2_hash_next(env, hi))
        {
            savan_subscriber_t * sub = NULL;
            axis2_hash_this(hi, NULL, NULL, &val);
            sub = (savan_subscriber_t *)val;
            if (sub)
            {
                axis2_char_t *id = savan_subscriber_get_id(sub, env);
                printf("[savan][out handler] Publishing to %s...\n", id);
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[savan][out handler] "
                    "Publishing to %s...", id);
                savan_subscriber_publish(sub, env, msg_ctx);
            }
        
            val = NULL;
        }

         axis2_msg_ctx_set_paused(msg_ctx, env, AXIS2_TRUE);
    }
       
    return AXIS2_SUCCESS;
}
