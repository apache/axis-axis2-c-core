#include <rp_layout.h>
#include <rp_secpolicy_builder.h>
#include <rp_policy_creator.h>
#include <rp_qname_matcher.h>

AXIS2_EXTERN rp_secpolicy_t *AXIS2_CALL 
rp_policy_create_from_file(
    const axis2_env_t *env,
    axis2_char_t *filename)
{

    axiom_xml_reader_t *reader = NULL;
    axiom_stax_builder_t *builder = NULL;
    axiom_document_t *document = NULL;
    axiom_node_t *root = NULL;
    rp_secpolicy_t *secpolicy = NULL;

    reader = axiom_xml_reader_create_for_file(env,filename,NULL);

    if (!reader)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_CREATING_XML_STREAM_READER,
                AXIS2_FAILURE);
        return NULL;
    }
    
    builder = axiom_stax_builder_create(env, reader);
    if(!builder)
    {
        AXIOM_XML_READER_FREE(reader, env);
        return NULL;
    }
    document = axiom_stax_builder_get_document(builder, env);
    if(!document)
    {
        axiom_stax_builder_free(builder, env);
        return NULL;
    }

    root = axiom_document_get_root_element(document, env);
    if(!root)
    {
        axiom_stax_builder_free(builder, env);
        AXIS2_LOG_INFO(env->log,"[rp][policy_creator] Root element is NULL");
        return NULL;
    }
    else    
    {
        secpolicy = rp_policy_create_from_om_node(env,root);
        return secpolicy;
    }
}


AXIS2_EXTERN rp_secpolicy_t *AXIS2_CALL 
rp_policy_create_from_om_node(
    const axis2_env_t *env,
    axiom_node_t *root)
{

    axiom_element_t *all_ele = NULL;
    axiom_element_t *root_ele = NULL;
    axiom_node_t *exat_node = NULL;
    axiom_element_t *exat_ele = NULL;
    axiom_node_t *all_node = NULL;
    rp_secpolicy_t *secpolicy = NULL;
    axis2_char_t *local_name = NULL;
    
    if(AXIOM_NODE_GET_NODE_TYPE(root, env) == AXIOM_ELEMENT)
    {
        root_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT (root, env);
        if(root_ele)
        {
            local_name = axiom_element_get_localname(root_ele,env);
            if(local_name)
            {
                if(AXIS2_STRCMP(local_name,RP_POLICY)==0)
                {                    
                    if(!rp_match_policy_qname(env,local_name,root,root_ele))
                    {
                        AXIS2_LOG_INFO(env->log,"[rp][policy_creator] Error in policy configurations.");
                        return NULL;
                    }
                }
                else return NULL;
            }
            else return NULL;                
        }
        else return NULL;
    }
    else
        return NULL;
    
    exat_node = axiom_node_get_first_element(root,env);
    if(exat_node)
    {
        if(AXIOM_NODE_GET_NODE_TYPE(exat_node, env) == AXIOM_ELEMENT)
        {
            exat_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT (exat_node, env);
            if(exat_ele)
            {
                local_name = axiom_element_get_localname(exat_ele,env);
                if(local_name)
                {
                    if(AXIS2_STRCMP(local_name,RP_EXACTLY_ONE)==0)
                    {
                        if(!rp_match_policy_qname(env,local_name,exat_node,exat_ele))
                        {
                            AXIS2_LOG_INFO(env->log,"[rp][policy_creator] Error in policy configurations.");
                            return NULL;
                        }
                    }
                    else return NULL;
                }
                else return NULL;
            }
            else return NULL;
        }
        else 
            return NULL;
    }
    else
        return NULL;

    all_node = axiom_node_get_first_element(exat_node,env);
    if(all_node)
    {
        if(AXIOM_NODE_GET_NODE_TYPE(all_node, env) == AXIOM_ELEMENT)
        {
            all_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(all_node, env);
            if(all_ele)
            {
                local_name = axiom_element_get_localname(all_ele,env);
                if(local_name)
                {
                    if(AXIS2_STRCMP(local_name,RP_ALL)==0)
                    {
                        if(!rp_match_policy_qname(env,local_name,all_node,all_ele))
                        {
                            AXIS2_LOG_INFO(env->log,"[rp][policy_creator] Error in policy configurations.");
                            return NULL;
                        }
                        else
                        {
                            secpolicy = rp_secpolicy_builder_build(env,all_node);
                            return secpolicy;
                        }
                    }
                    else return NULL;
                }
                else return NULL;
            }
            else return NULL;
        }
        else return NULL;
    }
    else return NULL;
}


