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

#ifndef WODEN_EXT_ELEMENT_H
#define WODEN_EXT_ELEMENT_H

/**
 * @file woden_ext_element.h
 * @brief Axis2 Extension Element Interface
 *          This interface represents WSDL 2.0 extension elements. That is, 
 *          any XML element information items that appear as [children] of a 
 *          WSDL 2.0 element and are not in  the WSDL 2.0 namespace 
 *          (http://www.w3.org/2006/01/wsdl).
 *          <p>
 *          Based on a similar interface from WSDL4J element extensibility.
 *
 *          TODO change this URL if the WSDL 2.0 namespace changes before spec 
 *          is finalized.
 *
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axutil_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axutil_hash.h>
#include <axis2_qname.h>
#include <woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_ext_element woden_ext_element_t;
typedef struct woden_ext_element_ops woden_ext_element_ops_t;
struct axis2_xml_ext_element;
struct axis2_uri;

/** @defgroup woden_ext_element Extension Element
  * @ingroup woden
  * @{
  */

struct woden_ext_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *ext_element,
            const axutil_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *ext_element,
            const axutil_env_t *env);


    axis2_status_t (AXIS2_CALL *
    set_ext_type) (void *ext_element,
                        const axutil_env_t *env,
                        axis2_qname_t *qname);

    axis2_qname_t *(AXIS2_CALL *
    get_ext_type) (void *ext_element,
                        const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_required) (void *ext_element,
                        const axutil_env_t *env,
                        axis2_bool_t required);

    axis2_bool_t (AXIS2_CALL *
    is_required) (void *ext_element,
                        const axutil_env_t *env);

};

struct woden_ext_element
{
    woden_ext_element_ops_t *ops;
};

AXIS2_EXTERN woden_ext_element_t * AXIS2_CALL
woden_ext_element_create(const axutil_env_t *env);

/**
 * This is an Axis2 C internal method. This is used only from constructor
 * of the child class
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_ext_element_resolve_methods(
        woden_ext_element_t *ext_element,
        const axutil_env_t *env,
        axutil_hash_t *methods);

#define WODEN_EXT_ELEMENT_FREE(ext_element, env) \
      (((woden_ext_element_t *) ext_element)->ops->free (ext_element, env))

#define WODEN_EXT_ELEMENT_ELEMENT_TYPE(ext_element, env) \
      (((woden_ext_element_t *) ext_element)->ops->\
         type (ext_element, env))

#define WODEN_EXT_ELEMENT_SET_EXT_TYPE(ext_element, env, qname) \
      (((woden_ext_element_t *) ext_element)->ops->set_ext_type(ext_element, env, \
                                                               qname))

#define WODEN_EXT_ELEMENT_GET_EXT_TYPE(ext_element, env) \
      (((woden_ext_element_t *) ext_element)->ops->get_ext_type(ext_element, env))

#define WODEN_EXT_ELEMENT_SET_REQUIRED(ext_element, env, required) \
      (((woden_ext_element_t *) ext_element)->ops->set_required(ext_element, env, \
                                                             required))

#define WODEN_EXT_ELEMENT_IS_REQUIRED(ext_element, env) \
      (((woden_ext_element_t *) ext_element)->ops->is_required(ext_element, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_EXT_ELEMENT_H */
