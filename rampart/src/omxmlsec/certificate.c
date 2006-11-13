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

#include <stdio.h>
#include <axis2_util.h>
#include <oxs_constants.h>
#include <oxs_certificate.h>


typedef struct oxs_certificate_impl
{
    oxs_certificate_t certificate;

    axis2_char_t *serial_number;
    axis2_char_t *subject;
    axis2_char_t *issuer;
    axis2_char_t *data;
}
oxs_certificate_impl_t;


/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(oxs_certificate) ((oxs_certificate_impl_t *)oxs_certificate)

/******************* function headers ******************************/
/* private functions */
static void
oxs_certificate_init_ops(
    oxs_certificate_t *certificate);

/*public functions*/
axis2_status_t AXIS2_CALL
oxs_certificate_free_impl(
    oxs_certificate_t *certificate,
    const axis2_env_t *env
);


axis2_char_t *AXIS2_CALL
oxs_certificate_get_serial_number_impl(
    const oxs_certificate_t *certificate,
    const axis2_env_t *env
);

axis2_char_t *AXIS2_CALL
oxs_certificate_get_subject_impl(
    const oxs_certificate_t *certificate,
    const axis2_env_t *env
);

axis2_char_t *AXIS2_CALL
oxs_certificate_get_issuer_impl(
    const oxs_certificate_t *certificate,
    const axis2_env_t *env
);

axis2_char_t *AXIS2_CALL
oxs_certificate_get_data_impl(
    const oxs_certificate_t *certificate,
    const axis2_env_t *env
);


axis2_status_t AXIS2_CALL
oxs_certificate_set_serial_number_impl(
    oxs_certificate_t *certificate,
    const axis2_env_t *env,
    axis2_char_t *serial_number
);

axis2_status_t AXIS2_CALL
oxs_certificate_set_subject_impl(
    oxs_certificate_t *certificate,
    const axis2_env_t *env,
    axis2_char_t *subject
);

axis2_status_t AXIS2_CALL
oxs_certificate_set_issuer_impl(
    oxs_certificate_t *certificate,
    const axis2_env_t *env,
    axis2_char_t *issuer
);

axis2_status_t AXIS2_CALL
oxs_certificate_set_data_impl(
    oxs_certificate_t *certificate,
    const axis2_env_t *env,
    axis2_char_t *data
);


/******************* end of function headers ******************************/
AXIS2_EXTERN oxs_certificate_t *AXIS2_CALL
oxs_certificate_create(const axis2_env_t *env)
{
    oxs_certificate_impl_t *certificate_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    certificate_impl = AXIS2_MALLOC(env->allocator, sizeof(oxs_certificate_impl_t));
    if (!certificate_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    certificate_impl->serial_number  = NULL;
    certificate_impl->subject  = NULL;
    certificate_impl->issuer  = NULL;
    certificate_impl->data  = NULL;


    certificate_impl->certificate.ops =  AXIS2_MALLOC(env->allocator, sizeof(oxs_certificate_ops_t));
    if (!certificate_impl->certificate.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        oxs_certificate_free(&(certificate_impl->certificate), env);
        return NULL;
    }
    oxs_certificate_init_ops(&(certificate_impl->certificate));

    return &(certificate_impl->certificate);
}

/* private functions */
static void
oxs_certificate_init_ops(
    oxs_certificate_t *certificate)
{
    certificate->ops->free = oxs_certificate_free_impl ;

    certificate->ops->get_serial_number= oxs_certificate_get_serial_number_impl ;
    certificate->ops->get_subject = oxs_certificate_get_subject_impl ;
    certificate->ops->get_issuer = oxs_certificate_get_issuer_impl ;
    certificate->ops->get_data = oxs_certificate_get_data_impl ;

    certificate->ops->set_serial_number = oxs_certificate_set_serial_number_impl ;
    certificate->ops->set_subject = oxs_certificate_set_subject_impl ;
    certificate->ops->set_issuer = oxs_certificate_set_issuer_impl ;
    certificate->ops->set_data = oxs_certificate_set_data_impl ;
}

/*public functions*/
axis2_status_t AXIS2_CALL
oxs_certificate_free_impl(oxs_certificate_t *certificate,
        const axis2_env_t *env
            )
{
    oxs_certificate_impl_t * certificate_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    certificate_impl = AXIS2_INTF_TO_IMPL(certificate);


    if (certificate_impl->serial_number)
    {
        AXIS2_FREE(env->allocator, certificate_impl->serial_number);
        certificate_impl->serial_number = NULL;
    }

    if (certificate_impl->subject)
    {
        AXIS2_FREE(env->allocator, certificate_impl->subject);
        certificate_impl->subject = NULL;
    }

    if (certificate_impl->issuer)
    {
        AXIS2_FREE(env->allocator, certificate_impl->issuer);
        certificate_impl->issuer = NULL;
    }

    if (certificate_impl->data)
    {
        AXIS2_FREE(env->allocator, certificate_impl->data);
        certificate_impl->data = NULL;
    }

    AXIS2_FREE(env->allocator,  certificate_impl);
    certificate_impl = NULL;

    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
oxs_certificate_get_serial_number_impl(
    const oxs_certificate_t *certificate,
    const axis2_env_t *env
)
{
    oxs_certificate_impl_t * certificate_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    certificate_impl = AXIS2_INTF_TO_IMPL(certificate);

    return certificate_impl->serial_number ;
}

axis2_char_t *AXIS2_CALL
oxs_certificate_get_subject_impl(
    const oxs_certificate_t *certificate,
    const axis2_env_t *env
)
{
    oxs_certificate_impl_t * certificate_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    certificate_impl = AXIS2_INTF_TO_IMPL(certificate);

    return certificate_impl->subject ;
}

axis2_char_t *AXIS2_CALL
oxs_certificate_get_issuer_impl(
    const oxs_certificate_t *certificate,
    const axis2_env_t *env
)
{
    oxs_certificate_impl_t * certificate_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    certificate_impl = AXIS2_INTF_TO_IMPL(certificate);

    return certificate_impl->issuer ;
}

axis2_char_t *AXIS2_CALL
oxs_certificate_get_data_impl(
    const oxs_certificate_t *certificate,
    const axis2_env_t *env
)
{
    oxs_certificate_impl_t * certificate_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    certificate_impl = AXIS2_INTF_TO_IMPL(certificate);

    return certificate_impl->data ;
}

axis2_status_t AXIS2_CALL
oxs_certificate_set_serial_number_impl(
    oxs_certificate_t *certificate,
    const axis2_env_t *env,
    axis2_char_t *serial_number
)
{
    oxs_certificate_impl_t * certificate_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, serial_number, AXIS2_FAILURE);

    certificate_impl = AXIS2_INTF_TO_IMPL(certificate);
    if (certificate_impl->serial_number)
    {
        AXIS2_FREE(env->allocator, certificate_impl->serial_number);
        certificate_impl->serial_number = NULL;
    }
    certificate_impl->serial_number = AXIS2_STRDUP(serial_number, env);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_certificate_set_subject_impl(
    oxs_certificate_t *certificate,
    const axis2_env_t *env,
    axis2_char_t *subject
)
{
    oxs_certificate_impl_t * certificate_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, subject, AXIS2_FAILURE);

    certificate_impl = AXIS2_INTF_TO_IMPL(certificate);
    if (certificate_impl->subject)
    {
        AXIS2_FREE(env->allocator, certificate_impl->subject);
        certificate_impl->subject = NULL;
    }
    certificate_impl->subject = AXIS2_STRDUP(subject, env);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_certificate_set_issuer_impl(
    oxs_certificate_t *certificate,
    const axis2_env_t *env,
    axis2_char_t *issuer
)
{
    oxs_certificate_impl_t * certificate_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, issuer, AXIS2_FAILURE);

    certificate_impl = AXIS2_INTF_TO_IMPL(certificate);
    if (certificate_impl->issuer)
    {
        AXIS2_FREE(env->allocator, certificate_impl->issuer);
        certificate_impl->issuer = NULL;
    }
    certificate_impl->issuer = AXIS2_STRDUP(issuer, env);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_certificate_set_data_impl(
    oxs_certificate_t *certificate,
    const axis2_env_t *env,
    axis2_char_t *data
)
{
    oxs_certificate_impl_t * certificate_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, data, AXIS2_FAILURE);

    certificate_impl = AXIS2_INTF_TO_IMPL(certificate);
    if (certificate_impl->data)
    {
        AXIS2_FREE(env->allocator, certificate_impl->data);
        certificate_impl->data = NULL;
    }
    certificate_impl->data = AXIS2_STRDUP(data, env);

    return AXIS2_SUCCESS;
}


/**********************Wrappers******************************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_certificate_free(oxs_certificate_t *certificate,
                    const axis2_env_t *env)
{
    return  certificate->ops->free(certificate, env);
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_certificate_get_serial_number(const oxs_certificate_t *certificate,
                    const axis2_env_t *env)
{
    return  certificate->ops->get_serial_number(certificate,env);   
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_certificate_get_subject(const oxs_certificate_t *certificate,
                    const axis2_env_t *env)
{
    return  certificate->ops->get_subject(certificate,env);   
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_certificate_get_issuer(const oxs_certificate_t *certificate,
                    const axis2_env_t *env)
{
    return  certificate->ops->get_issuer(certificate,env);   
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_certificate_get_data(const oxs_certificate_t *certificate,
                    const axis2_env_t *env)
{
    return  certificate->ops->get_data(certificate,env);   
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_certificate_set_serial_number(oxs_certificate_t *certificate,
                    const axis2_env_t *env,
                    axis2_char_t *serial_number)
{
    return  certificate->ops->set_serial_number(certificate,env, serial_number);   
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_certificate_set_subject(oxs_certificate_t *certificate,
                    const axis2_env_t *env,
                    axis2_char_t *subject)
{
    return  certificate->ops->set_subject(certificate,env, subject);   
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_certificate_set_issuer(oxs_certificate_t *certificate,
                    const axis2_env_t *env,
                    axis2_char_t *issuer)
{
    return  certificate->ops->set_issuer(certificate,env, issuer);   
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_certificate_set_data(oxs_certificate_t *certificate,
                    const axis2_env_t *env,
                    axis2_char_t *data)
{
    return  certificate->ops->set_data(certificate, env, data);   
}


