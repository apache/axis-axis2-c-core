#include "php.h"
#include "php_axis2.h"
#include "axis2_om_node.h"
#include "axis2_om_processing_instruction.h"

zend_function_entry axis2_om_pi_class_functions[] =
{
    PHP_FALIAS( , axis2_om_pi_set_value, NULL)
    PHP_FALIAS( , axis2_om_pi_set_target, NULL)
    PHP_FALIAS( , axis2_om_pi_get_value, NULL)
    PHP_FALIAS( , axis2_om_pi_getTarget, NULL)
    PHP_FALIAS( , axis2_om_pi_serialize, NULL)
    PHP_ME(om_pi, __construct, NULL, ZEND_ACC_PUBLIC)
    { NULL, NULL, NULL}
};



PHP_FUNCTION(axis2_om_pi_set_value){}
PHP_FUNCTION(axis2_om_pi_set_target){}
PHP_FUNCTION(axis2_om_pi_get_value){}
PHP_FUNCTION(axis2_om_pi_getTarget){}
PHP_FUNCTION(axis2_om_pi_serialize){}
PHP_METHOD(om_pi, __construct){}