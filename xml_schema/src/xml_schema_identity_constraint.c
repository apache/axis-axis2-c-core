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

#include <xml_schema/axis2_xml_schema_identity_constraint.h>
#include <xml_schema/axis2_xml_schema_obj_collection.h>
#include <xml_schema/axis2_xml_schema_xpath.h>

typedef struct axis2_xml_schema_identity_constraint_impl 
                axis2_xml_schema_identity_constraint_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *	Axis2 Other Extension  
 */ 
struct axis2_xml_schema_identity_constraint_impl
{
    axis2_xml_schema_identity_constraint_t id_constr;
    axis2_xml_schema_annotated_t *annotated;
    axis2_xml_schema_obj_collection_t *fields;
    axis2_char_t *name;
    axis2_xml_schema_xpath_t *selector;
    
    /** keyref field */
    axis2_qname_t *refer;
    
    axis2_hash_t *methods;
    axis2_hash_t *ht_super;
    axis2_xml_schema_types_t obj_type;
};

#define AXIS2_INTF_TO_IMPL(id_constr) \
        ((axis2_xml_schema_identity_constraint_impl_t *) id_constr)

axis2_status_t AXIS2_CALL 
axis2_xml_schema_identity_constraint_free(void *id_constr,
                        axis2_env_t **env);

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_identity_constraint_get_base_impl(void *id_constr,
                                        axis2_env_t **env);
                                        

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_identity_constraint_type(void *id_constr,
                                        axis2_env_t **env);
                                        

axis2_hash_t* AXIS2_CALL
axis2_xml_schema_identity_constraint_super_objs(void *id_constr,
                                        axis2_env_t **env);                                                                                
axis2_qname_t* AXIS2_CALL
axis2_xml_schema_identity_constraint_get_refer(void *id_constr,
                                             axis2_env_t **env); 
                                             
axis2_status_t AXIS2_CALL
axis2_xml_schema_identity_constraint_set_refer(void *id_constr,
                                             axis2_env_t **env,
                                              axis2_qname_t *refer);                                                                                  
                                        
axis2_xml_schema_obj_collection_t *AXIS2_CALL
axis2_xml_schema_identity_constraint_get_fields(void *id_constr,
                                axis2_env_t **env);

axis2_char_t *AXIS2_CALL
axis2_xml_schema_identity_constraint_get_name(void *id_constr,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_identity_constraint_set_name(void *id_constr,
                                axis2_env_t **env,
                                axis2_char_t *name);

axis2_xml_schema_xpath_t *AXIS2_CALL
axis2_xml_schema_identity_constraint_get_selector(void *id_constr,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_xml_schema_identity_constraint_set_selector(void *id_constr,
                                    axis2_env_t **env,
                                    axis2_xml_schema_xpath_t *selector);

AXIS2_DECLARE(axis2_xml_schema_identity_constraint_t *)
axis2_xml_schema_identity_constraint_create(axis2_env_t **env)
{
    axis2_xml_schema_identity_constraint_impl_t *id_cns_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    id_cns_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_identity_constraint_impl_t));
    if(!id_cns_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }                    

    id_cns_impl->annotated = NULL;
    id_cns_impl->methods = NULL;
    id_cns_impl->fields = NULL;
    id_cns_impl->name = NULL;
    id_cns_impl->selector = NULL;
    id_cns_impl->id_constr.base.ops = NULL;
    id_cns_impl->id_constr.ops = NULL;
    id_cns_impl->refer = NULL;
    id_cns_impl->ht_super = NULL;
    id_cns_impl->obj_type = AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT;
    
    id_cns_impl->id_constr.ops = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_xml_schema_identity_constraint_ops_t));
    if(!(id_cns_impl->id_constr.ops))
    {
        axis2_xml_schema_identity_constraint_free(
            &(id_cns_impl->id_constr), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    id_cns_impl->id_constr.ops->free = 
            axis2_xml_schema_identity_constraint_free;
    id_cns_impl->id_constr.ops->get_base_impl = 
            axis2_xml_schema_identity_constraint_get_base_impl;
    id_cns_impl->id_constr.ops->get_refer = 
            axis2_xml_schema_identity_constraint_get_refer;
    id_cns_impl->id_constr.ops->set_refer = 
            axis2_xml_schema_identity_constraint_set_refer;
    id_cns_impl->id_constr.ops->type = 
            axis2_xml_schema_identity_constraint_type;
    id_cns_impl->id_constr.ops->super_objs = 
            axis2_xml_schema_identity_constraint_super_objs;
    id_cns_impl->id_constr.ops->get_fields = 
            axis2_xml_schema_identity_constraint_get_fields;
    id_cns_impl->id_constr.ops->get_name = 
            axis2_xml_schema_identity_constraint_get_name;
    id_cns_impl->id_constr.ops->set_name = 
            axis2_xml_schema_identity_constraint_set_name;
    id_cns_impl->id_constr.ops->get_selector = 
            axis2_xml_schema_identity_constraint_get_selector;
    id_cns_impl->id_constr.ops->set_selector = 
            axis2_xml_schema_identity_constraint_set_selector;
   
    id_cns_impl->methods = axis2_hash_make(env);
    id_cns_impl->ht_super = axis2_hash_make(env);
    
    if(!id_cns_impl->methods || !id_cns_impl->ht_super)
    {
        axis2_xml_schema_identity_constraint_free(&(id_cns_impl->id_constr), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(id_cns_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_identity_constraint_free);
    axis2_hash_set(id_cns_impl->methods, "get_fields", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_identity_constraint_get_fields);
    axis2_hash_set(id_cns_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_identity_constraint_type);
    axis2_hash_set(id_cns_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_identity_constraint_super_objs);
    axis2_hash_set(id_cns_impl->methods, "get_refer", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_identity_constraint_get_refer);
    axis2_hash_set(id_cns_impl->methods, "set_refer", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_identity_constraint_set_refer);
    axis2_hash_set(id_cns_impl->methods, "get_name", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_identity_constraint_get_name);
    axis2_hash_set(id_cns_impl->methods, "set_name", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_identity_constraint_set_name);
    axis2_hash_set(id_cns_impl->methods, "get_selector", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_identity_constraint_get_selector);
    axis2_hash_set(id_cns_impl->methods, "set_selector", 
            AXIS2_HASH_KEY_STRING, axis2_xml_schema_identity_constraint_set_selector);
    
    id_cns_impl->annotated = axis2_xml_schema_annotated_create(env);
    if(!id_cns_impl->annotated)
    {
        axis2_xml_schema_identity_constraint_free(&(id_cns_impl->id_constr), env);
        return NULL;
    }
    
    axis2_hash_set(id_cns_impl->ht_super, "AXIS2_XML_SCHEMA_IDENTITY_CONSTRAINT", 
            AXIS2_HASH_KEY_STRING, &(id_cns_impl->id_constr));
    axis2_hash_set(id_cns_impl->ht_super, "AXIS2_XML_SCHEMA_ANNOTATED", 
            AXIS2_HASH_KEY_STRING, id_cns_impl->annotated);
    axis2_hash_set(id_cns_impl->ht_super, "AXIS2_XML_SCHEMA_OBJ", 
            AXIS2_HASH_KEY_STRING, 
            AXIS2_XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(id_cns_impl->annotated, env));                 
    status = axis2_xml_schema_annotated_resolve_methods(
            &(id_cns_impl->id_constr.base), 
            env, id_cns_impl->annotated, 
            id_cns_impl->methods);
    return &(id_cns_impl->id_constr);
}

AXIS2_DECLARE(axis2_xml_schema_identity_constraint_t *)
axis2_xml_schema_unique_create(axis2_env_t **env)
{
    axis2_xml_schema_identity_constraint_t *id_cns = NULL;
    id_cns = axis2_xml_schema_identity_constraint_create(env);
    AXIS2_INTF_TO_IMPL(id_cns)->obj_type = AXIS2_XML_SCHEMA_UNIQUE;
    return id_cns;        
}

AXIS2_DECLARE(axis2_xml_schema_identity_constraint_t *)
axis2_xml_schema_keyref_create(axis2_env_t **env)
{
    axis2_xml_schema_identity_constraint_t *id_cns = NULL;
    id_cns = axis2_xml_schema_identity_constraint_create(env);
    AXIS2_INTF_TO_IMPL(id_cns)->obj_type = AXIS2_XML_SCHEMA_KEYREF;
    return id_cns;        
}

AXIS2_DECLARE(axis2_xml_schema_identity_constraint_t *)
axis2_xml_schema_key_create(axis2_env_t **env)
{
    axis2_xml_schema_identity_constraint_t *id_cns = NULL;
    id_cns = axis2_xml_schema_identity_constraint_create(env);
    AXIS2_INTF_TO_IMPL(id_cns)->obj_type = AXIS2_XML_SCHEMA_KEY;
    return id_cns;        
}

AXIS2_DECLARE(axis2_xml_schema_identity_constraint_t *)
axis2_xml_schema_keyref_create(axis2_env_t **env);

AXIS2_DECLARE(axis2_xml_schema_identity_constraint_t *)
axis2_xml_schema_key_create(axis2_env_t **env);





axis2_status_t AXIS2_CALL
axis2_xml_schema_identity_constraint_free(void *id_constr,
                                    axis2_env_t **env)
{
    axis2_xml_schema_identity_constraint_impl_t *id_cns_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    id_cns_impl = AXIS2_INTF_TO_IMPL(id_constr);

    if(NULL != id_cns_impl->name)
    {
        AXIS2_FREE((*env)->allocator, id_cns_impl->name);
        id_cns_impl->name = NULL;
    }

    if(NULL != id_cns_impl->selector)
    {
        AXIS2_XML_SCHEMA_XPATH_FREE(id_cns_impl->selector, env);
        id_cns_impl->selector = NULL;
    }
    
    if(NULL != id_cns_impl->methods)
    {
        axis2_hash_free(id_cns_impl->methods, env);
        id_cns_impl->methods = NULL;
    }
    if(NULL != id_cns_impl->ht_super)
    {
        axis2_hash_free(id_cns_impl->ht_super, env);
        id_cns_impl->ht_super = NULL;
    }
    if(NULL != id_cns_impl->refer)
    {
        AXIS2_QNAME_FREE(id_cns_impl->refer, env);
        id_cns_impl->refer = NULL;
    }
    if(NULL != id_cns_impl->annotated)
    {
        AXIS2_XML_SCHEMA_ANNOTATED_FREE(id_cns_impl->annotated, env);
        id_cns_impl->annotated = NULL;
    }
    
    if(NULL != id_cns_impl->id_constr.ops)
    {
        AXIS2_FREE((*env)->allocator, id_cns_impl->id_constr.ops);
        id_cns_impl->id_constr.ops = NULL;
    }
    if(NULL != id_cns_impl->id_constr.base.ops)
    {
        AXIS2_FREE((*env)->allocator, id_cns_impl->id_constr.base.ops);
        id_cns_impl->id_constr.base.ops = NULL;
    }

    if(NULL != id_cns_impl)
    {
        AXIS2_FREE((*env)->allocator, id_cns_impl);
        id_cns_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_identity_constraint_get_base_impl(void *id_constr,
                                axis2_env_t **env)
{
    axis2_xml_schema_identity_constraint_impl_t *id_cns_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    id_cns_impl = AXIS2_INTF_TO_IMPL(id_constr);

    return id_cns_impl->annotated;
}

/*
AXIS2_DECLARE(axis2_status_t)
axis2_xml_schema_identity_constraint_resolve_methods(
                axis2_xml_schema_identity_constraint_t *id_constr,
                axis2_env_t **env,
                axis2_xml_schema_identity_constraint_t *id_cns_impl,
                axis2_hash_t *methods)
{
    axis2_xml_schema_identity_constraint_impl_t *identity_constraint_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, id_cns_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    
    identity_constraint_impl_l = (axis2_xml_schema_identity_constraint_impl_t *) id_cns_impl;
    
    id_constr->ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_xml_schema_identity_constraint_ops_t));
    if(NULL == id_constr->ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }            
    id_constr->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    id_constr->ops->get_base_impl = 
            identity_constraint_impl_l->id_constr.ops->get_base_impl;
    id_constr->ops->get_fields = 
            identity_constraint_impl_l->id_constr.ops->get_fields;
    id_constr->ops->get_name = 
            identity_constraint_impl_l->id_constr.ops->get_name;
    id_constr->ops->set_name = 
            identity_constraint_impl_l->id_constr.ops->set_name;
    id_constr->ops->get_selector = 
            identity_constraint_impl_l->id_constr.ops->get_selector;
    id_constr->ops->set_selector = 
            identity_constraint_impl_l->id_constr.ops->set_selector;
    
    return axis2_xml_schema_annotated_resolve_methods(&(id_constr->base), 
            env, identity_constraint_impl_l->annotated, methods);
}
*/
axis2_xml_schema_obj_collection_t *AXIS2_CALL
axis2_xml_schema_identity_constraint_get_fields(void *id_constr,
                                                axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(id_constr)->fields;
}
axis2_char_t *AXIS2_CALL
axis2_xml_schema_identity_constraint_get_name(void *id_constr,
                                axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(id_constr)->name;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_identity_constraint_set_name(void *id_constr,
                                axis2_env_t **env,
                                axis2_char_t *name)
{
    axis2_xml_schema_identity_constraint_impl_t *id_cns_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    id_cns_impl = AXIS2_INTF_TO_IMPL(id_constr);

    if(id_cns_impl->name)
    {
        AXIS2_FREE((*env)->allocator, id_cns_impl->name);
        id_cns_impl->name = NULL;
    }
    id_cns_impl->name = AXIS2_STRDUP(name, env);
    if(!id_cns_impl->name)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_xml_schema_xpath_t *AXIS2_CALL
axis2_xml_schema_identity_constraint_get_selector(void *id_constr,
                                                    axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(id_constr)->selector;
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_identity_constraint_set_selector(void *id_constr,
                                    axis2_env_t **env,
                                    axis2_xml_schema_xpath_t *selector)
{
    axis2_xml_schema_identity_constraint_impl_t *id_cns_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, selector, AXIS2_FAILURE);
    id_cns_impl = AXIS2_INTF_TO_IMPL(id_constr);
    
    id_cns_impl->selector = selector;
    return AXIS2_SUCCESS;
}


axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_identity_constraint_type(void *id_constr,
                                        axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(id_constr)->obj_type;
}                                        
                                        

axis2_hash_t* AXIS2_CALL
axis2_xml_schema_identity_constraint_super_objs(void *id_constr,
                                        axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(id_constr)->ht_super;
}                                                                                   
axis2_qname_t* AXIS2_CALL
axis2_xml_schema_identity_constraint_get_refer(void *id_constr,
                                             axis2_env_t **env)
{
    return AXIS2_INTF_TO_IMPL(id_constr)->refer;
}                                             
                                             
axis2_status_t AXIS2_CALL
axis2_xml_schema_identity_constraint_set_refer(void *id_constr,
                                             axis2_env_t **env,
                                              axis2_qname_t *refer)
{
    axis2_xml_schema_identity_constraint_impl_t *id_cns_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, refer, AXIS2_FAILURE);
    id_cns_impl = AXIS2_INTF_TO_IMPL(id_constr);
    if(NULL != id_cns_impl->refer)
    {   
        /*
        AXIS2_QNAME_FREE(id_cns_impl->reref, env);
        id_cns_impl->reref = NULL;
        */
    }
    id_cns_impl->refer = refer;
    return AXIS2_SUCCESS;
}                                              
