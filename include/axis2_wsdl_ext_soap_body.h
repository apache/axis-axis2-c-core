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

#ifndef AXIS2_WSDL_EXT_SOAP_BODY_H
#define AXIS2_WSDL_EXT_SOAP_BODY_H

/**
 * @file axis2_wsdl_ext_soap_body.h
 * @brief Axis2 Wsdl ext_soap_body interface
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
typedef struct axis2_wsdl_ext_soap_body axis2_wsdl_ext_soap_body_t;
typedef struct axis2_wsdl_ext_soap_body_ops axis2_wsdl_ext_soap_body_ops_t;

/** @defgroup axis2_wsdl_ext_soap_body Wsdl Ext Soap Body
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl Ext Soap Body ops struct
 * Encapsulator struct for ops of axis2_wsdl_ext_soap_body
 */
 struct axis2_wsdl_ext_soap_body_ops
{
   /** De-allocate memory
      * @return status code
      */
   axis2_status_t (AXIS2_CALL *
    free)(axis2_wsdl_ext_soap_body_t *ext_soap_body,
           const axis2_env_t *env);

    axis2_char_t *(AXIS2_CALL *
    get_use) (axis2_wsdl_ext_soap_body_t *ext_soap_body,
                                    const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_use) (axis2_wsdl_ext_soap_body_t *ext_soap_body,
                                    const axis2_env_t *env,
                                    axis2_char_t *use);

    axis2_char_t *(AXIS2_CALL *
    get_namespc_uri) (axis2_wsdl_ext_soap_body_t *ext_soap_body,
                                    const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_namespc_uri) (axis2_wsdl_ext_soap_body_t *ext_soap_body,
                                    const axis2_env_t *env,
                                    axis2_char_t *namespc_uri);
     
};

/** 
 * @brief Wsdl container struct
 *   Property for params  
 */  
 struct axis2_wsdl_ext_soap_body
{
   axis2_wsdl_ext_soap_body_ops_t *ops;
    struct axis2_wsdl_extensible_element *ext_element;
};

/**
 * Creates wsdl ext_soap_body struct
 * @return pointer to newly created wsdl ext_soap_body
 */
AXIS2_EXTERN axis2_wsdl_ext_soap_body_t * AXIS2_CALL 
axis2_wsdl_ext_soap_body_create (const axis2_env_t *env,
                                    axis2_qname_t *qtype);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_EXT_SOAP_BODY_FREE(ext_soap_body, env) \
      ((ext_soap_body)->ops->free (ext_soap_body, env))

#define AXIS2_WSDL_EXT_SOAP_BODY_GET_USE(ext_soap_body, env) \
      ((ext_soap_body)->ops->get_use (ext_soap_body, env))

#define AXIS2_WSDL_EXT_SOAP_BODY_SET_USE(ext_soap_body, env, use) \
      ((ext_soap_body)->ops->set_use (ext_soap_body, env, use))
        
#define AXIS2_WSDL_EXT_SOAP_BODY_GET_NAMESPC_URI(ext_soap_body, env) \
      ((ext_soap_body)->ops->get_namespc_uri (ext_soap_body, env))

#define AXIS2_WSDL_EXT_SOAP_BODY_SET_NAMESPC_URI(ext_soap_body, env, namespc_uri) \
      ((ext_soap_body)->ops->set_namespc_uri (ext_soap_body, env, namespc_uri))        

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_EXT_SOAP_BODY_H */
