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

#ifndef RAMPART_CONTEXT_H
#define RAMPART_CONTEXT_H

/** @defgroup rampart_context 
 * @ingroup rampart_context
 * @{
 */

#include <rp_includes.h>
#include <rp_secpolicy.h>
#include <rampart_authn_provider.h>
#include <rampart_util.h>
#include <axis2_property.h>
#include <rampart_constants.h>
#include <rampart_callback.h>
#include <rampart_authn_provider.h>

#ifdef __cplusplus
extern "C"
{
#endif
   
    typedef axis2_char_t *(AXIS2_CALL*
            pfunc)(const axis2_env_t *env,
            const axis2_char_t *username,
            void *param);

    typedef rampart_authn_provider_status_t (AXIS2_CALL*
            auth_password_func)(const axis2_env_t* env,
                const axis2_char_t *username,
                const axis2_char_t *password);

    typedef rampart_authn_provider_status_t (AXIS2_CALL*
            auth_digest_func)(const axis2_env_t* env,
                const axis2_char_t *username,
                const axis2_char_t *nonce,
                const axis2_char_t *created,
                const char *digest);

    typedef struct rampart_context_t rampart_context_t;

    AXIS2_EXTERN rampart_context_t *AXIS2_CALL
    rampart_context_create(const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rampart_context_free(rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN rp_secpolicy_t *AXIS2_CALL
    rampart_context_get_secpolicy(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rampart_context_set_secpolicy(rampart_context_t *rampart_context,
            const axis2_env_t *env,
            rp_secpolicy_t *secpolicy);

    AXIS2_EXTERN rampart_callback_t *AXIS2_CALL
    rampart_context_get_password_callback(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rampart_context_set_password_callback(rampart_context_t *rampart_context,
            const axis2_env_t *env,
            rampart_callback_t *password_callback_module);

    AXIS2_EXTERN pfunc AXIS2_CALL
    rampart_context_get_password_function(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rampart_context_set_password_function(rampart_context_t *rampart_context,
            const axis2_env_t *env,
            pfunc password_func);

    AXIS2_EXTERN auth_password_func AXIS2_CALL
    rampart_context_get_auth_password_function(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rampart_context_set_auth_password_function(rampart_context_t *rampart_context,
            const axis2_env_t *env,
            auth_password_func authenticate_with_password);

    AXIS2_EXTERN auth_digest_func AXIS2_CALL
    rampart_context_get_auth_digest_function(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rampart_context_set_auth_digest_function(rampart_context_t *rampart_context,
            const axis2_env_t *env,
            auth_digest_func authenticate_with_digest);

    AXIS2_EXTERN rampart_authn_provider_t *AXIS2_CALL
    rampart_context_get_authn_provider(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rampart_context_set_authn_provider(rampart_context_t *rampart_context,
            const axis2_env_t *env,
            rampart_authn_provider_t *authn_provider);

    AXIS2_EXTERN int AXIS2_CALL
    rampart_context_get_binding_type(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);
    
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rampart_context_is_include_timestamp(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rampart_context_is_include_username_token(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    rampart_context_get_user(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    rampart_context_get_password_callback_class(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    rampart_context_get_authn_module_name(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rampart_context_get_password_type(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rampart_context_is_encrypt_before_sign(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);
    
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rampart_context_get_nodes_to_encrypt(
            rampart_context_t *rampart_context,
            const axis2_env_t *env,
            axiom_soap_envelope_t *soap_envelope,
            axis2_array_list_t *nodes_to_encrypt);

    AXIS2_EXTERN rp_property_t *AXIS2_CALL 
    rampart_context_get_token(
            rampart_context_t *rampart_context,
            const axis2_env_t *env,
            axis2_bool_t for_encryption,
            axis2_bool_t server_side);
        
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rampart_context_check_is_derived_keys(
            const axis2_env_t *env,
            rp_property_t *token);
 
    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    rampart_context_get_enc_sym_algo(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    rampart_context_get_enc_asym_algo(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    rampart_context_get_encryption_prop_file(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    rampart_context_get_encryption_user(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    rampart_context_get_enc_key_identifier(
            rampart_context_t *rampart_context,
            rp_property_t *token,
            axis2_bool_t server_side,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    rampart_context_get_layout(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    rampart_context_get_decryption_prop_file(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rampart_context_check_whether_to_encrypt(
            rampart_context_t *rampart_context,
            const axis2_env_t *env);


#ifdef __cplusplus
}
#endif
#endif  
