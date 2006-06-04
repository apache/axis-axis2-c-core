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

#ifndef AXIS2_WSDL_EXT_SOAP_ADDRESS_H
#define AXIS2_WSDL_EXT_SOAP_ADDRESS_H

/**
 * @file axis2_wsdl_ext_soap_address.h
 * @brief Axis2 Wsdl ext_soap_address interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_wsdl_component.h>
#include <axis2_wsdl_ext.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_wsdl_component;    
typedef struct axis2_wsdl_ext_soap_address axis2_wsdl_ext_soap_address_t;
typedef struct axis2_wsdl_ext_soap_address_ops axis2_wsdl_ext_soap_address_ops_t;

/** @defgroup axis2_wsdl_ext_soap_address Wsdl Ext Soap Address
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl Ext Soap Address ops struct
 * Encapsulator struct for ops of axis2_wsdl_ext_soap_address
 */
AXIS2_DECLARE_DATA struct axis2_wsdl_ext_soap_address_ops
{
   /** De-allocate memory
      * @return status code
      */
   axis2_status_t (AXIS2_CALL *
    free)(axis2_wsdl_ext_soap_address_t *ext_soap_address,
           const axis2_env_t *env);

    axis2_char_t *(AXIS2_CALL *
    get_location_uri) (axis2_wsdl_ext_soap_address_t *ext_soap_address,
                                    const axis2_env_t *env);

    /**
     * Set the Endpoint Uri
     */
    axis2_status_t (AXIS2_CALL *
    set_location_uri) (axis2_wsdl_ext_soap_address_t *ext_soap_address,
                                    const axis2_env_t *env,
                                    axis2_char_t *location_uri);
 
};

/** 
 * @brief Wsdl container struct
 *   Property for params  
 * This Extensibility Element is extended to handle particularly the
 * SOAP Adress or the Endpoint URL.
 */  
AXIS2_DECLARE_DATA struct axis2_wsdl_ext_soap_address
{
    axis2_wsdl_ext_t base;
   axis2_wsdl_ext_soap_address_ops_t *ops;
    struct axis2_wsdl_extensible_element *ext_element;
};

/**
 * Creates wsdl ext_soap_address struct
 * @return pointer to newly created wsdl ext_soap_address
 */
AXIS2_EXTERN axis2_wsdl_ext_soap_address_t * AXIS2_CALL 
axis2_wsdl_ext_soap_address_create (const axis2_env_t *env,
                                    axis2_qname_t *qtype);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_EXT_SOAP_ADDRESS_FREE(ext_soap_address, env) \
      ((ext_soap_address)->ops->free (ext_soap_address, env))

#define AXIS2_WSDL_EXT_SOAP_ADDRESS_GET_LOCATION_URI(ext_soap_address, env) \
      ((ext_soap_address)->ops->get_location_uri (ext_soap_address, env))

#define AXIS2_WSDL_EXT_SOAP_ADDRESS_SET_LOCATION_URI(ext_soap_address, env, location_uri) \
      ((ext_soap_address)->ops->set_location_uri (ext_soap_address, env, location_uri))        

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_EXT_SOAP_ADDRESS_H */
