/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef XML_SCHEMA_TYPE_H
#define XML_SCHEMA_TYPE_H

/**
 * @file xml_schema_type.h
 * @brief Axis2 Xml Schema Type Interface
 *          The base class for all simple types and complex types.
 */

#include <xml_schema_annotated.h>
#include <axis2_hash.h>
#include <axis2_qname.h>

/** @defgroup xml_schema_type Xml Schema Type
  * @ingroup xml_schema
  * @{
  */

typedef struct xml_schema_type xml_schema_type_t;
typedef struct xml_schema_type_ops xml_schema_type_ops_t;
struct xml_schema_data_type;
struct xml_schema_derivation_method;
struct xml_schema;

#ifdef __cplusplus
extern "C"
{
#endif

struct xml_schema_type_ops
{
    axis2_status_t (AXIS2_CALL *
    free) (void *type,
            const axis2_env_t *env);

    xml_schema_annotated_t *(AXIS2_CALL *
    get_base_impl) (void *type,
                    const axis2_env_t *env);
                    
    axis2_hash_t* (AXIS2_CALL *
    super_objs)(void *type,
                const axis2_env_t *env);
                   
    xml_schema_types_t (AXIS2_CALL *
    get_type)(void *type,
          const axis2_env_t *env);                                                  

    void *(AXIS2_CALL *
    get_base_schema_type) (void *type,
                            const axis2_env_t *env);

    struct xml_schema_data_type *(AXIS2_CALL *
    get_data_type) (void *type,
                    const axis2_env_t *env);

    struct xml_schema_derivation_method *(AXIS2_CALL *
    get_derive_by) (void *type,
                            const axis2_env_t *env);

    struct xml_schema_derivation_method *(AXIS2_CALL *
    get_final) (void *type,
                        const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_final) (void *type,
                const axis2_env_t *env,
                struct xml_schema_derivation_method *final_derivation);

    struct xml_schema_derivation_method *(AXIS2_CALL *
    get_final_resolved) (void *type,
                        const axis2_env_t *env);

    axis2_bool_t (AXIS2_CALL *
    is_mixed) (void *type,
                        const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_mixed) (void *type,
                        const axis2_env_t *env,
                        axis2_bool_t is_mixed); 

    axis2_char_t *(AXIS2_CALL *
    get_name) (void *type,
                        const axis2_env_t *env); 

    axis2_status_t (AXIS2_CALL *
    set_name) (void *type,
                        const axis2_env_t *env,
                        axis2_char_t *name); 

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (void *type,
                        const axis2_env_t *env);

};

struct xml_schema_type
{
    xml_schema_annotated_t base;
    xml_schema_type_ops_t *ops;
};

/**
 * Creates new Xml Schema Type
 * Type types are usually interchangeable.
 * A local element declaration or reference to a global element
 * declaration (element), a compositor ( sequence, choice, or all),
 * a reference to a named content model group (group), or an element wildcard 
 * (any).
 */
AXIS2_EXTERN xml_schema_type_t * AXIS2_CALL
xml_schema_type_create(const axis2_env_t *env,
                                struct xml_schema *schema);

/**
 * This method is internal to Axis2 C. It is called from Child Constructor
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
xml_schema_type_resolve_methods(
                                xml_schema_type_t *type,
                                const axis2_env_t *env,
                                xml_schema_type_t *type_impl,
                                XML_SCHEMA_SUPER_OBJS_FN super_objs,
                                XML_SCHEMA_GET_TYPE_FN get_type,
                                XML_SCHEMA_FREE_FN free_fn);

#define XML_SCHEMA_TYPE_FREE(type, env) \
      (((xml_schema_type_t *) type)->ops->\
            free(type, env))

#define XML_SCHEMA_TYPE_GET_BASE_IMPL(type, env) \
      (((xml_schema_type_t *) type)->ops->\
            get_base_impl(type, env))

#define XML_SCHEMA_TYPE_GET_TYPE(sch_type, env) \
      (((xml_schema_type_t *) sch_type)->ops->\
           get_type(sch_type, env))

           
#define XML_SCHEMA_TYPE_SUPER_OBJS(type, env) \
      (((xml_schema_type_t *) type)->ops->\
           super_objs(type, env))                       

#define XML_SCHEMA_TYPE_GET_BASE_SCHEMA_TYPE(type, env) \
      (((xml_schema_type_t *) type)->ops->\
            get_base_schema_type(type, env))

#define XML_SCHEMA_TYPE_GET_DATA_TYPE(type, env, max_occurs) \
      (((xml_schema_type_t *) type)->ops->\
            get_data_type(type, env, max_occurs))

#define XML_SCHEMA_TYPE_DERIVE_BY(type, env) \
      (((xml_schema_type_t *) type)->ops->\
            get_derive_by(type, env))

#define XML_SCHEMA_TYPE_GET_FINAL(type, env) \
      (((xml_schema_type_t *) type)->ops->\
            get_final(type, env))

#define XML_SCHEMA_TYPE_SET_FINAL(type, env, final_derivation) \
        (((xml_schema_type_t *) type)->ops->\
            set_final(type, env, final_derivation)) 
            
#define XML_SCHEMA_TYPE_GET_FINAL_RESOLVED(type, env) \
        (((xml_schema_type_t *) type)->ops->\
            get_final_resolved(type, env))
            
#define XML_SCHEMA_TYPE_IS_MIXED(type, env) \
        (((xml_schema_type_t *) type)->ops->\
            get_is_mixed(type, env))   
            
#define XML_SCHEMA_TYPE_SET_MIXED(type, env) \
        (((xml_schema_type_t *) type)->ops->\
            get_set_mixed(type, env))  
            
#define XML_SCHEMA_TYPE_GET_NAME(type, env) \
        (((xml_schema_type_t *) type)->ops->\
            get_name(type, env))    

#define XML_SCHEMA_TYPE_SET_NAME(type, env, name) \
        (((xml_schema_type_t *) type)->ops->\
            set_name(type, env, name)) 
            
#define XML_SCHEMA_TYPE_GET_QNAME(type, env) \
        (((xml_schema_type_t *) type)->ops->\
            get_qname(type, env))    
                                                           
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_TYPE_H */
