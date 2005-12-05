#include "php.h"
#include "php_axis2.h"
#include "axis2_om_node.h"
#include "axis2_om_element.h"

zend_function_entry php_axis2_om_node_class_functions[] =
{
    PHP_FALIAS(addChild , axis2_om_node_add_child, NULL)
    PHP_FALIAS(detach , axis2_om_node_detach, NULL)
    PHP_FALIAS(insertSiblingAfter , axis2_om_node_insert_sibling_after, NULL)
    PHP_FALIAS(insertSiblingBefore , axis2_om_node_insert_sibling_before, NULL)
    PHP_FALIAS(setParent , axis2_om_node_set_parent, NULL)
    PHP_FALIAS(serialize , axis2_om_node_serialize, NULL)
    PHP_FALIAS(getParent , axis2_om_node_get_parent, NULL)
    PHP_FALIAS(getFirstChild , axis2_om_node_get_first_child, NULL)
    PHP_FALIAS(getLastChild , axis2_om_node_get_last_child, NULL)
    PHP_FALIAS(getPreviousSibling , axis2_om_node_get_previous_sibling, NULL)
    PHP_FALIAS(getNextSibling , axis2_om_node_get_next_sibling, NULL)
    PHP_FALIAS(getNodeType , axis2_om_node_get_node_type, NULL)
    PHP_FALIAS(isComplete , axis2_om_node_get_build_status, NULL)
    PHP_ME(om_node, __construct, NULL, ZEND_ACC_PRIVATE)
    { NULL, NULL, NULL}
};
/*
static zend_class_entry* php_axis2_get_class_type(int node_type)
{
    zend_class_entry *ce = NULL;
    switch(node_type)
    {
        case OM_ELEMENT:
            ce = axis2_om_element_class_entry;
            break;
        case OM_TEXT: 
            ce = axis2_om_text_class_entry;
            break;
        case OM_PI:
            ce = axis2_om_pi_class_entry;
            break;
        case OM_COMMENT:
            ce = axis2_om_comment_class_entry;            
            break;
        default:
            ce = NULL;
            break;    
    }
    return ce;
}
*/
PHP_METHOD(om_node, __construct)
{

}

/* {{{ proto void Axis2OMNode::addChild */
PHP_FUNCTION(axis2_om_node_add_child)
{
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_child = NULL;
    
    zval *object = NULL;
    zval *obj_child = NULL;
    
    om_node_ptr node_obj = NULL;
    om_node_ptr node_obj_child = NULL;
    
    axis2_om_node_t *om_node = NULL;
    axis2_om_node_t *child   = NULL;
    axis2_env_t *env = NULL;
     
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "OO",&object, axis2_om_node_class_entry ,&obj_child,axis2_om_node_class_entry) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    
    env = php_axis2_get_env();
 
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
       
    AXIS2_GET_OBJ(node_obj_child, obj_child, om_node_ptr, intern_child);
    
    om_node = (axis2_om_node_t *)(node_obj->ptr);
    child = (axis2_om_node_t *)(node_obj_child->ptr);
    /* this should be fixed */
    AXIS2_OM_NODE_ADD_CHILD(child, &env, om_node);    
    
}
/* }}} */

PHP_FUNCTION(axis2_om_node_detach)
{
    axis2_object_ptr intern = NULL;
    zval *object = NULL;
    om_node_ptr node_obj = NULL;
    axis2_om_node_t *om_node = NULL;
    axis2_env_t *env = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "OO",&object, axis2_om_node_class_entry ) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    
    env = php_axis2_get_env();
 
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
    om_node = (axis2_om_node_t *)(node_obj->ptr);
    AXIS2_OM_NODE_DETACH(om_node, &env);    
}

PHP_FUNCTION(axis2_om_node_set_parent)
{
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_parent = NULL;
    
    zval *object = NULL;
    zval *obj_parent = NULL;
    
    om_node_ptr node_obj = NULL;
    om_node_ptr node_obj_parent = NULL;
    
    axis2_om_node_t *om_node = NULL;
    axis2_om_node_t *parent   = NULL;
    axis2_env_t *env = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "OO",&object, axis2_om_node_class_entry ,&obj_parent,axis2_om_node_class_entry) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    
    env = php_axis2_get_env();
 
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
       
    AXIS2_GET_OBJ(node_obj_parent, obj_parent, om_node_ptr, intern_parent);
    
    om_node = (axis2_om_node_t *)(node_obj->ptr);
    parent = (axis2_om_node_t *)(node_obj_parent->ptr);
    AXIS2_OM_NODE_SET_PARENT(om_node, &env, parent);    
}

PHP_FUNCTION(axis2_om_node_get_node_type)
{
    axis2_object_ptr intern = NULL;
    zval *object = NULL;
    om_node_ptr node_obj = NULL;
    axis2_env_t *env = NULL;
    axis2_om_node_t *om_node = NULL;
    int node_type = 0;
        
    env = php_axis2_get_env();
    AXIS2_GET_THIS(object);
    
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
    
    om_node = (axis2_om_node_t *)(node_obj->ptr);
    node_type = AXIS2_OM_NODE_GET_NODE_TYPE(om_node, &env);
    RETURN_LONG(node_type);
}

PHP_FUNCTION(axis2_om_node_get_build_status)
{
    axis2_object_ptr intern = NULL;
    zval *object = NULL;
    om_node_ptr node_obj = NULL;
    axis2_env_t *env = NULL;
    axis2_om_node_t *om_node = NULL;
    axis2_bool_t status = AXIS2_FALSE;
        
    env = php_axis2_get_env();
    AXIS2_GET_THIS(object);
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
    
    om_node = (axis2_om_node_t *)(node_obj->ptr);
    status = AXIS2_OM_NODE_GET_BUILD_STATUS(om_node, &env);
    RETURN_BOOL(status);
}

PHP_FUNCTION(axis2_om_node_get_parent)
{
    zend_class_entry *parent_ce = NULL;
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_parent = NULL;
    
    zval *object = NULL;
    zval *obj_parent = NULL;
    
    om_node_ptr node_obj = NULL;
    
    axis2_om_node_t *om_node = NULL;
    axis2_om_node_t *parent   = NULL;
    axis2_env_t *env = NULL;
    int parent_node_type = 0;
    
    env = php_axis2_get_env();
    
    AXIS2_GET_THIS(object);
    
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
       
    om_node = (axis2_om_node_t *)(node_obj->ptr);
    parent = AXIS2_OM_NODE_GET_PARENT(om_node, &env);
    if(parent)
    {
        parent_node_type = AXIS2_OM_NODE_GET_NODE_TYPE(parent, &env);
        parent_ce = php_axis2_get_class_type(parent_node_type);
        obj_parent = php_axis2_create_om_node_object(parent, 
                        parent_node_type, parent_ce TSRMLS_CC);
        RETURN_ZVAL(obj_parent, NULL, NULL);    
    }
    RETURN_NULL();
}
PHP_FUNCTION(axis2_om_node_get_first_child)
{
    zend_class_entry *child_ce = NULL;
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_child = NULL;
    
    zval *object = NULL;
    zval *obj_child = NULL;
    
    om_node_ptr node_obj = NULL;
    
    axis2_om_node_t *om_node = NULL;
    axis2_om_node_t *child   = NULL;
    axis2_env_t *env = NULL;
    int child_node_type = 0;
    axis2_om_element_t *e = NULL;
    
    env = php_axis2_get_env();
    
    AXIS2_GET_THIS(object);
    
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
       
    om_node = (axis2_om_node_t *)(node_obj->ptr);
    
    child = AXIS2_OM_NODE_GET_FIRST_CHILD(om_node, &env);
    if(child)
    {
        child_node_type = AXIS2_OM_NODE_GET_NODE_TYPE(child, &env);
        child_ce = php_axis2_get_class_type(child_node_type);
        obj_child = php_axis2_create_om_node_object(child, 
                        child_node_type, child_ce TSRMLS_CC);
        RETURN_ZVAL(obj_child, NULL, NULL);   
        
    }
    RETURN_NULL();
    
}

PHP_FUNCTION(axis2_om_node_get_last_child)
{
    zend_class_entry *child_ce = NULL;
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_child = NULL;
    
    zval *object = NULL;
    zval *obj_child = NULL;
    
    om_node_ptr node_obj = NULL;
    
    axis2_om_node_t *om_node = NULL;
    axis2_om_node_t *child   = NULL;
    axis2_env_t *env = NULL;
    int child_node_type = 0;
    
    env = php_axis2_get_env();
    
    AXIS2_GET_THIS(object);
    
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
       
    om_node = (axis2_om_node_t *)(node_obj->ptr);
    child = AXIS2_OM_NODE_GET_LAST_CHILD(om_node, &env);
    if(child)
    {
        child_node_type = AXIS2_OM_NODE_GET_NODE_TYPE(child, &env);
        child_ce = php_axis2_get_class_type(child_node_type);
        obj_child = php_axis2_create_om_node_object(child, 
                        child_node_type, child_ce TSRMLS_CC);
        RETURN_ZVAL(obj_child, NULL, NULL);    
    }
    RETURN_NULL();
}

PHP_FUNCTION(axis2_om_node_get_previous_sibling)
{
    zend_class_entry *sibling_ce = NULL;
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_sibling = NULL;
    
    zval *object = NULL;
    zval *obj_sibling = NULL;
    
    om_node_ptr node_obj = NULL;
    
    axis2_om_node_t *om_node = NULL;
    axis2_om_node_t *sibling   = NULL;
    axis2_env_t *env = NULL;
    int sibling_node_type = 0;
    
    env = php_axis2_get_env();
    
    AXIS2_GET_THIS(object);
    
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
       
    om_node = (axis2_om_node_t *)(node_obj->ptr);
    sibling = AXIS2_OM_NODE_GET_PREVIOUS_SIBLING(om_node, &env);
    if(sibling)
    {
        sibling_node_type = AXIS2_OM_NODE_GET_NODE_TYPE(sibling, &env);
        sibling_ce = php_axis2_get_class_type(sibling_node_type);
        obj_sibling = php_axis2_create_om_node_object(sibling, 
                        sibling_node_type, sibling_ce TSRMLS_CC);
        RETURN_ZVAL(obj_sibling, NULL, NULL);    
    }
    RETURN_NULL();
}

PHP_FUNCTION(axis2_om_node_get_next_sibling)
{
    zend_class_entry *sibling_ce = NULL;
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_sibling = NULL;
    
    zval *object = NULL;
    zval *obj_sibling = NULL;
    
    om_node_ptr node_obj = NULL;
    
    axis2_om_node_t *om_node = NULL;
    axis2_om_node_t *sibling   = NULL;
    axis2_env_t *env = NULL;
    int sibling_node_type = 0;
    
    env = php_axis2_get_env();
    
    AXIS2_GET_THIS(object);
    
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
       
    om_node = (axis2_om_node_t *)(node_obj->ptr);
    sibling = AXIS2_OM_NODE_GET_NEXT_SIBLING(om_node, &env);
    if(sibling)
    {
        sibling_node_type = AXIS2_OM_NODE_GET_NODE_TYPE(sibling, &env);
        sibling_ce = php_axis2_get_class_type(sibling_node_type);
        obj_sibling = php_axis2_create_om_node_object(sibling, 
                        sibling_node_type, sibling_ce TSRMLS_CC);
        RETURN_ZVAL(obj_sibling, NULL, NULL);    
    }
    RETURN_NULL();
}
PHP_FUNCTION(axis2_om_node_serialize)
{
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_output = NULL;
    
    zval *object = NULL;
    zval *object_output = NULL;
    
    om_node_ptr node_obj = NULL;
    om_object_ptr om_obj_output = NULL;
    
    axis2_om_output_t *output = NULL;
    axis2_om_node_t *node = NULL;
    axis2_env_t *env = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "OO",&object, axis2_om_node_class_entry,
             &object_output , axis2_om_output_class_entry ) == FAILURE)
    {
        php_std_error_handling();
        return;
    }

    env = php_axis2_get_env();
    
    AXIS2_GET_OBJ(node_obj, object, om_node_ptr, intern);
    AXIS2_GET_OBJ(om_obj_output, object_output, om_object_ptr, intern_output);
    
    node = (axis2_om_node_t *)(node_obj->ptr);
    output = (axis2_om_output_t *)(om_obj_output->ptr);
    
    AXIS2_OM_NODE_SERIALIZE(node, &env, output);
}
PHP_FUNCTION(axis2_om_node_insert_sibling_after)
{
    php_printf("not implemented yet");
}
PHP_FUNCTION(axis2_om_node_insert_sibling_before)
{
    php_printf("not implemented yet");
}



