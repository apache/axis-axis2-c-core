/*
 *Copyright 2004,2005 The Apache Software Foundation.
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
#include <axis2_qname.h>
#include <axis2_svc.h>
#include <axiom_soap_header.h>
#include <axiom_soap_body.h>
#include <axiom_soap_header_block.h>
#include <axis2_endpoint_ref.h>
#include <axis2_property.h>
#include <rampart_constants.h>
#include <rampart_crypto_util.h>
#include <rampart_util.h>
#include <rampart_timestamp_token.h>
#include <oxs_axiom.h>

typedef struct rampart_timestamp_token_impl
{
    rampart_timestamp_token_t timestamp_token;
}
rampart_timestamp_token_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(timestamp_token) ((rampart_timestamp_token_impl_t *)timestamp_token)

/*************************** Function headers *********************************/
/** private functions */
static void
rampart_timestamp_token_init_ops(
    rampart_timestamp_token_t *timestamp_token);


/** public functions*/
axis2_status_t AXIS2_CALL
rampart_timestamp_token_free(rampart_timestamp_token_t *timestamp_token,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
rampart_timestamp_token_build(rampart_timestamp_token_t *timestamp_token,
        const axis2_env_t *env,
        const axis2_ctx_t *ctx,
        axiom_node_t *sec_node,
        const  axiom_namespace_t *sec_ns_obj,
        int ttl);

axis2_status_t AXIS2_CALL
rampart_timestamp_token_validate(rampart_timestamp_token_t *timestamp_token,
        const axis2_env_t *env,
        axiom_node_t *ts_node,
        axis2_array_list_t *sub_codes);

/*************************** end of function headers *********************************/
static void
rampart_timestamp_token_init_ops(
    rampart_timestamp_token_t *timestamp_token)
{
    timestamp_token->ops->free = rampart_timestamp_token_free;
    timestamp_token->ops->build = rampart_timestamp_token_build;
    timestamp_token->ops->validate = rampart_timestamp_token_validate;
}
rampart_timestamp_token_t *AXIS2_CALL
rampart_timestamp_token_create(
    const axis2_env_t *env)
{
    rampart_timestamp_token_impl_t *timestamp_token_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    timestamp_token_impl = (rampart_timestamp_token_impl_t *) AXIS2_MALLOC(env->allocator,
            sizeof(rampart_timestamp_token_impl_t));

    if (NULL == timestamp_token_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    timestamp_token_impl->timestamp_token.ops = AXIS2_MALLOC(env->allocator,
            sizeof(rampart_timestamp_token_ops_t));
    if (NULL == timestamp_token_impl->timestamp_token.ops)
    {
        rampart_timestamp_token_free(&(timestamp_token_impl->timestamp_token), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    rampart_timestamp_token_init_ops(&(timestamp_token_impl->timestamp_token));

    return &(timestamp_token_impl->timestamp_token);

}

axis2_status_t AXIS2_CALL
rampart_timestamp_token_free(rampart_timestamp_token_t *timestamp_token,
        const axis2_env_t *env)
{
    rampart_timestamp_token_impl_t *timestamp_token_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    timestamp_token_impl = AXIS2_INTF_TO_IMPL(timestamp_token);

    if (timestamp_token->ops)
    {
        AXIS2_FREE(env->allocator, timestamp_token->ops);
        timestamp_token->ops = NULL;
    }
    if (timestamp_token_impl)
    {
        AXIS2_FREE(env->allocator, timestamp_token_impl);
        timestamp_token_impl = NULL;
    }
    return AXIS2_SUCCESS;

}

axis2_status_t AXIS2_CALL
rampart_timestamp_token_build(rampart_timestamp_token_t *timestamp_token,
        const axis2_env_t *env,
        const axis2_ctx_t *ctx,
        axiom_node_t *sec_node,
        const  axiom_namespace_t *sec_ns_obj,
        int ttl)
{
    axiom_node_t *ts_node = NULL;
    axiom_node_t *created_node = NULL;
    axiom_node_t *expires_node = NULL;
    axiom_element_t *ts_ele = NULL;
    axiom_element_t *created_ele = NULL;
    axiom_element_t *expires_ele = NULL;
    axis2_char_t *created_val = NULL;
    axis2_char_t *expires_val = NULL;
    axiom_namespace_t *wsu_ns_obj = NULL;

    wsu_ns_obj = axiom_namespace_create(env, RAMPART_WSU_XMLNS,
            RAMPART_WSU);

    ts_ele = axiom_element_create(env, sec_node,
            RAMPART_SECURITY_TIMESTAMP,
            wsu_ns_obj,
            &ts_node);
    if (ts_ele)
    {
        /*First we build Created element*/
        created_ele = axiom_element_create(env, ts_node, RAMPART_SECURITY_TIMESTAMP_CREATED, wsu_ns_obj,
                &created_node);

        if (created_ele)
        {
            created_val = rampart_generate_time(env, 0);   /*Current time*/
            AXIOM_ELEMENT_SET_TEXT(created_ele, env, created_val, created_node);
        }
        /*Then we build Expires element*/
        /*If ttl<0 then we dont build the expires element.*/
        if(ttl<0){
            AXIS2_LOG_INFO(env->log, " [rampart][ts] ttl is %d which is less then zero. Thus we do not build the expires element. ");
            return AXIS2_SUCCESS;
        }
        expires_ele = axiom_element_create(env, ts_node, RAMPART_SECURITY_TIMESTAMP_EXPIRES, wsu_ns_obj,
                &expires_node);

        if (expires_ele)
        {
            expires_val = rampart_generate_time(env, ttl);
            AXIOM_ELEMENT_SET_TEXT(expires_ele, env, expires_val, expires_node);

        }
    }

    return AXIS2_SUCCESS;
}/*rampart_build_timestamp_token*/

axis2_status_t AXIS2_CALL
rampart_timestamp_token_validate(rampart_timestamp_token_t *timestamp_token,
        const axis2_env_t *env,
        axiom_node_t *ts_node,
        axis2_array_list_t *sub_codes)
{
    axis2_status_t validity = AXIS2_FAILURE;
    axiom_element_t *created_ele = NULL;
    axiom_element_t *expires_ele = NULL;
    axiom_element_t *ts_ele = NULL;
    axiom_node_t *created_node = NULL;
    axiom_node_t *expires_node = NULL;
    axis2_char_t *created_val = NULL;
    axis2_char_t *expires_val = NULL;
    axis2_char_t *current_val = NULL;

    /*Check: TIMESTAMP MUST contain exactly one CREATED*/
    if (1 !=  oxs_axiom_get_number_of_children_with_qname(env, ts_node, RAMPART_SECURITY_TIMESTAMP_CREATED, NULL, NULL))
    {
        AXIS2_ARRAY_LIST_ADD(sub_codes, env, "Timestamp must contain exactly one Created element");
        return AXIS2_FAILURE;
    }

    /*Check: TIMESTAMP MUST NOT contain more than one EXPIRES*/
    if (1 < oxs_axiom_get_number_of_children_with_qname(env, ts_node, RAMPART_SECURITY_TIMESTAMP_CREATED, NULL, NULL))
    {
        AXIS2_ARRAY_LIST_ADD(sub_codes, env, "Timestamp must not contain more than one Expires element");
        return AXIS2_FAILURE;
    }


    ts_ele = AXIOM_NODE_GET_DATA_ELEMENT(ts_node, env);

    if (!ts_ele)
    {
        AXIS2_ARRAY_LIST_ADD(sub_codes, env, "Cannot find Timestamp element");
        AXIS2_LOG_INFO(env->log, " Cannot find timestamp ... ");
        return AXIS2_FAILURE;
    }

    /*First child MUST be the Created element*/
    created_node = AXIOM_NODE_GET_FIRST_CHILD(ts_node, env);
    created_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(created_node, env);
    if (AXIS2_STRCMP(RAMPART_SECURITY_TIMESTAMP_CREATED ,
            AXIOM_ELEMENT_GET_LOCALNAME(created_ele, env)) != 0)
    {
        AXIS2_ARRAY_LIST_ADD(sub_codes, env, "The first elelment of a Timestamp token must be Created");
        AXIS2_LOG_INFO(env->log, "Cannot find created  in timestamp element. The first element MUST be CREATED");
        return AXIS2_FAILURE;
    }

    created_val = AXIOM_ELEMENT_GET_TEXT(created_ele, env, created_node);
    /*Check weather created is less than current time or not*/
    current_val = rampart_generate_time(env, 0);
    validity = rampart_compare_date_time(env, created_val, current_val);
    if (validity == AXIS2_FAILURE)
    {
        AXIS2_ARRAY_LIST_ADD(sub_codes, env, "Created time is not valid");
        return AXIS2_FAILURE;
    }

    /*Any TIMESTAMP containing an EXPIRES MUST contain a CREATED that preceeds its sibling EXPIRES.*/
    expires_node =  AXIOM_NODE_GET_NEXT_SIBLING(created_node, env);
    if (!expires_node)
    {
        AXIS2_LOG_INFO(env->log, "Cannot find expires  in timestamp element...");
        /*If the expire element is not present, it means that the message will not be expired.*/
        return AXIS2_SUCCESS;
    }
    expires_ele  = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(expires_node, env);
    if (AXIS2_STRCMP(RAMPART_SECURITY_TIMESTAMP_EXPIRES ,
            AXIOM_ELEMENT_GET_LOCALNAME(expires_ele, env)) != 0)
    {
        AXIS2_LOG_INFO(env->log, " The second element of timestamp token (if any) MUST be EXPIRES");
        AXIS2_ARRAY_LIST_ADD(sub_codes, env, "The second elelment must be Expires, if any");
        return AXIS2_FAILURE;
    }

    /*Now the expires element is present. So check weather this has a valid timestamp.
      If not it's a failure*/
    expires_val = AXIOM_ELEMENT_GET_TEXT(expires_ele, env, expires_node);

    /*Check weather time has expired or not*/
    validity = rampart_compare_date_time(env, current_val, expires_val);
    if (validity == AXIS2_FAILURE)
    {
        AXIS2_ARRAY_LIST_ADD(sub_codes, env, "Timestamp token has expired");
        return AXIS2_FAILURE;
    }

    /*free memory for qnames*/
    return validity;
}
