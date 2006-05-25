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

#ifndef AXIS2_XML_SCHEMA_COMPLEX_CONTENT_H
#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_H

/**
 * @file axis2_xml_schema_complex_content.h
 * @brief Axis2 Xml Schema Group Interface
 *          Class that defines groups at the schema level that are referenced
 *          from the complex types. Groups a set of element declarations so that
 *          they can be incorporated as a complex_content into complex type definitions.
 *          Represents the World Wide Web Consortium (W3C) complex_content element.
 */

#include <xml_schema/axis2_xml_schema_annotated.h>
#include <axis2_hash.h>

/** @defgroup axis2_xml_schema_complex_content Xml Schema Group
  * @ingroup axis2_xml_schema
  * @{
  */
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_xml_schema_complex_content 
                axis2_xml_schema_complex_content_t;
                
typedef struct axis2_xml_schema_complex_content_ops 
                axis2_xml_schema_complex_content_ops_t;

struct axis2_xml_schema_complex_content_ops
{
    axis2_status_t (AXIS2_CALL *
    free) (void *complex_content,
            axis2_env_t **env);

    axis2_xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl) (void *complex_content,
                    axis2_env_t **env);
                    
    axis2_xml_schema_types_t (AXIS2_CALL *
    type) (void *complex_content,
            axis2_env_t **env);
                    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *complex_content,
                    axis2_env_t **env);                                        

    void* (AXIS2_CALL *
    get_content)(
            void *complex_content,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_content)(
            void *complex_content,
            axis2_env_t **env,
            void *content);
    
    axis2_char_t* (AXIS2_CALL*
    to_string)(void *complex_content,
               axis2_env_t **env,
               axis2_char_t *prefix,
               int tab);
               
    axis2_bool_t (AXIS2_CALL *
    is_mixed)(void *complex_content,
              axis2_env_t **env);
              
    axis2_status_t (AXIS2_CALL *
    set_mixed)(void *complex_content,
               axis2_env_t **env,
               axis2_bool_t mixed);
};

struct axis2_xml_schema_complex_content
{
    axis2_xml_schema_annotated_t base;
    axis2_xml_schema_complex_content_ops_t *ops;
};

/**
 * Creates new Xml Schema Group
 */
AXIS2_DECLARE(axis2_xml_schema_complex_content_t *)
axis2_xml_schema_complex_content_create(axis2_env_t **env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_complex_content_resolve_methods(
                                axis2_xml_schema_complex_content_t *complex_content,
                                axis2_env_t **env,
                                axis2_xml_schema_complex_content_t *group_impl,
                                axis2_hash_t *methods);
*/

#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_FREE(complex_content, env) \
		(((axis2_xml_schema_complex_content_t *) complex_content)->ops->\
            free(complex_content, env))

#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_GET_BASE_IMPL(complex_content, env) \
		(((axis2_xml_schema_complex_content_t *) complex_content)->ops->\
            get_base_impl(complex_content, env))
            
#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_TYPE(complex_content, env) \
		(((axis2_xml_schema_complex_content_t *) complex_content)->ops->\
            type(complex_content, env))
            
#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_SUPER_OBJS(complex_content, env) \
		(((axis2_xml_schema_complex_content_t *) complex_content)->ops->\
            super_objs(complex_content, env))                        

#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_GET_CONTENT(complex_content, env) \
		(((axis2_xml_schema_complex_content_t *) complex_content)->ops->\
            get_content(complex_content, env))

#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_SET_CONTENT(complex_content, env, content) \
		(((axis2_xml_schema_complex_content_t *) complex_content)->ops->\
            set_content(complex_content, env, content ))

#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_TO_STRING(complex_content, env, prefix, tab) \
        (((axis2_xml_schema_complex_content_t *) complex_content)->ops->\
            to_string(complex_content, env, prefix, tab))            

#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_IS_MIXED(complex_content, env) \
        (((axis2_xml_schema_complex_content_t *) complex_content)->ops->\
            is_mixed(complex_content, env))
            
#define AXIS2_XML_SCHEMA_COMPLEX_CONTENT_SET_MIXED(complex_content, env, mixed) \
        (((axis2_xml_schema_complex_content_t *) complex_content)->ops->\
            set_mixed(complex_content, env, mixed))            
            
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_COMPLEX_CONTENT_H */
