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

#ifndef WODEN_WSDL_ELEMENT_H
#define WODEN_WSDL_ELEMENT_H

/**
 * @file woden_wsdl_element.h
 * @brief Axis2 Wsdl Element Interface
 * This abstract class defines the behaviour common to all WSDL elements.
 * That is, it implements support for extension attributes and elements.
 * This interface can be used as a common reference for all WSDL elements
 * represented by the Element API.
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_uri.h>
#include <axutil_array_list.h>
#include <woden.h>
#include <woden_attr_extensible.h>
#include <woden_element_extensible.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_wsdl_element woden_wsdl_element_t;
typedef struct woden_wsdl_element_ops woden_wsdl_element_ops_t;
typedef union woden_wsdl_element_base woden_wsdl_element_base_t;
struct woden_xml_attr;
struct woden_ext_element;

/** @defgroup woden_wsdl_element Wsdl Element
  * @ingroup woden
  * @{
  */

struct woden_wsdl_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *wsdl_element,
            const axutil_env_t *env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *wsdl_element,
            const axutil_env_t *env);
 
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *wsdl_element,
            const axutil_env_t *env);
 
    void *(AXIS2_CALL *
    get_element_extensible) (
            void *wsdl_element,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    get_attr_extensible) (
            void *wsdl_element,
            const axutil_env_t *env);

   
};

union woden_wsdl_element_base
{
    woden_attr_extensible_t attr_extensible;
    woden_element_extensible_t element_extensible;
};

struct woden_wsdl_element
{
    woden_wsdl_element_base_t base;
    woden_wsdl_element_ops_t *ops;
};

AXIS2_EXTERN woden_wsdl_element_t * AXIS2_CALL
woden_wsdl_element_create(
        const axutil_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_wsdl_element_t * AXIS2_CALL
woden_wsdl_element_to_attr_extensible(
        void *wsdl_element,
        const axutil_env_t *env);

AXIS2_EXTERN woden_wsdl_element_t * AXIS2_CALL
woden_wsdl_element_to_element_extensible(
        void *wsdl_element,
        const axutil_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_wsdl_element_resolve_methods(
        woden_wsdl_element_t *wsdl_element,
        const axutil_env_t *env,
        woden_wsdl_element_t *wsdl_element_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_WSDL_ELEMENT_FREE(wsdl_element, env) \
      (((woden_wsdl_element_t *) wsdl_element)->ops->\
         free (wsdl_element, env))

#define WODEN_WSDL_ELEMENT_SUPER_OBJS(wsdl_element, env) \
      (((woden_wsdl_element_t *) wsdl_element)->ops->\
         super_objs (wsdl_element, env))

#define WODEN_WSDL_ELEMENT_TYPE(wsdl_element, env) \
      (((woden_wsdl_element_t *) wsdl_element)->ops->\
         type (wsdl_element, env))

#define WODEN_WSDL_ELEMENT_GET_ELEMENT_EXTENSIBLE(wsdl_element, env) \
      (((woden_wsdl_element_t *) wsdl_element)->ops->\
         get_element_extensible (wsdl_element, env))

#define WODEN_WSDL_ELEMENT_GET_ATTR_EXTENSIBLE(wsdl_element, env) \
      (((woden_wsdl_element_t *) wsdl_element)->ops->\
         get_attr_extensible (wsdl_element, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_WSDL_ELEMENT_H */
