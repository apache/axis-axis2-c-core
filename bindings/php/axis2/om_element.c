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
#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <axis2_allocator.h>
#include <axis2_env.h>

zend_function_entry php_axis2_om_element_class_functions[]=
{
    PHP_FALIAS(findNamespace , axis2_om_element_find_namespace, NULL)
    PHP_FALIAS(declareNamespace , axis2_om_element_declare_namespace, NULL)
    PHP_FALIAS(findNamespaceWithQname , axis2_om_element_find_namespace_with_qname, NULL)
    PHP_FALIAS(addAttribute , axis2_om_element_add_attribute, NULL)
    PHP_FALIAS(getAttribute , axis2_om_element_get_attribute, NULL)
    PHP_FALIAS(findDeclaredNamespace , axis2_om_element_find_declared_namespace, NULL)
    PHP_FALIAS(getLocalname , axis2_om_element_get_localname, NULL)
    PHP_FALIAS(getNamespace , axis2_om_element_get_namespace, NULL)
    PHP_FALIAS(setNamespace , axis2_om_element_set_namespace, NULL)
    PHP_FALIAS(setLocalname , axis2_om_element_set_localname, NULL)
    PHP_ME(om_element, __construct, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL}
};

PHP_METHOD(om_element, __construct)
{
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_parent = NULL;
    axis2_object_ptr intern_ns = NULL;
    
    om_node_ptr node_obj = NULL;
    om_node_ptr node_obj_parent = NULL;
    om_object_ptr om_obj_ns = NULL;
    
    zval *object = NULL;
    zval *object_parent = NULL;
    zval *object_ns = NULL;
    
    axis2_env_t *env = NULL;
    char *lname = NULL;
    int lname_len = 0;
    char *n = NULL;

    axis2_om_element_t *om_element = NULL;
    axis2_om_node_t *node = NULL;
    axis2_om_node_t *node_parent = NULL;
    axis2_om_namespace_t *ns = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "OO!sO!",&object, axis2_om_element_class_entry, &object_parent,
                    axis2_om_node_class_entry, &lname, &lname_len,
                    &object_ns, axis2_om_namespace_class_entry) == FAILURE)
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
    if(object_ns)
    {
        AXIS2_GET_OBJ(om_obj_ns, object_ns, om_object_ptr, intern_ns);
        if(om_obj_ns && om_obj_ns->ptr)
            ns = (axis2_om_namespace_t *)(om_obj_ns->ptr);
    }
    
    om_element = axis2_om_element_create(&env, node_parent, lname, ns, &node);
    node_obj->ptr = node;
    intern->obj_type = OM_NODE_OBJ;
    intern->ptr = node_obj;
}


PHP_FUNCTION(axis2_om_element_get_localname)
{
    axis2_object_ptr intern = NULL;
    om_node_ptr node_obj = NULL;
    
    zval *object = NULL;
    
    axis2_env_t *env = NULL;
    char *localname = NULL;
    axis2_om_element_t *om_element = NULL;
    axis2_om_node_t *node = NULL;

    AXIS2_GET_THIS(object);
    
    env = php_axis2_get_env();
    
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
    node = (axis2_om_node_t*)(node_obj->ptr);
    om_element = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(node , &env);
   
    localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_element, &env);
    if(localname)
    {
        RETURN_STRING(localname, 1);
    }
    RETURN_NULL();
}


PHP_FUNCTION(axis2_om_element_get_namespace)
{
    axis2_object_ptr intern = NULL;
    om_node_ptr node_obj = NULL;
    zval *object = NULL;
    zval *object_ns = NULL;
    
    axis2_env_t *env = NULL;
    axis2_om_element_t *om_element = NULL;
    axis2_om_node_t *node = NULL;
    axis2_om_namespace_t *ns = NULL;
   
    AXIS2_GET_THIS(object);
    env = php_axis2_get_env();
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
    
    node = (axis2_om_node_t*)(node_obj->ptr);
    om_element = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(node , &env);
   
    ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(om_element, &env);
    if(ns)
    {
        object_ns = php_axis2_create_om_object(ns, OM_OBJ, axis2_om_namespace_class_entry TSRMLS_CC);         
        RETURN_ZVAL(object_ns, NULL, NULL);
    }
    RETURN_NULL();
}

PHP_FUNCTION(axis2_om_element_get_attribute)
{
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_qname = NULL;
    
    om_node_ptr node_obj = NULL;
    om_object_ptr qname_obj = NULL;
    
    zval *object = NULL;
    zval *object_attr = NULL;
    zval *object_qname = NULL;
    
    axis2_env_t *env = NULL;
    axis2_om_element_t *om_element = NULL;
    axis2_om_node_t *node = NULL;
    axis2_om_attribute_t *attr = NULL;
    axis2_qname_t *qname = NULL;
   
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "OO",&object, axis2_om_element_class_entry, &object_qname,
                    axis2_qname_class_entry) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
    
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
    AXIS2_GET_OBJ(qname_obj, object_qname, om_object_ptr, intern_qname);
    
    node = (axis2_om_node_t*)(node_obj->ptr);
    om_element = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(node , &env);
    qname = (axis2_qname_t *)(qname_obj->ptr);
    
    attr = AXIS2_OM_ELEMENT_GET_ATTRIBUTE(om_element, &env, qname);
    if(attr)
    {
        object_attr = php_axis2_create_om_object(attr, OM_OBJ, axis2_om_attribute_class_entry TSRMLS_CC);         
        RETURN_ZVAL(object_attr, NULL, NULL);
    }
    RETURN_NULL();
}


PHP_FUNCTION(axis2_om_element_add_attribute)
{
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_attr = NULL;
    
    om_node_ptr node_obj = NULL;
    om_object_ptr attr_obj = NULL;
    
    zval *object = NULL;
    zval *object_attr = NULL;
    
    axis2_env_t *env = NULL;
    axis2_om_element_t *om_element = NULL;
    axis2_om_node_t *node = NULL;
    axis2_om_attribute_t *attr = NULL;
   
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "OO",&object, axis2_om_element_class_entry, &object_attr,
                    axis2_om_attribute_class_entry) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
    
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
    AXIS2_GET_OBJ(attr_obj, object_attr, om_object_ptr, intern_attr);
    
    node = (axis2_om_node_t*)(node_obj->ptr);
    om_element = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(node , &env);
    attr = (axis2_om_attribute_t *)(attr_obj->ptr);
    if(attr)
    {
        AXIS2_OM_ELEMENT_ADD_ATTRIBUTE(om_element, &env, attr);
    }
}


PHP_FUNCTION(axis2_om_element_set_namespace)
{
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_ns = NULL;
    
    om_node_ptr node_obj = NULL;
    om_object_ptr om_obj = NULL;
    
    zval *object = NULL;
    zval *object_ns = NULL;
    
    axis2_env_t *env = NULL;
    axis2_om_element_t *om_element = NULL;
    axis2_om_node_t *node = NULL;
    axis2_om_namespace_t *ns = NULL;
   
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "OO",&object, axis2_om_element_class_entry, &object_ns,
                    axis2_om_namespace_class_entry) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
    
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
    AXIS2_GET_OBJ(om_obj, object_ns , om_object_ptr, intern_ns);
    
    node = (axis2_om_node_t*)(node_obj->ptr);
    om_element = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(node , &env);
    ns  = (axis2_om_namespace_t *)(om_obj->ptr);
    if(ns)
    {
        AXIS2_OM_ELEMENT_SET_NAMESPACE(om_element, &env, ns, node);
    }
}


PHP_FUNCTION(axis2_om_element_set_localname)
{
    axis2_object_ptr intern = NULL;
    om_node_ptr node_obj = NULL;
    zval *object = NULL;

    char *localname = NULL;
    int lnamelen = 0;
    
    axis2_env_t *env = NULL;
    axis2_om_element_t *om_element = NULL;
    axis2_om_node_t *node = NULL;
   
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "Os",&object, axis2_om_element_class_entry, &localname,
                    &lnamelen) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    env = php_axis2_get_env();
    
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
    
    node = (axis2_om_node_t*)(node_obj->ptr);
    om_element = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(node , &env);
    if(om_element)
    {
        AXIS2_OM_ELEMENT_SET_LOCALNAME(om_element, &env, localname);
    }
}

PHP_FUNCTION(axis2_om_element_find_namespace){}
PHP_FUNCTION(axis2_om_element_declare_namespace){}
PHP_FUNCTION(axis2_om_element_find_namespace_with_qname){}
PHP_FUNCTION(axis2_om_element_find_declared_namespace){}