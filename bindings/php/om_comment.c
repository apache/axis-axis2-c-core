
#include "php.h"
#include "php_axis2.h"
#include "axis2_om_node.h"
#include "axis2_om_comment.h"

zend_function_entry php_axis2_om_comment_class_functions[]=
{
    PHP_FALIAS(serialize , axis2_om_comment_serialize, NULL)
    PHP_FALIAS(getValue , axis2_om_comment_get_value, NULL)
    PHP_FALIAS(setValue , axis2_om_comment_set_value, NULL)
    PHP_ME(om_comment, __construct, NULL, ZEND_ACC_PUBLIC)
    { NULL, NULL, NULL}
};

PHP_FUNCTION(axis2_om_comment_serialize){}
PHP_FUNCTION(axis2_om_comment_get_value){}
PHP_FUNCTION(axis2_om_comment_set_value){}
PHP_METHOD(om_comment, __construct){}