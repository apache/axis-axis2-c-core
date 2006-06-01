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

#ifndef AXIS2_WSDL_EXT_SOAP_HEADER_H
#define AXIS2_WSDL_EXT_SOAP_HEADER_H

/**
 * @file axis2_wsdl_ext_soap_header.h
 * @brief Axis2 Wsdl ext_soap_header interface
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
typedef struct axis2_wsdl_ext_soap_header axis2_wsdl_ext_soap_header_t;
typedef struct axis2_wsdl_ext_soap_header_ops axis2_wsdl_ext_soap_header_ops_t;

/** @defgroup axis2_wsdl_ext_soap_header Wsdl Ext Soap Header
 * @ingroup axis2_wsdl
 * @{
 */

/** 
 * @brief Wsdl Ext Soap Header ops struct
 * Encapsulator struct for ops of axis2_wsdl_ext_soap_header
 */
AXIS2_DECLARE_DATA struct axis2_wsdl_ext_soap_header_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *
    free)(axis2_wsdl_ext_soap_header_t *ext_soap_header,
	        const axis2_env_t *env);

    axis2_char_t *(AXIS2_CALL *
    get_part) (axis2_wsdl_ext_soap_header_t *ext_soap_header,
                                    const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_part) (axis2_wsdl_ext_soap_header_t *ext_soap_header,
                                    const axis2_env_t *env,
                                    axis2_char_t *part);

    axis2_qname_t *(AXIS2_CALL *
    get_msg_qname) (axis2_wsdl_ext_soap_header_t *ext_soap_header,
                                    const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_msg_qname) (axis2_wsdl_ext_soap_header_t *ext_soap_header,
                                    const axis2_env_t *env,
                                    axis2_qname_t *msg_qname);
     
    axis2_qname_t *(AXIS2_CALL *
    get_qelement) (axis2_wsdl_ext_soap_header_t *ext_soap_header,
                                    const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_qelement) (axis2_wsdl_ext_soap_header_t *ext_soap_header,
                                    const axis2_env_t *env,
                                    axis2_qname_t *qelement);
     
};

/** 
 * @brief Wsdl container struct
 *	Property for params  
 */  
AXIS2_DECLARE_DATA struct axis2_wsdl_ext_soap_header
{
	axis2_wsdl_ext_soap_header_ops_t *ops;
    struct axis2_wsdl_ext_soap_body *ext_soap_body;
};

/**
 * Creates wsdl ext_soap_header struct
 * @return pointer to newly created wsdl ext_soap_header
 */
AXIS2_EXTERN axis2_wsdl_ext_soap_header_t * AXIS2_CALL 
axis2_wsdl_ext_soap_header_create (const axis2_env_t *env,
                                    axis2_qname_t *qtype);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_EXT_SOAP_HEADER_FREE(ext_soap_header, env) \
		((ext_soap_header)->ops->free (ext_soap_header, env))

#define AXIS2_WSDL_EXT_SOAP_HEADER_GET_PART(ext_soap_header, env) \
		((ext_soap_header)->ops->get_part (ext_soap_header, env))

#define AXIS2_WSDL_EXT_SOAP_HEADER_SET_PART(ext_soap_header, env, part) \
		((ext_soap_header)->ops->set_part (ext_soap_header, env, part))
        
#define AXIS2_WSDL_EXT_SOAP_HEADER_GET_MSG_QNAME(ext_soap_header, env) \
		((ext_soap_header)->ops->get_msg_qname (ext_soap_header, env))

#define AXIS2_WSDL_EXT_SOAP_HEADER_SET_MSG_QNAME(ext_soap_header, env, msg_qname) \
		((ext_soap_header)->ops->set_msg_qname (ext_soap_header, env, msg_qname))        

#define AXIS2_WSDL_EXT_SOAP_HEADER_GET_QELEMENT(ext_soap_header, env) \
		((ext_soap_header)->ops->get_qelement (ext_soap_header, env))

#define AXIS2_WSDL_EXT_SOAP_HEADER_SET_QELEMENT(ext_soap_header, env, qelement) \
		((ext_soap_header)->ops->set_qelement (ext_soap_header, env, qelement))        

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_EXT_SOAP_HEADER_H */
