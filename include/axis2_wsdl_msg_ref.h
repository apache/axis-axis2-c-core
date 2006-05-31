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

#ifndef AXIS2_WSDL_MSG_REF_H
#define AXIS2_WSDL_MSG_REF_H

/**
 * @file axis2_wsdl_msg_ref.h
 * @brief Axis2 Wsdl bindig message reference interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_wsdl_extensible_component.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_wsdl_extensible_component;
typedef struct axis2_wsdl_msg_ref axis2_wsdl_msg_ref_t;
typedef struct axis2_wsdl_msg_ref_ops axis2_wsdl_msg_ref_ops_t;

/** @defgroup axis2_wsdl_msg_ref Wsdl Binding Message Reference
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl Message Reference ops struct
 * Encapsulator struct for ops of axis2_wsdl_msg_ref
 */
AXIS2_DECLARE_DATA struct axis2_wsdl_msg_ref_ops
{
    /** De-allocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free)(axis2_wsdl_msg_ref_t *wsdl_msg_ref,
		    const axis2_env_t *env);

    /**
     * Method getDirection
     *
     * @return
     */
    axis2_char_t * (AXIS2_CALL *
    get_direction) (axis2_wsdl_msg_ref_t *msg_ref,
                                                const axis2_env_t *env);
    
    /**
     * Method setDirection
     *
     * @param direction
     */
    axis2_status_t (AXIS2_CALL *
    set_direction) (axis2_wsdl_msg_ref_t *msg_ref,
                                                const axis2_env_t *env,
                                                axis2_char_t *direction);
    
    /**
     * Method getMessageLabel
     *
     * @return
     */
    axis2_char_t * (AXIS2_CALL *
    get_msg_label) (axis2_wsdl_msg_ref_t *msg_ref,
                                                const axis2_env_t *env);
    
    /**
     * Method setMessageLabel
     *
     * @param messageLabel
     */
    axis2_status_t (AXIS2_CALL *
    set_msg_label) (axis2_wsdl_msg_ref_t *msg_ref,
                                                const axis2_env_t *env,
                                                axis2_char_t *msg_label);
                                                
    /**
     * Returns an Element which refers to the actual message that will get 
     *transported. This Element Abstracts all the Message Parts that was defined in 
     * the WSDL 1.1.
     * @return
     */
    axis2_qname_t * (AXIS2_CALL *
    get_element) (axis2_wsdl_msg_ref_t *msg_ref,
                                    const axis2_env_t *env);
    
    /**
     * Sets the Element that will Abstract the actual message. All the parts defined 
     * in WSDL 1.1 per message should be Encapsulated in this Element.
     * @param element
     */
    axis2_status_t (AXIS2_CALL *
    set_element) (axis2_wsdl_msg_ref_t *msg_ref,
                                    const axis2_env_t *env,
                                    axis2_qname_t *element);                                              
};

/** 
 * @brief Wsdl  message reference struct  
 */  
AXIS2_DECLARE_DATA struct axis2_wsdl_msg_ref
{
	axis2_wsdl_msg_ref_ops_t *ops;
    struct axis2_wsdl_extensible_component *extensible_component;
};

/**
 * Creates wsdl  message reference struct
 * @return pointer to newly created wsdl  message reference
 */
AXIS2_DECLARE(axis2_wsdl_msg_ref_t *) 
axis2_wsdl_msg_ref_create (const axis2_env_t *env);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_MSG_REF_FREE(wsdl_msg_ref, env) \
		((wsdl_msg_ref)->ops->free (wsdl_msg_ref, env))

#define AXIS2_WSDL_MSG_REF_GET_DIRECTION(wsdl_msg_ref, env) \
		((wsdl_msg_ref)->ops->get_direction (wsdl_msg_ref, env))

#define AXIS2_WSDL_MSG_REF_SET_DIRECTION(wsdl_msg_ref, env, direction) \
		((wsdl_msg_ref)->ops->set_direction (wsdl_msg_ref, env, direction))       

#define AXIS2_WSDL_MSG_REF_GET_MSG_LABEL(wsdl_msg_ref, env) \
		((wsdl_msg_ref)->ops->get_msg_label (wsdl_msg_ref, env))

#define AXIS2_WSDL_MSG_REF_SET_MSG_LABEL(wsdl_msg_ref, env, msg_label) \
		((wsdl_msg_ref)->ops->set_msg_label (wsdl_msg_ref, env, msg_label))
        
#define AXIS2_WSDL_MSG_REF_GET_ELEMENT(wsdl_msg_ref, env) \
		((wsdl_msg_ref)->ops->get_element (wsdl_msg_ref, env))

#define AXIS2_WSDL_MSG_REF_SET_ELEMENT(wsdl_msg_ref, env, element) \
		((wsdl_msg_ref)->ops->set_element (wsdl_msg_ref, env, element))        
        
/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_MSG_REF_H */
