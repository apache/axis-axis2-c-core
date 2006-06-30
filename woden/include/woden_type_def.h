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

#ifndef WODEN_TYPE_DEF_H
#define WODEN_TYPE_DEF_H

/**
 * @file woden_type_def.h
 * @brief Axis2 Type Definition Interface
 * This class implements support for parsing, creating and manipulating a
 * WSDL 2.0 &lt;wsdl:type_def&gt; XML element.
 * The &lt;wsdl:type_def&gt; element may contain mixed content, but this 
 * class does not attempt to understand that content. Instead it just wraps
 * the &lt;wsdl:type_def&gt; element's content model as a void *. 
 * 
 */

#include <woden.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_uri.h>

/** @defgroup woden_type_def Type Definition
  * @ingroup woden
  * @{
  */

typedef union woden_type_def_base woden_type_def_base_t;
typedef struct woden_type_def woden_type_def_t;
typedef struct woden_type_def_ops woden_type_def_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_type_def_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *type_def,
            const axis2_env_t *env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *type_def,
            const axis2_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (void *type_def,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    void *(AXIS2_CALL *
    get_base_impl) (
            void *type_def,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_qname) (
            void *type_def,
            const axis2_env_t *env,
            axis2_qname_t *qname);

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *type_def,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_system) (
            void *type_def,
            const axis2_env_t *env,
            axis2_uri_t *type_system_uri);

    axis2_uri_t *(AXIS2_CALL *
    get_system) (
            void *type_def,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_content_model) (
            void *type_def,
            const axis2_env_t *env,
            axis2_char_t *content_model);

    axis2_char_t *(AXIS2_CALL *
    get_content_model) (
            void *type_def,
            const axis2_env_t *env);


    axis2_status_t (AXIS2_CALL *
    set_content) (
            void *type_def,
            const axis2_env_t *env,
            void *type_def_content);

    void *(AXIS2_CALL *
    get_content) (
            void *type_def,
            const axis2_env_t *env);

};

struct woden_type_def
{
    woden_type_def_ops_t *ops;
};

AXIS2_EXTERN woden_type_def_t * AXIS2_CALL
woden_type_def_create(
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_type_def_t * AXIS2_CALL
woden_type_def_to_type_def_element(
        void *type_def,
        const axis2_env_t *env);

AXIS2_EXTERN woden_type_def_t * AXIS2_CALL
woden_type_def_to_attr_extensible(
        void *type_def,
        const axis2_env_t *env);

AXIS2_EXTERN woden_type_def_t * AXIS2_CALL
woden_type_def_to_element_extensible(
        void *type_def,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_type_def_resolve_methods(
        woden_type_def_t *type_def,
        const axis2_env_t *env,
        woden_type_def_t *type_def_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/


#define WODEN_TYPE_DEF_FREE(type_def, env) \
      (((woden_type_def_t *) type_def)->ops->free(type_def, env))

#define WODEN_TYPE_DEF_SUPER_OBJS(type_def, env) \
      (((woden_type_def_t *) type_def)->ops->\
         super_objs(type_def, env))

#define WODEN_TYPE_DEF_TYPE(type_def, env) \
      (((woden_type_def_t *) type_def)->ops->type(type_def, env))

#define WODEN_TYPE_DEF_GET_BASE_IMPL(type_def, env) \
      (((woden_type_def_t *) type_def)->ops->\
         get_base_impl(type_def, env))

#define WODEN_TYPE_DEF_SET_QNAME(type_def, env, qname) \
      (((woden_type_def_t *) type_def)->ops->\
         set_qname(type_def, env, qname))

#define WODEN_TYPE_DEF_GET_QNAME(type_def, env) \
      (((woden_type_def_t *) type_def)->ops->\
         get_qname(type_def, env))

#define WODEN_TYPE_DEF_SET_SYSTEM(type_def, env, type_system_uri) \
      (((woden_type_def_t *) type_def)->ops->\
         set_system(type_def, env, type_system_uri))

#define WODEN_TYPE_DEF_GET_SYSTEM(type_def, env) \
      (((woden_type_def_t *) type_def)->ops->\
         get_system(type_def, env))

#define WODEN_TYPE_DEF_SET_CONTENT_MODEL(type_def, env, content_model) \
      (((woden_type_def_t *) type_def)->ops->\
         set_content_model(type_def, env, content_model))

#define WODEN_TYPE_DEF_GET_CONTENT_MODEL(type_def, env) \
      (((woden_type_def_t *) type_def)->ops->\
         get_content_model(type_def, env))

#define WODEN_TYPE_DEF_SET_CONTENT(type_def, env, type_def_content) \
      (((woden_type_def_t *) type_def)->ops->\
         set_content(type_def, env, type_def_content))

#define WODEN_TYPE_DEF_GET_CONTENT(type_def, env) \
      (((woden_type_def_t *) type_def)->ops->\
         get_content(type_def, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_TYPE_DEF_H */
