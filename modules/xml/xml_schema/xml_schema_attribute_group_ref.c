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

#include <xml_schema/axis2_xml_schema_attribute_group_ref.h>

typedef struct axis2_xml_schema_attribute_group_ref_impl 
                axis2_xml_schema_attribute_group_ref_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_attribute_group_ref_impl
{
    axis2_xml_schema_attribute_group_ref_t grp_ref;
    
    axis2_xml_schema_annotated_t *annotated;

    axis2_qname_t *ref_qname;
    
    axis2_hash_t *methods; 
         
    axis2_xml_schema_types_t obj_type;
    
    axis2_hash_t* ht_super;
};

#define AXIS2_INTF_TO_IMPL(grp_ref) \
        ((axis2_xml_schema_attribute_group_ref_impl_t *) grp_ref)

/******************* function prototypes **********************************/
axis2_status_t AXIS2_CALL 
axis2_xml_schema_attribute_group_ref_free(void *grp_ref,
                        axis2_env_t **env);

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_attribute_group_ref_get_base_impl(void *grp_ref,
                                    axis2_env_t **env);

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_attribute_group_ref_type(void *grp_ref,
                                    axis2_env_t **env);

axis2_hash_t* AXIS2_CALL
axis2_xml_schema_attribute_group_ref_super_objs(void *grp_ref,
                                        axis2_env_t **env);

axis2_qname_t* AXIS2_CALL 
axis2_xml_schema_attribute_group_ref_get_ref_qname(
            void *grp_ref,
            axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_xml_schema_attribute_group_ref_set_ref_qname(
            void *grp_ref,
            axis2_env_t **env,
            axis2_qname_t *any_attr);  
    
 /************************ end function prototypes ******************************/
 
AXIS2_DECLARE(axis2_xml_schema_attribute_group_ref_t *)
axis2_xml_schema_attribute_group_ref_create(axis2_env_t **env)
{
    axis2_xml_schema_attribute_group_ref_impl_t *attr_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    attr_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_attribute_group_ref_impl_t));
    if(!attr_impl)
    {
        AXIS2_ERROR_SET((*env)->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    attr_impl->grp_ref.base.ops = NULL;
    attr_impl->grp_ref.ops = NULL;
    attr_impl->annotated = NULL;
    attr_impl->methods = NULL;
    attr_impl->ht_super = NULL;
    attr_impl->ref_qname = NULL;
    attr_impl->obj_type = AXIS2_XML_SCHEMA_ATTRIBUTE_GROUP_REF;
        
    attr_impl->grp_ref.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_attribute_group_ref_ops_t));

    if(!attr_impl->grp_ref.ops)
    {
        axis2_xml_schema_attribute_group_ref_free(&(attr_impl->grp_ref), env);
        AXIS2_ERROR_SET((*env)->error , 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    attr_impl->grp_ref.ops->free = 
        axis2_xml_schema_attribute_group_ref_free;
    attr_impl->grp_ref.ops->type =
        axis2_xml_schema_attribute_group_ref_type;
    attr_impl->grp_ref.ops->super_objs =
        axis2_xml_schema_attribute_group_ref_super_objs;                
    attr_impl->grp_ref.ops->get_base_impl = 
        axis2_xml_schema_attribute_group_ref_get_base_impl;

    attr_impl->grp_ref.ops->get_ref_qname =
        axis2_xml_schema_attribute_group_ref_get_ref_qname;
    attr_impl->grp_ref.ops->set_ref_qname =
        axis2_xml_schema_attribute_group_ref_set_ref_qname;        
                                                                    
    attr_impl->methods = axis2_hash_make(env);
    attr_impl->ht_super = axis2_hash_make(env);
    if(!attr_impl->methods || !attr_impl->ht_super)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(attr_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_attribute_group_ref_free);
    axis2_hash_set(attr_impl->methods, "type", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_attribute_group_ref_type);
    axis2_hash_set(attr_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_attribute_group_ref_super_objs);            
    
    attr_impl->annotated = axis2_xml_schema_annotated_create(env);
    if(!attr_impl->methods)
    {
        axis2_xml_schema_attribute_group_ref_free(&(attr_impl->grp_ref), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    axis2_hash_set(attr_impl->ht_super, "AXIS2_XML_SCHEMA_ATTRIBUTE", 
            AXIS2_HASH_KEY_STRING, &(attr_impl->grp_ref));
            
    axis2_hash_set(attr_impl->ht_super, "AXIS2_XML_SCHEMA_ANNOTATED", 
            AXIS2_HASH_KEY_STRING, attr_impl->annotated);

    axis2_hash_set(attr_impl->ht_super, "AXIS2_XML_SCHEMA_OBJ", 
            AXIS2_HASH_KEY_STRING, 
            AXIS2_XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(attr_impl->annotated, env));
                            
    status = axis2_xml_schema_annotated_resolve_methods(
            &(attr_impl->grp_ref.base), env, attr_impl->annotated, 
            attr_impl->methods); 
    return &(attr_impl->grp_ref);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_attribute_group_ref_free(void *grp_ref,
                            axis2_env_t **env)
{
    axis2_xml_schema_attribute_group_ref_impl_t *attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(grp_ref);

    if(NULL != attr_impl->ht_super)
    {
        axis2_hash_free(attr_impl->ht_super, env);
        attr_impl->ht_super = NULL;    
    }
    if(NULL != attr_impl->ref_qname)
    {
        AXIS2_QNAME_FREE(attr_impl->ref_qname, env);
        attr_impl->ref_qname = NULL;
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
    
    if(NULL != attr_impl->grp_ref.ops)
    {
        AXIS2_FREE((*env)->allocator, attr_impl->grp_ref.ops);
        attr_impl->grp_ref.ops = NULL;
    }
    if(NULL != attr_impl->grp_ref.base.ops)
    {
        AXIS2_FREE((*env)->allocator, attr_impl->grp_ref.base.ops);
        attr_impl->grp_ref.base.ops = NULL;    
    }

    AXIS2_FREE((*env)->allocator, attr_impl);
    attr_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_attribute_group_ref_get_base_impl(void *grp_ref,
                                axis2_env_t **env)
{
    axis2_xml_schema_attribute_group_ref_impl_t *attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    attr_impl = AXIS2_INTF_TO_IMPL(grp_ref);
    return attr_impl->annotated;
}

/*
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_attribute_group_ref_resolve_methods(
                                axis2_xml_schema_attribute_group_ref_t *grp_ref,
                                axis2_env_t **env,
                                axis2_xml_schema_attribute_group_ref_t *attr_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_attribute_group_ref_impl_t *any_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, attr_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    any_impl_l = (axis2_xml_schema_attribute_group_ref_impl_t *) attr_impl;
    
    grp_ref->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_attribute_group_ref_ops_t));
    if(NULL != grp_ref->ops)
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }            
            
    grp_ref->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    grp_ref->ops->get_base_impl = 
            any_impl_l->grp_ref.ops->get_base_impl;
    grp_ref->ops->get_namespace = 
            any_impl_l->grp_ref.ops->get_namespace;
    grp_ref->ops->set_namespace = 
            any_impl_l->grp_ref.ops->set_namespace;
    grp_ref->ops->get_process_content = 
            any_impl_l->grp_ref.ops->get_process_content;
    grp_ref->ops->set_process_content = 
            any_impl_l->grp_ref.ops->set_process_content;
    
    return axis2_xml_schema_annotated_resolve_methods(&(grp_ref->base), 
            env, any_impl_l->annotated, methods);
}
*/
axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_attribute_group_ref_type(void *grp_ref,
                                    axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(grp_ref)->obj_type;    
}                                    

axis2_hash_t* AXIS2_CALL
axis2_xml_schema_attribute_group_ref_super_objs(void *grp_ref,
                                        axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(grp_ref)->ht_super;
}

axis2_qname_t* AXIS2_CALL 
axis2_xml_schema_attribute_group_ref_get_ref_qname(
            void *grp_ref,
            axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(grp_ref)->ref_qname;
}

axis2_status_t AXIS2_CALL 
axis2_xml_schema_attribute_group_ref_set_ref_qname(
            void *grp_ref,
            axis2_env_t **env,
            axis2_qname_t *ref_qname)
{
    axis2_xml_schema_attribute_group_ref_impl_t *attr_grp_impl = NULL;
    AXIS2_PARAM_CHECK((*env)->error, ref_qname, AXIS2_FAILURE);
    attr_grp_impl = AXIS2_INTF_TO_IMPL(grp_ref);
    if(NULL != attr_grp_impl->ref_qname)
    {
        /** TODO */
    }
    attr_grp_impl->ref_qname = ref_qname;
    return AXIS2_SUCCESS;
}  
