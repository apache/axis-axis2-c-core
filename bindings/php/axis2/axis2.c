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
#include "axis2_om_element.h"


static zend_object_value axis2_objects_new(zend_class_entry *class_type TSRMLS_DC );



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

    allocator = axis2_allocator_init(NULL);    
    log = axis2_log_create(allocator, NULL);
    err = axis2_error_create(allocator);
    
    environment= axis2_env_create_with_error_log(allocator, err, log);
    
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
    
    REGISTER_AXIS2_CLASS(ce,"Axis2XMLWriter",NULL, php_axis2_xml_writer_class_functions, axis2_xml_writer_class_entry);
    
    REGISTER_AXIS2_CLASS(ce,"Axis2OMBuilder",NULL, php_axis2_om_stax_builder_class_functions, axis2_om_stax_builder_class_entry);
    
    REGISTER_AXIS2_CLASS(ce,"Axis2OMDocument",NULL, php_axis2_om_document_class_functions, axis2_om_document_class_entry);
    
    REGISTER_AXIS2_CLASS(ce,"Axis2OMOutput",NULL, php_axis2_om_output_class_functions, axis2_om_output_class_entry);
    
    
    REGISTER_AXIS2_CLASS(ce, "Axis2OMNamespace", NULL, php_axis2_om_namespace_class_functions, axis2_om_namespace_class_entry);
    
    REGISTER_AXIS2_CLASS(ce, "Axis2OMAttribute", NULL, php_axis2_om_attribute_class_functions, axis2_om_attribute_class_entry);
    
    REGISTER_AXIS2_CLASS(ce, "Axis2Qname", NULL, php_axis2_qname_class_functions, axis2_qname_class_entry);
   
    REGISTER_AXIS2_CLASS(ce, "Axis2OMNode", NULL, php_axis2_om_node_class_functions, axis2_om_node_class_entry);
    
    REGISTER_AXIS2_CLASS(ce, "Axis2OMElement", axis2_om_node_class_entry , php_axis2_om_element_class_functions , axis2_om_element_class_entry);
    
    REGISTER_AXIS2_CLASS(ce, "Axis2OMText", axis2_om_node_class_entry , php_axis2_om_text_class_functions , axis2_om_text_class_entry);
    
    REGISTER_AXIS2_CLASS(ce, "Axis2OMComment", axis2_om_node_class_entry , php_axis2_om_comment_class_functions , axis2_om_comment_class_entry);

    REGISTER_AXIS2_CLASS(ce, "Axis2OMPI", axis2_om_node_class_entry , php_axis2_om_pi_class_functions , axis2_om_pi_class_entry);

    
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
    
    REGISTER_LONG_CONSTANT("AXIS2_OM_ELEMENT",	OM_ELEMENT,		CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AXIS2_OM_TEXT",	    OM_TEXT,		CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AXIS2_OM_PI",	    OM_PI,		CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AXIS2_OM_DOCTYPE",	OM_DOCTYPE,		CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AXIS2_OM_COMMENT",	OM_COMMENT,		CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("AXIS2_OM_DOCUMENT",	OM_DOCUMENT,		CONST_CS | CONST_PERSISTENT);
    	
	
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
    om_object_ptr om_obj = NULL;
    om_node_ptr node_obj = NULL;
    axis2_env_t *env = NULL;
    axis2_om_element_t *om_ele = NULL;
    axis2_om_node_t *node = NULL;
    axis2_object_ptr intern_temp = NULL;
    axis2_object *intern = (axis2_object_ptr)object;
    zend_hash_destroy(intern->std.properties);
    FREE_HASHTABLE(intern->std.properties);
    
    
    env = php_axis2_get_env();
    /*TODO individual objects must be freed */
    if(intern->obj_type == OM_NODE_OBJ)
    {
     /* */    
    }
    else if(intern->obj_type == OM_OBJ)
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

/* {{{ create_axis2_om_object */
zval *php_axis2_create_om_object(void *obj, int obj_type, zend_class_entry *class_type TSRMLS_DC)
{
    zval *wrapper = NULL;
    zend_class_entry *ce = NULL;
    axis2_object *intern = NULL;
    om_object *om_obj = NULL;
    
    if(!obj)
    {
        return NULL;
    }
    ce = class_type;
   
    ALLOC_ZVAL(wrapper);
    /* ZVAL_NULL(wrapper); */
    object_init_ex(wrapper,ce);
    intern = (axis2_object_ptr)zend_objects_get_address(wrapper TSRMLS_CC);
    ZVAL_ADDREF(wrapper);
    
    om_obj = (om_object_ptr)emalloc(sizeof(om_object));
    om_obj->ptr = obj;
    om_obj->obj_type = obj_type;
    intern->ptr = om_obj;
    intern->obj_type = OM_OBJ;
    return (wrapper);   
}

/* }}} end create object */

/* {{{ create om_node object */

zval *php_axis2_create_om_node_object(void *obj, int node_type, zend_class_entry *class_type  TSRMLS_DC)
{
    zval *wrapper = NULL;
    zend_class_entry *ce = NULL;
    axis2_object *intern = NULL;
    om_node_t *node_obj = NULL;
    if(!obj)
    {
        return NULL;
    }
    ce = class_type;
    ALLOC_ZVAL(wrapper);
    /* ZVAL_NULL(wrapper); */
    object_init_ex(wrapper, ce);
    intern = (axis2_object_ptr)zend_objects_get_address(wrapper TSRMLS_CC);
    ZVAL_ADDREF(wrapper);
    
    node_obj = (om_node_ptr)emalloc(sizeof(om_node_t));
    node_obj->ptr = obj;
    node_obj->node_type = node_type;
    node_obj->builder = NULL;
    node_obj->doc = NULL;
    intern->ptr = node_obj;
    intern->obj_type = OM_NODE_OBJ;
    return (wrapper);
}
/* }}} end om_node_create obj */

/* {{{ set Object */
void php_axis2_set_object(void *obj,int obj_type, zval *wrapper_in TSRMLS_DC)
{
    axis2_object_ptr axis2_obj = NULL;
   
    axis2_obj = (axis2_object_ptr)zend_object_store_get_object(wrapper_in TSRMLS_CC);
    axis2_obj->obj_type = obj_type;
    axis2_obj->ptr = obj;
}
/* }}} end set object */

/* {{{ stream to buffer */
char* php_axis2_stream_to_buffer(php_stream *stream TSRMLS_DC)
{
    char *buffer = NULL;
    int size = 0;
    char *temp_buffer = NULL;
    size = php_stream_copy_to_mem(stream,&temp_buffer,PHP_STREAM_COPY_ALL, 0);
    buffer = estrdup(temp_buffer);
    pefree(temp_buffer, 0);
    return buffer;
}

zend_class_entry* php_axis2_get_class_type(int node_type)
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
/* }}} end stream to buffer */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
