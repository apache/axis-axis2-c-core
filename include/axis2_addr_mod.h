
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

#ifndef AXIS2_ADDR_MOD_H
#define AXIS2_ADDR_MOD_H

    /**
     * @defgroup axis2_mod_addr WS-Addressing Module
     * @ingroup axis2
     * @{
     * @}
     */

    /**
     * @defgroup axis2_addr_mod addressing module interface
     * @ingroup axis2_mod_addr
     * @{
     */

    /**
     * @file axis2_addr_mod.h
     */

#include <axis2_handler.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define ADDR_IN_HANDLER "AddressingInHandler"
#define ADDR_OUT_HANDLER "AddressingOutHandler"

/**
     * Creates Addressing in handler
     * @param env pointer to environment struct
     * @param name name of handler
     * @return returns reference to handler created
     */
    AXIS2_EXTERN axis2_handler_t *AXIS2_CALL
    axis2_addr_in_handler_create(
        const axutil_env_t * env,
        axutil_string_t * name);

    /**
     * Creates Addressing out handler
     * @param env pointer to environment struct
     * @param name name of handler
     * @return returns reference to handler created
     */
    AXIS2_EXTERN axis2_handler_t *AXIS2_CALL
    axis2_addr_out_handler_create(
        const axutil_env_t * env,
        axutil_string_t * name);

    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_ADDR_MOD_H */
