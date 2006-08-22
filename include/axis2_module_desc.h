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
#ifndef AXIS2_MODULE_DESC_H
#define AXIS2_MODULE_DESC_H

/** 
 * @defgroup axis2_module_desc module description
 * @ingroup axis2_desc
 * module holds information about a module. This information includes module 
 * parameters and handler information. 
 * Modules are avalibe to all services if axis2.xml has a module reference 
 * entry. Alternatively, a module could be made avalible to selected services
 * by including a module reference entry in services.xml.
 * @{
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_array_list.h>
#include <axis2_param_container.h>
#include <axis2_flow_container.h>
#include <axis2_param.h>
#include <axis2_op.h>
#include <axis2_conf.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_module_desc_ops */
    typedef struct axis2_module_desc_ops axis2_module_desc_ops_t;
    /** Type name for struct axis2_module_desc */
    typedef struct axis2_module_desc axis2_module_desc_t;

    struct axis2_op;
    struct axis2_conf;


    /**
     * module description ops struct.
     * Encapsulator struct for ops of axis2_module_desc.
     */
    struct axis2_module_desc_ops
    {
        /**
         * Frees module description.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_module_desc_t *module_desc,
                    const axis2_env_t *env);

        /**
         * Gets flow representing in flow.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @return pointer to flow that represents in flow, returns a reference
         * not a cloned copy
         */
        axis2_flow_t *(AXIS2_CALL *
                get_in_flow)(
                    const axis2_module_desc_t *module_desc,
                    const axis2_env_t *env);

        /**
         * Sets flow representing in flow.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @param in_flow pointer to flow representing in flow, module assumes 
         * ownership of flow
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_in_flow)(
                    axis2_module_desc_t *module_desc,
                    const axis2_env_t *env,
                    axis2_flow_t *in_flow);

        /**
         * Gets flow representing out flow.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @return pointer to flow that represents out flow, returns a reference
         * not a cloned copy         
         */
        axis2_flow_t *(AXIS2_CALL *
                get_out_flow)(
                    const axis2_module_desc_t *module_desc,
                    const axis2_env_t *env);

        /**
         * Sets flow representing out flow.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @param out_flow pointer to flow representing out flow, module assumes 
         * ownership of flow
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_out_flow)(
                    axis2_module_desc_t *module_desc,
                    const axis2_env_t *env,
                    axis2_flow_t *out_flow);

        /**
         * Gets flow representing fault in flow.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @return pointer to flow that represents fault in flow, returns a reference
         * not a cloned copy         
         */
        axis2_flow_t *(AXIS2_CALL *
                get_fault_in_flow)(
                    const axis2_module_desc_t *module_desc,
                    const axis2_env_t *env);

        /**
         * Sets flow representing fault in flow.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @param falut_in_flow pointer to flow representing fault in flow, 
         * module assumes ownership of flow
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_fault_in_flow)(
                    axis2_module_desc_t *module_desc,
                    const axis2_env_t *env,
                    axis2_flow_t *falut_in_flow);

        /**
         * Gets flow representing fault out flow.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @return pointer to flow that represents fault out flow, returns a 
         * reference not a cloned copy         
         */
        axis2_flow_t *(AXIS2_CALL *
                get_fault_out_flow)(
                    const axis2_module_desc_t *module_desc,
                    const axis2_env_t *env);

        /**
         * Sets flow representing fault out flow.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @param fault_out_flow pointer to flow representing fault out flow, 
         * module assumes ownership of flow
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_fault_out_flow)(
                    axis2_module_desc_t *module_desc,
                    const axis2_env_t *env,
                    axis2_flow_t *fault_out_flow);

        /**
         * Gets module QName.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @return pointer to QName
         */
        const axis2_qname_t *(AXIS2_CALL *
                get_qname)(
                    const axis2_module_desc_t *module_desc,
                    const axis2_env_t *env);

        /**
         * Sets module QName.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @param qname pointer to qname
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_qname)(
                    axis2_module_desc_t *module_desc,
                    const axis2_env_t *env,
                    const axis2_qname_t *qname);

        /**
         * Adds given operation to module.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @param op pointer to operation, module assumes ownership of operation
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_op)(
                    axis2_module_desc_t *module_desc,
                    const axis2_env_t *env,
                    struct axis2_op *op);

        /**
         * Gets all operations associated with module.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @return pointer to hash map containing the operations
         */
        axis2_hash_t *(AXIS2_CALL *
                get_all_ops)(
                    const axis2_module_desc_t *module_desc,
                    const axis2_env_t *env);

        /**
         * Gets parent which is of type configuration.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @return pointer to configuration, returns a reference not a 
         * cloned copy
         */
        struct axis2_conf *(AXIS2_CALL *
                get_parent)(
                    const axis2_module_desc_t *module_desc,
                    const axis2_env_t *env);

        /**
         * Sets parent which is of type configuration.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @param parent pointer to parent configuration, module does not assume
         * the ownership of configuration
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_parent)(
                    axis2_module_desc_t *module_desc,
                    const axis2_env_t *env,
                    struct axis2_conf *parent);

        /**
         * Add parameter
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @param param param
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_param)(
                    axis2_module_desc_t *module_desc,
                    const axis2_env_t *env,
                    axis2_param_t *param);

        /**
         * Gets parameter with given name.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @param name parameter name string
         * @return pointer to parameter corresponding to given name
         */
        axis2_param_t *(AXIS2_CALL *
                get_param)(
                    const axis2_module_desc_t *module_desc,
                    const axis2_env_t *env,
                    const axis2_char_t *name);

        /**
         * Gets all parameters associated with module.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @param pointer to array list containing all parameters
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_all_params)(
                    const axis2_module_desc_t *module_desc,
                    const axis2_env_t *env);

        /**
         * Checks if a given paramter is locked.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @param param_name parameter name string 
         * @return AXIS2_TRUE if named parameter is locked, else AXIS2_FALSE
         */
        axis2_bool_t (AXIS2_CALL *
                is_param_locked)(
                    const axis2_module_desc_t *module_desc,
                    const axis2_env_t *env,
                    const axis2_char_t *param_name);

        /**
         * Gets module associated with module description.
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @return pointer to module
         */
        struct axis2_module *(AXIS2_CALL *
                get_module)(
                    const axis2_module_desc_t *module_desc,
                    const axis2_env_t *env);

        /**
         * @param module_desc pointer to module description
         * @param env pointer to environment struct
         * @param module pointer to module, module description assumes ownership
         * of module
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_module)(
                    axis2_module_desc_t *module_desc,
                    const axis2_env_t *env,
                    struct axis2_module *module);

    };

    /**
     * module description struct.
     */
    struct axis2_module_desc
    {
        /** operations of module description */
        axis2_module_desc_ops_t *ops;
        /** 
         * flow container that encapsulates the flows associated with the 
         * module 
         */
        axis2_flow_container_t *flow_container;
        /** 
         * parameter container that stores all the parameters associated with 
         * the module 
         */
        axis2_param_container_t *params;
    };

    /**
     * Creates module description struct instance.
     * @param env pointer to environment struct
     * @return pointer to newly created module description
     */
    AXIS2_EXTERN axis2_module_desc_t *AXIS2_CALL
    axis2_module_desc_create (
        const axis2_env_t *env);

    /**
     * Creates module description struct instance with given QName.
     * @param env pointer to environment struct
     * @param qname pointer to QName
     * @return pointer to newly created module description
     */
    AXIS2_EXTERN axis2_module_desc_t *AXIS2_CALL
    axis2_module_desc_create_with_qname(
        const axis2_env_t *env,
        const axis2_qname_t *qname);

    /**
     * Frees module description passed as void pointer. This method will cast 
     * the void pointer parameter into appropriate type and then call module 
     * description free method on top of that pointer.
     * @param module_desc pointer to module description as a void pointer
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_module_desc_free_void_arg(
        void *module_desc,
        const axis2_env_t *env);


/** Frees module description.
    @sa axis2_module_desc_ops#free */
#define AXIS2_MODULE_DESC_FREE(module_desc, env) \
      ((module_desc)->ops->free(module_desc, env))

/** Gets in flow.
    @sa axis2_module_desc_ops#get_in_flow */
#define AXIS2_MODULE_DESC_GET_IN_FLOW(module_desc, env) \
        ((module_desc)->ops->get_in_flow (module_desc, env))

/** Sets in flow.
    @sa axis2_module_desc_ops#set_in_flow */
#define AXIS2_MODULE_DESC_SET_IN_FLOW(module_desc, env, in_flow) \
        ((module_desc)->ops->set_in_flow (module_desc, env, in_flow))

/** Gets out flow.
    @sa axis2_module_desc_ops#get_out_flow */
#define AXIS2_MODULE_DESC_GET_OUT_FLOW(module_desc, env) \
      ((module_desc)->ops->get_out_flow (module_desc, env))

/** Sets out flow.
    @sa axis2_module_desc_ops#set_out_flow */
#define AXIS2_MODULE_DESC_SET_OUT_FLOW(module_desc, env, out_flow) \
      ((module_desc)->ops->set_out_flow (module_desc, env, out_flow))

/** Gets fault in flow.
    @sa axis2_module_desc_ops#get_fault_in_flow */
#define AXIS2_MODULE_DESC_GET_FAULT_IN_FLOW(module_desc, env) \
      ((module_desc)->ops->get_fault_in_flow (module_desc, env))

/** Sets fault in flow.
    @sa axis2_module_desc_ops#set_fault_in_flow */
#define AXIS2_MODULE_DESC_SET_FAULT_IN_FLOW(module_desc, env, falut_in_flow) \
        ((module_desc)->ops->set_fault_in_flow(module_desc , env, falut_in_flow))

/** Gets fault out flow.
    @sa axis2_module_desc_ops#get_fault_out_flow */
#define AXIS2_MODULE_DESC_GET_FAULT_OUT_FLOW(module_desc, env) \
        ((module_desc)->ops->get_fault_out_flow(module_desc , env))

/** Sets fault out flow.
    @sa axis2_module_desc_ops#set_fault_out_flow */
#define AXIS2_MODULE_DESC_SET_FAULT_OUT_FLOW(module_desc, env, falut_out_flow) \
        ((module_desc)->ops->set_fault_out_flow(module_desc , env, falut_out_flow))

/** Gets QName.
    @sa axis2_module_desc_ops#get_qname */
#define AXIS2_MODULE_DESC_GET_QNAME(module_desc, env) \
        ((module_desc)->ops->get_qname(module_desc, env))

/** Sets QName.
    @sa axis2_module_desc_ops#set_qname */
#define AXIS2_MODULE_DESC_SET_QNAME(module_desc, env, qname) \
        ((module_desc)->ops->set_qname(module_desc, env, qname))

/** Adds given operation to list of operations.
    @sa axis2_module_desc_ops#add_op */
#define AXIS2_MODULE_DESC_ADD_OP(module_desc, env, op) \
        ((module_desc)->ops->add_op(module_desc, env, op))

/** Gets list of all operations.
    @sa axis2_module_desc_ops#get_all_ops */
#define AXIS2_MODULE_DESC_GET_ALL_OPS(module_desc, env) \
        ((module_desc)->ops->get_all_ops(module_desc, env))

/** Gets parent.
    @sa axis2_module_desc_ops#get_parent */
#define AXIS2_MODULE_DESC_GET_PARENT(module_desc, env) \
        ((module_desc)->ops->get_parent(module_desc, env))

/** Sets parent.
    @sa axis2_module_desc_ops#set_parent */
#define AXIS2_MODULE_DESC_SET_PARENT(module_desc, env, parent) \
        ((module_desc)->ops->set_parent(module_desc, env, parent))

/** Adds parameter.
    @sa axis2_module_desc_ops#add_param */
#define AXIS2_MODULE_DESC_ADD_PARAM(module_desc, env, param) \
        ((module_desc)->ops->add_param(module_desc, env, param))

/** Gets named parameter.
    @sa axis2_module_desc_ops#get_param */
#define AXIS2_MODULE_DESC_GET_PARAM(module_desc, env, name) \
        ((module_desc)->ops->get_param(module_desc, env, name))

/** Get all parameters.
    @sa axis2_module_desc_ops#get_all_params */
#define AXIS2_MODULE_DESC_GET_ALL_PARAMS(module_desc, env) \
        ((module_desc)->ops->get_all_params(module_desc, env))

/** Checks if named parameter is locked.
    @sa axis2_module_desc_ops#is_param_locked */
#define AXIS2_MODULE_DESC_IS_PARAM_LOCKED(module_desc, env, param_name) \
        ((module_desc)->ops->is_param_locked(module_desc, env, param_name))

/** Gets module associated with module description.
    @sa axis2_module_desc_ops#get_module */
#define AXIS2_MODULE_DESC_GET_MODULE(module_desc, env) \
        ((module_desc)->ops->get_module(module_desc, env))

/** Sets module associated with module description.
    @sa axis2_module_desc_ops#set_module */
#define AXIS2_MODULE_DESC_SET_MODULE(module_desc, env, module) \
        ((module_desc)->ops->set_module(module_desc, env, module))

/** @} */

#ifdef __cplusplus
}
#endif
#endif /* AXIS2_MODULE_DESC_H */
