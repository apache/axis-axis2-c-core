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

#ifndef AXIS2_XML_SCHEMA_GROUP_REF_H
#define AXIS2_XML_SCHEMA_GROUP_REF_H

/**
 * @file axis2_xml_schema_group_ref.h
 * @brief Axis2 Xml Schema Group Ref Interface
 *          Class used within complex types that defines the reference to
 *          groups defined at the schema level. Represents the World Wide
 *          Web Consortium (W3C) group element with ref attribute.
 */

#include <axis2_xml_schema_particle.h>
#include <axis2_hash.h>
#include <axis2_qname.h>

/** @defgroup_ref axis2_xml_schema_group_ref Xml Schema Group Ref
  * @ingroup_ref axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_group_ref axis2_xml_schema_group_ref_t;
typedef struct axis2_xml_schema_group_ref_ops axis2_xml_schema_group_ref_ops_t;
struct axis2_xml_schema_group_base;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_group_ref_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *group_ref,
            axis2_env_t **env);

    axis2_xml_schema_particle_t *(AXIS2_CALL *
    get_base_impl) (void *group_ref,
                    axis2_env_t **env);

    axis2_qname_t *(AXIS2_CALL *
    get_ref_qname)(void *group_ref,
                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_ref_qname)(void *group_ref,
                    axis2_env_t **env,
                    axis2_qname_t *ref_qname);
    
    struct axis2_xml_schema_group_base *(AXIS2_CALL *
    get_particle)(void *group_ref,
                    axis2_env_t **env);
    
};

struct axis2_xml_schema_group_ref
{
    axis2_xml_schema_particle_t base;
    axis2_xml_schema_group_ref_ops_t *ops;
};

/**
 * Creates new Xml Schema Group Ref
 */
AXIS2_DECLARE(axis2_xml_schema_group_ref_t *)
axis2_xml_schema_group_ref_create(axis2_env_t **env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_group_ref_resolve_methods(
                                axis2_xml_schema_group_ref_t *group_ref,
                                axis2_env_t **env,
                                axis2_xml_schema_group_ref_t *group_ref_impl,
                                axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_GROUP_REF_FREE(group_ref, env) \
		(((axis2_xml_schema_group_ref_t *) group_ref)->ops->\
            free(group_ref, env))

#define AXIS2_XML_SCHEMA_GROUP_REF_GET_BASE_IMPL(group_ref, env) \
		(((axis2_xml_schema_group_ref_t *) group_ref)->ops->\
            get_base_impl(group_ref, env))

#define AXIS2_XML_SCHEMA_GROUP_REF_GET_REF_QNAME(group_ref, env) \
		(((axis2_xml_schema_group_ref_t *) group_ref)->ops->\
            get_ref_qname(group_ref, env))

#define AXIS2_XML_SCHEMA_GROUP_REF_SET_REF_QNAME(group_ref, env, ref_qname) \
		(((axis2_xml_schema_group_ref_t *) group_ref)->ops->\
            set_ref_qname(group_ref, env, ref_qname))

#define AXIS2_XML_SCHEMA_GROUP_REF_GET_PARTICLE(group_ref, env) \
		(((axis2_xml_schema_group_ref_t *) group_ref)->ops->\
            get_particle(group_ref, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_GROUP_REF_H */
