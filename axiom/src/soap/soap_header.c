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
 
#include  "_axiom_soap_envelope.h"
 #include "_axiom_soap_header.h"
 #include <axiom_soap_header_block.h>
 #include <axis2_hash.h>
 #include <axiom_soap_const.h>
 #include <axiom_soap_builder.h>
 #include <stdio.h>
 #include <axiom_node_internal.h>
 #include <axis2_array_list.h>
 
 /******************* impl struct *********************************************/
 
 typedef struct axiom_soap_header_impl_t
 {
    axiom_soap_header_t soap_header;
    
    axiom_node_t *om_ele_node;
    
    int soap_version;
    
    axis2_hash_t *header_blocks;
    
    int hbnumber;
    
    axiom_soap_builder_t *soap_builder;
    
    axis2_array_list_t *header_block_keys;
    
 }axiom_soap_header_impl_t;
 
 /****************** Macro ****************************************************/
 
 #define AXIS2_INTF_TO_IMPL(header) ((axiom_soap_header_impl_t*)header)
 
 /****************** Function prototypes **************************************/

axis2_status_t AXIS2_CALL
axiom_soap_header_free(axiom_soap_header_t *header,
                       const axis2_env_t *env);
                                             
axiom_soap_header_block_t* AXIS2_CALL 
axiom_soap_header_add_header_block(axiom_soap_header_t* header,
                                   const axis2_env_t *env,
                                   const axis2_char_t *localname,
                                   axiom_namespace_t *ns); 
axis2_hash_t* AXIS2_CALL 
axiom_soap_header_examine_header_blocks
                                (axiom_soap_header_t* header,
                                 const axis2_env_t *env,
                                 axis2_char_t* param_role);
    
axiom_children_qname_iterator_t* AXIS2_CALL 
axiom_soap_header_examine_all_header_blocks
                                (axiom_soap_header_t* header,
                                 const axis2_env_t *env);
        
axiom_children_with_specific_attribute_iterator_t *
AXIS2_CALL axiom_soap_header_extract_header_blocks
                                (axiom_soap_header_t *header,
                                 const axis2_env_t *env,
                                 axis2_char_t *role);

axiom_node_t* AXIS2_CALL 
axiom_soap_header_get_base_node(axiom_soap_header_t *header,
                                const axis2_env_t *env);
                                             
axis2_status_t AXIS2_CALL 
axiom_soap_header_get_soap_version(axiom_soap_header_t *header,
                                   const axis2_env_t *env);
                                             
axis2_array_list_t* AXIS2_CALL
axiom_soap_header_get_header_blocks_with_namespace_uri
                                        (axiom_soap_header_t* header,
                                         const axis2_env_t *env,
                                         const axis2_char_t *ns_uri);  

axis2_hash_t* AXIS2_CALL 
axiom_soap_header_get_all_header_blocks(axiom_soap_header_t *header,
                                        const axis2_env_t *env);  
                                        
axis2_status_t AXIS2_CALL
axiom_soap_header_remove_header_block(axiom_soap_header_t *header,
                                      const axis2_env_t *env,
                                      axis2_qname_t *qname);                                    
                                                                                                   static axis2_bool_t AXIS2_CALL
axiom_soap_header_qname_matches(const axis2_env_t *env,
                                axis2_qname_t *element_qname,
                                axis2_qname_t *qname_to_match);          
                                   
/*************** function implementations *************************************/

AXIS2_EXTERN axiom_soap_header_t * AXIS2_CALL
axiom_soap_header_create(const axis2_env_t *env)
{
    axiom_soap_header_impl_t *header_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    header_impl = (axiom_soap_header_impl_t*)AXIS2_MALLOC(
                    env->allocator,
                    sizeof(axiom_soap_header_impl_t));
    if(!header_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    header_impl->soap_header.ops = NULL;
    header_impl->om_ele_node     = NULL;
    header_impl->hbnumber = 0;
    header_impl->header_blocks = NULL;
    /** default value */
    header_impl->soap_version = AXIOM_SOAP12;
    header_impl->header_block_keys = NULL;
    
    
    header_impl->soap_header.ops = NULL;
    header_impl->soap_header.ops = (axiom_soap_header_ops_t*)AXIS2_MALLOC(env->allocator,
                                    sizeof(axiom_soap_header_ops_t));
    if(!(header_impl->soap_header.ops))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE(env->allocator, header_impl);
        return NULL;
    }
    
    header_impl->header_block_keys = axis2_array_list_create(env, 10);
    if(!header_impl->header_block_keys)
    {
        AXIS2_FREE(env->allocator, header_impl->soap_header.ops);
        AXIS2_FREE(env->allocator, header_impl);
        return NULL;
    }
    
    header_impl->soap_header.ops->free_fn =
        axiom_soap_header_free;
    header_impl->soap_header.ops->add_header_block =
        axiom_soap_header_add_header_block;
    header_impl->soap_header.ops->examine_header_blocks =
        axiom_soap_header_examine_header_blocks;
    header_impl->soap_header.ops->examine_all_header_blocks =
        axiom_soap_header_examine_all_header_blocks;
    header_impl->soap_header.ops->extract_header_blocks =
        axiom_soap_header_extract_header_blocks;
    header_impl->soap_header.ops->get_base_node =
        axiom_soap_header_get_base_node;
    header_impl->soap_header.ops->get_soap_version =
        axiom_soap_header_get_soap_version; 
    header_impl->soap_header.ops->get_header_blocks_with_namespace_uri =
        axiom_soap_header_get_header_blocks_with_namespace_uri;
    header_impl->soap_header.ops->get_all_header_blocks =
        axiom_soap_header_get_all_header_blocks;        
    header_impl->soap_header.ops->remove_header_block =
        axiom_soap_header_remove_header_block;        
    return &(header_impl->soap_header);        
}

    
AXIS2_EXTERN axiom_soap_header_t * AXIS2_CALL
axiom_soap_header_create_with_parent(const axis2_env_t *env,
                                     axiom_soap_envelope_t *envelope)
{
    axiom_soap_header_impl_t *header_impl = NULL;
    axiom_soap_header_t *header = NULL;
    
    axiom_element_t *this_ele = NULL;
    axiom_node_t *this_node = NULL;
    axiom_node_t *body_node = NULL;    
    axiom_node_t *parent_node = NULL;
    axiom_element_t *parent_ele = NULL;
    
    axiom_namespace_t *parent_ns = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, envelope, NULL);
    
    header = axiom_soap_header_create(env);
    if(!header)
        return NULL;
    
    header_impl = AXIS2_INTF_TO_IMPL(header);
    
    
    header_impl->soap_version = AXIOM_SOAP_ENVELOPE_GET_SOAP_VERSION(envelope, env);
        
    parent_node = AXIOM_SOAP_ENVELOPE_GET_BASE_NODE(envelope, env);
    
    if(!parent_node || 
            AXIOM_NODE_GET_NODE_TYPE(parent_node, env) != AXIOM_ELEMENT)
    {
        AXIOM_SOAP_HEADER_FREE(header, env);
        return NULL;
    }        

    parent_ele = (axiom_element_t *)
                    AXIOM_NODE_GET_DATA_ELEMENT(parent_node, env);
    if(!parent_ele)
    {
        AXIOM_SOAP_HEADER_FREE(header, env);
        return NULL;
    }
    if(NULL != AXIOM_NODE_GET_FIRST_CHILD(parent_node, env))
    {
        body_node = AXIOM_NODE_GET_FIRST_CHILD(parent_node, env);
        AXIOM_NODE_DETACH(body_node, env);
    }

    parent_ns = AXIOM_ELEMENT_GET_NAMESPACE(parent_ele, env, parent_node);
    this_ele = axiom_element_create(env, parent_node,
                 AXIOM_SOAP_HEADER_LOCAL_NAME, parent_ns, &this_node);
    if(!this_ele)
    {
        AXIOM_SOAP_HEADER_FREE(header, env);
        return NULL;
    }
                           
    header_impl->om_ele_node = this_node;
    axiom_soap_envelope_set_header(envelope, env, header);
    
    if(NULL != body_node)
    {
        AXIOM_NODE_ADD_CHILD(parent_node, env, body_node);
    }
    return &(header_impl->soap_header);                 
}

axis2_status_t AXIS2_CALL
axiom_soap_header_free(axiom_soap_header_t *header,
                       const axis2_env_t *env)
{
    axiom_soap_header_impl_t *header_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    header_impl = AXIS2_INTF_TO_IMPL(header);
    
    if(NULL != header_impl->header_blocks)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first (header_impl->header_blocks ,env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
               axis2_hash_this (hi, NULL , NULL, &val);

                if (val)
                {   AXIOM_SOAP_HEADER_BLOCK_FREE((axiom_soap_header_block_t *)val, env);
                    val = NULL;
                }
        }
        
        axis2_hash_free (header_impl->header_blocks, env);
    }
    if(NULL != header_impl->header_block_keys)
    {
        int size = 0;
        void *val = NULL;
        int i = 0;
        size = AXIS2_ARRAY_LIST_SIZE(header_impl->header_block_keys, env);
        for(i = 0; i< size; i++)
        {
            val = AXIS2_ARRAY_LIST_GET(header_impl->header_block_keys, env, i);
            if(NULL != val)
            {
                AXIS2_FREE(env->allocator, (char*)val);
                val = NULL;
            }
        }
        AXIS2_ARRAY_LIST_FREE(header_impl->header_block_keys, env);
        header_impl->header_block_keys = NULL;
    }
    if(NULL != header->ops)
    {
        AXIS2_FREE(env->allocator, header->ops);
        header->ops = NULL;
    }
    AXIS2_FREE(env->allocator, header_impl);

    header_impl = NULL;
    
    return AXIS2_SUCCESS;
}
                                             
axiom_soap_header_block_t* AXIS2_CALL 
axiom_soap_header_add_header_block(axiom_soap_header_t* header,
                                   const axis2_env_t *env,
                                   const axis2_char_t *localname,
                                   axiom_namespace_t *ns)
{
    axiom_soap_header_impl_t *header_impl = NULL;
    axiom_soap_header_block_t *header_block = NULL;
    
    axiom_namespace_t *cloned_ns = NULL;
    
    axiom_node_t* header_block_node = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, localname, NULL);
    AXIS2_PARAM_CHECK(env->error, ns, NULL);
    
    header_impl = AXIS2_INTF_TO_IMPL(header);
    
    cloned_ns = AXIOM_NAMESPACE_CLONE(ns, env);
    if(!cloned_ns)
        return NULL;
        
    header_block = axiom_soap_header_block_create_with_parent(env, localname,cloned_ns, header);
    
    if(!header_block)
        return NULL;
    
    header_block_node = AXIOM_SOAP_HEADER_BLOCK_GET_BASE_NODE(
                            header_block, env);
    
    if(header_block_node)
    {
        axiom_element_t *hb_ele =  NULL;
        axis2_char_t *uri = NULL;
        axis2_char_t *prefix = NULL;
        
        axiom_node_set_complete(header_block_node, env, AXIS2_TRUE);
        
        hb_ele = (axiom_element_t *)
            AXIOM_NODE_GET_DATA_ELEMENT(header_block_node, env);
        if(hb_ele)
        {
            axiom_namespace_t *dec_ns = NULL;
            uri = AXIOM_NAMESPACE_GET_URI(cloned_ns, env);
            prefix = AXIOM_NAMESPACE_GET_PREFIX(cloned_ns, env);

            dec_ns = AXIOM_ELEMENT_FIND_DECLARED_NAMESPACE(hb_ele, env, uri, prefix);
            if(!dec_ns)
            {
                /** this namespace it not in hb_ele list so free it */
                AXIOM_NAMESPACE_FREE(cloned_ns, env);
                cloned_ns = NULL;
            }
        
        }
        return header_block;
    }
    else
    {
        return  NULL;
    }        
} 
axis2_hash_t* AXIS2_CALL 
axiom_soap_header_examine_header_blocks
                                (axiom_soap_header_t* header,
                                 const axis2_env_t *env,
                                 axis2_char_t* param_role)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, param_role, NULL);
    
    return AXIS2_INTF_TO_IMPL(header)->header_blocks;
}
    
axiom_children_qname_iterator_t* AXIS2_CALL 
axiom_soap_header_examine_all_header_blocks
                                (axiom_soap_header_t* header,
                                 const axis2_env_t *env)
{
    axiom_soap_header_impl_t *header_impl = NULL;
    axiom_element_t *om_ele = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    header_impl = AXIS2_INTF_TO_IMPL(header);
    if(!header_impl->om_ele_node)
        return NULL;
        
    om_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(header_impl->om_ele_node, env);
    
    if(om_ele)
    {
    return AXIOM_ELEMENT_GET_CHILDREN_WITH_QNAME(om_ele,
                env, NULL, header_impl->om_ele_node);
    }
    else
        return NULL;
}
        
axiom_children_with_specific_attribute_iterator_t *
AXIS2_CALL axiom_soap_header_extract_header_blocks
                                (axiom_soap_header_t *header,
                                 const axis2_env_t *env,
                                 axis2_char_t *role)
{
    axiom_soap_header_impl_t *header_impl = NULL;
    
    const axis2_char_t *localname = NULL;
    const axis2_char_t *nsuri     = NULL;
    
    axiom_node_t *first_node = NULL;
    axiom_element_t *first_ele =  NULL;
    
    axis2_qname_t *qn = NULL;
    
    axiom_element_t *header_om_ele = NULL;
    axiom_children_with_specific_attribute_iterator_t* iter = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    header_impl = AXIS2_INTF_TO_IMPL(header);

    if(header_impl->soap_version == AXIOM_SOAP_VERSION_NOT_SET)
        return NULL;
    if(header_impl->soap_version == AXIOM_SOAP11)
    {
        localname = AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI;
        nsuri     = AXIOM_SOAP11_ATTR_ACTOR;
    }
    if(header_impl->soap_version == AXIOM_SOAP12)
    {
        localname = AXIOM_SOAP12_SOAP_ROLE;
        nsuri     = AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI;
    }
    
    qn = axis2_qname_create(env, localname, nsuri, NULL);
    if(!qn)
        return NULL;
    
    header_om_ele = (axiom_element_t*)
        AXIOM_NODE_GET_DATA_ELEMENT(header_impl->om_ele_node, env);
    
    if(header_om_ele)
    {
        first_ele = AXIOM_ELEMENT_GET_FIRST_ELEMENT(header_om_ele, env,
                header_impl->om_ele_node, &first_node);
        if(NULL != first_node)
        {
            return axiom_children_with_specific_attribute_iterator_create(env, 
                    first_node, qn, role,   AXIS2_TRUE);
        }                    
    }

    AXIS2_QNAME_FREE(qn, env);
    
    return iter;        
            
}

axis2_status_t AXIS2_CALL 
axiom_soap_header_set_base_node(axiom_soap_header_t *header,
                                const axis2_env_t *env,
                                axiom_node_t *node)
{
    axiom_soap_header_impl_t *header_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, node, AXIS2_FAILURE);
    
    header_impl = AXIS2_INTF_TO_IMPL(header);
   
   if(AXIOM_NODE_GET_NODE_TYPE(node, env) != AXIOM_ELEMENT)
   {
        AXIS2_ERROR_SET(env->error, 
            AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
   }
   
   header_impl->om_ele_node = node;
   
   return AXIS2_SUCCESS;
}

axiom_node_t* AXIS2_CALL 
axiom_soap_header_get_base_node(axiom_soap_header_t *header,
                                const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(header)->om_ele_node;
}
/**
    set soap version and get soap version are internal functions  

*/
axis2_status_t AXIS2_CALL 
axiom_soap_header_get_soap_version(axiom_soap_header_t *header,
                                   const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(header)->soap_version;
}
                                             
axis2_status_t AXIS2_CALL 
axiom_soap_header_set_soap_version(axiom_soap_header_t *header,
                                   const axis2_env_t *env,
                                   int soap_version)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(header)->soap_version = soap_version;
    return AXIS2_SUCCESS;

}                                         

axis2_status_t AXIS2_CALL 
axiom_soap_header_set_header_block(axiom_soap_header_t *header,
                                   const axis2_env_t *env,
                                   struct axiom_soap_header_block *header_block)
{
    axis2_char_t *key = NULL ;
    axiom_soap_header_impl_t *header_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, header_block, AXIS2_FAILURE);
    
    header_impl = AXIS2_INTF_TO_IMPL(header);
    
    key = (axis2_char_t*)AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t)*10);
    
    if(!key)
        return AXIS2_FAILURE;
    
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
    if(NULL != header_impl->header_block_keys)
    {
        AXIS2_ARRAY_LIST_ADD(header_impl->header_block_keys, env, key);
    }
    return AXIS2_SUCCESS;
        
}         
axis2_status_t AXIS2_CALL 
axiom_soap_header_set_builder(axiom_soap_header_t *header,
                              const axis2_env_t *env,
                              struct axiom_soap_builder *builder)
{
    axiom_soap_header_impl_t *header_impl = NULL;
    AXIS2_PARAM_CHECK(env->error, builder, AXIS2_FAILURE);
    header_impl = AXIS2_INTF_TO_IMPL(header);
    
    header_impl->soap_builder = builder;
    return AXIS2_SUCCESS;
}

axis2_array_list_t* AXIS2_CALL
axiom_soap_header_get_header_blocks_with_namespace_uri
                                        (axiom_soap_header_t* header,
                                         const axis2_env_t *env,
                                         const axis2_char_t *ns_uri)
{
    axiom_soap_header_impl_t *header_impl = NULL;
    
    axis2_array_list_t *header_block_list = NULL;
    axis2_hash_index_t *hash_index = NULL;
    
    axiom_soap_header_block_t *header_block = NULL;
    
    axiom_node_t *header_block_om_node = NULL;
    axiom_element_t *header_block_om_ele = NULL;
    
    axiom_namespace_t *ns = NULL;
    axis2_char_t *hb_namespace_uri = NULL;
    
    int found = 0;
    void *hb =  NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ns_uri, NULL);
    
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
            header_block = (axiom_soap_header_block_t*)hb;
    
            header_block_om_node = AXIOM_SOAP_HEADER_BLOCK_GET_BASE_NODE(header_block, env);
            if(header_block_om_node)
            {
                header_block_om_ele  = (axiom_element_t *)
                    AXIOM_NODE_GET_DATA_ELEMENT(header_block_om_node, env);
                if(header_block_om_ele)
                {
                    ns = AXIOM_ELEMENT_GET_NAMESPACE(header_block_om_ele, env,
                             header_block_om_node);
                    if(NULL != ns)
                    {
                        hb_namespace_uri = AXIOM_NAMESPACE_GET_URI(ns, env);
                        if(AXIS2_STRCMP(hb_namespace_uri, ns_uri) == 0)
                        {
                            AXIS2_ARRAY_LIST_ADD(header_block_list, env, header_block);
                            found++;            
                        }
                    }
                }
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
axiom_soap_header_get_all_header_blocks(axiom_soap_header_t *header,
                                        const axis2_env_t *env)
{
    axiom_soap_header_impl_t *header_impl = NULL;
    header_impl = AXIS2_INTF_TO_IMPL(header);
    return header_impl->header_blocks;
}

axis2_status_t AXIS2_CALL
axiom_soap_header_remove_header_block(axiom_soap_header_t *header,
                                      const axis2_env_t *env,
                                      axis2_qname_t *qname)
{
    axiom_soap_header_impl_t *header_impl = NULL;
    axis2_char_t *qn_localname = NULL;
    axis2_char_t *qname_ns = NULL;
    axis2_char_t *qname_prefix = NULL;
    axis2_hash_index_t *hi  = NULL;
    
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    
    qn_localname = AXIS2_QNAME_GET_LOCALPART(qname, env);
    qname_ns  = AXIS2_QNAME_GET_URI(qname, env);
    qname_prefix = AXIS2_QNAME_GET_PREFIX(qname, env);
        
    header_impl = AXIS2_INTF_TO_IMPL(header);
    if(!header_impl->header_blocks)
        return AXIS2_FAILURE;
    
    
    for(hi = axis2_hash_first(header_impl->header_blocks, env);
        hi; hi = axis2_hash_next(env, hi))
    {
        void *val = NULL;
        void *key = NULL;
        
        axis2_hash_this(hi, &key, NULL, &val);
        if(NULL != val)
        {
            axiom_soap_header_block_t *header_block = NULL;
            axiom_element_t *ele = NULL;
            axiom_node_t *node   = NULL;
            
            header_block = (axiom_soap_header_block_t*)val;
            node = AXIOM_SOAP_HEADER_BLOCK_GET_BASE_NODE(header_block, env);
            if(NULL != node)
            {
                axis2_char_t *localname = NULL;
                axiom_namespace_t *ns        = NULL;
                axis2_char_t *prefix = NULL;
                axis2_char_t *ns_uri = NULL;
                axis2_qname_t *element_qname = NULL;
                                
                ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(node, env);
                element_qname = AXIOM_ELEMENT_GET_QNAME(ele, env, node);
                if(axiom_soap_header_qname_matches(env, element_qname, qname) == AXIS2_TRUE)
                {
                    AXIOM_NODE_DETACH(node, env);   
                    AXIOM_NODE_FREE_TREE(node, env);                
                    axis2_hash_set(header_impl->header_blocks, key, AXIS2_HASH_KEY_STRING,
                        NULL);
                    AXIOM_SOAP_HEADER_BLOCK_FREE(header_block, env);                        
                    return AXIS2_SUCCESS;
                }
            }            
        }
    }        
    return AXIS2_SUCCESS;
}                                      
                                      
static axis2_bool_t AXIS2_CALL
axiom_soap_header_qname_matches(const axis2_env_t *env,
                                axis2_qname_t *element_qname,
                                axis2_qname_t *qname_to_match)
{
    int lparts_match =  0;
    int uris_match = 0;
    axis2_char_t *ele_lpart = NULL;
    axis2_char_t *match_lpart = NULL;
    axis2_char_t *ele_nsuri = NULL;
    axis2_char_t *match_nsuri = NULL;
    
    if(!(qname_to_match))
        return AXIS2_TRUE;    
    if(qname_to_match)
    {
        match_lpart = AXIS2_QNAME_GET_LOCALPART(qname_to_match, env);
        match_nsuri = AXIS2_QNAME_GET_URI(qname_to_match, env);
    }
    if(element_qname)
    {
        ele_lpart = AXIS2_QNAME_GET_LOCALPART(element_qname, env);
        ele_nsuri = AXIS2_QNAME_GET_URI(element_qname, env);
    }
    
    lparts_match = ( !match_lpart || 
                     (AXIS2_STRCMP(match_lpart,"") == 0) ||
                     (element_qname && (AXIS2_STRCMP(ele_lpart, match_lpart) == 0)));

    
    uris_match = ( !match_nsuri || (AXIS2_STRCMP(match_nsuri,"") == 0) ||
                       (element_qname && (AXIS2_STRCMP(ele_nsuri, match_nsuri) == 0)));
    
    return lparts_match && uris_match;   
}