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

#ifndef AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_H
#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_H

/**
 * @file axis2_xml_schema_identity_constraint.h
 * @brief Axis2 Xml Schema Identity Constraint Interface
 *          Class for the identity constraints: key, keyref, and unique elements.
 */

#include <xml_schema/axis2_xml_schema_annotated.h>
#include <axis2_hash.h>

/** @defidentity_constraint axis2_xml_schema_identity_constraint Xml Schema 
  *  Identity Constraint
  * @inidentity_constraint axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_identity_constraint 
        axis2_xml_schema_identity_constraint_t;
typedef struct axis2_xml_schema_identity_constraint_ops 
        axis2_xml_schema_identity_constraint_ops_t;
struct axis2_xml_schema_xpath;
struct axis2_xml_schema_obj_collection;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_identity_constraint_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *identity_constraint,
            axis2_env_t **env);

    axis2_xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl) (void *identity_constraint,
                    axis2_env_t **env);

    struct axis2_xml_schema_obj_collection *(AXIS2_CALL *
    get_fields)(void *identity_constraint,
                axis2_env_t **env);

    axis2_char_t *(AXIS2_CALL *
    get_name)(void *identity_constraint,
                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_name)(void *identity_constraint,
                    axis2_env_t **env,
                    axis2_char_t *name);
    
    struct axis2_xml_schema_xpath *(AXIS2_CALL *
    get_selector)(void *identity_constraint,
                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_selector)(void *identity_constraint,
                    axis2_env_t **env,
                    struct axis2_xml_schema_xpath *selector);
};

struct axis2_xml_schema_identity_constraint
{
    axis2_xml_schema_annotated_t base;
    axis2_xml_schema_identity_constraint_ops_t *ops;
};

/**
 * Creates new Xml Schema Identity Constraint
 */
AXIS2_DECLARE(axis2_xml_schema_identity_constraint_t *)
axis2_xml_schema_identity_constraint_create(axis2_env_t **env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_identity_constraint_resolve_methods(
                axis2_xml_schema_identity_constraint_t *identity_constraint,
                axis2_env_t **env,
                axis2_xml_schema_identity_constraint_t *identity_constraint_impl,
                axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_FREE(identity_constraint, env) \
		(((axis2_xml_schema_identity_constraint_t *) identity_constraint)->ops->\
            free(identity_constraint, env))

#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_GET_BASE_IMPL(identity_constraint, env) \
		(((axis2_xml_schema_identity_constraint_t *) identity_constraint)->ops->\
            get_base_impl(identity_constraint, env))

#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_GET_FIELDS(identity_constraint, env) \
		(((axis2_xml_schema_identity_constraint_t *) identity_constraint)->ops->\
            get_fields(identity_constraint, env))

#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_GET_NAME(identity_constraint, env) \
		(((axis2_xml_schema_identity_constraint_t *) identity_constraint)->ops->\
            get_name(identity_constraint, env))

#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_SET_NAME(identity_constraint, env, name) \
		(((axis2_xml_schema_identity_constraint_t *) identity_constraint)->ops->\
            set_name(identity_constraint, env, name))

#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_GET_SELECTOR(identity_constraint, env) \
		(((axis2_xml_schema_identity_constraint_t *) identity_constraint)->ops->\
            get_selector(identity_constraint, env))

#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_SET_SELECTOR(identity_constraint, env, selector) \
		(((axis2_xml_schema_identity_constraint_t *) identity_constraint)->ops->\
            set_selector(identity_constraint, env, selector))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_H */
