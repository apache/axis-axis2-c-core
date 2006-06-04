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

#ifndef AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_REF_H
#define AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_REF_H

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


/** @defgroup_ref axis2_xml_schema_any Xml Schema Any
  * @ingroup_ref axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_attribute_group_ref 
                    axis2_xml_schema_attribute_group_ref_t;
typedef struct axis2_xml_schema_attribute_group_ref_ops 
                axis2_xml_schema_attribute_group_ref_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_attribute_group_ref_ops
{
    axis2_status_t (AXIS2_CALL *
    free) (
            void *grp_ref,
            const axis2_env_t *env);
            
    axis2_xml_schema_types_t (AXIS2_CALL *
    type)(
            void *grp_ref,
            const axis2_env_t *env);
          
    axis2_hash_t* (AXIS2_CALL *
    super_objs)(
            void *grp_ref,
            const axis2_env_t *env);
                                      

    axis2_xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl)(
            void *grp_ref,
            const axis2_env_t *env);
    

    axis2_qname_t* (AXIS2_CALL *
    get_ref_qname)(void *grp_ref,
              const axis2_env_t *env);
              
    axis2_status_t (AXIS2_CALL *
    set_ref_qname)(void *grp_ref,
              const axis2_env_t *env,
              axis2_qname_t *ref_name);
              
};

struct axis2_xml_schema_attribute_group_ref
{
    axis2_xml_schema_annotated_t base;
    axis2_xml_schema_attribute_group_ref_ops_t *ops;
};

AXIS2_EXTERN axis2_xml_schema_attribute_group_ref_t * AXIS2_CALL
axis2_xml_schema_attribute_group_ref_create(const axis2_env_t *env);

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
 
#define AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_REF_FREE(grp_ref, env) \
      (((axis2_xml_schema_attribute_group_ref_t *) grp_ref)->ops->free(grp_ref, env))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_REF_GET_BASE_IMPL(grp_ref, env) \
      (((axis2_xml_schema_attribute_group_ref_t *) grp_ref)->ops->get_base_impl(grp_ref, env))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_REF_TYPE(grp_ref, env) \
      (((axis2_xml_schema_attribute_group_ref_t *) grp_ref)->ops->type(grp_ref, env))
      
#define AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_REF_SUPER_OBJS(grp_ref, env) \
      (((axis2_xml_schema_attribute_group_ref_t *) grp_ref)->ops->super_objs(grp_ref, env))
      

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_REF_GET_REF_QNAME(grp_ref, env) \
      (((axis2_xml_schema_attribute_group_ref_t *) grp_ref)->ops->\
          get_ref_qname(grp_ref, env))

#define AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_REF_SET_REF_QNAME(grp_ref, env, ref_qname) \
      (((axis2_xml_schema_attribute_group_ref_t *) grp_ref)->ops->\
          set_ref_qname(grp_ref, env, ref_qname))

/******************************* end macros ***********************************************/
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_REF_H */
