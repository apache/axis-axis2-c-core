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

#ifndef AXIS2_WODEN_SOAP_BINDING_FAULT_REF_EXTS_H
#define AXIS2_WODEN_SOAP_BINDING_FAULT_REF_EXTS_H

/**
 * @file axis2_woden_soap_binding_fault_ref_exts.h
 * @brief Axis2 Soap Binding Fault Reference Extensions Interface
 * This interface represents the properties from the SOAP namespace
 * added to the WSDL 2.0 <code>binding_fault_ref</code> component as part 
 * of the SOAP binding extension.
 * <p>
 * These include:
 * <ul>
 * <li>{soap modules}</li>
 * </ul> 
 * 
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/extensions/axis2_woden_component_exts.h>
#include <axis2_array_list.h>

/** @defgroup axis2_woden_soap_binding_fault_ref_exts Soap Binding Fault Reference Extensions
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_soap_binding_fault_ref_exts_base axis2_woden_soap_binding_fault_ref_exts_base_t;
typedef struct axis2_woden_soap_binding_fault_ref_exts axis2_woden_soap_binding_fault_ref_exts_t;
typedef struct axis2_woden_soap_binding_fault_ref_exts_ops axis2_woden_soap_binding_fault_ref_exts_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_soap_binding_fault_ref_exts_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *soap_binding_fault_ref_exts,
            axis2_env_t **env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *soap_binding_fault_ref_exts,
            axis2_env_t **env);

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *soap_binding_fault_ref_exts,
            axis2_env_t **env);
    /**
     * @return the base implementation class
     */
    struct axis2_woden_component_exts *(AXIS2_CALL *
    get_base_impl) (
            void *soap_binding_fault_ref_exts,
            axis2_env_t **env);

    axis2_array_list_t *(AXIS2_CALL *
    get_soap_modules) (
            void *binding_fault_ref_exts,
            axis2_env_t **env);

};

struct axis2_woden_soap_binding_fault_ref_exts
{
    axis2_woden_component_exts_t component_exts;
    axis2_woden_soap_binding_fault_ref_exts_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_soap_binding_fault_ref_exts_t *)
axis2_woden_soap_binding_fault_ref_exts_create(
        axis2_env_t **env);


/***************************Woden C Internal Methods***************************/
AXIS2_DECLARE(axis2_woden_soap_binding_fault_ref_exts_t *)
axis2_woden_soap_binding_fault_ref_exts_to_component_exts(
        void *soap_binding_fault_ref_exts,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_status_t)
axis2_woden_soap_binding_fault_ref_exts_resolve_methods(
        axis2_woden_soap_binding_fault_ref_exts_t *soap_binding_fault_ref_exts,
        axis2_env_t **env,
        axis2_woden_soap_binding_fault_ref_exts_t *soap_binding_fault_ref_exts_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_SOAP_BINDING_FAULT_REF_EXTS_FREE(soap_binding_fault_ref_exts, env) \
		(((axis2_woden_soap_binding_fault_ref_exts_t *) soap_binding_fault_ref_exts)->ops->free(soap_binding_fault_ref_exts, env))

#define AXIS2_WODEN_SOAP_BINDING_FAULT_REF_EXTS_SUPER_OBJS(soap_binding_fault_ref_exts, env) \
		(((axis2_woden_soap_binding_fault_ref_exts_t *) soap_binding_fault_ref_exts)->ops->super_objs(soap_binding_fault_ref_exts, env))

#define AXIS2_WODEN_SOAP_BINDING_FAULT_REF_EXTS_TYPE(soap_binding_fault_ref_exts, env) \
		(((axis2_woden_soap_binding_fault_ref_exts_t *) soap_binding_fault_ref_exts)->ops->type(soap_binding_fault_ref_exts, env))

#define AXIS2_WODEN_SOAP_BINDING_FAULT_REF_EXTS_GET_BASE_IMPL(soap_binding_fault_ref_exts, env) \
		(((axis2_woden_soap_binding_fault_ref_exts_t *) soap_binding_fault_ref_exts)->ops->get_base_impl(soap_binding_fault_ref_exts, env))

#define AXIS2_WODEN_SOAP_BINDING_FAULT_REF_EXTS_GET_SOAP_MODULES(soap_binding_fault_ref_exts, env) \
		(((axis2_woden_soap_binding_fault_ref_exts_t *) soap_binding_fault_ref_exts)->ops->\
        get_soap_modules (soap_binding_fault_ref_exts, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_SOAP_BINDING_FAULT_REF_EXTS_H */
