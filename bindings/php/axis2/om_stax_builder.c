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
#include "axis2_om_stax_builder.h"

zend_function_entry php_axis2_om_stax_builder_class_functions[] =
{
    PHP_FALIAS(next , axis2_om_stax_builder_next, NULL)
    PHP_FALIAS(discard , axis2_om_stax_builder_discard_current_element, NULL)
    PHP_ME(om_stax_builder, __construct, NULL, ZEND_ACC_PUBLIC)
    { NULL, NULL, NULL}
};

PHP_METHOD(om_stax_builder, __construct)
{
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_reader = NULL;
    
    om_object_ptr om_obj = NULL;
    om_object_ptr om_obj_reader = NULL;
    
    zval *object = NULL;
    zval *object_reader = NULL;
    
    axis2_om_stax_builder_t *builder = NULL;
    axis2_xml_reader_t *reader = NULL;
    axis2_env_t *env = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "OO",&object, axis2_om_stax_builder_class_entry, &object_reader,
                    axis2_xml_reader_class_entry) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    
    env = php_axis2_get_env();
    AXIS2_GET_OBJ(om_obj_reader, object_reader, om_object_ptr, intern_reader);
    
    intern = (axis2_object_ptr)zend_object_store_get_object(object TSRMLS_CC);
    
    om_obj = (om_object_ptr)emalloc(sizeof(om_object));
    om_obj->obj_type = OM_STAX_BUILDER;
    om_obj->ptr = NULL;
    reader  = (axis2_xml_reader_t *)(om_obj_reader->ptr);
    builder = axis2_om_stax_builder_create(&env, reader);
    om_obj->ptr = builder;
    intern->ptr = om_obj; 
}

PHP_FUNCTION(axis2_om_stax_builder_next)
{
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_node = NULL;
    
    om_object_ptr om_obj = NULL;
    om_node_ptr node_obj = NULL;
    
    zval *object = NULL;
    zval *object_node = NULL;
    
    axis2_om_stax_builder_t *builder = NULL;
    axis2_om_node_t *node = NULL;
    axis2_env_t *env = NULL; 
    int node_type = 0;   
    zend_class_entry *ce_node = NULL;


    AXIS2_GET_THIS(object);
        
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);

    env = php_axis2_get_env();

    builder = (axis2_om_stax_builder_t *)(om_obj->ptr);
    
    
    if(builder)
    {
        node = AXIS2_OM_STAX_BUILDER_NEXT(builder, &env);
        /*
        if(node)
        {
            node_type = AXIS2_OM_NODE_GET_NODE_TYPE(node, &env);
            ce_node = php_axis2_get_class_type(node_type);
            object_node = php_axis2_create_om_node_object(node, node_type, ce_node TSRMLS_CC);
            RETURN_ZVAL(object_node, NULL, NULL);        
        }
                */     

    }
    
      
    RETURN_NULL();    
}

PHP_FUNCTION(axis2_om_stax_builder_discard_current_element)
{
     php_printf("not implemented yet");
}
