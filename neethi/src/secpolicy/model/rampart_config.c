
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

#include <rp_rampart_config.h>

struct rp_rampart_config_t
{
    axis2_char_t *user;
    axis2_char_t *encryption_user;
    axis2_char_t *password_callback_class;
    axis2_char_t *authenticate_module;
    axis2_char_t *replay_detector;
    axis2_char_t *sct_provider;
    axis2_char_t *password_type;
    axis2_char_t *time_to_live;
    axis2_char_t *clock_skew_buffer;
    axis2_char_t *need_millisecond_precision;
    axis2_char_t *receiver_certificate_file;
    axis2_char_t *certificate_file;
    axis2_char_t *private_key_file;
	axis2_char_t *pkcs12_file;
    axis2_char_t *rd_val;
    int ref;
};

AXIS2_EXTERN rp_rampart_config_t *AXIS2_CALL
rp_rampart_config_create(
    const axutil_env_t * env)
{
    rp_rampart_config_t *rampart_config = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    rampart_config = (rp_rampart_config_t *) AXIS2_MALLOC(env->allocator,
                                                          sizeof
                                                          (rp_rampart_config_t));

    if (rampart_config == NULL)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    rampart_config->user = NULL;
    rampart_config->encryption_user = NULL;
    rampart_config->password_callback_class = NULL;
    rampart_config->private_key_file = NULL;
    rampart_config->receiver_certificate_file = NULL;
    rampart_config->certificate_file = NULL;
    rampart_config->authenticate_module = NULL;
    rampart_config->replay_detector = NULL;
    rampart_config->sct_provider = NULL;
    rampart_config->password_type = NULL;
    rampart_config->time_to_live = NULL;
    rampart_config->clock_skew_buffer = NULL;
    rampart_config->need_millisecond_precision = NULL;
	rampart_config->pkcs12_file = NULL;
    rampart_config->rd_val = NULL;
    rampart_config->ref = 0;

    return rampart_config;
}

AXIS2_EXTERN void AXIS2_CALL
rp_rampart_config_free(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (rampart_config)
    {
        if (--(rampart_config->ref) > 0)
        {
            return;
        }

        AXIS2_FREE(env->allocator, rampart_config);
        rampart_config = NULL;
    }
    return;
}

/* Implementations */

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_user(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return rampart_config->user;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_user(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env,
    axis2_char_t * user)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, user, AXIS2_FAILURE);

    rampart_config->user = user;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_encryption_user(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return rampart_config->encryption_user;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_encryption_user(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env,
    axis2_char_t * encryption_user)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, encryption_user, AXIS2_FAILURE);

    rampart_config->encryption_user = encryption_user;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_password_callback_class(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return rampart_config->password_callback_class;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_password_callback_class(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env,
    axis2_char_t * password_callback_class)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, password_callback_class, AXIS2_FAILURE);

    rampart_config->password_callback_class = password_callback_class;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_authenticate_module(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return rampart_config->authenticate_module;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_authenticate_module(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env,
    axis2_char_t * authenticate_module)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, authenticate_module, AXIS2_FAILURE);

    rampart_config->authenticate_module = authenticate_module;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_replay_detector(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return rampart_config->replay_detector;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_replay_detector(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env,
    axis2_char_t * replay_detector)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, replay_detector, AXIS2_FAILURE);

    rampart_config->replay_detector = replay_detector;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_sct_provider(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return rampart_config->sct_provider;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_sct_provider(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env,
    axis2_char_t * sct_module)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, sct_module, AXIS2_FAILURE);

    rampart_config->sct_provider = sct_module;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_password_type(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return rampart_config->password_type;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_password_type(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env,
    axis2_char_t * password_type)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, password_type, AXIS2_FAILURE);

    rampart_config->password_type = password_type;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_private_key_file(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return rampart_config->private_key_file;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_private_key_file(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env,
    axis2_char_t * private_key_file)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, private_key_file, AXIS2_FAILURE);

    rampart_config->private_key_file = private_key_file;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_receiver_certificate_file(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return rampart_config->receiver_certificate_file;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_receiver_certificate_file(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env,
    axis2_char_t * receiver_certificate_file)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, receiver_certificate_file, AXIS2_FAILURE);

    rampart_config->receiver_certificate_file = receiver_certificate_file;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_certificate_file(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return rampart_config->certificate_file;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_certificate_file(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env,
    axis2_char_t * certificate_file)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, certificate_file, AXIS2_FAILURE);

    rampart_config->certificate_file = certificate_file;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_time_to_live(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return rampart_config->time_to_live;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_time_to_live(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env,
    axis2_char_t * time_to_live)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, time_to_live, AXIS2_FAILURE);

    rampart_config->time_to_live = time_to_live;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_clock_skew_buffer(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env)
{
    return rampart_config->clock_skew_buffer;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_clock_skew_buffer(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env,
    axis2_char_t * clock_skew_buffer)
{
    rampart_config->clock_skew_buffer = clock_skew_buffer;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_need_millisecond_precision(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env)
{
    return rampart_config->need_millisecond_precision;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_need_millisecond_precision(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env,
    axis2_char_t * need_millisecond_precision)
{
    rampart_config->need_millisecond_precision = need_millisecond_precision;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_pkcs12_file(
	rp_rampart_config_t * rampart_config,
	const axutil_env_t * env)
{
	return rampart_config->pkcs12_file;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_pkcs12_file(
	rp_rampart_config_t * rampart_config,
	const axutil_env_t *env,
	axis2_char_t * pkcs12_file)
{
	if(rampart_config)
	{
		if(pkcs12_file)
		{
			rampart_config->pkcs12_file = pkcs12_file;
			return AXIS2_SUCCESS;
		}

		return AXIS2_FAILURE;
	}

	return AXIS2_FAILURE;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_rd_val(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return rampart_config->rd_val;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_rd_val(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env,
    axis2_char_t * rd_val)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, rd_val, AXIS2_FAILURE);

    rampart_config->rd_val = rd_val;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_increment_ref(
    rp_rampart_config_t * rampart_config,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    rampart_config->ref++;
    return AXIS2_SUCCESS;
}
