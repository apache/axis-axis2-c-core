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
 
#include <axiom.h>
#include <platforms/axis2_platform_auto_sense.h>

#include <axis2_endpoint_ref.h>
#include <axis2_svc_client.h>

#include <savan_subscriber.h>

struct savan_subscriber_t
{
    axis2_char_t *id;
    axis2_endpoint_ref_t *end_to;
    axis2_endpoint_ref_t *notify_to;
    axis2_char_t *delivery_mode;
    axis2_char_t *expires;
    axis2_char_t *filter;
    axis2_bool_t renewed;
};

/*****************************************************************************/

AXIS2_EXTERN savan_subscriber_t * AXIS2_CALL
savan_subscriber_create(
    const axis2_env_t *env)
{
    savan_subscriber_t *subscriber = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    subscriber = AXIS2_MALLOC(env->allocator, sizeof(savan_subscriber_t));
     
    if (!subscriber)
    { 
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }
    
    subscriber->id = NULL;
    subscriber->end_to = NULL;
    subscriber->notify_to = NULL;
    subscriber->delivery_mode = NULL;
    subscriber->expires = NULL;
    subscriber->filter = NULL;
    subscriber->renewed = AXIS2_FALSE;
        
    return subscriber;
}

/******************************************************************************/

axis2_char_t * AXIS2_CALL
savan_subscriber_get_id(
    savan_subscriber_t *subscriber,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    return subscriber->id;
}

/******************************************************************************/

axis2_status_t AXIS2_CALL
savan_subscriber_set_id(
    savan_subscriber_t *subscriber,
    const axis2_env_t *env,
    const axis2_char_t *id)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    /* If id is already set, free it */
    if (subscriber->id != NULL)
    {
        AXIS2_FREE(env->allocator, subscriber->id);
        subscriber->id = NULL;
    }
    
    /* copy the new id */
    subscriber->id = axis2_strdup(id, env);

    return AXIS2_SUCCESS;
}

/******************************************************************************/

axis2_status_t AXIS2_CALL
savan_subscriber_set_end_to(
    savan_subscriber_t *subscriber,
    const axis2_env_t *env,
    axis2_endpoint_ref_t *end_to)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    subscriber->end_to = end_to;

    return AXIS2_SUCCESS;
}

/******************************************************************************/

axis2_status_t AXIS2_CALL
savan_subscriber_set_notify_to(
    savan_subscriber_t *subscriber,
    const axis2_env_t *env,
    axis2_endpoint_ref_t *notify_to)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    subscriber->notify_to = notify_to;

    return AXIS2_SUCCESS;
}    
            
/******************************************************************************/

axis2_status_t AXIS2_CALL
savan_subscriber_set_delivery_mode(
    savan_subscriber_t *subscriber,
    const axis2_env_t *env,
    const axis2_char_t *mode)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    /* if already set, free it */
    if (subscriber->delivery_mode != NULL)
    {
        AXIS2_FREE(env->allocator, subscriber->delivery_mode);
        subscriber->delivery_mode = NULL;
    }
    
    /* copy the new one */
    subscriber->delivery_mode = axis2_strdup(mode, env);

    return AXIS2_SUCCESS;
}    
            
/******************************************************************************/

axis2_status_t AXIS2_CALL
savan_subscriber_set_expires(
    savan_subscriber_t *subscriber,
    const axis2_env_t *env,
    const axis2_char_t *expires)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    /* if already set, free it */
    if (subscriber->expires != NULL)
    {
        AXIS2_FREE(env->allocator, subscriber->expires);
        subscriber->expires = NULL;
    }
    
    /* copy the new one */
    subscriber->expires = axis2_strdup(expires, env);

    return AXIS2_SUCCESS;
}

/******************************************************************************/

axis2_char_t * AXIS2_CALL
savan_subscriber_get_expires(
    savan_subscriber_t *subscriber,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    return subscriber->expires;
}

/******************************************************************************/

axis2_status_t AXIS2_CALL
savan_subscriber_set_filter(
    savan_subscriber_t *subscriber,
    const axis2_env_t *env,
    const axis2_char_t *filter)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    /* if already set, free it */
    if (subscriber->filter != NULL)
    {
        AXIS2_FREE(env->allocator, subscriber->filter);
        subscriber->filter = NULL;
    }
    
    /* copy the new one */
    subscriber->filter = axis2_strdup(filter, env);

    return AXIS2_SUCCESS;
}

/******************************************************************************/

axis2_status_t AXIS2_CALL
savan_subscriber_publish(
    savan_subscriber_t *subscriber,
    const axis2_env_t *env,
    struct axis2_msg_ctx *msg_ctx)
{
    axis2_svc_client_t *svc_client = NULL;
    axis2_op_client_t *op_client = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_conf_t *conf = NULL;
    axis2_svc_t *svc = NULL;
    axis2_char_t *path = NULL;
    axis2_options_t *options = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    axis2_qname_t *op_qname = NULL;

    printf("[savan][subscribe] publish...\n");
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[savan][subscribe] publish...");
	
    path = AXIS2_GETENV("AXIS2C_HOME");
    conf_ctx =  axis2_msg_ctx_get_conf_ctx(msg_ctx, env);
    conf =  axis2_conf_ctx_get_conf(conf_ctx, env);

    /* Get anonymous service from conf. This will be null for the first time, 
     * but then it will be created when we create the svc_client */
    svc = axis2_conf_get_svc(conf, env, AXIS2_ANON_SERVICE);

    svc_client = axis2_svc_client_create_with_conf_ctx_and_svc(env, path,
        conf_ctx, svc);

    /* Setup options */
    options = axis2_options_create(env);
    axis2_options_set_to(options, env, subscriber->notify_to);
    
    /* Set service client options */
    axis2_svc_client_set_options(svc_client, env, options);

    /* Engage addressing module */
    /*axis2_svc_client_engage_module(svc_client, env, AXIS2_MODULE_ADDRESSING);*/
    
    op_qname = axis2_qname_create(env, AXIS2_ANON_OUT_ONLY_OP, NULL, NULL);

    op_client = axis2_svc_client_create_op_client(svc_client, env,
        op_qname);

    AXIS2_OP_CLIENT_ADD_MSG_CTX(op_client, env, msg_ctx);
    status = AXIS2_OP_CLIENT_EXECUTE(op_client, env, AXIS2_TRUE);
    
    return status;
}

/******************************************************************************/

axis2_status_t AXIS2_CALL
savan_subscriber_set_renew_status(
    savan_subscriber_t *subscriber,
    const axis2_env_t *env,
    axis2_bool_t renewed)
{
    subscriber->renewed = renewed;

    return AXIS2_SUCCESS;
}

/******************************************************************************/

axis2_bool_t AXIS2_CALL
savan_subscriber_get_renew_status(
    savan_subscriber_t *subscriber,
    const axis2_env_t *env)
{
    return subscriber->renewed;
}
