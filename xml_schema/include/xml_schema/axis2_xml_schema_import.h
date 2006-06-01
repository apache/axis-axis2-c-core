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

#ifndef AXIS2_XML_SCHEMA_IMPORT_H
#define AXIS2_XML_SCHEMA_IMPORT_H

/**
 * @file axis2_xml_schema_import.h
 * @brief Axis2 Xml Schema Particle Interface
 *          Base class for all import types.
 */

#include <xml_schema/axis2_xml_schema_external.h>
#include <axis2_hash.h>

/** @defgroup axis2_xml_schema_include Xml Schema Particle
  * @ingroup axis2_xml_schema
  * @{
  */

typedef struct axis2_xml_schema_import 
                    axis2_xml_schema_import_t;
typedef struct axis2_xml_schema_import_ops 
                    axis2_xml_schema_import_ops_t;
                    
struct axis2_xml_schema;                    

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_xml_schema_import_ops
{
    axis2_status_t (AXIS2_CALL *
    free) (void *import,
            const axis2_env_t *env);

    axis2_xml_schema_external_t* (AXIS2_CALL *
    get_base_impl)(void *import,
                   const axis2_env_t *env);


    axis2_hash_t* (AXIS2_CALL *
    super_objs)(void *import,
                const axis2_env_t *env);
                
    axis2_xml_schema_types_t (AXIS2_CALL *
    type)(void *import,
                const axis2_env_t *env);                
    
    axis2_char_t* (AXIS2_CALL *
    get_namespace)(void *import,
                const axis2_env_t *env);
                
                
                
    axis2_status_t (AXIS2_CALL *
    set_namespace)(void *import,
                const axis2_env_t *env,
                axis2_char_t *ns);

};

struct axis2_xml_schema_import
{
    axis2_xml_schema_external_t base;
    axis2_xml_schema_import_ops_t *ops;
};

/**
 * Creates new Xml Schema import
 */
AXIS2_EXTERN axis2_xml_schema_import_t * AXIS2_CALL
axis2_xml_schema_import_create(const axis2_env_t *env);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_xml_schema_import_resolve_methods(
                                axis2_xml_schema_import_t *import,
                                const axis2_env_t *env,
                                axis2_xml_schema_import_t *import_impl,
                                axis2_hash_t *methods);
 */
 
 
#define AXIS2_XML_SCHEMA_IMPORT_FREE(import, env) \
		(((axis2_xml_schema_import_t *) import)->ops->\
            free(import, env))

#define AXIS2_XML_SCHEMA_IMPORT_GET_BASE_IMPL(import, env) \
		(((axis2_xml_schema_import_t *) import)->ops->\
            get_base_impl(import, env))
            
#define AXIS2_XML_SCHEMA_IMPORT_TYPE(import, env) \
		(((axis2_xml_schema_import_t *) import)->ops->\
            type(import, env))
            
#define AXIS2_XML_SCHEMA_IMPORT_SUPER_OBJS(import, env) \
		(((axis2_xml_schema_import_t *) import)->ops->\
            super_objs(import, env))                        

#define AXIS2_XML_SCHEMA_IMPORT_GET_NAMESPACE(import, env) \
		(((axis2_xml_schema_import_t *) import)->ops->\
            get_namespace(import, env))

#define AXIS2_XML_SCHEMA_IMPORT_SET_NAMESPACE(import, env, ns) \
		(((axis2_xml_schema_import_t *) import)->ops->\
            set_namespace(import, env, ns))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_IMPORT_H */
