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

#ifndef AXIS2_WSDL_EXT_SOAP_BINDING_H
#define AXIS2_WSDL_EXT_SOAP_BINDING_H

/**
 * @file axis2_wsdl_ext_soap_binding.h
 * @brief Axis2 Wsdl ext_soap_binding interface
 */

#include <axis2.h>
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
typedef struct axis2_wsdl_ext_soap_binding axis2_wsdl_ext_soap_binding_t;
typedef struct axis2_wsdl_ext_soap_binding_ops axis2_wsdl_ext_soap_binding_ops_t;

/** @defgroup axis2_wsdl_ext_soap_binding Wsdl Ext Soap Binding
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl Ext Soap Binding ops struct
 * Encapsulator struct for ops of axis2_wsdl_ext_soap_binding
 */
AXIS2_DECLARE_DATA struct axis2_wsdl_ext_soap_binding_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *
    free)(axis2_wsdl_ext_soap_binding_t *ext_soap_binding,
	        axis2_env_t **env);

    axis2_char_t *(AXIS2_CALL *
    get_style) (axis2_wsdl_ext_soap_binding_t *ext_soap_binding,
                                    axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_style) (axis2_wsdl_ext_soap_binding_t *ext_soap_binding,
                                    axis2_env_t **env,
                                    axis2_char_t *style);

    axis2_char_t *(AXIS2_CALL *
    get_transport_uri) (axis2_wsdl_ext_soap_binding_t *ext_soap_binding,
                                    axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_transport_uri) (axis2_wsdl_ext_soap_binding_t *ext_soap_binding,
                                    axis2_env_t **env,
                                    axis2_char_t *transport_uri);
     
};

/** 
 * @brief Wsdl container struct
 *	Property for params  
 */  
AXIS2_DECLARE_DATA struct axis2_wsdl_ext_soap_binding
{
	axis2_wsdl_ext_soap_binding_ops_t *ops;
    struct axis2_wsdl_extensible_element *ext_element;
};

/**
 * Creates wsdl ext_soap_binding struct
 * @return pointer to newly created wsdl ext_soap_binding
 */
AXIS2_DECLARE(axis2_wsdl_ext_soap_binding_t *) 
axis2_wsdl_ext_soap_binding_create (axis2_env_t **env,
                                    axis2_qname_t *qtype);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_EXT_SOAP_BINDING_FREE(ext_soap_binding, env) \
		((ext_soap_binding)->ops->free (ext_soap_binding, env))

#define AXIS2_WSDL_EXT_SOAP_BINDING_GET_STYLE(ext_soap_binding, env) \
		((ext_soap_binding)->ops->get_style (ext_soap_binding, env))

#define AXIS2_WSDL_EXT_SOAP_BINDING_SET_STYLE(ext_soap_binding, env, style) \
		((ext_soap_binding)->ops->set_style (ext_soap_binding, env, style))
        
#define AXIS2_WSDL_EXT_SOAP_BINDING_GET_TRANSPORT_URI(ext_soap_binding, env) \
		((ext_soap_binding)->ops->get_transport_uri (ext_soap_binding, env))

#define AXIS2_WSDL_EXT_SOAP_BINDING_SET_TRANSPORT_URI(ext_soap_binding, env, transport_uri) \
		((ext_soap_binding)->ops->set_transport_uri (wsdl_feature, env, transport_uri))        

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_EXT_SOAP_BINDING_H */
