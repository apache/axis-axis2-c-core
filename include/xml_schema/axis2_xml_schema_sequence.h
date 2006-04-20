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

#ifndef AXIS2_XML_SCHEMA_SEQUENCE_H
#define AXIS2_XML_SCHEMA_SEQUENCE_H

/**
 * @file axis2_xml_schema_sequence.h
 * @brief Axis2 Xml Schema Sequence Interface
 *          Requires the elements in the group to appear in the specified sequence
 *          within the containing element. Represents the World Wide Web Consortium
 *          (W3C) sequence (compositor) element.
 */

#include <xml_schema/axis2_xml_schema_group_base.h>
#include <axis2_hash.h>

/** @defsequence axis2_xml_schema_sequence Xml Schema Sequence
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_sequence axis2_xml_schema_sequence_t;
typedef struct axis2_xml_schema_sequence_ops axis2_xml_schema_sequence_ops_t;
struct axis2_xml_schema_group_base;
struct axis2_xml_schema_obj_collection;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_sequence_ops
{
	/** 
     * Desequenceocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *sequence,
            axis2_env_t **env);

    struct axis2_xml_schema_group_base *(AXIS2_CALL *
    get_base_impl) (void *sequence,
                    axis2_env_t **env);
    /*
     * The elements contained within the compositor.
     * Collection of Xml Schema Element, Xml Schema Group Ref,
     * Xml Schema Choice, Xml Schema Sequence, or Xml Schema Any.
     */
    struct axis2_xml_schema_obj_collection *(AXIS2_CALL *
    get_items)(void *sequence,
                axis2_env_t **env);
    
};

struct axis2_xml_schema_sequence
{
    struct axis2_xml_schema_group_base base;
    axis2_xml_schema_sequence_ops_t *ops;
};

/**
 * Creates new Xml Schema Sequence
 */
AXIS2_DECLARE(axis2_xml_schema_sequence_t *)
axis2_xml_schema_sequence_create(axis2_env_t **env);

/**
 * This method is internal to Axis2 C. It is csequenceed from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_sequence_resolve_methods(
                                axis2_xml_schema_sequence_t *sequence,
                                axis2_env_t **env,
                                axis2_xml_schema_sequence_t *sequence_impl,
                                axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_SEQUENCE_FREE(sequence, env) \
		(((axis2_xml_schema_sequence_t *) sequence)->ops->\
            free(sequence, env))

#define AXIS2_XML_SCHEMA_SEQUENCE_GET_BASE_IMPL(sequence, env) \
		(((axis2_xml_schema_sequence_t *) sequence)->ops->\
            get_base_impl(sequence, env))

#define AXIS2_XML_SCHEMA_SEQUENCE_GET_ITEMS(sequence, env) \
		(((axis2_xml_schema_sequence_t *) sequence)->ops->\
            get_items(sequence, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_SEQUENCE_H */
