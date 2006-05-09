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

#ifndef AXIS2_XML_SCHEMA_SIMPLE_TYPE_UNION_H
#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_UNION_H

/**
 * @file axis2_xml_schema_simple_type_union.h
 * @brief Axis2 Xml Schema Particle Interface
 *          Base class for all simple_type_union types.
 */


#include <xml_schema/axis2_xml_schema_simple_type.h>
#include <xml_schema/axis2_xml_schema_simple_type_content.h>
#include <xml_schema/axis2_xml_schema_obj_collection.h>
#include <axis2_hash.h>
#include <axis2_array_list.h>
/** @defgroup axis2_xml_schema_simple_type_union 
  * @ingroup axis2_xml_schema
  * @{
  */
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_xml_schema_simple_type_union 
                    axis2_xml_schema_simple_type_union_t;
typedef struct axis2_xml_schema_simple_type_union_ops 
                    axis2_xml_schema_simple_type_union_ops_t;

struct axis2_xml_schema_simple_type_union_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *simple_type_union,
            axis2_env_t **env);

    axis2_xml_schema_simple_type_content_t *(AXIS2_CALL *
    get_base_impl) (void *simple_type_union,
                    axis2_env_t **env);

    axis2_xml_schema_obj_collection_t*(AXIS2_CALL *
    get_base_types)(void *simple_type_union,
                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_member_types_source)(void *simple_type_union,
                             axis2_env_t **env,
                             axis2_char_t *member_types_source);
    
    axis2_char_t* (AXIS2_CALL *
    get_member_types_source)(void *simple_type_union,
                            axis2_env_t **env);
                    
    axis2_array_list_t* (AXIS2_CALL *
    get_member_types_qnames)(void *simple_type_union,
                axis2_env_t **env);
                
};

struct axis2_xml_schema_simple_type_union
{
    axis2_xml_schema_simple_type_content_t base;
    axis2_xml_schema_simple_type_union_ops_t *ops;
};

/**
 * Creates new Xml Schema Particle
 * Particle types are usually interchangeable.
 * A local element declaration or reference to a global element
 * declaration (element), a compositor ( sequence, choice, or all),
 * a reference to a named content model group (group), or an element wildcard 
 * (any).
 */
AXIS2_DECLARE(axis2_xml_schema_simple_type_union_t *)
axis2_xml_schema_simple_type_union_create(axis2_env_t **env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_simple_type_union_resolve_methods(
                    axis2_xml_schema_simple_type_union_t *simple_type_union,
                    axis2_env_t **env,
                    axis2_xml_schema_simple_type_union_t *simple_type_union_impl,
                    axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_UNION_FREE(simple_type_union, env) \
		(((axis2_xml_schema_simple_type_union_t *) simple_type_union)->ops->\
            free(simple_type_union, env))

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_UNION_GET_BASE_IMPL(simple_type_union, env) \
		(((axis2_xml_schema_simple_type_union_t *) simple_type_union)->ops->\
            get_base_impl(simple_type_union, env))

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_UNION_GET_BASE_TYPES(simple_type_union, env) \
		(((axis2_xml_schema_simple_type_union_t *) simple_type_union)->ops->\
            get_max_occurs(simple_type_union, env))

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_UNION_SET_MEMBER_TYPES_SOURCE(simple_type_union,\
         env, member_types_sources) \
		(((axis2_xml_schema_simple_type_union_t *) simple_type_union)->ops->\
            set_member_types_source(simple_type_union, env, member_types_sources))

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_UNION_GET_MEMBER_TYPES_SOURCE(simple_type_union, env) \
		(((axis2_xml_schema_simple_type_union_t *) simple_type_union)->ops->\
            get_member_types_source(simple_type_union, env))

#define AXIS2_XML_SCHEMA_SIMPLE_TYPE_GET_MEMBER_TYPES_QNAMES(simple_type_union, env) \
		(((axis2_xml_schema_simple_type_union_t *) simple_type_union)->ops->\
            set_member_types_qnames(simple_type_union, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_SIMPLE_TYPE_UNION_H */
