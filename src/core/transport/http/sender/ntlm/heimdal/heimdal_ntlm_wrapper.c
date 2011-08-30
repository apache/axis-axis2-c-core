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
#ifdef AXIS2_NTLM_ENABLED

#include <axis2_ntlm.h>
#include <axutil_utils_defines.h>
#include <axutil_utils.h>
#include <string.h>
#include <axutil_string.h>

#include <roken.h>
#include <krb5-types.h> /* or <inttypes.h> */
#include <heimntlm.h>
#include <openssl/rand.h>

void AXIS2_CALL
heimdal_ntlm_wrapper_free(
    axis2_ntlm_t * ntlm,
    const axutil_env_t * env);

axis2_status_t AXIS2_CALL
heimdal_ntlm_wrapper_create_type1_message(
    axis2_ntlm_t * ntlm,
    const axutil_env_t * env,
    axis2_char_t **encoded_message,
    int *encoded_len,
    const axis2_char_t *uname,
    const axis2_char_t *passwd,
    const int flags,
    const axis2_char_t *domain);

axis2_status_t AXIS2_CALL
heimdal_ntlm_wrapper_create_type3_message(
    axis2_ntlm_t * ntlm,
    const axutil_env_t * env,
    axis2_char_t *header_value,
    axis2_char_t **encoded_message,
    int *encoded_len,
    const axis2_char_t *uname,
    const axis2_char_t *passwd,
    const axis2_char_t *domain,
    const axis2_char_t *workstation);

typedef struct heimdal_ntlm_wrapper_impl_t
{
    axis2_ntlm_t ntlm;
} heimdal_ntlm_wrapper_impl_t;

#define AXIS2_INTF_TO_IMPL(p) ((heimdal_ntlm_wrapper_impl_t*)p)
#define AXIS2_IMPL_TO_INTF(p) &(p->ntlm)

static const axis2_ntlm_ops_t axis2_ntlm_ops_var = {
    heimdal_ntlm_wrapper_free,
    heimdal_ntlm_wrapper_create_type1_message,
    heimdal_ntlm_wrapper_create_type3_message };

AXIS2_EXTERN axis2_ntlm_t *AXIS2_CALL
axis2_ntlm_create(
    const axutil_env_t *env)
{
    heimdal_ntlm_wrapper_impl_t *wrapper_impl = NULL;
    wrapper_impl = (heimdal_ntlm_wrapper_impl_t *)AXIS2_MALLOC(env->allocator,
        sizeof(heimdal_ntlm_wrapper_impl_t));

    if(!wrapper_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No memory. Cannot create heimdal ntlm wrapper");
        return NULL;
    }
    memset(wrapper_impl, 0, sizeof(heimdal_ntlm_wrapper_impl_t));
    wrapper_impl->ntlm.ops = &axis2_ntlm_ops_var;
    return &(wrapper_impl->ntlm);
}

/**
 */
void AXIS2_CALL
heimdal_ntlm_wrapper_free(
    axis2_ntlm_t * ntlm,
    const axutil_env_t * env)
{
    heimdal_ntlm_wrapper_impl_t *ntlm_impl = NULL;
    ntlm_impl = AXIS2_INTF_TO_IMPL(ntlm);

    AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(ntlm));
    return;
}

axis2_status_t AXIS2_CALL
heimdal_ntlm_wrapper_create_type1_message(
    axis2_ntlm_t * ntlm,
    const axutil_env_t * env,
    axis2_char_t **encoded_message,
    int *encoded_len,
    const axis2_char_t *user,
    const axis2_char_t *password,
    const int flags,
    const axis2_char_t *domain)
{
	struct ntlm_type1 type1;
    struct ntlm_buf data;
    int ret;
    heimdal_ntlm_wrapper_impl_t *ntlm_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ntlm_impl = AXIS2_INTF_TO_IMPL(ntlm);


    memset(&type1, 0, sizeof(type1));

    if(flags != 0)
    {
        type1.flags = NTLM_NEG_UNICODE|NTLM_NEG_TARGET|NTLM_NEG_NTLM|flags;
    }
    else
    {
        type1.flags = NTLM_NEG_UNICODE|NTLM_NEG_TARGET|NTLM_NEG_NTLM;
    }
    if(domain)
    {
        type1.domain = rk_UNCONST(domain);
    }
    else
    {
        type1.domain = NULL;
    }
    type1.hostname = NULL;
    type1.os[0] = 0;
    type1.os[1] = 0;

    ret = heim_ntlm_encode_type1(&type1, &data);
    if (ret)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "[heimdal_wrapper] heim_ntlm_encode_type1 call failed");
        return AXIS2_FAILURE;
    }

    *encoded_message = AXIS2_MALLOC(env->allocator, (2 * data.length));
    if(!*encoded_message)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "[heimdal_wrapper] Memory allocation for encoded message failed");
        return AXIS2_FAILURE;
    }
    *encoded_len = axutil_base64_encode_binary(*encoded_message, data.data, data.length);

    if(data.data)
    {
        free(data.data);
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
heimdal_ntlm_wrapper_create_type3_message(
    axis2_ntlm_t * ntlm,
    const axutil_env_t * env,
    axis2_char_t *header_value,
    axis2_char_t **encoded_message,
    int *encoded_len,
    const axis2_char_t *user,
    const axis2_char_t *password,
    const axis2_char_t *domain,
    const axis2_char_t *workstation)
{
    
	heimdal_ntlm_wrapper_impl_t *ntlm_impl = NULL;
    struct ntlm_type3 type3;
    struct ntlm_type2 type2;
    struct ntlm_buf data;
    int ret;
    int len;

	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ntlm_impl = AXIS2_INTF_TO_IMPL(ntlm);
    
    memset(&type2, 0, sizeof(type2));

    len = axutil_strlen(header_value);
    len = 2 * len + 1;
	data.data = AXIS2_MALLOC(env->allocator, sizeof(char)*len);
    data.length = axutil_base64_decode_binary(data.data, header_value);
    ret = heim_ntlm_decode_type2(&data, &type2);
	AXIS2_FREE(env->allocator, data.data);
	data.data = NULL;
    if (ret)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "[heimdal_wrapper] heim_ntlm_decode_type2 call failed");
        return AXIS2_FAILURE;
    }

    memset(&type3, 0, sizeof(type3));

    type3.flags = type2.flags;
    type3.username = rk_UNCONST(user);
    type3.targetname = rk_UNCONST(type2.targetname);
    if(workstation)
    {
        type3.ws = rk_UNCONST(workstation);
    }
    else
    {
        type3.ws = NULL;
    }
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "type3.flags:***%d***", type3.flags);
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "type3.username:***%s***", type3.username);
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "type3.targetname:***%s***", type3.targetname);
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "type3.ws:***%s***", type3.ws);

    {
        struct ntlm_buf key;
        heim_ntlm_nt_key(password, &key);
        
         /* NTLM Version 1 if no targetinfo buffer.*/
         
        if (1 || type2.targetinfo.length == 0) 
        {

            if (type2.flags & NTLM_NEG_NTLM2_SESSION) 
            {
				unsigned char nonce[8];
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[heimdal_wrapper] ntlm2 session");
                

                if (RAND_bytes(nonce, sizeof(nonce)) != 1) 
                {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "[heimdal_wrapper] RAND_bytes function call failed");
                    return AXIS2_FAILURE;
                }

                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[heimdal_wrapper] calculate_ntlm2_sess");
                ret = heim_ntlm_calculate_ntlm2_sess(nonce,
                                 type2.challange,
                                 key.data,
                                 &type3.lm,
                                 &type3.ntlm);
            } 
            else 
            {
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[heimdal_wrapper] calculate_ntlm1");
                ret = heim_ntlm_calculate_ntlm1(key.data,
                        key.length,
                        type2.challange,
                        &type3.ntlm);

            }
            if (ret) 
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "[heimdal_wrapper] heim_ntlm_calculate_ntlm1 function call failed");
                return AXIS2_FAILURE;
            }

            if (type2.flags & NTLM_NEG_KEYEX)
            {
                struct ntlm_buf sessionkey;

                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[heimdal_wrapper] build_ntlm1_master");
                ret = heim_ntlm_build_ntlm1_master(key.data,
                               key.length,
                               &sessionkey,
                               &type3.sessionkey);
                if (ret) 
                {
                    if (type3.lm.data)
                    {
                        free(type3.lm.data);
                    }
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "[heimdal_wrapper] heim_ntlm_build_ntlm1_master function call failed");
                    return AXIS2_FAILURE;
                }

                free(sessionkey.data);
            }

        } 
        else 
        {
            unsigned char ntlmv2[16];
            struct ntlm_targetinfo ti;

            /* verify infotarget */

            ret = heim_ntlm_decode_targetinfo(&type2.targetinfo, 1, &ti);
            if(ret) 
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "[heimdal_wrapper] heim_ntlm_decode_targetinfo function call failed");
                return AXIS2_FAILURE;
            }

            if (ti.domainname && domain && strcmp(ti.domainname, domain) != 0) 
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "[heimdal_wrapper] heim_ntlm_decode_targetinfo function returned wrong data");
                return AXIS2_FAILURE;
            }

            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[heimdal_wrapper] calculate_ntlm2");
            ret = heim_ntlm_calculate_ntlm2(key.data,
                            key.length,
                            user,
                            domain,
                            type2.challange,
                            &type2.targetinfo,
                            ntlmv2,
                            &type3.ntlm);
            if (ret) 
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "[heimdal_wrapper] heim_ntlm_calculate_ntlm2 function call failed");
                return AXIS2_FAILURE;
            }

            if (type2.flags & NTLM_NEG_KEYEX)
            {
                struct ntlm_buf sessionkey;
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[heimdal_wrapper] build_ntlm1_master");
                ret = heim_ntlm_build_ntlm1_master(ntlmv2, sizeof(ntlmv2),
                                   &sessionkey,
                                   &type3.sessionkey);
                memset(ntlmv2, 0, sizeof(ntlmv2));
                if (ret) 
                {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "[heimdal_wrapper] heim_ntlm_build_ntlm1_master function call failed");
                    return AXIS2_FAILURE;
                }

                free(sessionkey.data);
            }
        }
       
        if(key.data)
        {
            free(key.data);
        }
    }

    ret = heim_ntlm_encode_type3(&type3, &data);
    if (ret)
    {
        heim_ntlm_free_type2(&type2);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "[heimdal_wrapper] heim_ntlm_encode_type3 call failed");
        return AXIS2_FAILURE;
    }

    *encoded_message = AXIS2_MALLOC(env->allocator, (2 * data.length));
    if(!*encoded_message)
    {
        heim_ntlm_free_type2(&type2);
        free(type3.ntlm.data);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "[heimdal_wrapper] Memory allocation for encoded message failed");
        return AXIS2_FAILURE;
    }
    *encoded_len = axutil_base64_encode_binary(*encoded_message, data.data, data.length);
    heim_ntlm_free_type2(&type2);
    free(type3.ntlm.data);

    return AXIS2_SUCCESS;
}

#endif
