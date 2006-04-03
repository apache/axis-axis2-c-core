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
 
 #ifndef AXIS2_WSDL4C_MESSAGE_H
 #define AXIS2_WSDL4C_MESSAGE_H
 
 /**
 * @file axis2_wsdl4c_message.h
 * @brief defines WSDL4C_MESSAGE
 */
#include <axis2_array_list.h>
#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_wsdl4c_part.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2 wsdl4c_message
 * @ingroup axis2_wsdl4c
 * @{
 */
    void *
    axis2_wsdl4c_msg_create(void* parser);


    void
    axis2_wsdl4c_msg_destroy(void *message);
 

    /**
     * @name axis2_wsdl4c_msg_get_num_parts
     * @return number of parts in the message
     */
    int
    axis2_wsdl4c_msg_get_num_parts(void *message); 


    /**
     * @name axis2_wsdl4c_msg_get_part_index
     * @param the name of the part
     * @return index of the of the part whose name is passed
     *         index:0..number of parts
     */
    int
    axis2_wsdl4c_msg_get_part_index(void *message, 
                                    char *name);

    /** 
     * @name axis2_wsdl4c_msg_get_part_type
     * @param the index  of the part       index:0..number of parts
     * @return type id  of the part
     *         for ex if we have <part name="one" type="xsd:int">
     *         the id returned represents xsd:int,the schema type for integers
     */
    int
    axis2_wsdl4c_msg_get_part_type_a_index(void *message, 
                                            int index);


    int
    axis2_wsdl4c_msg_get_part_type_a_name(void *message, 
                                            char *name); 


    /** 
     * @name axis2_wsdl4c_msg_get_part_element
     * @param the index  of the part  index:0..number of parts
     * @return pointer to the Element which the part uses
     *         for ex if we have <part name="one" element="ns:elem">
     *         a pointer to the Element representing ns:elem is returned
     *         If the part's reftype is Type ,0 is returned
     */
    void *
    axis2_wsdl4c_msg_get_part_element(void *message, 
                                        int index);


    /** 
     * @name axis2_wsdl4c_msg_get_message_part
     * @param the index  of the part,or the name
     * @return pointer to the Part
     */
    void*
    axis2_wsdl4c_msg_get_message_part_a_index(void *message, 
                                                size_t index); 


    void *
    axis2_wsdl4c_msg_get_message_part_a_nam(void *message, 
                                            char *nam);


    /**
     * @name axis2_wsdl4c_msg_get_part_content_schema_id
     * @param the index  of the part,or the name
     * @return schema id to which the part's type or element belongs to
     */
    int
    axis2_wsdl4c_msg_get_part_content_schema_id_a_index(void *message, 
                                                        int index);


    int
    axis2_wsdl4c_msg_get_part_content_schema_id_a_name(void *message, 
                                                        char *name);


    char *
    axis2_wsdl4c_msg_get_part_name(void *message, 
                                    int index);


    axis2_wsdl4c_part_ref_type_t
    axis2_wsdl4c_msg_get_part_ref_type_a_nam(void *message, 
                                                char *nam);


    axis2_wsdl4c_part_ref_type_t
    axis2_wsdl4c_msg_get_part_ref_type_a_index(void *message, 
                                                int index);


    void
    axis2_wsdl4c_msg_add_part(void *message, char *pname, 
                                axis2_wsdl4c_part_ref_type_t reftype, 
                                void *d, 
                                int schema_id);

    /**
     * get_name
     * @return name of the Wsdl Element
     */
    char *
    axis2_wsdl4c_msg_get_name(void *message);

    /**
     * get_documentation
     *   
     */
    char *
    axis2_wsdl4c_msg_get_documentation(void *message);  

    /**
     *@name get_extensibility_elements/get_extensibility_attributes
     *@brief return the extensibility elements/attributes belonging 
     *       to a given namespace
     *@params namspace uri
     *@params reference to a vector<int>
     *@return true if any extensibility elements were found
     */
    int *
    axis2_wsdl4c_msg_get_extensibility_elements(void *message,
                                                    char *namespc);

    int *
    axis2_wsdl4c_msg_get_extensibility_attributes(void *message,
                                                        char *namespc);

    void
    axis2_wsdl4c_msg_set_name(void *message,
                                    char *name);
  
    void
    axis2_wsdl4c_msg_add_ext_element(void *message,
                                    int ident);
 
    void
    axis2_wsdl4c_msg_add_ext_attribute(void *message,
                                            int ident);

    void
    axis2_wsdl4c_msg_set_documentation(void *message, 
                                            char *doc);


/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL4C_MESSAGE_H */
