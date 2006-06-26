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
 
 #ifndef AXIS2_WSDL4C_SOAP_H
 #define AXIS2_WSDL4C_SOAP_H
 
 /**
 * @file axis2_wsdl4c_soap.h
 * @brief defines WSDL4C_SOAP
 */
#include <axis2_array_list.h>
#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_wsdl4c_soap wsdl4c_soap
 * @ingroup axis2_wsdl4c 
 * @{
 */
#define AXIS2_WSDL4C_HTTP_TRANSPORT "http://schemas.xmlsoap.org/soap/http"
#define AXIS2_WSDL4C_HTTP_BINDING "http://schemas.xmlsoap.org/wsdl/http/"
#define AXIS2_WSDL4C_SOAP_ENC_URI "http://schemas.xmlsoap.org/soap/encoding/"
#define AXIS2_WSDL4C_SOAP_ENV_URI "http://schemas.xmlsoap.org/soap/envelope/"
#define AXIS2_WSDL4C_SOAP_BINDING_URI "http://schemas.xmlsoap.org/wsdl/soap/"
    typedef enum
    {
      AXIS2_WSDL4C_LITERAL,
      AXIS2_WSDL4C_ENCODED
    } axis2_wsdl4c_encoding_t;

    typedef enum
    {
      AXIS2_WSDL4C_RPC,
      AXIS2_WSDL4C_DOC
    } axis2_wsdl4c_style_t;

    typedef enum
    {
      AXIS2_WSDL4C_NONE,
      AXIS2_WSDL4C_HTTP,
      AXIS2_WSDL4C_SMTP
    } axis2_wsdl4c_transport_t;

    void *
    axis2_wsdl4c_soap_create(axis2_char_t *schema_path);

    void
    axis2_wsdl4c_soap_destroy(void *soap);

    /**
     * Set path to directory containing XML schemas.
     */
    void
    axis2_wsdl4c_soap_set_schema_path(void *soap, 
                                        axis2_char_t *schema_path); 

    axis2_wsdl4c_transport_t 
    axis2_wsdl4c_soap_get_transport_method(void *soap);
 
    axis2_wsdl4c_style_t
    axis2_wsdl4c_soap_get_style(void *soap);
 
    /**
     * Returns the namespace URI of the wsdl
     * extensibility elements that it can handle.
     */
    axis2_char_t *
    axis2_wsdl4c_soap_get_namespace(void *soap); 
 
    void
    axis2_wsdl4c_soap_set_namespace_prefix(void *soap, 
                                            axis2_char_t *prefix);
 
    axis2_char_t *
    axis2_wsdl4c_soap_get_namespace_prefix(void *soap); 
   
 
    axis2_bool_t
    axis2_wsdl4c_soap_is_namespace_handler(void *soap, 
                                            axis2_char_t *ns);
  
 
    axis2_char_t *
    axis2_wsdl4c_soap_get_extensibility_schema(void *soap); 
 
 
    axis2_char_t *
    axis2_wsdl4c_soap_get_encoding_schema(void *soap);
 

    void
    axis2_wsdl4c_soap_set_schema_parser(void *soap, 
                                        void *schema_parser);
 

    /** 
     * Parent is the Wsdl parent element type under which the extensibility 
     * element has come
     */
    int 
    axis2_wsdl4c_soap_handle_element(void *soap,
                                        int parent,
                                        void *xml_pull_parser);

 
    /**
     * att_name is the extensibility attribute 
     */
    int 
    axis2_wsdl4c_soap_handle_attribute(void *soap, 
                                        int parent,
                                        axis2_char_t *att_name,
                                        void *xml_pull_parser);
 
    /** R
     * eturns a valid extensibilty element 
     */
    int 
    axis2_wsdl4c_soap_get_element_name(void *soap, 
                                        int id);
 
    int 
    axis2_wsdl4c_soap_get_elem_attribute(void *soap, 
                                            int id,
                                            int att_num);
 
    int 
    axis2_wsdl4c_soap_get_elem_attribute_value(void *soap,
                                                int id,
                                                int att_num); 

 
    /** 
     * Returns a valid extensibility attribute 
     */
    int 
    axis2_wsdl4c_soap_get_attribute_name(void *soap, int id);
 

    /** 
     * This is the start of all ids that must be used for elems/attributes
     * in this namespace
     */
    void 
    axis2_wsdl4c_soap_set_start_id(void *soap,
                                    int id);


    int
    axis2_wsdl4c_soap_get_start_id(void *soap);
 

    void
    axis2_wsdl4c_soap_set_wsdl_parser(void *soap,
                                        void *wp);
 
    void * 
    axis2_wsdl4c_soap_wsdl_parser(void *soap); 
 
    axis2_bool_t
    axis2_wsdl4c_soap_was_used(void *soap);

    void 
    axis2_wsdl4c_soap_get_operation_info(void *soap,
                                            int elem_id, 
                                            axis2_char_t **soap_action, 
                                            axis2_wsdl4c_style_t *style);
 
    void 
    axis2_wsdl4c_soap_get_body_info(void *soap,
                                    int elem_id,
                                    axis2_char_t **ns,
                                    axis2_wsdl4c_encoding_t *use,
                                    axis2_char_t **encoding_style);
 
    void 
    axis2_wsdl4c_soap_get_header_info(void *soap,
                                        int elem_id,    
                                        int *part_id,
                                        void **message);
 
    axis2_bool_t
    axis2_wsdl4c_soap_get_service_location(void *soap,
                                            int elem_id,
                                            axis2_char_t **location);

    /** TODO add more methods like this */
    axis2_bool_t
    axis2_wsdl4c_soap_is_soap_body(void *soap,
                                    int id);
 
    axis2_bool_t
    axis2_wsdl4c_soap_is_soap_header(void *soap,
                                    int id);


/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL4C_SOAP_H */
