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
 
 #ifndef AXIS2_WSDL4C_BINDING_H
 #define AXIS2_WSDL4C_BINDING_H
 
 /**
 * @file axis2_wsdl4c_binding.h
 * @brief defines WSDL4C_BINDING constants
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
 * @defgroup axis2_wsdl4c_binding wsdl4c_binding
 * @ingroup axis2_wsdl4c 
 * @{
 */
    void *
    axis2_wsdl4c_binding_create(void* parser);

    void
    axis2_wsdl4c_binding_destroy(void *binding);

    /**
     * axis2_wsdl4c_binding_get_binding_info
     * @return the extensibility element id of the  main binding extension 
     * element. In case of soap it would  be the id of the soap:binding element
     */
    int
    axis2_wsdl4c_binding_get_binding_info(void *binding);

    /**
     * axis2_wsdl4c_binding_get_port_type
     * @return the wsdl port type associated with this binding
     */
    void *
    axis2_wsdl4c_binding_get_port_type(void *binding);

    /**
     * axis2_wsdl4c_binding_get_service_ext_id
     * @return the id of the service element assocaited with this binding
     */
    int
    axis2_wsdl4c_binding_get_service_ext_id(void *binding);

    /**
     * axis2_wsdl4c_binding_num_ops
     * @return number of operations in this binding
     */
    int
    axis2_wsdl4c_binding_num_ops(void *binding);

    /**
     * axis2_wsdl4c_binding_get_operation
     * @param the index of the operation (index ranges from 0 to number of 
     * operations)
     * @return pointer to the  operation
     */
    void *
    axis2_wsdl4c_binding_get_operation(void *binding, 
                                        int index);

    /**
     * axis2_wsdl4c_binding_get_binding_method
     * @param void
     * @return The namespace of the binding protocol 
     *         example "http://schemas.xmlsoap.org/wsdl/soap/"
     */
    char *
    axis2_wsdl4c_binding_get_binding_method(void *binding);

    /**
     * axis2_wsdl4c_binding_get_op_binding
     * @return The number of binding extensibility elements assocated with each
     *  message of the operation whose index is given
     *  returns a pointer to a list of extensibility binding ids
     */
    int
    axis2_wsdl4c_binding_get_op_binding(void *binding, 
                                        int index, 
                                        int **bindings);

    int
    axis2_wsdl4c_binding_get_output_binding(void *binding, 
                                                int index, 
                                                int ** bindings);

    int
    axis2_wsdl4c_binding_get_input_binding(void *binding, 
                                            int index, 
                                            int ** bindings);

    int
    axis2_wsdl4c_binding_get_fault_binding(void *binding, 
                                            int index, 
                                            int ** bindings);

    void
    axis2_wsdl4c_binding_set_port_type(void *binding, 
                                        void *port_type);

    void
    axis2_wsdl4c_binding_set_binding_info(void *binding, 
                                            int id);

    void
    axis2_wsdl4c_binding_set_method(void *binding, 
                                    char *ns);

    void
    axis2_wsdl4c_binding_set_service_ext_id(void *binding, 
                                            int id);

    /**
     * Add an operation to the binding .The index returned is to be used
     * to add more bindings to the operation
     */
    int
    axis2_wsdl4c_binding_add_operation(void *binding, 
                                        void *operation);
 
    void
    axis2_wsdl4c_binding_add_op_binding(void *binding, 
                                        int index, 
                                        int oBn);
    void
    axis2_wsdl4c_binding_add_output_binding(void *binding, 
                                        int index, 
                                        int opBn);

    void
    axis2_wsdl4c_binding_add_input_binding(void *binding, 
                                    int index, 
                                    int ipBn);

    void
    axis2_wsdl4c_binding_add_fault_binding(void *binding, 
                                    int index, 
                                    int fBn); 
    
    /**
     * get_name
     * @return name of the Wsdl Element
     */
    char *
    axis2_wsdl4c_binding_get_name(void *binding);

    /**
     * get_documentation
     *   
     */
    char *
    axis2_wsdl4c_binding_get_documentation(void *binding);  

    /**
     *@name get_extensibility_elements/get_extensibility_attributes
     *@brief return the extensibility elements/attributes belonging 
     *       to a given namespace
     *@params namspace uri
     *@return true if any extensibility elements were found
     */
    int *
    axis2_wsdl4c_binding_get_extensibility_elements(void *binding,
                                                    char *namespc);

    int *
    axis2_wsdl4c_binding_get_extensibility_attributes(void *binding,
                                                        char *namespc);

    void
    axis2_wsdl4c_binding_set_name(void *binding,
                                    char *name);
  
    void
    axis2_wsdl4c_binding_add_ext_element(void *binding,
                                    int ident);
 
    void
    axis2_wsdl4c_binding_add_ext_attribute(void *binding,
                                            int ident);

    void
    axis2_wsdl4c_binding_set_documentation(void *binding, 
                                            char *doc);

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL4C_BINDING_H */
