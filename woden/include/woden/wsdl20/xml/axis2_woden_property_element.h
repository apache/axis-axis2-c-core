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

#ifndef AXIS2_WODEN_PROPERTY_ELEMENT_H
#define AXIS2_WODEN_PROPERTY_ELEMENT_H

/**
 * @file axis2_woden_property_element.h
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

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_url.h>
#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_documentable_element.h>
#include <woden/wsdl20/xml/axis2_woden_nested_element.h>
#include <woden/axis2_woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_property_element axis2_woden_property_element_t;
typedef struct axis2_woden_property_element_ops axis2_woden_property_element_ops_t;
typedef union axis2_woden_property_element_base axis2_woden_property_element_base_t;
struct axis2_xml_schema_type;

/** @defgroup axis2_woden_property_element Property Element
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_property_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *doc_el,
            const axis2_env_t *env);
    
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *doc_el,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_ref) (void *doc_el,
            const axis2_env_t *env,
            axis2_url_t *uri);

    axis2_url_t *(AXIS2_CALL *
    get_ref) (void *doc_el,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_value) (void *doc_el,
            const axis2_env_t *env,
            void *value);

    void *(AXIS2_CALL *
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
    struct axis2_xml_schema_type *(AXIS2_CALL *
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

union axis2_woden_property_element_base
{
    axis2_woden_documentable_element_t documentable_element;
    axis2_woden_nested_element_t nested_element;
};

struct axis2_woden_property_element
{
    axis2_woden_property_element_base_t base;
    axis2_woden_property_element_ops_t *ops;
};

/************************Woden C Internal Methods*****************************/
axis2_status_t AXIS2_CALL
axis2_woden_property_element_resolve_methods(
        axis2_woden_property_element_t *property_element,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_PROPERTY_ELEMENT_FREE(doc_el, env) \
      (((axis2_woden_property_element_t *) doc_el)->ops->\
         free (doc_el, env))

#define AXIS2_WODEN_PROPERTY_ELEMENT_TYPE(doc_el, env) \
      (((axis2_woden_property_element_t *) doc_el)->ops->\
         type (doc_el, env))

#define AXIS2_WODEN_PROPERTY_ELEMENT_SET_REF(doc_el, env, uri) \
      (((axis2_woden_property_element_t *) doc_el)->ops->\
         set_ref(doc_el, env, uri))

#define AXIS2_WODEN_PROPERTY_ELEMENT_GET_REF(doc_el, env) \
      (((axis2_woden_property_element_t *) doc_el)->ops->\
         get_ref(doc_el, env))

#define AXIS2_WODEN_PROPERTY_ELEMENT_SET_CONSTRAINT_QNAME(doc_el, env, qname) \
      (((axis2_woden_property_element_t *) doc_el)->ops->\
         set_constraint_qname(doc_el, env, qname))

#define AXIS2_WODEN_PROPERTY_ELEMENT_GET_CONSTRAINT_QNAME(doc_el, env) \
      (((axis2_woden_property_element_t *) doc_el)->ops->\
         get_constraint_qname(doc_el, env))

#define AXIS2_WODEN_PROPERTY_ELEMENT_GET_CONSTRAINT(doc_el, env) \
      (((axis2_woden_property_element_t *) doc_el)->ops->\
         get_constraint(doc_el, env))

#define AXIS2_WODEN_PROPERTY_ELEMENT_SET_HAS_VALUE_TOKEN(doc_el, env, b) \
      (((axis2_woden_property_element_t *) doc_el)->ops->\
         set_has_value_token(doc_el, env, b))

#define AXIS2_WODEN_PROPERTY_ELEMENT_HAS_VALUE_TOKEN(doc_el, env) \
      (((axis2_woden_property_element_t *) doc_el)->ops->\
         has_value_token(doc_el, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_PROPERTY_ELEMENT_H */
