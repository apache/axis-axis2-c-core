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

#include <axiom_node.h>
#include <axiom_element.h>
#include <axiom_soap_body.h>
#include <axis2_options.h>
#include <platforms/axis2_platform_auto_sense.h>

#include <savan_publishing_client.h>
#include <savan_constants.h>

struct savan_publishing_client_t
{
    axis2_conf_ctx_t *conf_ctx;
    axis2_svc_t *svc;
};

/******************************************************************************/


/******************************************************************************/

AXIS2_EXTERN savan_publishing_client_t * AXIS2_CALL
savan_publishing_client_create(
    const axutil_env_t *env,
    axis2_conf_ctx_t *conf_ctx,
    axis2_svc_t *svc)
{
    savan_publishing_client_t *client = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    client = AXIS2_MALLOC(env->allocator, sizeof(savan_publishing_client_t));
     
    if (!client)
    { 
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }
    
    client->conf_ctx = conf_ctx;
    client->svc = svc;

    return client;
}

/******************************************************************************/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
savan_publishing_client_publish(
    savan_publishing_client_t *client,
    const axutil_env_t *env,
    axiom_node_t *payload)
{
    const axis2_char_t *repo_path = NULL;
    const axis2_char_t *address = NULL;
    axis2_endpoint_ref_t* endpoint_ref = NULL;
    axis2_options_t *options = NULL;
    axis2_svc_client_t* svc_client = NULL;
    axis2_qname_t *op_qname = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[savan][pub client] "
        "publish...");
    
    repo_path = AXIS2_GETENV("AXIS2C_HOME");
    
    /* Set end point reference of a dummy service */
    address = "http://localhost:9090/axis2/services/dummy";
    
    /* Create EPR with given address */
    endpoint_ref = axis2_endpoint_ref_create(env, address);

    /* Setup options */
    options = axis2_options_create(env);
    axis2_options_set_to(options, env, endpoint_ref);
    axis2_options_set_action(options, env,
            "http://ws.apache.org/axis2/c/savan/dummy");
    
    op_qname = axis2_qname_create(env, "publish", NULL, NULL);
        
    /* Create service client */
    svc_client = axis2_svc_client_create_with_conf_ctx_and_svc(env, repo_path,
        client->conf_ctx, client->svc);
    
    if (!svc_client)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[savan] Failed to create a"
            "service client for publishing"); 
        return AXIS2_FAILURE;
    }

    /* Set service client options */
    axis2_svc_client_set_options(svc_client, env, options);

    axis2_svc_client_engage_module(svc_client, env, "savan");

    /* Send publishing message */
    axis2_svc_client_send_robust_with_op_qname(svc_client, env, op_qname, payload); 
    
    return AXIS2_SUCCESS;
}
