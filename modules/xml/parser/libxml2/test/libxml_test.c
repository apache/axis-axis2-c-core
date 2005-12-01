#include <axis2_xml_reader.h>
#include <axis2_env.h>
#include <axis2_defines.h>
#include <axis2_stream_default.h>
#include <axis2_log_default.h>
#include <axis2_error_default.h>
#include <axis2.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    axis2_env_t *env = NULL;
    axis2_allocator_t *allocator = NULL;
    axis2_xml_reader_t *parser = NULL;
    axis2_stream_t *stream = NULL;
    axis2_error_t *error = NULL;
    axis2_log_t *log     = NULL;
    int token = 0;
    char *p = NULL;
    int attr_count =0;
    int ns_count =0;
    int i = 0;
    char *filename ="namespaces.xml";
    if(argc > 1)
        filename = argv[1];
    
    allocator = axis2_allocator_init(NULL);
    log = axis2_log_create(allocator, NULL);
    error = axis2_error_create(allocator);
    stream = axis2_stream_create(allocator, NULL);
    env       = axis2_env_create_with_error_stream_log(allocator, error, stream, log);
    parser = axis2_xml_reader_create_for_file(&env,filename,NULL);
    if(!parser)
    {   printf("error creatin parser ");
        return;
    }
    printf("running test\n\n\n");
        
    do{
        token = AXIS2_XML_READER_NEXT(parser, &env);
        if(token == -1) break;
        
        switch(token)
        {
            case AXIS2_XML_READER_START_DOCUMENT:
            break;
    
            case AXIS2_XML_READER_SPACE:
            break;
            
            case AXIS2_XML_READER_START_ELEMENT:
                
	            printf ("\n<");
        	    p = AXIS2_XML_READER_GET_PREFIX (parser, &env);
                
        	    if (p)
	            {
            	    printf ("%s:", p);
                    AXIS2_XML_READER_XML_FREE(parser, &env, p);
                    p = NULL;
        	    }
                 
                p = AXIS2_XML_READER_GET_NAME(parser, &env);
	            printf ("%s", p);
                AXIS2_XML_READER_XML_FREE(parser, &env, p);
	    	    p = NULL;
                ns_count = AXIS2_XML_READER_GET_NAMESPACE_COUNT(parser, &env);                
               /* printf("  __ ns count %d ",ns_count); */
    	        for (i=1; i <= ns_count ; i++)
	            {
		            p = AXIS2_XML_READER_GET_NAMESPACE_PREFIX_BY_NUMBER (parser, &env, i);
        		    printf (" xmlns");
                    if(p)
        		    printf (":%s=\"", p);
                    AXIS2_XML_READER_XML_FREE(parser, &env, p);
                    p = NULL;
    		        p = AXIS2_XML_READER_GET_NAMESPACE_URI_BY_NUMBER (parser, &env, i);
	    	        printf ("%s\" ", p);
                    AXIS2_XML_READER_XML_FREE(parser, &env, p);
		        }     

                attr_count = AXIS2_XML_READER_GET_ATTRIBUTE_COUNT (parser, &env);
               /* printf("__ attr count %d ",attr_count); */
	            for (i = 1 ; i <= attr_count ; i++)
        	    { 
	        	    p = AXIS2_XML_READER_GET_ATTRIBUTE_PREFIX_BY_NUMBER (parser, &env, i);
        		    if (p)
		            {
        		        printf (" %s:", p);
                        AXIS2_XML_READER_XML_FREE(parser, &env, p);
		                p = NULL;
                        p = AXIS2_XML_READER_GET_ATTRIBUTE_NAME_BY_NUMBER (parser, &env, i );
        		        printf ("%s=\"", p);
                        AXIS2_XML_READER_XML_FREE(parser, &env, p);
                        p = NULL;
    		            p = AXIS2_XML_READER_GET_ATTRIBUTE_VALUE_BY_NUMBER (parser, &env, i);
            		    printf ("%s\"", p);
                        AXIS2_XML_READER_XML_FREE(parser, &env, p);
                        p = NULL;
		            }
        	        else
        		    {
		                p = AXIS2_XML_READER_GET_ATTRIBUTE_NAME_BY_NUMBER (parser, &env, i);
        		        printf (" %s=\"", p);
                        AXIS2_XML_READER_XML_FREE(parser, &env, p);
                        p = NULL;
		                p = AXIS2_XML_READER_GET_ATTRIBUTE_VALUE_BY_NUMBER (parser, &env, i);
        		        printf ("%s\"", p);
                        AXIS2_XML_READER_XML_FREE(parser, &env, p);
                        p = NULL;
        		    }
	            } 
            	    

        	printf (">");
            
            break;
           
            case AXIS2_XML_READER_END_ELEMENT:
                
           	    printf ("</");
                p = AXIS2_XML_READER_GET_PREFIX (parser, &env);
        	    if (p)
	            {
		            printf ("%s:", p);
                    AXIS2_XML_READER_XML_FREE(parser, &env, p);
	            }
	            
                p = AXIS2_XML_READER_GET_NAME (parser, &env);
	            printf ("%s", p);
	            printf (">\n");
                AXIS2_XML_READER_XML_FREE(parser, &env, p);
            break;
            
            case AXIS2_XML_READER_CHARACTER:
                p = AXIS2_XML_READER_GET_VALUE (parser, &env);
                if(p)
                {                    
                    printf("%s",p);
                    AXIS2_XML_READER_XML_FREE(parser, &env, p);    
                }                    
            break;
                
            case AXIS2_XML_READER_EMPTY_ELEMENT:
                 
                printf ("\n<");
        	    p = AXIS2_XML_READER_GET_PREFIX (parser, &env);
                
        	    if (p)
	            {
            	    printf ("%s:", p);
                    AXIS2_XML_READER_XML_FREE(parser, &env, p);
                    p = NULL;
        	    }
                 
                p = AXIS2_XML_READER_GET_NAME(parser, &env);
	            printf ("%s", p);
                AXIS2_XML_READER_XML_FREE(parser, &env, p);
	    	    p = NULL;
                printf(">");
                
            break;
                
            case AXIS2_XML_READER_COMMENT:
                
                p = AXIS2_XML_READER_GET_VALUE(parser, &env);
                printf ("<!-- %s -->", p);
                AXIS2_XML_READER_XML_FREE(parser, &env, p);
            break;
            
            case AXIS2_XML_READER_PROCESSING_INSTRUCTION:
                
                p = AXIS2_XML_READER_GET_PI_TARGET(parser, &env);
                printf ("<? %s ", p);
                AXIS2_XML_READER_XML_FREE(parser, &env, p);
                p = AXIS2_XML_READER_GET_PI_DATA(parser, &env);
                printf (" %s ?>", p);
                AXIS2_XML_READER_XML_FREE(parser, &env, p);
            break;
            
            case AXIS2_XML_READER_DOCUMENT_TYPE:
                p= AXIS2_XML_READER_GET_DTD(parser, &env);
                printf("<!DOCTYPE %s>",p);
                AXIS2_XML_READER_XML_FREE(parser, &env, p);
                break;
            
            default:
            break;
        }
        
        
    }while(1);
    
    
    AXIS2_XML_READER_FREE(parser, &env);
    axis2_env_free(env);
    printf("\n\n");
    getchar();
    return 0;
}
