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

#ifndef AXIS2_WODEN_IMPORTED_SCHEMA_H
#define AXIS2_WODEN_IMPORTED_SCHEMA_H

/**
 * @file axis2_woden_imported_schema.h
 * @brief Axis2 Imported Schema Interface
 *          This interface represents a schema import, &lt;xs:import&gt;. It 
 *          extends the abstract class SchemaImpl, adding support for the 
 *          <code>schemaLocation</code> attribute.
 */

#include <woden/schema/axis2_woden_schema.h>

/** @defgroup axis2_woden_imported_schema Imported Schema
  * @ingroup axis2_wsdl
  * @{
  */

typedef struct axis2_woden_imported_schema axis2_woden_imported_schema_t;
typedef struct axis2_woden_imported_schema_ops axis2_woden_imported_schema_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_imported_schema_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *schema,
            const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    to_imported_schema_free) (
            void *schema,
            const axis2_env_t *env);

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *schema,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    axis2_woden_schema_t *(AXIS2_CALL *
    get_base_impl) (
            void *schema,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_location) (
            void *schema,
            const axis2_env_t *env,
            struct axis2_url *location);

    struct axis2_url *(AXIS2_CALL *
    get_location) (
            void *schema,
            const axis2_env_t *env);

  
};

struct axis2_woden_imported_schema
{
    axis2_woden_schema_t schema;
    axis2_woden_imported_schema_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_imported_schema_t *)
axis2_woden_imported_schema_create(
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_DECLARE(axis2_woden_imported_schema_t *)
axis2_woden_imported_schema_to_schema(
        void *schema,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_status_t)
axis2_woden_imported_schema_resolve_methods(
        axis2_woden_imported_schema_t *schema,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_IMPORTED_SCHEMA_FREE(schema, env) \
		(((axis2_woden_imported_schema_t *) schema)->ops->free(schema, env))

#define AXIS2_WODEN_IMPORTED_SCHEMA_GET_BASE_IMPL(schema, env) \
		(((axis2_woden_imported_schema_t *) schema)->ops->get_base_impl(schema, \
                                                                        env))

#define AXIS2_WODEN_IMPORTED_SCHEMA_SET_LOCATION(schema, env, location) \
		(((axis2_woden_imported_schema_t *) schema)->ops->set_location(schema, \
                                                                    env, location))

#define AXIS2_WODEN_IMPORTED_SCHEMA_GET_LOCATION(schema, env) \
		(((axis2_woden_imported_schema_t *) schema)->ops->get_location(schema, \
                                                                       env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_IMPORTED_SCHEMA_H */
