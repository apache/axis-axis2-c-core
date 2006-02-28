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
 #include <axis2_soap_header_block.h>
 #include <axis2_hash.h>
 #include <axis2_soap.h>
 #include <axis2_soap_builder.h>
 #include <stdio.h>
 
 /******************* impl struct *********************************************/
 
 typedef struct axis2_soap_header_impl_t
 {
    axis2_soap_header_t soap_header;
    
    axis2_om_node_t *om_ele_node;
    
    axis2_om_element_t *om_ele;
    
    int soap_version;
    
    axis2_hash_t *header_blocks;
    
    int hbnumber;
    
    axis2_soap_builder_t *soap_builder;
    
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

axis2_status_t AXIS2_CALL 
axis2_soap_header_set_header_block(axis2_soap_header_t *header,
                                   axis2_env_t **env,
                                   axis2_soap_header_block_t *header_block);
                                   
axis2_status_t AXIS2_CALL 
axis2_soap_header_set_builder(axis2_soap_header_t *header,
                              axis2_env_t **env,
                              axis2_soap_builder_t *builder); 

axis2_array_list_t* AXIS2_CALL
axis2_soap_header_get_header_blocks_with_namespace_uri
                                        (axis2_soap_header_t* header,
                                         axis2_env_t **env,
                                         axis2_char_t *ns_uri);  

axis2_hash_t* AXIS2_CALL 
axis2_soap_header_get_all_header_blocks(axis2_soap_header_t *header,
                                        axis2_env_t **env);                                                                      
                                   
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
    header_impl->hbnumber = 0;
    header_impl->header_blocks = NULL;
    header_impl->soap_version = AXIS2_SOAP12;
    
    
    header_impl->soap_header.ops = NULL;
    header_impl->soap_header.ops = (axis2_soap_header_ops_t*)AXIS2_MALLOC((*env)->allocator,
                                    sizeof(axis2_soap_header_ops_t));
    if(!(header_impl->soap_header.ops))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE((*env)->allocator, header_impl);
        return NULL;
    }
    
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
        
    header_impl->soap_header.ops->get_soap_version =
        axis2_soap_header_get_soap_version; 
    header_impl->soap_header.ops->set_header_block = 
        axis2_soap_header_set_header_block;
    header_impl->soap_header.ops->set_builder =
        axis2_soap_header_set_builder;      
    header_impl->soap_header.ops->get_header_blocks_with_namespace_uri =
        axis2_soap_header_get_header_blocks_with_namespace_uri;
    header_impl->soap_header.ops->get_all_header_blocks =
        axis2_soap_header_get_all_header_blocks;        
        
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
    header_impl->soap_version = AXIS2_SOAP_ENVELOPE_GET_SOAP_VERSION(envelope, env);
        
    parent_node = AXIS2_SOAP_ENVELOPE_GET_BASE_NODE(envelope, env);
    parent_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(parent_node, env);
    parent_ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(parent_ele, env);
    
    this_ele = axis2_om_element_create(env, parent_node,
                 AXIS2_SOAP_HEADER_LOCAL_NAME, parent_ns, &this_node);
    header_impl->om_ele = this_ele;
    header_impl->om_ele_node = this_node;
    AXIS2_SOAP_ENVELOPE_SET_HEADER(envelope, env, header);
    return &(header_impl->soap_header);                 
}

axis2_status_t AXIS2_CALL
axis2_soap_header_free(axis2_soap_header_t *header,
                       axis2_env_t **env)
{
    axis2_soap_header_impl_t *header_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    header_impl = AXIS2_INTF_TO_IMPL(header);
    
    if(header_impl->header_blocks)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        void *key = NULL;
            
        for (hi = axis2_hash_first (header_impl->header_blocks ,env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
               axis2_hash_this (hi, &key, NULL, &val);

                if (val)
                {   AXIS2_SOAP_HEADER_BLOCK_FREE((axis2_soap_header_block_t *)val, env);
                    val = NULL;
                }
                if(key)
                {
                    AXIS2_FREE((*env)->allocator, key);
                    key = NULL;                    
                }
                    
                   
         }
        AXIS2_FREE((*env)->allocator, hi); 
        axis2_hash_free (header_impl->header_blocks, env);
    }
    if(header->ops)
    {
        AXIS2_FREE((*env)->allocator, header->ops);
        header->ops = NULL;
    }
    AXIS2_FREE((*env)->allocator, header_impl);
    header_impl = NULL;
    return AXIS2_SUCCESS;
}
                                             
axis2_soap_header_block_t* AXIS2_CALL 
axis2_soap_header_add_header_block(axis2_soap_header_t* header,
                                   axis2_env_t **env,
                                   axis2_char_t *localname,
                                   axis2_om_namespace_t *ns)
{
    axis2_soap_header_impl_t *header_impl = NULL;
    axis2_soap_header_block_t *header_block = NULL;
    axis2_om_node_t* header_block_node = NULL;
    axis2_char_t *key = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, localname, NULL);
    AXIS2_PARAM_CHECK((*env)->error, ns, NULL);
    header_impl = AXIS2_INTF_TO_IMPL(header);
    header_block = axis2_soap_header_block_create_with_parent(env, localname, ns, header);
    header_block_node = AXIS2_SOAP_HEADER_BLOCK_GET_BASE_NODE(
                            header_block, env);
    AXIS2_OM_NODE_SET_BUILD_STATUS(header_block_node, env, AXIS2_TRUE);
    key = (axis2_char_t*)AXIS2_MALLOC((*env)->allocator, sizeof(axis2_char_t)*10);
    return header_block;
} 
axis2_hash_t* AXIS2_CALL 
axis2_soap_header_examine_header_blocks
                                (axis2_soap_header_t* header,
                                 axis2_env_t **env,
                                 axis2_char_t* param_role)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, param_role, NULL);
    return AXIS2_INTF_TO_IMPL(header)->header_blocks;


}
    
axis2_om_children_qname_iterator_t* AXIS2_CALL 
axis2_soap_header_examine_all_header_blocks
                                (axis2_soap_header_t* header,
                                 axis2_env_t **env)
{
    axis2_soap_header_impl_t *header_impl = NULL;
    axis2_om_element_t *om_ele = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    header_impl = AXIS2_INTF_TO_IMPL(header);
    om_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(header_impl->om_ele_node, env);
    return AXIS2_OM_ELEMENT_GET_CHILDREN_WITH_QNAME(om_ele,
                env, NULL, header_impl->om_ele_node);

}
        
axis2_om_children_with_specific_attribute_iterator_t *
AXIS2_CALL axis2_soap_header_extract_header_blocks
                                (axis2_soap_header_t *header,
                                 axis2_env_t **env,
                                 axis2_char_t *role)
{
    axis2_soap_header_impl_t *header_impl = NULL;
    axis2_char_t *localname = NULL;
    axis2_char_t *nsuri     = NULL;
    axis2_om_node_t *first_node = NULL;
    axis2_om_element_t *first_ele =  NULL;
    axis2_qname_t *qn = NULL;
    axis2_om_children_with_specific_attribute_iterator_t* iter = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    header_impl = AXIS2_INTF_TO_IMPL(header);

    if(header_impl->soap_version == AXIS2_SOAP_VERSION_NOT_SET)
        return NULL;
    if(header_impl->soap_version == AXIS2_SOAP11)
    {
        localname = AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI;
        nsuri     = AXIS2_SOAP11_ATTR_ACTOR;
    }
    if(header_impl->soap_version == AXIS2_SOAP12)
    {
        localname = AXIS2_SOAP12_SOAP_ROLE;
        nsuri     = AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI;
    }
    
    qn = axis2_qname_create(env, localname, nsuri, NULL);
    first_ele = AXIS2_OM_ELEMENT_GET_FIRST_ELEMENT(header_impl->om_ele, env,
                header_impl->om_ele_node, &first_node);
    iter = axis2_om_children_with_specific_attribute_iterator_create(env, 
                first_node, qn, role,   AXIS2_TRUE);
    AXIS2_QNAME_FREE(qn, env);
    return iter;        
            
}

axis2_status_t AXIS2_CALL 
axis2_soap_header_set_base_node(axis2_soap_header_t *header,
                                axis2_env_t **env,
                                axis2_om_node_t *node)
{
    axis2_soap_header_impl_t *header_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, node, AXIS2_FAILURE);
    header_impl = AXIS2_INTF_TO_IMPL(header);
   
   if(AXIS2_OM_NODE_GET_NODE_TYPE(node, env) != AXIS2_OM_ELEMENT)
   {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
   }
   header_impl->om_ele_node = node;
   header_impl->om_ele = (axis2_om_element_t *)
            AXIS2_OM_NODE_GET_DATA_ELEMENT(node, env);
   return AXIS2_SUCCESS;

}

axis2_om_node_t* AXIS2_CALL 
axis2_soap_header_get_base_node(axis2_soap_header_t *header,
                                axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(header)->om_ele_node;
}
                                             
axis2_status_t AXIS2_CALL 
axis2_soap_header_get_soap_version(axis2_soap_header_t *header,
                                   axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(header)->soap_version;

}
                                             
axis2_status_t AXIS2_CALL 
axis2_soap_header_set_soap_version(axis2_soap_header_t *header,
                                   axis2_env_t **env,
                                   int soap_version)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(header)->soap_version = soap_version;
    return AXIS2_SUCCESS;

}                                         

axis2_status_t AXIS2_CALL 
axis2_soap_header_set_header_block(axis2_soap_header_t *header,
                                   axis2_env_t **env,
                                   struct axis2_soap_header_block *header_block)
{
    axis2_char_t *key = NULL ;
    axis2_soap_header_impl_t *header_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, header_block, AXIS2_FAILURE);
    header_impl = AXIS2_INTF_TO_IMPL(header);
    key = (axis2_char_t*)AXIS2_MALLOC((*env)->allocator, sizeof(axis2_char_t)*10);
    sprintf(key,"%d", header_impl->hbnumber++);
    if(header_impl->header_blocks)
    {
        axis2_hash_set(header_impl->header_blocks,
             key , AXIS2_HASH_KEY_STRING, header_block);
    }
    else
    {
            header_impl->header_blocks = axis2_hash_make(env);
            axis2_hash_set(header_impl->header_blocks,
                key , AXIS2_HASH_KEY_STRING, header_block);
    }
    return AXIS2_SUCCESS;
        
}         
axis2_status_t AXIS2_CALL 
axis2_soap_header_set_builder(axis2_soap_header_t *header,
                              axis2_env_t **env,
                              struct axis2_soap_builder *builder)
{
    axis2_soap_header_impl_t *header_impl = NULL;
    AXIS2_PARAM_CHECK((*env)->error, builder, AXIS2_FAILURE);
    header_impl = AXIS2_INTF_TO_IMPL(header);
    header_impl->soap_builder = builder;
    return AXIS2_SUCCESS;
}

axis2_array_list_t* AXIS2_CALL
axis2_soap_header_get_header_blocks_with_namespace_uri
                                        (axis2_soap_header_t* header,
                                         axis2_env_t **env,
                                         axis2_char_t *ns_uri)
{
    axis2_soap_header_impl_t *header_impl = NULL;
    axis2_array_list_t *header_block_list = NULL;
    axis2_hash_index_t *hash_index = NULL;
    axis2_soap_header_block_t *header_block = NULL;
    axis2_om_node_t *header_block_om_node = NULL;
    axis2_om_element_t *header_block_om_ele = NULL;
    axis2_om_namespace_t *ns = NULL;
    axis2_char_t *hb_namespace_uri = NULL;
    int found = 0;
    void *hb =  NULL;
    AXIS2_PARAM_CHECK((*env)->error, ns_uri, NULL);
    header_impl = AXIS2_INTF_TO_IMPL(header);
    if(!(header_impl->header_blocks))
        return NULL;
    
    header_block_list = axis2_array_list_create(env, 10);
    if(!header_block_list)
        return NULL;
        
    for(hash_index = axis2_hash_first(header_impl->header_blocks, env);
            hash_index; hash_index = axis2_hash_next( env, hash_index))
    {
        axis2_hash_this(hash_index, NULL, NULL, &hb);
        if(hb)
        {
            header_block = (axis2_soap_header_block_t*)hb;
            header_block_om_node = AXIS2_SOAP_HEADER_BLOCK_GET_BASE_NODE(header_block, env);
            header_block_om_ele  = (axis2_om_element_t *)
                AXIS2_OM_NODE_GET_DATA_ELEMENT(header_block_om_node, env);
            ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(header_block_om_ele, env);
            hb_namespace_uri = AXIS2_OM_NAMESPACE_GET_URI(ns, env);
            if(AXIS2_STRCMP(hb_namespace_uri, ns_uri) == 0)
            {
                AXIS2_ARRAY_LIST_ADD(header_block_list, env, header_block);
                found++;            
            }            
        
            hb = NULL;
            header_block = NULL;
            header_block_om_ele = NULL;
            header_block_om_node = NULL;
            ns = NULL;
            hb_namespace_uri = NULL;
        }            
    }
    if(found > 0)
    {
        return header_block_list;  
    }
    else
    {
        AXIS2_ARRAY_LIST_FREE(header_block_list, env);
    }
    return NULL;        
}

axis2_hash_t* AXIS2_CALL 
axis2_soap_header_get_all_header_blocks(axis2_soap_header_t *header,
                                        axis2_env_t **env)
{
    axis2_soap_header_impl_t *header_impl = NULL;
    header_impl = AXIS2_INTF_TO_IMPL(header);
    return header_impl->header_blocks;
}
