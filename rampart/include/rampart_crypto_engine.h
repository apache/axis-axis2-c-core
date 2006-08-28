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
#include <axis2_env.h>
#include <axiom_soap.h>
#include <axis2_msg_ctx.h>
#include <rampart_action.h>

/**
  * @file rampart_crypto_engine.h
  * @brief 
  */
#ifndef RAMPART_CRYPTO_ENGINE
#define RAMPART_CRYPTO_ENGINE

#ifdef __cplusplus
extern "C" {
#endif

    /** Type name for struct rampart_crypto_engine_ops */
    typedef struct rampart_crypto_engine_ops rampart_crypto_engine_ops_t;

    /** Type name for struct rampart_crypto_engine */
    typedef struct rampart_crypto_engine rampart_crypto_engine_t;

    /**
     * rampart_crypto_engine ops struct
     * Encapsulator struct for ops of rampart_crypto_engine
     */
    struct rampart_crypto_engine_ops
    {
        /**
         * Deallocate memory
         * @param engine pointer to engine
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    rampart_crypto_engine_t *engine,
                    const axis2_env_t *env);

        axis2_status_t (AXIS2_CALL *
        encrypt_message) (
                rampart_crypto_engine_t *engine,
                const axis2_env_t *env,
                axis2_msg_ctx_t *msg_ctx,
                rampart_actions_t *actions,
                axiom_soap_envelope_t *soap_envelope,
                axiom_node_t *sec_node 
                );
        
        axis2_status_t (AXIS2_CALL *
        decrypt_message) (
                rampart_crypto_engine_t *engine,
                const axis2_env_t *env,
                axis2_msg_ctx_t *msg_ctx,
                rampart_actions_t *actions,
                axiom_soap_envelope_t *soap_envelope,
                axiom_node_t *sec_node 
                );

    };


    /**
     * Engine struct
     */
    struct rampart_crypto_engine
    {
        /** Operations of rampart_crypto_engine */
        rampart_crypto_engine_ops_t *ops;
    };

    /**
     * Creates rampart_crypto_engine struct
     * @param env pointer to environment struct
     * @return pointer to newly created rampart_crypto_engine
     */
    AXIS2_EXTERN rampart_crypto_engine_t *AXIS2_CALL
    rampart_crypto_engine_create (
        const axis2_env_t *env);


/*************************** Function macros **********************************/

#define RAMPART_CRYPTO_ENGINE_FREE(engine, env) \
      ((engine)->ops->free (engine, env))

#define RAMPART_CRYPTO_ENGINE_ENCRYPT_MESSAGE(engine, env, msg_ctx, actions, soap_envelope, sec_node ) \
      ((engine)->ops->encrypt_message(engine, env, msg_ctx, actions, soap_envelope, sec_node))

#define RAMPART_CRYPTO_ENGINE_DECRYPT_MESSAGE(engine, env, msg_ctx, actions, soap_envelope, sec_node ) \
      ((engine)->ops->decrypt_message(engine, env, msg_ctx, actions, soap_envelope, sec_node))


/* @} */
#ifdef __cplusplus
}
#endif

#endif    /* !RAMPART_CRYPTO_H */
