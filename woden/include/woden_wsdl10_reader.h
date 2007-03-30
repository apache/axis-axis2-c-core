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

#ifndef WODEN_WSDL10_READER_H
#define WODEN_WSDL10_READER_H

/**
 * @file woden_wsdl10_reader.h
 * @brief Woden Wsdl Reader Interface
 * Implements the wsdl_reader behaviour for AXIOM-based parsing.
 * 
 */

#include <woden.h>
#include <woden_wsdl10_ext_registry.h>
#include <axiom_document.h>

/** @defgroup woden_wsdl10_reader Woden Wsdl Reader
  * @ingroup woden
  * @{
  */

typedef union woden_wsdl10_reader_base woden_wsdl10_reader_base_t;
typedef struct woden_wsdl10_reader woden_wsdl10_reader_t;
typedef struct woden_wsdl10_reader_ops woden_wsdl10_reader_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_wsdl10_reader_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *reader,
            const axutil_env_t *env);
 
    void *(AXIS2_CALL *
    read_wsdl) (
            void *reader,
            const axutil_env_t *env,
            axiom_node_t *root_node,
            const axis2_char_t *uri);

    axis2_status_t (AXIS2_CALL *
    set_ext_registry) (
            void *reader,
            const axutil_env_t *env,
            woden_wsdl10_ext_registry_t *ext_reg);

    woden_wsdl10_ext_registry_t *(AXIS2_CALL *
    get_ext_registry) (
            void *reader,
            const axutil_env_t *env);

   
};

struct woden_wsdl10_reader
{
    woden_wsdl10_reader_ops_t *ops;
};

AXIS2_EXTERN woden_wsdl10_reader_t * AXIS2_CALL
woden_wsdl10_reader_create(
        const axutil_env_t *env);

#define WODEN_WSDL10_READER_FREE(reader, env) \
      (((woden_wsdl10_reader_t *) reader)->ops->free(reader, env))

#define WODEN_WSDL10_READER_READ_WSDL(reader, env, root_node, uri) \
      (((woden_wsdl10_reader_t *) reader)->ops->\
      read_wsdl (reader, env, root_node, uri))

#define WODEN_WSDL10_READER_SET_EXT_REGISTRY(reader, env, ext_reg) \
      (((woden_wsdl10_reader_t *) reader)->ops->\
      set_ext_registry (reader, env, ext_reg))

#define WODEN_WSDL10_READER_GET_EXT_REGISTRY(reader, env) \
      (((woden_wsdl10_reader_t *) reader)->ops->\
       get_ext_registry(reader, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_WSDL10_READER_H */
