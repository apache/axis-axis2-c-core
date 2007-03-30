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

#ifndef AXIOM_SOAP_FAULT_ROLE_H
#define AXIOM_SOAP_FAULT_ROLE_H


/**
 * @file axiom_soap_fault_role.h
 * @brief axiom_soap_fault_role 
 */
#include <axutil_env.h>
#include <axiom_soap_fault.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axiom_soap_fault_role axiom_soap_fault_role_t;
    /**
     * @defgroup axiom_soap_fault_role soap fault role
     * @ingroup axiom_soap
     * @{
     */

    /**
      * creates a soap struct 
      * @param env Environment. MUST NOT be NULL
      */
    AXIS2_EXTERN axiom_soap_fault_role_t * AXIS2_CALL
    axiom_soap_fault_role_create_with_parent(const axutil_env_t *env,
            axiom_soap_fault_t *fault);

    /**
      * Free an axiom_soap_fault_role
      * @param  fault_role pointer to soap_fault_role struct
      * @param  env Environment. MUST NOT be NULL
      * @return satus of the op. AXIS2_SUCCESS on success 
      *         else AXIS2_FAILURE
      */

    AXIS2_EXTERN void AXIS2_CALL
    axiom_soap_fault_role_free(axiom_soap_fault_role_t *fault_role,
            const axutil_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_soap_fault_role_set_role_value(axiom_soap_fault_role_t *fault_role,
            const axutil_env_t *env,
            axis2_char_t* uri);

    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axiom_soap_fault_role_get_role_value(axiom_soap_fault_role_t *fault_role,
            const axutil_env_t *env);

    AXIS2_EXTERN axiom_node_t* AXIS2_CALL
    axiom_soap_fault_role_get_base_node(axiom_soap_fault_role_t *fault_role,
            const axutil_env_t *env);

    /** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIOM_SOAP_FAULT_ROLE_H */
