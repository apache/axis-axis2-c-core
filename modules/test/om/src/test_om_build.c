#include <axis2_stax_ombuilder.h>
#include <axis2_om_document.h>
#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <axis2_om_text.h>
#include <apr.h>

int main()
{
    READER *red = NULL;
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

getchar();

}
/*
<?xml version="1.0" ?>
<builder xmlns="http://www.wso2.com/"  test="xml"><document>TEST</document><a></a><b></b></builder>

*/



