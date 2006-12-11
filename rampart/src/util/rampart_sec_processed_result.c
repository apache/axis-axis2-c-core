/*
 *   Copyright 2003-2004 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

/* 
 *
 */
#include <rampart_util.h>
#include <axis2_util.h>
#include <axis2_property.h>
#include <axis2_msg_ctx.h>
#include <rampart_constants.h>
#include <rampart_sec_processed_result.h>

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_set_security_processed_result(const axis2_env_t *env,
        axis2_msg_ctx_t *msg_ctx,
        axis2_char_t *key,
        void *value)
{
    axis2_hash_t *sec_processed_results = NULL;

    sec_processed_results = rampart_get_all_security_processed_results(env, msg_ctx);
    if(!sec_processed_results){
        return AXIS2_FAILURE;
    }
    axis2_hash_set(sec_processed_results, key, AXIS2_HASH_KEY_STRING, value);
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[rampart][spr] Set %s in Security Processed Results of message context ", key);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN void *AXIS2_CALL
rampart_get_security_processed_result(const axis2_env_t *env,
        axis2_msg_ctx_t *msg_ctx,
        axis2_char_t *key)
{
    axis2_hash_t *sec_processed_results = NULL;

    sec_processed_results = rampart_get_all_security_processed_results(env, msg_ctx);
    if(!sec_processed_results){
        return NULL;
    }
    return axis2_hash_get(sec_processed_results, key, AXIS2_HASH_KEY_STRING);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_set_security_processed_results_property(const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_hash_t *sec_processed_results = NULL;
    axis2_property_t *sec_processed_results_prop = NULL;

    if(!msg_ctx){
        return AXIS2_FAILURE;
    }

    sec_processed_results = axis2_hash_make(env);
    sec_processed_results_prop = axis2_property_create(env);
    
    AXIS2_PROPERTY_SET_VALUE(sec_processed_results_prop, env, sec_processed_results);
    AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env, RAMPART_SECURITY_PROCESSED_RESULTS, sec_processed_results_prop, AXIS2_FALSE);

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_hash_t* AXIS2_CALL
rampart_get_all_security_processed_results(const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_property_t *sec_processed_results_prop = NULL;
    axis2_hash_t *sec_processed_results = NULL;

    sec_processed_results_prop = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env, RAMPART_SECURITY_PROCESSED_RESULTS, AXIS2_FALSE);
    if(!sec_processed_results_prop){
         AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[rampart][spr] Cannot get %s from msg ctx ", RAMPART_SECURITY_PROCESSED_RESULTS);
        return NULL;
    }

    sec_processed_results = (axis2_hash_t*)AXIS2_PROPERTY_GET_VALUE(sec_processed_results_prop, env);
    if(!sec_processed_results){
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[rampart][spr] Cannot get Security Processed Results Hash table from the property");
        return NULL;
    }

    return sec_processed_results;
}

AXIS2_EXTERN void AXIS2_CALL
rampart_print_security_processed_results_set(const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_hash_t *sec_processed_results = NULL;
    axis2_hash_index_t *hi = NULL;
    const void *key = NULL;
    void *val = NULL;
    
    sec_processed_results = rampart_get_all_security_processed_results(env, msg_ctx);
    if(!sec_processed_results){
        return;
    }
    
    for (hi = axis2_hash_first(sec_processed_results, env); hi; hi = axis2_hash_next(env, hi)) {
        axis2_hash_this(hi, &key, NULL, &val);
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[rampart][spr] (key, val) %s = %s\n", (axis2_char_t*)key, (axis2_char_t*)val);
    }

}
