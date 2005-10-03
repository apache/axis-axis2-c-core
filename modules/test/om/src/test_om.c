#include <apr.h>
#include <axis2_node.h>
#include <axis2_om_element.h>


int
main ()
{
    if ( apr_initialize () != APR_SUCCESS )
        return -1;
    axis2_node_t *ele1, *ele2, *ele3;
    axis2_om_attribute_t *attr1, *attr2;
    axis2_om_namespace_t *ns1, *ns2;


    ns1 = axis2_om_namespace_create ("ns", "test");
    ele1 = axis2_om_element_create ("root", ns1, NULL);

    ns2 = axis2_om_namespace_create ("ns1", "test1");
    ele2 = axis2_om_element_create ("ele1", ns2, ele1);

    attr1 = axis2_om_attribute_create ("ele1", "attr1", ns2);

    axis2_om_element_add_attribute (ele2, attr1);


    ele3 = axis2_om_element_create ("ele2", ns2, NULL);

    axis2_node_add_child (ele1, ele2);






}
