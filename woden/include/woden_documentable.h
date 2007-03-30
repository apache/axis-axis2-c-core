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

#ifndef WODEN_DOCUMENTABLE_H
#define WODEN_DOCUMENTABLE_H

/**
 * @file woden_documentable.h
 * @brief Axis2 Documentable Interface
 * An abstract superclass for WSDL 2.0 elements which can have &lt;documentation&gt; 
 * child elements. That is, all WSDL 2.0 elements except the &lt;documentation&gt; 
 * element itself. Defines accessor methods for manipulating DocumentationElements.
 * 
 */

#include <woden.h>
#include <woden_documentable_element.h>
#include <woden_wsdl_obj.h>

/** @defgroup woden_documentable Documentable
  * @ingroup woden
  * @{
  */

typedef union woden_documentable_base woden_documentable_base_t;
typedef struct woden_documentable woden_documentable_t;
typedef struct woden_documentable_ops woden_documentable_ops_t;
struct woden_documentation_element;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_documentable_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *documentable,
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    to_documentable_free) (
            void *documentable,
            const axutil_env_t *env);

    axutil_hash_t *(AXIS2_CALL *
    super_objs) (
            void *documentable,
            const axutil_env_t *env);
    
    /**
     * @return the base implementation class
     */
    woden_wsdl_obj_t *(AXIS2_CALL *
    get_base_impl) (
            void *documentable,
            const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    add_documentation_element) (
            void *types,
            const axutil_env_t *env,
            struct woden_documentation_element *documentation);

    axutil_array_list_t *(AXIS2_CALL *
    get_documentation_elements) (
            void *types,
            const axutil_env_t *env);


};

union woden_documentable_base
{
    woden_wsdl_obj_t wsdl_obj;
    woden_documentable_element_t documentable_element;
};

struct woden_documentable
{
    woden_documentable_base_t base;
    woden_documentable_ops_t *ops;
};

AXIS2_EXTERN woden_documentable_t * AXIS2_CALL
woden_documentable_create(
        const axutil_env_t *env);

/************************Woden C Internal Methods******************************/

AXIS2_EXTERN woden_documentable_t * AXIS2_CALL
woden_documentable_to_documentable_element(
        void *documentable,
        const axutil_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_documentable_resolve_methods(
        woden_documentable_t *documentable,
        const axutil_env_t *env,
        woden_documentable_t *documentable_impl,
        axutil_hash_t *methods);
/************************End of Woden C Internal Methods***********************/


#define WODEN_DOCUMENTABLE_FREE(documentable, env) \
      (((woden_documentable_t *) documentable)->ops->\
         free(documentable, env))

#define WODEN_DOCUMENTABLE_TO_DOCUMENTABLE_FREE(documentable, env) \
      (((woden_documentable_t *) documentable)->ops->\
         to_documentable_free(documentable, env))

#define WODEN_DOCUMENTABLE_SUPER_OBJS(documentable, env) \
      (((woden_documentable_t *) documentable)->ops->\
         super_objs(documentable, env))

#define WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env) \
      (((woden_documentable_t *) documentable)->ops->\
         get_base_impl(documentable, env))

#define WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(documentable, env, \
        documentation) \
      (((woden_documentable_t *) documentable)->ops->\
         add_documentation_element(documentable, env, documentation))

#define WODEN_DOCUMENTABLE_GET_DOCUMENTATION_ELEMENTS(documentable, env) \
      (((woden_documentable_t *) documentable)->ops->\
         get_documentation_elements(documentable, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_DOCUMENTABLE_H */
