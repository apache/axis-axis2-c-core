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

#ifndef RAMPART_AUTHN_PROVIDER_H
#define RAMPART_AUTHN_PROVIDER_H


#ifdef __cplusplus
extern "C"
{
#endif
    /*typedef enum {
        AUTHN_PROVIDER_DENIED,
        AUTHN_PROVIDER_GRANTED,
        AUTHN_PROVIDER_FOUND,
        AUTHN_PROVIDER_USER_NOT_FOUND,
        AUTHN_PROVIDER_GENERAL_ERROR
    } rampart_authn_provider_status;
    */
    /**
     * Struct to authenticate username/password pair
     * @defgroup rampart_authn_provider rampart authn_provider
     * @ingroup rampart_util
     * @{
     */

    /** 
     * Type name for struct rampart_authn_provider_ops 
     */
    typedef struct rampart_authn_provider_ops rampart_authn_provider_ops_t;

    /**
     * Type name for struct rampart_authn_provider
     */

    typedef struct rampart_authn_provider rampart_authn_provider_t;
    
    /**
     */
     struct rampart_authn_provider_ops
    {            
           

            axis2_status_t (AXIS2_CALL*
            rampart_authn_provider_check_password)(
                rampart_authn_provider_t *authn_provider,
                const axis2_env_t* env,
                axis2_msg_ctx_t *msg_ctx,
                const axis2_char_t *username, 
                const axis2_char_t *password
            );
            
            axis2_status_t (AXIS2_CALL*
            rampart_authn_provider_check_password_digest)(
                rampart_authn_provider_t *authn_provider,
                const axis2_env_t* env,
                axis2_msg_ctx_t *msg_ctx,
                const char *nonce, 
                size_t nonce_length, 
                const char *digest
            );

            axis2_status_t (AXIS2_CALL*
            free)(rampart_authn_provider_t *authn_provider,
                  const axis2_env_t* env);

    };

    struct rampart_authn_provider
    {
        rampart_authn_provider_ops_t *ops;
    };

/*************************** Function macros **********************************/
/*TODO write the implementation*/
#define RAMPART_AUTHN_PROVIDER_FREE(authn_provider, env) \
      ((authn_provider)->ops->free (authn_provider, env))

#define RAMPART_AUTHN_PROVIDER_CHECK_PASSWORD(authn_provider, env, msg_ctx, username, password) \
      ((authn_provider)->ops->authn_provider_password(authn_provider, env, msg_ctx, username, password))

#define RAMPART_AUTHN_PROVIDER_CHECK_PASSWORD_DIGEST(authn_provider, env, msg_ctx, nonce, nonce_length, digest) \
      ((authn_provider)->ops->authn_provider_password(authn_provider, env, msg_ctx, nonce, nonce_length, digest))

    /** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* RAMPART_AUTHN_PROVIDER_H */

