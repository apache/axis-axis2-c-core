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
#include "axis2_om_attribute.h"

zend_function_entry php_axis2_om_attribute_class_functions[]=
{
    PHP_FALIAS(getQname , axis2_om_attribute_get_qname, NULL)
    PHP_FALIAS(serialize, axis2_om_attribute_serialize, NULL)
    PHP_FALIAS(getLocalname , axis2_om_attribute_get_localname, NULL)
    PHP_FALIAS(getValue , axis2_om_attribute_get_value, NULL)
    PHP_FALIAS(setLocalname , axis2_om_attribute_set_localname, NULL)
    PHP_FALIAS(setValue , axis2_om_attribute_set_value, NULL)
    PHP_FALIAS(getNamespace , axis2_om_attribute_get_namespace, NULL)
    PHP_FALIAS(setNamespace , axis2_om_attribute_set_namespace, NULL)
    PHP_ME(om_attribute, __construct, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

PHP_METHOD(om_attribute, __construct)
{
    axis2_object_ptr intern = NULL;
    om_object_ptr om_obj = NULL;
    zval *object = NULL;
    axis2_env_t *env = NULL;
    char *lname = NULL;
    char *val =  NULL;
    int lname_len = 0;
    int val_len = 0;
    axis2_om_attribute_t *om_attr = NULL;
    
    axis2_object_ptr intern_ns = NULL;
    om_object_ptr om_obj_ns = NULL;
    zval *ns_obj = NULL;
    axis2_om_namespace_t *ns = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "OssO!",&object, axis2_om_attribute_class_entry ,&lname, &lname_len, &val, &val_len,
                    &ns_obj,axis2_om_namespace_class_entry) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
    intern = (axis2_object_ptr)zend_object_store_get_object(object TSRMLS_CC);
    om_obj = (om_object_ptr)emalloc(sizeof(om_object));
    om_obj->obj_type = OM_ATTRIBUTE;
    om_obj->ptr = NULL;
   
    if(ns_obj)
    {
        AXIS2_GET_OBJ(om_obj_ns, ns_obj, om_object_ptr, intern_ns);
        if(om_obj_ns && om_obj_ns->ptr)
            ns = (axis2_om_namespace_t *)(om_obj_ns->ptr);
    }
            
    om_attr = axis2_om_attribute_create(&env, lname, val, ns);
    om_obj->ptr = om_attr;
    intern->ptr = om_obj;        
}

PHP_FUNCTION(axis2_om_attribute_get_localname)
{
    axis2_object_ptr intern = NULL;
    zval *object = NULL;
    om_object_ptr om_obj = NULL;
    axis2_env_t *env = NULL;
    axis2_om_attribute_t *attr = NULL;
    char *lname = NULL;
    
    env = php_axis2_get_env();
    AXIS2_GET_THIS(object);
    
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);
    
    attr = (axis2_om_attribute_t *)(om_obj->ptr);
    
    if(attr)
    {
       lname = AXIS2_OM_ATTRIBUTE_GET_LOCALNAME(attr, &env);
       if(lname)
            RETURN_STRING(lname, 1);     
    }
    RETURN_NULL();
}

/* {{{ proto qname getValue */
PHP_FUNCTION(axis2_om_attribute_get_value)
{
    axis2_object_ptr intern = NULL;
    zval *object = NULL;
    om_object_ptr om_obj = NULL;
    axis2_env_t *env = NULL;
    axis2_om_attribute_t *attr = NULL;
    char *value = NULL;
    
    env = php_axis2_get_env();
    AXIS2_GET_THIS(object);
    
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);
    
    attr = (axis2_om_attribute_t *)(om_obj->ptr);
    
    if(attr)
    {
       value = AXIS2_OM_ATTRIBUTE_GET_VALUE(attr, &env);
       if(value)
            RETURN_STRING(value, 1);     
    }
    RETURN_NULL();
}
/* }}} end getValue */
PHP_FUNCTION(axis2_om_attribute_get_namespace)
{
    axis2_object_ptr intern = NULL;
    zval *object = NULL;
    om_object_ptr om_obj = NULL;
    axis2_env_t *env = NULL;
    axis2_om_attribute_t *attr = NULL;
    axis2_om_namespace_t *ns = NULL;
    zval *namespace_obj = NULL;  
    
    env = php_axis2_get_env();
    AXIS2_GET_THIS(object);
    
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);
    
    attr = (axis2_om_attribute_t *)(om_obj->ptr);
    
    if(attr)
    {
       ns = AXIS2_OM_ATTRIBUTE_GET_NAMESPACE(attr, &env);
       namespace_obj = php_axis2_create_om_object(ns, 
                            OM_NAMESPACE, axis2_om_namespace_class_entry TSRMLS_CC);
       RETURN_ZVAL(namespace_obj, NULL, NULL);                    
    }
    RETURN_NULL();
}


PHP_FUNCTION(axis2_om_attribute_get_qname)
{
    axis2_object_ptr intern = NULL;
    zval *object = NULL;
    om_object_ptr om_obj = NULL;
    axis2_env_t *env = NULL;
    axis2_om_attribute_t *attr = NULL;
    axis2_qname_t *qname = NULL;
    zval *qname_obj = NULL;  
    
    env = php_axis2_get_env();
    AXIS2_GET_THIS(object);
    
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);

    attr = (axis2_om_attribute_t *)(om_obj->ptr);
    if(attr)
    {
       qname = AXIS2_OM_ATTRIBUTE_GET_QNAME(attr, &env);
       qname_obj = php_axis2_create_om_object(qname, 
                            AXIS2_QNAME, axis2_qname_class_entry TSRMLS_CC);
       RETURN_ZVAL(qname_obj, NULL, NULL);                    
    }
    RETURN_NULL();
}


PHP_FUNCTION(axis2_om_attribute_set_localname)
{
    axis2_object_ptr intern = NULL;
    om_object_ptr om_obj = NULL;
    zval *object = NULL;
    axis2_env_t *env = NULL;
    char *lname = NULL;
    int lname_len = 0;
    axis2_om_attribute_t *om_attr = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "Os",&object, axis2_om_attribute_class_entry ,&lname, &lname_len) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
   
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);
    if(om_obj)
    {
       om_attr = (axis2_om_attribute_t*)(om_obj->ptr);
       AXIS2_OM_ATTRIBUTE_SET_LOCALNAME(om_attr, &env, lname);
    }
}
PHP_FUNCTION(axis2_om_attribute_set_value)
{
    axis2_object_ptr intern = NULL;
    om_object_ptr om_obj = NULL;
    zval *object = NULL;
    axis2_env_t *env = NULL;
    char *value = NULL;
    int value_len = 0;
    axis2_om_attribute_t *om_attr = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "Os",&object, axis2_om_attribute_class_entry ,&value, &value_len) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
   
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);
    if(om_obj)
    {
       om_attr = (axis2_om_attribute_t*)(om_obj->ptr);
       AXIS2_OM_ATTRIBUTE_SET_VALUE(om_attr, &env, value);
    }

}

PHP_FUNCTION(axis2_om_attribute_set_namespace)
{
    axis2_object_ptr intern = NULL;
    axis2_object_ptr ns_intern = NULL;
    zval *object = NULL;
    om_object_ptr om_obj = NULL;
    om_object_ptr om_ns_obj = NULL;
    axis2_env_t *env = NULL;
    axis2_om_attribute_t *attr = NULL;
    axis2_om_namespace_t *ns = NULL;
    zval *ns_obj = NULL;  
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "OO",&object, axis2_om_attribute_class_entry ,&ns_obj, axis2_om_namespace_class_entry) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    
    env = php_axis2_get_env();
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);
    AXIS2_GET_OBJ(om_ns_obj, ns_obj, om_object_ptr, ns_intern);
    
    attr = (axis2_om_attribute_t *)(om_obj->ptr);
    ns   = (axis2_om_namespace_t *)(om_ns_obj->ptr);
    
    if(attr && ns)
    {
        AXIS2_OM_ATTRIBUTE_SET_NAMESPACE(attr, &env, ns);   
    }
    RETURN_NULL();
}

PHP_FUNCTION(axis2_om_attribute_serialize)
{
}
