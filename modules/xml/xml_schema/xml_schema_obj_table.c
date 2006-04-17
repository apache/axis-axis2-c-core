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
 
#include <axis2_xml_schema_obj_table.h>
#include <axis2_hash.h>

typedef struct axis2_xml_schema_obj_table_impl axis2_xml_schema_obj_table_impl_t;

/** 
 * @brief Xml Schema Obj Struct Impl
 *	Axis2 Xml Schema Obj  
 */ 
struct axis2_xml_schema_obj_table_impl
{
    axis2_xml_schema_obj_table_t obj_table;
    axis2_hash_t *collection;
    axis2_array_list_t *names;
    axis2_array_list_t *values;
};

#define INTF_TO_IMPL(obj_table) ((axis2_xml_schema_obj_table_impl_t *) obj_table)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_obj_table_free(void *obj_table,
                axis2_env_t **env);

int AXIS2_CALL
axis2_xml_schema_obj_table_get_count(void *obj_table,
                                            axis2_env_t **env);

axis2_xml_schema_obj_t *AXIS2_CALL
axis2_xml_schema_obj_table_get_item(void *obj_table,
                                            axis2_env_t **env,
                                            axis2_qname_t *qname);

axis2_array_list_t *AXIS2_CALL
axis2_xml_schema_obj_table_get_names(void *obj_table,
                            axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL
axis2_xml_schema_obj_table_get_values(void *obj_table,
                            axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_xml_schema_obj_table_contains(void *obj_table,
                                            axis2_env_t **env,
                                            axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
axis2_xml_schema_obj_table_add(void *obj_table,
                                    axis2_env_t **env,
                                    axis2_qname_t *qname,
                                    axis2_xml_schema_obj_t *value);

AXIS2_DECLARE(axis2_xml_schema_obj_table_t *)
axis2_xml_schema_obj_table_create(axis2_env_t **env)
{
    axis2_xml_schema_obj_table_impl_t *obj_table_impl = NULL;
    
    obj_table_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_obj_table_impl_t));

    obj_table_impl->collection = NULL;
    obj_table_impl->names = NULL;
    obj_table_impl->values = NULL;

    obj_table_impl->obj_table.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_obj_table_ops_t)); 

    obj_table_impl->collection = axis2_hash_make(env);
    if(!obj_table_impl->collection)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    obj_table_impl->obj_table.ops->free = 
            axis2_xml_schema_obj_table_free;
    obj_table_impl->obj_table.ops->get_count = 
            axis2_xml_schema_obj_table_get_count;
    obj_table_impl->obj_table.ops->get_item = 
            axis2_xml_schema_obj_table_get_item;
    obj_table_impl->obj_table.ops->get_names = 
            axis2_xml_schema_obj_table_get_names;
    obj_table_impl->obj_table.ops->get_values = 
            axis2_xml_schema_obj_table_get_values;
    obj_table_impl->obj_table.ops->contains = 
            axis2_xml_schema_obj_table_contains;
    obj_table_impl->obj_table.ops->add = 
            axis2_xml_schema_obj_table_add;

    return &(obj_table_impl->obj_table);
}


axis2_status_t AXIS2_CALL
axis2_xml_schema_obj_table_free(void *obj_table,
                                        axis2_env_t **env)
{
    axis2_xml_schema_obj_table_impl_t *obj_table_impl = NULL;

    obj_table_impl = INTF_TO_IMPL(obj_table);

    if(obj_table_impl->names)
    {
        AXIS2_ARRAY_LIST_FREE(obj_table_impl->names, env);
        obj_table_impl->names = NULL;
    }

    if(obj_table_impl->values)
    {
        AXIS2_ARRAY_LIST_FREE(obj_table_impl->values, env);
        obj_table_impl->values = NULL;
    }

    if(obj_table_impl->collection)
    {
        axis2_hash_free(obj_table_impl->collection, env);
        obj_table_impl->collection = NULL;
    }

    if((&(obj_table_impl->obj_table))->ops)
    {
        free((&(obj_table_impl->obj_table))->ops);
        (&(obj_table_impl->obj_table))->ops = NULL;
    }

    if(obj_table_impl)
    {
        free(obj_table_impl);
        obj_table_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_obj_table_resolve_methods(
                        axis2_xml_schema_obj_table_t *obj_table,
                        axis2_env_t **env,
                        axis2_xml_schema_obj_table_t *obj_table_impl,
                        axis2_hash_t *methods)
{    
    axis2_xml_schema_obj_table_impl_t *obj_table_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, obj_table_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    obj_table_impl_l = (axis2_xml_schema_obj_table_impl_t *) obj_table_impl;
    
    obj_table->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_obj_table_ops_t));
    obj_table->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    obj_table->ops->get_count = 
            obj_table_impl_l->obj_table.ops->get_count;
    obj_table->ops->get_item = 
            obj_table_impl_l->obj_table.ops->get_item; 
    obj_table->ops->get_names = 
            obj_table_impl_l->obj_table.ops->get_names; 
    obj_table->ops->get_values = 
            obj_table_impl_l->obj_table.ops->get_values;
    obj_table->ops->contains = 
            obj_table_impl_l->obj_table.ops->contains;
    obj_table->ops->add = 
            obj_table_impl_l->obj_table.ops->add; 
    return AXIS2_SUCCESS;    

}

int AXIS2_CALL
axis2_xml_schema_obj_table_get_count(void *obj_table,
                                            axis2_env_t **env)
{
    axis2_xml_schema_obj_table_impl_t *obj_table_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    obj_table_impl = INTF_TO_IMPL(obj_table);
    return axis2_hash_count(obj_table_impl->collection);
}

axis2_xml_schema_obj_t *AXIS2_CALL
axis2_xml_schema_obj_table_get_item(void *obj_table,
                                            axis2_env_t **env,
                                            axis2_qname_t *qname)
{
    axis2_xml_schema_obj_table_impl_t *obj_table_impl = NULL;
    axis2_char_t *name = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    obj_table_impl = INTF_TO_IMPL(obj_table);

    name = AXIS2_QNAME_TO_STRING(qname, env);
    return axis2_hash_get(obj_table_impl->collection, name, 
            AXIS2_HASH_KEY_STRING);
}

axis2_array_list_t *AXIS2_CALL
axis2_xml_schema_obj_table_get_names(void *obj_table,
                            axis2_env_t **env)
{
    axis2_xml_schema_obj_table_impl_t *obj_table_impl = NULL;
    axis2_hash_index_t *hi = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    if(obj_table_impl->names)
    {
       int i = 0;
       int size = 0;

       size = AXIS2_ARRAY_LIST_SIZE(obj_table_impl->names, env);
       for (i = 0; i < size; i++)
       {
            AXIS2_FREE((*env)->allocator, AXIS2_ARRAY_LIST_GET(obj_table_impl->
                        names, env, i));
       }
    }
    else
    {
        obj_table_impl->names = axis2_array_list_create(env, 0);
        if(!obj_table_impl->names)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
    }
    for (hi = axis2_hash_first (obj_table_impl->collection, env); hi;
             hi = axis2_hash_next ( env, hi))
    {
        void *val = NULL;

        axis2_hash_this (hi, &val, NULL, NULL);
        AXIS2_ARRAY_LIST_ADD(obj_table_impl->names, env, val);
        val = NULL;
           
    }
    return obj_table_impl->names;
}

axis2_array_list_t *AXIS2_CALL
axis2_xml_schema_obj_table_get_values(void *obj_table,
                            axis2_env_t **env)
{
    axis2_xml_schema_obj_table_impl_t *obj_table_impl = NULL;
    axis2_hash_index_t *hi = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    if(!obj_table_impl->values)
    {
        obj_table_impl->values = axis2_array_list_create(env, 0);
        if(!obj_table_impl->values)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
    }
    for (hi = axis2_hash_first (obj_table_impl->collection, env); hi;
             hi = axis2_hash_next ( env, hi))
    {
        void *val = NULL;

        axis2_hash_this (hi, NULL, NULL, &val);
        AXIS2_ARRAY_LIST_ADD(obj_table_impl->values, env, val);
        val = NULL;
           
    }
    return obj_table_impl->values;
}

axis2_bool_t AXIS2_CALL
axis2_xml_schema_obj_table_contains(void *obj_table,
                                            axis2_env_t **env,
                                            axis2_qname_t *qname)
{
    axis2_xml_schema_obj_table_impl_t *obj_table_impl = NULL;
    void *value = NULL;
    axis2_char_t *name = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    obj_table_impl = INTF_TO_IMPL(obj_table);
    
    name = AXIS2_QNAME_TO_STRING(qname, env);
    value = axis2_hash_get(obj_table_impl->collection, name, 
            AXIS2_HASH_KEY_STRING);
    if(value) return AXIS2_TRUE;
    else return AXIS2_FALSE;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_obj_table_add(void *obj_table,
                                    axis2_env_t **env,
                                    axis2_qname_t *qname,
                                    axis2_xml_schema_obj_t *value)
{
    axis2_xml_schema_obj_table_impl_t *obj_table_impl = NULL;
    axis2_char_t *name = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
    obj_table_impl = INTF_TO_IMPL(obj_table);

    name = AXIS2_QNAME_TO_STRING(qname, env);
    axis2_hash_set(obj_table_impl->collection, name, AXIS2_HASH_KEY_STRING, value);
    return AXIS2_SUCCESS;
}


