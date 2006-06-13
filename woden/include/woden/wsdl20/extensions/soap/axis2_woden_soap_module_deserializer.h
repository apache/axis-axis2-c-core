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

#ifndef AXIS2_WODEN_SOAP_MODULE_DESERIALIZER_H
#define AXIS2_WODEN_SOAP_MODULE_DESERIALIZER_H

/**
 * @file axis2_woden_soap_module_deserializer.h
 * @brief Axis2 Soap Module Deserializer Soap Module Deserializer
 * Deserializes the &lt;wsoap:module&gt; extension element into a 
 * soap_module_element.
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/extensions/axis2_woden_ext_deserializer.h>
#include <woden/wsdl20/extensions/axis2_woden_ext_registry.h>
#include <axiom_node.h>
#include <axis2_qname.h>

/** @defgroup axis2_woden_soap_module_deserializer Soap Module Deserializer
  * @ingroup axis2_wsdl
  * @{
  */

typedef struct axis2_woden_soap_module_deserializer 
        axis2_woden_soap_module_deserializer_t;
typedef struct axis2_woden_soap_module_deserializer_ops 
        axis2_woden_soap_module_deserializer_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_soap_module_deserializer_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *mod_deser,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *mod_deser,
            const axis2_env_t *env);

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *mod_deser,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    struct axis2_woden_ext_deserializer *(AXIS2_CALL *
    get_base_impl) (
            void *mod_deser,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    marshall) (
            void *mod_deser,
            const axis2_env_t *env,
            axis2_char_t *parent_type,
            void *parent,
            axis2_qname_t *element_type,
            axiom_node_t *el_node,
            void *desc,
            axis2_woden_ext_registry_t *ext_reg);

};

struct axis2_woden_soap_module_deserializer
{
    axis2_woden_ext_deserializer_t ext_deserializer;
    axis2_woden_soap_module_deserializer_ops_t *ops;
};

AXIS2_EXTERN axis2_woden_soap_module_deserializer_t * AXIS2_CALL
axis2_woden_soap_module_deserializer_create(
        const axis2_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN axis2_woden_soap_module_deserializer_t * AXIS2_CALL
axis2_woden_soap_module_deserializer_to_ext_deserializer(
        void *mod_deser,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_woden_soap_module_deserializer_resolve_methods(
        axis2_woden_soap_module_deserializer_t *mod_deser,
        const axis2_env_t *env,
        axis2_woden_soap_module_deserializer_t *mod_deser_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_SOAP_MODULE_DESERIALIZER_FREE(mod_deser, env) \
      (((axis2_woden_soap_module_deserializer_t *) mod_deser)->ops->free(mod_deser, env))

#define AXIS2_WODEN_SOAP_MODULE_DESERIALIZER_SUPER_OBJS(mod_deser, env) \
      (((axis2_woden_soap_module_deserializer_t *) mod_deser)->ops->super_objs(mod_deser, env))

#define AXIS2_WODEN_SOAP_MODULE_DESERIALIZER_TYPE(mod_deser, env) \
      (((axis2_woden_soap_module_deserializer_t *) mod_deser)->ops->type(mod_deser, env))

#define AXIS2_WODEN_SOAP_MODULE_DESERIALIZER_GET_BASE_IMPL(mod_deser, env) \
      (((axis2_woden_soap_module_deserializer_t *) mod_deser)->ops->get_base_impl(mod_deser, env))

#define AXIS2_WODEN_SOAP_MODULE_DESERIALIZER_MARSHALL(mod_deser, env, \
        parent_type, parent, element_type, el_node, desc, ext_reg) \
      (((axis2_woden_soap_module_deserializer_t *) mod_deser)->\
         marshall(mod_deser, env, parent_type, parent, element_type, el_node, \
             desc, ext_reg))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_SOAP_MODULE_DESERIALIZER_H */
