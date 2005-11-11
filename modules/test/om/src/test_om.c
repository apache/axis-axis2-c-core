    #include <axis2_om_stax_builder.h>
#include <axis2_om_document.h>
#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <axis2_om_text.h>

#include <stdio.h>
#include <guththila_xml_pull_parser.h>

axis2_allocator_t *allocator = NULL;
axis2_env_t *environment = NULL;

guththila_environment_t *my_guththila_environment = NULL;
guththila_allocator_t *my_guththila_allocator = NULL;


int
test_om_build (char *file_name)
{
    guththila_reader_t *reader = NULL;
    guththila_xml_pull_parser_t *parser = NULL;
    axis2_om_element_t *ele1 = NULL, *ele2 = NULL, *ele3 = NULL, *ele4 = NULL;
    axis2_om_stax_builder_t *builder = NULL;
    axis2_om_text_t *text = NULL;
    axis2_om_document_t *document = NULL;
    axis2_om_node_t *node1 = NULL, *node2 = NULL, *node3 = NULL;
    FILE *fp = NULL;
    axis2_om_output_t *om_output = NULL;
    axis2_om_namespace_t* ns = NULL;

    fp = fopen (file_name, "r");
  
   
    reader = guththila_reader_create (my_guththila_environment, fp);

    parser =
        guththila_xml_pull_parser_create (my_guththila_environment, reader);

    guththila_xml_pull_parser_read (my_guththila_environment, parser);
 
    
    builder = axis2_om_stax_builder_create (&environment, parser,
                                      my_guththila_environment);
           
    document = axis2_om_document_create (&environment, NULL, builder);
    node1 = AXIS2_OM_DOCUMENT_GET_ROOT_ELEMENT (document,&environment);
    if(node1)
    {
        printf ("NODE TYPE %d\n",AXIS2_OM_NODE_GET_NODE_TYPE(node1,&environment));
    
        ele1 =AXIS2_OM_NODE_GET_DATA_ELEMENT(node1,&environment);
        if(ele1)
             
        printf ("root localname %s\n",AXIS2_OM_ELEMENT_GET_LOCALNAME(ele1,&environment));
         
        ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(ele1,&environment);
    
        if (ns)
        {
            printf ("root ns prefix %s\n", AXIS2_OM_NAMESPACE_GET_PREFIX(ns,&environment));
            printf ("root ns uri %s\n", AXIS2_OM_NAMESPACE_GET_PREFIX(ns,&environment));

        }
    }
    
    node2 = AXIS2_OM_DOCUMENT_BUILD_NEXT( document , &environment);
    do
    {

        if (!node2)
            break;

        switch (AXIS2_OM_NODE_GET_NODE_TYPE(node2,&environment))
        {
        case AXIS2_OM_ELEMENT:
            ele2 =(axis2_om_element_t*) AXIS2_OM_NODE_GET_DATA_ELEMENT(node2, &environment);
            if(ele2 && AXIS2_OM_ELEMENT_GET_LOCALNAME(ele2,&environment))
                printf("Element localname %s\n" , AXIS2_OM_ELEMENT_GET_LOCALNAME(ele2,&environment)); 
                        
            break;
        case AXIS2_OM_TEXT:
            
            text = (axis2_om_text_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(node2,&environment);    
            if( text && AXIS2_OM_TEXT_GET_VALUE(text ,&environment))
                printf("Text value %s \n", AXIS2_OM_TEXT_GET_VALUE(text,&environment));
            break;

        default:
            break;
        }

        node2 = AXIS2_OM_DOCUMENT_BUILD_NEXT (document , &environment);
    }
    while (node2);
    printf ("END: pull document\n");

    printf ("Serialize pulled document\n");
   
    om_output = axis2_om_output_create (&environment, NULL, NULL);
    AXIS2_OM_NODE_SERIALIZE (node1, &environment , om_output);
    
    AXIS2_FREE (environment->allocator, om_output);
    guththila_xml_pull_parser_free (my_guththila_environment, parser);
    
    printf ("\n\n");
}



int
test_om_serialize ()
{
    printf ("START: build document\n");
    /*
       <book xmlns:axiomc="http://ws.apache.org/axis2/c/om" xmlns:isbn="urn:ISBN:0-395-74341-6">
       <title>Axis2/C OM HOWTO</title>
       <isbn:number>1748491379</isbn:number>
       <author title="Mr" name="Axitoc Oman" />
       <notes>
       <p xmlns="urn:w3-org-ns:HTML">
       This is <i>vey good</i> book on OM!
       </p>
       </notes>
       </book>
    */
    axis2_om_element_t *ele1 = NULL, *ele2 = NULL, *ele3 = NULL, *ele4 =
        NULL, *ele5 = NULL;
    axis2_om_node_t *node1 = NULL, *node2 = NULL, *node3 = NULL, *node4 =
        NULL, *node5 = NULL, *node6 = NULL, *node7 = NULL;
    axis2_om_attribute_t *attr1 = NULL, *attr2 = NULL;
    axis2_om_namespace_t *ns1 = NULL, *ns2 = NULL, *ns3 = NULL;
    axis2_om_text_t *text1 = NULL, *text2 = NULL, *text3 = NULL;
    axis2_om_output_t *om_output = NULL;

    ns1 =
        axis2_om_namespace_create (&environment,
                                   "http://ws.apache.org/axis2/c/om",
                                   "axiom");
    ns2 =
        axis2_om_namespace_create (&environment, "urn:ISBN:0-395-74341-6",
                                   "isbn");
    ns3 =
        axis2_om_namespace_create (&environment, "urn:w3-org-ns:HTML", "NULL");

    ele1 = axis2_om_element_create (&environment, NULL, "book", ns1, &node1);
    AXIS2_OM_ELEMENT_DECLARE_NAMESPACE(ele1,&environment,node1,ns2);
    
    ele2 = axis2_om_element_create (&environment, node1, "title", ns1, &node2);
    
    text1 = axis2_om_text_create (&environment, node2, "Axis2/C OM HOWTO", &node3);
    
    ele3 = axis2_om_element_create (&environment, node1, "number", ns2, &node4);
    
    text1 = axis2_om_text_create (&environment, node4, "1748491379", &node5);
    
    ele4 = axis2_om_element_create (&environment, node1, "author", ns1, &node6);
    
    attr1 = axis2_om_attribute_create (&environment, "title", "Mr", ns1);
    
    AXIS2_OM_ELEMENT_ADD_ATTRIBUTE(ele4, &environment,attr1);
    
    attr2 = axis2_om_attribute_create (&environment, "name", "Axitoc Oman", ns1);
    
    AXIS2_OM_ELEMENT_ADD_ATTRIBUTE(ele4,&environment, attr2);
    
   
    /* serializing stuff */ 
    om_output = axis2_om_output_create (&environment, NULL, NULL);

    printf ("Serialize built document\n");
    int status = AXIS2_OM_NODE_SERIALIZE (node1,&environment ,om_output);
    if (status != AXIS2_SUCCESS)
    {
        printf ("\naxis2_om_node_serialize failed\n");
        return status;
    }
    else
        printf ("\naxis2_om_node_serialize success\n");
    /* end serializing stuff */

     AXIS2_OM_NODE_FREE(node1,&environment);
     AXIS2_FREE(environment->allocator,om_output);
     printf ("\nDONE\n");

    return 0;
}

int
main (int argc, char *argv[])
{
    char *file_name = "test.xml";
    if (argc > 1)
        file_name = argv[1];
    allocator = axis2_allocator_init (NULL);
    environment = axis2_env_create (allocator);

    my_guththila_allocator = guththila_allocator_init (NULL);
    my_guththila_environment =
        guththila_environment_create (my_guththila_allocator, NULL, NULL,
                                      NULL, NULL);

    test_om_build (file_name);
    test_om_serialize ();    
    
    
}
