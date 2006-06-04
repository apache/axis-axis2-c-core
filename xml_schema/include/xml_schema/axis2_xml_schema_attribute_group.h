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

#ifndef AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_H
#define AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_H

/**
 * @file axis2_xml_schema_any.h
 * @brief Axis2 Xml Schema Any Interface
 *          Enables any element from the specified namespace or namespaces
 *          to appear in the containing complexType element. Represents the
 *          World Wide Web Consortium (W3C) any element.
 *
 */
#include <axis2_qname.h>
#include <xml_schema/axis2_xml_schema_particle.h>
#include <xml_schema/axis2_xml_schema_obj_collection.h>
#include <xml_schema/axis2_xml_schema_any_attribute.h>
#include <xml_schema/axis2_xml_schema_content_processing.h>
#include <xml_schema/axis2_xml_schema_simple_type.h>


/** @defgroup axis2_xml_schema_any Xml Schema Any
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_attribute_group 
                    axis2_xml_schema_attribute_group_t;
typedef struct axis2_xml_schema_attribute_group_ops 
                axis2_xml_schema_attribute_group_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_attribute_group_ops
{
    axis2_status_t (AXIS2_CALL *
    free) (
            void *attr_grp,
            const axis2_env_t *env);
            
    axis2_xml_schema_types_t (AXIS2_CALL *
    type)(
            void *attr_grp,
            const axis2_env_t *env);
          
    axis2_hash_t* (AXIS2_CALL *
    super_objs)(
            void *attr_grp,
            const axis2_env_t *env);
                                      

    axis2_xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl)(
            void *attr_grp,
            const axis2_env_t *env);
    
    axis2_xml_schema_any_attribute_t* (AXIS2_CALL *
    get_any_attribute)(
            void *attr_grp,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_any_attribute)(
            void *attr_grp,
            const axis2_env_t *env,
            axis2_xml_schema_any_attribute_t* any_attr);  
    
    axis2_xml_schema_obj_collection_t* (AXIS2_CALL *
    get_attributes)(void *attr_grp,
                    const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_attributes)(void *attr_grp,
                     const axis2_env_t *env,
                     axis2_xml_schema_obj_collection_t* attributes);

    axis2_char_t* (AXIS2_CALL *
    get_name)(void *attr_grp,
              const axis2_env_t *env);
              
    axis2_status_t (AXIS2_CALL *
    set_name)(void *attr_grp,
              const axis2_env_t *env,
              axis2_char_t *name);
              
};

struct axis2_xml_schema_attribute_group
{
    axis2_xml_schema_annotated_t base;
    axis2_xml_schema_attribute_group_ops_t *ops;
};

AXIS2_EXTERN axis2_xml_schema_attribute_group_t * AXIS2_CALL
axis2_xml_schema_attribute_group_create(const axis2_env_t *env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_xml_schema_attribute_resolve_methods(
                                axis2_xml_schema_attribute_t *attr,
                                const axis2_env_t *env,
                                axis2_xml_schema_attribute_t *any_impl,
                                axis2_hash_t *methods);
 */
/******************************* Macros *************************************************/ 
 
#define AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_FREE(attr_grp, env) \
      (((axis2_xml_schema_attribute_group_t *) attr_grp)->ops->free(attr_grp, env))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_GET_BASE_IMPL(attr_grp, env) \
      (((axis2_xml_schema_attribute_group_t *) attr_grp)->ops->get_base_impl(attr_grp, env))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_TYPE(attr_grp, env) \
      (((axis2_xml_schema_attribute_group_t *) attr_grp)->ops->type(attr_grp, env))
      
#define AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_SUPER_OBJS(attr_grp, env) \
      (((axis2_xml_schema_attribute_group_t *) attr_grp)->ops->super_objs(attr_grp, env))
      

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_GET_ANY_ATTRIBUTE(attr_grp, env) \
      (((axis2_xml_schema_attribute_group_t *) attr_grp)->ops->\
          get_any_attribute(attr_grp, env))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_SET_ANY_ATTRIBUTE(attr_grp, env, any_attr) \
      (((axis2_xml_schema_attribute_group_t *) attr_grp)->ops->\
          set_any_attribute(attr_grp, env, any_attr))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_SET_ATTRIBUTES(attr_grp, env, attrs) \
      (((axis2_xml_schema_attribute_group_t *) attr_grp)->ops->\
          set_attributes(attr_grp, env, attrs))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_GET_ATTRIBUTES(attr_grp, env) \
      (((axis2_xml_schema_attribute_group_t *) attr_grp)->ops->\
          get_attributes(attr_grp, env))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_GET_NAME(attr_grp, env) \
      (((axis2_xml_schema_attribute_group_t *) attr_grp)->ops->\
          get_name(attr_grp, env))
          
#define AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_SET_NAME(attr_grp, env, name) \
      (((axis2_xml_schema_attribute_group_t *) attr_grp)->ops->\
          set_name(attr_grp, env, name))

/******************************* end macros ***********************************************/
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_H */
