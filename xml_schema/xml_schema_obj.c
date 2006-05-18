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
 
#include <xml_schema/axis2_xml_schema_obj.h>
#include <axis2_hash.h>
#include <axis2_utils.h>

typedef struct axis2_xml_schema_obj_impl 
                    axis2_xml_schema_obj_impl_t;

/** 
 * @brief Xml Schema Obj Struct Impl
 *	Axis2 Xml Schema Obj  
 */ 
struct axis2_xml_schema_obj_impl
{
    axis2_xml_schema_obj_t obj;
    
    axis2_xml_schema_types_t obj_type;
    
    axis2_hash_t *ht_super;
    
    int line_num;
    
    int line_pos;
    
    axis2_char_t *source_uri;
};

#define AXIS2_INTF_TO_IMPL(obj) \
        ((axis2_xml_schema_obj_impl_t *) obj)

/***************** function pointers ******************************************/

axis2_status_t AXIS2_CALL 
axis2_xml_schema_obj_free(
        void *obj,
        axis2_env_t **env);

axis2_hash_t *AXIS2_CALL 
axis2_xml_schema_obj_super_objs(
        void *obj,
        axis2_env_t **env);

axis2_xml_schema_types_t AXIS2_CALL 
axis2_xml_schema_obj_type(
        void *obj,
        axis2_env_t **env);

axis2_char_t * AXIS2_CALL
axis2_xml_schema_obj_get_source_uri(
        void *obj,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_obj_set_source_uri(
        void *obj,
        axis2_env_t **env,
        axis2_char_t *source_uri);

axis2_bool_t AXIS2_CALL
axis2_xml_schema_obj_equals(
        void *obj,
        axis2_env_t **env,
        void *obj_comp);

int AXIS2_CALL
axis2_xml_schema_obj_get_line_num(
        void *obj,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_obj_set_line_num(
        void *obj,
        axis2_env_t **env,
        int line_num);

int AXIS2_CALL
axis2_xml_schema_obj_get_line_pos(
        void *obj,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_obj_set_line_pos(
        void *obj,
        axis2_env_t **env,
        int line_pos);

/******************* end function pointers ************************************/

AXIS2_DECLARE(axis2_xml_schema_obj_t *)
axis2_xml_schema_obj_create(axis2_env_t **env)
{
    axis2_xml_schema_obj_impl_t *obj_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    obj_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_obj_impl_t));
    if(!obj_impl)
    {
        AXIS2_ERROR_SET((*env)->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    obj_impl->obj_type = AXIS2_XML_SCHEMA_OBJ;
    obj_impl->ht_super = NULL;
    obj_impl->line_num = -1;
    obj_impl->line_pos = -1;
    obj_impl->source_uri = NULL;
    obj_impl->obj.ops   = NULL;
    
    obj_impl->obj.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_obj_ops_t)); 
    if(!obj_impl->obj.ops)
    {
        axis2_xml_schema_obj_free(&(obj_impl->obj), env);
        AXIS2_ERROR_SET((*env)->error , 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    obj_impl->obj.ops->free = 
        axis2_xml_schema_obj_free;
    obj_impl->obj.ops->super_objs = 
        axis2_xml_schema_obj_super_objs;
    obj_impl->obj.ops->type = 
        axis2_xml_schema_obj_type;
    obj_impl->obj.ops->get_line_num = 
        axis2_xml_schema_obj_get_line_num;
    obj_impl->obj.ops->set_line_num = 
        axis2_xml_schema_obj_set_line_num;
    obj_impl->obj.ops->get_line_pos = 
        axis2_xml_schema_obj_get_line_pos;
    obj_impl->obj.ops->set_line_pos = 
        axis2_xml_schema_obj_set_line_pos;
    obj_impl->obj.ops->get_source_uri = 
        axis2_xml_schema_obj_get_source_uri;
    obj_impl->obj.ops->set_source_uri = 
        axis2_xml_schema_obj_set_source_uri;
    obj_impl->obj.ops->equals = 
        axis2_xml_schema_obj_equals;

    obj_impl->ht_super = axis2_hash_make(env);
    if(!obj_impl->ht_super)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(obj_impl->ht_super, "AXIS2_XML_SCHEMA_OBJ", AXIS2_HASH_KEY_STRING, 
            &(obj_impl->obj));
    
    return &(obj_impl->obj);
}


axis2_status_t AXIS2_CALL
axis2_xml_schema_obj_free(void *obj,
                axis2_env_t **env)
{
    axis2_xml_schema_obj_impl_t *obj_impl = NULL;

    obj_impl = AXIS2_INTF_TO_IMPL(obj);

    if(NULL != obj_impl->source_uri)
    {
        AXIS2_FREE((*env)->allocator, obj_impl->source_uri);
        obj_impl->source_uri = NULL;
    }

    if(obj_impl->ht_super)
    {
        axis2_hash_free(obj_impl->ht_super, env);
        obj_impl->ht_super = NULL;
    }

    if(NULL != obj_impl->obj.ops)
    {
        AXIS2_FREE((*env)->allocator, obj_impl->obj.ops);
        obj_impl->obj.ops = NULL;
    }

    if(NULL != obj_impl)
    {
        AXIS2_FREE((*env)->allocator, obj_impl);
        obj_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_xml_schema_obj_super_objs(void *obj,
                axis2_env_t **env)
{
    axis2_xml_schema_obj_impl_t *obj_impl = NULL;
    obj_impl = AXIS2_INTF_TO_IMPL(obj);
    return obj_impl->ht_super;
}

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_obj_type(
        void *obj,
        axis2_env_t **env)
{
    axis2_xml_schema_obj_impl_t *obj_impl = NULL;
    obj_impl = AXIS2_INTF_TO_IMPL(obj);
    return obj_impl->obj_type;
}

AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_obj_resolve_methods(
        axis2_xml_schema_obj_t *obj,
        axis2_env_t **env,
        axis2_xml_schema_obj_t *obj_impl,
        axis2_hash_t *methods)
{    
    axis2_xml_schema_obj_impl_t *obj_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, obj_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    obj_impl_l = (axis2_xml_schema_obj_impl_t *) obj_impl;
    
    obj->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_obj_ops_t));
    if(!obj->ops)
    {
        AXIS2_ERROR_SET((*env)->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }            
            
    obj->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
            
    obj->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
            
    obj->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);

    obj->ops->get_line_num = axis2_hash_get(methods,
            "get_line_num", AXIS2_HASH_KEY_STRING);
    if(!obj->ops->get_line_num)
            obj->ops->get_line_num = 
            obj_impl_l->obj.ops->get_line_num;
    
    obj->ops->set_line_num = axis2_hash_get(methods,
            "set_line_num", AXIS2_HASH_KEY_STRING);
    if(!obj->ops->set_line_num)
            obj->ops->set_line_num = 
            obj_impl_l->obj.ops->set_line_num; 
    
    obj->ops->get_line_pos = axis2_hash_get(methods,
            "get_line_pos", AXIS2_HASH_KEY_STRING);
    if(!obj->ops->get_line_pos)
            obj->ops->get_line_pos = 
            obj_impl_l->obj.ops->get_line_pos;
    
    obj->ops->set_line_pos = axis2_hash_get(methods,
            "set_line_pos", AXIS2_HASH_KEY_STRING);
    if(!obj->ops->set_line_pos)
            obj->ops->set_line_pos = 
            obj_impl_l->obj.ops->set_line_pos; 
    
    obj->ops->get_source_uri = axis2_hash_get(methods,
            "get_source_uri", AXIS2_HASH_KEY_STRING);
    if(!obj->ops->get_source_uri)
            obj->ops->get_source_uri = 
            obj_impl_l->obj.ops->get_source_uri;
    
    obj->ops->set_source_uri = axis2_hash_get(methods,
            "set_source_uri", AXIS2_HASH_KEY_STRING);
    if(!obj->ops->set_source_uri)
            obj->ops->set_source_uri = 
            obj_impl_l->obj.ops->set_source_uri; 

    obj->ops->equals = obj_impl_l->obj.ops->equals;
    
    return AXIS2_SUCCESS;    
}
int AXIS2_CALL
axis2_xml_schema_obj_get_line_num(void *obj,
                                    axis2_env_t **env)
{
    axis2_xml_schema_obj_impl_t *obj_impl = NULL;
    axis2_hash_t *super = NULL;

    super = AXIS2_XML_SCHEMA_OBJ_SUPER_OBJS(obj, env);
    if(NULL != super)
    {
        obj_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, "AXIS2_XML_SCHEMA_OBJ", 
                    AXIS2_HASH_KEY_STRING));
        if(NULL != obj_impl)
            return obj_impl->line_num;
    }
    return -1;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_obj_set_line_num(void *obj,
                        axis2_env_t **env,
                        int line_num)
{
    axis2_xml_schema_obj_impl_t *obj_impl = NULL;
    axis2_hash_t *super = NULL;

    super = AXIS2_XML_SCHEMA_OBJ_SUPER_OBJS(obj, env);
    if(NULL != super)
    {
        obj_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, "AXIS2_XML_SCHEMA_OBJ", 
                AXIS2_HASH_KEY_STRING));
    }
    else
    {
        
    
    }
    obj_impl->line_num = line_num;

    return AXIS2_SUCCESS;
}

int AXIS2_CALL
axis2_xml_schema_obj_get_line_pos(void *obj,
                                    axis2_env_t **env)
{
    axis2_xml_schema_obj_impl_t *obj_impl = NULL;
    axis2_hash_t *super = NULL;
    
    super = AXIS2_XML_SCHEMA_OBJ_SUPER_OBJS(obj, env);
    if(NULL != super)
    {
        obj_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, "AXIS2_XML_SCHEMA_OBJ", 
                    AXIS2_HASH_KEY_STRING));
    }
    return obj_impl->line_pos;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_obj_set_line_pos(void *obj,
                        axis2_env_t **env,
                        int line_pos)
{
    axis2_xml_schema_obj_impl_t *obj_impl = NULL;
    axis2_hash_t *super = NULL;

    super = AXIS2_XML_SCHEMA_OBJ_SUPER_OBJS(obj, env);
    if(NULL != super)
    {
        obj_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, "AXIS2_XML_SCHEMA_OBJ", 
                    AXIS2_HASH_KEY_STRING));
        if(NULL != obj_impl)
            obj_impl->line_pos = line_pos;
    }
    return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL
axis2_xml_schema_obj_get_source_uri(void *obj,
                                    axis2_env_t **env)
{
    axis2_xml_schema_obj_impl_t *obj_impl = NULL;
    axis2_hash_t *super = NULL;
    super = AXIS2_XML_SCHEMA_OBJ_SUPER_OBJS(obj, env);
    obj_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, "AXIS2_XML_SCHEMA_OBJ", 
                AXIS2_HASH_KEY_STRING));
    return obj_impl->source_uri;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_obj_set_source_uri(void *obj,
                        axis2_env_t **env,
                        axis2_char_t *source_uri)
{
    axis2_xml_schema_obj_impl_t *obj_impl = NULL;
    axis2_hash_t *super = NULL;

    super = AXIS2_XML_SCHEMA_OBJ_SUPER_OBJS(obj, env);
    obj_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, "AXIS2_XML_SCHEMA_OBJ", 
                AXIS2_HASH_KEY_STRING));

    if(NULL != obj_impl->source_uri)
    {
        AXIS2_FREE((*env)->allocator, obj_impl->source_uri);
        obj_impl->source_uri = NULL;
    }
    obj_impl->source_uri = AXIS2_STRDUP(source_uri, env);

    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_xml_schema_obj_equals(void *obj,
                                axis2_env_t **env,
                                void *obj_comp)
{
    axis2_xml_schema_obj_impl_t *obj_impl = NULL;
    axis2_hash_t *super = NULL;

    super = AXIS2_XML_SCHEMA_OBJ_SUPER_OBJS(obj, env);
    obj_impl = AXIS2_INTF_TO_IMPL(axis2_hash_get(super, "AXIS2_XML_SCHEMA_OBJ", 
                AXIS2_HASH_KEY_STRING));
    /****
        TODO
    
    ***/
    return AXIS2_TRUE;
}

