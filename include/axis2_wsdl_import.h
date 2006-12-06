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

#ifndef AXIS2_WSDL_IMPORT_H
#define AXIS2_WSDL_IMPORT_H

/**
 * @file axis2_wsdl_import.h
 * @brief Axis2 Wsdl import interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_wsdl_component.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_wsdl_component;    
typedef struct axis2_wsdl_import axis2_wsdl_import_t;
typedef struct axis2_wsdl_import_ops axis2_wsdl_import_ops_t;

/** @defgroup axis2_wsdl_import Wsdl Import
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl Import ops struct
 * Encapsulator struct for ops of axis2_wsdl_import
 */
 struct axis2_wsdl_import_ops
{
   /** De-allocate memory
      * @return status code
      */
   axis2_status_t (AXIS2_CALL *
    free)(axis2_wsdl_import_t *wsdl_import,
           const axis2_env_t *env);
                               
    /**
     * Method get_namespace
     *
     * @return namespace
     */
    axis2_char_t *(AXIS2_CALL *
    get_namespace) (axis2_wsdl_import_t *wsdl_import,
                                    const axis2_env_t *env);
                                    
    /**
     * Method set_namespace
     *
     * @param namespace
     */
    axis2_status_t (AXIS2_CALL *
    set_namespace) (axis2_wsdl_import_t *wsdl_import,
                                    const axis2_env_t *env,
                                    axis2_char_t *namespace);
    
    
    /**
     * Method isRequired
     *
     * @return
     */
    axis2_char_t *(AXIS2_CALL *
    get_location) (axis2_wsdl_import_t *wsdl_import,
                                    const axis2_env_t *env);
                                    
    /**
     * Method set_location
     *
     * @param location
     */
    axis2_status_t (AXIS2_CALL *
    set_location) (axis2_wsdl_import_t *wsdl_import,
                                    const axis2_env_t *env,
                                    axis2_char_t *location);  


};

/** 
 * @brief Wsdl container struct
 *   Property for params  
 */  
 struct axis2_wsdl_import
{
   axis2_wsdl_import_ops_t *ops;
    struct axis2_wsdl_component *wsdl_component;
};

/**
 * Creates wsdl import struct
 * @return pointer to newly created wsdl import
 */
AXIS2_EXTERN axis2_wsdl_import_t *AXIS2_CALL axis2_wsdl_import_create (const axis2_env_t *env);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_IMPORT_FREE(wsdl_import, env) \
      ((wsdl_import)->ops->free (wsdl_import, env))

#define AXIS2_WSDL_IMPORT_GET_NAMESPACE(wsdl_import, env) \
      ((wsdl_import)->ops->get_namespace (wsdl_import, env))

#define AXIS2_WSDL_IMPORT_SET_NAMESPACE(wsdl_import, env, namespace) \
      ((wsdl_import)->ops->set_namespace (wsdl_import, env, namespace))
        
#define AXIS2_WSDL_IMPORT_GET_LOCATION(wsdl_import, env) \
      ((wsdl_import)->ops->is_location (wsdl_import, env))

#define AXIS2_WSDL_IMPORT_SET_LOCATION(wsdl_import, env, location) \
      ((wsdl_import)->ops->set_location (wsdl_import, env, location))        

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_IMPORT_H */
