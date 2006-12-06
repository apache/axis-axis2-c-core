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

#ifndef WODEN_IMPORTED_SCHEMA_H
#define WODEN_IMPORTED_SCHEMA_H

/**
 * @file woden_imported_schema.h
 * @brief Axis2 Imported Schema Interface
 *          This interface represents a schema import, &lt;xs:import&gt;. It 
 *          extends the abstract class SchemaImpl, adding support for the 
 *          <code>schemaLocation</code> attribute.
 */

#include <woden_schema.h>

/** @defgroup woden_imported_schema Imported Schema
  * @ingroup woden
  * @{
  */

typedef struct woden_imported_schema woden_imported_schema_t;
typedef struct woden_imported_schema_ops woden_imported_schema_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_imported_schema_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *schema,
            const axis2_env_t *env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *schema,
            const axis2_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *schema,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    woden_schema_t *(AXIS2_CALL *
    get_base_impl) (
            void *schema,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_location) (
            void *schema,
            const axis2_env_t *env,
            struct axis2_uri *location);

    struct axis2_uri *(AXIS2_CALL *
    get_location) (
            void *schema,
            const axis2_env_t *env);

  
};

struct woden_imported_schema
{
    woden_schema_t schema;
    woden_imported_schema_ops_t *ops;
};

AXIS2_EXTERN woden_imported_schema_t * AXIS2_CALL
woden_imported_schema_create(
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_imported_schema_t * AXIS2_CALL
woden_imported_schema_to_schema(
        void *schema,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_imported_schema_resolve_methods(
        woden_imported_schema_t *schema,
        const axis2_env_t *env,
        woden_imported_schema_t *schema_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_IMPORTED_SCHEMA_FREE(schema, env) \
      (((woden_imported_schema_t *) schema)->ops->free(schema, env))

#define WODEN_IMPORTED_SCHEMA_SUPER_OBJS(schema, env) \
      (((woden_imported_schema_t *) schema)->ops->super_objs(schema, env))

#define WODEN_IMPORTED_SCHEMA_TYPE(schema, env) \
      (((woden_imported_schema_t *) schema)->ops->type(schema, env))

#define WODEN_IMPORTED_SCHEMA_GET_BASE_IMPL(schema, env) \
      (((woden_imported_schema_t *) schema)->ops->get_base_impl(schema, \
                                                                        env))

#define WODEN_IMPORTED_SCHEMA_SET_LOCATION(schema, env, location) \
      (((woden_imported_schema_t *) schema)->ops->set_location(schema, \
                                                                    env, location))

#define WODEN_IMPORTED_SCHEMA_GET_LOCATION(schema, env) \
      (((woden_imported_schema_t *) schema)->ops->get_location(schema, \
                                                                       env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_IMPORTED_SCHEMA_H */
