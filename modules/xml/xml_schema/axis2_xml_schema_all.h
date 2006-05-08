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

#ifndef AXIS2_XML_SCHEMA_ALL_H
#define AXIS2_XML_SCHEMA_ALL_H

/**
 * @file axis2_xml_schema_all.h
 * @brief Axis2 Xml Schema All Interface
 *          Permits the elements in the group to appear (or not appear)
 *          in any order in the containing element. Represents the World
 *          Wide Web Consortium (W3C) all element (compositor).
 */

#include <xml_schema/axis2_xml_schema_group_base.h>
#include <axis2_hash.h>

/** @defall axis2_xml_schema_all Xml Schema All
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_all axis2_xml_schema_all_t;
typedef struct axis2_xml_schema_all_ops axis2_xml_schema_all_ops_t;
struct axis2_xml_schema_group_base;
struct axis2_xml_schema_obj_collection;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_all_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *all,
            axis2_env_t **env);

    struct axis2_xml_schema_group_base *(AXIS2_CALL *
    get_base_impl) (void *all,
                    axis2_env_t **env);

    struct axis2_xml_schema_obj_collection *(AXIS2_CALL *
    get_items)(void *all,
                axis2_env_t **env);
    
};

struct axis2_xml_schema_all
{
    struct axis2_xml_schema_group_base base;
    axis2_xml_schema_all_ops_t *ops;
};

/**
 * Creates new Xml Schema All
 */
AXIS2_DECLARE(axis2_xml_schema_all_t *)
axis2_xml_schema_all_create(axis2_env_t **env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_all_resolve_methods(
                                axis2_xml_schema_all_t *all,
                                axis2_env_t **env,
                                axis2_xml_schema_all_t *all_impl,
                                axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_ALL_FREE(all, env) \
		(((axis2_xml_schema_all_t *) all)->ops->\
            free(all, env))

#define AXIS2_XML_SCHEMA_ALL_GET_BASE_IMPL(all, env) \
		(((axis2_xml_schema_all_t *) all)->ops->\
            get_base_impl(all, env))

#define AXIS2_XML_SCHEMA_ALL_GET_ITEMS(all, env) \
		(((axis2_xml_schema_all_t *) all)->ops->\
            get_items(all, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_ALL_H */
