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

#ifndef AXIS2_PARAM_H
#define AXIS2_PARAM_H

/**
 * @file axis2_param.h
 * @brief Axis2 param interface
 */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_om_node.h>
#include <axis2_core.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_param Handlers
 * @ingroup axis2_core_description
 * @{
 */
    
/**
  * TEXT PARAM
  */
  static const int AXIS2_TEXT_PARAM = 0;

/**
 * Field DOM_PARAM
 */
  static const int AXIS2_DOM_PARAM = 1;
    

/**
 *   \brief param operations struct
 */
AXIS2_DECLARE_DATA   struct axis2_param_ops_s 
    {
    
    /**
     * Parameter name accessor
     * @return name of the param
     */
    axis2_char_t* (AXIS2_CALL *get_name)(axis2_param_t *param, 
											axis2_env_t **env);

    /**
     * Parameter value accessor
     * @return Object
     */
    void* (AXIS2_CALL *get_value)(axis2_param_t *param, 
									axis2_env_t **env);

    /**
     * param name mutator
     * @param name
     */
     axis2_status_t (AXIS2_CALL *set_name)(axis2_param_t *param, 
		 									axis2_env_t **env, 
	 										axis2_char_t *name);

    /**
     * Method setValue
     *
     * @param value
     */
     axis2_status_t (AXIS2_CALL *set_value)(axis2_param_t *param, 
		 										axis2_env_t **env, 
	 											void *value);

    /**
     * Method isLocked
     *
     * @return boolean
     */
     axis2_bool_t (AXIS2_CALL *is_locked)(axis2_param_t *param,
											 axis2_env_t **env);

    /**
     * Method setLocked
     *
     * @param value
     */
     axis2_status_t (AXIS2_CALL *set_locked)(axis2_param_t *param,
											 	axis2_env_t **env,
											 	axis2_bool_t value);

    /**
     * Method getParameterType
     *
     * @return int
     */
     int (AXIS2_CALL *get_param_type)(axis2_param_t *param, 
		 									axis2_env_t **env);

     axis2_status_t (AXIS2_CALL *set_param_type)(axis2_param_t *param, 
														axis2_env_t **env,
														int type);

    /**
     *  Paramter can be any thing it can be XML element with number of child 
	 *  elements , so if someone wants to access the XML elemet we need to store 
	 *  that , at the deployment time , to store the XMLelment of the param 
	 *  can use this method it will store whole
     * <parameter name="ServiceClass1" locked="false">org.apache.axis2.sample.
	 * echo.EchoImpl</parameter> element
     * @param element  <code>OMElement<code>
     */

     axis2_status_t (AXIS2_CALL *set_param_element)(axis2_param_t *param, 
														axis2_env_t **env, 
														axis2_om_node_t *element);

    /**
     *  To get the whole paramter element
     * @return <code>OMElement<code>
     */
     axis2_om_node_t* (AXIS2_CALL *get_param_element)
													(axis2_param_t *param, 
														axis2_env_t **env);
         
     axis2_status_t (AXIS2_CALL *free)(axis2_param_t *param, 
	 									axis2_env_t **env);
};
 
/** 
 * \brief Parameter struct
 */
struct axis2_param_s
{
	/** Parameter related operations */
	axis2_param_ops_t *ops;
};

/**
 * creates param struct
 */
AXIS2_DECLARE(axis2_param_t*) 
axis2_param_create(axis2_env_t **env, 
						axis2_char_t *name, 
						void *value);

#define AXIS2_PARAM_FREE(param, env) \
    ((param->ops)->free (param, env))

#define AXIS2_PARAM_SET_NAME(param, env, name) \
    ((param->ops)->set_name (param, env, name))

#define AXIS2_PARAM_GET_NAME(param, env) \
    ((param->ops)->get_name (param, env))

#define AXIS2_PARAM_SET_VALUE(param, env, value) \
    (((param->ops)->set_value (param , env, value))

#define AXIS2_PARAM_GET_VALUE(param, env) \
    ((param->ops)->get_value (param, env))

#define AXIS2_PARAM_IS_LOCKED(param, env) \
    ((param->ops)->is_locked (param, env))

#define AXIS2_PARAM_SET_LOCKED(param, env, locked) \
    ((param->ops)->set_locked (param, env, locked))

#define AXIS2_PARAM_SET_ELEMENT(param, env, element) \
    (((param->ops)->set_param_element (param , env, element))

#define AXIS2_PARAM_GET_ELEMENT(param, env) \
    ((param->ops)->get_param_element (param, env))
	
/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_PARAM_H */
