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

#ifndef XML_SCHEMA_OBJ_H
#define XML_SCHEMA_OBJ_H

/**
 * @file xml_schema_obj.h
 * @brief Axis2 Xml Schema Obj  interface
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils_defines.h>
#include <axis2_array_list.h>
#include <axis2_hash.h>
#include <xml_schema_defines.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct xml_schema_obj xml_schema_obj_t;
typedef struct xml_schema_obj_ops xml_schema_obj_ops_t;

/** @defgroup xml_schema_obj Xml Schema Obj
  * @ingroup xml_schema
  * @{
  */

struct xml_schema_obj_ops
{
    axis2_status_t (AXIS2_CALL *
    free) (
            void *obj,
            const axis2_env_t *env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *obj,
            const axis2_env_t *env);
    
    xml_schema_types_t (AXIS2_CALL *
    get_type) (
            void *obj,
            const axis2_env_t *env);

    int (AXIS2_CALL *
    get_line_num) (void *obj,
                    const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_line_num) (void *obj,
                    const axis2_env_t *env,
                    int line_num);

    int (AXIS2_CALL *
    get_line_pos) (void *obj,
                    const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_line_pos) (void *obj,
                    const axis2_env_t *env,
                    int line_pos);

    axis2_char_t * (AXIS2_CALL *
    get_source_uri) (void *obj,
                        const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_source_uri) (void *obj,
                        const axis2_env_t *env,
                        axis2_char_t *source_uri);

    axis2_bool_t (AXIS2_CALL *
    equals) (void *obj,
             const axis2_env_t *env,
             void *obj_comp);

};

struct xml_schema_obj
{
    xml_schema_obj_ops_t *ops;
};

AXIS2_EXTERN xml_schema_obj_t * AXIS2_CALL
xml_schema_obj_create(const axis2_env_t *env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
xml_schema_obj_resolve_methods(xml_schema_obj_t *schema_obj,
                                        const axis2_env_t *env,
                                        xml_schema_obj_t *schema_obj_impl,
                                        axis2_hash_t *methods);

#define XML_SCHEMA_OBJ_FREE(obj, env) \
      (((xml_schema_obj_t *) obj)->ops->free (obj, env))

#define XML_SCHEMA_OBJ_SUPER_OBJS(obj, env) \
      (((xml_schema_obj_t *) obj)->ops->super_objs (obj, env))

#define XML_SCHEMA_OBJ_GET_TYPE(obj, env) \
      (((xml_schema_obj_t *) obj)->ops->get_type (obj, env))

#define XML_SCHEMA_OBJ_GET_LINE_NUM(obj, env) \
      (((xml_schema_obj_t *) obj)->ops->get_line_num (obj, env))

#define XML_SCHEMA_OBJ_SET_LINE_NUM(obj, env, line_num) \
      (((xml_schema_obj_t *) obj)->ops->set_line_num (obj, env, line_num))

#define XML_SCHEMA_OBJ_GET_LINE_POS(obj, env) \
      (((xml_schema_obj_t *) obj)->ops->get_line_pos (obj, env))

#define XML_SCHEMA_OBJ_SET_LINE_POS(obj, env, line_pos) \
      (((xml_schema_obj_t *) obj)->ops->set_line_pos (obj, env, line_pos))

#define XML_SCHEMA_OBJ_GET_SOURCE_URI(obj, env) \
      (((xml_schema_obj_t *) obj)->ops->get_source_uri (obj, env))

#define XML_SCHEMA_OBJ_SET_SOURCE_URI(obj, env, source_uri) \
      (((xml_schema_obj_t *) obj)->ops->set_source_uri (obj, env, \
                                                                source_uri))

#define XML_SCHEMA_OBJ_EQUALS(obj, env) \
      (((xml_schema_obj_t *) obj)->ops->equals (obj, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_OBJ_H */
