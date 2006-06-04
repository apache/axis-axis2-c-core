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

#ifndef AXIS2_XML_SCHEMA_SIMPLE_CONTENT_H
#define AXIS2_XML_SCHEMA_SIMPLE_CONTENT_H

/**
 * @file axis2_xml_schema_simple_content.h
 * @brief Axis2 Xml Schema Group Interface
 *          Class that defines groups at the schema level that are referenced
 *          from the complex types. Groups a set of element declarations so that
 *          they can be incorporated as a sim_content into complex type definitions.
 *          Represents the World Wide Web Consortium (W3C) sim_content element.
 */

#include <xml_schema/axis2_xml_schema_annotated.h>
#include <axis2_hash.h>

/** @defgroup axis2_xml_schema_simple_content Xml Schema Group
  * @ingroup axis2_xml_schema
  * @{
  */



#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_xml_schema_simple_content 
                axis2_xml_schema_simple_content_t;
                
typedef struct axis2_xml_schema_simple_content_ops 
                axis2_xml_schema_simple_content_ops_t;

struct axis2_xml_schema_simple_content_ops
{
    axis2_status_t (AXIS2_CALL *
    free) (void *sim_content,
            const axis2_env_t *env);

    axis2_xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl) (void *sim_content,
                    const axis2_env_t *env);
                    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *sim_content,
                const axis2_env_t *env);
                    
    axis2_xml_schema_types_t (AXIS2_CALL *
    type) (void *sim_content,
           const axis2_env_t *env);                                        

    void* (AXIS2_CALL *
    get_content)(
            void *sim_content,
            const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_content)(
            void *sim_content,
            const axis2_env_t *env,
            void *content);
    
    axis2_char_t* (AXIS2_CALL*
    to_string)(void *sim_content,
               const axis2_env_t *env,
               axis2_char_t *prefix,
               int tab);
    
};

struct axis2_xml_schema_simple_content
{
    axis2_xml_schema_annotated_t base;
    axis2_xml_schema_simple_content_ops_t *ops;
};

/**
 * Creates new Xml Schema Group
 */
AXIS2_EXTERN axis2_xml_schema_simple_content_t * AXIS2_CALL
axis2_xml_schema_simple_content_create(const axis2_env_t *env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_xml_schema_simple_content_resolve_methods(
                                axis2_xml_schema_simple_content_t *sim_content,
                                const axis2_env_t *env,
                                axis2_xml_schema_simple_content_t *group_impl,
                                axis2_hash_t *methods);
 */
 
#define AXIS2_XML_SCHEMA_SIMPLE_CONTENT_FREE(sim_content, env) \
      (((axis2_xml_schema_simple_content_t *) sim_content)->ops->\
            free(sim_content, env))

#define AXIS2_XML_SCHEMA_SIMPLE_CONTENT_GET_BASE_IMPL(sim_content, env) \
      (((axis2_xml_schema_simple_content_t *) sim_content)->ops->\
            get_base_impl(sim_content, env))

#define AXIS2_XML_SCHEMA_SIMPLE_CONTENT_SUPER_OBJS(sim_content, env) \
      (((axis2_xml_schema_simple_content_t *) sim_content)->ops->\
            super_objs(sim_content, env))
            
#define AXIS2_XML_SCHEMA_SIMPLE_CONTENT_TYPE(sim_content, env) \
      (((axis2_xml_schema_simple_content_t *) sim_content)->ops->\
            type(sim_content, env))
                        
#define AXIS2_XML_SCHEMA_SIMPLE_CONTENT_GET_CONTENT(sim_content, env) \
      (((axis2_xml_schema_simple_content_t *) sim_content)->ops->\
            get_content(sim_content, env))

#define AXIS2_XML_SCHEMA_SIMPLE_CONTENT_SET_CONTENT(sim_content, env, content) \
      (((axis2_xml_schema_simple_content_t *) sim_content)->ops->\
            set_content(sim_content, env, content))

#define AXIS2_XML_SCHEMA_SIMPLE_CONTENT_TO_STRING(sim_content, env, prefix, tab) \
        (((axis2_xml_schema_simple_content_t *) sim_content)->ops->\
            to_string(sim_content, env, prefix, tab))            

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_SIMPLE_CONTENT_H */
