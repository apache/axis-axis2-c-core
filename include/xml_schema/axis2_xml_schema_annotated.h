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

#ifndef AXIS2_XML_SCHEMA_ANNOTATED_H
#define AXIS2_XML_SCHEMA_ANNOTATED_H

/**
 * @file axis2_xml_schema_annotated.h
 * @brief Axis2 Xml Schema Annotated Interface
 */

#include <axis2_xml_schema_obj.h>

/** @defgroup axis2_xml_schema_annotated Xml Schema Annotated
  * @ingroup axis2_xml_schema
  * @{
  */


                    
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_xml_schema_annotated 
                    axis2_xml_schema_annotated_t;
typedef struct axis2_xml_schema_annotated_ops 
                    axis2_xml_schema_annotated_ops_t;
                    
typedef enum axis2_xml_schema_content_model_types_t
{
    AXIS2_XML_SCHEMA_SIMPLE_CONTENT = 0,
    AXIS2_XML_SCHEMA_COMPLEX_CONTENT
}axis2_xml_schema_content_model_types_t; 


typedef enum axis2_xml_schema_content_types_t
{
    AXIS2_XML_SCHEMA_COMPLEX_CONTENT_EXTENSION = 0,
    AXIS2_XML_SCHEMA_COMPLEX_CONTENT_RESTRICTION,
    AXIS2_XML_SCHEMA_SIMPLE_CONTENT_EXTENSION,
    AXIS2_XML_SCHEMA_SIMPLE_CONTENT_RESTRICTION 

}axis2_xml_schema_content_types_t;

struct axis2_xml_schema_annotated_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free)(void *annotated,
            axis2_env_t **env);
    
    axis2_xml_schema_obj_t *(AXIS2_CALL *
    get_base_impl) (void *annotated,
                    axis2_env_t **env);

    axis2_char_t *(AXIS2_CALL *
    get_id) (void *annotated,
                axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_id) (void *annotated,
                axis2_env_t **env,
                axis2_char_t *id);

    struct axis2_xml_schema_annotation *(AXIS2_CALL *
    get_annotation) (void *annotated,
                        axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_annotation) (void *annotated,
                        axis2_env_t **env,
                        struct axis2_xml_schema_annotation *
                            annotation);

    axis2_array_list_t *(AXIS2_CALL *
    get_unhandled_attrs) (void *annotated,
                            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_unhandled_attrs) (void *annotated,
                            axis2_env_t **env,
                            axis2_array_list_t *
                                unhandled_attrs);
};

struct axis2_xml_schema_annotated
{
    axis2_xml_schema_obj_t base;
    axis2_xml_schema_annotated_ops_t *ops;
};

AXIS2_DECLARE(axis2_xml_schema_annotated_t *)
axis2_xml_schema_annotated_create(axis2_env_t **env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_annotated_resolve_methods(
                                axis2_xml_schema_annotated_t *annotated,
                                axis2_env_t **env,
                                axis2_xml_schema_annotated_t *annotated_impl,
                                axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_ANNOTATED_FREE(annotated, env) \
		(((axis2_xml_schema_annotated_t *) annotated)->ops->free(\
            annotated, env))

#define AXIS2_XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(annotated, env) \
		(((axis2_xml_schema_annotated_t *) annotated)->ops->get_base_impl(\
            annotated, env))

#define AXIS2_XML_SCHEMA_ANNOTATED_GET_ID(annotated, env) \
		(((axis2_xml_schema_annotated_t *) annotated)->ops->get_id(\
            annotated, env))

#define AXIS2_XML_SCHEMA_ANNOTATED_SET_ID(annotated, env, id) \
		(((axis2_xml_schema_annotated_t *) annotated)->ops->set_id(\
            annotated, env, id))

#define AXIS2_XML_SCHEMA_ANNOTATED_GET_ANNOTATION(annotated, env) \
		(((axis2_xml_schema_annotated_t *) annotated)->ops->get_annotation(\
            annotated, env))

#define AXIS2_XML_SCHEMA_ANNOTATED_SET_ANNOTATION(annotated, env, \
        annotation) \
		(((axis2_xml_schema_annotated_t *) annotated)->ops->set_annotation(\
            annotated, env, annotation))

#define AXIS2_XML_SCHEMA_ANNOTATED_GET_UNHANDLED_ATTRS(annotated, env) \
		(((axis2_xml_schema_annotated_t *) annotated)->ops->get_unhandled_attrs(\
            annotated, env))

#define AXIS2_XML_SCHEMA_ANNOTATED_SET_UNHANDLED_ATTRS(annotated, \
        env, unhandled_attrs) \
		(((axis2_xml_schema_annotated_t *) annotated)->ops->set_unhandled_attrs(\
            annotated, env, unhandled_attrs))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_ANNOTATED_H */
