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
#include "axis2_xml_reader.h"

zend_function_entry php_axis2_xml_reader_class_functions[]=
{
    PHP_FALIAS(next, axis2_xml_reader_next, NULL)
    PHP_FALIAS(getAttributeCount, axis2_xml_reader_get_attribute_count, NULL)
    PHP_FALIAS(getAttributeName , axis2_xml_reader_get_attribute_name_by_number, NULL)
    PHP_FALIAS(getAttributePrefix, axis2_xml_reader_get_attribute_prefix_by_number, NULL)
    PHP_FALIAS(getAttributeNamespace, axis2_xml_reader_get_attribute_namespace_by_number, NULL)
    PHP_FALIAS(getAttributeValue, axis2_xml_reader_get_attribute_value_by_number, NULL)
    PHP_FALIAS(getName, axis2_xml_reader_get_name, NULL)
    PHP_FALIAS(getValue, axis2_xml_reader_get_value, NULL)
    PHP_FALIAS(getPrefix, axis2_xml_reader_get_prefix, NULL)
    PHP_FALIAS(getNamespaceCount, axis2_xml_reader_get_namespace_count, NULL)
    PHP_FALIAS(getNamespaceUri, axis2_xml_reader_get_namespace_uri_by_number, NULL)
    PHP_FALIAS(getNamespacePrefix, axis2_xml_reader_get_namespace_prefix_by_number, NULL)
    PHP_FALIAS(getPITarget , axis2_xml_reader_get_pi_target, NULL)
    PHP_FALIAS(getPIData, axis2_xml_reader_get_pi_data, NULL)
    PHP_ME(xml_reader, __construct, NULL, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL} 
};

PHP_METHOD(xml_reader , __construct)
{
    zval *object = NULL;
    axis2_object_ptr axis2_obj = NULL;
    om_object_ptr om_obj = NULL;
    char *path;
    int length;
    axis2_env_t *env;
    
  
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &object, axis2_xml_reader_class_entry, &path, &length) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
     
    om_obj = emalloc(sizeof(om_object));
    om_obj->ptr = NULL;
    om_obj->obj_type = AXIS2_XML_READER;
    om_obj->ptr = axis2_xml_reader_create_for_file(&env, path, NULL);
    om_obj->ref_count = 1;
    
    axis2_obj = (axis2_object_ptr)zend_object_store_get_object(object TSRMLS_CC);   
    axis2_obj->ptr = om_obj;
   
}

PHP_FUNCTION(axis2_xml_reader_next)
{
    axis2_object_ptr axis2_obj = NULL;
    zval *obj = NULL;
    axis2_xml_reader_t *reader = NULL;
    axis2_env_t *env = NULL;
    long i = 0;
    
    obj = getThis();
    env = php_axis2_get_env();
    axis2_obj = (axis2_object_ptr)zend_object_store_get_object(obj TSRMLS_CC);
    reader =(axis2_xml_reader_t*) (((om_object_ptr)(axis2_obj->ptr))->ptr);
    i = AXIS2_XML_READER_NEXT(reader, &env);
    RETURN_LONG(i);
}
PHP_FUNCTION(axis2_xml_reader_get_attribute_count)
{
    axis2_object_ptr axis2_obj = NULL;
    zval *obj = NULL;
    axis2_xml_reader_t *reader = NULL;
    axis2_env_t *env = NULL;
    int i = 0;
    
    obj = getThis();
    env = php_axis2_get_env();
    axis2_obj = (axis2_object_ptr)zend_object_store_get_object(obj TSRMLS_CC);
    reader =(axis2_xml_reader_t*) (((om_object_ptr)(axis2_obj->ptr))->ptr);
    i = AXIS2_XML_READER_GET_ATTRIBUTE_COUNT(reader, &env);
    RETURN_LONG(i);
}
PHP_FUNCTION(axis2_xml_reader_get_attribute_name_by_number)
{
    axis2_object_ptr axis2_obj = NULL;
    zval *obj = NULL;
    axis2_xml_reader_t *reader = NULL;
    axis2_env_t *env = NULL;
    char *attr_name = NULL;
    char *name;
    long i = 0;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &obj, axis2_xml_reader_class_entry, &i) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
    axis2_obj = (axis2_object_ptr)zend_object_store_get_object(obj TSRMLS_CC);
    reader =(axis2_xml_reader_t*) (((om_object_ptr)(axis2_obj->ptr))->ptr);
    attr_name = AXIS2_XML_READER_GET_ATTRIBUTE_NAME_BY_NUMBER(reader, &env, i);
    if(!attr_name)
    {
        RETURN_NULL();
    }
    else
    {
    name = estrdup(attr_name);
    AXIS2_XML_READER_XML_FREE(reader, &env, attr_name);
    }
    RETURN_STRING(name, 0);
}
PHP_FUNCTION(axis2_xml_reader_get_attribute_prefix_by_number)
{
    axis2_object_ptr axis2_obj = NULL;
    zval *obj = NULL;
    axis2_xml_reader_t *reader = NULL;
    axis2_env_t *env = NULL;
    char *attr_prefix = NULL;
    char *prefix;
    long i = 0;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &obj, axis2_xml_reader_class_entry, &i) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
    axis2_obj = (axis2_object_ptr)zend_object_store_get_object(obj TSRMLS_CC);
    reader =(axis2_xml_reader_t*) (((om_object_ptr)(axis2_obj->ptr))->ptr);
    attr_prefix = AXIS2_XML_READER_GET_ATTRIBUTE_PREFIX_BY_NUMBER(reader, &env, i);
    if(!attr_prefix)
    {
        RETURN_NULL();
    }
    else
    {
    prefix = estrdup(attr_prefix);
    AXIS2_XML_READER_XML_FREE(reader, &env, attr_prefix);
    }
    RETURN_STRING(prefix, 0);

}
PHP_FUNCTION(axis2_xml_reader_get_attribute_namespace_by_number)
{
    axis2_object_ptr axis2_obj = NULL;
    zval *obj = NULL;
    axis2_xml_reader_t *reader = NULL;
    axis2_env_t *env = NULL;
    char *attr_ns_uri = NULL;
    char *uri;
    long i = 0;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &obj, axis2_xml_reader_class_entry, &i) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
    axis2_obj = (axis2_object_ptr)zend_object_store_get_object(obj TSRMLS_CC);
    reader =(axis2_xml_reader_t*) (((om_object_ptr)(axis2_obj->ptr))->ptr);
    attr_ns_uri = AXIS2_XML_READER_GET_ATTRIBUTE_NAMESPACE_BY_NUMBER(reader, &env, i);
    if(!attr_ns_uri)
    {
        RETURN_NULL();
    }    
    else
    {
    uri = estrdup(attr_ns_uri);
    AXIS2_XML_READER_XML_FREE(reader, &env, attr_ns_uri);
    }
    RETURN_STRING(uri, 0);
}
PHP_FUNCTION(axis2_xml_reader_get_attribute_value_by_number)
{
    axis2_object_ptr axis2_obj = NULL;
    zval *obj = NULL;
    axis2_xml_reader_t *reader = NULL;
    axis2_env_t *env = NULL;
    char *attr_value = NULL;
    char *value;
    long i = 0;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &obj, axis2_xml_reader_class_entry, &i) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
    axis2_obj = (axis2_object_ptr)zend_object_store_get_object(obj TSRMLS_CC);
    reader =(axis2_xml_reader_t*) (((om_object_ptr)(axis2_obj->ptr))->ptr);
    attr_value = AXIS2_XML_READER_GET_ATTRIBUTE_VALUE_BY_NUMBER(reader, &env, i);
    if(!attr_value)
    {
        RETURN_NULL();
    }
    else
    {   
    value = estrdup(attr_value);
    AXIS2_XML_READER_XML_FREE(reader, &env, attr_value);
    }
    RETURN_STRING(value, 0);

}
PHP_FUNCTION(axis2_xml_reader_get_name)
{
    axis2_object_ptr axis2_obj = NULL;
    zval *obj = NULL;
    axis2_xml_reader_t *reader = NULL;
    axis2_env_t *env = NULL;
    char *name = NULL;
    char *na = NULL;
    
    obj = getThis();
    env = php_axis2_get_env();
    axis2_obj = (axis2_object_ptr)zend_object_store_get_object(obj TSRMLS_CC);
    reader =(axis2_xml_reader_t*) (((om_object_ptr)(axis2_obj->ptr))->ptr);
    name = AXIS2_XML_READER_GET_NAME(reader, &env);
    if(!name)
    {
        RETURN_NULL();
    }
    else
    {        
    na = estrdup(name);
    if(name)
        AXIS2_XML_READER_XML_FREE(reader, &env, name);
    }
    RETURN_STRING(na, 0);

}
PHP_FUNCTION(axis2_xml_reader_get_value)
{
    axis2_object_ptr axis2_obj = NULL;
    zval *obj = NULL;
    axis2_xml_reader_t *reader = NULL;
    axis2_env_t *env = NULL;
    char *value = NULL;
    char *val = NULL;
    
    obj = getThis();
    env = php_axis2_get_env();
    axis2_obj = (axis2_object_ptr)zend_object_store_get_object(obj TSRMLS_CC);
    reader =(axis2_xml_reader_t*) (((om_object_ptr)(axis2_obj->ptr))->ptr);
    value = AXIS2_XML_READER_GET_VALUE(reader, &env);
    if(!value)
    {
        RETURN_NULL();
    }
    else
    {
    val = estrdup(value);
    if(value)
        AXIS2_XML_READER_XML_FREE(reader, &env, value);
    }
    RETURN_STRING(val, 0);

}
PHP_FUNCTION(axis2_xml_reader_get_prefix)
{
    axis2_object_ptr axis2_obj = NULL;
    zval *obj = NULL;
    axis2_xml_reader_t *reader = NULL;
    axis2_env_t *env = NULL;
    char *prefix = NULL;
    char *pre = NULL;
    
    obj = getThis();
    env = php_axis2_get_env();
    axis2_obj = (axis2_object_ptr)zend_object_store_get_object(obj TSRMLS_CC);
    reader =(axis2_xml_reader_t*) (((om_object_ptr)(axis2_obj->ptr))->ptr);
    prefix = AXIS2_XML_READER_GET_PREFIX(reader, &env);
    if(!prefix)
    {
       RETURN_NULL();
    }
    else
    {
    pre = estrdup(prefix);
    if(prefix)
        AXIS2_XML_READER_XML_FREE(reader, &env, prefix);
    }
    RETURN_STRING(pre, 0);

}
PHP_FUNCTION(axis2_xml_reader_get_namespace_count)
{
    axis2_object_ptr axis2_obj = NULL;
    zval *obj = NULL;
    axis2_xml_reader_t *reader = NULL;
    axis2_env_t *env = NULL;
    long i =0;
    
    obj = getThis();
    env = php_axis2_get_env();
    axis2_obj = (axis2_object_ptr)zend_object_store_get_object(obj TSRMLS_CC);
    reader =(axis2_xml_reader_t*) (((om_object_ptr)(axis2_obj->ptr))->ptr);
    i = AXIS2_XML_READER_GET_NAMESPACE_COUNT(reader, &env);
    RETURN_LONG(i);

}
PHP_FUNCTION(axis2_xml_reader_get_namespace_uri_by_number)
{
    axis2_object_ptr axis2_obj = NULL;
    zval *obj = NULL;
    axis2_xml_reader_t *reader = NULL;
    axis2_env_t *env = NULL;
    char *ns_uri = NULL;
    char *uri = NULL;
    long i = 0;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &obj, axis2_xml_reader_class_entry, &i) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
    axis2_obj = (axis2_object_ptr)zend_object_store_get_object(obj TSRMLS_CC);
    reader =(axis2_xml_reader_t*) (((om_object_ptr)(axis2_obj->ptr))->ptr);
    ns_uri = AXIS2_XML_READER_GET_NAMESPACE_URI_BY_NUMBER(reader, &env, i);
    if(!ns_uri)
    {
        RETURN_NULL();
    }
    else
    {
        uri = estrdup(ns_uri);
        AXIS2_XML_READER_XML_FREE(reader, &env, ns_uri);
    }
    RETURN_STRING(uri, 0);

}
PHP_FUNCTION(axis2_xml_reader_get_namespace_prefix_by_number)
{
    axis2_object_ptr axis2_obj = NULL;
    zval *obj = NULL;
    axis2_xml_reader_t *reader = NULL;
    axis2_env_t *env = NULL;
    char *ns_prefix = NULL;
    char *prefix = NULL;
    long i = 0;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Ol", &obj, axis2_xml_reader_class_entry, &i) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
    axis2_obj = (axis2_object_ptr)zend_object_store_get_object(obj TSRMLS_CC);
    reader =(axis2_xml_reader_t*) (((om_object_ptr)(axis2_obj->ptr))->ptr);
    ns_prefix = AXIS2_XML_READER_GET_NAMESPACE_PREFIX_BY_NUMBER(reader, &env, i);
    if(!ns_prefix)
    {
        RETURN_NULL();
    }
    else
    {
    prefix = estrdup(ns_prefix);
    AXIS2_XML_READER_XML_FREE(reader, &env, ns_prefix);
    }
    RETURN_STRING(prefix, 0);
}
PHP_FUNCTION(axis2_xml_reader_get_pi_target)
{
    axis2_object_ptr axis2_obj = NULL;
    zval *obj = NULL;
    axis2_xml_reader_t *reader = NULL;
    axis2_env_t *env = NULL;
    char *pi_target = NULL;
    char *target = NULL;
    
    obj = getThis();
    env = php_axis2_get_env();
    axis2_obj = (axis2_object_ptr)zend_object_store_get_object(obj TSRMLS_CC);
    reader =(axis2_xml_reader_t*) (((om_object_ptr)(axis2_obj->ptr))->ptr);
    pi_target = AXIS2_XML_READER_GET_PI_TARGET(reader, &env);
    if(!pi_target)
    {
        RETURN_NULL();
    }
    else
    {
    target = estrdup(pi_target);
    if(pi_target)
        AXIS2_XML_READER_XML_FREE(reader, &env, pi_target);
    }
    RETURN_STRING(target, 0);

}
PHP_FUNCTION(axis2_xml_reader_get_pi_data)
{
    axis2_object_ptr axis2_obj = NULL;
    zval *obj = NULL;
    axis2_xml_reader_t *reader = NULL;
    axis2_env_t *env = NULL;
    char *pi_data = NULL;
    char *data = NULL;
    
    obj = getThis();
    env = php_axis2_get_env();
    axis2_obj = (axis2_object_ptr)zend_object_store_get_object(obj TSRMLS_CC);
    reader =(axis2_xml_reader_t*) (((om_object_ptr)(axis2_obj->ptr))->ptr);
    pi_data = AXIS2_XML_READER_GET_PI_DATA(reader, &env);
    if(!pi_data)
    {
        RETURN_NULL();
    }
    else
    {
    data = estrdup(pi_data);
    if(pi_data)
        AXIS2_XML_READER_XML_FREE(reader, &env, pi_data);
    }
    RETURN_STRING(data, 0);
}