#include <axutil_uri.h>
#include <axiom_util.h>
#include <axiom.h>
#include <axis2_util.h>
#include <axiom_node.h>
#include "../../../util/test/util/create_env.h"
    FILE *f = NULL;
    axiom_node_t *node = NULL;
    axiom_node_t *child = NULL;
    axiom_types_t node_type;
    axiom_node_t *first_node = NULL;
    axiom_element_t *my_ele = NULL;
    axiom_element_t *first_element = NULL;
    axis2_char_t *uri = "http://www.develop.com/student";
    const axutil_uri_t * uri1 = NULL;
    axiom_node_t *last_child = NULL;
    axiom_element_t *localname_last_child = NULL;
    axiom_element_t *localname_next_sibling = NULL;
    axiom_element_t *uri_localname_first_child = NULL;
    axiom_element_t *uri_localname_last_child = NULL;
    axiom_element_t *uri_localname_next_sibling = NULL;
    axiom_element_t *localname_first_child = NULL;
    axis2_char_t *localname = NULL;
    axiom_element_t *data_element = NULL;
    axiom_element_t *next_sibling = NULL;
    axiom_namespace_t *ns = NULL;
    axiom_element_t *last_element = NULL;
    axutil_qname_t *qname = NULL;
    axis2_char_t *attr_name = NULL;
    axiom_attribute_t *attr = NULL;
    axis2_char_t *attr_value = NULL;
    axiom_element_t *localname_attr_first_child = NULL;
    axiom_element_t *localname_attr_last_child = NULL;
    axiom_element_t *localname_attr_next_sibling = NULL;
    axiom_child_element_iterator_t * child_element = NULL;
    axis2_char_t * localpart = "type";
    axis2_char_t *child_node_text = NULL;
    axis2_char_t *node_namespace_uri = NULL;
    axiom_document_t * new_document = NULL;
    axutil_array_list_t * names;
    axiom_node_t *parent = NULL;
    axis2_char_t * target = NULL;
    axis2_char_t * value = NULL;
    axiom_node_t *temp_node = NULL;
    axiom_xml_reader_t *xml_reader = NULL;
int read_input_callback(char *buffer, int size, void* ctx)
{
     return fread(buffer, sizeof(char), size, f);
}
int close_input_callback(void *ctx)
{
     return fclose(f);
}
axis2_status_t build_and_serialize_om(axutil_env_t *env)
{
    axiom_node_t *root_node = NULL;

	    axiom_element_t *root_ele = NULL;
	    axiom_document_t *document = NULL;
	    axiom_stax_builder_t *om_builder = NULL;

	    f = fopen("test.xml","r");
	    xml_reader = axiom_xml_reader_create_for_io(env, read_input_callback, close_input_callback, NULL, NULL);
    if(!xml_reader)
     return -1;
    
    om_builder = axiom_stax_builder_create(env, xml_reader);
    if(!om_builder)
    {
         axiom_xml_reader_free(xml_reader, env);
         return AXIS2_FAILURE;
    }
    
    document = axiom_stax_builder_get_document(om_builder, env);
    if(!document)
    {
         axiom_stax_builder_free(om_builder, env);
         return AXIS2_FAILURE;
    }
    
    root_node = axiom_document_get_root_element(document, env);
    
    if(!root_node)
    {
         axiom_stax_builder_free(om_builder, env);
         return AXIS2_FAILURE;
    }
    if(root_node)
    {
        if(axiom_node_get_node_type(root_node, env) == AXIOM_ELEMENT)
        {
            root_ele = (axiom_element_t*)axiom_node_get_data_element(root_node, env);
            if(root_ele)
            {  
                printf(" %s  is the root element  \n" ,axiom_element_get_localname(root_ele, env));
            }
        }
    }

    axiom_document_build_all(document, env);
    child = axiom_node_get_first_child(root_node, env);
    printf ("%s\n", axiom_node_to_string (child, env)); 
    node = axiom_node_get_next_sibling(child, env);
    temp_node = axiom_node_get_next_sibling  (node, env);
    child = axiom_node_get_first_child(node, env);
    printf (" %s\n", axiom_node_to_string (temp_node, env));
    node_type = axiom_node_get_node_type(child,env);   
    data_element =(axiom_element_t*)axiom_node_get_data_element(child,env); 
    last_child = axiom_node_get_last_child(temp_node,env);
    ns = axiom_element_get_namespace((axiom_element_t*)axiom_node_get_data_element(root_node,env), env, root_node);
    printf("\nThe namespace = %s\n", axiom_namespace_to_string(ns,env));
    uri = axiom_namespace_get_uri(ns,env);
    axiom_util_get_next_siblng_element_with_localnames(my_ele,env,node,names,&child);   
    axiom_util_get_last_child_element_with_localnames(my_ele,env,node,names,&child);
    my_ele = axiom_util_get_first_child_element_with_uri(root_node,env,uri,&child);
    axiom_util_get_first_child_element_with_localnames(my_ele,env,node,names,&child);
    child = axiom_node_get_last_child(node, env); 
    axiom_util_new_document(env,uri1);
    printf("\nmy_ele = ");
    printf("%s\n ",axiom_element_to_string(my_ele,env,child));
    first_element =  axiom_util_get_first_child_element(my_ele,env,child,&child);
    printf("The first element = %s\n",axiom_element_to_string(first_element,env,node));
    last_element = axiom_util_get_last_child_element(my_ele,env,root_node,&child);
    localname = axiom_element_get_localname(my_ele,env); 
    localname_last_child = axiom_util_get_last_child_element_with_localname(my_ele,env,root_node,localname,&child);       
    localname_next_sibling = axiom_util_get_next_siblng_element_with_localname(my_ele,env,root_node,localname,&child);
    uri_localname_first_child = axiom_util_get_first_child_element_with_uri_localname(my_ele,env,root_node,localname,uri,&child);
    uri_localname_last_child = axiom_util_get_last_child_element_with_uri_localname(my_ele,env,root_node,localname,uri,&child);
    uri_localname_next_sibling = axiom_util_get_next_sibling_element_with_uri_localname(my_ele,env,root_node,localname,uri,&child);
    qname =  axutil_qname_create(env,localpart, NULL, NULL);
    printf("The qname is ");	
    printf("%s",axutil_qname_to_string(qname,env));
    printf("\nThe localname is ");
    printf("%s\n", axiom_element_get_localname(my_ele, env));
    attr = axiom_element_get_attribute(my_ele,env,qname);
    attr_name = axiom_attribute_get_localname(attr,env);
    attr_value = axiom_element_get_attribute_value(my_ele,env,qname);
    localname_attr_first_child =  axiom_util_get_first_child_element_with_localname_attr(my_ele,env,root_node,localname,attr_name,attr_value,&child);
    localname_attr_last_child = axiom_util_get_last_child_element_with_localname_attr(my_ele,env,root_node,localname,attr_name,attr_value,&child);
    localname_attr_next_sibling = axiom_util_get_next_siblng_element_with_localname_attr(my_ele,env,root_node,localname,attr_name,attr_value,&child);
    axiom_util_get_child_node_text(node,env);
    node_namespace_uri = axiom_util_get_node_namespace_uri(node,env);
    child_element =  axiom_util_get_child_elements(my_ele,env,node);
    printf("%s\n",axiom_element_to_string(localname_attr_next_sibling,env,node));
    printf("%s\n","test is SUCCESS");
    return AXIS2_SUCCESS;
}

int main()
{
    int status = AXIS2_SUCCESS;
    axutil_env_t *env = NULL;
    status = build_and_serialize_om(env);

    if(status == AXIS2_FAILURE)
    {
        printf(" build AXIOM failed");
    }

    axutil_env_free(env);
    return 0;
}






