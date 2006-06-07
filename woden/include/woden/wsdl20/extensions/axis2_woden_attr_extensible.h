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

#ifndef AXIS2_WODEN_ATTR_EXTENSIBLE_H
#define AXIS2_WODEN_ATTR_EXTENSIBLE_H

/**
 * @file axis2_woden_attr_extensible.h
 * @brief Axis2 Attribute Extensible Interface
 * Common code for handling extension attributes. 
 * Can be reused by inheritance or by delegation.
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_uri.h>
#include <axis2_array_list.h>
#include <woden/axis2_woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_attr_extensible axis2_woden_attr_extensible_t;
typedef struct axis2_woden_attr_extensible_ops axis2_woden_attr_extensible_ops_t;
struct axis2_woden_xml_attr;

/** @defgroup axis2_woden_attr_extensible Attribute Extensible
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_attr_extensible_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *attr_extensible,
            const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    to_attr_extensible_free) (
            void *attr_extensible,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *attr_extensible,
            const axis2_env_t *env);
    
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *attr_extensible,
            const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_ext_attr) (
            void *extensible,
            const axis2_env_t *env,
            axis2_qname_t *attr_type,
            struct axis2_woden_xml_attr *attr); 

    void *(AXIS2_CALL *
    get_ext_attr) (
            void *extensible,
            const axis2_env_t *env,
            axis2_qname_t *attr_type); 

    axis2_array_list_t *(AXIS2_CALL *
    get_ext_attrs) (
            void *extensible,
            const axis2_env_t *env); 

    axis2_array_list_t *(AXIS2_CALL *
    get_ext_attrs_for_namespace) (
            void *extensible,
            const axis2_env_t *env,
            axis2_uri_t *namespc);

    axis2_bool_t (AXIS2_CALL *
    has_ext_attrs_for_namespace) (
            void *extensible,
            const axis2_env_t *env,
            axis2_uri_t *namespc);


};

struct axis2_woden_attr_extensible
{
    axis2_woden_attr_extensible_ops_t *ops;
};

AXIS2_EXTERN axis2_woden_attr_extensible_t * AXIS2_CALL
axis2_woden_attr_extensible_create(const axis2_env_t *env);

/**
 * This is an Axis2 C internal method. This is used only from constructor
 * of the child class
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_woden_attr_extensible_resolve_methods(
        axis2_woden_attr_extensible_t *extensible,
        const axis2_env_t *env,
        axis2_woden_attr_extensible_t *extensible_impl,
        axis2_hash_t *methods);

#define AXIS2_WODEN_ATTR_EXTENSIBLE_FREE(extensible, env) \
      (((axis2_woden_attr_extensible_t *) extensible)->ops->\
         free (extensible, env))

#define AXIS2_WODEN_ATTR_EXTENSIBLE_TO_ATTR_EXTENSIBLE_FREE(extensible, env) \
      (((axis2_woden_attr_extensible_t *) extensible)->ops->\
         to_attr_extensible_free (extensible, env))

#define AXIS2_WODEN_ATTR_EXTENSIBLE_SUPER_OBJS(extensible, env) \
      (((axis2_woden_attr_extensible_t *) extensible)->ops->\
         super_objs (extensible, env))

#define AXIS2_WODEN_ATTR_EXTENSIBLE_TYPE(extensible, env) \
      (((axis2_woden_attr_extensible_t *) extensible)->ops->\
         type (extensible, env))

#define AXIS2_WODEN_ATTR_EXTENSIBLE_SET_EXT_ATTR(extensible, env, attr_type, \
        attr) \
      (((axis2_woden_attr_extensible_t *) extensible)->ops->\
         set_ext_attr(extensible, env, attr_type, attr))

#define AXIS2_WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTR(extensible, env, \
        attr_type) \
      (((axis2_woden_attr_extensible_t *) extensible)->ops->\
         get_ext_attr(extensible, env, attr_type))

#define AXIS2_WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTRS(extensible, env) \
      (((axis2_woden_attr_extensible_t *) extensible)->ops->\
         get_ext_attrs(extensible, env))

#define AXIS2_WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTRS_FOR_NAMESPACE(extensible, \
        env, namespc) \
      (((axis2_woden_attr_extensible_t *) extensible)->ops->\
         get_ext_attrs_for_namespace(extensible, env, namespc))

#define AXIS2_WODEN_ATTR_EXTENSIBLE_HAS_EXT_ATTRS_FOR_NAMESPACE(\
        extensible, env, namespc) \
      (((axis2_woden_attr_extensible_t *) extensible)->ops->\
         has_ext_attrs_for_namespace(extensible, env, namespc))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_ATTR_EXTENSIBLE_H */
