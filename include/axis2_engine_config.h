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
 
#ifndef AXIS2_CONFIG_H
#define AXIS2_CONFIG_H

/**
 * @file axis2_config.h
 * @brief axis2 engine configuration interface. Global states of all the Axis2
 *        can be accessed here
 */

#include <axis2_core.h>
#include <axis2_param_container.h>
#include <axis2_svc_grp.h>

#ifdef __cplusplus
extern "C" 
{
#endif
/** @defgroup axis2_config Axis Configuration
  * @ingroup axis2_core_engine
  * @{
  */

typedef struct axis2_engine_config_ops_s axis2_engine_config_ops_t;
typedef struct axis2_engine_config_s axis2_engine_config_t;
    
struct axis2_svc_grp_s;
struct axis2_svc_s;
struct axis2_operation_s;    
    
/** 
 * @brief Engine Configuration operations struct
 * Encapsulator struct for operations of axis2_config
 */
struct axis2_engine_config_ops_s
{
	axis2_status_t (AXIS2_CALL *free) (axis2_engine_config_t *engine_config, 
                                        axis2_env_t **env);
    
	axis2_status_t (AXIS2_CALL *add_svc_grp) (axis2_engine_config_t *engine_config, 
                                                axis2_env_t **env, 
                                                struct axis2_svc_grp_s *svc_grp);
    
	struct axis2_svc_grp_s *(AXIS2_CALL *get_svc_grp) (
                                            axis2_engine_config_t *engine_config, 
                                            axis2_env_t **env, 
                                            const axis2_char_t *svc_grp_name);
    
	axis2_status_t (AXIS2_CALL *add_svc) (axis2_engine_config_t *engine_config, 
                                            axis2_env_t **env, 
                                            struct axis2_svc_s* svc);
    
	struct axis2_svc_s *(AXIS2_CALL *get_svc) (axis2_engine_config_t *engine_config, 
                                        axis2_env_t **env, 
                                        const axis2_char_t* svc_name);
    
	axis2_status_t (AXIS2_CALL *remove_svc) (axis2_engine_config_t *engine_config, 
                                                axis2_env_t **env, 
                                                const axis2_char_t *name);
};

/**
 * @brief Engine configuration struct
 * Axis2 Engine Configuration
 * The palce where all the global states of Axis is accessed.
 * All the global states kept in the <code>EngineRegistry</code> and all the
 * Service states kept in the <code>MessageContext</code>. Other runtime
 * artifacts does not keep states forward from the execution.
 */
struct axis2_engine_config_s
{
	axis2_engine_config_ops_t *ops;
};

/** create Engine configuration struct
 * @return pointer to newly created engine configuration
 */
axis2_engine_config_t * AXIS2_CALL 
axis2_engine_config_create(axis2_env_t **env);

/************************* Start of function macros	***************************/
	
#define AXIS2_ENGINE_CONFIG_free(engine_config, env) \
		(engine_config->free(engine_config, env);
	
#define AXIS2_ENGINE_CONFIG_add_svc_grp(engine_config, env, svc_grp) \
        (engine_config->ops->add_svc_grp (engine_config, env, svc_grp));

#define AXIS2_ENGINE_CONFIG_GET_SVC_GRP(engine_config, env, svc_grp_name) \
        (engine_config->ops->get_svc_grp (engine_config, env, svc_grp_name));

#define AXIS2_ENGINE_CONFIG_ADD_SVC(engine_config, env, svc) \
		(engine_config->ops->get_svc (engine_config, env, svc));
		
#define AXIS2_ENGINE_CONFIG_GET_SVC(engine_config, env, svc_name) \
		(engine_config->ops->get_svc (engine_config, env, svc_name));

#define AXIS2_ENGINE_CONFIG_REMOVE_SVC(engine_config, env, svc_name) \
		(engine_config->ops->remove_svc (engine_config, env, svc_name));

/************************* End of function macros *****************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_CONFIG_H */
