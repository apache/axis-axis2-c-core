#include <apr.h>
#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <unistd.h>
#include <axis2_errno.h>
#include <axis2_om_text.h>

int
main ()
{
    printf("START\n");
    if ( apr_initialize () != APR_SUCCESS )
        return -1;
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

    ns1 = axis2_om_namespace_create ("http://ws.apache.org/axis2/c/om", "axiom");
    ns2 = axis2_om_namespace_create ("urn:ISBN:0-395-74341-6", "isbn");
    ns3 = axis2_om_namespace_create ("urn:w3-org-ns:HTML", "NULL");
    
    ele1 = axis2_om_element_create (NULL, "book", ns1, &node1);
    axis2_om_element_declare_namespace(node1, ns2);

    ele2 = axis2_om_element_create (node1, "title", ns1, &node2);
    text1 = axis2_om_text_create (node2, "Axis2/C OM HOWTO", &node3);

    ele3 = axis2_om_element_create (node1, "number", ns2, &node4);
    text1 = axis2_om_text_create (node4, "1748491379", &node5);

    ele4 = axis2_om_element_create (node1, "author", ns1, &node6);
    attr1 = axis2_om_attribute_create ("title", "Mr", ns1);
    axis2_om_element_add_attribute (node6, attr1);
    attr2 = axis2_om_attribute_create ("name", "Axitoc Oman", ns1);
    axis2_om_element_add_attribute (node6, attr2);
    

    ele2 = axis2_om_element_create (NULL, "notes", ns1, &node2);
    ele3 = axis2_om_element_create (node2, "p", ns3, &node3);
    text1 = axis2_om_text_create (node3, "This is <i>vey good</i> book on OM!", &node4);

    //ele3 = axis2_om_element_create (NULL, "ele3", ns3 , &node3);
    axis2_om_node_add_child (node1, node2);

    // serializing stuff
     om_output = axis2_create_om_output(stderr);
     //axis2_om_element_serialize_start_part(ele3, om_output);
     //axis2_om_element_serialize_end_part(ele3, om_output);

     int status = axis2_om_node_serialize(node1, om_output);
     if (status != AXIS2_SUCCESS)
     {
        printf("\naxis2_om_node_serialize failed\n");
        return status;
     }
    // end serializing stuff
    
    printf("\nDONE\n");

    return 0;
}
