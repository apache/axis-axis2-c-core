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
#include "axis2_om_namespace.h"

zend_function_entry php_axis2_om_namespace_class_functions[]=
{
    PHP_FALIAS(equals , axis2_om_namespace_equals, NULL)
    PHP_FALIAS(serialize , axis2_om_namespace_serialize, NULL)
    PHP_FALIAS(getUri , axis2_om_namespace_get_uri, NULL)
    PHP_FALIAS(getPrefix , axis2_om_namespace_get_prefix, NULL)
    PHP_ME(om_namespace, __construct, NULL, ZEND_ACC_PUBLIC)
    {NULL, NULL, NULL} 
};

/* {{{ om_namespace __construct [ string, string ] */
PHP_METHOD(om_namespace, __construct)
{
    axis2_object_ptr intern = NULL;
    zval *object = NULL;
    om_object_ptr om_obj = NULL;
    axis2_env_t *env = NULL;
    char *ns_uri = NULL;
    char *prefix = NULL;
    int urilen = 0;
    int prefixlen = 0;
    axis2_om_namespace_t *om_namespace = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "Oss",&object, axis2_om_namespace_class_entry ,&ns_uri, &urilen, &prefix, &prefixlen) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
  
    env = php_axis2_get_env();
    intern = (axis2_object_ptr)zend_object_store_get_object(object TSRMLS_CC);
    om_obj = (om_object_ptr)emalloc(sizeof(om_object));
    om_obj->obj_type = OM_NAMESPACE;
    om_obj->ptr = NULL;
    
    om_namespace = axis2_om_namespace_create (&env, ns_uri, prefix);
    if(!om_namespace)
    {
        /*php_error_docref(NULL TSRMLS_CC, E_OUTOFMEMORY, " namespace create failure ");*/
    }
    
    om_obj->ptr = om_namespace;
    
    intern->obj_type = OM_OBJ;
    intern->ptr = om_obj;
}

/* }}} end om_namespace __construct */

/* {{{ om_namespace equals */
PHP_FUNCTION(axis2_om_namespace_equals)
{
    axis2_object_ptr intern= NULL;
    axis2_object_ptr intern1 = NULL;
    zval *object = NULL;
    zval *ns_obj = NULL;
    om_object_ptr this_ns = NULL;
    om_object_ptr ns_to_cmp = NULL;
    axis2_env_t *env = NULL;
    axis2_om_namespace_t *ns1 = NULL;
    axis2_om_namespace_t *ns2 = NULL;
    axis2_bool_t equals = AXIS2_FALSE;
    
    env = php_axis2_get_env();
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "OO",&object, axis2_om_namespace_class_entry , &ns_obj, axis2_om_namespace_class_entry) == FAILURE)
    {
        php_std_error_handling();
        return;
    } 
    
    AXIS2_GET_OBJ(this_ns, object, om_object_ptr, intern);
    AXIS2_GET_OBJ(ns_to_cmp, ns_obj, om_object_ptr, intern1);
    
    ns1 = (axis2_om_namespace_t *)(this_ns->ptr);
    ns2 = (axis2_om_namespace_t *)(ns_to_cmp->ptr);
    
    equals = AXIS2_OM_NAMESPACE_EQUALS(ns1, &env, ns2);
    
    if(equals == AXIS2_TRUE)
    {
        RETURN_BOOL(1);
    }
    if(equals == AXIS2_FALSE) 
    {
        RETURN_BOOL(0);
    }
}
/* }}} end om_namespace equals */



PHP_FUNCTION(axis2_om_namespace_get_uri)
{
    axis2_object_ptr intern = NULL;
    zval *object = NULL;
    om_object_ptr om_obj = NULL;
    axis2_env_t *env = NULL;
    axis2_om_namespace_t *ns = NULL;
    char *uri;
    
    env = php_axis2_get_env();
    AXIS2_GET_THIS(object);
    
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);
    
    ns = (axis2_om_namespace_t *)(om_obj->ptr);
    
    if(ns)
    {
       uri = AXIS2_OM_NAMESPACE_GET_URI(ns, &env);
       if(uri)
            RETURN_STRING(uri, 1);     
    }
    RETURN_NULL();
}


PHP_FUNCTION(axis2_om_namespace_get_prefix)
{
    axis2_object_ptr intern = NULL;
    zval *object = NULL;
    om_object_ptr om_obj = NULL;
    axis2_env_t *env = NULL;
    axis2_om_namespace_t *ns = NULL;
    char *prefix = NULL;
    
    env = php_axis2_get_env();
    AXIS2_GET_THIS(object);
    
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);
    
    ns = (axis2_om_namespace_t *)(om_obj->ptr);
    
    if(ns)
    {
       prefix = AXIS2_OM_NAMESPACE_GET_PREFIX(ns, &env);
       if(prefix)
            RETURN_STRING(prefix, 1);     
    }
    RETURN_NULL();
}

PHP_FUNCTION(axis2_om_namespace_serialize){}
