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

#ifndef AXIS2_HANDLER_DESC_H
#define AXIS2_HANDLER_DESC_H

/**
 * @file axis2_handler_desc.h
 * @brief Axis2 handler_desc interface
 */

#include <axis2_defines.h>
#include <axis2_qname.h>
#include <axis2_param.h>
#include <axis2_param_container.h>
#include <axis2_phase_rule.h>
#include <axis2_handler.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_handler_desc;
    struct axis2_handler_desc_ops;
    
/**
 * @defgroup axis2_handler_desc Handler Description
 * @ingroup axis2_description
 * @{
 */

/**
 *   \brief Handler Description ops struct
 */
 AXIS2_DECLARE_DATA   typedef struct axis2_handler_desc_ops
    { 
        axis2_qname_t* (AXIS2_CALL *
        get_qname) (struct axis2_handler_desc *handler_desc, 
                        axis2_env_t **env);
        
        axis2_status_t (AXIS2_CALL *
        set_qname) (struct axis2_handler_desc *handler_desc, 
                        axis2_env_t **env, 
                        axis2_qname_t *qname);
        
        axis2_phase_rule_t* (AXIS2_CALL *
        get_rules) (struct axis2_handler_desc *handler_desc, 
                        axis2_env_t **env);
        
        axis2_status_t (AXIS2_CALL *
        set_rules) (struct axis2_handler_desc *handler_desc, 
                        axis2_env_t **env, 
                        axis2_phase_rule_t *phase_rule);
        
        axis2_param_t* (AXIS2_CALL *
        get_param) (struct axis2_handler_desc *handler_desc, 
                        axis2_env_t **env, 
                        axis2_char_t *name);
        
        axis2_status_t (AXIS2_CALL *
        add_param) (struct axis2_handler_desc *handler_desc, 
                        axis2_env_t **env, 
                        axis2_param_t * param);
        
        axis2_array_list_t* (AXIS2_CALL *
        get_params) (struct axis2_handler_desc *handler_desc, 
                     axis2_env_t **env);

        /**
        * checks whether the paramter is locked at any levle 
        */
        axis2_bool_t (AXIS2_CALL *
        is_param_locked)(struct axis2_handler_desc *handler_desc, 
                         axis2_env_t **env, axis2_char_t * param_name);
        
        axis2_handler_t* (AXIS2_CALL *
        get_handler) (struct axis2_handler_desc *handler_desc, 
                      axis2_env_t **env);
        
        axis2_status_t (AXIS2_CALL *
        set_handler) (struct axis2_handler_desc *handler_desc, 
                        axis2_env_t **env, 
                        axis2_handler_t * handler);
        
        axis2_char_t* (AXIS2_CALL *
        get_class_name) (struct axis2_handler_desc *handler_desc, 
                            axis2_env_t **env);
        
        axis2_status_t (AXIS2_CALL *
        set_class_name) (struct axis2_handler_desc *handler_desc, 
                            axis2_env_t **env, 
                            axis2_char_t *class_name);
        
        axis2_param_container_t* (AXIS2_CALL *
        get_parent) (struct axis2_handler_desc *handler_desc, 
                                               axis2_env_t **env);
        
        axis2_status_t (AXIS2_CALL *
        set_parent) (struct axis2_handler_desc *handler_desc, 
                        axis2_env_t **env,
                        axis2_param_container_t * parent);
        
        axis2_status_t (AXIS2_CALL *
        free) (struct axis2_handler_desc *handler_desc, 
                axis2_env_t **env);
        
    } axis2_handler_desc_ops_t;
	
   /** 
    * \brief Handler Description struct
    */
    typedef struct axis2_handler_desc
    {
        /** Handler Description related ops */
        axis2_handler_desc_ops_t *ops;
        
        /** param container */
        axis2_param_container_t *param_container;
        
    } axis2_handler_desc_t;


/**
 * creates handler_desc struct
 * @param qname qname, can be NULL
 */
AXIS2_DECLARE(axis2_handler_desc_t*) 
axis2_handler_desc_create_with_qname(axis2_env_t **env, axis2_qname_t *qname);

#define AXIS2_HANDLER_DESC_GET_HANDLER(handler_desc, env) \
        ((handler_desc)->ops->get_handler(handler_desc, env))
    
#define AXIS2_HANDLER_DESC_SET_HANDLER(handler_desc, env, handler) \
        ((handler_desc)->ops->set_handler(handler_desc, env, handler))
    
#define AXIS2_HANDLER_DESC_GET_RULES(handler_desc, env) \
        ((handler_desc)->ops->get_rules(handler_desc, env))
    
#define AXIS2_HANDLER_DESC_SET_RULES(handler_desc, env, rules) \
        ((handler_desc)->ops->set_rules(handler_desc, env, rules))
    
#define AXIS2_HANDLER_DESC_GET_QNAME(handler_desc, env) \
        ((handler_desc)->ops->get_qname(handler_desc, env))
    
#define AXIS2_HANDLER_DESC_SET_QNAME(handler_desc, env, name) \
        ((handler_desc)->ops->set_qname(handler_desc, env, name))
    
#define AXIS2_HANDLER_DESC_GET_PARAM(handler_desc, env, name) \
        ((handler_desc)->ops->get_param(handler_desc, env, name))
        
#define AXIS2_HANDLER_DESC_ADD_PARAM(handler_desc, env, param) \
        ((handler_desc)->ops->add_param(handler_desc, env, param))
        
#define AXIS2_HANDLER_DESC_IS_PHASE_FIRST(handler_desc, env) \
        ((handler_desc)->ops->is_phase_first((handler_desc, env))
        
#define AXIS2_HANDLER_DESC_GET_PHASE_FIRST(handler_desc, env, phase_first) \
        ((handler_desc)->ops->set_phase_first(handler_desc, env, phase_first))
        
#define AXIS2_HANDLER_DESC_IS_PHASE_LAST(handler_desc, env) \
        ((handler_desc)->ops->is_phase_last(handler_desc, env))
        
#define AXIS2_HANDLER_DESC_GET_PHASE_LAST(handler_desc, env, phase_last) \
        ((handler_desc)->ops->set_phase_last(handler_desc, env, phase_last))

#define AXIS2_HANDLER_DESC_GET_CLASS_NAME(handler_desc, env) \
        ((handler_desc)->ops->get_class_name(handler_desc, env))
        
#define AXIS2_HANDLER_DESC_SET_CLASS_NAME(handler_desc, env, class_name) \
        ((handler_desc)->ops->set_class_name(handler_desc, env, class_name))

#define AXIS2_HANDLER_DESC_GET_PARENT(handler_desc, env) \
        ((handler_desc)->ops->get_parent(handler_desc, env))
        
#define AXIS2_HANDLER_DESC_SET_PARENT(handler_desc, env, parent) \
        ((handler_desc)->ops->set_parent(handler_desc, env, parent))
        
#define AXIS2_HANDLER_DESC_FREE(handler_desc, env) \
        ((handler_desc)->ops->free(handler_desc, env))
    
/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_HANDLER_DESC_H */
