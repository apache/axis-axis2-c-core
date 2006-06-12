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

#ifndef AXIS2_WODEN_EXT_DESERIALIZER_H
#define AXIS2_WODEN_EXT_DESERIALIZER_H

/**
 * @file axis2_woden_ext_deserializer.h
 * @brief Axis2 Extension Deserializer Interface
 * This interface represents a &lt;ext_deserializer&gt; XML element 
 * information item. It declares the behaviour required to support 
 * parsing, creating and manipulating a &lt;ext_deserializer&gt; element.
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_qname.h>
#include <axis2_hash.h>
#include <axis2_om_node.h>
#include <woden/axis2_woden.h>
#include <woden/wsdl20/extensions/axis2_woden_ext_registry.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_ext_deserializer axis2_woden_ext_deserializer_t;
typedef struct axis2_woden_ext_deserializer_ops axis2_woden_ext_deserializer_ops_t;

/** @defgroup axis2_woden_ext_deserializer Extension Deserializer
  * @ingroup axis2_ext_deserializer
  * @{
  */

struct axis2_woden_ext_deserializer_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *ext_deserializer,
            const axis2_env_t *env);
 
    axis2_woden_obj_types_t (AXIS2_CALL *
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
            axis2_om_node_t *ext_el_node,
            void *desc,
            axis2_woden_ext_registry_t *ext_reg);

};

struct axis2_woden_ext_deserializer
{
    axis2_woden_ext_deserializer_ops_t *ops;
};

AXIS2_EXTERN axis2_woden_ext_deserializer_t * AXIS2_CALL
axis2_woden_ext_deserializer_create(
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_woden_ext_deserializer_resolve_methods(
        axis2_woden_ext_deserializer_t *ext_deserializer,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_EXT_DESERIALIZER_FREE(ext_deserializer, env) \
      (((axis2_woden_ext_deserializer_t *) ext_deserializer)->ops->\
         free (ext_deserializer, env))

#define AXIS2_WODEN_EXT_DESERIALIZER_TYPE(ext_deserializer, env) \
      (((axis2_woden_ext_deserializer_t *) ext_deserializer)->ops->\
         type (ext_deserializer, env))

#define AXIS2_WODEN_EXT_DESERIALIZER_UNMARSHALL(ext_deserializer, env, \
        parent_type, parent, ext_type, ext_el_node, desc, ext_reg) \
      (((axis2_woden_ext_deserializer_t *) ext_deserializer)->ops->\
         unmarshall (ext_deserializer, env, parent_type, parent, ext_type, \
             ext_el_node, desc, ext_reg))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_EXT_DESERIALIZER_H */
