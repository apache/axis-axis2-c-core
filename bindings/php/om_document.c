
#include "php.h"
#include "php_axis2.h"
#include "axis2_om_document.h"

zend_function_entry php_axis2_om_document_class_functions[] = 
{
    PHP_FALIAS(buildNext , axis2_om_document_build_next, NULL)
    PHP_FALIAS(addChild , axis2_om_document_add_child, NULL)
    PHP_FALIAS(getRootElement , axis2_om_document_get_root_element, NULL)
    PHP_FALIAS(setRootElement , axis2_om_document_set_root_element, NULL)
    PHP_FALIAS(buildAll , axis2_om_document_build_all, NULL)
    PHP_ME(om_document, __construct, NULL, ZEND_ACC_PUBLIC)
    { NULL, NULL, NULL}    
};


PHP_METHOD(om_document, __construct)
{
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_root = NULL;
    axis2_object_ptr intern_builder = NULL;
   
    
    om_object_ptr om_obj = NULL;
    om_node_ptr node_obj_root = NULL;
    om_object_ptr om_obj_builder = NULL;
    
    
    zval *object = NULL;
    zval *object_root = NULL;
    zval *object_builder = NULL;
    
    
    axis2_om_stax_builder_t *builder = NULL;
    axis2_om_node_t *root = NULL;
    axis2_om_document_t *document = NULL;
    axis2_env_t *env = NULL;
    
    if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC,
             getThis(), "OO!O!",&object, axis2_om_document_class_entry, &object_root,
             axis2_om_node_class_entry, &object_builder, 
             axis2_om_stax_builder_class_entry) == FAILURE)
    {
        php_std_error_handling();
        return;
    }
    
    env = php_axis2_get_env();
    
    AXIS2_GET_OBJ(om_obj_builder, object_builder, om_object_ptr, intern_builder);
    
    AXIS2_GET_OBJ(node_obj_root, object_root, om_node_ptr, intern_root);
    
    intern = (axis2_object_ptr)zend_object_store_get_object(object TSRMLS_CC);
    om_obj = (om_object_ptr)emalloc(sizeof(om_object));
    om_obj->obj_type = OM_DOCUMENT;
    om_obj->ptr = NULL;
    
    builder = (axis2_om_stax_builder_t *)(om_obj_builder->ptr);
    root    = (axis2_om_node_t *)(node_obj_root->ptr);
    
    document = axis2_om_document_create(&env, root, builder);
    om_obj->ptr = document;
    intern->ptr = om_obj; 
}

PHP_FUNCTION(axis2_om_document_build_next)
{
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_node = NULL;
    
    
    om_object_ptr om_obj = NULL;
    om_node_ptr node_obj = NULL;
        
    zval *object = NULL;
    zval *object_node = NULL;
    
    
    axis2_om_node_t *node = NULL;
    axis2_om_document_t *document = NULL;
    axis2_env_t *env = NULL; 
    int node_type = 0;   
    zend_class_entry *ce_node = NULL;

    AXIS2_GET_THIS(object);
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);
    env = php_axis2_get_env();
    
    document = (axis2_om_document_t *)(om_obj->ptr);
    
    
    
    if(document)
    {
        node = AXIS2_OM_DOCUMENT_BUILD_NEXT(document, &env);  
        if(node)
        {
            node_type = AXIS2_OM_NODE_GET_NODE_TYPE(node, &env);
            ce_node = php_axis2_get_class_type(node_type);
            object_node = php_axis2_create_om_node_object(node, node_type, ce_node TSRMLS_CC);
            RETURN_ZVAL(object_node, NULL, NULL);        
        }    
    }    
    RETURN_NULL();    
}
PHP_FUNCTION(axis2_om_document_add_child)
{
     php_printf("not implemented yet");
}
PHP_FUNCTION(axis2_om_document_get_root_element)
{
    axis2_object_ptr intern = NULL;
    axis2_object_ptr intern_node = NULL;
    
    
    om_object_ptr om_obj = NULL;
    om_node_ptr node_obj = NULL;
        
    zval *object = NULL;
    zval *object_node = NULL;
    
    
    axis2_om_node_t *node = NULL;
    axis2_om_document_t *document = NULL;
    axis2_env_t *env = NULL; 
    int node_type = 0;   
    zend_class_entry *ce_node = NULL;

    AXIS2_GET_THIS(object);
    AXIS2_GET_OBJ(om_obj, object, om_object_ptr, intern);
    env = php_axis2_get_env();
    
    document = (axis2_om_document_t *)(om_obj->ptr);
    
    
    
    if(document)
    {
        node = AXIS2_OM_DOCUMENT_GET_ROOT_ELEMENT(document, &env);  
        if(node)
        {
            node_type = AXIS2_OM_NODE_GET_NODE_TYPE(node, &env);
            ce_node = php_axis2_get_class_type(node_type);
            object_node = php_axis2_create_om_node_object(node, node_type, ce_node TSRMLS_CC);
            RETURN_ZVAL(object_node, NULL, NULL);        
        }    
    }    
    RETURN_NULL();    
}

PHP_FUNCTION(axis2_om_document_set_root_element)
{
    php_printf("not implemented yet");
}
PHP_FUNCTION(axis2_om_document_build_all)
{
 php_printf("not implemented yet");
}
