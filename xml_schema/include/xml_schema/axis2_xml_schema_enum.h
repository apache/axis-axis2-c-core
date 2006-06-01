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

#ifndef AXIS2_XML_SCHEMA_ENUM_H
#define AXIS2_XML_SCHEMA_ENUM_H

/**
 * @file axis2_xml_schema_enum.h
 * @brief Axis2 Xml Schema Enum  interface
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils_defines.h>
#include <axis2_array_list.h>
#include <axis2_hash.h>
#include <xml_schema/axis2_xml_schema_defines.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_xml_schema_enum axis2_xml_schema_enum_t;
typedef struct axis2_xml_schema_enum_ops axis2_xml_schema_enum_ops_t;

/** @defgroup axis2_xml_schema_enum Xml Schema Enum
  * @ingroup axis2_xml_schema
  * @{
  */

struct axis2_xml_schema_enum_ops
{
    axis2_status_t (AXIS2_CALL *
    free)(
            void *schema_enum,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs)(
            void *schema_enum,
            const axis2_env_t *env);

    axis2_xml_schema_types_t (AXIS2_CALL *
    type) (
            void *schema_enum,
            const axis2_env_t *env);

    axis2_char_t* (AXIS2_CALL *
    get_value)(
            void *schema_enum,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_value)(
            void *schema_enum,
            const axis2_env_t *env,
            axis2_char_t *value);

    axis2_bool_t (AXIS2_CALL *
    equals)(void *schema_enum,
            const axis2_env_t *env,
            void *obj);

    axis2_array_list_t* (AXIS2_CALL *
    get_values)(
            void *schema_enum, 
            const axis2_env_t *env);
    
};

struct axis2_xml_schema_enum
{
    axis2_xml_schema_enum_ops_t *ops;
};

AXIS2_EXTERN axis2_xml_schema_enum_t * AXIS2_CALL
axis2_xml_schema_enum_create(const axis2_env_t *env,
                             axis2_char_t *value);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_xml_schema_enum_resolve_methods(axis2_xml_schema_enum_t *schema_enum,
                                      const axis2_env_t *env,
                                      axis2_xml_schema_enum_t *schema_enum_impl,
                                      axis2_hash_t *methods);

AXIS2_EXTERN int AXIS2_CALL
axis2_xml_schema_enum_index(axis2_char_t *value,
                            const axis2_env_t *env,
                            axis2_array_list_t *values);
                            
/***************** macros ****************************************************/                           

#define AXIS2_XML_SCHEMA_ENUM_FREE(schema_enum, env) \
		(((axis2_xml_schema_enum_t *) schema_enum)->ops->free (schema_enum, env))

#define AXIS2_XML_SCHEMA_ENUM_SUPER_OBJS(schema_enum, env) \
		(((axis2_xml_schema_enum_t *) schema_enum)->ops->super_objs (schema_enum, env))

#define AXIS2_XML_SCHEMA_ENUM_TYPE(schema_enum, env) \
		(((axis2_xml_schema_enum_t *) schema_enum)->ops->type (schema_enum, env))

#define AXIS2_XML_SCHEMA_ENUM_GET_VALUE(schema_enum, env) \
		(((axis2_xml_schema_enum_t *) schema_enum)->ops->get_value \
         (schema_enum, env))

#define AXIS2_XML_SCHEMA_ENUM_SET_VALUE(schema_enum, env, value) \
		(((axis2_xml_schema_enum_t *) schema_enum)->ops->set_value \
         (schema_enum, env, value))

#define AXIS2_XML_SCHEMA_ENUM_EQUALS(schema_enum, env) \
		(((axis2_xml_schema_enum_t *) schema_enum)->ops->equals (schema_enum, env))

#define AXIS2_XML_SCHEMA_ENUM_GET_VALUES(schema_enum, env) \
		(((axis2_xml_schema_enum_t *) schema_enum)->ops->get_values \
         (schema_enum, env))

/**************** end macros *************************************************/
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_ENUM_H */
