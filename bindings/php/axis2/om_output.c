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
#include "axis2_om_output.h"
#include "axis2_xml_writer.h"

extern zend_function_entry php_axis2_om_output_class_functions[] =
{
    PHP_ME(om_output, __construct, NULL, ZEND_ACC_PUBLIC)
    { NULL, NULL, NULL}
};

PHP_METHOD(om_output, __construct)
{
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_writer = NULL;
    
    zval *object = NULL;
    zval *object_writer = NULL;
    
    om_object_ptr om_obj = NULL;
    om_object_ptr om_obj_writer = NULL;
    
    axis2_om_output_t *om_output = NULL;
    axis2_xml_writer_t *writer = NULL;
    axis2_env_t *env = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "OO",&object, axis2_om_output_class_entry,
                 &object_writer, axis2_xml_writer_class_entry) == FAILURE)
    {
        php_std_error_handling();
        return;
    }

    env = php_axis2_get_env();    
    intern = (axis2_object_ptr)zend_object_store_get_object(object TSRMLS_CC);
    om_obj = (om_object_ptr)emalloc(sizeof(om_object));
    om_obj->obj_type = OM_OUTPUT;
    om_obj->ptr = NULL;   
    
    AXIS2_GET_OBJ(om_obj_writer, object_writer, om_object_ptr, intern_writer);
    writer = (axis2_xml_writer_t *)(om_obj_writer->ptr);
       
    om_output = axis2_om_output_create(&env, writer);
    om_obj->ptr = om_output;
    
    intern->obj_type = OM_OBJ;
    intern->ptr = om_obj;
}