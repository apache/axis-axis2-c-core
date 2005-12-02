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


typedef enum {

    OM_ELEMENT = 0,
    OM_NODE,
    OM_PI,
    OM_COMMENT,
    OM_TEXT,
    OM_DOCTYPE,
    AXIS2_XML_READER,
    AXIS2_XML_WRITER,
    OM_DOCUMENT,
    OM_STAX_BUILDER
}om_obj_types;


#endif /* AXIS2_FE_H */