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

#ifndef AXIS2_WSDL_EXTENSIBLE_COMPONENT_H
#define AXIS2_WSDL_EXTENSIBLE_COMPONENT_H

/**
 * @file axis2_wsdl_extensible_component.h
 * @brief Axis2 Wsdl extensible component interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_linked_list.h>
#include <axis2_wsdl_feature.h>
#include <axis2_wsdl_property.h>
#include <axis2_wsdl_component.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_wsdl_component;
struct axis2_wsdl_feature; 
struct axis2_wsdl_property;    
typedef struct axis2_wsdl_extensible_component axis2_wsdl_extensible_component_t;
typedef struct axis2_wsdl_extensible_component_ops axis2_wsdl_extensible_component_ops_t;

/** @defgroup axis2_wsdl_extensible_component Wsdl Extensible Component
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl Extensible Component ops struct
 * Encapsulator struct for ops of axis2_wsdl_extensible_component
 */
AXIS2_DECLARE_DATA struct axis2_wsdl_extensible_component_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *free)(
                    axis2_wsdl_extensible_component_t *wsdl_extensible_component,
			        const axis2_env_t *env);

    /**
     * Will add a <code>WSDLFeature</code> to the feature list.
     * If feature is null it will not be added.
     * <code>ExtensibleComponent</code>
     *
     * @param wsdl_feature
     */
    axis2_status_t (AXIS2_CALL *
    add_feature) (axis2_wsdl_extensible_component_t *extensible_component,
                const axis2_env_t *env,
                struct axis2_wsdl_feature *wsdl_feature);
        
    /**
     * Will return the <code>WSDLFeature</code>s. If there aren't
     * any features an empty list will be returned.
     *
     * @return
     */
    axis2_linked_list_t *(AXIS2_CALL *
    get_features) (axis2_wsdl_extensible_component_t *extensible_component,
                   const axis2_env_t *env);
        
    /**
     * Will add the property to the component properties. If the property is null it will
     * not be added.
     *
     * @param wsdlProperty
     */
    axis2_status_t (AXIS2_CALL *
    add_property) (axis2_wsdl_extensible_component_t *extensible_component,
                   const axis2_env_t *env,
                   struct axis2_wsdl_property *wsdl_property);
        
    /**
     * Returns the Component Properties. If none exist an empty list will be returned.
     *
     * @return
     */
    axis2_linked_list_t *(AXIS2_CALL *
    get_properties) (axis2_wsdl_extensible_component_t *extensible_component,
                     const axis2_env_t *env);
};

/** 
 * @brief Wsdl container struct
 *	Property for params  
 */  
AXIS2_DECLARE_DATA struct axis2_wsdl_extensible_component
{
	axis2_wsdl_extensible_component_ops_t *ops;
    struct axis2_wsdl_component *wsdl_component;
};

/**
 * Creates wsdl property struct
 * @return pointer to newly created wsdl property
 */
AXIS2_EXTERN axis2_wsdl_extensible_component_t * AXIS2_CALL 
axis2_wsdl_extensible_component_create (const axis2_env_t *env);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_EXTENSIBLE_COMPONENT_FREE(extensible_component, env) \
		((extensible_component)->ops->free (extensible_component, env))

#define AXIS2_WSDL_EXTENSIBLE_COMPONENT_ADD_FEATURE(extensible_component, env, feature) \
		((extensible_component)->ops->add_feature (extensible_component, env, feature))

#define AXIS2_WSDL_EXTENSIBLE_COMPONENT_GET_FEATURES(extensible_component, env) \
		((extensible_component)->ops->get_features (extensible_component, env))

#define AXIS2_WSDL_EXTENSIBLE_COMPONENT_ADD_PROPERTY(extensible_component, env, property) \
		((extensible_component)->ops->add_property (extensible_component, env, property))

#define AXIS2_WSDL_EXTENSIBLE_COMPONENT_GET_PROPERTIES(extensible_component, env) \
		((extensible_component)->ops->get_properties (extensible_component, env))       

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_EXTENSIBLE_COMPONENT_H */
