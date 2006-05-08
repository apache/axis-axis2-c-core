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

#ifndef AXIS2_XML_SCHEMA_TYPE_H
#define AXIS2_XML_SCHEMA_TYPE_H

/**
 * @file axis2_xml_schema_type.h
 * @brief Axis2 Xml Schema Type Interface
 *          The base class for all simple types and complex types.
 */

#include <xml_schema/axis2_xml_schema_annotated.h>
#include <axis2_hash.h>
#include <axis2_qname.h>

/** @defgroup axis2_xml_schema_type Xml Schema Type
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_type axis2_xml_schema_type_t;
typedef struct axis2_xml_schema_type_ops axis2_xml_schema_type_ops_t;
struct axis2_xml_schema_data_type;
struct axis2_xml_schema_derivation_method;
struct axis2_xml_schema;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_type_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *type,
            axis2_env_t **env);

    axis2_xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl) (void *type,
                    axis2_env_t **env);

    void *(AXIS2_CALL *
    get_base_schema_type) (void *type,
                            axis2_env_t **env);

    struct axis2_xml_schema_data_type *(AXIS2_CALL *
    get_data_type) (void *type,
                    axis2_env_t **env);

    struct axis2_xml_schema_derivation_method *(AXIS2_CALL *
    get_derive_by) (void *type,
                            axis2_env_t **env);

    struct axis2_xml_schema_derivation_method *(AXIS2_CALL *
    get_final) (void *type,
                        axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_final) (void *type,
                axis2_env_t **env,
                struct axis2_xml_schema_derivation_method *final_derivation);

    struct axis2_xml_schema_derivation_method *(AXIS2_CALL *
    get_final_resolved) (void *type,
                        axis2_env_t **env);

    axis2_bool_t (AXIS2_CALL *
    is_mixed) (void *type,
                        axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_mixed) (void *type,
                        axis2_env_t **env,
                        axis2_bool_t is_mixed); 

    axis2_char_t *(AXIS2_CALL *
    get_name) (void *type,
                        axis2_env_t **env); 

    axis2_status_t (AXIS2_CALL *
    set_name) (void *type,
                        axis2_env_t **env,
                        axis2_char_t *name); 

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (void *type,
                        axis2_env_t **env);

};

struct axis2_xml_schema_type
{
    axis2_xml_schema_annotated_t base;
    axis2_xml_schema_type_ops_t *ops;
};

/**
 * Creates new Xml Schema Type
 * Type types are usually interchangeable.
 * A local element declaration or reference to a global element
 * declaration (element), a compositor ( sequence, choice, or all),
 * a reference to a named content model group (group), or an element wildcard 
 * (any).
 */
AXIS2_DECLARE(axis2_xml_schema_type_t *)
axis2_xml_schema_type_create(axis2_env_t **env,
                                struct axis2_xml_schema *schema);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_type_resolve_methods(
                                axis2_xml_schema_type_t *type,
                                axis2_env_t **env,
                                axis2_xml_schema_type_t *type_impl,
                                axis2_hash_t *methods);

#define AXIS2_XML_SCHEMA_TYPE_FREE(type, env) \
		(((axis2_xml_schema_type_t *) type)->ops->\
            free(type, env))

#define AXIS2_XML_SCHEMA_TYPE_GET_BASE_IMPL(type, env) \
		(((axis2_xml_schema_type_t *) type)->ops->\
            get_base_impl(type, env))

#define AXIS2_XML_SCHEMA_TYPE_GET_BASE_SCHEMA_TYPE(type, env) \
		(((axis2_xml_schema_type_t *) type)->ops->\
            get_base_schema_type(type, env))

#define AXIS2_XML_SCHEMA_TYPE_GET_DATA_TYPE(type, env, max_occurs) \
		(((axis2_xml_schema_type_t *) type)->ops->\
            get_data_type(type, env, max_occurs))

#define AXIS2_XML_SCHEMA_TYPE_DERIVE_BY(type, env) \
		(((axis2_xml_schema_type_t *) type)->ops->\
            get_derive_by(type, env))

#define AXIS2_XML_SCHEMA_TYPE_GET_FINAL(type, env) \
		(((axis2_xml_schema_type_t *) type)->ops->\
            get_final(type, env))

#define AXIS2_XML_SCHEMA_TYPE_SET_FINAL(type, env, final_derivation) \
        (((axis2_xml_schema_type_t *) type)->ops->\
            set_final(type, env, final_derivation)) 
            
#define AXIS2_XML_SCHEMA_TYPE_GET_FINAL_RESOLVED(type, env) \
        (((axis2_xml_schema_type_t *) type)->ops->\
            get_final_resolved(type, env))
            
#define AXIS2_XML_SCHEMA_TYPE_IS_MIXED(type, env) \
        (((axis2_xml_schema_type_t *) type)->ops->\
            get_is_mixed(type, env))   
            
#define AXIS2_XML_SCHEMA_TYPE_SET_MIXED(type, env) \
        (((axis2_xml_schema_type_t *) type)->ops->\
            get_set_mixed(type, env))  
            
#define AXIS2_XML_SCHEMA_TYPE_GET_NAME(type, env) \
        (((axis2_xml_schema_type_t *) type)->ops->\
            get_name(type, env))    

#define AXIS2_XML_SCHEMA_TYPE_SET_NAME(type, env, name) \
        (((axis2_xml_schema_type_t *) type)->ops->\
            set_name(type, env, name)) 
            
#define AXIS2_XML_SCHEMA_TYPE_GET_QNAME(type, env) \
        (((axis2_xml_schema_type_t *) type)->ops->\
            get_qname(type, env))    
                                                           
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_TYPE_H */
