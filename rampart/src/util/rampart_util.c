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

/* 
 *
 */
#include <stdio.h>
#include <rampart_util.h>
#include <axis2_util.h>
#include <axis2_base64.h>
#include <axis2_property.h>
#include <time.h>
#include <oxs_buffer.h>
#include <openssl_util.h>
#include <axis2_msg_ctx.h>
#include <rampart_constants.h>
#include <rampart_callback.h>
#include <rampart_credentials.h>

/*Calculate the hash of concatenated string of
 * nonce, created and the password.
 *
 */
#define SIZE 256
#define SIZE_HASH 32
#define SIZE_NONCE 24


/*#define PRINTINFO 1 */

AXIS2_EXTERN void* AXIS2_CALL
rampart_load_module(const axis2_env_t *env,
    axis2_char_t *module_name)
{
    axis2_dll_desc_t *dll_desc = NULL;
    axis2_param_t *impl_info_param = NULL;
    void *ptr = NULL;

    dll_desc = axis2_dll_desc_create(env);
    AXIS2_DLL_DESC_SET_NAME(dll_desc, env, module_name);
    impl_info_param = axis2_param_create(env, NULL, NULL);
    AXIS2_PARAM_SET_VALUE(impl_info_param, env, dll_desc);
    axis2_class_loader_init(env);
    ptr = axis2_class_loader_create_dll(env, impl_info_param);

    if (!ptr)
    {
        AXIS2_LOG_INFO(env->log, "[rampart][rampart_util] Unable to load the module %s. ERROR", module_name);
        return NULL;
    }

    return ptr;
}

AXIS2_EXTERN rampart_credentials_status_t AXIS2_CALL
rampart_call_credentials(const axis2_env_t *env,
    rampart_credentials_t *cred_module,
    axis2_msg_ctx_t *msg_ctx,
    axis2_char_t **username,
    axis2_char_t **password)
{
    rampart_credentials_status_t cred_status = RAMPART_CREDENTIALS_GENERAL_ERROR;

    cred_status = RAMPART_CREDENTIALS_USERNAME_GET(cred_module, env, msg_ctx, username, password);
    return cred_status;
}

AXIS2_EXTERN rampart_credentials_t* AXIS2_CALL
rampart_load_credentials_module(const axis2_env_t *env,
    axis2_char_t *cred_module_name)
{
    rampart_credentials_t *cred = NULL;

    cred = (rampart_credentials_t*)rampart_load_module(env, cred_module_name);
    if (!cred)
    {
        AXIS2_LOG_INFO(env->log, "[rampart][rampart_util] Unable to identify the credentials  module %s. ERROR", cred_module_name);
        return AXIS2_FAILURE;
    }

    return cred;
}

AXIS2_EXTERN rampart_authn_provider_t* AXIS2_CALL
rampart_load_auth_module(const axis2_env_t *env,
    axis2_char_t *auth_module_name)
{
    rampart_authn_provider_t *authp = NULL;

    authp = (rampart_authn_provider_t*)rampart_load_module(env, auth_module_name);
    if (!authp)
    {
        AXIS2_LOG_INFO(env->log, "[rampart][rampart_util] Unable to identify the authentication module %s. ERROR", auth_module_name);
        return AXIS2_FAILURE;
    }

    return authp;
}

AXIS2_EXTERN rampart_authn_provider_status_t AXIS2_CALL
rampart_authenticate_un_pw(const axis2_env_t *env,
    rampart_authn_provider_t *authp,
    const axis2_char_t *username,
    const axis2_char_t *password,
    const axis2_char_t *nonce,/*Can be NULL if plain text*/
    const axis2_char_t *created,/*Can be NULL if plain text*/
    const axis2_char_t *password_type,
    axis2_msg_ctx_t *msg_ctx)
{
    rampart_authn_provider_status_t auth_status = RAMPART_AUTHN_PROVIDER_GENERAL_ERROR;

    if (!authp)
    {
        return RAMPART_AUTHN_PROVIDER_GENERAL_ERROR;
    }
    /*If password digest*/
    if(0 == AXIS2_STRCMP(password_type, RAMPART_PASSWORD_DIGEST_URI)){
        auth_status = RAMPART_AUTHN_PROVIDER_CHECK_PASSWORD_DIGEST(authp, env, msg_ctx, username, nonce, created, password);
    }else{
        auth_status = RAMPART_AUTHN_PROVIDER_CHECK_PASSWORD(authp, env, msg_ctx, username, password);
    }

    return auth_status;
}

AXIS2_EXTERN rampart_callback_t* AXIS2_CALL
rampart_load_pwcb_module(const axis2_env_t *env,
        axis2_char_t *callback_module_name)
{
    rampart_callback_t *cb = NULL;

    cb = (rampart_callback_t*)rampart_load_module(env, callback_module_name);
    if (!cb)
    {
        AXIS2_LOG_INFO(env->log, "[rampart][rampart_util] Unable to identify the callback module %s. ERROR", callback_module_name);
        return AXIS2_FAILURE;
    }

    return cb;

}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
rampart_callback_password(const axis2_env_t *env,
        rampart_callback_t *callback_module,
        const axis2_char_t *username)
{
    axis2_char_t *password = NULL;
    void *cb_prop_val= NULL;

    /*Get the password thru the callback*/
    password = RAMPART_CALLBACK_CALLBACK_PASSWORD(callback_module, env, username, cb_prop_val);

    AXIS2_LOG_INFO(env->log, "[rampart][rampart_usernametoken] Password taken from the callback module . SUCCESS");
    return password;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
rampart_generate_nonce(const axis2_env_t *env)
{
    oxs_buffer_t *buffer = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    char *rand_str = NULL;
    axis2_char_t* encoded_str = NULL;

    buffer = oxs_buffer_create(env);
    status = generate_random_data(env, buffer, 16);
    rand_str = (char*)OXS_BUFFER_GET_DATA(buffer, env);
    encoded_str = AXIS2_MALLOC(env->allocator, sizeof(char) * (SIZE_NONCE+1));
    axis2_base64_encode(encoded_str, rand_str, OXS_BUFFER_GET_SIZE(buffer, env));
    OXS_BUFFER_FREE(buffer, env);

    return encoded_str;
}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
rampart_generate_time(const axis2_env_t *env, int ttl)
{
    axis2_date_time_t *dt = NULL;
    axis2_char_t *dt_str = NULL;

    dt = axis2_date_time_create_with_offset(env, ttl);
    dt_str =  axis2_date_time_serialize_date_time(dt, env);
    axis2_date_time_free(dt, env);
    return dt_str;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_compare_date_time(const axis2_env_t *env, axis2_char_t *dt1_str, axis2_char_t *dt2_str)
{
    axis2_status_t status = AXIS2_FAILURE;
    axis2_date_time_t *dt1 = NULL;
    axis2_date_time_t *dt2 = NULL;
    int yyyy1, mm1, dd1, hh1, mi1, ss1, ml1;
    int yyyy2, mm2, dd2, hh2, mi2, ss2, ml2;


    dt1 = axis2_date_time_create(env);
    dt2 = axis2_date_time_create(env);

    status =  axis2_date_time_deseriasize_date_TIME(dt1, env, dt1_str);
    if (status == AXIS2_FAILURE)
    {
        return AXIS2_FAILURE;
    }

    yyyy1 =  axis2_date_time_get_year(dt1, env);
    mm1 =  axis2_date_time_get_month(dt1, env);
    dd1 =  axis2_date_time_get_date(dt1, env);
    hh1 =   axis2_date_time_get_hour(dt1, env);
    mi1 =  axis2_date_time_get_minute(dt1, env);
    ss1 =  axis2_date_time_get_second(dt1, env);
    ml1 =  axis2_date_time_get_msec(dt1, env);

    status =  axis2_date_time_deseriasize_date_TIME(dt2, env, dt2_str);
    if (status == AXIS2_FAILURE)
    {
        return AXIS2_FAILURE;
    }

    yyyy2 =  axis2_date_time_get_year(dt2, env);
    mm2 =  axis2_date_time_get_month(dt2, env);
    dd2 =  axis2_date_time_get_date(dt2, env);
    hh2 =   axis2_date_time_get_hour(dt2, env);
    mi2 =  axis2_date_time_get_minute(dt2, env);
    ss2 =  axis2_date_time_get_second(dt2, env);
    ml2 =  axis2_date_time_get_msec(dt2, env);
    /**
    Comparison.
    We expect dt1_str < dt2_str/ Otherwise FAILURE
    */
    if (yyyy1 < yyyy2)
    {
        return AXIS2_SUCCESS;
    }
    else if (yyyy1 > yyyy2)
    {
        return AXIS2_FAILURE;
    }

    if (mm1 < mm2)
    {
        return AXIS2_SUCCESS;
    }
    else if (mm1 > mm2)
    {
        return AXIS2_FAILURE;
    }

    if (dd1 < dd2)
    {
        return AXIS2_SUCCESS;
    }
    else if (dd1 > dd2)
    {
        return AXIS2_FAILURE;
    }

    if (hh1 < hh2)
    {
        return AXIS2_SUCCESS;
    }
    else if (hh1 > hh2)
    {
        return AXIS2_FAILURE;
    }
     if (mi1 < mi2)
    {
        return AXIS2_SUCCESS;
    }
    else if (mi1 > mi2)
    {
        return AXIS2_FAILURE;
    }

    if (ss1 < ss2)
    {
        return AXIS2_SUCCESS;
    }
    else if (ss1 > ss2)
    {
        return AXIS2_FAILURE;
    }

    if (ml1 < ml2)
    {
        return AXIS2_SUCCESS;
    }
    else if (ml1 > ml2)
    {
        return AXIS2_FAILURE;
    }

    /*axis2_date_time_free(dt1, env);
    axis2_date_time_free(dt2, env);*/

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_print_info(const axis2_env_t *env, axis2_char_t* info)
{
#ifdef PRINTINFO
    printf("[rampart]: %s\n", info);
#endif
    return AXIS2_SUCCESS;
}



