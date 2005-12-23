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
 #include <axis2_soap_fault.h>
 #include <axis2_soap.h>
 #include <axis2_soap_body.h>
 #include <axis2_om_node.h>
 #include <axis2_om_element.h>
 #include <axis2_soap_fault_code.h>
 #include <axis2_soap_fault_detail.h>
 #include <axis2_soap_fault_reason.h>
 #include <axis2_soap_fault_role.h>
 #include <axis2_soap_fault_node.h>
 #include <axis2_soap_utils.h>
 
/**************** impl struct ************************************************/

typedef struct axis2_soap_fault_impl_t
{
    axis2_soap_fault_t soap_fault;
    
    axis2_om_node_t *om_ele_node;
    
    axis2_om_element_t *om_ele;

    axis2_soap_body_t *parent;

    axis2_soap_fault_code_t *fcode;
    
    axis2_soap_fault_reason_t *freason;

    axis2_soap_fault_node_t *fnode;
    
    axis2_soap_fault_role_t *frole;
    
    axis2_soap_fault_detail_t *fdetail;
    
    axis2_char_t *exception;
    
    int soap_version;
    
}axis2_soap_fault_impl_t;

/************************* Macro **********************************************/

#define AXIS2_INTF_TO_IMPL(fault) ((axis2_soap_fault_impl_t*)fault)

/************************ function prototype **********************************/


axis2_status_t AXIS2_CALL 
axis2_soap_fault_free(axis2_soap_fault_t *fault,
                      axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_code(axis2_soap_fault_t *fault,
                          axis2_env_t **env,
                          axis2_soap_fault_code_t *code);
                                        
axis2_soap_fault_code_t * AXIS2_CALL 
axis2_soap_fault_get_code(axis2_soap_fault_t *fault,
                          axis2_env_t **env);
                                    
axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_reason(axis2_soap_fault_t *fault,
                            axis2_env_t **env,
                            axis2_soap_fault_reason_t *reason);
                                        
axis2_soap_fault_reason_t* AXIS2_CALL 
axis2_soap_fault_get_reason(axis2_soap_fault_t *fault,
                            axis2_env_t **env);
                                        
axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_node(axis2_soap_fault_t *fault,
                          axis2_env_t **env,
                          axis2_soap_fault_node_t *node);
                                        
axis2_soap_fault_node_t* AXIS2_CALL 
axis2_soap_fault_get_node(axis2_soap_fault_t *fault,
                          axis2_env_t **env);
                                        
axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_role(axis2_soap_fault_t *fault,
                          axis2_env_t **env,
                          axis2_soap_fault_role_t *role);
                                        
axis2_soap_fault_role_t* AXIS2_CALL 
axis2_soap_fault_get_role(axis2_soap_fault_t *fault,
                          axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_detail(axis2_soap_fault_t *fault,
                            axis2_env_t **env,
                            axis2_soap_fault_detail_t *detail);
                                
axis2_soap_fault_detail_t* AXIS2_CALL 
axis2_soap_fault_get_detail(axis2_soap_fault_t *fault,
                            axis2_env_t **env);   

axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_base_node(axis2_soap_fault_t *fault,
                               axis2_env_t **env,
                               axis2_om_node_t *node);
                                        
axis2_om_node_t* AXIS2_CALL 
axis2_soap_fault_get_base_node(axis2_soap_fault_t *fault,
                               axis2_env_t **env);  
                                                
axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_soap_version(axis2_soap_fault_t *fault,
                                  axis2_env_t **env,
                                  int soap_version);
                                    
int AXIS2_CALL 
axis2_soap_fault_get_soap_version(axis2_soap_fault_t *fault,
                                  axis2_env_t **env);    
                                  
axis2_char_t * AXIS2_CALL 
axis2_soap_fault_get_exception(axis2_soap_fault_t *fault,
                               axis2_env_t **env);
                                        
axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_exception(axis2_soap_fault_t *fault,
                               axis2_env_t **env,
                               axis2_char_t *exception);                                                                                                                                                                                                                                                                                         

/***************** function implementations ***********************************/

AXIS2_DECLARE(axis2_soap_fault_t *)
axis2_soap_fault_create(axis2_env_t **env)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    fault_impl = (axis2_soap_fault_impl_t*)AXIS2_MALLOC(
                    (*env)->allocator,
                    sizeof(axis2_soap_fault_impl_t));
    if(!fault_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    fault_impl->exception = NULL;
    fault_impl->fcode = NULL;
    fault_impl->fdetail = NULL;
    fault_impl->fnode = NULL;
    fault_impl->freason = NULL;
    fault_impl->frole = NULL;
    fault_impl->om_ele = NULL;
    fault_impl->om_ele_node = NULL;
    fault_impl->parent = NULL;
    fault_impl->soap_fault.ops = NULL;
    fault_impl->soap_version = AXIS2_SOAP_VERSION_NOT_SET;

    fault_impl->soap_fault.ops = (axis2_soap_fault_ops_t*)AXIS2_MALLOC(
                                 (*env)->allocator,
                                 sizeof(axis2_soap_fault_ops_t));

    if(!(fault_impl->soap_fault.ops))
    {
        AXIS2_FREE((*env)->allocator, fault_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;    
    }

    fault_impl->soap_fault.ops->get_soap_version =
        axis2_soap_fault_get_soap_version;
    fault_impl->soap_fault.ops->set_soap_version =
        axis2_soap_fault_set_soap_version;
    fault_impl->soap_fault.ops->free_fn =
        axis2_soap_fault_free;
    fault_impl->soap_fault.ops->get_base_node =
        axis2_soap_fault_get_base_node;
    fault_impl->soap_fault.ops->set_base_node =
        axis2_soap_fault_set_base_node;
    fault_impl->soap_fault.ops->set_code =
        axis2_soap_fault_set_code;
    fault_impl->soap_fault.ops->get_code =
        axis2_soap_fault_get_code;
    fault_impl->soap_fault.ops->set_reason =
        axis2_soap_fault_set_reason;
    fault_impl->soap_fault.ops->get_reason =
        axis2_soap_fault_get_reason;        
    fault_impl->soap_fault.ops->set_role =
        axis2_soap_fault_set_role;
    fault_impl->soap_fault.ops->get_role =
        axis2_soap_fault_get_role;
    fault_impl->soap_fault.ops->get_node =
        axis2_soap_fault_get_node;
    fault_impl->soap_fault.ops->set_node =
        axis2_soap_fault_set_node;
    fault_impl->soap_fault.ops->get_detail =
        axis2_soap_fault_get_detail;
    fault_impl->soap_fault.ops->set_detail =
        axis2_soap_fault_set_detail; 
    fault_impl->soap_fault.ops->get_exception =
        axis2_soap_fault_get_exception;
    fault_impl->soap_fault.ops->set_exception =
        axis2_soap_fault_set_exception;                    
                                                                                               
    return &(fault_impl->soap_fault);
}


AXIS2_DECLARE(axis2_soap_fault_t *)
axis2_soap_fault_create_with_parent(axis2_env_t **env,
                                    axis2_soap_body_t *body)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    axis2_soap_fault_t *fault = NULL;
    axis2_om_element_t *this_ele = NULL;
    axis2_om_node_t *this_node = NULL;
    axis2_om_node_t *parent_node = NULL;
    axis2_om_element_t *parent_ele = NULL;
    axis2_om_namespace_t *parent_ns = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, body, NULL);
    
    fault = axis2_soap_fault_create(env);
    if(!fault)
        return NULL;
        
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    
    fault_impl->parent = body;
    
    parent_node = AXIS2_SOAP_FAULT_GET_BASE_NODE(fault, env);
    
    parent_ele  = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(
                        parent_node, env);
    
    parent_ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(parent_ele, env);
    
    this_ele = axis2_om_element_create(env, 
                                       parent_node,                             
                                       AXIS2_SOAP_FAULT_LOCAL_NAME,
                                       parent_ns,
                                       &this_node);
    fault_impl->om_ele = this_ele;
    fault_impl->om_ele_node = this_node;                                       
    return  &(fault_impl->soap_fault);  

}


AXIS2_DECLARE(axis2_soap_fault_t *)
axis2_soap_fault_create_with_exception(axis2_env_t **env,
                                        axis2_soap_body_t *body,  
                                        axis2_char_t* exception)
{
    axis2_soap_fault_t *fault = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, body, NULL);
    AXIS2_PARAM_CHECK((*env)->error, exception, NULL);
    fault = axis2_soap_fault_create_with_parent(env, body);
    if(!fault)
        return NULL;
    axis2_soap_fault_set_exception(fault, env, exception);
    return fault;
}


axis2_status_t AXIS2_CALL 
axis2_soap_fault_free(axis2_soap_fault_t *fault,
                      axis2_env_t **env)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(fault, env, AXIS2_FAILURE);
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
        AXIS2_FREE((*env)->allocator, fault->ops);
        fault->ops = NULL;
    }
    AXIS2_FREE((*env)->allocator, fault_impl);
    fault_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_code(axis2_soap_fault_t *fault,
                          axis2_env_t **env,
                          axis2_soap_fault_code_t *code)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    axis2_om_node_t *code_node = NULL;
    axis2_om_node_t *this_node = NULL;
    axis2_om_node_t *my_node = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(fault, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, code, AXIS2_FAILURE);
    
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
   /* axis2_soap_fault_get_code(fault_code, env); */
    if(fault_impl->fcode)
    {
        my_node = AXIS2_SOAP_FAULT_CODE_GET_BASE_NODE(
                            fault_impl->fcode, env);
        AXIS2_SOAP_FAULT_CODE_FREE(fault_impl->fcode, env);
        fault_impl->fcode = NULL;
    }
    
    code_node = AXIS2_SOAP_FAULT_CODE_GET_BASE_NODE(
                            code, env);
    
    axis2_soap_utils_set_new_node(env, 
                    fault_impl->om_ele_node,
                    &my_node, 
                    code_node);
                    
    fault_impl->fcode = code;
    return AXIS2_SUCCESS;  
}
                                        
axis2_soap_fault_code_t * AXIS2_CALL 
axis2_soap_fault_get_code(axis2_soap_fault_t *fault,
                          axis2_env_t **env)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    axis2_om_node_t *code_node = NULL;
    axis2_om_node_t *this_node = NULL;
    axis2_soap_fault_code_t* code = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(fault, env, NULL);
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    this_node = fault_impl->om_ele_node;
    
    if(!(fault_impl->fcode))
    {
        code_node = axis2_soap_utils_get_child_with_name(env, 
                                            this_node, 
                                            AXIS2_SOAP12_SOAP_FAULT_CODE_LOCAL_NAME);
                                                    
        code = axis2_soap_fault_code_create(env);
        AXIS2_SOAP_FAULT_CODE_SET_BASE_NODE(code , env, code_node);
        fault_impl->fcode = code;
    }
    return fault_impl->fcode; 
}
                                    
axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_reason(axis2_soap_fault_t *fault,
                            axis2_env_t **env,
                            axis2_soap_fault_reason_t *reason)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    axis2_om_node_t *reason_node = NULL;
    axis2_om_node_t *this_node = NULL;
    axis2_om_node_t *my_node = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(fault, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, reason, AXIS2_FAILURE);
    
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
   /* axis2_soap_fault_get_reason(fault_reason, env); */
    if(fault_impl->freason)
    {
        my_node = AXIS2_SOAP_FAULT_REASON_GET_BASE_NODE(
                            fault_impl->freason, env);
        AXIS2_SOAP_FAULT_REASON_FREE(fault_impl->freason, env);
        fault_impl->freason = NULL;
    }
    
    reason_node = AXIS2_SOAP_FAULT_REASON_GET_BASE_NODE(
                            reason, env);
    
    axis2_soap_utils_set_new_node(env, 
                    fault_impl->om_ele_node,
                    &my_node, 
                    reason_node);
                    
    fault_impl->freason = reason;
    return AXIS2_SUCCESS;  

}
                                        
axis2_soap_fault_reason_t* AXIS2_CALL 
axis2_soap_fault_get_reason(axis2_soap_fault_t *fault,
                            axis2_env_t **env)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    axis2_om_node_t *reason_node = NULL;
    axis2_om_node_t *this_node = NULL;
    axis2_soap_fault_reason_t* reason = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(fault, env, NULL);
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    this_node = fault_impl->om_ele_node;
    
    if(!(fault_impl->freason))
    {
        reason_node = axis2_soap_utils_get_child_with_name(env, 
                                            this_node, 
                                            AXIS2_SOAP12_SOAP_FAULT_REASON_LOCAL_NAME);
                                                    
        reason = axis2_soap_fault_reason_create(env);
        AXIS2_SOAP_FAULT_REASON_SET_BASE_NODE(reason , env, reason_node);
        fault_impl->freason = reason;
    }
    return fault_impl->freason; 
}
                                        
axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_node(axis2_soap_fault_t *fault,
                          axis2_env_t **env,
                          axis2_soap_fault_node_t *node)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    axis2_om_node_t *node_node = NULL;
    axis2_om_node_t *this_node = NULL;
    axis2_om_node_t *my_node = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(fault, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, node, AXIS2_FAILURE);
    
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
   /* axis2_soap_fault_get_node(fault_node, env); */
    if(fault_impl->fnode)
    {
        my_node = AXIS2_SOAP_FAULT_NODE_GET_BASE_NODE(
                            fault_impl->fnode, env);
        AXIS2_SOAP_FAULT_NODE_FREE(fault_impl->fnode, env);
        fault_impl->fnode = NULL;
    }
    
    node_node = AXIS2_SOAP_FAULT_NODE_GET_BASE_NODE(
                            node, env);
    
    axis2_soap_utils_set_new_node(env, 
                    fault_impl->om_ele_node,
                    &my_node, 
                    node_node);
                    
    fault_impl->fnode = node;
    return AXIS2_SUCCESS;

}
                                        
axis2_soap_fault_node_t* AXIS2_CALL 
axis2_soap_fault_get_node(axis2_soap_fault_t *fault,
                          axis2_env_t **env)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    axis2_om_node_t *node_node = NULL;
    axis2_om_node_t *this_node = NULL;
    axis2_soap_fault_node_t* node = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(fault, env, NULL);
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    this_node = fault_impl->om_ele_node;
    
    if(!(fault_impl->fnode))
    {
        node_node = axis2_soap_utils_get_child_with_name(env, 
                                            this_node, 
                                            AXIS2_SOAP12_SOAP_FAULT_NODE_LOCAL_NAME);
                                                    
        node = axis2_soap_fault_node_create(env);
        AXIS2_SOAP_FAULT_NODE_SET_BASE_NODE(node , env, node_node);
        fault_impl->fnode = node;
    }
    return fault_impl->fnode; 

}
                                        
axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_role(axis2_soap_fault_t *fault,
                          axis2_env_t **env,
                          axis2_soap_fault_role_t *role)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    axis2_om_node_t *role_node = NULL;
    axis2_om_node_t *this_node = NULL;
    axis2_om_node_t *my_node = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(fault, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, role, AXIS2_FAILURE);
    
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
   /* axis2_soap_fault_get_role(fault_role, env); */
    if(fault_impl->frole)
    {
        my_node = AXIS2_SOAP_FAULT_ROLE_GET_BASE_NODE(
                            fault_impl->frole, env);
        AXIS2_SOAP_FAULT_ROLE_FREE(fault_impl->frole, env);
        fault_impl->frole = NULL;
    }
    
    role_node = AXIS2_SOAP_FAULT_ROLE_GET_BASE_NODE(
                            role, env);
    
    axis2_soap_utils_set_new_node(env, 
                    fault_impl->om_ele_node,
                    &my_node, 
                    role_node);
                    
    fault_impl->frole = role;
    return AXIS2_SUCCESS;  

}
                                        
axis2_soap_fault_role_t* AXIS2_CALL 
axis2_soap_fault_get_role(axis2_soap_fault_t *fault,
                          axis2_env_t **env)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    axis2_om_node_t *role_node = NULL;
    axis2_om_node_t *this_node = NULL;
    axis2_soap_fault_role_t* role = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(fault, env, NULL);
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    this_node = fault_impl->om_ele_node;
    
    if(!(fault_impl->frole))
    {
        role_node = axis2_soap_utils_get_child_with_name(env, 
                                            this_node, 
                                            AXIS2_SOAP12_SOAP_FAULT_ROLE_LOCAL_NAME);
                                                    
        role = axis2_soap_fault_role_create(env);
        AXIS2_SOAP_FAULT_ROLE_SET_BASE_NODE(role , env, role_node);
        fault_impl->frole = role;
    }
    return fault_impl->frole; 
}

axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_detail(axis2_soap_fault_t *fault,
                            axis2_env_t **env,
                            axis2_soap_fault_detail_t *detail)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    axis2_om_node_t *detail_node = NULL;
    axis2_om_node_t *this_node = NULL;
    axis2_om_node_t *my_node = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(fault, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, detail, AXIS2_FAILURE);
    
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
   /* axis2_soap_fault_get_detail(fault_detail, env); */
    if(fault_impl->fdetail)
    {
        my_node = AXIS2_SOAP_FAULT_DETAIL_GET_BASE_NODE(
                            fault_impl->fdetail, env);
        AXIS2_SOAP_FAULT_DETAIL_FREE(fault_impl->fdetail, env);
        fault_impl->fdetail = NULL;
    }
    
    detail_node = AXIS2_SOAP_FAULT_DETAIL_GET_BASE_NODE(
                            detail, env);
    
    axis2_soap_utils_set_new_node(env, 
                    fault_impl->om_ele_node,
                    &my_node, 
                    detail_node);
                    
    fault_impl->fdetail = detail;
    return AXIS2_SUCCESS;  


}
                                
axis2_soap_fault_detail_t* AXIS2_CALL 
axis2_soap_fault_get_detail(axis2_soap_fault_t *fault,
                            axis2_env_t **env)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    axis2_om_node_t *detail_node = NULL;
    axis2_om_node_t *this_node = NULL;
    axis2_soap_fault_detail_t* detail = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(fault, env, NULL);
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    this_node = fault_impl->om_ele_node;
    
    if(!(fault_impl->fdetail))
    {
        detail_node = axis2_soap_utils_get_child_with_name(env, 
                                            this_node, 
                                            AXIS2_SOAP12_SOAP_FAULT_DETAIL_LOCAL_NAME);
                                                    
        detail = axis2_soap_fault_detail_create(env);
        AXIS2_SOAP_FAULT_DETAIL_SET_BASE_NODE(detail , env, detail_node);
        fault_impl->fdetail = detail;
    }
    return fault_impl->fdetail; 
}   

axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_base_node(axis2_soap_fault_t *fault,
                               axis2_env_t **env,
                               axis2_om_node_t *node)
{
   axis2_soap_fault_impl_t *fault_impl = NULL;
   AXIS2_FUNC_PARAM_CHECK(fault, env, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK((*env)->error, node, AXIS2_FAILURE);
   fault_impl = AXIS2_INTF_TO_IMPL(fault);
   
   if(AXIS2_OM_NODE_GET_NODE_TYPE(node, env) != AXIS2_OM_ELEMENT)
   {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
   }
   fault_impl->om_ele_node = node;
   fault_impl->om_ele = (axis2_om_element_t *)
            AXIS2_OM_NODE_GET_DATA_ELEMENT(node, env);
   return AXIS2_SUCCESS;

}
                                        
axis2_om_node_t* AXIS2_CALL 
axis2_soap_fault_get_base_node(axis2_soap_fault_t *fault,
                               axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(fault, env, NULL);
    return AXIS2_INTF_TO_IMPL(fault)->om_ele_node;
}  
                                                
axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_soap_version(axis2_soap_fault_t *fault,
                                  axis2_env_t **env,
                                  int soap_version)
{
    AXIS2_FUNC_PARAM_CHECK(fault, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, soap_version, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(fault)->soap_version = soap_version;
    return AXIS2_SUCCESS;

}
                                    
int AXIS2_CALL 
axis2_soap_fault_get_soap_version(axis2_soap_fault_t *fault,
                                  axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(fault, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(fault)->soap_version;
}
                                  
axis2_char_t * AXIS2_CALL 
axis2_soap_fault_get_exception(axis2_soap_fault_t *fault,
                               axis2_env_t **env)
{
    axis2_soap_fault_detail_t *detail = NULL;
    axis2_om_node_t *detail_node = NULL;
    axis2_om_element_t *detail_ele = NULL;
    axis2_om_node_t *exception_node = NULL;
    axis2_om_element_t *exception_ele = NULL;
    axis2_qname_t *qn = NULL;
    axis2_char_t *excep = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(fault, env, NULL);
    detail = axis2_soap_fault_get_detail(fault, env);
    if(!detail)
        return NULL;
    detail_node = AXIS2_SOAP_FAULT_DETAIL_GET_BASE_NODE(detail, env);
    detail_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(
                    detail_node, env);
    qn = axis2_qname_create(env,
             AXIS2_SOAP_FAULT_DETAIL_EXCEPTION_ENTRY, NULL, NULL);
    
    exception_ele = AXIS2_OM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(
                        detail_ele, env,
                        qn, detail_node, &exception_node);
    AXIS2_QNAME_FREE(qn, env);                        
    if(exception_ele && 
           (excep = AXIS2_OM_ELEMENT_GET_TEXT(exception_ele, env, exception_node)))
    {
        return excep;
    }
        return NULL;
}
                                        
axis2_status_t AXIS2_CALL 
axis2_soap_fault_set_exception(axis2_soap_fault_t *fault,
                               axis2_env_t **env,
                               axis2_char_t *exception)
{
    axis2_soap_fault_impl_t *fault_impl = NULL;
    axis2_soap_fault_detail_t *detail = NULL;
    axis2_om_node_t *fault_detail_entry_node = NULL;
    axis2_om_element_t *fault_detail_ele = NULL;
    AXIS2_FUNC_PARAM_CHECK(fault, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, exception, AXIS2_FAILURE);
    fault_impl = AXIS2_INTF_TO_IMPL(fault);
    detail = axis2_soap_fault_get_detail(fault, env);
    if(!detail)
    {
        if(fault_impl->soap_version == AXIS2_SOAP11)
            detail = axis2_soap11_fault_detail_create(env, fault);
        if(fault_impl->soap_version == AXIS2_SOAP12)
            detail = axis2_soap12_fault_detail_create(env, fault);
    }
    axis2_soap_fault_set_detail(fault, env, detail);

    fault_detail_ele = axis2_om_element_create(env, NULL, 
                            AXIS2_SOAP_FAULT_DETAIL_EXCEPTION_ENTRY,
                            NULL, &fault_detail_entry_node);

    AXIS2_OM_ELEMENT_SET_TEXT(fault_detail_ele,
         env, exception, fault_detail_entry_node);

    AXIS2_SOAP_FAULT_DETAIL_ADD_DETAIL_ENTRY(detail, env, fault_detail_entry_node);             
    return AXIS2_SUCCESS;
}                                                                                                                                                                                                                                                                                        
                                  
