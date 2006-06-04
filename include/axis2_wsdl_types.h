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

#ifndef AXIS2_WSDL_TYPES_H
#define AXIS2_WSDL_TYPES_H

/**
 * @file axis2_wsdl_types.h
 * @brief Axis2 Wsdl interface interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_hash.h>
#include <axis2_linked_list.h>
#include <axis2_qname.h>

#ifdef __cplusplus
extern "C"
{
#endif
    
struct axis2_wsdl_extensible_element;   
struct axis2_wsdl_extensible_component;    
typedef struct axis2_wsdl_types axis2_wsdl_types_t;
typedef struct axis2_wsdl_types_ops axis2_wsdl_types_ops_t;

/** @defgroup axis2_wsdl_types Wsdl Types
 * @ingroup axis2_wsdl
 * @{
 */


/** 
 * @brief Wsdl Types ops struct
 * Encapsulator struct for ops of axis2_wsdl_types
 */
AXIS2_DECLARE_DATA struct axis2_wsdl_types_ops
{
   /** De-allocate memory
      * @return status code
      */
   axis2_status_t (AXIS2_CALL *
    free)(axis2_wsdl_types_t *wsdl_types,
          const axis2_env_t *env);
    /**
     * Adds the <code>ExtensionElement</code> to the map keyed with the 
     * <code>axis2_qname_t</code>
     *
     * @param element
     */
    axis2_status_t (AXIS2_CALL *
    add_element) (axis2_wsdl_types_t *wsdl_types,
                    const axis2_env_t *env,
                    struct axis2_wsdl_extensible_element *element);


    /**
     * Will return the first Element with the given <code>QName</code>
     * Returns null if not found.
     *
     * @param qname
     * @return
     */
    struct axis2_wsdl_extensible_element *(AXIS2_CALL *
    get_first_element) (axis2_wsdl_types_t *wsdl_types,
                        const axis2_env_t *env,
                        axis2_qname_t *qname);


};

/** 
 * @brief Wsdl container struct
 *   Property for params  
 */  
AXIS2_DECLARE_DATA struct axis2_wsdl_types
{
   axis2_wsdl_types_ops_t *ops;
    struct axis2_wsdl_extensible_component *ext_component;
};

/**
 * Creates wsdl property struct
 * @return pointer to newly created wsdl property
 */
AXIS2_EXTERN axis2_wsdl_types_t * AXIS2_CALL 
axis2_wsdl_types_create (const axis2_env_t *env);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_TYPES_FREE(wsdl_types, env) \
      ((wsdl_types)->ops->free (wsdl_types, env))

#define AXIS2_WSDL_TYPES_ADD_ELEMENT(wsdl_types, env, element) \
      ((wsdl_types)->ops->add_element (wsdl_types, env, element))

#define AXIS2_WSDL_TYPES_GET_FIRST_ELEMENT(wsdl_types, env, qname) \
      ((wsdl_types)->ops->get_first_element (wsdl_types, env, qname))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_TYPES_H */
