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
#include <axis2_qname.h>

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
    free) (void *id_constr,
            axis2_env_t **env);

    axis2_xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl) (void *id_constr,
                    axis2_env_t **env);
                    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *id_constr,
                axis2_env_t **env);
                
                        
    axis2_xml_schema_types_t (AXIS2_CALL *
    type) (void *id_constr,
            axis2_env_t **env);   
        
    /** xml schema keyref method */            
    axis2_qname_t* (AXIS2_CALL *
    get_refer)(void *id_constr,
               axis2_env_t **env);

    /** xml schema keyref method */               
    axis2_status_t (AXIS2_CALL *                                                                    set_refer)(void *id_constr,
               axis2_env_t **env,
               axis2_qname_t *refer);

    struct axis2_xml_schema_obj_collection *(AXIS2_CALL *
    get_fields)(void *id_constr,
                axis2_env_t **env);

    axis2_char_t *(AXIS2_CALL *
    get_name)(void *id_constr,
                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_name)(void *id_constr,
                    axis2_env_t **env,
                    axis2_char_t *name);
    
    struct axis2_xml_schema_xpath *(AXIS2_CALL *
    get_selector)(void *id_constr,
                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_selector)(void *id_constr,
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

AXIS2_DECLARE(axis2_xml_schema_identity_constraint_t *)
axis2_xml_schema_unique_create(axis2_env_t **env);

AXIS2_DECLARE(axis2_xml_schema_identity_constraint_t *)
axis2_xml_schema_keyref_create(axis2_env_t **env);

AXIS2_DECLARE(axis2_xml_schema_identity_constraint_t *)
axis2_xml_schema_key_create(axis2_env_t **env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_identity_constraint_resolve_methods(
                axis2_xml_schema_identity_constraint_t *identity_constraint,
                axis2_env_t **env,
                axis2_xml_schema_identity_constraint_t *identity_constraint_impl,
                axis2_hash_t *methods);
*/                
/**************** Macros *****************************************************/                

#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_FREE(id_constr, env) \
		(((axis2_xml_schema_identity_constraint_t *) id_constr)->ops->\
            free(id_constr, env))

#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_GET_BASE_IMPL(id_constr, env) \
		(((axis2_xml_schema_identity_constraint_t *) id_constr)->ops->\
            get_base_impl(id_constr, env))


#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_TYPE(id_constr, env) \
		(((axis2_xml_schema_identity_constraint_t *) id_constr)->ops->\
            type(id_constr, env))

#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_SUPER_OBJS(id_constr, env) \
		(((axis2_xml_schema_identity_constraint_t *) id_constr)->ops->\
            super_objs(id_constr, env))

#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_GET_REFER(id_constr, env) \
		(((axis2_xml_schema_identity_constraint_t *) id_constr)->ops->\
            get_refer(id_constr, env))
            
#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_SET_REFER(id_constr, env, refer) \
		(((axis2_xml_schema_identity_constraint_t *) id_constr)->ops->\
            set_refer(id_constr, env, refer))            

#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_GET_FIELDS(id_constr, env) \
		(((axis2_xml_schema_identity_constraint_t *) id_constr)->ops->\
            get_fields(id_constr, env))

#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_GET_NAME(id_constr, env) \
		(((axis2_xml_schema_identity_constraint_t *) id_constr)->ops->\
            get_name(id_constr, env))

#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_SET_NAME(id_constr, env, name) \
		(((axis2_xml_schema_identity_constraint_t *) id_constr)->ops->\
            set_name(id_constr, env, name))

#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_GET_SELECTOR(id_constr, env) \
		(((axis2_xml_schema_identity_constraint_t *) id_constr)->ops->\
            get_selector(id_constr, env))

#define AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_SET_SELECTOR(id_constr, env, selector) \
		(((axis2_xml_schema_identity_constraint_t *) id_constr)->ops->\
            set_selector(id_constr, env, selector))

/********************** macros **************************************************/
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT_H */
