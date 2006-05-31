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

#ifndef AXIS2_XML_SCHEMA_GROUP_BASE_H
#define AXIS2_XML_SCHEMA_GROUP_BASE_H

/**
 * @file axis2_xml_schema_group_base.h
 * @brief Axis2 Xml Schema Group Base Interface
 *          An abstract class for xml_schema_choice, xml_schema_all, or 
 *          xml_schema_sequence.
 *
 */

#include <xml_schema/axis2_xml_schema_defines.h>
#include <xml_schema/axis2_xml_schema_particle.h>
#include <xml_schema/axis2_xml_schema_obj_collection.h>

/** @defgroup axis2_xml_schema_group_base Xml Schema Group Base
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_group_base 
                    axis2_xml_schema_group_base_t;
typedef struct axis2_xml_schema_group_base_ops 
                    axis2_xml_schema_group_base_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_group_base_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *group_base,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *group_base,
            const axis2_env_t *env);

    axis2_xml_schema_types_t (AXIS2_CALL *
    type) (void *group_base,
            const axis2_env_t *env);

    axis2_xml_schema_particle_t *(AXIS2_CALL *
    get_base_impl) (void *group_base,
                    const axis2_env_t *env);
    
    axis2_xml_schema_obj_collection_t *(AXIS2_CALL *
    get_items)(void *group_base,
                    const axis2_env_t *env);
    /**
     * only valid if the type is AXIS2_XML_SCHEMA_SEQUENCE
     */
    axis2_status_t (AXIS2_CALL *
    to_string)(void *group_base,
               const axis2_env_t *env,
               axis2_char_t *prefix,
               int tab);
               

};

struct axis2_xml_schema_group_base
{
    axis2_xml_schema_particle_t base;
    axis2_xml_schema_group_base_ops_t *ops;
};


AXIS2_DECLARE(axis2_xml_schema_group_base_t *)
axis2_xml_schema_group_base_create(const axis2_env_t *env);

AXIS2_DECLARE(axis2_xml_schema_group_base_t *)
axis2_xml_schema_all_create(const axis2_env_t *env);

AXIS2_DECLARE(axis2_xml_schema_group_base_t *)
axis2_xml_schema_sequence_create(const axis2_env_t *env);

AXIS2_DECLARE(axis2_xml_schema_group_base_t *)
axis2_xml_schema_choice_create(const axis2_env_t *env);


/**
 * This method is internal to Axis2 C. It is called from Child Constructor
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_group_base_resolve_methods(
                                axis2_xml_schema_group_base_t *group_base,
                                const axis2_env_t *env,
                                axis2_xml_schema_group_base_t *group_base_impl,
                                axis2_hash_t *methods);
*/
/********************** macros ***********************************************/
#define AXIS2_XML_SCHEMA_GROUP_BASE_FREE(group_base, env) \
		(((axis2_xml_schema_group_base_t *) group_base)->ops->free(group_base, env))

#define AXIS2_XML_SCHEMA_GROUP_BASE_SUPER_OBJS(group_base, env) \
		(((axis2_xml_schema_group_base_t *) group_base)->ops->super_objs(group_base, env))

#define AXIS2_XML_SCHEMA_GROUP_BASE_TYPE(group_base, env) \
		(((axis2_xml_schema_group_base_t *) group_base)->ops->type(group_base, env))

#define AXIS2_XML_SCHEMA_GROUP_BASE_GET_BASE_IMPL(group_base, env) \
		(((axis2_xml_schema_group_base_t *) group_base)->ops->get_base_impl(group_base, env))

#define AXIS2_XML_SCHEMA_GROUP_BASE_GET_ITEMS(group_base, env) \
		(((axis2_xml_schema_group_base_t *) group_base)->ops->get_items(group_base, env))

#define AXIS2_XML_SCHEMA_GROUP_BASE_TO_STRING(group_base, env, prefix, tab) \
        (((axis2_xml_schema_group_base_t *) group_base)->ops->to_string(group_base, \
            env, prefix, tab))

/*********************** end macros ********************************************/
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_GROUP_BASE_H */
