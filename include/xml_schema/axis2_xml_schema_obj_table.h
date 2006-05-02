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

#ifndef AXIS2_XML_SCHEMA_OBJ_TABLE_H
#define AXIS2_XML_SCHEMA_OBJ_TABLE_H

/**
 * @file axis2_xml_schema_obj_table.h
 * @brief Axis2 Xml Schema Obj Table  interface
 *          A table class that provides read-only helpers for Xml Schema 
 *          Object objects. This class is used to provide the tables for 
 *          contained elements that are within the schema as tables that 
 *          are accessed from the Xml Schema class (for example, Attributes, 
 *          Attribute Groups, Elements, and so on).
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_const.h>
#include <axis2_array_list.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_xml_schema_obj.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_xml_schema_obj_table 
                    axis2_xml_schema_obj_table_t;
typedef struct axis2_xml_schema_obj_table_ops 
                    axis2_xml_schema_obj_table_ops_t;

/** @defgroup axis2_xml_schema_obj_table Xml Schema Obj Table
  * @ingroup axis2_xml_schema
  * @{
  */

struct axis2_xml_schema_obj_table_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (axis2_xml_schema_obj_table_t *obj_table,
            axis2_env_t **env);

    int (AXIS2_CALL *
    get_count) (axis2_xml_schema_obj_table_t *obj_table,
                axis2_env_t **env);

    axis2_xml_schema_obj_t *(AXIS2_CALL *
    get_item) (axis2_xml_schema_obj_table_t *obj_table,
               axis2_env_t **env,
               axis2_qname_t *qname);

    axis2_array_list_t *(AXIS2_CALL *
    get_names) (axis2_xml_schema_obj_table_t *obj_table,
                axis2_env_t **env);

    axis2_array_list_t *(AXIS2_CALL *
    get_values) (axis2_xml_schema_obj_table_t *obj_table,
                 axis2_env_t **env);

    axis2_bool_t (AXIS2_CALL *
    contains) (axis2_xml_schema_obj_table_t *obj_table,
               axis2_env_t **env,
               axis2_qname_t *qname);

    axis2_status_t (AXIS2_CALL *
    add) (axis2_xml_schema_obj_table_t *obj_table,
          axis2_env_t **env,
          axis2_qname_t *qname,
          axis2_xml_schema_obj_t *value);
};

struct axis2_xml_schema_obj_table
{
    axis2_xml_schema_obj_table_ops_t *ops;
};

AXIS2_DECLARE(axis2_xml_schema_obj_table_t *)
axis2_xml_schema_obj_table_create(axis2_env_t **env);

/********************* macros ************************************************/
#define AXIS2_XML_SCHEMA_OBJ_TABLE_FREE(obj_table, env) \
		((obj_table)->ops->free (obj_table, env))

#define AXIS2_XML_SCHEMA_OBJ_TABLE_GET_COUNT(obj_table, env) \
		((obj_table)->ops->get_count (obj_table, env))

#define AXIS2_XML_SCHEMA_OBJ_TABLE_GET_ITEM(obj_table, env, qname) \
		((obj_table)->ops->get_item (obj_table, env, qname))

#define AXIS2_XML_SCHEMA_OBJ_TABLE_GET_NAMES(obj_table, env) \
		((obj_table)->ops->get_names (obj_table, env))

#define AXIS2_XML_SCHEMA_OBJ_TABLE_GET_VALUES(obj_table, env) \
		((obj_table)->ops->get_values (obj_table, env))

#define AXIS2_XML_SCHEMA_OBJ_TABLE_CONTAINS(obj_table, env, qname) \
		((obj_table)->ops->contains (obj_table, env, qname))

#define AXIS2_XML_SCHEMA_OBJ_TABLE_ADD(obj_table, env, value) \
		((obj_table)->ops->add (obj_table, env, value))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_OBJ_TABLE_H */
