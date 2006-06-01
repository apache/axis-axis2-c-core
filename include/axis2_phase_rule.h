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

#ifndef AXIS2_PHASE_RULE_H
#define AXIS2_PHASE_RULE_H

/**
 * @file axis2_phase_rule.h
 * @brief Axis2 phase_rule interface
 */

#include <axis2_defines.h>
#include <axis2_qname.h>
#include <axis2_param.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_phase_rule;
    struct axis2_phase_rule_ops;
    
/**
 * @defgroup axis2_phase_rule Phase Rules
 * @ingroup axis2_description
 * @{
 */

/**
 *   \brief Phase Rule ops struct
 */
 AXIS2_DECLARE_DATA   typedef struct axis2_phase_rule_ops
    {        
        axis2_char_t* (AXIS2_CALL *
		get_before) (struct axis2_phase_rule *phase_rule, 
                     const axis2_env_t *env);
		
        axis2_status_t (AXIS2_CALL *
		set_before) (struct axis2_phase_rule *phase_rule, 
                     const axis2_env_t *env, 
					 axis2_char_t *before);
		
        axis2_char_t* (AXIS2_CALL *
		get_after) (struct axis2_phase_rule * phase_rule, 
                    const axis2_env_t *env);
		
        axis2_status_t (AXIS2_CALL *
		set_after) (struct axis2_phase_rule * phase_rule, 
                    const axis2_env_t *env, 
					axis2_char_t *after);
		
        axis2_char_t* (AXIS2_CALL *
		get_name) (struct axis2_phase_rule * phase_rule, 
                   const axis2_env_t *env);
		
        axis2_status_t (AXIS2_CALL *
		set_name) (struct axis2_phase_rule * phase_rule, 
                   const axis2_env_t *env, 
				   const axis2_char_t *name);
		
        axis2_bool_t (AXIS2_CALL *
		is_phase_first) (struct axis2_phase_rule * phase_rule, 
                         const axis2_env_t *env);
		
        axis2_status_t (AXIS2_CALL *
		set_phase_first) (struct axis2_phase_rule * phase_rule, 
                          const axis2_env_t *env, 
						  axis2_bool_t phae_first);
		
        axis2_bool_t (AXIS2_CALL *
		is_phase_last) (struct axis2_phase_rule * phase_rule, 
                        const axis2_env_t *env);
		
        axis2_status_t (AXIS2_CALL *
		set_phase_last) (struct axis2_phase_rule * phase_rule, 
                         const axis2_env_t *env, 
						 axis2_bool_t phae_last);

        axis2_status_t (AXIS2_CALL *
		free) (struct axis2_phase_rule *phase_rule, 
               const axis2_env_t *env);
		
        struct axis2_phase_rule* (AXIS2_CALL *
		clone) (struct axis2_phase_rule *phase_rule, 
                const axis2_env_t *env);
		
    } axis2_phase_rule_ops_t;
	
   /** 
    * \brief Phase Rule struct
    */
    typedef struct axis2_phase_rule
    {
        /** Phase Rule related ops */
        axis2_phase_rule_ops_t *ops;
    } axis2_phase_rule_t;


/**
 * creates phase_rule struct
 */
AXIS2_EXTERN axis2_phase_rule_t* AXIS2_CALL 
axis2_phase_rule_create(const axis2_env_t *env, 
						axis2_char_t *phase_name);
    
#define AXIS2_PHASE_RULE_GET_BEFORE(phase_rule, env) \
		((phase_rule)->ops->get_before(phase_rule, env))
	
#define AXIS2_PHASE_RULE_SET_BEFORE(phase_rule, env, before) \
		((phase_rule)->ops->set_before(phase_rule, env, before))
	
#define AXIS2_PHASE_RULE_GET_AFTER(phase_rule, env) \
		((phase_rule)->ops->get_after(phase_rule, env))
	
#define AXIS2_PHASE_RULE_SET_AFTER(phase_rule, env, after) \
		((phase_rule)->ops->set_after(phase_rule, env, after))
	
#define AXIS2_PHASE_RULE_GET_NAME(phase_rule, env) \
		((phase_rule)->ops->get_name(phase_rule, env))
		
#define AXIS2_PHASE_RULE_SET_NAME(phase_rule, env, name)\
		((phase_rule)->ops->set_name(phase_rule, env, name))
		
#define AXIS2_PHASE_RULE_IS_PHASE_FIRST(phase_rule, env) \
		((phase_rule)->ops->is_phase_first(phase_rule, env))
		
#define AXIS2_PHASE_RULE_SET_PHASE_FIRST(phase_rule, env, phase_first) \
		((phase_rule)->ops->set_phase_first(phase_rule, env, phase_first))
		
#define AXIS2_PHASE_RULE_IS_PHASE_LAST(phase_rule, env) \
		((phase_rule)->ops->is_phase_last(phase_rule, env))
		
#define AXIS2_PHASE_RULE_SET_PHASE_LAST(phase_rule, env, phase_last) \
		((phase_rule)->ops->set_phase_last(phase_rule, env, phase_last))
		
#define AXIS2_PHASE_RULE_FREE(phase_rule, env) \
		((phase_rule)->ops->free(phase_rule, env))
		
#define AXIS2_PHASE_RULE_CLONE(phase_rule, env) \
		((phase_rule)->ops->clone(phase_rule, env))
    
/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_PHASE_RULE_H */
