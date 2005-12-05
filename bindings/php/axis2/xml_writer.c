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


#include "php.h"
#include "php_axis2.h"
#include "axis2_xml_writer.h"


zend_function_entry php_axis2_xml_writer_class_functions[]=
{
    PHP_FALIAS(writeStartElementWithNamespace, axis2_xml_writer_write_start_element_with_namespace, NULL)
    PHP_FALIAS(endStartElement, axis2_xml_writer_end_start_element, NULL)
    PHP_FALIAS(getXML, axis2_xml_writer_get_xml, NULL)
    PHP_FALIAS(writeStartElement, axis2_xml_writer_write_start_element, NULL)
    PHP_FALIAS(writeEndElement, axis2_xml_writer_write_end_element, NULL)
    PHP_ME(axis2_xml_writer, __construct, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};


/**
   PHP_FALIAS(writeEmptyElement, axis2_xml_writer_write_empty_element, NULL)
    PHP_FALIAS(writeEmptyElementWithNamespace, axis2_xml_writer_write_empty_element_with_namespace, NULL)
    PHP_FALIAS(writeEmptyElementWithNamespacePrefix, axis2_xml_writer_write_empty_element_with_namespace_prefix, NULL)
    
    PHP_FALIAS(writeEndDocument, axis2_xml_writer_write_end_document, NULL)
    PHP_FALIAS(writeAttribute, axis2_xml_writer_write_attribute, NULL)
    PHP_FALIAS(writeAttributeWithNamespace, axis2_xml_writer_write_attribute_with_namespace, NULL)
    PHP_FALIAS(writeAttributeWithNamespacePrefix, axis2_xml_writer_write_attribute_with_namespace_prefix, NULL)
    PHP_FALIAS(writeNamespace, axis2_xml_writer_write_namespace, NULL)
    PHP_FALIAS(writeComment, axis2_xml_writer_write_comment, NULL)
    PHP_FALIAS(writeProcessingInstruction, axis2_xml_writer_write_processing_instruction, NULL)
    PHP_FALIAS(writeProcessingInstructionData, axis2_xml_writer_write_processing_instruction_data, NULL)
    PHP_FALIAS(writeStartDocument, axis2_xml_writer_write_start_document, NULL)
    PHP_FALIAS(writeStartDocumentWithVersion, axis2_xml_writer_write_start_document_with_version, NULL)
    PHP_FALIAS(writeStartDocumentWithVersionEncoding, axis2_xml_writer_write_start_document_with_version_encoding, NULL)
    
    
    PHP_FALIAS(writeCharacters, axis2_xml_writer_write_characters, NULL)
    
    
    
    PHP_FALIAS(writeStartElementNSPrefix, axis2_xml_writer_write_start_element_with_namespace_prefix, NULL)

*/


PHP_METHOD(axis2_xml_writer, __construct)
{
    axis2_object_ptr intern = NULL;
    zval *object = NULL;
    om_object_ptr om_obj = NULL;
    axis2_env_t *env = NULL;
    char *encoding = NULL;
    int enc_len = 0;
    int pdef = 0;
    int cmp = 0;
    axis2_xml_writer_t *writer = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "Os!ll",&object, axis2_xml_writer_class_entry, &encoding, &enc_len,&pdef, &cmp) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
    intern = (axis2_object_ptr)zend_object_store_get_object(object TSRMLS_CC);
    om_obj = (om_object_ptr)emalloc(sizeof(om_object));
    
    om_obj->obj_type = AXIS2_XML_WRITER;
    
    om_obj->ptr = NULL;
    
    writer = axis2_xml_writer_create_for_memory (&env,encoding, pdef, cmp);
    
    if(!writer)
    {
        /*php_error_docref(NULL TSRMLS_CC, E_OUTOFMEMORY, " writer create failure ");*/
    }
    
    om_obj->ptr = writer;
    intern->obj_type = OM_OBJ;
    intern->ptr = om_obj;
}

PHP_FUNCTION(axis2_xml_writer_write_start_element)
{
    axis2_object_ptr intern = NULL;
    om_object_ptr om_obj = NULL;
    zval *object = NULL;
    axis2_env_t *env = NULL;
    char *lname = NULL;
    int lname_len = 0;
    axis2_xml_writer_t *writer = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "Os",&object, axis2_xml_writer_class_entry ,&lname, &lname_len) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);
    writer = (axis2_xml_writer_t *)(om_obj->ptr);
    AXIS2_XML_WRITER_WRITE_START_ELEMENT(writer, &env, lname);
}

PHP_FUNCTION(axis2_xml_writer_end_start_element)
{
    axis2_object_ptr intern = NULL;
    om_object_ptr om_obj = NULL;
    zval *object = NULL;
    axis2_env_t *env = NULL;
    axis2_xml_writer_t *writer = NULL;
    
    env = php_axis2_get_env();
    AXIS2_GET_THIS(object);    
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);
    writer = (axis2_xml_writer_t *)(om_obj->ptr);
    AXIS2_XML_WRITER_END_START_ELEMENT(writer, &env);
}

PHP_FUNCTION(axis2_xml_writer_write_end_element)
{
    axis2_object_ptr intern = NULL;
    om_object_ptr om_obj = NULL;
    zval *object = NULL;
    axis2_env_t *env = NULL;
    axis2_xml_writer_t *writer = NULL;
    
    env = php_axis2_get_env();
    AXIS2_GET_THIS(object);
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);
    writer = (axis2_xml_writer_t *)(om_obj->ptr);
    AXIS2_XML_WRITER_WRITE_END_ELEMENT(writer, &env);
}
PHP_FUNCTION(axis2_xml_writer_write_start_element_with_namespace)
{
    axis2_object_ptr intern = NULL;
    om_object_ptr om_obj = NULL;
    zval *object = NULL;
    axis2_env_t *env = NULL;
    
    char *lname = NULL;
    int lname_len = 0;
    char *uri = NULL;
    int uri_len = 0;
    
    axis2_xml_writer_t *writer = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "Oss",&object, axis2_xml_writer_class_entry , &lname, &lname_len, &uri, &uri_len) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    
    env = php_axis2_get_env();
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);
    writer = (axis2_xml_writer_t *)(om_obj->ptr);
    AXIS2_XML_WRITER_WRITE_START_ELEMENT_WITH_NAMESPACE(writer, &env, lname, uri);

}
PHP_FUNCTION(axis2_xml_writer_write_start_element_with_namespace_prefix){}

PHP_FUNCTION(axis2_xml_writer_write_end_document){}
PHP_FUNCTION(axis2_xml_writer_write_attribute){}
PHP_FUNCTION(axis2_xml_writer_write_attribute_with_namespace){}
PHP_FUNCTION(axis2_xml_writer_write_attribute_with_namespace_prefix){}
PHP_FUNCTION(axis2_xml_writer_write_namespace){}
PHP_FUNCTION(axis2_xml_writer_write_default_namespace){}
PHP_FUNCTION(axis2_xml_writer_write_comment){}
PHP_FUNCTION(axis2_xml_writer_write_processing_instruction){}
PHP_FUNCTION(axis2_xml_writer_write_processing_instruction_data){}
PHP_FUNCTION(axis2_xml_writer_write_cdata){}
PHP_FUNCTION(axis2_xml_writer_write_dtd){}
PHP_FUNCTION(axis2_xml_writer_write_entity_ref){}
PHP_FUNCTION(axis2_xml_writer_write_start_document){}
PHP_FUNCTION(axis2_xml_writer_write_start_document_with_version){}
PHP_FUNCTION(axis2_xml_writer_write_start_document_with_version_encoding){}
PHP_FUNCTION(axis2_xml_writer_write_characters){}
PHP_FUNCTION(axis2_xml_writer_get_prefix){}
PHP_FUNCTION(axis2_xml_writer_set_prefix){}
PHP_FUNCTION(axis2_xml_writer_set_default_prefix){}
PHP_FUNCTION(axis2_xml_writer_write_encoded){}


PHP_FUNCTION(axis2_xml_writer_write_empty_element){}
PHP_FUNCTION(axis2_xml_writer_write_empty_element_with_namespace){}
PHP_FUNCTION(axis2_xml_writer_write_empty_element_with_namespace_prefix){}

PHP_FUNCTION(axis2_xml_writer_get_xml)
{
    axis2_object_ptr intern = NULL;
    om_object_ptr om_obj = NULL;
    zval *object = NULL;
    axis2_env_t *env = NULL;
    axis2_xml_writer_t *writer = NULL;
    char *output = NULL;

    env = php_axis2_get_env();
    
    AXIS2_GET_THIS(object);    
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);
    
    writer = (axis2_xml_writer_t *)(om_obj->ptr);
    output = AXIS2_XML_WRITER_GET_XML(writer, &env);
    if(output)
    {
        RETURN_STRING(output, 1);
    } 
    RETURN_NULL();
}
