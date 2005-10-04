#include <apr.h>
#include <axis2_om_node.h>
#include <axis2_om_element.h>


int
main ()
{
    printf("START\n");
    if ( apr_initialize () != APR_SUCCESS )
        return -1;
    axis2_om_element_t *ele1, *ele2, *ele3;
    axis2_om_node_t *node1, *node2, *node3;
    axis2_om_attribute_t *attr1, *attr2;
    axis2_om_namespace_t *ns1, *ns2, *ns3;
    axis2_om_output_t* om_output;

    ns1 = axis2_om_namespace_create ("ns1", "test1");
    ns2 = axis2_om_namespace_create ("ns2", "test2");
    ns3 = axis2_om_namespace_create ("ns3", "test3");
    
    ele1 = axis2_om_element_create (NULL, "root", ns1, node1);
    ele2 = axis2_om_element_create (node1, "ele1", ns2, node2);

    //attr1 = axis2_om_attribute_create ("ele1", "attr1", ns2);
    //axis2_om_element_add_attribute (ele2, attr1);
    


    ele3 = axis2_om_element_create (NULL, "ele3",ns3 , node3);

    if (ele3->ns)
        printf("%s\n",ele3->ns->uri);
    else
        printf("ns null\n");

    //axis2_om_node_add_child (node1, node3);

    // serializing stuff
     om_output = axis2_create_om_output(stdout);
     axis2_om_element_serialize_start_part(ele3, om_output);
    // end serializing stuff
    
    printf("\nDONE\n");
}
