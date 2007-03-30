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

#ifndef WODEN_SCHEMA_H
#define WODEN_SCHEMA_H

/**
 * @file woden_schema.h
 * @brief Axis2 Schema Interface
 *          Abstract implementation of an XML Schema.
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_uri.h>
#include <woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_schema woden_schema_t;
typedef struct woden_schema_ops woden_schema_ops_t;
struct xml_schema;

/** @defgroup woden_schema Schema
  * @ingroup woden
  * @{
  */

struct woden_schema_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *schema,
            const axutil_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *schema,
            const axutil_env_t *env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *schema,
            const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_namespace) (
            void *schema,
            const axutil_env_t *env,
            axis2_uri_t *namespc);

    axis2_uri_t *(AXIS2_CALL *
    get_namespace) (
            void *schema,
            const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_schema_def) (
            void *schema,
            const axutil_env_t *env,
            struct xml_schema *schema_def);

    struct xml_schema *(AXIS2_CALL *
    get_schema_def) (
            void *schema,
            const axutil_env_t *env);

    /* ************************************************************
     *  Non-API implementation methods
     * ************************************************************/

    /* 
     * Indicates whether the schema is referenceable by the containing WSDL, as
     * defined by the WSDL 2.0 spec. In brief, a schema is referenceable if it is 
     * inlined directly under &lt;types&gt; and has a target namespace or if it is
     * resolved from a schema import directly under &lt;types&gt; and the import 
     * namespace matches the schema's target namespace.
     * So for example, when the API is used to retrieve the schema element 
     * declaration identified by a QName attribute, the implementation checks this
     * referenceable property to determine which schemas can be used to resolve the 
     * qname. 
     * Referenceability is determined during validation. If the validation feature
     * is disabled, any inlined or imported schema will be considered referenceable.
     */
    axis2_status_t (AXIS2_CALL *
    set_referenceable) (
            void *schema,
            const axutil_env_t *env,
            axis2_bool_t referenceable);

    axis2_bool_t (AXIS2_CALL *
    is_referenceable) (
            void *schema,
            const axutil_env_t *env);

    axis2_char_t *(AXIS2_CALL *
    get_namespace_as_string) (
            void *schema,
            const axutil_env_t *env);

  
};

struct woden_schema
{
    woden_schema_ops_t *ops;
};

AXIS2_EXTERN woden_schema_t * AXIS2_CALL
woden_schema_create(const axutil_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_schema_resolve_methods(
        woden_schema_t *schema,
        const axutil_env_t *env,
        woden_schema_t *schema_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_SCHEMA_FREE(schema, env) \
      (((woden_schema_t *) schema)->ops->free (schema, env))

#define WODEN_SCHEMA_SUPER_OBJS(schema, env) \
      (((woden_schema_t *) schema)->ops->super_objs (schema, env))

#define WODEN_SCHEMA_TYPE(schema, env) \
      (((woden_schema_t *) schema)->ops->type (schema, env))

#define WODEN_SCHEMA_SET_NAMESPACE(schema, env, namespc) \
      (((woden_schema_t *) schema)->ops->set_namespace(schema, env, \
                                                               namespc))

#define WODEN_SCHEMA_GET_NAMESPACE(schema, env) \
      (((woden_schema_t *) schema)->ops->get_namespace(schema, env))

#define WODEN_SCHEMA_SET_SCHEMA_DEF(schema, env, schema_def) \
      (((woden_schema_t *) schema)->ops->set_schema_def(schema, env, \
                                                                schema_def))

#define WODEN_SCHEMA_GET_SCHEMA_DEF(schema, env) \
      (((woden_schema_t *) schema)->ops->get_schema_def(schema, env))

#define WODEN_SCHEMA_SET_REFERENCEABLE(schema, env, referenceable) \
      (((woden_schema_t *) schema)->ops->set_referenceable(schema, env, \
                                                             referenceable))

#define WODEN_SCHEMA_IS_REFERENCEABLE(schema, env) \
      (((woden_schema_t *) schema)->ops->is_referenceable(schema, env))

#define WODEN_SCHEMA_GET_NAMESPACE_AS_STRING(schema, env) \
      (((woden_schema_t *) schema)->ops->get_namespace_as_string(schema, \
                                                                         env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_SCHEMA_H */
