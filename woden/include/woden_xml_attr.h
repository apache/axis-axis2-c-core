/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WODEN_XML_ATTR_H
#define WODEN_XML_ATTR_H

/**
 * @file woden_xml_attr.h
 * @brief Axis2 Xml Attribute Interface
 *          This is an abstract superclass for all classes representing different
 *          types of XML attribute information items.
 */

#include <axutil_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_generic_obj.h>
#include <woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_xml_attr woden_xml_attr_t;
typedef struct woden_xml_attr_ops woden_xml_attr_ops_t;
struct axiom_node;
struct axiom_element;

/** @defgroup woden_xml_attr Xml Attribute
  * @ingroup woden
  * @{
  */

struct woden_xml_attr_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *xml_attr,
            const axis2_env_t *env);
      
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *xml_attr,
            const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    init) (
            void *xml_attr,
            const axis2_env_t *env,
            struct axiom_element *owner_el,
            struct axiom_node *owner_node,
            axis2_qname_t *attr_type, 
            axis2_char_t *attr_value);

    axis2_qname_t *(AXIS2_CALL *
    get_attribute_type) (
            void *xml_attr,
            const axis2_env_t *env);

    axis2_generic_obj_t *(AXIS2_CALL *
    get_content) (
            void *xml_attr,
            const axis2_env_t *env);

    axis2_char_t *(AXIS2_CALL *
    to_external_form) (
            void *xml_attr,
            const axis2_env_t *env);

    axis2_bool_t (AXIS2_CALL *
    is_valid) (
            void *xml_attr,
            const axis2_env_t *env);

    /* ************************************************************
     *  Non-API implementation methods 
     * ************************************************************/

    void *(AXIS2_CALL *
    convert) (
            void *xml_attr,
            const axis2_env_t *env,
            struct axiom_element *owner_el,
            struct axiom_node *ownder_node,
            axis2_char_t *attr_value);

    /*
     * Validity is initialized to true, but set to false if the attribute's
     * value is null or if it cannot be converted to an object of the 
     * appropriate type). This method may be used to change the validity later
     * (e.g. if Validation determines an error).
     * 
     * TODO confirm this method is needed, otherwise remove it.
     */
    axis2_status_t (AXIS2_CALL *
    set_valid) (
            void *xml_attr,
            const axis2_env_t *env,
            axis2_bool_t validity);
      
};

struct woden_xml_attr
{
    woden_xml_attr_ops_t *ops;
};

AXIS2_EXTERN woden_xml_attr_t * AXIS2_CALL
woden_xml_attr_create(
        const axis2_env_t *env,
        struct axiom_element *owner_el,
        struct axiom_node *owner_node,
        axis2_qname_t *attr_type, 
        axis2_char_t *attr_value);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_xml_attr_resolve_methods(
        woden_xml_attr_t *xml_attr,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_XML_ATTR_FREE(xml_attr, env) \
      (((woden_xml_attr_t *) xml_attr)->ops->free (xml_attr, env))

#define WODEN_XML_ATTR_TYPE(xml_attr, env) \
      (((woden_xml_attr_t *) xml_attr)->ops->type (xml_attr, env))

#define WODEN_XML_ATTR_INIT(xml_attr, env, owner_el, owner_node, \
        attr_type, attr_value) \
      (((woden_xml_attr_t *) xml_attr)->ops->init(xml_attr, env, \
            owner_el, owner_node, attr_type, attr_value))

#define WODEN_XML_ATTR_GET_ATTRIBUTE_TYPE(xml_attr, env) \
      (((woden_xml_attr_t *) xml_attr)->ops->\
         get_attribute_type(xml_attr, env))

#define WODEN_XML_ATTR_GET_CONTENT(xml_attr, env) \
      (((woden_xml_attr_t *) xml_attr)->ops->get_content(xml_attr, env))

#define WODEN_XML_ATTR_TO_EXTERNAL_FORM(xml_attr, env) \
      (((woden_xml_attr_t *) xml_attr)->ops->\
         to_external_form(xml_attr, env))

#define WODEN_XML_ATTR_IS_VALID(xml_attr, env) \
      (((woden_xml_attr_t *) xml_attr)->ops->is_valid(xml_attr, env))

#define WODEN_XML_ATTR_CONVERT(xml_attr, env, owner_el, owner_node, \
        attr_type, attr_value) \
      (((woden_xml_attr_t *) xml_attr)->ops->convert(xml_attr, env, \
            owner_el, owner_node, attr_type, attr_value))

#define WODEN_XML_ATTR_SET_VALID(xml_attr, env, validity) \
      (((woden_xml_attr_t *) xml_attr)->ops->set_valid(xml_attr, \
                                                  env, validity))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_XML_ATTR_H */
