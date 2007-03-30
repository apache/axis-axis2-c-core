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

#ifndef WODEN_SOAP_HEADER_BLOCK_H
#define WODEN_SOAP_HEADER_BLOCK_H

/**
 * @file woden_soap_header_block.h
 * @brief Axis2 Soap Header Block Interface
 * This interface represents the &lt;wsoap:header&gt; extension element that 
 * can appear within a Binding Fault or Binding Message Reference.
 * 
 */

#include <woden.h>
#include <woden_soap_header_block_element.h>

/** @defgroup woden_soap_header_block Soap Header Block
  * @ingroup woden
  * @{
  */

typedef union woden_soap_header_block_base woden_soap_header_block_base_t;
typedef struct woden_soap_header_block woden_soap_header_block_t;
typedef struct woden_soap_header_block_ops woden_soap_header_block_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_soap_header_block_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL * 
    free) (
            void *header_block,
            const axutil_env_t *env);

    axis2_hash_t *(AXIS2_CALL * 
    super_objs) (
            void *header_block,
            const axutil_env_t *env);

    woden_obj_types_t (AXIS2_CALL * 
    type) (
            void *header_block,
            const axutil_env_t *env);

    struct woden_component_exts *(AXIS2_CALL *
    get_base_impl) (
            void *header_block,
            const axutil_env_t *env);


    /* ***********************************************************************
     *  Component model methods (SOAPHeaderBlock interface), some shared with Element model
     * ***********************************************************************/

    void *(AXIS2_CALL *
    get_element_decl) (
            void *header_block,
            const axutil_env_t *env);

    axis2_bool_t (AXIS2_CALL * 
    must_understand) (
            void *header_block,
            const axutil_env_t *env);

    axis2_bool_t (AXIS2_CALL * 
    is_required) (
            void *header_block,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    get_parent) (
            void *header_block,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    to_element) (
            void *header_block,
            const axutil_env_t *env);

    /* ************************************************************
     *  Non-API implementation methods
     * ************************************************************/

    axis2_status_t (AXIS2_CALL * 
    set_element_decl) (
            void *header_block,
            const axutil_env_t *env,
            void *element_decl);

    axis2_status_t (AXIS2_CALL * 
    set_types) (
            void *header_block,
            const axutil_env_t *env,
            void *types);


    
};

struct woden_soap_header_block
{
    woden_soap_header_block_element_t header_block_element;
    woden_soap_header_block_ops_t *ops;
};

AXIS2_EXTERN woden_soap_header_block_t * AXIS2_CALL
woden_soap_header_block_create(
        const axutil_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_soap_header_block_t * AXIS2_CALL
woden_soap_header_block_to_soap_header_block_element(
        void *header_block,
        const axutil_env_t *env);

AXIS2_EXTERN woden_soap_header_block_t * AXIS2_CALL
woden_soap_header_block_to_ext_element(
        void *header_block,
        const axutil_env_t *env);

AXIS2_EXTERN woden_soap_header_block_t * AXIS2_CALL
woden_soap_header_block_to_attr_extensible(
        void *header_block,
        const axutil_env_t *env);

AXIS2_EXTERN woden_soap_header_block_t * AXIS2_CALL
woden_soap_header_block_to_element_extensible(
        void *header_block,
        const axutil_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_soap_header_block_resolve_methods(
        woden_soap_header_block_t *soap_header_block,
        const axutil_env_t *env,
        woden_soap_header_block_t *soap_header_block_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_SOAP_HEADER_BLOCK_FREE(soap_header_block, env) \
      (((woden_soap_header_block_t *) soap_header_block)->ops->\
       free(soap_header_block, env))

#define WODEN_SOAP_HEADER_BLOCK_SUPER_OBJS(soap_header_block, env) \
      (((woden_soap_header_block_t *) soap_header_block)->\
       ops->super_objs(soap_header_block, env))

#define WODEN_SOAP_HEADER_BLOCK_TYPE(soap_header_block, env) \
      (((woden_soap_header_block_t *) soap_header_block)->ops->\
       type(soap_header_block, env))

#define WODEN_SOAP_HEADER_BLOCK_GET_BASE_IMPL(soap_header_block, env) \
      (((woden_soap_header_block_t *) soap_header_block)->ops->\
       get_base_impl(soap_header_block, env))

#define WODEN_SOAP_HEADER_BLOCK_GET_ELEMENT_DECL(soap_header_block, env) \
      (((woden_soap_header_block_t *) soap_header_block)->ops->\
        get_element_decl (soap_header_block, env))

#define WODEN_SOAP_HEADER_BLOCK_MUST_UNDERSTAND(soap_header_block, env) \
      (((woden_soap_header_block_t *) soap_header_block)->ops->\
        must_understand (soap_header_block, env))

#define WODEN_SOAP_HEADER_BLOCK_IS_REQUIRED(soap_header_block, env) \
      (((woden_soap_header_block_t *) soap_header_block)->ops->\
        is_required (soap_header_block, env))

#define WODEN_SOAP_HEADER_BLOCK_GET_PARENT(soap_header_block, env) \
      (((woden_soap_header_block_t *) soap_header_block)->ops->\
        get_parent (soap_header_block, env))

#define WODEN_SOAP_HEADER_BLOCK_TO_ELEMENT(soap_header_block, env) \
      (((woden_soap_header_block_t *) soap_header_block)->ops->\
        to_element (soap_header_block, env))

#define WODEN_SOAP_HEADER_BLOCK_SET_ELEMENT_DECL(soap_header_block, env, element_decl) \
      (((woden_soap_header_block_t *) soap_header_block)->ops->\
        set_element_decl (soap_header_block, env, element_decl))

#define WODEN_SOAP_HEADER_BLOCK_SET_TYPES(soap_header_block, env, types) \
      (((woden_soap_header_block_t *) soap_header_block)->ops->\
        set_types (soap_header_block, env, types))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_SOAP_HEADER_BLOCK_H */
