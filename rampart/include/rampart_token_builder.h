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



#include <axis2_utils_defines.h>
#include <axis2_defines.h>
#include <axis2_date_time.h>
#include <axis2_env.h>
#include <axis2_property.h>
#include <axis2_msg_ctx.h>
#include <rampart_authn_provider.h>
#include <rampart_credentials.h>
#include <rampart_callback.h>
#include <oxs_x509_cert.h>
/**
  * @file rampart_token_builder.h
  * @brief Reference Token builfing/of rampart
  */
#ifndef RAMPART_TOKEN_BUILDER_H
#define RAMPART_TOKEN_BUILDER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RTBP_UNKNOWN = 0,
    RTBP_EMBEDDED,
    RTBP_KEY_IDENTIFIER,
    RTBP_X509DATA_ISSUER_SERIAL,
    RTBP_X509DATA_X509CERTIFICATE
} rampart_token_build_pattern_t;


AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_token_build_security_token_reference(const axis2_env_t *env,
    axiom_node_t *parent,
    oxs_x509_cert_t *cert,
    rampart_token_build_pattern_t pattern);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_token_build_embedded(const axis2_env_t *env,
    axiom_node_t *parent,
    oxs_x509_cert_t *cert);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_token_build_key_identifier(const axis2_env_t *env,
    axiom_node_t *parent,
    oxs_x509_cert_t *cert);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_token_build_x509_data(const axis2_env_t *env,
    axiom_node_t *parent,
    oxs_x509_cert_t *cert);
/* @} */
#ifdef __cplusplus
}
#endif

#endif    /* !RAMPART_TOKEN_BUILDER_H */
                                                                                                                           

