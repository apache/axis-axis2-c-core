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

#ifndef AXIS2_XML_SCHEMA_OBJ_COLLECTION_H
#define AXIS2_XML_SCHEMA_OBJ_COLLECTION_H

/**
 * @file axis2_xml_schema_obj_collection.h
 * @brief Axis2 Xml Schema Obj Collection  interface
 *          An object collection class to handle xml_schema_objs when 
 *          collections are returned from method calls.
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_const.h>
#include <axis2_array_list.h>
#include <axis2_hash.h>
#include <axis2_xml_schema_obj.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_xml_schema_obj_collection 
                    axis2_xml_schema_obj_collection_t;
typedef struct axis2_xml_schema_obj_collection_ops 
                    axis2_xml_schema_obj_collection_ops_t;

/** @defgroup axis2_xml_schema_obj_collection Xml Schema Obj Collection
  * @ingroup axis2_xml_schema
  * @{
  */

struct axis2_xml_schema_obj_collection_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (axis2_xml_schema_obj_collection_t *obj_collection,
            axis2_env_t **env);

    int (AXIS2_CALL *
    get_count) (axis2_xml_schema_obj_collection_t *obj_collection,
                    axis2_env_t **env);

    axis2_xml_schema_obj_t *(AXIS2_CALL *
    get_item) (axis2_xml_schema_obj_collection_t *obj_collection,
                    axis2_env_t **env,
                    int i);

    axis2_status_t (AXIS2_CALL *
    set_item) (axis2_xml_schema_obj_collection_t *obj_collection,
                    axis2_env_t **env,
                    int i,
                    axis2_xml_schema_obj_t *item);

    axis2_status_t (AXIS2_CALL *
    add) (axis2_xml_schema_obj_collection_t *obj_collection,
                    axis2_env_t **env,
                    axis2_xml_schema_obj_t *item);

    axis2_bool_t (AXIS2_CALL *
    contains) (axis2_xml_schema_obj_collection_t *obj_collection,
                        axis2_env_t **env,
                        axis2_xml_schema_obj_t *item);

    axis2_status_t (AXIS2_CALL *
    index_of) (axis2_xml_schema_obj_collection_t *obj_collection,
                        axis2_env_t **env,
                        axis2_xml_schema_obj_t *item);

    void* (AXIS2_CALL *
    remove_at) (axis2_xml_schema_obj_collection_t *obj_collection,
                axis2_env_t **env,
                int i);
                
    axis2_array_list_t* (AXIS2_CALL *
    to_array)(axis2_xml_schema_obj_collection_t *obj_collection,
              axis2_env_t **env);                

};

struct axis2_xml_schema_obj_collection
{
    axis2_xml_schema_obj_collection_ops_t *ops;
};

AXIS2_DECLARE(axis2_xml_schema_obj_collection_t *)
axis2_xml_schema_obj_collection_create(axis2_env_t **env);

/******************* MACROS ***************************************************/
#define AXIS2_XML_SCHEMA_OBJ_COLLECTION_FREE(obj_collection, env) \
		((obj_collection)->ops->free (obj_collection, env))

#define AXIS2_XML_SCHEMA_OBJ_COLLECTION_GET_COUNT(obj_collection, env) \
		((obj_collection)->ops->get_count (obj_collection, env))

#define AXIS2_XML_SCHEMA_OBJ_COLLECTION_GET_ITEM(obj_collection, env, i) \
		((obj_collection)->ops->get_item (obj_collection, env, i))

#define AXIS2_XML_SCHEMA_OBJ_COLLECTION_SET_ITEM(obj_collection, env, i, item) \
		((obj_collection)->ops->set_item (obj_collection, env, i, item))

#define AXIS2_XML_SCHEMA_OBJ_COLLECTION_ADD(obj_collection, env, item) \
		((obj_collection)->ops->add (obj_collection, env, item))

#define AXIS2_XML_SCHEMA_OBJ_COLLECTION_CONTAINS(obj_collection, env, item) \
		((obj_collection)->ops->contains (obj_collection, env, item))

#define AXIS2_XML_SCHEMA_OBJ_COLLECTION_INDEX_OF(obj_collection, env, item) \
		((obj_collection)->ops->index_of (obj_collection, env, item))

#define AXIS2_XML_SCHEMA_OBJ_COLLECTION_REMOVE_AT(obj_collection, env, i) \
		((obj_collection)->ops->remove_at (obj_collection, env, i))

#define AXIS2_XML_SCHEMA_OBJ_COLLECTION_TO_ARRAY(obj_collection, env) \
        ((obj_collection)->ops->to_array(obj_collection, env))
        
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_OBJ_COLLECTION_H */
