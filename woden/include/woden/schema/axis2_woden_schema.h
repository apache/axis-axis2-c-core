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

#ifndef AXIS2_WODEN_SCHEMA_H
#define AXIS2_WODEN_SCHEMA_H

/**
 * @file axis2_woden_schema.h
 * @brief Axis2 Schema Interface
 *          Abstract implementation of an XML Schema.
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_uri.h>
#include <woden/axis2_woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_schema axis2_woden_schema_t;
typedef struct axis2_woden_schema_ops axis2_woden_schema_ops_t;
struct axis2_xml_schema;

/** @defgroup axis2_woden_schema Schema
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_schema_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *schema,
            const axis2_env_t *env);
    
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *schema,
            const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_namespace) (
            void *schema,
            const axis2_env_t *env,
            axis2_uri_t *namespc);

    axis2_uri_t *(AXIS2_CALL *
    get_namespace) (
            void *schema,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_schema_def) (
            void *schema,
            const axis2_env_t *env,
            struct axis2_xml_schema *schema_def);

    struct axis2_xml_schema *(AXIS2_CALL *
    get_schema_def) (
            void *schema,
            const axis2_env_t *env);

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
            const axis2_env_t *env,
            axis2_bool_t referenceable);

    axis2_bool_t (AXIS2_CALL *
    is_referenceable) (
            void *schema,
            const axis2_env_t *env);

    axis2_char_t *(AXIS2_CALL *
    get_namespace_as_string) (
            void *schema,
            const axis2_env_t *env);

  
};

struct axis2_woden_schema
{
    axis2_woden_schema_ops_t *ops;
};

AXIS2_EXTERN axis2_woden_schema_t * AXIS2_CALL
axis2_woden_schema_create(const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_woden_schema_resolve_methods(
        axis2_woden_schema_t *schema,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_SCHEMA_FREE(schema, env) \
      (((axis2_woden_schema_t *) schema)->ops->free (schema, env))

#define AXIS2_WODEN_SCHEMA_TYPE(schema, env) \
      (((axis2_woden_schema_t *) schema)->ops->type (schema, env))

#define AXIS2_WODEN_SCHEMA_SET_NAMESPACE(schema, env, namespc) \
      (((axis2_woden_schema_t *) schema)->ops->set_namespace(schema, env, \
                                                               namespc))

#define AXIS2_WODEN_SCHEMA_GET_NAMESPACE(schema, env) \
      (((axis2_woden_schema_t *) schema)->ops->get_namespace(schema, env))

#define AXIS2_WODEN_SCHEMA_SET_SCHEMA_DEF(schema, env, schema_def) \
      (((axis2_woden_schema_t *) schema)->ops->set_schema_def(schema, env, \
                                                                schema_def))

#define AXIS2_WODEN_SCHEMA_GET_SCHEMA_DEF(schema, env) \
      (((axis2_woden_schema_t *) schema)->ops->get_schema_def(schema, env))

#define AXIS2_WODEN_SCHEMA_SET_REFERENCEABLE(schema, env, referenceable) \
      (((axis2_woden_schema_t *) schema)->ops->set_referenceable(schema, env, \
                                                             referenceable))

#define AXIS2_WODEN_SCHEMA_IS_REFERENCEABLE(schema, env) \
      (((axis2_woden_schema_t *) schema)->ops->is_referenceable(schema, env))

#define AXIS2_WODEN_SCHEMA_GET_NAMESPACE_AS_STRING(schema, env) \
      (((axis2_woden_schema_t *) schema)->ops->get_namespace_as_string(schema, \
                                                                         env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_SCHEMA_H */
