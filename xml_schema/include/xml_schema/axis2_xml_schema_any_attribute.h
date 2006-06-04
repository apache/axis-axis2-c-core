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

#ifndef AXIS2_XML_SCHEMA_ANY_ATTRIBUTE_H
#define AXIS2_XML_SCHEMA_ANY_ATTRIBUTE_H

/**
 * @file axis2_xml_schema_any.h
 * @brief Axis2 Xml Schema Any Interface
 *          Enables any element from the specified namespace or namespaces
 *          to appear in the containing complexType element. Represents the
 *          World Wide Web Consortium (W3C) any element.
 *
 */

#include <xml_schema/axis2_xml_schema_particle.h>
#include <xml_schema/axis2_xml_schema_content_processing.h>

/** @defgroup axis2_xml_schema_any Xml Schema Any
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_any_attribute 
                    axis2_xml_schema_any_attribute_t;
typedef struct axis2_xml_schema_any_attribute_ops 
                axis2_xml_schema_any_attribute_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_any_attribute_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *any_attr,
            const axis2_env_t *env);
            
    axis2_xml_schema_types_t (AXIS2_CALL *
    type)(void *any_attr,
          const axis2_env_t *env);
          
    axis2_hash_t* (AXIS2_CALL *
    super_objs)(void *any_attr,
                const axis2_env_t *env);
                                      

    axis2_xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl) (void *any_attr,
                    const axis2_env_t *env);
    
    axis2_char_t *(AXIS2_CALL *
    get_namespace)(void *any_attr,
                    const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_namespace) (void *any_attr,
                    const axis2_env_t *env,
                    axis2_char_t *ns);

    struct axis2_xml_schema_content_processing *(AXIS2_CALL *
    get_process_content) (void *any_attr,
                          const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_process_content)(void *any_attr,
                        const axis2_env_t *env,
                        struct axis2_xml_schema_content_processing *process_content);
};

struct axis2_xml_schema_any_attribute
{
    axis2_xml_schema_annotated_t base;
    axis2_xml_schema_any_attribute_ops_t *ops;
};

AXIS2_EXTERN axis2_xml_schema_any_attribute_t * AXIS2_CALL
axis2_xml_schema_any_attribute_create(const axis2_env_t *env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_xml_schema_any_attribute_resolve_methods(
                                axis2_xml_schema_any_attribute_t *any_attr,
                                const axis2_env_t *env,
                                axis2_xml_schema_any_attribute_t *any_impl,
                                axis2_hash_t *methods);
 */
/******************************* Macros *************************************************/ 
 
#define AXIS2_XML_SCHEMA_ANY_ATTRIBUTE_FREE(any_attr, env) \
      (((axis2_xml_schema_any_attribute_t *) any_attr)->ops->free(any_attr, env))

#define AXIS2_XML_SCHEMA_ANY_ATTRIBUTE_GET_BASE_IMPL(any_attr, env) \
      (((axis2_xml_schema_any_attribute_t *) any_attr)->ops->get_base_impl(any_attr, env))


#define AXIS2_XML_SCHEMA_ANY_ATTRIBUTE_TYPE(any_attr, env) \
      (((axis2_xml_schema_any_attribute_t *) any_attr)->ops->type(any_attr, env))
      
      
#define AXIS2_XML_SCHEMA_ANY_ATTRIBUTE_SUPER_OBJS(any_attr, env) \
      (((axis2_xml_schema_any_attribute_t *) any_attr)->ops->super_objs(any_attr, env))
      
      
#define AXIS2_XML_SCHEMA_ANY_ATTRIBUTE_GET_NAMESPACE(any_attr, env) \
      (((axis2_xml_schema_any_attribute_t *) any_attr)->ops->get_namespace(any_attr, env))

#define AXIS2_XML_SCHEMA_ANY_ATTRIBUTE_SET_NAMESPACE(any_attr, env, namespc) \
      (((axis2_xml_schema_any_attribute_t *) any_attr)->ops->set_namespace(any_attr, env, namespc))

#define AXIS2_XML_SCHEMA_ANY_ATTRIBUTE_GET_PROCESS_CONTENT(any_attr, env) \
      (((axis2_xml_schema_any_attribute_t *) any_attr)->ops->get_process_content(any_attr, env))

#define AXIS2_XML_SCHEMA_ANY_ATTRIBUTE_SET_PROCESS_CONTENT(any_attr, env, process_content) \
      (((axis2_xml_schema_any_attribute_t *) any_attr)->ops->set_process_content(any_attr, env, process_content))

/******************************* end macros ***********************************************/
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_ANY_ATTRIBUTE_H */
