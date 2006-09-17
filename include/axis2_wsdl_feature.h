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

#ifndef AXIS2_WSDL_FEATURE_H
#define AXIS2_WSDL_FEATURE_H

/**
 * @file axis2_wsdl_feature.h
 * @brief Axis2 Wsdl feature interface
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
typedef struct axis2_wsdl_feature axis2_wsdl_feature_t;
typedef struct axis2_wsdl_feature_ops axis2_wsdl_feature_ops_t;

/** @defgroup axis2_wsdl_feature Wsdl Feature
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl Feature ops struct
 * Encapsulator struct for ops of axis2_wsdl_feature
 */
 struct axis2_wsdl_feature_ops
{
   /** De-allocate memory
      * @return status code
      */
   axis2_status_t (AXIS2_CALL *
    free)(axis2_wsdl_feature_t *wsdl_feature,
           const axis2_env_t *env);
                               
    /**
     * Method getName
     *
     * @return
     */
    axis2_char_t *(AXIS2_CALL *
    get_name) (axis2_wsdl_feature_t *wsdl_feature,
                                    const axis2_env_t *env);
                                    
    /**
     * Method setName
     *
     * @param name
     */
    axis2_status_t (AXIS2_CALL *
    set_name) (axis2_wsdl_feature_t *wsdl_feature,
                                    const axis2_env_t *env,
                                    axis2_char_t *name);
    
    
    /**
     * Method isRequired
     *
     * @return
     */
    axis2_bool_t (AXIS2_CALL *
    is_required) (axis2_wsdl_feature_t *wsdl_feature,
                                    const axis2_env_t *env);
                                    
    /**
     * Method setRequired
     *
     * @param required
     */
    axis2_status_t (AXIS2_CALL *
    set_required) (axis2_wsdl_feature_t *wsdl_feature,
                                    const axis2_env_t *env,
                                    axis2_bool_t required);  


};

/** 
 * @brief Wsdl container struct
 *   Property for params  
 */  
 struct axis2_wsdl_feature
{
   axis2_wsdl_feature_ops_t *ops;
    struct axis2_wsdl_component *wsdl_component;
};

/**
 * Creates wsdl feature struct
 * @return pointer to newly created wsdl feature
 */
AXIS2_EXTERN axis2_wsdl_feature_t *AXIS2_CALL axis2_wsdl_feature_create (const axis2_env_t *env);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_FEATURE_FREE(wsdl_feature, env) \
      ((wsdl_feature)->ops->free (wsdl_feature, env))

#define AXIS2_WSDL_FEATURE_GET_NAME(wsdl_feature, env) \
      ((wsdl_feature)->ops->get_name (wsdl_feature, env))

#define AXIS2_WSDL_FEATURE_SET_NAME(wsdl_feature, env, name) \
      ((wsdl_feature)->ops->set_name (wsdl_feature, env, name))
        
#define AXIS2_WSDL_FEATURE_IS_REQUIRED(wsdl_feature, env) \
      ((wsdl_feature)->ops->is_required (wsdl_feature, env))

#define AXIS2_WSDL_FEATURE_SET_REQUIRED(wsdl_feature, env, required) \
      ((wsdl_feature)->ops->set_required (wsdl_feature, env, required))        

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_FEATURE_H */
