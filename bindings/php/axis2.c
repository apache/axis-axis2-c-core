/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2004 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.0 of the PHP license,       |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_0.txt.                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header,v 1.15 2004/01/08 16:46:52 sniper Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_axis2.h"
#include "axis2_env.h"
#include "axis2_error_default.h"
#include "axis2_stream_default.h"
#include "axis2_log_default.h"


zend_object_value axis2_objects_new(zend_class_entry *class_type TSRMLS_DC );



/* If you declare any globals in php_axis2.h uncomment this:
*/
ZEND_DECLARE_MODULE_GLOBALS(axis2)


/* True global resources - no need for thread safety here */
static int le_axis2;

static zend_object_handlers axis2_object_handlers;

/* {{{ axis2_functions[]
 *
 * Every user visible function must have an entry in axis2_functions[].
 */
function_entry axis2_functions[] = {
	PHP_FE(axis2_helloworld,	NULL)		
	{NULL, NULL, NULL}	/* Must be the last line in axis2_functions[] */
};
/* }}} */

/* {{{ axis2_module_entry
 */
zend_module_entry axis2_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"axis2",
	axis2_functions,
	PHP_MINIT(axis2),
	PHP_MSHUTDOWN(axis2),
	PHP_RINIT(axis2),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(axis2),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(axis2),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_AXIS2
ZEND_GET_MODULE(axis2)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("axis2.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_axis2_globals, axis2_globals)
    STD_PHP_INI_ENTRY("axis2.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_axis2_globals, axis2_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_axis2_init_globals
 */

static void php_axis2_init_globals(zend_axis2_globals *axis2_globals)
{
    axis2_allocator_t *allocator = NULL;
    axis2_error_t *err = NULL;
    axis2_log_t *log = NULL;
    axis2_stream_t *stream = NULL;
    axis2_env_t *environment = NULL;
    allocator = axis2_allocator_init (NULL);
    log = axis2_log_create(allocator, NULL);
    stream = axis2_stream_create(allocator, NULL);
    err = axis2_error_create(allocator);
    
    environment= axis2_env_create_with_error_stream_log(allocator, err,stream, log);
    
	axis2_globals->global_value = 0;
	axis2_globals->global_string = NULL;
	axis2_globals->env = environment;
	axis2_globals->allocator = allocator;
}

/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(axis2)
{
    zend_class_entry ce;
	
	ZEND_INIT_MODULE_GLOBALS(axis2, php_axis2_init_globals, NULL);
    /* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	
	
	memcpy(&axis2_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	axis2_object_handlers.clone_obj = NULL;
	
	REGISTER_AXIS2_CLASS(ce,"Axis2XMLReader",NULL, php_axis2_xml_reader_class_functions, axis2_xml_reader_class_entry);
    
    
    
    
    
    REGISTER_LONG_CONSTANT("AXIS2_XML_READER_START_DOCUMENT",	AXIS2_XML_READER_START_DOCUMENT,		CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AXIS2_XML_READER_START_ELEMENT",	AXIS2_XML_READER_START_ELEMENT ,		CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AXIS2_XML_READER_END_ELEMENT",	    AXIS2_XML_READER_END_ELEMENT,		    CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AXIS2_XML_READER_SPACE",	        AXIS2_XML_READER_SPACE,		            CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AXIS2_XML_READER_EMPTY_ELEMENT",	AXIS2_XML_READER_EMPTY_ELEMENT,	     	CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AXIS2_XML_READER_CHARACTER",	    AXIS2_XML_READER_CHARACTER,		        CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AXIS2_XML_READER_ENTITY_REFERANCE",	AXIS2_XML_READER_ENTITY_REFERANCE,		CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AXIS2_XML_READER_COMMENT",	        AXIS2_XML_READER_COMMENT,		        CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AXIS2_XML_READER_PROCESSING_INSTRUCTION",	AXIS2_XML_READER_PROCESSING_INSTRUCTION,		CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AXIS2_XML_READER_CDATA",	        AXIS2_XML_READER_CDATA,		CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AXIS2_XML_READER_DOCUMENT_TYPE",	AXIS2_XML_READER_DOCUMENT_TYPE,		CONST_CS | CONST_PERSISTENT);
    
    	
	
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(axis2)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	php_axis2_destroy_env();
	return SUCCESS;
}
/* }}} */



void php_axis2_objects_free_storage(void *object TSRMLS_DC)
{
    axis2_object *intern = (axis2_object_ptr)object;
    zend_hash_destroy(intern->std.properties);
    FREE_HASHTABLE(intern->std.properties);
    
    if(intern->node_list)
    {
        zend_hash_destroy(intern->node_list);
        FREE_HASHTABLE(intern->node_list);
    
    }
    /*TODO individaul objects must be freed */
    if(intern->ptr)
    {
    
    
    }
    

}


static zend_object_value axis2_objects_new(zend_class_entry *class_type TSRMLS_DC )
{
    zend_object_value retval;
    axis2_object *intern = NULL;
    zval *tmp = NULL;
    
    intern = emalloc(sizeof(axis2_object));
    intern->std.ce = class_type;
    intern->std.in_get = 0;
    intern->std.in_set = 0;
    
    intern->ptr = NULL;
    intern->prop_handler = NULL;
    ALLOC_HASHTABLE(intern->std.properties);
    zend_hash_init(intern->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_copy(intern->std.properties, &class_type->default_properties,
             (copy_ctor_func_t)zval_add_ref, (void*)tmp, sizeof(zval *));
    retval.handle = zend_objects_store_put(intern, NULL,
            ( zend_objects_free_object_storage_t)php_axis2_objects_free_storage,NULL TSRMLS_CC);
            
    intern->handle = retval.handle;
    retval.handlers = &axis2_object_handlers;            
    return retval;
}













/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(axis2)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(axis2)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(axis2)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "axis2 support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */





/* {{{ proto string axis2_helloworld()
  say welcome note */
PHP_FUNCTION(axis2_helloworld)
{
    php_printf(" welcome axis2");
}
/* }}} */



void php_axis2_destroy_env()
{
    TSRMLS_FETCH();
    axis2_env_free(AXIS2_G(env));
    AXIS2_G(env) = NULL;
    AXIS2_G(allocator) = NULL;
}

axis2_env_t *php_axis2_get_env()
{
    TSRMLS_FETCH();
    return AXIS2_G(env);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
