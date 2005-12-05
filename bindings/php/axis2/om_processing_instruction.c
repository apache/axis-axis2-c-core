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
#include "axis2_om_node.h"
#include "axis2_om_processing_instruction.h"

zend_function_entry php_axis2_om_pi_class_functions[] =
{
    PHP_FALIAS(setValue , axis2_om_pi_set_value, NULL)
    PHP_FALIAS(setTarget , axis2_om_pi_set_target, NULL)
    PHP_FALIAS(getValue , axis2_om_pi_get_value, NULL)
    PHP_FALIAS(getTarget , axis2_om_pi_getTarget, NULL)
  /*  PHP_FALIAS(serialize , axis2_om_pi_serialize, NULL) */
    PHP_ME(om_pi, __construct, NULL, ZEND_ACC_PUBLIC)
    { NULL, NULL, NULL}
};


PHP_METHOD(om_pi, __construct)
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
    char *target = NULL;
    int target_len = 0;
    
    axis2_om_processing_instruction_t *om_pi = NULL;
    axis2_om_node_t *node = NULL;
    axis2_om_node_t *node_parent = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "OO!ss",&object, axis2_om_pi_class_entry, &object_parent,
                    axis2_om_node_class_entry, &target, &target_len, &value, &value_len) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    
    env = php_axis2_get_env();
    
    intern = (axis2_object_ptr)zend_object_store_get_object(object TSRMLS_CC);
    
    node_obj = (om_node_ptr)emalloc(sizeof(om_node_t));
    node_obj->builder = NULL;
    node_obj->doc = NULL;
    node_obj->node_type = OM_PI;
    
    if(object_parent)
    {
        AXIS2_GET_OBJ(node_obj_parent, object_parent, om_node_ptr, intern_parent);
        if(node_obj_parent && node_obj_parent->ptr)
            node_parent = (axis2_om_node_t *)(node_obj_parent->ptr);
    }
    
    om_pi = axis2_om_processing_instruction_create(&env, node_parent, target, value, &node);
    node_obj->ptr = node;
    intern->obj_type = OM_NODE_OBJ;
    intern->ptr = node_obj;
}

PHP_FUNCTION(axis2_om_pi_get_value)
{
    axis2_object_ptr intern = NULL;
    om_node_ptr node_obj = NULL;
    zval *object = NULL;
    axis2_env_t *env = NULL;
    char *value = NULL;

    axis2_om_processing_instruction_t *om_pi = NULL;
    axis2_om_node_t *node = NULL;
    
    
    env = php_axis2_get_env();
    
    AXIS2_GET_THIS(object);
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
    node = (axis2_om_node_t *)(node_obj->ptr);
    if(node)
    {
        om_pi = (axis2_om_processing_instruction_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(node, &env);
        value = AXIS2_OM_PROCESSING_INSTRUCTION_GET_VALUE(om_pi, &env);
        RETURN_STRING(value, 1);
    }
    RETURN_NULL();
}


PHP_FUNCTION(axis2_om_pi_getTarget)
{
    axis2_object_ptr intern = NULL;
    om_node_ptr node_obj = NULL;
    zval *object = NULL;
    axis2_env_t *env = NULL;
    char *target = NULL;

    axis2_om_processing_instruction_t *om_pi = NULL;
    axis2_om_node_t *node = NULL;
    
    
    env = php_axis2_get_env();
    
    AXIS2_GET_THIS(object);
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
    node = (axis2_om_node_t *)(node_obj->ptr);
    if(node)
    {
        om_pi = (axis2_om_processing_instruction_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(node, &env);
        target = AXIS2_OM_PROCESSING_INSTRUCTION_GET_TARGET(om_pi, &env);
        RETURN_STRING(target , 1);
    }
    RETURN_NULL();
}

PHP_FUNCTION(axis2_om_pi_set_value)
{
    axis2_object_ptr intern = NULL;
    om_node_ptr node_obj = NULL;
    
    zval *object = NULL;
    axis2_env_t *env = NULL;
    char *value = NULL;
    int value_len = 0;

    axis2_om_processing_instruction_t *om_pi = NULL;
    axis2_om_node_t *node = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "Os",&object, axis2_om_pi_class_entry, &value, &value_len) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
    
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
    
    node = (axis2_om_node_t *)(node_obj->ptr);
    if(node)
    {
        om_pi = (axis2_om_processing_instruction_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(node, &env);     
        AXIS2_OM_PROCESSING_INSTRUCION_SET_VALUE(om_pi, &env, value);
    }
}
PHP_FUNCTION(axis2_om_pi_set_target)
{
    axis2_object_ptr intern = NULL;
    om_node_ptr node_obj = NULL;
    
    zval *object = NULL;
    axis2_env_t *env = NULL;
    char *target = NULL;
    int target_len = 0;

    axis2_om_processing_instruction_t *om_pi = NULL;
    axis2_om_node_t *node = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "Os",&object, axis2_om_pi_class_entry, &target, &target_len) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
    
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
    
    node = (axis2_om_node_t *)(node_obj->ptr);
    if(node)
    {
        om_pi = (axis2_om_processing_instruction_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(node, &env);     
        AXIS2_OM_PROCESSING_INSTRUCION_SET_TARGET(om_pi, &env, target);
    }
}

PHP_FUNCTION(axis2_om_pi_serialize){}

