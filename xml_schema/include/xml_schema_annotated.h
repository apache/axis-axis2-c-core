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

#ifndef XML_SCHEMA_ANNOTATED_H
#define XML_SCHEMA_ANNOTATED_H

/**
 * @file xml_schema_annotated.h
 * @brief Axis2 Xml Schema Annotated Interface
 */

#include <xml_schema_defines.h>
#include <xml_schema_obj.h>

/** @defgroup xml_schema_annotated Xml Schema Annotated
  * @ingroup xml_schema
  * @{
  */
                    
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct xml_schema_annotated 
                    xml_schema_annotated_t;
typedef struct xml_schema_annotated_ops 
                    xml_schema_annotated_ops_t;
                    
struct xml_schema_annotated_ops
{
    axis2_status_t (AXIS2_CALL *
    free)(void *annotated,
          const axis2_env_t *env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs)(
            void *annotated,
            const axis2_env_t *env);
    
    xml_schema_types_t (AXIS2_CALL *
    get_type)(
            void *annotated,
            const axis2_env_t *env);

    xml_schema_obj_t *(AXIS2_CALL *
    get_base_impl)(
            void *annotated,
            const axis2_env_t *env);

    axis2_char_t *(AXIS2_CALL *
    get_id) (void *annotated,
             const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_id) (
            void *annotated,
            const axis2_env_t *env,
            axis2_char_t *id);

    struct xml_schema_annotation *(AXIS2_CALL *
    get_annotation) (
            void *annotated,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_annotation)(
            void *annotated,
            const axis2_env_t *env,
            struct xml_schema_annotation *annotation);

    axis2_array_list_t *(AXIS2_CALL *
    get_unhandled_attrs) (
            void *annotated,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_unhandled_attrs) (
            void *annotated,
            const axis2_env_t *env,
            axis2_array_list_t *unhandled_attrs);
};

struct xml_schema_annotated
{
    xml_schema_obj_t base;
    xml_schema_annotated_ops_t *ops;
};

AXIS2_EXTERN xml_schema_annotated_t * AXIS2_CALL
xml_schema_annotated_create(const axis2_env_t *env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
xml_schema_annotated_resolve_methods(
                                xml_schema_annotated_t *annotated,
                                const axis2_env_t *env,
                                xml_schema_annotated_t *annotated_impl,
                                axis2_hash_t *methods);
                                
/*************** Macros *******************************************************/                                

#define XML_SCHEMA_ANNOTATED_FREE(annotated, env) \
      (((xml_schema_annotated_t *) annotated)->ops->free(\
            annotated, env))

#define XML_SCHEMA_ANNOTATED_SUPER_OBJS(annotated, env) \
      (((xml_schema_annotated_t *) annotated)->ops->super_objs(\
            annotated, env))

#define XML_SCHEMA_ANNOTATED_GET_TYPE(annotated, env) \
      (((xml_schema_annotated_t *) annotated)->ops->get_type(\
            annotated, env))

#define XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(annotated, env) \
      (((xml_schema_annotated_t *) annotated)->ops->get_base_impl(\
            annotated, env))

#define XML_SCHEMA_ANNOTATED_GET_ID(annotated, env) \
      (((xml_schema_annotated_t *) annotated)->ops->get_id(\
            annotated, env))

#define XML_SCHEMA_ANNOTATED_SET_ID(annotated, env, id) \
      (((xml_schema_annotated_t *) annotated)->ops->set_id(\
            annotated, env, id))

#define XML_SCHEMA_ANNOTATED_GET_ANNOTATION(annotated, env) \
      (((xml_schema_annotated_t *) annotated)->ops->get_annotation(\
            annotated, env))

#define XML_SCHEMA_ANNOTATED_SET_ANNOTATION(annotated, env, \
        annotation) \
      (((xml_schema_annotated_t *) annotated)->ops->set_annotation(\
            annotated, env, annotation))

#define XML_SCHEMA_ANNOTATED_GET_UNHANDLED_ATTRS(annotated, env) \
      (((xml_schema_annotated_t *) annotated)->ops->get_unhandled_attrs(\
            annotated, env))

#define XML_SCHEMA_ANNOTATED_SET_UNHANDLED_ATTRS(annotated, \
        env, unhandled_attrs) \
      (((xml_schema_annotated_t *) annotated)->ops->set_unhandled_attrs(\
            annotated, env, unhandled_attrs))

/**************************** end macros ***************************************/
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_ANNOTATED_H */
