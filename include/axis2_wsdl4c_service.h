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
 
 #ifndef AXIS2_WSDL4C_SERVICE_H
 #define AXIS2_WSDL4C_SERVICE_H
 
 /**
 * @file axis2_wsdl4c_service.h
 * @brief defines WSDL4C_SERVICE constants
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
 * @defgroup axis2 wsdl4c_service constants
 * @ingroup axis2_wsdl4c_service 
 * @{
 */
    void *
    axis2_wsdl4c_service_create(void* parser);

    void
    axis2_wsdl4c_service_destroy(void *service);

    /**
    * axis2_wsdl4c_service_add_port
    * @return the extensibility element id of the  main binding extension element.
    *  In case of soap it would  be the id of the soap:binding element
    */
    void
    axis2_wsdl4c_service_add_port(void *service,
                                    char *name,
                                    void *binding,
                                    int svc_ext_id);
 
   /**
	* @param name of the port
	* @return service extension id of the port
	*/	
    int
    axis2_wsdl4c_service_get_port_extention(void *service,
                                            char *name);
 
   /**
	* Get the binding associated with the port
	* @param port name
	* @return binding associated with the port
	*/	
    const void *
    axis2_wsdl4c_service_get_port_binding(void *service,
                                            char *name);

	/**
	 * return names of the ports associated with the service
	 */
    axis2_array_list_t *
    axis2_wsdl4c_service_get_ports(void *service);

    /**
    * get_name
    * @return name of the Wsdl Element
    */
    char *
    axis2_wsdl4c_service_get_name(void *service);

    /**
    * get_documentation
    *   
    */
    char *
    axis2_wsdl4c_service_get_documentation(void *service);  

    /**
    *@name get_extensibility_elements/get_extensibility_attributes
    *@brief return the extensibility elements/attributes belonging 
    *       to a given namespace
    *@params namspace uri
    *@params reference to a vector<int>
    *@return true if any extensibility elements were found
    */
    int *
    axis2_wsdl4c_service_get_extensibility_elements(void *service,
                                                    char *namespc);

    int *
    axis2_wsdl4c_service_get_extensibility_attributes(void *service,
                                                        char *namespc);

    void
    axis2_wsdl4c_service_set_name(void *service,
                                    char *name);
  
    void
    axis2_wsdl4c_service_add_ext_element(void *service,
                                    int ident);
 
    void
    axis2_wsdl4c_service_add_ext_attribute(void *service,
                                            int ident);

    void
    axis2_wsdl4c_service_set_documentation(void *service, 
                                            char *doc);

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL4C_SERVICE_H */
