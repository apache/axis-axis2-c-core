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

#include <axis2_xml_schema_attribute_group.h>
#include <axis2_utils.h>

typedef struct axis2_xml_schema_attribute_group_impl 
                axis2_xml_schema_attribute_group_impl_t;

/** 
 * @brief axis2_xml_schema_attribute_group_impl
 */ 
struct axis2_xml_schema_attribute_group_impl
{
    axis2_xml_schema_attribute_group_t attr_grp;
    
    axis2_xml_schema_annotated_t *annotated;

    axis2_xml_schema_obj_collection_t *attributes;
    
    axis2_xml_schema_any_attribute_t *any_attribute;
        
    axis2_char_t *name;
    
    axis2_hash_t *methods; 
         
    axis2_xml_schema_types_t obj_type;
    
    axis2_hash_t* ht_super;
};

#define AXIS2_INTF_TO_IMPL(attr_grp) \
        ((axis2_xml_schema_attribute_group_impl_t *) attr_grp)

/******************* function prototypes **********************************/
axis2_status_t AXIS2_CALL 
axis2_xml_schema_attribute_group_free(void *attr_grp,
                        const axis2_env_t *env);

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_attribute_group_get_base_impl(
        void *attr_grp,
        const axis2_env_t *env);

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_attribute_group_get_type(
        void *attr_grp,
        const axis2_env_t *env);

axis2_hash_t* AXIS2_CALL
axis2_xml_schema_attribute_group_super_objs(void *attr_grp,
                                        const axis2_env_t *env);

axis2_xml_schema_any_attribute_t* AXIS2_CALL 
axis2_xml_schema_attribute_group_get_any_attribute(
            void *attr_grp,
            const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
axis2_xml_schema_attribute_group_set_any_attribute(
            void *attr_grp,
            const axis2_env_t *env,
            axis2_xml_schema_any_attribute_t *any_attr);  
    
axis2_xml_schema_obj_collection_t*  AXIS2_CALL 
axis2_xml_schema_attribute_group_get_attributes(void *attr_grp,
                                                const axis2_env_t *env);

axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_group_set_attributes(void *attr_grp,
                     const axis2_env_t *env,
                     axis2_xml_schema_obj_collection_t *attrs);

axis2_char_t*  AXIS2_CALL 
axis2_xml_schema_attribute_group_get_name(void *attr_grp,
              const axis2_env_t *env);
              
axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_group_set_name(void *attr_grp,
              const axis2_env_t *env,
              axis2_char_t *name);
              
 /************************ end function prototypes ******************************/
 
AXIS2_EXTERN axis2_xml_schema_attribute_group_t * AXIS2_CALL
axis2_xml_schema_attribute_group_create(const axis2_env_t *env)
{
    axis2_xml_schema_attribute_group_impl_t *attr_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    attr_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_attribute_group_impl_t));
    if(!attr_impl)
    {
        AXIS2_ERROR_SET(env->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    attr_impl->attr_grp.base.ops = NULL;
    attr_impl->attr_grp.ops = NULL;
    attr_impl->annotated = NULL;
    attr_impl->methods = NULL;
    attr_impl->ht_super = NULL;
    attr_impl->any_attribute = NULL;
    attr_impl->name = NULL;
    attr_impl->attributes = NULL;
    attr_impl->obj_type = AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP;
        
    attr_impl->attr_grp.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_attribute_group_ops_t));

    if(!attr_impl->attr_grp.ops)
    {
        axis2_xml_schema_attribute_group_free(&(attr_impl->attr_grp), env);
        AXIS2_ERROR_SET(env->error , 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    attr_impl->attributes = axis2_xml_schema_obj_collection_create(env);
    
    attr_impl->attr_grp.ops->free = 
        axis2_xml_schema_attribute_group_free;
    attr_impl->attr_grp.ops->get_type =
        axis2_xml_schema_attribute_group_get_type;
    attr_impl->attr_grp.ops->super_objs =
        axis2_xml_schema_attribute_group_super_objs;                
    attr_impl->attr_grp.ops->get_base_impl = 
        axis2_xml_schema_attribute_group_get_base_impl;

    attr_impl->attr_grp.ops->get_any_attribute =
        axis2_xml_schema_attribute_group_get_any_attribute;
    attr_impl->attr_grp.ops->get_attributes =
        axis2_xml_schema_attribute_group_get_attributes;        
    attr_impl->attr_grp.ops->set_attributes =
        axis2_xml_schema_attribute_group_set_attributes;
    attr_impl->attr_grp.ops->get_name =
        axis2_xml_schema_attribute_group_get_name;
    attr_impl->attr_grp.ops->set_name =
        axis2_xml_schema_attribute_group_set_name;
                                                                    
    attr_impl->methods = axis2_hash_make(env);
    attr_impl->ht_super = axis2_hash_make(env);
    if(!attr_impl->methods || !attr_impl->ht_super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(attr_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_attribute_group_free);
    axis2_hash_set(attr_impl->methods, "get_type", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_attribute_group_get_type);
    axis2_hash_set(attr_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_attribute_group_super_objs);            
    
    attr_impl->annotated = axis2_xml_schema_annotated_create(env);
    if(!attr_impl->methods)
    {
        axis2_xml_schema_attribute_group_free(&(attr_impl->attr_grp), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    axis2_hash_set(attr_impl->ht_super, "AXIS2_XML_SCHEMA_ATTRIBUTE", 
            AXIS2_HASH_KEY_STRING, &(attr_impl->attr_grp));
            
    axis2_hash_set(attr_impl->ht_super, "AXIS2_XML_SCHEMA_ANNOTATED", 
            AXIS2_HASH_KEY_STRING, attr_impl->annotated);

    axis2_hash_set(attr_impl->ht_super, "AXIS2_XML_SCHEMA_OBJ", 
            AXIS2_HASH_KEY_STRING, 
            AXIS2_XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(attr_impl->annotated, env));
                            
    status = axis2_xml_schema_annotated_resolve_methods(
            &(attr_impl->attr_grp.base), env, attr_impl->annotated, 
            attr_impl->methods); 
    return &(attr_impl->attr_grp);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_attribute_group_free(void *attr_grp,
                            const axis2_env_t *env)
{
    axis2_xml_schema_attribute_group_impl_t *attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr_grp);

    if(NULL != attr_impl->ht_super)
    {
        axis2_hash_free(attr_impl->ht_super, env);
        attr_impl->ht_super = NULL;    
    }
    if(NULL != attr_impl->any_attribute)
    {
        AXIS2_XML_SCHEMA_ANY_ATTRIBUTE_FREE(attr_impl->any_attribute, env);
        attr_impl->any_attribute = NULL;
    }
    if(NULL != attr_impl->attributes)
    {   
        /** TODO free individaul attributes */
        AXIS2_XML_SCHEMA_OBJ_COLLECTION_FREE(attr_impl->attributes, env);
        attr_impl->attributes = NULL;
    }
    if(NULL != attr_impl->name)
    {
        AXIS2_FREE(env->allocator, attr_impl->name);
        attr_impl->name = NULL;
    }
    
    if(NULL != attr_impl->methods)
    {
        axis2_hash_free(attr_impl->methods, env);
        attr_impl->methods = NULL;
    }
   
    if(NULL != attr_impl->annotated)
    {
        AXIS2_XML_SCHEMA_ANNOTATED_FREE(attr_impl->annotated, env);
        attr_impl->annotated = NULL;
    }
    
    if(NULL != attr_impl->attr_grp.ops)
    {
        AXIS2_FREE(env->allocator, attr_impl->attr_grp.ops);
        attr_impl->attr_grp.ops = NULL;
    }
    if(NULL != attr_impl->attr_grp.base.ops)
    {
        AXIS2_FREE(env->allocator, attr_impl->attr_grp.base.ops);
        attr_impl->attr_grp.base.ops = NULL;    
    }

    AXIS2_FREE(env->allocator, attr_impl);
    attr_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_attribute_group_get_base_impl(void *attr_grp,
                                const axis2_env_t *env)
{
    axis2_xml_schema_attribute_group_impl_t *attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    attr_impl = AXIS2_INTF_TO_IMPL(attr_grp);
    return attr_impl->annotated;
}

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_attribute_group_get_type(void *attr_grp,
                                    const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr_grp)->obj_type;    
}                                    

axis2_hash_t* AXIS2_CALL
axis2_xml_schema_attribute_group_super_objs(void *attr_grp,
                                        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr_grp)->ht_super;
}

axis2_xml_schema_any_attribute_t* AXIS2_CALL 
axis2_xml_schema_attribute_group_get_any_attribute(
            void *attr_grp,
            const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr_grp)->any_attribute;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_attribute_group_set_any_attribute(
            void *attr_grp,
            const axis2_env_t *env,
            axis2_xml_schema_any_attribute_t *any_attr)
{
    axis2_xml_schema_attribute_group_impl_t *attr_grp_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, any_attr, AXIS2_FAILURE);
    attr_grp_impl = AXIS2_INTF_TO_IMPL(attr_grp);
    if(NULL != attr_grp_impl->any_attribute)
    {
        /** TODO */
    }
    attr_grp_impl->any_attribute = any_attr;
    return AXIS2_SUCCESS;
}  
    
axis2_xml_schema_obj_collection_t*  AXIS2_CALL 
axis2_xml_schema_attribute_group_get_attributes(void *attr_grp,
                                                const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr_grp)->attributes;
}

axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_group_set_attributes(void *attr_grp,
                     const axis2_env_t *env,
                     axis2_xml_schema_obj_collection_t *attrs)
{
    axis2_xml_schema_attribute_group_impl_t *attr_grp_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, attrs, AXIS2_FAILURE);
    attr_grp_impl = AXIS2_INTF_TO_IMPL(attr_grp);
    if(NULL != attr_grp_impl->attributes)
    {
        /** TODO */
    }
    attr_grp_impl->attributes = attrs;
    return AXIS2_SUCCESS;
}

axis2_char_t*  AXIS2_CALL 
axis2_xml_schema_attribute_group_get_name(void *attr_grp,
              const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr_grp)->name;
}
              
axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_group_set_name(void *attr_grp,
              const axis2_env_t *env,
              axis2_char_t *name)
{
    axis2_xml_schema_attribute_group_impl_t *attr_grp_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, name, AXIS2_FAILURE);
    attr_grp_impl = AXIS2_INTF_TO_IMPL(attr_grp);
    if(NULL != attr_grp_impl->name)
    {
        AXIS2_FREE(env->allocator, attr_grp_impl->name);
        attr_grp_impl->name = NULL;
    }
    attr_grp_impl->name = AXIS2_STRDUP(name, env);
    return AXIS2_SUCCESS;

}