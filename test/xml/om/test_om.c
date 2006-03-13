/*
 *   Copyright 2004,2005 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <axis2_om_stax_builder.h>
#include <axis2_om_document.h>
#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <axis2_om_text.h>
#include <axis2_stream.h>
#include <axis2_log_default.h>
#include <axis2_error_default.h>
#include <axis2_xml_reader.h>
#include <stdio.h>
#include <axis2_xml_writer.h>
#include <axis2_env.h>
/** 
   Define the environment related variables globaly so that they are available 
   for both functions 
*/

axis2_allocator_t *allocator = NULL;
axis2_env_t *environment = NULL;
axis2_stream_t *stream = NULL;
axis2_error_t *error = NULL;
axis2_log_t *axis_log     = NULL;
FILE *f = NULL;
/** a method that demonstrate creating a om model using an xml file */


int read_input(char *buffer,int size,void* ctx)
{
   return fread(buffer, sizeof(char),size,f);
}

int
test_om_build (char *filename)
{
    axis2_om_element_t *ele1 = NULL, *ele2 = NULL;
    axis2_om_stax_builder_t *builder = NULL;
    axis2_om_text_t *text = NULL;
    axis2_om_document_t *document = NULL;
    axis2_om_node_t *node1 = NULL, *node2 = NULL;
    axis2_om_output_t *om_output = NULL;
    axis2_om_namespace_t* ns = NULL;
    axis2_xml_reader_t *reader = NULL;
    axis2_xml_writer_t *writer = NULL;
    char *buffer = NULL;
    f =fopen(filename, "r");
    if(!f)
        return -1;
      
    /** create pull parser */
     reader = axis2_xml_reader_create_for_memory(&environment, read_input,NULL ,NULL, NULL);
    
    if(!reader)
    {
        printf("ERROR CREATING PULLPARSER");
        return -1;
    }
    /** create axis2_om_stax_builder by parsing pull_parser struct */
    
    builder = axis2_om_stax_builder_create (&environment,reader);
    
    if(!builder)
    {
        printf("ERROR CREATING PULL PARSER");
        return -1;
    }
    /** 
        create an om document
        document is the container of om model created using builder 
    */
           
    document = axis2_om_document_create (&environment, NULL, builder);
    /**
        get root element , building starts hear 
     */
    
    node1 = AXIS2_OM_DOCUMENT_GET_ROOT_ELEMENT (document,&environment);
    if(!node1)
    {
        printf(" root element null ");
        AXIS2_OM_STAX_BUILDER_FREE(builder, &environment);
        return;
    }
    if(node1)
    {
        /** print root node information */
        
       
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
    /** build the document continuously untill all the xml file is built in to a om model */
    
    
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
            {
                printf("\n localname %s\n" , AXIS2_OM_ELEMENT_GET_LOCALNAME(ele2,&environment)); 
            }
                        
            break;
        case AXIS2_OM_TEXT:
            
            text = (axis2_om_text_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(node2,&environment);    
            if( text && AXIS2_OM_TEXT_GET_VALUE(text ,&environment))
                printf("\n text value  %s \n", AXIS2_OM_TEXT_GET_VALUE(text,&environment));
            break;

        default:
            break;
        }

        node2 = AXIS2_OM_DOCUMENT_BUILD_NEXT (document , &environment);
    }
    while (node2);
    printf ("END: pull document\n");

    printf ("Serialize pulled document\n");
    
    writer = axis2_xml_writer_create_for_memory(&environment, NULL, AXIS2_TRUE, 0);
    om_output = axis2_om_output_create (&environment, writer);

    AXIS2_OM_NODE_SERIALIZE (node1, &environment , om_output);
    
    buffer = AXIS2_XML_WRITER_GET_XML(writer, &environment);
    
    AXIS2_OM_OUTPUT_FREE(om_output, &environment);  
    if(buffer)
        printf("%s",buffer);
  
    AXIS2_OM_STAX_BUILDER_FREE(builder, &environment);
    if(buffer)
        AXIS2_FREE(environment->allocator, buffer); 
    printf ("\ndone\n");
    fclose(f);
    return 0;
}



int
test_om_serialize ()
{

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
    int status;
    axis2_om_element_t *ele1 = NULL, *ele2 = NULL, *ele3 = NULL, *ele4 =
        NULL;
    axis2_om_node_t *node1 = NULL, *node2 = NULL, *node3 = NULL, *node4 =
        NULL, *node5 = NULL, *node6 = NULL;
    axis2_om_attribute_t *attr1 = NULL, *attr2 = NULL;
    axis2_om_namespace_t *ns1 = NULL, *ns2 = NULL, *ns3 = NULL;
    axis2_om_text_t *text1 = NULL;
    axis2_om_output_t *om_output = NULL;
    axis2_xml_writer_t *writer = NULL;
    axis2_char_t *output_buffer = NULL;

    ns1 =
        axis2_om_namespace_create (&environment,
                                   "http://ws.apache.org/axis2/c/om",
                                   "axiom");
    ns2 =
        axis2_om_namespace_create (&environment, "urn:ISBN:0-395-74341-6",
                                   "isbn");
    ele1 = axis2_om_element_create (&environment, NULL, "book", ns1, &node1);
    AXIS2_OM_ELEMENT_DECLARE_NAMESPACE(ele1,&environment,node1,ns2);
    
    ele2 = axis2_om_element_create (&environment, node1, "title", ns1, &node2);
    
    text1 = axis2_om_text_create (&environment, node2, "Axis2/C OM HOWTO", &node3);
    
    ele3 = axis2_om_element_create (&environment, node1, "number", ns2, &node4);
    
    text1 = axis2_om_text_create (&environment, node4, "1748491379", &node5);
    
    ele4 = axis2_om_element_create (&environment, node1, "author", ns1, &node6);
    
    attr1 = axis2_om_attribute_create (&environment, "title", "Mr", ns1);
    
    AXIS2_OM_ELEMENT_ADD_ATTRIBUTE(ele4, &environment,attr1, node6);
    
    attr2 = axis2_om_attribute_create (&environment, "name", "Axitoc Oman", ns1);
    
    AXIS2_OM_ELEMENT_ADD_ATTRIBUTE(ele4,&environment, attr2, node6);
    
   
    /* serializing stuff */
    writer = axis2_xml_writer_create_for_memory(&environment, NULL, AXIS2_TRUE, 0);
    om_output = axis2_om_output_create (&environment, writer);

    printf ("Serialize built document\n");
    status = AXIS2_OM_NODE_SERIALIZE (node1,&environment ,om_output);
    if (status != AXIS2_SUCCESS)
    {
        printf ("\naxis2_om_node_serialize failed\n");
        return status;
    }
    else
        printf ("\naxis2_om_node_serialize success\n");
    /* end serializing stuff */

     AXIS2_OM_NODE_FREE_TREE(node1,&environment);
     output_buffer = AXIS2_XML_WRITER_GET_XML(writer, &environment);
     
     AXIS2_OM_OUTPUT_FREE(om_output, &environment);
     if(output_buffer)
     {
        printf("%s",output_buffer);
        AXIS2_FREE(environment->allocator, output_buffer); 
     }        
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
    axis_log = axis2_log_create(allocator, NULL, NULL);
    error = axis2_error_create(allocator);
    
    environment = axis2_env_create_with_error_log(allocator, error,  axis_log);
    test_om_build (file_name);
    test_om_serialize(); 

    axis2_env_free(environment); 
    axis2_allocator_free(allocator);
    return 0;
}


