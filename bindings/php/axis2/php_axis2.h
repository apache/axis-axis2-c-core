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

#ifndef PHP_AXIS2_H
#define PHP_AXIS2_H

extern zend_module_entry axis2_module_entry;
#define phpext_axis2_ptr &axis2_module_entry

#ifdef PHP_WIN32
#define PHP_AXIS2_API __declspec(dllexport)
#else
#define PHP_AXIS2_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include "axis2_ce.h"
#include "axis2_fe.h"
#include "axis2_common.h"

#include <axis2.h>
#include <axis2_env.h>

PHP_MINIT_FUNCTION(axis2);
PHP_MSHUTDOWN_FUNCTION(axis2);
PHP_RINIT_FUNCTION(axis2);
PHP_RSHUTDOWN_FUNCTION(axis2);
PHP_MINFO_FUNCTION(axis2);

PHP_FUNCTION(axis2_helloworld);	/* For testing, remove later. */



typedef struct axis2_object{
    zend_object std;
    void *ptr;
    int obj_type;
    HashTable *prop_handler;
    zend_object_handle handle;
    HashTable *node_list;
}axis2_object;

typedef axis2_object *axis2_object_ptr;

void php_axis2_objects_free_storage(void *object TSRMLS_DC);
void php_axis2_destroy_env();
axis2_env_t *php_axis2_get_env();
zval *php_axis2_create_om_object(void* obj, int obj_type, zend_class_entry *class_type TSRMLS_DC);
zval *php_axis2_create_om_node_object(void* obj,int node_type, zend_class_entry *class_type  TSRMLS_DC);
void php_axis2_set_object(void *obj,int obj_type, zval *wrapper_in TSRMLS_DC); 
char* php_axis2_stream_to_buffer(php_stream *stream TSRMLS_DC);
zend_class_entry* php_axis2_get_class_type(int node_type);
/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     */

ZEND_BEGIN_MODULE_GLOBALS(axis2)
	long  global_value;
	char *global_string;
	axis2_env_t *env;
	axis2_allocator_t *allocator;
ZEND_END_MODULE_GLOBALS(axis2)


/* In every utility function you add that needs to use variables 
   in php_axis2_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as AXIS2_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define AXIS2_G(v) TSRMG(axis2_globals_id, zend_axis2_globals *, v)
#else
#define AXIS2_G(v) (axis2_globals.v)
#endif

#endif	/* PHP_AXIS2_H */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */


