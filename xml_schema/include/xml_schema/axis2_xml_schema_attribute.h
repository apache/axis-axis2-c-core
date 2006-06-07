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

#ifndef AXIS2_XML_SCHEMA_ATTRIBUTE_H
#define AXIS2_XML_SCHEMA_ATTRIBUTE_H

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
#include <xml_schema/axis2_xml_schema_form.h>
#include <xml_schema/axis2_xml_schema_use.h>
#include <xml_schema/axis2_xml_schema_content_processing.h>
#include <xml_schema/axis2_xml_schema_simple_type.h>


/** @defgroup axis2_xml_schema_any Xml Schema Any
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_attribute 
                    axis2_xml_schema_attribute_t;
typedef struct axis2_xml_schema_attribute_ops 
                axis2_xml_schema_attribute_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_attribute_ops
{
    axis2_status_t (AXIS2_CALL *
    free) (
            void *attr,
            const axis2_env_t *env);
            
    axis2_xml_schema_types_t (AXIS2_CALL *
    get_type)(
            void *attr,
            const axis2_env_t *env);
          
    axis2_hash_t* (AXIS2_CALL *
    super_objs)(
            void *attr,
            const axis2_env_t *env);
                                      

    axis2_xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl)(
            void *attr,
            const axis2_env_t *env);
    
    void *(AXIS2_CALL *
    get_attr_type)(
            void *attr,
            const axis2_env_t *env);

    axis2_char_t *(AXIS2_CALL *
    get_default_value)(
            void *attr,
            const axis2_env_t *env);                        

    axis2_status_t (AXIS2_CALL *
    set_default_value)(
            void *attr,
            const axis2_env_t *env,
            axis2_char_t *default_value);  
    
    axis2_char_t* (AXIS2_CALL *
    get_fixed_value)(void *attr,
                        const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_fixed_value)(void *attr,
                     const axis2_env_t *env,
                     axis2_char_t *fixed_value);

    struct axis2_xml_schema_form *(AXIS2_CALL *
    get_schema_form) (void *attr,
                      const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_schema_form)(void *attr,
                     const axis2_env_t *env,
                     axis2_xml_schema_form_t *schema_form);
                     
    axis2_qname_t* (AXIS2_CALL *
    get_qname)(void *attr,
               const axis2_env_t *env);
               
    axis2_status_t (AXIS2_CALL *
    set_qname)(void *attr,
               const axis2_env_t *env,
               axis2_qname_t *qualified_name);
               
    axis2_char_t* (AXIS2_CALL *
    get_name)(void *attr,
              const axis2_env_t *env);
              
    axis2_status_t (AXIS2_CALL *
    set_name)(void *attr,
              const axis2_env_t *env,
              axis2_char_t *name);
              
    axis2_qname_t* (AXIS2_CALL *
    get_ref_name)(void *attr,
                  const axis2_env_t *env);
                                              
    axis2_status_t (AXIS2_CALL *
    set_ref_name)(void *attr,
                  const axis2_env_t *env,
                  axis2_qname_t *ref_name);
                  
    axis2_xml_schema_simple_type_t* (AXIS2_CALL *
    get_schema_type)(void *attr,
                     const axis2_env_t *env);
                     
    axis2_status_t (AXIS2_CALL *
    set_schema_type)(void *attr,
                     const axis2_env_t *env,
                     axis2_xml_schema_simple_type_t* sch_type);                     
                     
    axis2_qname_t* (AXIS2_CALL *
    get_schema_type_name)(void *attr,
                          const axis2_env_t *env);
                          
                              
    axis2_status_t (AXIS2_CALL *
    set_schema_type_name)(void *attr,
                          const axis2_env_t *env,
                          axis2_qname_t *sch_type_name);
                          
    axis2_xml_schema_use_t* (AXIS2_CALL *
    get_use)(void *attr,
             const axis2_env_t *env);
             
    axis2_status_t (AXIS2_CALL *
    set_use)(void *attr,
             const axis2_env_t *env,
             axis2_xml_schema_use_t *use);
             
    axis2_char_t* (AXIS2_CALL*
    to_string)(void *attr,
               const axis2_env_t *env,
               axis2_char_t *prefix,
               int tab);                          
};

struct axis2_xml_schema_attribute
{
    axis2_xml_schema_annotated_t base;
    axis2_xml_schema_attribute_ops_t *ops;
};

AXIS2_EXTERN axis2_xml_schema_attribute_t * AXIS2_CALL
axis2_xml_schema_attribute_create(const axis2_env_t *env);


/******************************* Macros *************************************************/ 
 
#define AXIS2_XML_SCHEMA_ATTRIBUTE_FREE(attr, env) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->free(attr, env))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GET_BASE_IMPL(attr, env) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->get_base_impl(attr, env))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GET_TYPE(attr, env) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->get_type(attr, env))
      
#define AXIS2_XML_SCHEMA_ATTRIBUTE_SUPER_OBJS(attr, env) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->super_objs(attr, env))
      

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GET_ATTR_TYPE(attr, env) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->get_namespace(attr, env))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GET_DEFAULT_VALUE(attr, env) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          get_default_value(attr, env))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_SET_DEFAULT_VALUE(attr, env, default_val) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          set_default_value(attr, env, default_val))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GET_FIXED_VALUE(attr, env) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          get_fixed_value(attr, env))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_SET_FIXED_VALUE(attr, env, default_val) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          set_fixed_value(attr, env, default_val))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GET_SCHEMA_FORM(attr, env) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          get_schema_form(attr, env))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_SET_SCHEMA_FORM(attr, env, form) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          set_schema_form(attr, env, form))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GET_QNAME(attr, env) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          get_qname(attr, env))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_SET_QNAME(attr, env, qualified_name) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          set_qname(attr, env, qualified_name))
          
#define AXIS2_XML_SCHEMA_ATTRIBUTE_GET_NAME(attr, env) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          get_name(attr, env))
          
#define AXIS2_XML_SCHEMA_ATTRIBUTE_SET_NAME(attr, env, name) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          set_name(attr, env, name))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GET_REF_NAME(attr, env) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          get_ref_name(attr, env))
          
#define AXIS2_XML_SCHEMA_ATTRIBUTE_SET_REF_NAME(attr, env, ref_name) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          set_ref_name(attr, env, ref_name))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GET_SCHEMA_TYPE(attr, env) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          get_schema_type(attr, env))
          
#define AXIS2_XML_SCHEMA_ATTRIBUTE_SET_SCHEMA_TYPE(attr, env, sch_type) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          set_schema_type(attr, env, sch_type))
          
#define AXIS2_XML_SCHEMA_ATTRIBUTE_GET_SCHEMA_TYPE_NAME(attr, env) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          get_schema_type_name(attr, env))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_SET_SCHEMA_TYPE_NAME(attr, env, sch_type_name) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          set_schema_type_name(attr, env, sch_type_name))
          
#define AXIS2_XML_SCHEMA_ATTRIBUTE_GET_USE(attr, env) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          get_use(attr, env))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_SET_USE(attr, env, use) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          set_schema_type_name(attr, env, use))   
          
#define AXIS2_XML_SCHEMA_ATTRIBUTE_TO_STRING(attr, env, prefix, tab) \
      (((axis2_xml_schema_attribute_t *) attr)->ops->\
          to_string(attr, env, prefix, tab))   
                 

/******************************* end macros ***********************************************/
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_ATTRIBUTE_H */
