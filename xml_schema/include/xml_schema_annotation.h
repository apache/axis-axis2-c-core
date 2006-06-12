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

#ifndef XML_SCHEMA_ANNOTATION_H
#define XML_SCHEMA_ANNOTATION_H

/**
 * @file xml_schema_annotation.h
 * @brief Axis2 Xml Schema Annotation Interface
 */

#include <xml_schema_defines.h>
#include <xml_schema_obj.h>
#include <xml_schema_obj_collection.h>
#include <axis2_hash.h>

/** @defgroup xml_schema_annotation Xml Schema Annotation
  * @ingroup xml_schema
  * @{
  */

typedef struct xml_schema_annotation xml_schema_annotation_t;
typedef struct xml_schema_annotation_ops xml_schema_annotation_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct xml_schema_annotation_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *annotation,
            const axis2_env_t *env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *annotation,
            const axis2_env_t *env);

    xml_schema_types_t (AXIS2_CALL *
    get_type) (
            void *annotation,
            const axis2_env_t *env);

    xml_schema_obj_t *(AXIS2_CALL *
    get_base_impl) (void *annotation,
                        const axis2_env_t *env);
    
    xml_schema_obj_collection_t *(AXIS2_CALL *
    get_items)(void *annotation,
                const axis2_env_t *env);
    
};

struct xml_schema_annotation
{
    xml_schema_obj_t base;
    xml_schema_annotation_ops_t *ops;
};

AXIS2_EXTERN xml_schema_annotation_t * AXIS2_CALL
xml_schema_annotation_create(const axis2_env_t *env);

/***************** Macros *****************************************/

#define XML_SCHEMA_ANNOTATION_FREE(annotation, env) \
      (((xml_schema_annotation_t *) annotation)->ops->\
            free(annotation, env))

#define XML_SCHEMA_ANNOTATION_SUPER_OBJS(annotation, env) \
      (((xml_schema_annotation_t *) annotation)->ops->\
            super_objs(annotation, env))

#define XML_SCHEMA_ANNOTATION_GET_TYPE(annotation, env) \
      (((xml_schema_annotation_t *) annotation)->ops->\
            get_type(annotation, env))

#define XML_SCHEMA_ANNOTATION_GET_ITEMS(annotation, env) \
      (((xml_schema_annotation_t *) annotation)->ops->\
            get_items(annotation, env))

#define XML_SCHEMA_ANNOTATION_GET_BASE_IMPL(annotation, env) \
      (((xml_schema_annotation_t *) annotation)->ops->\
            get_base_impl(annotation, env))

/************************ end macros *******************************/
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_ANNOTATION_H */
