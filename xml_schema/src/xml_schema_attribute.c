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

#include <xml_schema/axis2_xml_schema_attribute.h>
#include <axis2_utils.h>

typedef struct axis2_xml_schema_attribute_impl 
                axis2_xml_schema_attribute_impl_t;

/** 
 * @brief Other Extension Struct Impl
 *   Axis2 Other Extension  
 */ 
struct axis2_xml_schema_attribute_impl
{
    axis2_xml_schema_attribute_t attr;
    
    axis2_xml_schema_annotated_t *annotated;

    axis2_xml_schema_form_t *form;
    
    axis2_xml_schema_use_t *use;
    
    void *attribute_type;
    
    axis2_char_t *default_value;
    
    axis2_char_t *fixed_value;
    
    axis2_char_t *name;
    
    axis2_xml_schema_simple_type_t *schema_type;
    
    axis2_qname_t *schema_type_name;    
    
    axis2_qname_t *qualified_name;
    
    axis2_qname_t *ref_name;
    
    axis2_hash_t *methods; 
         
    axis2_xml_schema_types_t obj_type;
    
    axis2_hash_t* ht_super;
};

#define AXIS2_INTF_TO_IMPL(attr) \
        ((axis2_xml_schema_attribute_impl_t *) attr)

/******************* function prototypes **********************************/
axis2_status_t AXIS2_CALL 
axis2_xml_schema_attribute_free(void *attr,
                        const axis2_env_t *env);

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_attribute_get_base_impl(void *attr,
                                    const axis2_env_t *env);

axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_attribute_type(void *attr,
                                    const axis2_env_t *env);

axis2_hash_t* AXIS2_CALL
axis2_xml_schema_attribute_super_objs(void *attr,
                                        const axis2_env_t *env);

void * AXIS2_CALL 
axis2_xml_schema_attribute_get_attr_type(
            void *attr,
            const axis2_env_t *env);

axis2_char_t * AXIS2_CALL 
axis2_xml_schema_attribute_get_default_value(
            void *attr,
            const axis2_env_t *env);                        

axis2_status_t AXIS2_CALL 
axis2_xml_schema_attribute_set_default_value(
            void *attr,
            const axis2_env_t *env,
            axis2_char_t *default_value);  
    
axis2_char_t*  AXIS2_CALL 
axis2_xml_schema_attribute_get_fixed_value(void *attr,
                        const axis2_env_t *env);

axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_set_fixed_value(void *attr,
                     const axis2_env_t *env,
                     axis2_char_t *fixed_value);

axis2_xml_schema_form_t* AXIS2_CALL 
axis2_xml_schema_attribute_get_schema_form (void *attr,
                      const axis2_env_t *env);

axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_set_schema_form(void *attr,
                     const axis2_env_t *env,
                     axis2_xml_schema_form_t *schema_form);
                     
axis2_qname_t*  AXIS2_CALL 
axis2_xml_schema_attribute_get_qname(void *attr,
               const axis2_env_t *env);
               
axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_set_qname(void *attr,
               const axis2_env_t *env,
               axis2_qname_t *qualified_name);
               
axis2_char_t*  AXIS2_CALL 
axis2_xml_schema_attribute_get_name(void *attr,
              const axis2_env_t *env);
              
axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_set_name(void *attr,
              const axis2_env_t *env,
              axis2_char_t *name);
              
axis2_qname_t*  AXIS2_CALL 
axis2_xml_schema_attribute_get_ref_name(void *attr,
                  const axis2_env_t *env);
                                              
axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_set_ref_name(void *attr,
                  const axis2_env_t *env,
                  axis2_qname_t *ref_name);
                  
axis2_xml_schema_simple_type_t*  AXIS2_CALL 
axis2_xml_schema_attribute_get_schema_type(void *attr,
                     const axis2_env_t *env);
                     
axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_set_schema_type(void *attr,
                     const axis2_env_t *env,
                     axis2_xml_schema_simple_type_t* sch_type);                     
                     
axis2_qname_t*  AXIS2_CALL 
axis2_xml_schema_attribute_get_schema_type_name(void *attr,
                          const axis2_env_t *env);
                          
                              
axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_set_schema_type_name(void *attr,
                          const axis2_env_t *env,
                          axis2_qname_t *sch_type_name);
                          
axis2_xml_schema_use_t*  AXIS2_CALL 
axis2_xml_schema_attribute_get_use(void *attr,
             const axis2_env_t *env);
             
axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_set_use(void *attr,
             const axis2_env_t *env,
             axis2_xml_schema_use_t *use);
             
axis2_char_t*  AXIS2_CALL
axis2_xml_schema_attribute_to_string(void *attr,
               const axis2_env_t *env,
               axis2_char_t *prefix,
               int tab);                                                  
 /************************ end function prototypes ******************************/
 
AXIS2_EXTERN axis2_xml_schema_attribute_t * AXIS2_CALL
axis2_xml_schema_attribute_create(const axis2_env_t *env)
{
    axis2_xml_schema_attribute_impl_t *attr_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    attr_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_attribute_impl_t));
    if(!attr_impl)
    {
        AXIS2_ERROR_SET(env->error , AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    attr_impl->attr.base.ops = NULL;
    attr_impl->attr.ops = NULL;
    attr_impl->annotated = NULL;
    attr_impl->methods = NULL;
    attr_impl->ht_super = NULL;
    attr_impl->attribute_type = NULL;
    attr_impl->default_value = NULL;
    attr_impl->fixed_value = NULL;
    attr_impl->form = NULL;
    attr_impl->use = NULL;
    attr_impl->obj_type = AXIS2_XML_SCHEMA_ATTRIBUTE;
    attr_impl->schema_type_name = NULL;
    attr_impl->schema_type = NULL;
    attr_impl->qualified_name = NULL;
    attr_impl->name = NULL;
    attr_impl->ref_name = NULL;
        
    attr_impl->attr.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_xml_schema_attribute_ops_t));

    if(!attr_impl->attr.ops)
    {
        axis2_xml_schema_attribute_free(&(attr_impl->attr), env);
        AXIS2_ERROR_SET(env->error , 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    attr_impl->use = axis2_xml_schema_use_create(env, "none");
    attr_impl->form = axis2_xml_schema_form_create(env, "none");
    
    if(!attr_impl->use || !attr_impl->form)
    {
        axis2_xml_schema_attribute_free(&(attr_impl->attr), env);
        return NULL;
    }            
    attr_impl->attr.ops->free = 
        axis2_xml_schema_attribute_free;
    attr_impl->attr.ops->type =
        axis2_xml_schema_attribute_type;
    attr_impl->attr.ops->super_objs =
        axis2_xml_schema_attribute_super_objs;                
    attr_impl->attr.ops->get_base_impl = 
        axis2_xml_schema_attribute_get_base_impl;
    attr_impl->attr.ops->get_attr_type =
        axis2_xml_schema_attribute_get_attr_type;
    attr_impl->attr.ops->get_default_value =
        axis2_xml_schema_attribute_get_default_value;        
    attr_impl->attr.ops->set_default_value =
        axis2_xml_schema_attribute_set_default_value;
    attr_impl->attr.ops->get_fixed_value =
        axis2_xml_schema_attribute_get_fixed_value;
    attr_impl->attr.ops->set_fixed_value =
        axis2_xml_schema_attribute_set_fixed_value;
    attr_impl->attr.ops->get_schema_form =
        axis2_xml_schema_attribute_get_schema_form;
    attr_impl->attr.ops->set_schema_form =
        axis2_xml_schema_attribute_set_schema_form;
    attr_impl->attr.ops->get_qname =
        axis2_xml_schema_attribute_get_qname;
    attr_impl->attr.ops->set_qname =
        axis2_xml_schema_attribute_set_qname;
    attr_impl->attr.ops->get_name =
        axis2_xml_schema_attribute_get_name;
    attr_impl->attr.ops->set_name =
        axis2_xml_schema_attribute_set_name;        
    attr_impl->attr.ops->set_ref_name =
        axis2_xml_schema_attribute_set_ref_name;
    attr_impl->attr.ops->get_schema_type =
        axis2_xml_schema_attribute_get_schema_type;
    attr_impl->attr.ops->set_schema_type =
        axis2_xml_schema_attribute_set_schema_type;
    attr_impl->attr.ops->get_schema_type_name =
        axis2_xml_schema_attribute_get_schema_type_name;
    attr_impl->attr.ops->set_schema_type_name =
        axis2_xml_schema_attribute_set_schema_type_name;
    attr_impl->attr.ops->get_use =
        axis2_xml_schema_attribute_get_use;
    attr_impl->attr.ops->set_use =
        axis2_xml_schema_attribute_set_use;
    attr_impl->attr.ops->to_string =
        axis2_xml_schema_attribute_to_string;
                                                                    
    attr_impl->methods = axis2_hash_make(env);
    attr_impl->ht_super = axis2_hash_make(env);
    if(!attr_impl->methods || !attr_impl->ht_super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(attr_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_xml_schema_attribute_free);
    axis2_hash_set(attr_impl->methods, "type", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_attribute_type);
    axis2_hash_set(attr_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING,
            axis2_xml_schema_attribute_super_objs);            
    
    attr_impl->annotated = axis2_xml_schema_annotated_create(env);
    if(!attr_impl->methods)
    {
        axis2_xml_schema_attribute_free(&(attr_impl->attr), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    axis2_hash_set(attr_impl->ht_super, "AXIS2_XML_SCHEMA_ATTRIBUTE", 
            AXIS2_HASH_KEY_STRING, &(attr_impl->attr));
            
    axis2_hash_set(attr_impl->ht_super, "AXIS2_XML_SCHEMA_ANNOTATED", 
            AXIS2_HASH_KEY_STRING, attr_impl->annotated);

    axis2_hash_set(attr_impl->ht_super, "AXIS2_XML_SCHEMA_OBJ", 
            AXIS2_HASH_KEY_STRING, 
            AXIS2_XML_SCHEMA_ANNOTATED_GET_BASE_IMPL(attr_impl->annotated, env));
                            
    status = axis2_xml_schema_annotated_resolve_methods(
            &(attr_impl->attr.base), env, attr_impl->annotated, 
            attr_impl->methods); 
    return &(attr_impl->attr);
}

axis2_status_t AXIS2_CALL
axis2_xml_schema_attribute_free(void *attr,
                            const axis2_env_t *env)
{
    axis2_xml_schema_attribute_impl_t *attr_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);

    if(NULL != attr_impl->attribute_type)
    {
        /* TODO */
    }
    if(NULL != attr_impl->ht_super)
    {
        axis2_hash_free(attr_impl->ht_super, env);
        attr_impl->ht_super = NULL;    
    }
    if(NULL != attr_impl->default_value)
    {
        AXIS2_FREE(env->allocator, attr_impl->default_value);
        attr_impl->default_value = NULL;
    }
    if(NULL != attr_impl->default_value)
    {
        AXIS2_FREE(env->allocator, attr_impl->fixed_value);
        attr_impl->fixed_value = NULL;    
    }
    if(NULL != attr_impl->name)
    {
        AXIS2_FREE(env->allocator, attr_impl->name);
        attr_impl->name = NULL;
    }
    
    if(NULL != attr_impl->form)
    {
        AXIS2_XML_SCHEMA_FORM_FREE(attr_impl->form, env);
        attr_impl->form = NULL;
    }
    if(NULL != attr_impl->use)
    {
        AXIS2_XML_SCHEMA_USE_FREE(attr_impl->use, env);
        attr_impl->use = NULL;
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
    
    if(NULL != attr_impl->attr.ops)
    {
        AXIS2_FREE(env->allocator, attr_impl->attr.ops);
        attr_impl->attr.ops = NULL;
    }
    if(NULL != attr_impl->attr.base.ops)
    {
        AXIS2_FREE(env->allocator, attr_impl->attr.base.ops);
        attr_impl->attr.base.ops = NULL;    
    }

    AXIS2_FREE(env->allocator, attr_impl);
    attr_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_xml_schema_annotated_t *AXIS2_CALL
axis2_xml_schema_attribute_get_base_impl(void *attr,
                                const axis2_env_t *env)
{
    axis2_xml_schema_attribute_impl_t *attr_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    return attr_impl->annotated;
}

/*
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_xml_schema_attribute_resolve_methods(
                                axis2_xml_schema_attribute_t *attr,
                                const axis2_env_t *env,
                                axis2_xml_schema_attribute_t *attr_impl,
                                axis2_hash_t *methods)
{
    axis2_xml_schema_attribute_impl_t *any_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, attr_impl, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    
    any_impl_l = (axis2_xml_schema_attribute_impl_t *) attr_impl;
    
    attr->ops = AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_xml_schema_attribute_ops_t));
    if(NULL != attr->ops)
    {
        AXIS2_ERROR_SET(env->error, 
            AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }            
            
    attr->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    attr->ops->get_base_impl = 
            any_impl_l->attr.ops->get_base_impl;
    attr->ops->get_namespace = 
            any_impl_l->attr.ops->get_namespace;
    attr->ops->set_namespace = 
            any_impl_l->attr.ops->set_namespace;
    attr->ops->get_process_content = 
            any_impl_l->attr.ops->get_process_content;
    attr->ops->set_process_content = 
            any_impl_l->attr.ops->set_process_content;
    
    return axis2_xml_schema_annotated_resolve_methods(&(attr->base), 
            env, any_impl_l->annotated, methods);
}
*/



axis2_xml_schema_types_t AXIS2_CALL
axis2_xml_schema_attribute_type(void *attr,
                                    const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->obj_type;    
}                                    

axis2_hash_t* AXIS2_CALL
axis2_xml_schema_attribute_super_objs(void *attr,
                                        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->ht_super;
}

void * AXIS2_CALL 
axis2_xml_schema_attribute_get_attr_type(
            void *attr,
            const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->attribute_type;
}            

axis2_char_t * AXIS2_CALL 
axis2_xml_schema_attribute_get_default_value(
            void *attr,
            const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->default_value;
}                                    

axis2_status_t AXIS2_CALL 
axis2_xml_schema_attribute_set_default_value(
            void *attr,
            const axis2_env_t *env,
            axis2_char_t *default_value)
{
    axis2_xml_schema_attribute_impl_t *attr_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, default_value, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    if(NULL != attr_impl->default_value)
    {
        AXIS2_FREE(env->allocator, attr_impl->default_value);
        attr_impl->default_value = NULL;
    }
    attr_impl->default_value = AXIS2_STRDUP(default_value, env);
    return AXIS2_SUCCESS;
}            
    
axis2_char_t*  AXIS2_CALL 
axis2_xml_schema_attribute_get_fixed_value(void *attr,
                        const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->fixed_value;
}

axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_set_fixed_value(void *attr,
                     const axis2_env_t *env,
                     axis2_char_t *fixed_value)
{
    axis2_xml_schema_attribute_impl_t *attr_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, fixed_value, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    if(NULL != attr_impl->fixed_value)
    {
        AXIS2_FREE(env->allocator, attr_impl->fixed_value);
        attr_impl->fixed_value = NULL;
    }
    attr_impl->fixed_value = AXIS2_STRDUP(fixed_value, env);
    return AXIS2_SUCCESS;
}

axis2_xml_schema_form_t* AXIS2_CALL 
axis2_xml_schema_attribute_get_schema_form(void *attr,
                      const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->form;
}

axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_set_schema_form(void *attr,
                     const axis2_env_t *env,
                     axis2_xml_schema_form_t *schema_form)
{
    axis2_xml_schema_attribute_impl_t *attr_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, schema_form, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    if(NULL != attr_impl->form)
    {
    /* TODO */
    }
    attr_impl->form = schema_form;
    return AXIS2_SUCCESS;    

}
                     
axis2_qname_t*  AXIS2_CALL 
axis2_xml_schema_attribute_get_qname(void *attr,
               const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->qualified_name;
}
               
axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_set_qname(void *attr,
               const axis2_env_t *env,
               axis2_qname_t *qualified_name)
{
    axis2_xml_schema_attribute_impl_t *attr_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, qualified_name, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    if(NULL != attr_impl->qualified_name)
    {
    /* TODO */
    }
    attr_impl->qualified_name = qualified_name;
    return AXIS2_SUCCESS;    



}
               
axis2_char_t*  AXIS2_CALL 
axis2_xml_schema_attribute_get_name(void *attr,
              const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->name;
}
              
axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_set_name(void *attr,
              const axis2_env_t *env,
              axis2_char_t *name)
{
    axis2_xml_schema_attribute_impl_t *attr_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, name, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    if(NULL != attr_impl->name)
    {
        AXIS2_FREE(env->allocator, attr_impl->name);
        attr_impl->name = NULL;
    }
    attr_impl->name = AXIS2_STRDUP(name, env);
    return AXIS2_SUCCESS;
}
              
axis2_qname_t*  AXIS2_CALL 
axis2_xml_schema_attribute_get_ref_name(void *attr,
                  const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->ref_name;
}
                                              
axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_set_ref_name(void *attr,
                  const axis2_env_t *env,
                  axis2_qname_t *ref_name)
{
    axis2_xml_schema_attribute_impl_t *attr_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, ref_name, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    if(NULL != attr_impl->ref_name)
    {
    /* TODO */
    }
    attr_impl->ref_name = ref_name;
    return AXIS2_SUCCESS;    
}
                  
axis2_xml_schema_simple_type_t*  AXIS2_CALL 
axis2_xml_schema_attribute_get_schema_type(void *attr,
                     const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->schema_type;
}
                     
axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_set_schema_type(void *attr,
                     const axis2_env_t *env,
                     axis2_xml_schema_simple_type_t* sch_type)
{
    axis2_xml_schema_attribute_impl_t *attr_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, sch_type, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    if(NULL != attr_impl->schema_type)
    {
    /* TODO */
    }
    attr_impl->schema_type = sch_type;
    return AXIS2_SUCCESS;  

}                     
                     
axis2_qname_t*  AXIS2_CALL 
axis2_xml_schema_attribute_get_schema_type_name(void *attr,
                          const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->schema_type_name;
}
                          
                              
axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_set_schema_type_name(void *attr,
                          const axis2_env_t *env,
                          axis2_qname_t *sch_type_name)
{
    axis2_xml_schema_attribute_impl_t *attr_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, sch_type_name, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    if(NULL != attr_impl->schema_type_name)
    {
    /* TODO */
    }
    attr_impl->schema_type_name = sch_type_name;
    return AXIS2_SUCCESS;

}
                          
axis2_xml_schema_use_t*  AXIS2_CALL 
axis2_xml_schema_attribute_get_use(void *attr,
             const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(attr)->use;
}
             
axis2_status_t  AXIS2_CALL 
axis2_xml_schema_attribute_set_use(void *attr,
             const axis2_env_t *env,
             axis2_xml_schema_use_t *use)
{
    axis2_xml_schema_attribute_impl_t *attr_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, use, AXIS2_FAILURE);
    attr_impl = AXIS2_INTF_TO_IMPL(attr);
    if(NULL != attr_impl->use)
    {
    /* TODO */
    }
    attr_impl->use = use;
    return AXIS2_SUCCESS;

}
             
axis2_char_t*  AXIS2_CALL
axis2_xml_schema_attribute_to_string(void *attr,
               const axis2_env_t *env,
               axis2_char_t *prefix,
               int tab)
{
    /** TODO */
    return NULL;
}                                         