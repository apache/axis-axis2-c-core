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

#ifndef AXIS2_CONF_INIT_H
#define AXIS2_CONF_INIT_H

/**
 * @file axis2_conf_init.h
 * @brief Axis2 configuration initilizing functions
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_dep_engine.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_conf_init configuration initilizing functions
 * @ingroup axis2_deployment
 * @{
 */

/**
 * Builds the configuration for the Server
 * @param env Environment. MUST NOT be NULL
 * @param repo_name repository name
 * @return pointer to an instance of configuration context properly initialized
 */
AXIS2_DECLARE(axis2_conf_ctx_t*) build_conf_ctx (const axis2_env_t *env,
    axis2_char_t *repo_name);
/**
 * Builds the Configuration for the Client
 * @param env Environment. MUST NOT be NULL
 * @param axis2_home. axis2 home for client. 
 * @return pointer to an instance of configuration context properly initialized
 */
AXIS2_DECLARE(axis2_conf_ctx_t*) build_client_conf_ctx(const axis2_env_t *env,
        axis2_char_t *axis2_home);


/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_CONF_INIT_H */
