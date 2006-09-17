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
  
#ifndef W2C_EMITTER_H
#define W2C_EMITTER_H

/**
 * @file w2c_emitter.h
 * @brief represent command line option
 */

#include <axis2_utils.h>
#include <w2c_engine_configuration.h>
#include <w2c_config_property_loader.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_emitter represent command line option
 * @ingroup w2c_wsdl_utils
 * @{
 */

typedef struct w2c_emitter w2c_emitter_t;
typedef struct w2c_emitter_ops w2c_emitter_ops_t;

  struct w2c_emitter_ops
{
   /**
    * free w2c_emitter.
    * @param  emitter pointer to emitter struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    free)(w2c_emitter_t *emitter,
         const axis2_env_t *env);

   /**
    * pass configurations and properties.
    * @param  emitter pointer to emitter struct
    * @param  env Environment. MUST NOT be NULL
    * @config configurations derived from user options
    * @loader loaded configuration properties
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_config)(w2c_emitter_t *emitter,
          const axis2_env_t *env,
          w2c_engine_configuration_t *config,
          w2c_config_property_loader_t *loader );
 
   /**
    * emit skeleton code.
    * @param  emitter pointer to emitter struct
    * @param  env Environment. MUST NOT be NULL
    */
    axis2_status_t (AXIS2_CALL *
    emit_skel)(w2c_emitter_t *emitter,
          const axis2_env_t *env);
  
   /**
    * emit stub code.
    * @param  emitter pointer to emitter struct
    * @param  env Environment. MUST NOT be NULL
    */
    axis2_status_t (AXIS2_CALL *
    emit_stub)(w2c_emitter_t *emitter,
          const axis2_env_t *env);

};

  struct w2c_emitter
{
     struct w2c_emitter_ops *ops;
};

/** this is an interface for emitter */

/*************************** Function macros **********************************/

#define W2C_EMITTER_FREE(emitter, env) \
      ((emitter)->ops->free (emitter, env)) 

#define W2C_EMITTER_SET_CONFIG(emitter, env, config, loader) \
      ((emitter)->ops->set_config(emitter, env, config, loader))

#define W2C_EMITTER_EMIT_SKEL(emitter, env) \
      ((emitter)->ops->emit_skel(emitter, env ))

#define W2C_EMITTER_EMIT_STUB(emitter, env) \
      ((emitter)->ops->emit_stub(emitter, env ))

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_EMITTER_H */
