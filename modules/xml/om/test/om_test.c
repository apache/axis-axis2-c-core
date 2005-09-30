#include <stdio.h>
#include <axis2c_om_element.h>
#include <axis2c_om_namespace.h>
#include <axis2c_node.h>
#include <apr.h>

/**
 *  This code shows how the currently implemented code support building of om tree programmtically
 *
 */





int main()
{
    axis2c_om_attribute_t *attr1;
	axis2c_om_namespace_t *ns1;
	axis2c_node_t *root,*ele1,*ele2;
	
	if(apr_initialize()!= APR_SUCCESS)
	{
		return -1;
	}


	ns1  = axis2c_create_om_namespace("ns1","test");
	
	root = axis2c_create_om_element("root",ns1);
	ele1 = axis2c_create_om_element_with_parent("ele1",ns1,root);
	ele2 = axis2c_create_om_element("ele2",ns1);

	axis2c_node_add_child(ele1,ele2);

	attr1 = axis2c_create_om_attribute("attribute1","x",ns1);

	axis2c_om_element_add_attribute(ele2,attr1);
	

	printf("%s",axis2c_om_element_get_localname(ele2->parent->parent));
    
    getchar();
    
    return 0;
}
