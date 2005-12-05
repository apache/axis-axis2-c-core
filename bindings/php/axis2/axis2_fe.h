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


#ifndef AXIS2_FE_H
#define AXIS2_FE_H

extern zend_function_entry php_axis2_xml_reader_class_functions[];
extern zend_function_entry php_axis2_xml_writer_class_functions[];
extern zend_function_entry php_axis2_om_element_class_functions[];
extern zend_function_entry php_axis2_om_comment_class_functions[];
extern zend_function_entry php_axis2_om_text_class_functions[];
extern zend_function_entry php_axis2_om_pi_class_functions[];
extern zend_function_entry php_axis2_om_stax_builder_class_functions[];
extern zend_function_entry php_axis2_om_document_class_functions[];
extern zend_function_entry php_axis2_om_attribute_class_functions[];
extern zend_function_entry php_axis2_om_namespace_class_functions[];
extern zend_function_entry php_axis2_om_node_class_functions[];
extern zend_function_entry php_axis2_qname_class_functions[];
extern zend_function_entry php_axis2_om_output_class_functions[];


/** axis2_xml_reader functions */
PHP_METHOD(xml_reader, __construct);
PHP_FUNCTION(axis2_xml_reader_next);
PHP_FUNCTION(axis2_xml_reader_get_attribute_count);
PHP_FUNCTION(axis2_xml_reader_get_attribute_name_by_number);
PHP_FUNCTION(axis2_xml_reader_get_attribute_prefix_by_number);
PHP_FUNCTION(axis2_xml_reader_get_attribute_namespace_by_number);
PHP_FUNCTION(axis2_xml_reader_get_attribute_value_by_number);
PHP_FUNCTION(axis2_xml_reader_get_name);
PHP_FUNCTION(axis2_xml_reader_get_value);
PHP_FUNCTION(axis2_xml_reader_get_prefix);
PHP_FUNCTION(axis2_xml_reader_get_namespace_count);
PHP_FUNCTION(axis2_xml_reader_get_namespace_uri_by_number);
PHP_FUNCTION(axis2_xml_reader_get_namespace_prefix_by_number);
PHP_FUNCTION(axis2_xml_reader_get_pi_target);
PHP_FUNCTION(axis2_xml_reader_get_pi_data);

/** xml_writer functions */

PHP_FUNCTION(axis2_xml_writer_write_start_element);
PHP_FUNCTION(axis2_xml_writer_end_start_element);
PHP_FUNCTION(axis2_xml_writer_write_start_element_with_namespace);
PHP_FUNCTION(axis2_xml_writer_write_start_element_with_namespace_prefix);
PHP_FUNCTION(axis2_xml_writer_write_empty_element);
PHP_FUNCTION(axis2_xml_writer_write_empty_element_with_namespace);
PHP_FUNCTION(axis2_xml_writer_write_empty_element_with_namespace_prefix);
PHP_FUNCTION(axis2_xml_writer_write_end_element);
PHP_FUNCTION(axis2_xml_writer_write_end_document);
PHP_FUNCTION(axis2_xml_writer_write_attribute);
PHP_FUNCTION(axis2_xml_writer_write_attribute_with_namespace);
PHP_FUNCTION(axis2_xml_writer_write_attribute_with_namespace_prefix);
PHP_FUNCTION(axis2_xml_writer_write_namespace);
PHP_FUNCTION(axis2_xml_writer_write_default_namespace);
PHP_FUNCTION(axis2_xml_writer_write_comment);
PHP_FUNCTION(axis2_xml_writer_write_processing_instruction);
PHP_FUNCTION(axis2_xml_writer_write_processing_instruction_data);
PHP_FUNCTION(axis2_xml_writer_write_cdata);
PHP_FUNCTION(axis2_xml_writer_write_dtd);
PHP_FUNCTION(axis2_xml_writer_write_entity_ref);
PHP_FUNCTION(axis2_xml_writer_write_start_document);
PHP_FUNCTION(axis2_xml_writer_write_start_document_with_version);
PHP_FUNCTION(axis2_xml_writer_write_start_document_with_version_encoding);
PHP_FUNCTION(axis2_xml_writer_write_characters);
PHP_FUNCTION(axis2_xml_writer_get_prefix);
PHP_FUNCTION(axis2_xml_writer_set_prefix);
PHP_FUNCTION(axis2_xml_writer_set_default_prefix);
PHP_FUNCTION(axis2_xml_writer_write_encoded);
PHP_FUNCTION(axis2_xml_writer_get_xml);
PHP_METHOD(axis2_xml_writer, __construct);

/********* axis2 qname functions ***************/
PHP_FUNCTION(axis2_qname_equals);
PHP_FUNCTION(axis2_qname_clone);
PHP_FUNCTION(axis2_qname_get_uri);
PHP_FUNCTION(axis2_qname_get_prefix);
PHP_FUNCTION(axis2_qname_get_localpart);
PHP_METHOD(axis2_qname, __construct);

/******** axis2_om_attribute functions *********/

PHP_FUNCTION(axis2_om_attribute_get_qname);
PHP_FUNCTION(axis2_om_attribute_serialize);
PHP_FUNCTION(axis2_om_attribute_get_localname);
PHP_FUNCTION(axis2_om_attribute_get_value);
PHP_FUNCTION(axis2_om_attribute_set_localname);
PHP_FUNCTION(axis2_om_attribute_set_value);
PHP_FUNCTION(axis2_om_attribute_get_namespace);
PHP_FUNCTION(axis2_om_attribute_set_namespace);
PHP_METHOD(om_attribute, __construct); 

/******* om_namespace functions ******************/
PHP_FUNCTION(axis2_om_namespace_equals);
PHP_FUNCTION(axis2_om_namespace_serialize);
PHP_FUNCTION(axis2_om_namespace_get_uri);
PHP_FUNCTION(axis2_om_namespace_get_prefix);
PHP_METHOD(om_namespace, __construct);

/******* om_element functions ********************/
PHP_FUNCTION(axis2_om_element_find_namespace);
PHP_FUNCTION(axis2_om_element_declare_namespace);
PHP_FUNCTION(axis2_om_element_find_namespace_with_qname);
PHP_FUNCTION(axis2_om_element_add_attribute);
PHP_FUNCTION(axis2_om_element_get_attribute);
PHP_FUNCTION(axis2_om_element_serialize);
PHP_FUNCTION(axis2_om_element_find_declared_namespace);
PHP_FUNCTION(axis2_om_element_get_localname);
PHP_FUNCTION(axis2_om_element_get_namespace);
PHP_FUNCTION(axis2_om_element_set_namespace);
PHP_FUNCTION(axis2_om_element_set_localname);
PHP_METHOD(om_element, __construct);

/******* om_comment functions ******************/

PHP_FUNCTION(axis2_om_comment_serialize);
PHP_FUNCTION(axis2_om_comment_get_value);
PHP_FUNCTION(axis2_om_comment_set_value);
PHP_METHOD(om_comment, __construct);

/******** om_document functions ******************/

PHP_FUNCTION(axis2_om_document_build_next);
PHP_FUNCTION(axis2_om_document_add_child);
PHP_FUNCTION(axis2_om_document_get_root_element);
PHP_FUNCTION(axis2_om_document_set_root_element);
PHP_FUNCTION(axis2_om_document_build_all);
PHP_METHOD(om_document, __construct);


/******* om_node_functions **********************/

PHP_FUNCTION(axis2_om_node_add_child);
PHP_FUNCTION(axis2_om_node_detach);
PHP_FUNCTION(axis2_om_node_insert_sibling_after);
PHP_FUNCTION(axis2_om_node_insert_sibling_before);
PHP_FUNCTION(axis2_om_node_set_parent);
PHP_FUNCTION(axis2_om_node_serialize);
PHP_FUNCTION(axis2_om_node_get_parent);
PHP_FUNCTION(axis2_om_node_get_first_child);
PHP_FUNCTION(axis2_om_node_get_last_child);
PHP_FUNCTION(axis2_om_node_get_previous_sibling);
PHP_FUNCTION(axis2_om_node_get_next_sibling);
PHP_FUNCTION(axis2_om_node_get_node_type);
PHP_FUNCTION(axis2_om_node_get_build_status);
PHP_METHOD(om_node, __construct);

/****** om_text functions ************************/
PHP_FUNCTION(axis2_om_text_serialize);
PHP_FUNCTION(axis2_om_text_set_value);
PHP_FUNCTION(axis2_om_text_get_value);
PHP_METHOD(om_text, __construct);

/***** om_stax_builder **************************/

PHP_FUNCTION(axis2_om_stax_builder_next);
PHP_FUNCTION(axis2_om_stax_builder_discard_current_element);
PHP_METHOD(om_stax_builder, __construct);


/******** processing instruction functions ******/
PHP_FUNCTION(axis2_om_pi_set_value);
PHP_FUNCTION(axis2_om_pi_set_target);
PHP_FUNCTION(axis2_om_pi_get_value);
PHP_FUNCTION(axis2_om_pi_getTarget);
PHP_FUNCTION(axis2_om_pi_serialize);
PHP_METHOD(om_pi, __construct);

/******** om_output *****************************/
PHP_METHOD(om_output, __construct);

typedef enum {

    NODE_INVALID = 0,
    OM_DOCUMENT,
    OM_ELEMENT,
    OM_DOCTYPE,
    OM_COMMENT,
    OM_ATTRIBUTE,
    OM_NAMESPACE,
    OM_PI,
    OM_TEXT,
    OM_NODE,
    AXIS2_XML_READER,
    AXIS2_XML_WRITER,
    OM_STAX_BUILDER,
    AXIS2_QNAME,
    OM_OUTPUT
}om_obj_types;

typedef enum{
    OM_OBJ = 0,
    OM_NODE_OBJ
}axis2_obj_types;

#endif /* AXIS2_FE_H */

