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
 * @file axis2_module_desc.h
 * @brief axis2 module_desc interface
 * <p>This holds the information about a Module. </p>
 * <ol>
 * <li>parameters<li>
 * <li>handlers<li>
 * <ol>
 * <p>Handler are registered once they are avlible but they avalibe to all 
 * services if axis2.xml has a module ref="." or avalible to a single service 
 * if services.xml have module ref=".."</p>
 */

#include <axis2.h>
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
#include <axis2_module.h>


#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_op;
struct axis2_conf;	
typedef struct axis2_module_desc_ops axis2_module_desc_ops_t;
typedef struct axis2_module_desc axis2_module_desc_t;	
	

/** @defgroup axis2_module_desc Module Description
  * @ingroup axis2_module_desc
  * @{
  */

/** 
 * @brief Module Description ops struct
 * Encapsulator struct for ops of axis2_module_desc
 */
struct axis2_module_desc_ops
{
    axis2_status_t (AXIS2_CALL *
    free) (axis2_module_desc_t *module_desc,
                                axis2_env_t **env);
        
    /**
     * Get fault out flow
     * @return in flow
     */
    axis2_flow_t *(AXIS2_CALL *
    get_inflow) (axis2_module_desc_t *module_desc,
                                        axis2_env_t **env);
    
    /**
     * Set in flow
     * @param inflow in flow
     */
    axis2_status_t (AXIS2_CALL *
    set_inflow) (axis2_module_desc_t *module_desc,
                                        axis2_env_t **env,
                                        axis2_flow_t *inflow);
    
    /**
     * Get out flow
     * @return out flow
     */
    axis2_flow_t *(AXIS2_CALL *
    get_outflow) (axis2_module_desc_t *module_desc,
                                        axis2_env_t **env);
    
    /**
     * Set out flow
     * @param outflow out flow
     */
    axis2_status_t (AXIS2_CALL *
    set_outflow) (axis2_module_desc_t *module_desc,
                                        axis2_env_t **env,
                                        axis2_flow_t *outflow);
    
    /**
     * Get fault in flow
     * @return fault in flow
     */
    axis2_flow_t * (AXIS2_CALL *
    get_fault_inflow) (axis2_module_desc_t *module_desc,
                                            axis2_env_t **env);
    
    /**
     * set fault in flow
     * @param falut_inflow falut in flow
     */
    axis2_status_t (AXIS2_CALL *
    set_fault_inflow) (axis2_module_desc_t *module_desc,
                                            axis2_env_t **env,
                                            axis2_flow_t *falut_inflow);
    
    /**
     * Get fault out flow
     * @return fault out flow
     */
    axis2_flow_t *(AXIS2_CALL *
    get_fault_outflow) (axis2_module_desc_t *module_desc,
                                                axis2_env_t **env);
    
    /**
     * Set fault out flow
     * @param fault_outflow fault out flow
     */
    axis2_status_t (AXIS2_CALL *
    set_fault_outflow) (axis2_module_desc_t *module_desc,
                                                axis2_env_t **env,
                                                axis2_flow_t *fault_outflow);
    
    /**
     * @return
     */
    axis2_qname_t * (AXIS2_CALL *
    get_name ) (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env);
    
    /**
     * @param name
     */
    axis2_status_t (AXIS2_CALL *
    set_name ) (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_qname_t *qname);
    
    axis2_status_t (AXIS2_CALL *
    add_op ) (axis2_module_desc_t *module_desc,
                                        axis2_env_t **env,
                                        struct axis2_op *op);
    
    axis2_hash_t * (AXIS2_CALL *
    get_ops ) (axis2_module_desc_t *module_desc,
                                        axis2_env_t **env);
    
    struct axis2_conf * (AXIS2_CALL *
    get_parent ) (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_parent ) (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    struct axis2_conf *parent);
    
    /**
     * Add parameter
     * @param param
     */
    axis2_status_t (AXIS2_CALL *
    add_param) (axis2_module_desc_t *module_desc,
                                        axis2_env_t **env,
                                        axis2_param_t *param);
    
    /**
     * @param name
     * @return parameter
     */
    axis2_param_t * (AXIS2_CALL *
    get_param) (axis2_module_desc_t *module_desc,
                                        axis2_env_t **env,
                                        axis2_char_t *name);
    
    axis2_array_list_t * (AXIS2_CALL *
    get_params) (axis2_module_desc_t *module_desc,
                                        axis2_env_t **env);
    
    /**
     * To check whether a given paramter is locked
     * @param param_name
     * @return whether parameter is locked
     */
    axis2_bool_t (AXIS2_CALL *
    is_param_locked ) (axis2_module_desc_t *module_desc,
                                        axis2_env_t **env,
                                        axis2_char_t *param_name);
                                        
    /**
     * @return
     */
    struct axis2_module *(AXIS2_CALL *
    get_module) (axis2_module_desc_t *module_desc,
                                        axis2_env_t **env);
    
    /**
     * @param module
     */
    axis2_status_t (AXIS2_CALL *
    set_module) (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    struct axis2_module *module);                                        

};

/**
 * @brief Module Description struct
 * Axis2 Module Description
 * <p>This holds the information about a Module. </p>
 * <ol>
 * <li>parameters<li>
 * <li>handlers<li>
 * <ol>
 * <p>Handler are registered once they are avlible but they avalibe to all 
 * services if axis2.xml has a module ref="." or avalible to a single service if
 *     services.xml have module ref=".."</p>
 */
struct axis2_module_desc
{
	axis2_module_desc_ops_t *ops;
    
    axis2_flow_container_t *flow_container;
    
    axis2_param_container_t *params;
    
};

/** create Module Description struct
 * @return pointer to newly created module description
 */
AXIS2_DECLARE(axis2_module_desc_t *) 
axis2_module_desc_create (axis2_env_t **env);

/** create Module Description struct
 * @return pointer to newly created module description
 */
AXIS2_DECLARE(axis2_module_desc_t *) 
axis2_module_desc_create_with_qname (axis2_env_t **env, 
                                        axis2_qname_t *qname);

/**
 * Free module_desc passed as void pointer. This will be
 * cast into appropriate type and then pass the cast object
 * into the module_desc structure's free method
 */
AXIS2_DECLARE(axis2_status_t) 
axis2_module_desc_free_void_arg (void *module_desc,
                                        axis2_env_t **env);

/**
 * Free module_desc array passed as void pointer.
 */
AXIS2_DECLARE(axis2_status_t) 
axis2_module_desc_array_list_free (void *module_desc_list,
                                axis2_env_t **env);
                                
/************************* Start of function macros	***************************/
	
#define AXIS2_MODULE_DESC_FREE(module_desc, env) \
		((module_desc)->ops->free(module_desc, env))
	
#define AXIS2_MODULE_DESC_GET_INFLOW(module_desc, env) \
        ((module_desc)->ops->get_inflow (module_desc, env))

#define AXIS2_MODULE_DESC_SET_INFLOW(module_desc, env, inflow) \
        ((module_desc)->ops->set_inflow (module_desc, env, inflow))

#define AXIS2_MODULE_DESC_GET_OUTFLOW(module_desc, env) \
		((module_desc)->ops->get_outflow (module_desc, env))
		
#define AXIS2_MODULE_DESC_SET_OUTFLOW(module_desc, env, outflow) \
		((module_desc)->ops->set_outflow (module_desc, env, outflow))

#define AXIS2_MODULE_DESC_GET_FAULT_INFLOW(module_desc, env) \
		((module_desc)->ops->get_fault_inflow (module_desc, env))
        
#define AXIS2_MODULE_DESC_SET_FAULT_INFLOW(module_desc, env, falut_inflow) \
        ((module_desc)->ops->set_fault_inflow(module_desc , env, falut_inflow))
		
#define AXIS2_MODULE_DESC_GET_FAULT_OUTFLOW(module_desc, env) \
        ((module_desc)->ops->get_fault_outflow(module_desc , env))
		
#define AXIS2_MODULE_DESC_SET_FAULT_OUTFLOW(module_desc, env, falut_outflow) \
        ((module_desc)->ops->set_fault_outflow(module_desc , env, falut_outflow))
		
#define AXIS2_MODULE_DESC_GET_NAME(module_desc, env) \
        ((module_desc)->ops->get_name(module_desc, env))

#define AXIS2_MODULE_DESC_SET_NAME(module_desc, env, qname) \
        ((module_desc)->ops->set_name(module_desc, env, qname))

#define AXIS2_MODULE_DESC_ADD_OP(module_desc, env, op) \
        ((module_desc)->ops->add_op(module_desc, env, op))
        
#define AXIS2_MODULE_DESC_GET_OPS(module_desc, env) \
        ((module_desc)->ops->get_ops(module_desc, env))

#define AXIS2_MODULE_DESC_GET_PARENT(module_desc, env) \
        ((module_desc)->ops->get_parent(module_desc, env))
        
#define AXIS2_MODULE_DESC_SET_PARENT(module_desc, env, parent) \
        ((module_desc)->ops->set_parent(module_desc, env, parent))

#define AXIS2_MODULE_DESC_ADD_PARAM(module_desc, env, param) \
        ((module_desc)->ops->add_param(module_desc, env, param))
        
#define AXIS2_MODULE_DESC_GET_PARAM(module_desc, env, name) \
        ((module_desc)->ops->get_param(module_desc, env, name))

#define AXIS2_MODULE_DESC_GET_PARAMS(module_desc, env) \
        ((module_desc)->ops->get_params(module_desc, env))
        
#define AXIS2_MODULE_DESC_IS_PARAM_LOCKED(module_desc, env, param_name) \
        ((module_desc)->ops->is_param_locked(module_desc, env, param_name))

#define AXIS2_MODULE_DESC_GET_MODULE(module_desc, env) \
        ((module_desc)->ops->get_module(module_desc, env))
        
#define AXIS2_MODULE_DESC_SET_MODULE(module_desc, env, module) \
        ((module_desc)->ops->set_module(module_desc, env, module))        
        
/************************* End of function macros *****************************/
/** @} */

#ifdef __cplusplus
}
#endif
#endif /* AXIS2_MODULE_DESC_H */
