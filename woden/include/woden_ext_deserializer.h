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

#ifndef WODEN_EXT_DESERIALIZER_H
#define WODEN_EXT_DESERIALIZER_H

/**
 * @file woden_ext_deserializer.h
 * @brief Axis2 Extension Deserializer Interface
 * This interface represents a &lt;ext_deserializer&gt; XML element 
 * information item. It declares the behaviour required to support 
 * parsing, creating and manipulating a &lt;ext_deserializer&gt; element.
 */

#include <axutil_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_qname.h>
#include <axis2_hash.h>
#include <axiom_node.h>
#include <woden.h>
#include <woden_ext_registry.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_ext_deserializer woden_ext_deserializer_t;
typedef struct woden_ext_deserializer_ops woden_ext_deserializer_ops_t;

/** @defgroup woden_ext_deserializer Extension Deserializer
  * @ingroup axis2_ext_deserializer
  * @{
  */

struct woden_ext_deserializer_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *ext_deserializer,
            const axis2_env_t *env);
 
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *ext_deserializer,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    unmarshall) (
            void *ext_deserializer,
            const axis2_env_t *env,
            axis2_char_t *parent_type,
            void *parent,
            axis2_qname_t *ext_type,
            axiom_node_t *ext_el_node,
            void *desc,
            woden_ext_registry_t *ext_reg);

};

struct woden_ext_deserializer
{
    woden_ext_deserializer_ops_t *ops;
};

AXIS2_EXTERN woden_ext_deserializer_t * AXIS2_CALL
woden_ext_deserializer_create(
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_ext_deserializer_resolve_methods(
        woden_ext_deserializer_t *ext_deserializer,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_EXT_DESERIALIZER_FREE(ext_deserializer, env) \
      (((woden_ext_deserializer_t *) ext_deserializer)->ops->\
         free (ext_deserializer, env))

#define WODEN_EXT_DESERIALIZER_TYPE(ext_deserializer, env) \
      (((woden_ext_deserializer_t *) ext_deserializer)->ops->\
         type (ext_deserializer, env))

#define WODEN_EXT_DESERIALIZER_UNMARSHALL(ext_deserializer, env, \
        parent_type, parent, ext_type, ext_el_node, desc, ext_reg) \
      (((woden_ext_deserializer_t *) ext_deserializer)->ops->\
         unmarshall (ext_deserializer, env, parent_type, parent, ext_type, \
             ext_el_node, desc, ext_reg))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_EXT_DESERIALIZER_H */
