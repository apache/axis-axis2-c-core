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

#ifndef AXIS2_PARAMETER_H
#define AXIS2_PARAMETER_H

/**
 * @file axis2_parameter.h
 * @brief Axis2 parameter interface
 */

#include <axis2_defines.h>
#include <axis2_environment.h>
#include <axis2_om_node.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_parameter;
    struct axis2_parameter_ops;

/**
 * @defgroup axis2_parameter Handlers
 * @ingroup axis2_core_description
 * @{
 */
    
/**
  * TEXT PARAMETER
  */
  static const int AXIS2_TEXT_PARAMETER = 0;

/**
 * Field DOM_PARAMETER
 */
  static const int AXIS2_DOM_PARAMETER = 1;
    

/**
 *   \brief parameter operations struct
 */
AXIS2_DECLARE_DATA   typedef struct axis2_parameter_ops 
    {
    
    /**
     * Parameter name accessor
     * @return name of the parameter
     */
    axis2_char_t* (AXIS2_CALL *get_name)(struct axis2_parameter *parameter, axis2_env_t **env);

    /**
     * Parameter value accessor
     * @return Object
     */
    void* (AXIS2_CALL *get_value)(struct axis2_parameter *parameter, axis2_env_t **env);

    /**
     * parameter name mutator
     * @param name
     */
     axis2_status_t (AXIS2_CALL *set_name)(struct axis2_parameter *parameter, axis2_env_t **env, axis2_char_t *name);

    /**
     * Method setValue
     *
     * @param value
     */
     axis2_status_t (AXIS2_CALL *set_value)(struct axis2_parameter *parameter, axis2_env_t **env, void *value);

    /**
     * Method isLocked
     *
     * @return boolean
     */
     axis2_bool_t (AXIS2_CALL *is_locked)(struct axis2_parameter *parameter, axis2_env_t **env);

    /**
     * Method setLocked
     *
     * @param value
     */
     axis2_status_t (AXIS2_CALL *set_locked)(struct axis2_parameter *parameter, axis2_env_t **env, axis2_bool_t value);

    /**
     * Method getParameterType
     *
     * @return int
     */
     int (AXIS2_CALL *get_parameter_type)(struct axis2_parameter *parameter, axis2_env_t **env);

     axis2_status_t (AXIS2_CALL *set_parameter_type)(struct axis2_parameter *parameter, axis2_env_t **env, int type);

    /**
     *  Paramter can be any thing it can be XML element with number of child elements , so if some
     * one wants to access the XML elemet we need to store that , at the deployment time , to store
     * the XMLelment of the parameter can use this method it will store whole
     * <parameter name="ServiceClass1" locked="false">org.apache.axis2.sample.echo.EchoImpl</parameter>
     * element
     * @param element  <code>OMElement<code>
     */

     axis2_status_t (AXIS2_CALL *set_parameter_element)(struct axis2_parameter *parameter, axis2_env_t **env, axis2_om_node_t *element);

    /**
     *  To get the whole paramter element
     * @return <code>OMElement<code>
     */
     axis2_om_node_t* (AXIS2_CALL *get_parameter_element)(struct axis2_parameter *parameter, axis2_env_t **env);
         
     axis2_status_t (AXIS2_CALL *free)(struct axis2_parameter *parameter, axis2_env_t **env);
} axis2_parameter_ops_t;
 
/** 
 * \brief Parameter struct
 */
    typedef struct axis2_parameter
    {
        /** Parameter related operations */
        axis2_parameter_ops_t *ops;
    } axis2_parameter_t;

/**
 * creates parameter struct
 */
AXIS2_DECLARE(axis2_parameter_t*) axis2_parameter_create(axis2_env_t **env, axis2_char_t *name, void *value);
/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_PARAMETER_H */
