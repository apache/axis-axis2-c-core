/*
 *   Copyright 2003-2004 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <stdio.h>
#include <oxs_constants.h>
#include <oxs_error.h>
#include <oxs_token_reference_list.h>
#include <axiom_element.h>
#include <oxs_token_data_reference.h>
#include <axis2_array_list.h>

AXIS2_EXTERN axiom_node_t* AXIS2_CALL
oxs_token_build_reference_list_element(const axis2_env_t *env,
                        axiom_node_t *parent
                    )
{
    axiom_node_t *reference_list_node = NULL;
    axiom_element_t *reference_list_ele = NULL;
    axis2_status_t ret; 
    axiom_namespace_t *ns_obj = NULL;

    ns_obj = axiom_namespace_create (env, OXS_EncNs,
                                              OXS_xenc);
    

    reference_list_ele = axiom_element_create(env, parent, OXS_NodeReferenceList, ns_obj, &reference_list_node );
    if(!reference_list_ele)
    {   
        oxs_error(ERROR_LOCATION,
                    OXS_ERROR_ELEMENT_FAILED,"Error creating ReferenceList element");
        return NULL;
    }  
 
    return reference_list_node; 
}

AXIS2_EXTERN axis2_array_list_t *AXIS2_CALL
oxs_token_get_reference_list_data(const axis2_env_t *env, axiom_node_t *ref_list_node)
{
    axis2_array_list_t *list = NULL;
    axiom_children_qname_iterator_t *iter = NULL;
    axiom_element_t *ref_list_ele = NULL;
    axis2_qname_t *qname = NULL;

    if(!ref_list_node){
       oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
            "reference list node is NULL");
        return NULL;
    }
    ref_list_ele = AXIOM_NODE_GET_DATA_ELEMENT(ref_list_node, env);

    /*Get children*/
    qname = axis2_qname_create(env, OXS_NodeDataReference, NULL, NULL);
    iter = AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(ref_list_ele, env, qname, ref_list_node);
    AXIS2_QNAME_FREE(qname, env);
    qname= NULL;

    list = axis2_array_list_create(env, 0);
    
    while(AXIS2_TRUE == AXIOM_CHILDREN_QNAME_ITERATOR_HAS_NEXT(iter, env))
    {
        axiom_node_t *dref_node = NULL;
        axis2_char_t *dref_val = NULL;
        
        dref_node = AXIOM_CHILDREN_QNAME_ITERATOR_NEXT(iter, env);
        dref_val = oxs_token_get_data_reference(env, dref_node);
        
        AXIS2_ARRAY_LIST_ADD(list, env, dref_val);
    }

    return list;
}

