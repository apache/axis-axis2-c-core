/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2004 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.0 of the PHP license,       |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_0.txt.                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

#include "php.h"
#include "php_axis2.h"
#include "axis2_om_node.h"
#include "axis2_om_text.h"

zend_function_entry php_axis2_om_text_class_functions[]  = 
{
    PHP_FALIAS(getValue, axis2_om_text_get_value, NULL)
    PHP_FALIAS(setValue, axis2_om_text_get_value, NULL)
 /*   PHP_FALIAS(serialize, axis2_om_text_serialize, NULL) */
    PHP_ME(om_text, __construct, NULL, ZEND_ACC_PUBLIC)    
    { NULL, NULL, NULL}
};


PHP_METHOD(om_text, __construct)
{
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_parent = NULL;
    
    om_node_ptr node_obj = NULL;
    om_node_ptr node_obj_parent = NULL;
    
    zval *object = NULL;
    zval *object_parent = NULL;
    
    axis2_env_t *env = NULL;
    char *value = NULL;
    int value_len = 0;

    axis2_om_text_t *om_text = NULL;
    axis2_om_node_t *node = NULL;
    axis2_om_node_t *node_parent = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "OO!s",&object, axis2_om_text_class_entry, &object_parent,
                    axis2_om_node_class_entry, &value, &value_len) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
    
    intern = (axis2_object_ptr)zend_object_store_get_object(object TSRMLS_CC);
    node_obj = (om_node_ptr)emalloc(sizeof(om_node_t));
    node_obj->builder = NULL;
    node_obj->doc = NULL;
    node_obj->node_type = OM_ELEMENT;
    
    if(object_parent)
    {
        AXIS2_GET_OBJ(node_obj_parent, object_parent, om_node_ptr, intern_parent);
        if(node_obj_parent && node_obj_parent->ptr)
            node_parent = (axis2_om_node_t *)(node_obj_parent->ptr);
    }
    
    om_text = axis2_om_text_create(&env, node_parent, value, &node);
    node_obj->ptr = node;
    intern->obj_type = OM_NODE_OBJ;
    intern->ptr = node_obj;
}

PHP_FUNCTION(axis2_om_text_set_value)
{
    axis2_object_ptr intern = NULL;
    om_node_ptr node_obj = NULL;
    zval *object = NULL;

    char *value = NULL;
    int value_len = 0;
    
    axis2_env_t *env = NULL;
    axis2_om_text_t *om_text = NULL;
    axis2_om_node_t *node = NULL;
   
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "Os",&object, axis2_om_text_class_entry, &value,
                    &value_len) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
    
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
    
    node = (axis2_om_node_t*)(node_obj->ptr);
    om_text = (axis2_om_text_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(node , &env);
    if(om_text)
    {
        AXIS2_OM_TEXT_SET_VALUE(om_text, &env, value);
    }
}


PHP_FUNCTION(axis2_om_text_get_value)
{
    axis2_object_ptr intern = NULL;
    om_node_ptr node_obj = NULL;
    
    zval *object = NULL;
    
    axis2_env_t *env = NULL;
    char *value = NULL;
    axis2_om_text_t *om_text = NULL;
    axis2_om_node_t *node = NULL;

    AXIS2_GET_THIS(object);
    
    env = php_axis2_get_env();
    
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
    node = (axis2_om_node_t*)(node_obj->ptr);
    om_text = (axis2_om_text_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(node , &env);
   
    value = AXIS2_OM_TEXT_GET_VALUE(om_text, &env);
    if(value)
    {
        RETURN_STRING(value, 1);
    }
    RETURN_NULL();
}
PHP_FUNCTION(axis2_om_text_serialize)
{

}