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
 
 #ifndef AXIS2_WSDL4C_PORT_TYPE_H
 #define AXIS2_WSDL4C_PORT_TYPE_H
 
 /**
 * @file axis2_wsdl4c_port_type.h
 * @brief defines WSDL4C_PORT_TYPE constants
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
 * @defgroup axis2 wsdl4c_port_type constants
 * @ingroup axis2_wsdl4c_port_type 
 * @{
 */
    void *
    axis2_wsdl4c_port_type_create(void* parser);

    void
    axis2_wsdl4c_port_type_destroy(void *port_type);

    int
    axis2_wsdl4c_port_type_get_num_ops(void *port_type);

    /**
    * axis2_wsdl4c_port_type_get_operation
    * @param index of the operation index:0..ops.size()-1
    * @return pointer to  the Operation
    */
    const void *
    axis2_wsdl4c_port_type_get_operation_a_index(void *port_type, 
                                        int index);

    /**
    * axis2_wsdl4c_port_type_get_operation
    * @param name of the operation
    * @return pointer to  the Operation
    */
    const void *
    axis2_wsdl4c_port_type_get_operation_a_qname(void *port_type, 
                                        void *qname);

    int
    axis2_wsdl4c_port_type_get_operation_index(void *port_type, 
                                        void *qname);

    /**
    * axis2_wsdl4c_port_type_get_operations
    * @return axis2_array_list_t
    * returns array list of operations 
    */
    axis2_array_list_t *
    axis2_wsdl4c_port_type_get_operations(void *port_type);

    /**
    * @name   binding
    * @return const Binding*
    * returns Binding associated with the port type 
    * which has a binding protocol specified by 'nsp'
    */
    const void *
    axis2_wsdl4c_port_type_binding(void *port_type, 
                            char *nsp);

    void
    axis2_wsdl4c_port_type_add_op(void *port_type, 
                        void *op);
    
    void
    axis2_wsdl4c_port_type_set_binding(void *port_type, 
                                void *bn);

    /**
    * get_name
    * @return name of the Wsdl Element
    */
    char *
    axis2_wsdl4c_port_type_get_name(void *port_type);

    /**
    * get_documentation
    *   
    */
    char *
    axis2_wsdl4c_port_type_get_documentation(void *port_type);  

    /**
    *@name get_extensibility_elements/get_extensibility_attributes
    *@brief return the extensibility elements/attributes belonging 
    *       to a given namespace
    *@params namspace uri
    *@params reference to a vector<int>
    *@return true if any extensibility elements were found
    */
    int *
    axis2_wsdl4c_port_type_get_extensibility_elements(void *port_type,
                                                    char *namespc);

    int *
    axis2_wsdl4c_port_type_get_extensibility_attributes(void *port_type,
                                                        char *namespc);

    void
    axis2_wsdl4c_port_type_set_name(void *port_type,
                                    char *name);
  
    void
    axis2_wsdl4c_port_type_add_ext_element(void *port_type,
                                    int ident);
 
    void
    axis2_wsdl4c_port_type_add_ext_attribute(void *port_type,
                                            int ident);

    void
    axis2_wsdl4c_port_type_set_documentation(void *port_type, 
                                            char *doc);

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL4C_PORT_TYPE_H */
