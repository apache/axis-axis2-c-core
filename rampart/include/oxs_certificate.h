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

#ifndef OXS_CERTIFICATE_H
#define OXS_CERTIFICATE_H


/**
  * @file oxs_certificate.h
  * @brief 
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axiom_node.h>
#include <oxs_certificate.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup oxs_certificate 
 * @ingroup 
 * @{
 */


    /**
     * Serial number
     * Subject
     * Issuer
     * Data
     */
    /** Type name for struct  oxs_certificate_ops */
    typedef struct oxs_certificate_ops oxs_certificate_ops_t;

    /** Type name for struct oxs_certificate */
    typedef struct oxs_certificate oxs_certificate_t;

    struct oxs_certificate_ops
    {

        axis2_status_t (AXIS2_CALL *
        free)(
                    oxs_certificate_t *certificate,
                    const axis2_env_t *env
                    );

        axis2_char_t *(AXIS2_CALL *
        get_serial_number)(
                    const oxs_certificate_t *certificate,
                    const axis2_env_t *env
                    );

        axis2_char_t *(AXIS2_CALL *
        get_subject)(
                    const oxs_certificate_t *certificate,
                    const axis2_env_t *env
                    );

        axis2_char_t *(AXIS2_CALL *
        get_issuer)(
                    const oxs_certificate_t *certificate,
                    const axis2_env_t *env
                    );

        axis2_char_t *(AXIS2_CALL *
        get_data)(
                    const oxs_certificate_t *certificate,
                    const axis2_env_t *env
                    );

        axis2_status_t (AXIS2_CALL *
        set_serial_number)(
                    oxs_certificate_t *certificate,
                    const axis2_env_t *env,
                    axis2_char_t *serial_number
                    );

        axis2_status_t (AXIS2_CALL *
        set_issuer)(
                    oxs_certificate_t *certificate,
                    const axis2_env_t *env,
                    axis2_char_t *issuer
                    );

        axis2_status_t (AXIS2_CALL *
        set_subject)(
                    oxs_certificate_t *certificate,
                    const axis2_env_t *env,
                    axis2_char_t *subject
                    );


        axis2_status_t (AXIS2_CALL *
        set_data)(
                    oxs_certificate_t *certificate,
                    const axis2_env_t *env,
                    axis2_char_t *data
                    );

    };
    
    struct oxs_certificate
    {
        /** operations of oxs_certificate */
        oxs_certificate_ops_t *ops;
    };

    /*Create function*/
    AXIS2_EXTERN oxs_certificate_t *AXIS2_CALL
    oxs_certificate_create(const axis2_env_t *env);


/**********************Wrappers******************************************/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_certificate_free(oxs_certificate_t *certificate,
                    const axis2_env_t *env);

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_certificate_get_serial_number(const oxs_certificate_t *certificate,
                    const axis2_env_t *env);

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_certificate_get_subject(const oxs_certificate_t *certificate,
                    const axis2_env_t *env);

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_certificate_get_issuer(const oxs_certificate_t *certificate,
                    const axis2_env_t *env);

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
oxs_certificate_get_data(const oxs_certificate_t *certificate,
                    const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_certificate_set_serial_number(oxs_certificate_t *certificate,
                    const axis2_env_t *env,
                    axis2_char_t *serial_number);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_certificate_set_subject(oxs_certificate_t *certificate,
                    const axis2_env_t *env,
                    axis2_char_t *subject);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_certificate_set_issuer(oxs_certificate_t *certificate,
                    const axis2_env_t *env,
                    axis2_char_t *issuer);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_certificate_set_data(oxs_certificate_t *certificate,
                    const axis2_env_t *env,
                    axis2_char_t *data);

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_CERTIFICATE_H */
