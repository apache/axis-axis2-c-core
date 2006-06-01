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

#ifndef AXIS2_XML_SCHEMA_SIMPLE_TYPE_H
#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_H

/**
 * @file axis2_xml_schema_type.h
 * @brief Axis2 Xml Schema Type Interface
 *          The base class for all simple types and complex types.
 */

#include <xml_schema/axis2_xml_schema_annotated.h>
#include <xml_schema/axis2_xml_schema_type.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <xml_schema/axis2_xml_schema_simple_type_content.h>
/** @defgroup axis2_xml_schema_simple_type Xml Schema Type
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_simple_type 
                    axis2_xml_schema_simple_type_t;
typedef struct axis2_xml_schema_simple_type_ops 
                    axis2_xml_schema_simple_type_ops_t;
#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_simple_type_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *simple_type,
            const axis2_env_t *env);

    axis2_xml_schema_type_t *(AXIS2_CALL *
    get_base_impl) (void *simple_type,
                    const axis2_env_t *env);
                    
    axis2_xml_schema_types_t (AXIS2_CALL *
    type) (void *simple_type,
            const axis2_env_t *env);
                    
    axis2_hash_t* (AXIS2_CALL *
    super_objs) (void *simple_type,
                 const axis2_env_t *env);                                        
                    
    axis2_xml_schema_simple_type_content_t* (AXIS2_CALL *
    get_content)(void *simple_type,
                 const axis2_env_t *env);
                 
    axis2_status_t (AXIS2_CALL*
    set_content)(void *simple_type,
                 const axis2_env_t *env,
                 axis2_xml_schema_simple_type_content_t *simple_content);
                 
    axis2_char_t* (AXIS2_CALL *
    to_string)(void *simple_type,
               const axis2_env_t *env,
               axis2_char_t *prefix,
               int tab);
};

struct axis2_xml_schema_simple_type
{
    axis2_xml_schema_type_t base;
    axis2_xml_schema_simple_type_ops_t *ops;
};

/**
 * Creates new Xml Schema Type
 * Type types are usually interchangeable.
 * A local element declaration or reference to a global element
 * declaration (element), a compositor ( sequence, choice, or all),
 * a reference to a named content model group (group), or an element wildcard 
 * (any).
 */
AXIS2_EXTERN axis2_xml_schema_simple_type_t * AXIS2_CALL
axis2_xml_schema_simple_type_create(const axis2_env_t *env,
                                struct axis2_xml_schema *schema);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_xml_schema_simple_type_resolve_methods(
                                axis2_xml_schema_simple_type_t *simple_type,
                                const axis2_env_t *env,
                                axis2_xml_schema_simple_type_t *type_impl,
                                axis2_hash_t *methods);
 */
 /***************** Macros ***************************************************/
#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_FREE(simple_type, env) \
		(((axis2_xml_schema_simple_type_t *) simple_type)->ops->\
            free(simple_type, env))

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_GET_BASE_IMPL(simple_type, env) \
		(((axis2_xml_schema_simple_type_t *) simple_type)->ops->\
            get_base_impl(simple_type, env))

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_TYPE(simple_type, env) \
		(((axis2_xml_schema_simple_type_t *) simple_type)->ops->\
            type(simple_type, env))
            
#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_SUPER_OBJS(simple_type, env) \
		(((axis2_xml_schema_simple_type_t *) simple_type)->ops->\
            super_objs(simple_type, env))            

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_GET_CONTENT(simple_type, env) \
		(((axis2_xml_schema_simple_type_t *) simple_type)->ops->\
            get_content(simple_type, env))

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_SET_CONTENT(simple_type, env, content) \
		(((axis2_xml_schema_simple_type_t *) simple_type)->ops->\
            set_content(simple_type, env, content))

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_TO_STRING(simple_type, env) \
		(((axis2_xml_schema_simple_type_t *) simple_type)->ops->\
            to_string(simple_type, env))
                                                           
/************************ end macros ********************************************/                                                           
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_SIMPLE_TYPE_H */
