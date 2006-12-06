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

#ifndef XML_SCHEMA_SIMPLE_TYPE_LIST_H
#define XML_SCHEMA_SIMPLE_TYPE_LIST_H

/**
 * @file xml_schema_simple_type_list.h
 * @brief Axis2 Xml Schema Particle Interface
 *          Base class for all simple_type_list types.
 */


#include <xml_schema_simple_type.h>
#include <xml_schema_simple_type_content.h>
#include <xml_schema_obj_collection.h>
#include <axis2_hash.h>
/** @defgroup xml_schema_simple_type_list 
  * @ingroup xml_schema
  * @{
  */
#ifdef __cplusplus
extern "C"
{
#endif

typedef struct xml_schema_simple_type_list 
                    xml_schema_simple_type_list_t;
typedef struct xml_schema_simple_type_list_ops 
                    xml_schema_simple_type_list_ops_t;

struct xml_schema_simple_type_list_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *simple_type_list,
            const axis2_env_t *env);

    xml_schema_simple_type_content_t *(AXIS2_CALL *
    get_base_impl) (void *simple_type_list,
                    const axis2_env_t *env);

    xml_schema_types_t (AXIS2_CALL *
    get_type)(void *simple_type_list,
          const axis2_env_t *env);
    
    axis2_hash_t* (AXIS2_CALL *
    super_objs)(void *simple_type_list,
          const axis2_env_t *env);
          

    xml_schema_simple_type_t *(AXIS2_CALL *
    get_item_type)(void *simple_type_list,
                const axis2_env_t *env);

    /** */
    axis2_status_t (AXIS2_CALL *
    set_item_type)(void *simple_type_list,
                   const axis2_env_t *env,
                   xml_schema_simple_type_t *base_type);
    
    axis2_qname_t* (AXIS2_CALL *
    get_item_type_name)(void *simple_type_list,
                const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_item_type_name)(void *simple_type_list,
                    const axis2_env_t *env,
                    axis2_qname_t *qname);
};

struct xml_schema_simple_type_list
{
    xml_schema_simple_type_content_t base;
    xml_schema_simple_type_list_ops_t *ops;
};

/**
 * Creates new Xml Schema Particle
 * Particle types are usually interchangeable.
 * A local element declaration or reference to a global element
 * declaration (element), a compositor ( sequence, choice, or all),
 * a reference to a named content model group (group), or an element wildcard 
 * (any).
 */
AXIS2_EXTERN xml_schema_simple_type_list_t * AXIS2_CALL
xml_schema_simple_type_list_create(const axis2_env_t *env);

 /********************** Macros ***********************************************/
 
#define XML_SCHEMA_SIMPLE_TYPE_LIST_FREE(simple_type_list, env) \
      (((xml_schema_simple_type_list_t *) simple_type_list)->ops->\
            free(simple_type_list, env))

#define XML_SCHEMA_SIMPLE_TYPE_LIST_GET_BASE_IMPL(simple_type_list, env) \
      (((xml_schema_simple_type_list_t *) simple_type_list)->ops->\
            get_base_impl(simple_type_list, env))
            
#define XML_SCHEMA_SIMPLE_TYPE_LIST_TYPE(simple_type_list, env) \
      (((xml_schema_simple_type_list_t *) simple_type_list)->ops->\
            get_type(simple_type_list, env))
            
#define XML_SCHEMA_SIMPLE_TYPE_LIST_SUPER_OBJS(simple_type_list, env) \
      (((xml_schema_simple_type_list_t *) simple_type_list)->ops->\
            super_objs(simple_type_list, env))                        

#define XML_SCHEMA_SIMPLE_TYPE_LIST_GET_ITEM_TYPE(simple_type_list, env) \
      (((xml_schema_simple_type_list_t *) simple_type_list)->ops->\
            get_item_type(simple_type_list, env))

#define XML_SCHEMA_SIMPLE_TYPE_LIST_SET_ITEM_TYPE(simple_type_list,\
         env, base_type) \
      (((xml_schema_simple_type_list_t *) simple_type_list)->ops->\
            set_item_type(simple_type_list, env, base_type))

#define XML_SCHEMA_SIMPLE_TYPE_LIST_GET_ITEM_TYPE_NAME(simple_type_list, env) \
      (((xml_schema_simple_type_list_t *) simple_type_list)->ops->\
            get_item_type_name(simple_type_list, env))

#define XML_SCHEMA_SIMPLE_TYPE_LIST_SET_ITEM_TYPE_NAME(simple_type_list, \
         env, base_type_name) \
      (((xml_schema_simple_type_list_t *) simple_type_list)->ops->\
            set_item_type_name(simple_type_list, env, base_type_name))

/************************ end macros **********************************************/
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_SIMPLE_TYPE_LIST_H */
