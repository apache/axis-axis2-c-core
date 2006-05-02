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

#ifndef AXIS2_ECHO_STUB_H
#define AXIS2_ECHO_STUB_H

/**
 * @file axis2_math_stub.h
 * @brief axis2 math stub interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_stub.h>
#include <axis2_endpoint_ref.h>
#include <axis2_om_node.h>

#ifdef __cplusplus
extern "C"
{
#endif

axis2_om_node_t *
axis2_math_stub_add(axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_om_node_t *node);

axis2_om_node_t *
axis2_math_stub_sub(axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_om_node_t *node);

axis2_om_node_t *
axis2_math_stub_mul(axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_om_node_t *node);

axis2_om_node_t *
axis2_math_stub_div(axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_om_node_t *node);

/**
 * Creates axis2_stub struct
 * @param endpoint reference
 * @return pointer to newly created axis2_stub struct
 */
axis2_stub_t *
axis2_math_stub_create_with_endpoint_ref_and_client_home(axis2_env_t **env,
                                            axis2_endpoint_ref_t *endpoint_ref,
                                            axis2_char_t *client_home);

/**
 * Creates axis2_stub struct
 * @param endpoint uri
 * @return pointer to newly created axis2_stub struct
 */
axis2_stub_t *
axis2_math_stub_create_with_endpoint_uri_and_client_home(axis2_env_t **env,
                                            axis2_char_t *endpoint_uri,
                                            axis2_char_t *client_home);
/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_ECHO_STUB_H */
