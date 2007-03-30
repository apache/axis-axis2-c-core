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

#ifndef XML_SCHEMA_OBJ_TABLE_H
#define XML_SCHEMA_OBJ_TABLE_H

/** @defgroup xml_schema_obj_table Xml Schema Obj Table
  * @ingroup xml_schema
  * @{
  */
  
/**
 * @file xml_schema_obj_table.h
 * @brief xml_schema_obj_table collection 
 * xml_schema_collection is a collection interface that
 * provides read-only helpers for xml_schema_obj structs.
 * This is used to provide the tables for contained elements that 
 * are within the schema as tables that are accessed from the xml_schema functons
 * (for example, Attributes, Attribute Groups, Elements, and so on).
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axutil_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axutil_array_list.h>
#include <axutil_hash.h>
#include <axis2_qname.h>
#include <xml_schema_obj.h>

#ifdef __cplusplus
extern "C"
{
#endif

/* Type name for struct xml_schema_obj_table */
typedef struct xml_schema_obj_table xml_schema_obj_table_t;

/* Type name for struct xml_schema_obj_table_ops */
typedef struct xml_schema_obj_table_ops xml_schema_obj_table_ops_t;


/**
 * struct xml_schema_obj_table_ops */
 
struct xml_schema_obj_table_ops
{
   /** 
     * Free xml_schema_obj_table struct
     * When freing struct instances, whose references kept 
     * in the table are not freed.
     * @param obj_table pointer to xml_schema_obj_table
     * @param env environment struct
     * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on failure.
     */
    axis2_status_t (AXIS2_CALL *
    free)(
            xml_schema_obj_table_t *obj_table,
            const axutil_env_t *env);
            
    /**
     * Get number of elements in the table 
     * @param obj_table pointer to obj_table struct instance
     * @param env environment 
     * @return Number of elements , 0 if no elements are present
     */
    int (AXIS2_CALL *
    get_count)(
            xml_schema_obj_table_t *obj_table,
            const axutil_env_t *env);
    /**
     * Get an item in the table by giving matching qname 
     * @param obj_table pointer to obj_table struct instance 
     * @param env environment struct
     * @param qname pointer to qname struct
     */
    void *(AXIS2_CALL *
    get_item)(
            xml_schema_obj_table_t *obj_table,
            const axutil_env_t *env,
            const axis2_qname_t *qname);

    /**
     * Get an array_list containing the qnames which
     * are used as keys in the table
     * @param obj_table pointer to xml_schema_obj_table struct instance
     * @param env environment struct
     * @returns axutil_array_list struct instance containing keys 
     */
    axutil_array_list_t *(AXIS2_CALL *
    get_names)(
            xml_schema_obj_table_t *obj_table,
            const axutil_env_t *env);

    /**
     * Get an array list of values stored in the table 
     * @param obj_table pointer struct xml_schema_obj_table
     * @param env environment
     * @returns array_list containing all values in the xml_schema_obj_table
     * The returned values are read only and must not be modified
     */
    axutil_array_list_t *(AXIS2_CALL *
    get_values)(
            xml_schema_obj_table_t *obj_table,
            const axutil_env_t *env);

    /** 
     * Checks whether there is a value stored with a qiven qname 
     * @param obj_table pointer to xml_schema_obj_table
     * @param env environment
     * @param qname pointer to axis2_qname_t 
     * @returns AXIS2_TRUE if value exists, AXIS2_FALSE otherwise
     */
    axis2_bool_t (AXIS2_CALL *
    contains)(
            xml_schema_obj_table_t *obj_table,
            const axutil_env_t *env,
            axis2_qname_t *qname);

    axis2_status_t (AXIS2_CALL *
    add) (
            xml_schema_obj_table_t *obj_table,
            const axutil_env_t *env,
            axis2_qname_t *qname,
            void *value);
          
    axis2_status_t (AXIS2_CALL *
    put)(
            xml_schema_obj_table_t *obj_table,
            const axutil_env_t *env,
            axis2_char_t *key,
            void *value);
         
    axutil_hash_t* (AXIS2_CALL *
    get_hash_table)(
            xml_schema_obj_table_t *obj_table,
            const axutil_env_t *env);
            
    void* (AXIS2_CALL*
    get)(   xml_schema_obj_table_t *obj_table,
            const axutil_env_t *env,
            axis2_char_t *key);            
            
                                           
};

struct xml_schema_obj_table
{
    xml_schema_obj_table_ops_t *ops;
};

AXIS2_EXTERN xml_schema_obj_table_t * AXIS2_CALL
xml_schema_obj_table_create(const axutil_env_t *env);

/********************* macros ************************************************/
#define XML_SCHEMA_OBJ_TABLE_FREE(obj_table, env) \
      ((obj_table)->ops->free (obj_table, env))

#define XML_SCHEMA_OBJ_TABLE_GET_COUNT(obj_table, env) \
      ((obj_table)->ops->get_count (obj_table, env))

#define XML_SCHEMA_OBJ_TABLE_GET_ITEM(obj_table, env, qname) \
      ((obj_table)->ops->get_item (obj_table, env, qname))

#define XML_SCHEMA_OBJ_TABLE_GET_NAMES(obj_table, env) \
      ((obj_table)->ops->get_names (obj_table, env))

#define XML_SCHEMA_OBJ_TABLE_GET_VALUES(obj_table, env) \
      ((obj_table)->ops->get_values (obj_table, env))

#define XML_SCHEMA_OBJ_TABLE_CONTAINS(obj_table, env, qname) \
      ((obj_table)->ops->contains (obj_table, env, qname))

#define XML_SCHEMA_OBJ_TABLE_ADD(obj_table, env, qname, value) \
      ((obj_table)->ops->add (obj_table, env, qname, value))
      
#define XML_SCHEMA_OBJ_TABLE_PUT(obj_table, env, key, value) \
      ((obj_table)->ops->put(obj_table, env, key, value))
      
#define XML_SCHEMA_OBJ_TABLE_GET(obj_table, env, key) \
      ((obj_table)->ops->get(obj_table, env, key))
      
#define XML_SCHEMA_OBJ_TABLE_GET_HASH_TABLE(obj_table, env) \
      ((obj_table)->ops->get_hash_table(obj_table, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_OBJ_TABLE_H */
