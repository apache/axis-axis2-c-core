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

#ifndef AXIS2_XML_SCHEMA_GROUP_H
#define AXIS2_XML_SCHEMA_GROUP_H

/**
 * @file axis2_xml_schema_group.h
 * @brief Axis2 Xml Schema Group Interface
 *          Class that defines groups at the schema level that are referenced
 *          from the complex types. Groups a set of element declarations so that
 *          they can be incorporated as a group into complex type definitions.
 *          Represents the World Wide Web Consortium (W3C) group element.
 */

#include <xml_schema/axis2_xml_schema_annotated.h>
#include <axis2_hash.h>

/** @defgroup axis2_xml_schema_group Xml Schema Group
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_group axis2_xml_schema_group_t;
typedef struct axis2_xml_schema_group_ops axis2_xml_schema_group_ops_t;
struct axis2_xml_schema_group_base;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_group_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *group,
            axis2_env_t **env);

    axis2_xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl) (void *group,
                    axis2_env_t **env);

    axis2_char_t *(AXIS2_CALL *
    get_name)(void *group,
                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_name)(void *group,
                    axis2_env_t **env,
                    axis2_char_t *name);
    
    struct axis2_xml_schema_group_base *(AXIS2_CALL *
    get_particle)(void *group,
                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_particle)(void *group,
                    axis2_env_t **env,
                    struct axis2_xml_schema_group_base *particle);
};

struct axis2_xml_schema_group
{
    axis2_xml_schema_annotated_t base;
    axis2_xml_schema_group_ops_t *ops;
};

/**
 * Creates new Xml Schema Group
 */
AXIS2_DECLARE(axis2_xml_schema_group_t *)
axis2_xml_schema_group_create(axis2_env_t **env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_group_resolve_methods(
                                axis2_xml_schema_group_t *group,
                                axis2_env_t **env,
                                axis2_xml_schema_group_t *group_impl,
                                axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_GROUP_FREE(group, env) \
		(((axis2_xml_schema_group_t *) group)->ops->\
            free(group, env))

#define AXIS2_XML_SCHEMA_GROUP_GET_BASE_IMPL(group, env) \
		(((axis2_xml_schema_group_t *) group)->ops->\
            get_base_impl(group, env))

#define AXIS2_XML_SCHEMA_GROUP_GET_NAME(group, env) \
		(((axis2_xml_schema_group_t *) group)->ops->\
            get_name(group, env))

#define AXIS2_XML_SCHEMA_GROUP_SET_NAME(group, env, name) \
		(((axis2_xml_schema_group_t *) group)->ops->\
            set_name(group, env, name))

#define AXIS2_XML_SCHEMA_GROUP_GET_PARTICLE(group, env) \
		(((axis2_xml_schema_group_t *) group)->ops->\
            get_particle(group, env))

#define AXIS2_XML_SCHEMA_GROUP_SET_PARTICLE(group, env, particle) \
		(((axis2_xml_schema_group_t *) group)->ops->\
            set_particle(group, env, particle))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_GROUP_H */
