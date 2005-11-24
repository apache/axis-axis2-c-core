#include <guththila_environment.h>
#include <guththila_defines.h>
#include <guththila_xml_pull_parser.h>
#include <guththila_xml_stream_writer.h>
#include <stdio.h>
#include <stdlib.h>

FILE *f;

int read_input(char *buffer,int size)
{
   return fread(buffer,sizeof(char), size, f);
}



void close_input(void)
{
    fclose(f);
}



int main(int argc,char *argv[])
{
    guththila_allocator_t *allocator = NULL;
    guththila_environment_t *env = NULL;
    guththila_reader_t *reader = NULL;
    guththila_reader_t *reader_mem = NULL;
    guththila_xml_pull_parser_t *parser = NULL;
    guththila_attribute_t *a = NULL;
    guththila_element_t *e = NULL;
    guththila_xml_stream_writer_t *writer;
    int ia =0;
    int d =0;
    int ix =0;
    char *p =0;
    int token = 0;
    char *filename = "test.xml";

    if(argc > 1)
        filename = argv[1];
    
    f =fopen(filename,"r");
    if(!f)
        return;
    
    allocator = guththila_allocator_init(NULL);
    env       = guththila_environment_create(allocator,NULL,NULL);
  
    reader_mem = guththila_reader_create_for_memory(env, read_input);
    parser = guththila_xml_pull_parser_create(env,reader_mem);
    guththila_xml_pull_parser_read(env,parser);

    printf("running test");

do{
  token = guththila_xml_pull_parser_next(env,parser);
  if(token==-1) break;
    switch(token)
    {
        case GUTHTHILA_START_DOCUMENT:
            printf ("<?xml ");
	        ix = guththila_xml_pull_parser_get_attribute_count (env,parser);
	        for (; ix > 0; ix--)
	        {
		    	a = guththila_xml_pull_parser_get_attribute (env,parser);
		        p = guththila_xml_pull_parser_get_attribute_name (env,parser, a);
		        printf ("%s=\"", p);
                GUTHTHILA_FREE(env->allocator, p);
		        p = guththila_xml_pull_parser_get_attribute_value (env,parser, a);
		        printf ("%s\" ", p);
		        GUTHTHILA_FREE (env->allocator,p);
	        }
	        printf ("?>");
            break;
    
        case GUTHTHILA_SPACE:
            break;
        
        case GUTHTHILA_START_ELEMENT:
            {
	             printf ("<");
        	     p = guththila_xml_pull_parser_get_prefix (env,parser);
        	     if (p)
	             {
            		printf ("%s:", p);
                    GUTHTHILA_FREE(env->allocator, p);
        	     }
                 
                 p = guththila_xml_pull_parser_get_name (env,parser);
	             printf ("%s", p);
                 GUTHTHILA_FREE(env->allocator, p);
	    	    
                 ia = guththila_xml_pull_parser_get_attribute_count (env,parser);
	             for ( ; ia > 0; ia--)
        	     { 
	        	    p = guththila_xml_pull_parser_get_attribute_namespace_by_number (env,parser, ia);
        		    if (p)
		            {
        		        printf (" %s:", p);
                        GUTHTHILA_FREE(env->allocator, p);
		                
                        p = guththila_xml_pull_parser_get_attribute_name_by_number (env,parser, ia);
        		        printf ("%s=\"", p);
                        GUTHTHILA_FREE(env->allocator, p);
                        
    		            p = guththila_xml_pull_parser_get_attribute_value_by_number (env,parser, ia);
            		    printf ("%s\"", p);
                        GUTHTHILA_FREE(env->allocator, p);
		            }
        	        else
        		    {
		                p = guththila_xml_pull_parser_get_attribute_name_by_number (env,parser, ia);
        		        printf (" %s=\"", p);
                        GUTHTHILA_FREE(env->allocator, p);
                        
		                p = guththila_xml_pull_parser_get_attribute_value_by_number (env,parser, ia);
        		        printf ("%s\"", p);
                        GUTHTHILA_FREE(env->allocator, p);
        		    }
	            } 
            	    e = guththila_stack_last (env,parser->dep);
	                d = e->depth->count;
	                    
    	        for (; d > 0; d--)
	            {
		            p = guththila_xml_pull_parser_get_namespace_prefix_by_number (env,parser, d);
        		    if (strncmp (p, "xmlns", 5))
		            printf (" xmlns:");
        		    printf ("%s=\"", p);
                    GUTHTHILA_FREE(env->allocator, p);
                    
    		        p = guththila_xml_pull_parser_get_namespace_uri_by_number (env,parser, d);
	    	        printf ("%s\" ", p);
		         }
        	    if (guththila_event == GUTHTHILA_START_ELEMENT)
	                  printf (">");
    	        else
	              printf ("/>");
        	    }
        	    break;
         
        case GUTHTHILA_END_ELEMENT:
           	    printf ("</");
                p = guththila_xml_pull_parser_get_prefix (env,parser);
        	    if (p)
	            {
		            printf ("%s:", p);
                    GUTHTHILA_FREE(env->allocator, p);
	            }
	            
                p = guththila_xml_pull_parser_get_name (env,parser);
	            printf ("%s", p);
	            printf (">");
                GUTHTHILA_FREE(env->allocator, p);
            break;
            
        case GUTHTHILA_CHARACTER:
                p = guththila_xml_pull_parser_get_value (env,parser);
                if(p)
                printf("%s",p);
                GUTHTHILA_FREE(env->allocator, p);
            break;
            
        case GUTHTHILA_EMPTY_ELEMENT:
             break;
         default:
         break;
    }
       
 
}while(1);

printf("\n");
guththila_xml_pull_parser_free(env,parser);
guththila_reader_free(env,reader_mem);



printf("\n\n\n\n");
writer = guththila_create_xml_stream_writer(env,NULL ,NULL,1);
guththila_xml_stream_writer_write_start_document_with_version(env,writer,"1.0");
guththila_xml_stream_writer_write_start_element(env,writer,"test");
guththila_xml_stream_writer_write_attribute(env,writer,"test","axis2c");
guththila_xml_stream_writer_write_comment(env,writer,"Hello");

guththila_xml_stream_writer_write_empty_element_with_namespace(env,writer,"empty_element","t");

printf("\n\n\n\n");
guththila_xml_stream_writer_free(env, writer);

guththila_environment_free(env);

getchar();
}
