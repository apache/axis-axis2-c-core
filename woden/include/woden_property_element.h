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

#ifndef WODEN_PROPERTY_ELEMENT_H
#define WODEN_PROPERTY_ELEMENT_H

/**
 * @file woden_property_element.h
 * Represents the &lt;property&gt; element and its child elements;
 * &lt;documentation&gt;, 
 * &lt;value&gt; or &lt;constraint&gt; and
 * any extension elements.
 * <p>
 * A property may have a value or a constraint but not both. If it
 * has a value, the &lt;constraint&gt; element may only contain the
 * the NM Token <code>#value</code>, in which case 
 * <code>has_value_token()</code> will return true
 * and <code>get_constraint</code> will return null.
 * @brief Axis2 Property Element Interface
 *
 */

#include <axutil_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_generic_obj.h>
#include <axis2_uri.h>
#include <woden.h>
#include <woden_documentable_element.h>
#include <woden_nested_element.h>
#include <woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_property_element woden_property_element_t;
typedef struct woden_property_element_ops woden_property_element_ops_t;
typedef union woden_property_element_base woden_property_element_base_t;
struct xml_schema_type;

/** @defgroup woden_property_element Property Element
  * @ingroup woden
  * @{
  */

struct woden_property_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *doc_el,
            const axis2_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *doc_el,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_ref) (void *doc_el,
            const axis2_env_t *env,
            axis2_uri_t *uri);

    axis2_uri_t *(AXIS2_CALL *
    get_ref) (void *doc_el,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_value) (void *doc_el,
            const axis2_env_t *env,
            axis2_generic_obj_t *value);

    axis2_generic_obj_t *(AXIS2_CALL *
    get_value) (void *doc_el,
            const axis2_env_t *env);

    /**
     * Set the QName represented by the &lt;constraint&gt; element within the
     * &lt;property&gt; element. This QName identifies a global type definition 
     * from a schema inlined or imported within the &lt;types&gt; element. 
     * Note that the use of the NMToken <code>#value</code> 
     * as the constraint, instead of a QName, is captured using the 
     * <code>set_has_value_token</code> method.
     */ 
    axis2_status_t (AXIS2_CALL *
    set_constraint_qname) (
            void *property_el,
            const axis2_env_t *env,
            axis2_qname_t *qname);
   
    axis2_qname_t *(AXIS2_CALL *
    get_constraint_qname) (
            void *property_el,
            const axis2_env_t *env);

    /**
     * Returns the Schema type definition identified by the QName in the &lt;
     * constraint&gt; element within the &lt;property&gt; element.
     * If this QName does not resolve to an type definition in a schema that 
     * is visible to the containing WSDL description, null will be returned by 
     * this method. To be visible, the Schema must have been correctly imported 
     * or inlined within the &lt;types&gt; element.
     * 
     * @return the Xml Schema Type identified by the &lt;constraint&gt; element
     */
    struct xml_schema_type *(AXIS2_CALL *
    get_constraint) (
            void *property_el,
            const axis2_env_t *env);

    /**
     * Set to true if &lt;constraint&gt; specifies the NMToken <code>"#value"
     * </code> rather than a QName, indicating that this property specifies a 
     * value rather than a constraint.
     * Set to false if &lt;constraint&gt; does not specify the NMToken "#value", 
     * or if it is omitted. 
     */
    axis2_status_t (AXIS2_CALL *
    set_has_value_token) (
            void *property_el,
            const axis2_env_t *env,
            axis2_bool_t b);
    
    /**
     * Returns true if &lt;constraint&gt; specifies the NMToken "#value" rather 
     * than a QName, indicating that this property specifies a value rather than 
     * a constraint. Returns false if &lt;constraint&gt; does not specify the 
     * NMToken "#value", or if it is omitted.
     */ 
    axis2_bool_t (AXIS2_CALL * 
    has_value_token) (
            void *property_el,
            const axis2_env_t *env);

};

union woden_property_element_base
{
    woden_documentable_element_t documentable_element;
    woden_nested_element_t nested_element;
};

struct woden_property_element
{
    woden_property_element_base_t base;
    woden_property_element_ops_t *ops;
};

/************************Woden C Internal Methods*****************************/
axis2_status_t AXIS2_CALL
woden_property_element_resolve_methods(
        woden_property_element_t *property_element,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_PROPERTY_ELEMENT_FREE(doc_el, env) \
      (((woden_property_element_t *) doc_el)->ops->\
         free (doc_el, env))

#define WODEN_PROPERTY_ELEMENT_TYPE(doc_el, env) \
      (((woden_property_element_t *) doc_el)->ops->\
         type (doc_el, env))

#define WODEN_PROPERTY_ELEMENT_SET_REF(doc_el, env, uri) \
      (((woden_property_element_t *) doc_el)->ops->\
         set_ref(doc_el, env, uri))

#define WODEN_PROPERTY_ELEMENT_GET_REF(doc_el, env) \
      (((woden_property_element_t *) doc_el)->ops->\
         get_value(doc_el, env))

#define WODEN_PROPERTY_ELEMENT_SET_VALUE(doc_el, env, value) \
      (((woden_property_element_t *) doc_el)->ops->\
         set_value(doc_el, env, value))

#define WODEN_PROPERTY_ELEMENT_GET_VALUE(doc_el, env) \
      (((woden_property_element_t *) doc_el)->ops->\
         get_value(doc_el, env))


#define WODEN_PROPERTY_ELEMENT_SET_CONSTRAINT_QNAME(doc_el, env, qname) \
      (((woden_property_element_t *) doc_el)->ops->\
         set_constraint_qname(doc_el, env, qname))

#define WODEN_PROPERTY_ELEMENT_GET_CONSTRAINT_QNAME(doc_el, env) \
      (((woden_property_element_t *) doc_el)->ops->\
         get_constraint_qname(doc_el, env))

#define WODEN_PROPERTY_ELEMENT_GET_CONSTRAINT(doc_el, env) \
      (((woden_property_element_t *) doc_el)->ops->\
         get_constraint(doc_el, env))

#define WODEN_PROPERTY_ELEMENT_SET_HAS_VALUE_TOKEN(doc_el, env, b) \
      (((woden_property_element_t *) doc_el)->ops->\
         set_has_value_token(doc_el, env, b))

#define WODEN_PROPERTY_ELEMENT_HAS_VALUE_TOKEN(doc_el, env) \
      (((woden_property_element_t *) doc_el)->ops->\
         has_value_token(doc_el, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_PROPERTY_ELEMENT_H */
