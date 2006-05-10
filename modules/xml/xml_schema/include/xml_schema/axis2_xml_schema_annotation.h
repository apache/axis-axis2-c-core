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

#ifndef AXIS2_XML_SCHEMA_ANNOTATION_H
#define AXIS2_XML_SCHEMA_ANNOTATION_H

/**
 * @file axis2_xml_schema_annotation.h
 * @brief Axis2 Xml Schema Annotation Interface
 */

#include <xml_schema/axis2_xml_schema_defines.h>
#include <xml_schema/axis2_xml_schema_obj.h>
#include <axis2_hash.h>

/** @defgroup axis2_xml_schema_annotation Xml Schema Annotation
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_annotation axis2_xml_schema_annotation_t;
typedef struct axis2_xml_schema_annotation_ops axis2_xml_schema_annotation_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_annotation_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *annotation,
            axis2_env_t **env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *annotation,
            axis2_env_t **env);

    axis2_xml_schema_types_t (AXIS2_CALL *
    type) (
            void *annotation,
            axis2_env_t **env);

    axis2_xml_schema_obj_t *(AXIS2_CALL *
    get_base_impl) (void *annotation,
                        axis2_env_t **env);
    
    axis2_hash_t *(AXIS2_CALL *
    get_items)(void *annotation,
                axis2_env_t **env);
    
};

struct axis2_xml_schema_annotation
{
    axis2_xml_schema_obj_t base;
    axis2_xml_schema_annotation_ops_t *ops;
};

AXIS2_DECLARE(axis2_xml_schema_annotation_t *)
axis2_xml_schema_annotation_create(axis2_env_t **env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_annotation_resolve_methods(
                                axis2_xml_schema_annotation_t *annotation,
                                axis2_env_t **env,
                                axis2_xml_schema_annotation_t *annotation_impl,
                                axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_ANNOTATION_FREE(annotation, env) \
		(((axis2_xml_schema_annotation_t *) annotation)->ops->\
            free(annotation, env))

#define AXIS2_XML_SCHEMA_ANNOTATION_SUPER_OBJS(annotation, env) \
		(((axis2_xml_schema_annotation_t *) annotation)->ops->\
            super_objs(annotation, env))

#define AXIS2_XML_SCHEMA_ANNOTATION_TYPE(annotation, env) \
		(((axis2_xml_schema_annotation_t *) annotation)->ops->\
            type(annotation, env))

#define AXIS2_XML_SCHEMA_ANNOTATION_GET_ITEMS(annotation, env) \
		(((axis2_xml_schema_annotation_t *) annotation)->ops->\
            get_items(annotation, env))

#define AXIS2_XML_SCHEMA_ANNOTATION_GET_BASE_IMPL(annotation, env) \
		(((axis2_xml_schema_annotation_t *) annotation)->ops->\
            get_base_impl(annotation, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_ANNOTATION_H */
