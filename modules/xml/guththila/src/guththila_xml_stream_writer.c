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
 *
 */
 
#include "guththila_xml_stream_writer.h"

#include <apr_strings.h>
  
  guththila_xml_stream_writer_t* guththila_create_xml_stream_writer(FILE* writer, char* encoding, int prefix_defaulting)
{
    if (!writer)
        return 0;
    
    guththila_xml_stream_writer_t *stream_writer = (guththila_xml_stream_writer_t*) malloc (sizeof (guththila_xml_stream_writer_t));
    stream_writer->writer = writer;
    if (encoding)
        stream_writer->encoding = encoding;
    stream_writer->prefix_defaulting = prefix_defaulting;

      //initialize APR pool, we ned this pool to allocate memory to hash map and stack

    apr_status_t status;

    if (!guththila_apr_pool)
    {
        status = apr_pool_create(&guththila_apr_pool, NULL);
    }

    stream_writer->element_stack = apr_array_make(guththila_apr_pool, 64, sizeof(char**));
    stream_writer->namespace_hash = apr_hash_make(guththila_apr_pool);
    stream_writer->prefix_hash = apr_hash_make(guththila_apr_pool);
    stream_writer->in_start_element = 0;
    stream_writer->empty_element = 0;

    return stream_writer;
}

  int guththila_xml_stream_writer_end_start_element(guththila_xml_stream_writer_t* stream_writer)
{
    if (!stream_writer)
        return GUTHTHILA_STREAM_WRITER_ERROR_STREAM_STRUCT_NULL;
    if (!stream_writer->in_start_element)
        return GUTHTHILA_STREAM_WRITER_ERROR_NOT_IN_START_ELEMENT;
    if (stream_writer->empty_element)
    {
        if (fputs("/", stream_writer->writer) == EOF)
        {
            return GUTHTHILA_STREAM_WRITER_ERROR_WRITING_TO_STREAM;
        }

          //pop last element and delete that elelemt
        char*** del_element = (char***) apr_array_pop( stream_writer->element_stack );
        if (del_element)
        {
            if ((*del_element)[0])
                free((*del_element)[0]);
            if ((*del_element)[1])
                free((*del_element)[1]);
            free(del_element);
        }
        del_element = 0;
        
          //end pop element
          

        stream_writer->empty_element = 0;
    }
    fputs(">", stream_writer->writer);
    stream_writer->in_start_element = 0;

    return GUTHTHILA_SUCCESS;
}

  int guththila_xml_stream_writer_write_start_element(guththila_xml_stream_writer_t* stream_writer, char* local_name)
{
    if (!stream_writer)
        return GUTHTHILA_STREAM_WRITER_ERROR_STREAM_STRUCT_NULL;
    if (!local_name)
        return GUTHTHILA_STREAM_WRITER_ERROR_LOCAL_NAME_NULL;
    guththila_xml_stream_writer_end_start_element(stream_writer);
          //namespaces.pushContext();
    fputs("<", stream_writer->writer);
    fputs(local_name, stream_writer->writer);

          //push element to stack
    char*** new_element = (char***)apr_array_push(stream_writer->element_stack);
    *new_element = (char**) malloc( sizeof(char**) * 2 );
    (*new_element)[0] = 0;
    (*new_element)[1] = strdup(local_name);
          //end push element

    stream_writer->in_start_element = 1;

    return GUTHTHILA_SUCCESS;
}

  
  int guththila_xml_stream_writer_write_start_element_with_namespace(guththila_xml_stream_writer_t* stream_writer, char* local_name, char* namespace_uri)
{
    if (!stream_writer)
        return GUTHTHILA_STREAM_WRITER_ERROR_STREAM_STRUCT_NULL;
    if (!local_name)
        return GUTHTHILA_STREAM_WRITER_ERROR_LOCAL_NAME_NULL;
    if (!namespace_uri)
        return GUTHTHILA_STREAM_WRITER_ERROR_guththila_namespace_t_NULL;
      
    guththila_xml_stream_writer_end_start_element(stream_writer);
        
    char* prefix = guththila_xml_stream_writer_get_prefix(stream_writer, namespace_uri);

    int is_declared = (prefix != 0);
      
    if (!is_declared)
    {
        if (stream_writer->prefix_defaulting)
            prefix = GUTHTHILA_DEFAULT_NS_PREFIX;
        else
            return GUTHTHILA_STREAM_WRITER_ERROR_guththila_namespace_t_NOT_DECLARED;
    }
      
    fputs("<", stream_writer->writer);
        
    if (prefix && strlen(prefix) != 0)
    {
        fputs(prefix, stream_writer->writer);
        fputs(":", stream_writer->writer);
    }

    fputs(local_name, stream_writer->writer);

    if (stream_writer->prefix_defaulting && !is_declared)
    {
        if (prefix)
            guththila_xml_stream_writer_write_namespace(stream_writer, prefix, namespace_uri);
    }
        //push element to stack
    //char** new_element = (char**)apr_array_push(stream_writer->element_stack);
    char*** new_element = (char***)apr_array_push(stream_writer->element_stack);
    *new_element = (char**) malloc( sizeof(char**) * 2 );
    if(prefix)
        (*new_element)[0] = strdup(prefix);
    else
        (*new_element)[0] = 0;
    (*new_element)[1] = strdup(local_name);
          //end push element
      
    stream_writer->in_start_element = 1;
      
    return GUTHTHILA_SUCCESS;
}

  int guththila_xml_stream_writer_write_start_element_with_namespace_prefix(guththila_xml_stream_writer_t* stream_writer, char* local_name, char* namespace_uri, char* prefix)
{
    if (!stream_writer)
        return GUTHTHILA_STREAM_WRITER_ERROR_STREAM_STRUCT_NULL;
    if (!local_name)
        return GUTHTHILA_STREAM_WRITER_ERROR_LOCAL_NAME_NULL;
    if (!namespace_uri)
        return GUTHTHILA_STREAM_WRITER_ERROR_guththila_namespace_t_NULL;
    if (!prefix)
        return GUTHTHILA_STREAM_WRITER_ERROR_PREFIX_NULL;
    
        guththila_xml_stream_writer_end_start_element(stream_writer);

        char* current_prefix = guththila_xml_stream_writer_get_prefix(stream_writer, namespace_uri);
        int is_current = 0;
        if(current_prefix)
            is_current = ( strcmp(prefix, current_prefix) != 0 );
        fputs("<", stream_writer->writer);

        if (prefix && strlen(prefix) != 0)
        {
            fputs(prefix, stream_writer->writer);
            fputs(":", stream_writer->writer);
        }
        
        fputs(local_name, stream_writer->writer);

        if (stream_writer->prefix_defaulting && !is_current)
        {
            if (prefix)
                guththila_xml_stream_writer_write_namespace(stream_writer, prefix, namespace_uri);
        }

        //push element to stack
        //char** new_element = (char**)apr_array_push(stream_writer->element_stack);
    char*** new_element = (char***)apr_array_push(stream_writer->element_stack);
        
        *new_element = (char**) malloc( sizeof(char**) * 2 );
        if(prefix)
            (*new_element)[0] = strdup(prefix);
        else
            (*new_element)[0] = 0;
        (*new_element)[1] = strdup(local_name);
          //end push element

        stream_writer->in_start_element = 1;

        return GUTHTHILA_SUCCESS;
    }


    int guththila_xml_stream_writer_write_empty_element(guththila_xml_stream_writer_t* stream_writer, char* local_name)
    {
        int ret = guththila_xml_stream_writer_write_start_element(stream_writer,  local_name);
        
        if (ret != GUTHTHILA_SUCCESS)
            return ret;
        
        stream_writer->empty_element = 1;
        
        return GUTHTHILA_SUCCESS;
    }

    int guththila_xml_stream_writer_write_empty_element_with_namespace(guththila_xml_stream_writer_t* stream_writer, char* local_name, char* namespace_uri)
    {
        int ret = guththila_xml_stream_writer_write_start_element_with_namespace(stream_writer,  local_name, namespace_uri);
        
        if (ret != GUTHTHILA_SUCCESS)
            return ret;
        
        stream_writer->empty_element = 1;
        
        return GUTHTHILA_SUCCESS;
    }

    int guththila_xml_stream_writer_write_empty_element_with_namespace_prefix(guththila_xml_stream_writer_t* stream_writer, char* local_name, char* namespace_uri, char* prefix)
    {
        int ret = guththila_xml_stream_writer_write_start_element_with_namespace_prefix(stream_writer,  local_name, namespace_uri, prefix);
        
        if (ret != GUTHTHILA_SUCCESS)
            return ret;
        
        stream_writer->empty_element = 1;
        
        return GUTHTHILA_SUCCESS;
    }

    int guththila_xml_stream_writer_write_end_element(guththila_xml_stream_writer_t* stream_writer)
    {
        guththila_xml_stream_writer_end_start_element(stream_writer);
    
            // pop element stack
        char*** element_data = (char***) apr_array_pop(stream_writer->element_stack);
        if (!element_data )
            return GUTHTHILA_STREAM_WRITER_ERROR_ELEMENT_STACK_EMPTY;

            //end pop element stack

        fputs("</", stream_writer->writer);

        if ((*element_data)[0] && strlen((*element_data)[0]) > 0 )
        {
            fputs((*element_data)[0], stream_writer->writer);
            fputs(":", stream_writer->writer);
            free( (*element_data)[0]);
        }

        if((*element_data)[1] && strlen((*element_data)[1]) > 0)
        {
            fputs((*element_data)[1], stream_writer->writer);
            free( (*element_data)[1]);
        }
        else
            return GUTHTHILA_STREAM_WRITER_ERROR_ELEMENT_STACK_EMPTY;
        
        fputs(">", stream_writer->writer);
       
        *element_data = 0;

        return GUTHTHILA_SUCCESS;
    }


    int guththila_xml_stream_writer_write_end_document(guththila_xml_stream_writer_t* stream_writer)

    {
        while (!apr_is_empty_array(stream_writer->element_stack))
        {
            int ret = guththila_xml_stream_writer_write_end_element (stream_writer);
            if (ret != GUTHTHILA_SUCCESS)
                return ret;
        
        }
        return GUTHTHILA_SUCCESS;
    }


    int guththila_xml_stream_writer_write_attribute(guththila_xml_stream_writer_t* stream_writer, char* local_name, char* value)
    {
        if (!stream_writer->in_start_element)
            return GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_STATE;

        fputs(" ", stream_writer->writer);
        fputs(local_name, stream_writer->writer);
        fputs("=\"", stream_writer->writer);
        guththila_xml_stream_writer_write_encoded(stream_writer, value, 1);
        fputs("\"", stream_writer->writer);

        return GUTHTHILA_SUCCESS;
    }

    
    int guththila_xml_stream_writer_write_attribute_with_namespace(guththila_xml_stream_writer_t* stream_writer, char* local_name, char* value, char* namespace_uri)
    {
        if (!stream_writer->in_start_element)
            return GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_STATE;

        char* prefix = guththila_xml_stream_writer_get_prefix(stream_writer, namespace_uri);
        if (!prefix)
        {
            if (stream_writer->prefix_defaulting)
            {
                prefix = GUTHTHILA_DEFAULT_NS_PREFIX;
                guththila_xml_stream_writer_write_namespace(stream_writer, prefix, namespace_uri);
            }
            else
                return GUTHTHILA_STREAM_WRITER_ERROR_guththila_namespace_t_NOT_DECLARED;
        }

        fputs(" ", stream_writer->writer);
            
        if (prefix && strlen(prefix) > 0)
        {
            fputs(prefix, stream_writer->writer);
            fputs(":", stream_writer->writer);
        }
        fputs(local_name, stream_writer->writer);
        fputs("=\"", stream_writer->writer);
        guththila_xml_stream_writer_write_encoded(stream_writer, value, 1);
        fputs("\"", stream_writer->writer);

        return GUTHTHILA_SUCCESS;
    }

    int guththila_xml_stream_writer_write_attribute_with_namespace_prefix(guththila_xml_stream_writer_t* stream_writer, char* local_name, char* value, char* namespace_uri, char* prefix)
    {
        if (!stream_writer->in_start_element)
            return GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_STATE;
        
        char* current_prefix = guththila_xml_stream_writer_get_prefix(stream_writer, namespace_uri);
        if (!current_prefix)
        {
            if (stream_writer->prefix_defaulting)
                guththila_xml_stream_writer_write_namespace(stream_writer, prefix, namespace_uri);
            else
                return GUTHTHILA_STREAM_WRITER_ERROR_guththila_namespace_t_NOT_DECLARED;
        }
        else if (strcmp(current_prefix, prefix) != 0 )
            return GUTHTHILA_STREAM_WRITER_ERROR_guththila_namespace_t_NOT_DECLARED;

        fputs(" ", stream_writer->writer);

        if (prefix && strlen(prefix) > 0)
        {
            fputs(prefix, stream_writer->writer);
            fputs(":", stream_writer->writer);
        }
        fputs(local_name, stream_writer->writer);
        fputs("=\"", stream_writer->writer);
        guththila_xml_stream_writer_write_encoded(stream_writer, value, 1);
        fputs("\"", stream_writer->writer);

        return GUTHTHILA_SUCCESS;
    }

    int guththila_xml_stream_writer_write_namespace(guththila_xml_stream_writer_t* stream_writer, char* prefix, char* namespace_uri)
    {
        if (!stream_writer->in_start_element)
            return GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_STATE;
        
        if (!prefix)
            prefix = GUTHTHILA_DEFAULT_NS_PREFIX;

        char* declared_prefix = guththila_xml_stream_writer_get_prefix(stream_writer, namespace_uri);
        if (declared_prefix && strcmp(prefix, declared_prefix) == 0)
            return GUTHTHILA_SUCCESS;
        guththila_xml_stream_writer_set_prefix(stream_writer, prefix, namespace_uri);
        
        fputs(" ", stream_writer->writer);
        fputs("xmlns", stream_writer->writer);
        
        if (strcmp(GUTHTHILA_DEFAULT_NS_PREFIX, prefix) != 0)
        {
            fputs(":", stream_writer->writer);
            fputs(prefix, stream_writer->writer);
        }
        fputs("=\"", stream_writer->writer);
        fputs(namespace_uri, stream_writer->writer);
        fputs("\"", stream_writer->writer);
        return GUTHTHILA_SUCCESS;
    }

    int guththila_xml_stream_writer_write_default_namespace(guththila_xml_stream_writer_t* stream_writer, char* namespace_uri)
    {
        int ret = guththila_xml_stream_writer_write_namespace(stream_writer, GUTHTHILA_DEFAULT_NS_PREFIX, namespace_uri);
        return ret;
    }


    int guththila_xml_stream_writer_write_comment(guththila_xml_stream_writer_t* stream_writer, char* data)
    {
        guththila_xml_stream_writer_end_start_element(stream_writer);

        if (!data)
            return GUTHTHILA_STREAM_WRITER_ERROR_COMMENT_NULL;

        char* ptr = data;
        while (*ptr)
        {
            if (*ptr++ == '-')
                if (*ptr == '-')
                    return GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_COMMENT;
        }
        
        fputs("<!--", stream_writer->writer);

        fputs(data, stream_writer->writer);
        fputs("-->", stream_writer->writer);
        return GUTHTHILA_SUCCESS;
    }

   
    int guththila_xml_stream_writer_write_processing_instruction(guththila_xml_stream_writer_t* stream_writer, char* target)
    {
        int ret = guththila_xml_stream_writer_write_processing_instruction_data(stream_writer, target, 0);
        return ret;
    }

    int guththila_xml_stream_writer_write_processing_instruction_data(guththila_xml_stream_writer_t* stream_writer, char* target, char* data)
    {
        if (!target)
            return GUTHTHILA_STREAM_WRITER_ERROR_PROCESSING_INSTRUCTION_TARGET_NULL;
    
        guththila_xml_stream_writer_end_start_element(stream_writer);

        fputs("<?", stream_writer->writer);
        fputs(target, stream_writer->writer);
    
        if (data)
        {
            fputs(" ", stream_writer->writer);
            fputs(data, stream_writer->writer);
        }

        fputs("?>", stream_writer->writer);
    
        return GUTHTHILA_SUCCESS;
    }


    int guththila_xml_stream_writer_write_cdata(guththila_xml_stream_writer_t* stream_writer, char* data)
    {
        if (!data)
            return GUTHTHILA_STREAM_WRITER_ERROR_CDATA_NULL;
    
        guththila_xml_stream_writer_end_start_element(stream_writer);

        char* ptr = data;
        while (*ptr)
        {
            if (*ptr++ == ']')
                if (*ptr == ']')
                    return GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_CDATA;
        }

        fputs("<![CDATA[", stream_writer->writer);
        fputs(data, stream_writer->writer);
        fputs("]]>", stream_writer->writer);
    
        return GUTHTHILA_SUCCESS;
    }


    int guththila_xml_stream_writer_write_dtd(guththila_xml_stream_writer_t* stream_writer, char* dtd)
    {
        if (!dtd)
            return GUTHTHILA_STREAM_WRITER_ERROR_DTD_NULL;

        fputs( "<!DOCTYPE ", stream_writer->writer);
        fputs(dtd, stream_writer->writer);
        fputs(">", stream_writer->writer);
    
        return GUTHTHILA_SUCCESS;
    }


    int guththila_xml_stream_writer_write_entity_ref(guththila_xml_stream_writer_t* stream_writer, char* name)
    {
        if (!name)
            return GUTHTHILA_STREAM_WRITER_ERROR_ENTITY_REF_NULL;
    
        guththila_xml_stream_writer_end_start_element(stream_writer);

        fputs( "&", stream_writer->writer);
        fputs(name, stream_writer->writer);
        fputs(";", stream_writer->writer);
        
        return GUTHTHILA_SUCCESS;
    }


    int guththila_xml_stream_writer_write_start_document(guththila_xml_stream_writer_t* stream_writer)
    {
        int ret = guththila_xml_stream_writer_write_start_document_with_version_encoding(stream_writer, 0, 0);
        return ret;
    }


    int guththila_xml_stream_writer_write_start_document_with_version(guththila_xml_stream_writer_t* stream_writer, char* version)
    {
        int ret = guththila_xml_stream_writer_write_start_document_with_version_encoding(stream_writer, 0, version);
        return ret;
    
    }

   
    int guththila_xml_stream_writer_write_start_document_with_version_encoding(guththila_xml_stream_writer_t* stream_writer, char* encoding, char* version)
    {
        if (!version)
            version = "1.0";
    
        encoding = "UTF-8"; // The parameter is ignored
    
        if ( (strcmp("1.0", version) != 0) && (strcmp("1.1", version) != 0 ) )
            return GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_XML_VERSION;
    
        fputs( "<?xml version=\"", stream_writer->writer);
        fputs( version, stream_writer->writer);
        fputs( "\" encoding=\"", stream_writer->writer);
        fputs( encoding, stream_writer->writer);
        fputs( "\"?>", stream_writer->writer);
        fputs( "\r\n", stream_writer->writer);
    
        return GUTHTHILA_SUCCESS;
    }

    int guththila_xml_stream_writer_write_characters(guththila_xml_stream_writer_t* stream_writer, char* text)
    {
        if (!text)
            return GUTHTHILA_STREAM_WRITER_ERROR_TEXT_NULL;
    
        guththila_xml_stream_writer_end_start_element(stream_writer);

        if (text)
            guththila_xml_stream_writer_write_encoded(stream_writer, text, 0);

        return GUTHTHILA_SUCCESS;
    }

    char* guththila_xml_stream_writer_get_prefix(guththila_xml_stream_writer_t* stream_writer, char* uri)
    {
        if (!uri)
            return 0;
    
        char* prefix = apr_hash_get(stream_writer->namespace_hash, uri, APR_HASH_KEY_STRING );
        return prefix;
    }

    int guththila_xml_stream_writer_set_prefix(guththila_xml_stream_writer_t* stream_writer, char* prefix, char* uri)
    {
        if (!prefix)
            return GUTHTHILA_STREAM_WRITER_ERROR_PREFIX_NULL;

        if (!uri)
            return GUTHTHILA_STREAM_WRITER_ERROR_guththila_namespace_t_NULL;
    
        if ( strcmp(prefix, "xml") == 0 || strcmp (prefix, "xmlns") == 0)
            return GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_PREFIX;

        apr_hash_set(stream_writer->namespace_hash, uri, APR_HASH_KEY_STRING, prefix);
        apr_hash_set(stream_writer->prefix_hash, prefix, APR_HASH_KEY_STRING, uri); //over write another prefix
        return GUTHTHILA_SUCCESS;
    }


    int guththila_xml_stream_writer_set_default_prefix(guththila_xml_stream_writer_t* stream_writer, char* uri){
        return guththila_xml_stream_writer_set_prefix(stream_writer, GUTHTHILA_DEFAULT_NS_PREFIX, uri);

    }

    int guththila_xml_stream_writer_write_encoded(guththila_xml_stream_writer_t* stream_writer, char* text, int in_attr)
    {
        if (!text)
            return GUTHTHILA_STREAM_WRITER_ERROR_TEXT_NULL;
    
        char* start = text;
        char* ptr = text;
    
        for (ptr = text; *ptr; ptr++)
        {
            char c = *ptr;
            if (c == '<' || c == '>' || c == '&')
            {
                char* to_write = apr_pstrndup(guththila_apr_pool, start, (apr_size_t)(ptr - start));
                if (to_write)
                {
                    fputs( to_write, stream_writer->writer);

                    //free (to_write);
                }
                else
                    return GUTHTHILA_STREAM_WRITER_ERROR_OUT_OF_MEMORY;
            
                if (c == '<')
                    fputs( "&lt;", stream_writer->writer);
            
                else if (c == '>')
                    fputs( "&gt;", stream_writer->writer);
                else
                    fputs( "&amp;", stream_writer->writer);
            
                start = ptr + 1;
            }
            else if (in_attr && (c == '"' || c == '\''))
            {
                char* to_write = apr_pstrndup(guththila_apr_pool, start, (apr_size_t)(ptr - start));
                if (to_write)
                {
                    fputs( to_write, stream_writer->writer);

                    //free (to_write);
                }
                else
                    return GUTHTHILA_STREAM_WRITER_ERROR_OUT_OF_MEMORY;
            
            
            
                if (c == '"')
                    fputs( "&quot;", stream_writer->writer);
                else
                    fputs( "&apos;", stream_writer->writer);
                start = ptr + 1;

            }
        }
    
        if (ptr - start)
        {
            char* to_write = apr_pstrndup(guththila_apr_pool, start, (apr_size_t)(ptr - start));
            if (to_write)
            {
                fputs( to_write, stream_writer->writer);

                //free (to_write);
            }
            else
                return GUTHTHILA_STREAM_WRITER_ERROR_OUT_OF_MEMORY;
        }

        return GUTHTHILA_SUCCESS;
    }

