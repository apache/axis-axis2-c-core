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
#include "axis2_qname.h"

zend_function_entry php_axis2_qname_class_functions[]=
{
    PHP_FALIAS(equals , axis2_qname_equals, NULL )
    PHP_FALIAS(clone , axis2_qname_clone, NULL )
    PHP_FALIAS(getUri , axis2_qname_get_uri, NULL )
    PHP_FALIAS(getPrefix , axis2_qname_get_prefix, NULL )
    PHP_FALIAS(getLocalpart , axis2_qname_get_localpart, NULL )
    PHP_ME(axis2_qname,__construct, NULL, ZEND_ACC_PUBLIC)
    {NULL , NULL, NULL}
};
/* {{{ proto void Axis2Qname::__construct 
        (string localpart,[string namespaceuri], string prefix) 
*/
PHP_METHOD(axis2_qname, __construct)
{
    axis2_object_ptr intern = NULL;
    zval *object = NULL;
    om_object_ptr om_obj = NULL;
    axis2_env_t *env = NULL;
    char *lpart = NULL;
    int lpart_len = 0;
    char *ns_uri = NULL;
    int urilen = 0;
    char *prefix = NULL;
    int prefixlen = 0;
    axis2_qname_t *qname = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
            getThis(), "Oss!s",&object, axis2_qname_class_entry ,&lpart, &lpart_len, &ns_uri, &urilen,
                    &prefix, &prefixlen) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    
    env = php_axis2_get_env();
    intern = (axis2_object_ptr)zend_object_store_get_object(object TSRMLS_CC);
    om_obj = (om_object_ptr)emalloc(sizeof(om_object));
    om_obj->obj_type = AXIS2_QNAME;
    
    qname = axis2_qname_create(&env, lpart, ns_uri, prefix);
    om_obj->ptr = qname;
    intern->ptr = om_obj;
    intern->obj_type = OM_OBJ;     
}
/* }}} end Axis2Qname::_construct */

/* {{{ proto string getUri */
PHP_FUNCTION(axis2_qname_get_uri)
{
    axis2_object_ptr intern = NULL;
    zval *object = NULL;
    om_object_ptr om_obj = NULL;
    axis2_env_t *env = NULL;
    axis2_qname_t *qname = NULL;
    char *uri;
    
    env = php_axis2_get_env();
    AXIS2_GET_THIS(object);
    
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);
    
    qname = (axis2_qname_t *)(om_obj->ptr);
    
    if(qname)
    {
       uri = AXIS2_QNAME_GET_URI(qname, &env);
       if(uri)
            RETURN_STRING(uri, 1);     
    }
    RETURN_NULL();
}

/* }}} end getUri */

/* {{{ proto string getPrefix */
PHP_FUNCTION(axis2_qname_get_prefix)
{
     axis2_object_ptr intern = NULL;
    zval *object = NULL;
    om_object_ptr om_obj = NULL;
    axis2_env_t *env = NULL;
    axis2_qname_t *qname = NULL;
    char *prefix;
    
    env = php_axis2_get_env();
    AXIS2_GET_THIS(object);
    
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);
    
    qname = (axis2_qname_t *)(om_obj->ptr);
    
    if(qname)
    {
       prefix = AXIS2_QNAME_GET_PREFIX(qname, &env);
       if(prefix)
            RETURN_STRING(prefix, 1);     
    }
    RETURN_NULL();  
}
/* }}} end getPrefix */

/* {{{ proto string getLocalpart */
PHP_FUNCTION(axis2_qname_get_localpart)
{
    axis2_object_ptr intern = NULL;
    zval *object = NULL;
    om_object_ptr om_obj = NULL;
    axis2_env_t *env = NULL;
    axis2_qname_t *qname = NULL;
    char *localname;
    
    env = php_axis2_get_env();
    AXIS2_GET_THIS(object);
    
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);
    
    qname = (axis2_qname_t *)(om_obj->ptr);
    
    if(qname)
    {
       localname = AXIS2_QNAME_GET_PREFIX(qname, &env);
       RETURN_STRING(localname, 1);     
    }
    RETURN_NULL();
}

/* }}} end getLocalpart */

/* {{{ proto void equals */
PHP_FUNCTION(axis2_qname_equals)
{
   axis2_object_ptr intern= NULL;
    axis2_object_ptr intern_cmp = NULL;
    zval *object = NULL;
    zval *object_cmp = NULL;
    om_object_ptr om_qname = NULL;
    om_object_ptr om_qname_cmp = NULL;
    axis2_env_t *env = NULL;
    axis2_qname_t *qname = NULL;
    axis2_qname_t *qname_cmp = NULL;
    axis2_bool_t equals = AXIS2_FALSE;
    
    env = php_axis2_get_env();
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "OO",&object, axis2_qname_class_entry ,
                  &object_cmp, axis2_qname_class_entry) == FAILURE)
    {
        php_std_error_handling();
        return;
    } 
    
    AXIS2_GET_OBJ(om_qname, object, om_object_ptr, intern);
    AXIS2_GET_OBJ(om_qname_cmp, object_cmp, om_object_ptr, intern_cmp);
    
    qname = (axis2_qname_t *)(om_qname->ptr);
    qname_cmp = (axis2_qname_t *)(om_qname_cmp->ptr);
    
    equals = AXIS2_QNAME_EQUALS(qname, &env, qname_cmp);
    
    if(equals == AXIS2_TRUE)
    {
        RETURN_BOOL(1);
    }
    if(equals == AXIS2_FALSE) 
    {
        RETURN_BOOL(0);
    }

}
/* }}} end equals */

PHP_FUNCTION(axis2_qname_clone)
{
    php_printf("not implemented");
}