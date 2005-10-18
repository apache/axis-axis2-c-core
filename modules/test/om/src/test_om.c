/*#include <axis2_om_stax_builder.h>*/
#include <axis2_om_document.h>
#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <axis2_om_text.h>

#include <stdio.h>
#include <apr.h>
#include <apr_general.h>

axis2_allocator_t *allocator = NULL;
axis2_string_t *string = NULL;
axis2_environment_t *environment = NULL;

int test_om_build()
{
    /*READER *red = NULL;
    XML_PullParser *parser = NULL;
    axis2_om_element_t *ele1=NULL,*ele2=NULL,*ele3 = NULL,*ele4 = NULL;
    axis2_stax_om_builder_t *builder = NULL;
    axis2_om_document_t *document = NULL;
    axis2_om_node_t *node1 = NULL ,*node2 = NULL ,*node3 = NULL ;
    FILE *fp = NULL;
    
    fp = fopen ("test.xml", "r");
    
    red = Reader_createReader (fp);
    
    parser = XML_PullParser_createPullParser (red);
    
    XML_PullParser_read (parser);
  
  
    if(apr_initialize() != APR_SUCCESS)
    {
       return;
    }
    if(!fp)
    {
      printf("Read Failed");
    }

    builder = axis2_stax_om_builder_create(parser);
    document = axis2_om_document_create(NULL,builder);
    node1  = axis2_om_document_get_root_element(document);
    printf(((axis2_om_element_t*)(node1->data_element))->localname);
    printf("\n");
    printf(((axis2_om_element_t*)(node1->data_element))->ns->prefix);
    printf("\n");
    printf(((axis2_om_element_t*)(node1->data_element))->ns->uri);
    printf("\n");
    node2  = axis2_om_document_get_first_child(document);
    printf("\n");
    printf(((axis2_om_element_t*)(node2->data_element))->localname);
    
    node3 = axis2_om_document_get_first_child(document);
    printf("\n");
    printf(((axis2_om_text_t*)(node3->data_element))->value);

*/

}
/*
<?xml version="1.0" ?>
<builder xmlns="http://www.wso2.com/"  test="xml"><document>TEST</document><a></a><b></b></builder>

*/

int test_om_serialize ()
{
    printf("START\n");
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
    axis2_om_element_t *ele1 = NULL, *ele2 = NULL, *ele3 = NULL, *ele4 = NULL, *ele5 = NULL;
    axis2_om_node_t *node1 = NULL, *node2 = NULL, *node3 = NULL, *node4 = NULL,
    *node5 = NULL, *node6 = NULL, *node7 = NULL;
    axis2_om_attribute_t *attr1 = NULL, *attr2 = NULL;
    axis2_om_namespace_t *ns1 = NULL, *ns2 = NULL, *ns3 = NULL;
    axis2_om_text_t *text1 = NULL, *text2 = NULL, *text3 = NULL;
    axis2_om_output_t* om_output = NULL;

    ns1 = axis2_om_namespace_create (environment, "http://ws.apache.org/axis2/c/om", "axiom");
    ns2 = axis2_om_namespace_create (environment, "urn:ISBN:0-395-74341-6", "isbn");
    ns3 = axis2_om_namespace_create (environment, "urn:w3-org-ns:HTML", "NULL");
    
    ele1 = axis2_om_element_create (environment, NULL, "book", ns1, &node1);
    /*axis2_om_element_declare_namespace(environment, node1, ns2); */

    ele2 = axis2_om_element_create (environment, node1, "title", ns1, &node2);
    text1 = axis2_om_text_create (environment, node2, "Axis2/C OM HOWTO", &node3);

    ele3 = axis2_om_element_create (environment, node1, "number", ns2, &node4);
    text1 = axis2_om_text_create (environment, node4, "1748491379", &node5);

    ele4 = axis2_om_element_create (environment, node1, "author", ns1, &node6);
    attr1 = axis2_om_attribute_create (environment, "title", "Mr", ns1);
    axis2_om_element_add_attribute (environment, ele4, attr1);
    attr2 = axis2_om_attribute_create (environment, "name", "Axitoc Oman", ns1);
    axis2_om_element_add_attribute (environment, ele4, attr2);
    

    ele2 = axis2_om_element_create (environment, NULL, "notes", ns1, &node2);
    ele3 = axis2_om_element_create (environment, node2, "p", ns3, &node3);
    text1 = axis2_om_text_create (environment, node3, "This is <i>vey good</i> book on OM!", &node4);

    /*ele3 = axis2_om_element_create (NULL, "ele3", ns3 , &node3);*/
    axis2_om_node_add_child (environment, node1, node2);

    /* serializing stuff */
     om_output = axis2_create_om_output(environment, NULL);
     /*axis2_om_element_serialize_start_part(ele3, om_output);
     axis2_om_element_serialize_end_part(ele3, om_output);*/

     int status = axis2_om_node_serialize(environment, node1, om_output);
     if (status != AXIS2_SUCCESS)
     {
        printf("\naxis2_om_node_serialize failed\n");
        return status;
     }
     else
        printf("\naxis2_om_node_serialize success\n");
    /* end serializing stuff*/
    
    printf("\nDONE\n");

    return 0;
}

int main(void)
{
    allocator = axis2_allocator_init (NULL);
    environment = axis2_environment_create (allocator, NULL, NULL, NULL, NULL);
    if ( apr_initialize () != APR_SUCCESS )
        return -1;
    test_om_build();
    test_om_serialize();
}
