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
 #include "_axis2_soap_fault.h"
 #include <axis2_soap_const.h>
 #include "_axis2_soap_body.h"
 #include <axiom_node.h>
 #include <axiom_element.h>
 #include <axis2_soap_fault_code.h>
 #include <axis2_soap_fault_detail.h>
 #include <axis2_soap_fault_value.h>
 #include <axis2_soap_fault_text.h>
 #include <axis2_soap_fault_reason.h>
 #include <axis2_soap_fault_role.h>
 #include <axis2_soap_fault_node.h>
 #include <axis2_soap_builder.h>
 
/**************** impl struct ************************************************/

typedef struct axis2_soap_fault_impl_t
{
    axis2_soap_fault_t soap_fault;
    /* om element node corresponding to soap fault */
    axiom_node_t *om_ele_node;
    
    /* soap fault code sub element */
    axis2_soap_fault_code_t *fcode;
    /* soap fault reason sub element */
    axis2_soap_fault_reason_t *freason;
    /* soap fault node sub element */
    axis2_soap_fault_node_t *fnode;
    /* soap fault role sub element */
    axis2_soap_fault_role_t *frole;
    /* soap fault detail sub element */
    axis2_soap_fault_detail_t *fdetail;
    
    axis2_char_t *exception;
    /* reference to soap builder */
    axis2_soap_builder_t *soap_builder;
    
    int soap_version;
    
}axis2_soap_fault_impl_t;

/************************* Macro **********************************************/

#define AXIS2_INTF_TO_IMPL(fault) ((axis2_soap_fault_impl_t*)fault)

/************************ function prototype **********************************/


axis2_status_t AXIS2_CALL 
axis2_soap_fault_free(axis2_soap_fault_t *fault,
                      const axis2_env_t *env);


                                        
axis2_soap_fault_code_t * AXIS2_CALL 
axis2_soap_fault_get_code(axis2_soap_fault_t *fault,
                          const axis2_env_t *env);
                                    

                                        
axis2_soap_fault_reason_t* AXIS2_CALL 
axis2_soap_fault_get_reason(axis2_soap_fault_t *fault,
                            const axis2_env_t *env);
                                        

                                        
axis2_soap_fault_node_t* AXIS2_CALL 
axis2_soap_fault_get_node(axis2_soap_fault_t *fault,
                          const axis2_env_t *env);
                                        
axis2_soap_fault_role_t* AXIS2_CALL 
axis2_soap_fault_get_role(axis2_soap_fault_t *fault,
                          const axis2_env_t *env);

                                
axis2_soap_fault_detail_t* AXIS2_CALL 
axis2_soap_fault_get_detail(axis2_soap_fault_t *fault,
                            const axis2_env_t *env);   


                                        
axiom_node_t* AXIS2_CALL 
axis2_soap_fault_get_base_node(axis2_soap_fault_t *fault,
                               const axis2_env_t *env);  
                                                
                                  
axis2_char_t * AXIS2_CALL 
axis2_soap_fault_get_exception(axis2_soap_fault_t *fault,
                               const axis2_env_t *env);
                                        
axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_exception(axis2_soap_fault_t *fault,
                               const axis2_env_t *env,
                               axis2_char_t *exception);                                                                                                                                                                                                                                                                                         

/***************** function implementations ***********************************/

AXIS2_EXTERN axis2_soap_fault_t * AXIS2_CALL
axis2_soap_fault_create(const axis2_env_t *env)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    fault_impl = (axis2_soap_fault_impl_t*)AXIS2_MALLOC(
                    env->allocator,
                    sizeof(axis2_soap_fault_impl_t));
    if(!fault_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    fault_impl->exception = NULL;
    fault_impl->fcode = NULL;
    fault_impl->fdetail = NULL;
    fault_impl->fnode = NULL;
    fault_impl->freason = NULL;
    fault_impl->frole = NULL;
    fault_impl->om_ele_node = NULL;
    fault_impl->soap_fault.ops = NULL;
    fault_impl->soap_builder = NULL;
    fault_impl->soap_fault.ops = (axis2_soap_fault_ops_t*)AXIS2_MALLOC(
                                 env->allocator,
                                 sizeof(axis2_soap_fault_ops_t));

    if(!(fault_impl->soap_fault.ops))
    {
        AXIS2_FREE(env->allocator, fault_impl);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;    
    }

    fault_impl->soap_fault.ops->free_fn =
        axis2_soap_fault_free;
  
    fault_impl->soap_fault.ops->get_base_node =
        axis2_soap_fault_get_base_node;

    fault_impl->soap_fault.ops->get_code =
        axis2_soap_fault_get_code;

    fault_impl->soap_fault.ops->get_reason =
        axis2_soap_fault_get_reason;        

    fault_impl->soap_fault.ops->get_role =
        axis2_soap_fault_get_role;

    fault_impl->soap_fault.ops->get_node =
        axis2_soap_fault_get_node;

    fault_impl->soap_fault.ops->get_detail =
        axis2_soap_fault_get_detail;

    fault_impl->soap_fault.ops->get_exception =
        axis2_soap_fault_get_exception;
    fault_impl->soap_fault.ops->set_exception =
        axis2_soap_fault_set_exception;
    return &(fault_impl->soap_fault);
}


AXIS2_EXTERN axis2_soap_fault_t * AXIS2_CALL
axis2_soap_fault_create_with_parent(const axis2_env_t *env,
                                    axis2_soap_body_t *body)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    axis2_soap_fault_t *fault = NULL;
    axiom_element_t *this_ele = NULL;
    axiom_node_t *this_node = NULL;
    axiom_node_t *parent_node = NULL;
    axiom_element_t *parent_ele = NULL;
    axiom_namespace_t *parent_ns = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, body, NULL);
    
    fault = axis2_soap_fault_create(env);
    if(!fault)
        return NULL;
        
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    parent_node = AXIS2_SOAP_BODY_GET_BASE_NODE(body, env);
    if(!parent_node)
    {
        AXIS2_FREE(env->allocator, fault_impl);
        return NULL;
    }
    fault_impl->soap_version = 
        AXIS2_SOAP_BODY_GET_SOAP_VERSION(body, env);
    
    parent_ele  = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(
                        parent_node, env);
    if(!parent_ele)
    {
        AXIS2_FREE(env->allocator, fault_impl);
        return NULL;
    }
    parent_ns = AXIOM_ELEMENT_GET_NAMESPACE(parent_ele, env, parent_node);
    
    this_ele = axiom_element_create(env, 
                                       parent_node,                             
                                       AXIS2_SOAP_FAULT_LOCAL_NAME,
                                       parent_ns,
                                       &this_node);
    if(!this_ele)
    {
        AXIS2_FREE(env->allocator, fault_impl);
        return NULL;
    }
                                       
    fault_impl->om_ele_node = this_node;
    axis2_soap_body_set_fault (body, env, fault);    
    return  &(fault_impl->soap_fault);  
}


AXIS2_EXTERN axis2_soap_fault_t * AXIS2_CALL
axis2_soap_fault_create_with_exception(const axis2_env_t *env,
                                        axis2_soap_body_t *body,  
                                        axis2_char_t* exception)
{
    axis2_soap_fault_t *fault = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, body, NULL);
    AXIS2_PARAM_CHECK(env->error, exception, NULL);
    fault = axis2_soap_fault_create_with_parent(env, body);
    if(!fault)
        return NULL;
    status = axis2_soap_fault_set_exception(fault, env, exception);
    if(status == AXIS2_FAILURE)
    {
        axis2_soap_fault_free(fault, env);
        return NULL;
    }
    return fault;
}


axis2_status_t AXIS2_CALL 
axis2_soap_fault_free(axis2_soap_fault_t *fault,
                      const axis2_env_t *env)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    
    if(fault_impl->fcode)
    {
        AXIS2_SOAP_FAULT_CODE_FREE(fault_impl->fcode, env);
        fault_impl->fcode = NULL;
    }
    if(fault_impl->fdetail)
    {
        AXIS2_SOAP_FAULT_DETAIL_FREE(fault_impl->fdetail, env);
        fault_impl->fdetail = NULL;
    }
    if(fault_impl->fnode)
    {
        AXIS2_SOAP_FAULT_NODE_FREE(fault_impl->fnode, env);
        fault_impl->fnode = NULL;
    }    
    if(fault_impl->freason)
    {
        AXIS2_SOAP_FAULT_REASON_FREE(fault_impl->freason, env);
        fault_impl->freason = NULL;
    }
    if(fault_impl->frole)
    {
        AXIS2_SOAP_FAULT_ROLE_FREE(fault_impl->frole, env);
        fault_impl->frole = NULL;
    }
    if(fault->ops)
    {
        AXIS2_FREE(env->allocator, fault->ops);
        fault->ops = NULL;
    }
    AXIS2_FREE(env->allocator, fault_impl);
    fault_impl = NULL;
    return AXIS2_SUCCESS;
}
/**
 This is an internal function, and should only be used by soap 
 builder and builder helpers
*/

axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_code(axis2_soap_fault_t *fault,
                          const axis2_env_t *env,
                          axis2_soap_fault_code_t *code)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, code, AXIS2_FAILURE);
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    if(!(fault_impl->fcode))
    {
        fault_impl->fcode = code;
        return AXIS2_SUCCESS;
    }
    else
    {
        AXIS2_LOG_DEBUG(env->log,  
            AXIS2_LOG_SI, "tring to set multiple code elements to fault ");
       
    }
    return AXIS2_SUCCESS;  
}

axis2_soap_fault_code_t * AXIS2_CALL 
axis2_soap_fault_get_code(axis2_soap_fault_t *fault,
                          const axis2_env_t *env)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    int status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, NULL);
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    if(fault_impl->fcode)
    {
        return fault_impl->fcode;
    } 
    else if(fault_impl->soap_builder)
    {
        while(!(fault_impl->fcode) && !(AXIOM_NODE_IS_COMPLETE(fault_impl->om_ele_node, env)))
        {
            status = AXIS2_SOAP_BUILDER_NEXT(fault_impl->soap_builder, env);
            if(status == AXIS2_FAILURE)
                break;
        }            
    }
    return fault_impl->fcode; 
}
/**
    internal function 
    to be used by soap builder only

*/
axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_reason(axis2_soap_fault_t *fault,
                            const axis2_env_t *env,
                            axis2_soap_fault_reason_t *reason)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, reason, AXIS2_FAILURE);
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    if(!(fault_impl->freason))
    {
        fault_impl->freason = reason;
        return AXIS2_SUCCESS;
    }
    else
    {
        AXIS2_LOG_DEBUG(env->log, 
            AXIS2_LOG_SI, "tring to set fault reason twice");
    }
    return AXIS2_FAILURE;  
}
                                        
axis2_soap_fault_reason_t* AXIS2_CALL 
axis2_soap_fault_get_reason(axis2_soap_fault_t *fault,
                            const axis2_env_t *env)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    int status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, NULL);
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    if(fault_impl->freason)
    {
        return fault_impl->freason;
    } 
    else if(fault_impl->soap_builder)
    {
        while(!(fault_impl->freason) && !(AXIOM_NODE_IS_COMPLETE(fault_impl->om_ele_node, env)))
        {
            status = AXIS2_SOAP_BUILDER_NEXT(fault_impl->soap_builder, env);
            if(status == AXIS2_FAILURE)
                break;
        }            
    }
    return fault_impl->freason; 
}
/**
    internal function 
    to be used by soap builders only

*/
                                        
axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_node(axis2_soap_fault_t *fault,
                          const axis2_env_t *env,
                          axis2_soap_fault_node_t *node)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, node, AXIS2_FAILURE);
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    if(!(fault_impl->fnode))
    {
        fault_impl->fnode = node;
        return AXIS2_SUCCESS;
    }
    else
    {
        AXIS2_LOG_DEBUG(env->log, 
            AXIS2_LOG_SI, "tring to set fault node more than once");
    }
    return AXIS2_FAILURE;
}
                                        
axis2_soap_fault_node_t* AXIS2_CALL 
axis2_soap_fault_get_node(axis2_soap_fault_t *fault,
                          const axis2_env_t *env)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    int status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, NULL);
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    
    if(fault_impl->fnode)
    {
        return fault_impl->fnode;
    } 
    else if(fault_impl->soap_builder)
    {
        while(!(fault_impl->fnode) && !(AXIOM_NODE_IS_COMPLETE(fault_impl->om_ele_node, env)))
        {
            status = AXIS2_SOAP_BUILDER_NEXT(fault_impl->soap_builder, env);
            if(status == AXIS2_FAILURE)
                break;
        }            
    }
    return fault_impl->fnode; 
}
/**
    internal function to
    be used by soap builder only

*/
            
axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_role(axis2_soap_fault_t *fault,
                          const axis2_env_t *env,
                          axis2_soap_fault_role_t *role)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, role, AXIS2_FAILURE);
    
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    if(!(fault_impl->frole))
    {
        fault_impl->frole = role;
        return AXIS2_FAILURE;
    }
    else
    {
        AXIS2_LOG_DEBUG(env->log, 
            AXIS2_LOG_SI, "tring to set fault role more than once ");
    }
    return AXIS2_FAILURE;  

}
                                        
axis2_soap_fault_role_t* AXIS2_CALL 
axis2_soap_fault_get_role(axis2_soap_fault_t *fault,
                          const axis2_env_t *env)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    int status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, NULL);
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    if(fault_impl->frole)
    {
        return fault_impl->frole;
    } 
    else if(fault_impl->soap_builder)
    {
        while(!(fault_impl->frole) && !(AXIOM_NODE_IS_COMPLETE(fault_impl->om_ele_node, env)))
        {
            status = AXIS2_SOAP_BUILDER_NEXT(fault_impl->soap_builder, env);
            if(status == AXIS2_FAILURE)
                break;
        }            
    }
    return fault_impl->frole; 
}
/**
    internal function 
    to be used by soap builders only
*/
axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_detail(axis2_soap_fault_t *fault,
                            const axis2_env_t *env,
                            axis2_soap_fault_detail_t *detail)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, detail, AXIS2_FAILURE);
    
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    if(!(fault_impl->fdetail))
    {
        fault_impl->fdetail = detail;
        return AXIS2_SUCCESS;  
    }
    else
    {
        AXIS2_LOG_DEBUG(env->log, 
            AXIS2_LOG_SI," tring to set fault detail more than once");
    
    }
    return AXIS2_FAILURE;
}
                                
axis2_soap_fault_detail_t* AXIS2_CALL 
axis2_soap_fault_get_detail(axis2_soap_fault_t *fault,
                            const axis2_env_t *env)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    int status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, NULL);
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    if(fault_impl->fdetail)
    {
        return fault_impl->fdetail;
    } 
    else if(fault_impl->soap_builder)
    {
        while(!(fault_impl->fdetail) && !(AXIOM_NODE_IS_COMPLETE(fault_impl->om_ele_node, env)))
        {
            status = AXIS2_SOAP_BUILDER_NEXT(fault_impl->soap_builder, env);
            if(status == AXIS2_FAILURE)
                break;
        }            
    }
    return fault_impl->fdetail; 
}   

axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_base_node(axis2_soap_fault_t *fault,
                               const axis2_env_t *env,
                               axiom_node_t *node)
{
   axis2_soap_fault_impl_t *fault_impl = NULL;
   AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK(env->error, node, AXIS2_FAILURE);
   fault_impl = AXIS2_INTF_TO_IMPL(fault);

    if(AXIOM_NODE_GET_NODE_TYPE(node, env) != AXIOM_ELEMENT)
   {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
   }
   fault_impl->om_ele_node = node;
   return AXIS2_SUCCESS;

}
                                        
axiom_node_t* AXIS2_CALL 
axis2_soap_fault_get_base_node(axis2_soap_fault_t *fault,
                               const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(fault)->om_ele_node;
}  
                                                
                                  
axis2_char_t * AXIS2_CALL 
axis2_soap_fault_get_exception(axis2_soap_fault_t *fault,
                               const axis2_env_t *env)
{
    axis2_soap_fault_detail_t *detail = NULL;
    axiom_node_t *detail_node = NULL;
    axiom_element_t *detail_ele = NULL;
    axiom_node_t *exception_node = NULL;
    axiom_element_t *exception_ele = NULL;
    axis2_qname_t *qn = NULL;
    axis2_char_t *excep = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    detail = axis2_soap_fault_get_detail(fault, env);
    if(!detail)
        return NULL;
    detail_node = AXIS2_SOAP_FAULT_DETAIL_GET_BASE_NODE(detail, env);
    if(NULL != detail_node)
    {
        detail_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(
                    detail_node, env);
        
        qn = axis2_qname_create(env,
             AXIS2_SOAP_FAULT_DETAIL_EXCEPTION_ENTRY, NULL, NULL);
        if(qn)
        {
            exception_ele = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(
                        detail_ele, env,
                        qn, detail_node, &exception_node);
            AXIS2_QNAME_FREE(qn, env);                        
            if(exception_ele && 
               (excep = AXIOM_ELEMENT_GET_TEXT(exception_ele, env, exception_node)))
            {
                return excep;
            }
        }
    }
    return NULL;
}
                                        
axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_exception(axis2_soap_fault_t *fault,
                               const axis2_env_t *env,
                               axis2_char_t *exception)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    axis2_soap_fault_detail_t *detail = NULL;
    axiom_node_t *fault_detail_entry_node = NULL;
    axiom_element_t *fault_detail_ele = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    AXIS2_PARAM_CHECK(env->error, exception, AXIS2_FAILURE);
    
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    
    detail = axis2_soap_fault_get_detail(fault, env);
    
    if(!detail)
    {
        detail = axis2_soap_fault_detail_create_with_parent(env, fault);
        if(!detail)
            return AXIS2_FAILURE;
    }
    /** create an om element with the exception enrty */
    
    fault_detail_ele = axiom_element_create(env, NULL, 
                            AXIS2_SOAP_FAULT_DETAIL_EXCEPTION_ENTRY,
                            NULL, &fault_detail_entry_node);
    if(!fault_detail_ele)
    {
        return AXIS2_FAILURE;
    }
    /** set the exception string as a text node of newly created om element */
    AXIOM_ELEMENT_SET_TEXT(fault_detail_ele,
         env, exception, fault_detail_entry_node);
    /** now add this om element as a child of soap fault detail om element node */
    return AXIS2_SOAP_FAULT_DETAIL_ADD_DETAIL_ENTRY(detail, env, fault_detail_entry_node);             
}                                                                                                                                                                                                                                                                                        
                                  
axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_builder(axis2_soap_fault_t *fault,
                               const axis2_env_t *env,
                              axis2_soap_builder_t *builder)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, builder, AXIS2_FAILURE);
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    fault_impl->soap_builder = builder;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_soap_fault_t * AXIS2_CALL
axis2_soap_fault_create_default_fault(const axis2_env_t *env,
                                      struct axis2_soap_body *parent,
                                      const axis2_char_t *code_value,
                                      const axis2_char_t *reason_text,
                                      const int soap_version)
{
    axis2_soap_fault_t *soap_fault = NULL;
    axiom_node_t *fault_node    = NULL;
    
    axis2_soap_fault_code_t *soap_fault_code = NULL;
    axis2_soap_fault_value_t *soap_fault_value = NULL;
    
    axis2_soap_fault_reason_t *soap_fault_reason = NULL;
    axis2_soap_fault_text_t *soap_fault_text = NULL;
    
    axiom_node_t *value_node  = NULL;
    axiom_element_t *value_ele = NULL;
    
    axiom_node_t *text_node = NULL;
    axiom_element_t *text_ele = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, code_value, NULL);
    AXIS2_PARAM_CHECK(env->error, reason_text, NULL);

    soap_fault = axis2_soap_fault_create_with_parent(env, parent);
    if(!soap_fault)
        return NULL;
    fault_node = AXIS2_SOAP_FAULT_GET_BASE_NODE(soap_fault, env);
    if(!fault_node)
    {
        AXIS2_SOAP_FAULT_FREE(soap_fault, env);
        return NULL;
    }

    soap_fault_code = axis2_soap_fault_code_create_with_parent(env, soap_fault);
    if(!soap_fault_code)
    {
        AXIS2_SOAP_FAULT_FREE(soap_fault, env);
        AXIOM_NODE_FREE_TREE(fault_node, env);
        return NULL;
    }
    soap_fault_reason = axis2_soap_fault_reason_create_with_parent(env, soap_fault);
    if(!soap_fault_reason)
    {
        AXIS2_SOAP_FAULT_FREE(soap_fault, env);
        AXIOM_NODE_FREE_TREE(fault_node, env);
        return NULL;
    }
        
    soap_fault_value = axis2_soap_fault_value_create_with_code( env,soap_fault_code);
    if(!soap_fault_value)
    {
        AXIS2_SOAP_FAULT_FREE(soap_fault, env);
        AXIOM_NODE_FREE_TREE(fault_node, env);
        return NULL;
    }
    
    value_node = AXIS2_SOAP_FAULT_VALUE_GET_BASE_NODE(soap_fault_value, env);
    if(!value_node)
    {
        return NULL;
    }
    value_ele  = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(value_node, env);
    
    AXIOM_ELEMENT_SET_TEXT(value_ele, env, code_value, value_node);
    
    soap_fault_text = axis2_soap_fault_text_create_with_parent( env, soap_fault_reason);
    if(!soap_fault_text)
    {
        AXIS2_SOAP_FAULT_FREE(soap_fault, env);
        AXIOM_NODE_FREE_TREE(fault_node, env);
        return NULL;
    }
    AXIS2_SOAP_FAULT_TEXT_SET_LANG(soap_fault_text, env, "en");
    
    text_node = AXIS2_SOAP_FAULT_TEXT_GET_BASE_NODE(soap_fault_text, env);
    if(!text_node)
    {
        return NULL;
    }
    text_ele  = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(text_node, env);
    
    AXIOM_ELEMENT_SET_TEXT(text_ele, env, reason_text, text_node);
    
    return soap_fault;
}

axis2_status_t AXIS2_CALL
axis2_soap_fault_set_soap_version(axis2_soap_fault_t *fault,
                                  const axis2_env_t *env,
                                  int soap_version)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    fault_impl->soap_version = soap_version;
    return AXIS2_SUCCESS;
}                                  
                                  
int AXIS2_CALL
axis2_soap_fault_get_soap_version(axis2_soap_fault_t *fault,
                                  const axis2_env_t *env)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    return fault_impl->soap_version;
}
