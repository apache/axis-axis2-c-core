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
 
 #ifndef AXIS2_WSDL4C_OPERATION_H
 #define AXIS2_WSDL4C_OPERATION_H
 
 /**
 * @file axis2_wsdl4c_operation.h
 * @brief defines WSDL4C_OPERATION constants
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
 * @defgroup axis2 wsdl4c_operation constants
 * @ingroup axis2_wsdl4c_operation 
 * @{
 */

    enum axis2_wsdl4c_optype
    {
        AXIS2_WSDL4C_OP_NONE ,
        AXIS2_WSDL4C_OP_IN ,
        AXIS2_WSDL4C_OP_OUT ,
        AXIS2_WSDL4C_OP_IN_OUT ,
        AXIS2_WSDL4C_OP_OUT_IN
    };

    enum axis2_wsdl4c_msg_type
    {
        AXIS2_WSDL4C_INPUT,
        AXIS2_WSDL4C_OUTPUT,
        AXIS2_WSDL4C_FAULT
    };

    typedef enum axis2_wsdl4c_optype axis2_wsdl4c_optype_t;
    typedef enum axis2_wsdl4c_msg_type axis2_wsdl4c_msg_type_t;

    void *
    axis2_wsdl4c_operation_create(void* parser, 
                                    void *port_type);

    void
    axis2_wsdl4c_operation_destroy(void *operation);

    /**
    * axis2_wsdl4c_operation_get_message
    * @param mesage type ,Input,Output or Fault
    * @return pointer to the message
    */
    const void *
    axis2_wsdl4c_operation_get_message(void *operation, 
                                        axis2_wsdl4c_msg_type_t type );

    axis2_array_list_t *
    axis2_wsdl4c_operation_get_faults(void *operation);

    /**
    * axis2_wsdl4c_operation_get_optype
    * @return type of the operation in,in-out,out,out-in
    */
    axis2_wsdl4c_optype_t
    axis2_wsdl4c_operation_get_type(void *operation);

    /**
    * axis2_wsdl4c_operation_port_type
    * @return The porttype to which this operation belongs
    */
    const void *
    axis2_wsdl4c_operation_port_type(void *operation);

    void
    axis2_wsdl4c_operation_set_message(void *operation, 
                                        void *message, 
                                        axis2_wsdl4c_msg_type_t type);

    /**
    * get_name
    * @return name of the Wsdl Element
    */
    const char *
    axis2_wsdl4c_operation_get_name(void *operation);

    /**
    * get_documentation
    *   
    */
    const char *
    axis2_wsdl4c_operation_get_documentation(void *operation);  

    /**
    *@name get_extensibility_elements/get_extensibility_attributes
    *@brief return the extensibility elements/attributes belonging 
    *       to a given namespace
    *@params namspace uri
    *@params reference to a vector<int>
    *@return true if any extensibility elements were found
    */
    int *
    axis2_wsdl4c_operation_get_extensibility_elements(void *operation,
                                                    char *namespc);

    int *
    axis2_wsdl4c_operation_get_extensibility_attributes(void *operation,
                                                        char *namespc);

    void
    axis2_wsdl4c_operation_set_name(void *operation,
                                    char *name);
  
    void
    axis2_wsdl4c_operation_add_ext_element(void *operation,
                                    int ident);
 
    void
    axis2_wsdl4c_operation_add_ext_attribute(void *operation,
                                            int ident);

    void
    axis2_wsdl4c_operation_set_documentation(void *operation, 
                                            char *doc);

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL4C_OPERATION_H */
