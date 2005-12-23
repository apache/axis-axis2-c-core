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
 
 #include <axis2_soap_envelope.h>
 #include <axis2_soap_header.h>
 #include <axis2_hash.h>
 #include <axis2_soap.h>
 
 /******************* impl struct *********************************************/
 
 typedef struct axis2_soap_header_impl_t
 {
    axis2_soap_header_t soap_header;
    
    axis2_om_node_t *om_ele_node;
    
    axis2_om_element_t *om_ele;
    
    int soap_version;
    
    axis2_hash_t *header_blocks;
    
 }axis2_soap_header_impl_t;
 
 /****************** Macro ****************************************************/
 
 #define AXIS2_INTF_TO_IMPL(header) ((axis2_soap_header_impl_t*)header)
 
 /****************** Function prototypes **************************************/

axis2_status_t AXIS2_CALL
axis2_soap_header_free(axis2_soap_header_t *header,
                       axis2_env_t **env);
                                             
axis2_soap_header_block_t* AXIS2_CALL 
axis2_soap_header_add_header_block(axis2_soap_header_t* header,
                                   axis2_env_t **env,
                                   axis2_char_t *localname,
                                   axis2_om_namespace_t *ns); 
axis2_hash_t* AXIS2_CALL 
axis2_soap_header_examine_header_blocks
                                (axis2_soap_header_t* header,
                                 axis2_env_t **env,
                                 axis2_char_t* param_role);
    
axis2_om_children_qname_iterator_t* AXIS2_CALL 
axis2_soap_header_examine_all_header_blocks
                                (axis2_soap_header_t* header,
                                 axis2_env_t **env);
        
axis2_om_children_with_specific_attribute_iterator_t *
AXIS2_CALL axis2_soap_header_extract_header_blocks
                                (axis2_soap_header_t *header,
                                 axis2_env_t **env,
                                 axis2_char_t *role);

axis2_status_t AXIS2_CALL 
axis2_soap_header_set_base_node(axis2_soap_header_t *header,
                                axis2_env_t **env,
                                axis2_om_node_t *node);

axis2_om_node_t* AXIS2_CALL 
axis2_soap_header_get_base_node(axis2_soap_header_t *header,
                                axis2_env_t **env);
                                             
axis2_status_t AXIS2_CALL 
axis2_soap_header_get_soap_version(axis2_soap_header_t *header,
                                   axis2_env_t **env);
                                             
axis2_status_t AXIS2_CALL 
axis2_soap_header_set_soap_version(axis2_soap_header_t *header,
                                   axis2_env_t **env,
                                   int soap_version);                                                                                                                                                                                                       

/*************** function implementations *************************************/

AXIS2_DECLARE(axis2_soap_header_t *)
axis2_soap_header_create(axis2_env_t **env)
{
    axis2_soap_header_impl_t *header_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    header_impl = (axis2_soap_header_impl_t*)AXIS2_MALLOC(
                    (*env)->allocator,
                    sizeof(axis2_soap_header_impl_t));
    if(!header_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    header_impl->soap_header.ops = NULL;
    header_impl->om_ele          = NULL;
    header_impl->om_ele_node     = NULL;
    
    header_impl->soap_header.ops->free_fn =
        axis2_soap_header_free;
    header_impl->soap_header.ops->add_header_block =
        axis2_soap_header_add_header_block;
    header_impl->soap_header.ops->examine_header_blocks =
        axis2_soap_header_examine_header_blocks;
    header_impl->soap_header.ops->examine_all_header_blocks =
        axis2_soap_header_examine_all_header_blocks;
    header_impl->soap_header.ops->extract_header_blocks =
        axis2_soap_header_extract_header_blocks;
    header_impl->soap_header.ops->set_base_node =
        axis2_soap_header_set_base_node;
    header_impl->soap_header.ops->get_base_node =
        axis2_soap_header_get_base_node;
    header_impl->soap_header.ops->set_soap_version =
        axis2_soap_header_set_soap_version;
    header_impl->soap_header.ops->get_soap_version =
        axis2_soap_header_get_soap_version;
    return &(header_impl->soap_header);        
}

    
AXIS2_DECLARE(axis2_soap_header_t *)
axis2_soap_header_create_with_parent(axis2_env_t **env,
                                     axis2_soap_envelope_t *envelope)
{
    axis2_soap_header_impl_t *header_impl = NULL;
    axis2_soap_header_t *header = NULL;
    axis2_om_element_t *this_ele = NULL;
    axis2_om_node_t *this_node = NULL;
    axis2_om_node_t *parent_node = NULL;
    axis2_om_element_t *parent_ele = NULL;
    axis2_om_namespace_t *parent_ns = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, envelope, NULL);
    header = axis2_soap_header_create(env);
    if(!header)
        return NULL;
    header_impl = AXIS2_INTF_TO_IMPL(header);        
        
    parent_node = AXIS2_SOAP_ENVELOPE_GET_BASE_NODE(envelope, env);
    parent_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(parent_node, env);
    parent_ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(parent_ele, env);
    
    this_ele = axis2_om_element_create(env, parent_node,
                 AXIS2_SOAP_HEADER_LOCAL_NAME, parent_ns, &this_node);
    header_impl->om_ele = this_ele;
    header_impl->om_ele_node = this_node;
    return &(header_impl->soap_header);                 
}

axis2_status_t AXIS2_CALL
axis2_soap_header_free(axis2_soap_header_t *header,
                       axis2_env_t **env){}
                                             
axis2_soap_header_block_t* AXIS2_CALL 
axis2_soap_header_add_header_block(axis2_soap_header_t* header,
                                   axis2_env_t **env,
                                   axis2_char_t *localname,
                                   axis2_om_namespace_t *ns){} 
axis2_hash_t* AXIS2_CALL 
axis2_soap_header_examine_header_blocks
                                (axis2_soap_header_t* header,
                                 axis2_env_t **env,
                                 axis2_char_t* param_role){}
    
axis2_om_children_qname_iterator_t* AXIS2_CALL 
axis2_soap_header_examine_all_header_blocks
                                (axis2_soap_header_t* header,
                                 axis2_env_t **env){}
        
axis2_om_children_with_specific_attribute_iterator_t *
AXIS2_CALL axis2_soap_header_extract_header_blocks
                                (axis2_soap_header_t *header,
                                 axis2_env_t **env,
                                 axis2_char_t *role){}

axis2_status_t AXIS2_CALL 
axis2_soap_header_set_base_node(axis2_soap_header_t *header,
                                axis2_env_t **env,
                                axis2_om_node_t *node){}

axis2_om_node_t* AXIS2_CALL 
axis2_soap_header_get_base_node(axis2_soap_header_t *header,
                                axis2_env_t **env){}
                                             
axis2_status_t AXIS2_CALL 
axis2_soap_header_get_soap_version(axis2_soap_header_t *header,
                                   axis2_env_t **env){}
                                             
axis2_status_t AXIS2_CALL 
axis2_soap_header_set_soap_version(axis2_soap_header_t *header,
                                   axis2_env_t **env,
                                   int soap_version){}                                         
