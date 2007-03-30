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

#ifndef WODEN_SOAP_HEADER_BLOCK_DESERIALIZER_H
#define WODEN_SOAP_HEADER_BLOCK_DESERIALIZER_H

/**
 * @file woden_soap_header_block_deserializer.h
 * @brief Axis2 Soap Header Block Deserializer Soap Header Block Deserializer
 * Deserializes the &lt;wsoap:header_block&gt; extension element into a 
 * soap_header_block_element.
 * 
 */

#include <woden.h>
#include <woden_ext_deserializer.h>
#include <woden_ext_registry.h>
#include <axiom_node.h>
#include <axis2_qname.h>

/** @defgroup woden_soap_header_block_deserializer Soap Header Block Deserializer
  * @ingroup woden
  * @{
  */

typedef struct woden_soap_header_block_deserializer 
        woden_soap_header_block_deserializer_t;
typedef struct woden_soap_header_block_deserializer_ops 
        woden_soap_header_block_deserializer_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct woden_soap_header_block_deserializer_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *mod_deser,
            const axutil_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *mod_deser,
            const axutil_env_t *env);

    woden_obj_types_t (AXIS2_CALL *
    type) (void *mod_deser,
            const axutil_env_t *env);
    /**
     * @return the base implementation class
     */
    struct woden_ext_deserializer *(AXIS2_CALL *
    get_base_impl) (
            void *mod_deser,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    marshall) (
            void *mod_deser,
            const axutil_env_t *env,
            axis2_char_t *parent_type,
            void *parent,
            axis2_qname_t *element_type,
            axiom_node_t *el_node,
            void *desc,
            woden_ext_registry_t *ext_reg);

};

struct woden_soap_header_block_deserializer
{
    woden_ext_deserializer_t ext_deserializer;
    woden_soap_header_block_deserializer_ops_t *ops;
};

AXIS2_EXTERN woden_soap_header_block_deserializer_t * AXIS2_CALL
woden_soap_header_block_deserializer_create(
        const axutil_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_soap_header_block_deserializer_t * AXIS2_CALL
woden_soap_header_block_deserializer_to_ext_deserializer(
        void *mod_deser,
        const axutil_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_soap_header_block_deserializer_resolve_methods(
        woden_soap_header_block_deserializer_t *mod_deser,
        const axutil_env_t *env,
        woden_soap_header_block_deserializer_t *mod_deser_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_SOAP_HEADER_BLOCK_DESERIALIZER_FREE(mod_deser, env) \
      (((woden_soap_header_block_deserializer_t *) mod_deser)->ops->free(mod_deser, env))

#define WODEN_SOAP_HEADER_BLOCK_DESERIALIZER_SUPER_OBJS(mod_deser, env) \
      (((woden_soap_header_block_deserializer_t *) mod_deser)->ops->super_objs(mod_deser, env))

#define WODEN_SOAP_HEADER_BLOCK_DESERIALIZER_TYPE(mod_deser, env) \
      (((woden_soap_header_block_deserializer_t *) mod_deser)->ops->type(mod_deser, env))

#define WODEN_SOAP_HEADER_BLOCK_DESERIALIZER_GET_BASE_IMPL(mod_deser, env) \
      (((woden_soap_header_block_deserializer_t *) mod_deser)->ops->get_base_impl(mod_deser, env))

#define WODEN_SOAP_HEADER_BLOCK_DESERIALIZER_MARSHALL(mod_deser, env, \
        parent_type, parent, element_type, el_node, desc, ext_reg) \
      (((woden_soap_header_block_deserializer_t *) mod_deser)->\
         marshall(mod_deser, env, parent_type, parent, element_type, el_node, \
             desc, ext_reg))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_SOAP_HEADER_BLOCK_DESERIALIZER_H */
