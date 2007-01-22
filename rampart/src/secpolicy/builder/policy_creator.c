#include <rp_layout.h>
/*#include <rp_secpolicy.h>*/
#include <rp_secpolicy_builder.h>
#include <rp_policy_creator.h>


AXIS2_EXTERN rp_secpolicy_t *AXIS2_CALL 
rp_policy_create_from_file(
    const axis2_env_t *env,
    axis2_char_t *filename)
{

    axiom_xml_reader_t *reader = NULL;
    axiom_stax_builder_t *builder = NULL;
    axiom_document_t *document = NULL;
    axiom_node_t *root = NULL;
    axiom_element_t *all_ele = NULL;
    axiom_element_t *root_ele = NULL;
    axiom_node_t *exat_node = NULL;
    axiom_element_t *exat_ele = NULL;
    axiom_node_t *all_node = NULL;
    rp_secpolicy_t *secpolicy = NULL;

    reader = axiom_xml_reader_create_for_file(env,filename,NULL);

    if (!reader)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_CREATING_XML_STREAM_READER,
                AXIS2_FAILURE);
        printf("xml reader creation failed\n");
        return NULL;
    }
    
    builder = axiom_stax_builder_create(env, reader);
    if(!builder)
    {
        AXIOM_XML_READER_FREE(reader, env);
        printf("Builder creation failed\n");
        return NULL;
    }
    document = AXIOM_STAX_BUILDER_GET_DOCUMENT(builder, env);
    if(!document)
    {
        AXIOM_STAX_BUILDER_FREE(builder, env);
        printf("Document creation failed\n");
        return NULL;
    }

    root = AXIOM_DOCUMENT_GET_ROOT_ELEMENT(document, env);
    if(!root)
    {
        AXIOM_STAX_BUILDER_FREE(builder, env);
        return NULL;
    }

    if(root)
    {
        if(AXIOM_NODE_GET_NODE_TYPE(root, env) == AXIOM_ELEMENT)
        {
            root_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT (root, env);
            if(root_ele)
            {
                printf("%s\n" , AXIOM_ELEMENT_GET_LOCALNAME(root_ele, env));
            }
        }
        else
            return NULL;
    }
    exat_node = AXIOM_NODE_GET_FIRST_CHILD(root,env);
    if(exat_node)
    {
        if(AXIOM_NODE_GET_NODE_TYPE(exat_node, env) == AXIOM_ELEMENT)
        {
            exat_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT (exat_node, env);
            if(exat_ele)
            {
                printf("%s\n" , AXIOM_ELEMENT_GET_LOCALNAME(exat_ele, env));
            }
        }
        else 
            return NULL;
    }
    else
        return NULL;

    all_node = AXIOM_NODE_GET_FIRST_CHILD(exat_node,env);
    if(all_node)
    {
        if(AXIOM_NODE_GET_NODE_TYPE(all_node, env) == AXIOM_ELEMENT)
        {
            all_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT (all_node, env);
            if(all_ele)
            {
                printf("%s\n" , AXIOM_ELEMENT_GET_LOCALNAME(all_ele, env));
                secpolicy = rp_secpolicy_builder_build(env,all_node);
            }
        }
        else 
            return NULL;
    }
    printf("Successful\n");
    return secpolicy;

}
