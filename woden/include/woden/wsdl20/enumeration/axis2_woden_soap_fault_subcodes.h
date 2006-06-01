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

#ifndef AXIS2_WODEN_SOAP_FAULT_SUBCODES_H
#define AXIS2_WODEN_SOAP_FAULT_SUBCODES_H

/**
 * @file axis2_woden_soap_fault_subcodes.h
 * @brief Axis2 Soap Fault Subcodes Interface
 * This class represents the {soap fault subcodes} property that forms part of the 
 * SOAP extensions to the WSDL <code>woden_binding_fault</code> component.
 * This property may contain either a list of QNames representing the subcodes or 
 * the xs:token #any.
 * <p>
 * This class will restrict the possible values to a collection of QNames or the string "#any".
 * It provides methods to query whether the property contains QNames or a token and 
 * methods to retrieve the property value of the appropriate type.
 * 
 * 
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <woden/axis2_woden.h>
#include <axis2_array_list.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_soap_fault_subcodes axis2_woden_soap_fault_subcodes_t;
typedef struct axis2_woden_soap_fault_subcodes_ops axis2_woden_soap_fault_subcodes_ops_t;
struct axis2_woden_ext_element;

/** @defgroup axis2_woden_soap_fault_subcodes Soap Fault Subcodes
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_soap_fault_subcodes_ops
{
	/** 
     * Deallocate memory
     * @return status subcodes
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *soap_fault_subcodes,
            const axis2_env_t *env);

    axis2_bool_t (AXIS2_CALL *
    is_qnames) (
            void *soap_fault_subcodes,
            const axis2_env_t *env);

    axis2_bool_t (AXIS2_CALL *
    is_token) (
            void *soap_fault_subcodes,
            const axis2_env_t *env);

    axis2_array_list_t *(AXIS2_CALL *
    get_qnames) (
            void *soap_fault_subcodes,
            const axis2_env_t *env);

    axis2_char_t *(AXIS2_CALL *
    get_token) (
            void *soap_fault_subcodes,
            const axis2_env_t *env);


};

struct axis2_woden_soap_fault_subcodes
{
    axis2_woden_soap_fault_subcodes_ops_t *ops;
    
};

AXIS2_DECLARE(axis2_woden_soap_fault_subcodes_t *)
axis2_woden_soap_fault_subcodes_create(
        const axis2_env_t *env,
        axis2_char_t *token,
        axis2_array_list_t *subcode_qns);

AXIS2_DECLARE(void *)
axis2_woden_soap_fault_subcodes_get_soap_fault_subcodes_any(
        const axis2_env_t *env);

#define AXIS2_WODEN_SOAP_FAULT_SUBCODES_FREE(soap_fault_subcodes, env) \
		(((axis2_woden_soap_fault_subcodes_t *) soap_fault_subcodes)->ops->\
         free (soap_fault_subcodes, env))

#define AXIS2_WODEN_SOAP_FAULT_SUBCODES_IS_QNAMES(soap_fault_subcodes, env) \
		(((axis2_woden_soap_fault_subcodes_t *) soap_fault_subcodes)->ops->\
         is_qnames (soap_fault_subcodes, env))

#define AXIS2_WODEN_SOAP_FAULT_SUBCODES_IS_TOKEN(soap_fault_subcodes, env) \
		(((axis2_woden_soap_fault_subcodes_t *) soap_fault_subcodes)->ops->\
         is_token (soap_fault_subcodes, env))

#define AXIS2_WODEN_SOAP_FAULT_SUBCODES_GET_QNAMES(soap_fault_subcodes, env) \
		(((axis2_woden_soap_fault_subcodes_t *) soap_fault_subcodes)->ops->\
         get_qnames (soap_fault_subcodes, env))

#define AXIS2_WODEN_SOAP_FAULT_SUBCODES_GET_TOKEN(soap_fault_subcodes, env) \
		(((axis2_woden_soap_fault_subcodes_t *) soap_fault_subcodes)->ops->\
         get_token (soap_fault_subcodes, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_SOAP_FAULT_SUBCODES_H */
