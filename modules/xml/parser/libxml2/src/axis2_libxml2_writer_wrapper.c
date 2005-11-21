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
 
#include <libxml/xmlwriter.h>
#include <axis2.h>
#include <axis2_env.h>
#include <axis2_xml_writer.h>
/*******************************************************************************/

#define ENCODING "ISO-8859-1"

#define AXIS2_LIBXML2_WRITER_MEMORY 1
#define AXIS2_LIBXML2_WRITER_FILE   2


/*********************** function prototypes ***********************************/
axis2_status_t AXIS2_CALL 
axis2_libxml2_writer_wrapper_free(axis2_xml_writer_t *writer,
                                  axis2_env_t **env);
        
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_start_element(
                                    axis2_xml_writer_t *writer,
                                    axis2_env_t **env,
                                    axis2_char_t *localname);
        
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_end_start_element(axis2_xml_writer_t *writer,
                                               axis2_env_t **env);
        
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_start_element_with_namespace(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname,
                                        axis2_char_t *namespace_uri);
        
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_start_element_with_namespace_prefix(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname,
                                        axis2_char_t *namespace_uri,
                                        axis2_char_t *prefix);       
                                    
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_empty_element(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname);
        
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_empty_element_with_namespace(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname,
                                        axis2_char_t *namespace_uri);

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_empty_element_with_namespace_prefix(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname,
                                        axis2_char_t *namespace_uri,
                                        axis2_char_t *prefix);

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_end_element(axis2_xml_writer_t *writer,
                                               axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_end_document(axis2_xml_writer_t *writer,
                                               axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_attribute(axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *localname,
                                           axis2_char_t *value);
                                               
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_attribute_with_namespace(
                                          axis2_xml_writer_t *writer,
                                          axis2_env_t **env,
                                          axis2_char_t *localname,
                                          axis2_char_t *value,
                                          axis2_char_t *namespace_uri);
                                          
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_attribute_with_namespace_prefix(
                                          axis2_xml_writer_t *writer,
                                          axis2_env_t **env,
                                          axis2_char_t *localname,
                                          axis2_char_t *value,
                                          axis2_char_t *namespace_uri,
                                          axis2_char_t *prefix);
                                          
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_namespace(axis2_xml_writer_t *writer,
                                             axis2_env_t **env,
                                             axis2_char_t *prefix,
                                             axis2_char_t *namespace_uri);   


axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_default_namespace
                                            (axis2_xml_writer_t *writer,
                                             axis2_env_t **env,
                                             axis2_char_t *namespace_uri);  

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_comment(axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *value);
                                           
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_processing_instruction(
                                           axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *target);
                                           
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_processing_instruction_data(
                                           axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *target,
                                           axis2_char_t *data);
                                           
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_cdata(axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *data);

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_dtd(axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *dtd); 

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_entity_ref(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *name); 

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_start_document(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env);  

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_start_document_with_version(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *version);   

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_start_document_with_version_encoding(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *encoding,
                                         axis2_char_t *version);
                                         
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_characters(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *text);   

axis2_char_t* AXIS2_CALL
axis2_libxml2_writer_wrapper_get_prefix(  axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *uri);

axis2_status_t AXIS2_CALL 
axis2_libxml2_writer_wrapper_set_prefix(  axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *prefix,
                                         axis2_char_t *uri);
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_set_default_prefix(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *uri);                                                 

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_encoded(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *text,
                                         int in_attr);



/**************************** end function pointers ****************************/


typedef struct axis2_libxml2_writer_wrapper_impl
{
    axis2_xml_writer_t writer;
    xmlTextWriterPtr xml_writer;
    xmlBufferPtr buffer;
    int writer_type;
    
}axis2_libxml2_writer_wrapper_impl_t;


/***************************** Macros ******************************************/

#define AXIS2_INTF_TO_IMPL(p) ((axis2_libxml2_writer_wrapper_impl_t*)p)

/****************************** End macro **************************************/

AXIS2_DECLARE(axis2_xml_writer_t *)
axis2_xml_writer_create(axis2_env_t **env,
                        void *filename,
                        axis2_char_t *encoding,
                        int is_prefix_default,
                        int compression)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl;
    int rc;
    AXIS2_ENV_CHECK(env, NULL);
    
    writer_impl = (axis2_libxml2_writer_wrapper_impl_t *)AXIS2_MALLOC((*env)->allocator,
                   sizeof(axis2_libxml2_writer_wrapper_impl_t));
    if(!writer_impl)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    
    writer_impl->xml_writer = xmlNewTextWriterFilename(filename, compression);
    
    if(!(writer_impl->xml_writer))
    {
        AXIS2_ERROR_SET((*env)->error,
                   AXIS2_ERROR_CREATING_XML_STREAM_WRITER, NULL);
        AXIS2_FREE((*env)->allocator, writer_impl);
                
    }
    writer_impl->writer.ops = NULL;
    writer_impl->writer.ops = (axis2_xml_writer_ops_t*)AXIS2_MALLOC((*env)->allocator,
                                    sizeof(axis2_xml_writer_ops_t));
    
    if(!(writer_impl->writer.ops))
    {
        AXIS2_FREE((*env)->allocator,writer_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    /* operations */
    writer_impl->writer.ops->free = axis2_libxml2_writer_wrapper_free;
    writer_impl->writer.ops->write_start_element = axis2_libxml2_writer_wrapper_write_start_element;
    writer_impl->writer.ops->write_start_element_with_namespace =
            axis2_libxml2_writer_wrapper_write_start_element_with_namespace;
    writer_impl->writer.ops->write_start_element_with_namespace_prefix = 
            axis2_libxml2_writer_wrapper_write_start_element_with_namespace_prefix;
    
    writer_impl->writer.ops->write_empty_element = axis2_libxml2_writer_wrapper_write_empty_element;
    writer_impl->writer.ops->write_empty_element_with_namespace  =
            axis2_libxml2_writer_wrapper_write_empty_element_with_namespace;
    writer_impl->writer.ops->write_empty_element_with_namespace_prefix =
            axis2_libxml2_writer_wrapper_write_empty_element_with_namespace_prefix;
    
    writer_impl->writer.ops->write_end_element = 
            axis2_libxml2_writer_wrapper_write_end_element;
    writer_impl->writer.ops->write_end_document = 
            axis2_libxml2_writer_wrapper_write_end_document;
    
    writer_impl->writer.ops->write_attribute = 
            axis2_libxml2_writer_wrapper_write_attribute;
    writer_impl->writer.ops->write_attribute_with_namespace =
            axis2_libxml2_writer_wrapper_write_attribute_with_namespace;
    writer_impl->writer.ops->write_attribute_with_namespace_prefix =
            axis2_libxml2_writer_wrapper_write_attribute_with_namespace_prefix;
    writer_impl->writer.ops->write_namespace = 
            axis2_libxml2_writer_wrapper_write_namespace;
    writer_impl->writer.ops->write_default_namespace =
            axis2_libxml2_writer_wrapper_write_default_namespace;
    writer_impl->writer.ops->write_comment = 
            axis2_libxml2_writer_wrapper_write_comment;
    writer_impl->writer.ops->write_processing_instruction = 
            axis2_libxml2_writer_wrapper_write_processing_instruction;
    writer_impl->writer.ops->write_processing_instruction_data = 
            axis2_libxml2_writer_wrapper_write_processing_instruction_data;
    writer_impl->writer.ops->write_cdata = 
            axis2_libxml2_writer_wrapper_write_cdata;
    writer_impl->writer.ops->write_dtd = 
            axis2_libxml2_writer_wrapper_write_dtd;
    writer_impl->writer.ops->write_entity_ref = 
            axis2_libxml2_writer_wrapper_write_entity_ref;
    writer_impl->writer.ops->write_start_document = 
            axis2_libxml2_writer_wrapper_write_start_document;
    writer_impl->writer.ops->write_start_document_with_version = 
            axis2_libxml2_writer_wrapper_write_start_document_with_version;
    writer_impl->writer.ops->write_start_document_with_version_encoding = 
            axis2_libxml2_writer_wrapper_write_start_document_with_version_encoding;
    writer_impl->writer.ops->write_characters = 
            axis2_libxml2_writer_wrapper_write_characters;
    writer_impl->writer.ops->get_prefix =
            axis2_libxml2_writer_wrapper_get_prefix;
    writer_impl->writer.ops->set_prefix = axis2_libxml2_writer_wrapper_set_prefix;
    writer_impl->writer.ops->set_default_prefix = 
            axis2_libxml2_writer_wrapper_set_default_prefix;
    writer_impl->writer.ops->write_encoded = 
            axis2_libxml2_writer_wrapper_write_encoded;    
    return &(writer_impl->writer);
}

/*********************** writer create func for file ***************************/

AXIS2_DECLARE(axis2_xml_writer_t *)
axis2_xml_writer_create_for_memory(axis2_env_t **env,
                                   void *buffer,
                                   axis2_char_t *encoding, 
                                   int is_prefix_default,
                                   int compression)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl;
    AXIS2_ENV_CHECK(env, NULL);
    
    writer_impl = (axis2_libxml2_writer_wrapper_impl_t *)AXIS2_MALLOC((*env)->allocator,
                   sizeof(axis2_libxml2_writer_wrapper_impl_t));
    if(!writer_impl)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    
    
    
    writer_impl->writer.ops = NULL;
    writer_impl->writer.ops = (axis2_xml_writer_ops_t*)AXIS2_MALLOC((*env)->allocator,
                                    sizeof(axis2_xml_writer_ops_t));
    if(!(writer_impl->writer.ops))
    {
        AXIS2_FREE((*env)->allocator,writer_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    /* operations */
    writer_impl->writer.ops->free = axis2_libxml2_writer_wrapper_free;
    writer_impl->writer.ops->write_start_element = axis2_libxml2_writer_wrapper_write_start_element;
    writer_impl->writer.ops->write_start_element_with_namespace =
            axis2_libxml2_writer_wrapper_write_start_element_with_namespace;
    writer_impl->writer.ops->write_start_element_with_namespace_prefix = 
            axis2_libxml2_writer_wrapper_write_start_element_with_namespace_prefix;
    
    writer_impl->writer.ops->write_empty_element = axis2_libxml2_writer_wrapper_write_empty_element;
    writer_impl->writer.ops->write_empty_element_with_namespace  =
            axis2_libxml2_writer_wrapper_write_empty_element_with_namespace;
    writer_impl->writer.ops->write_empty_element_with_namespace_prefix =
            axis2_libxml2_writer_wrapper_write_empty_element_with_namespace_prefix;
    
    writer_impl->writer.ops->write_end_element = 
            axis2_libxml2_writer_wrapper_write_end_element;
    writer_impl->writer.ops->write_end_document = 
            axis2_libxml2_writer_wrapper_write_end_document;
    
    writer_impl->writer.ops->write_attribute = 
            axis2_libxml2_writer_wrapper_write_attribute;
    writer_impl->writer.ops->write_attribute_with_namespace =
            axis2_libxml2_writer_wrapper_write_attribute_with_namespace;
    writer_impl->writer.ops->write_attribute_with_namespace_prefix =
            axis2_libxml2_writer_wrapper_write_attribute_with_namespace_prefix;
    writer_impl->writer.ops->write_namespace = 
            axis2_libxml2_writer_wrapper_write_namespace;
    writer_impl->writer.ops->write_default_namespace =
            axis2_libxml2_writer_wrapper_write_default_namespace;
    writer_impl->writer.ops->write_comment = 
            axis2_libxml2_writer_wrapper_write_comment;
    writer_impl->writer.ops->write_processing_instruction = 
            axis2_libxml2_writer_wrapper_write_processing_instruction;
    writer_impl->writer.ops->write_processing_instruction_data = 
            axis2_libxml2_writer_wrapper_write_processing_instruction_data;
    writer_impl->writer.ops->write_cdata = 
            axis2_libxml2_writer_wrapper_write_cdata;
    writer_impl->writer.ops->write_dtd = 
            axis2_libxml2_writer_wrapper_write_dtd;
    writer_impl->writer.ops->write_entity_ref = 
            axis2_libxml2_writer_wrapper_write_entity_ref;
    writer_impl->writer.ops->write_start_document = 
            axis2_libxml2_writer_wrapper_write_start_document;
    writer_impl->writer.ops->write_start_document_with_version = 
            axis2_libxml2_writer_wrapper_write_start_document_with_version;
    writer_impl->writer.ops->write_start_document_with_version_encoding = 
            axis2_libxml2_writer_wrapper_write_start_document_with_version_encoding;
    writer_impl->writer.ops->write_characters = 
            axis2_libxml2_writer_wrapper_write_characters;
    writer_impl->writer.ops->get_prefix =
            axis2_libxml2_writer_wrapper_get_prefix;
    writer_impl->writer.ops->set_prefix = axis2_libxml2_writer_wrapper_set_prefix;
    writer_impl->writer.ops->set_default_prefix = 
            axis2_libxml2_writer_wrapper_set_default_prefix;
    writer_impl->writer.ops->write_encoded = 
            axis2_libxml2_writer_wrapper_write_encoded;    
    return &(writer_impl->writer);
}

/*******************************************************************************/
axis2_status_t AXIS2_CALL 
axis2_libxml2_writer_wrapper_free(axis2_xml_writer_t *writer,
                                  axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
  
    if(writer->ops)
        AXIS2_FREE((*env)->allocator, writer->ops);
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(writer));
    return AXIS2_SUCCESS;
}
        
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_start_element(
                                    axis2_xml_writer_t *writer,
                                    axis2_env_t **env,
                                    axis2_char_t *localname)
{
    int status = AXIS2_SUCCESS;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    return status;
}

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_end_start_element(axis2_xml_writer_t *writer,
                                               axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
   }
        
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_start_element_with_namespace(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname,
                                        axis2_char_t *namespace_uri)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error,namespace_uri, AXIS2_FAILURE);
   }
        
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_start_element_with_namespace_prefix(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname,
                                        axis2_char_t *namespace_uri,
                                        axis2_char_t *prefix)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname,  AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespace_uri, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, prefix, AXIS2_FAILURE);
       
}
                                    
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_empty_element(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    
    
}
        
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_empty_element_with_namespace(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname,
                                        axis2_char_t *namespace_uri)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespace_uri, AXIS2_FAILURE);
   }

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_empty_element_with_namespace_prefix(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname,
                                        axis2_char_t *namespace_uri,
                                        axis2_char_t *prefix)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespace_uri, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, prefix, AXIS2_FAILURE);
    
}

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_end_element(axis2_xml_writer_t *writer,
                                               axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
}

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_end_document(axis2_xml_writer_t *writer,
                                               axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
  
}

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_attribute(axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *localname,
                                           axis2_char_t *value)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
}
                                               
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_attribute_with_namespace(
                                          axis2_xml_writer_t *writer,
                                          axis2_env_t **env,
                                          axis2_char_t *localname,
                                          axis2_char_t *value,
                                          axis2_char_t *namespace_uri)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname,  AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespace_uri, AXIS2_FAILURE);
    
}
                                          
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_attribute_with_namespace_prefix(
                                          axis2_xml_writer_t *writer,
                                          axis2_env_t **env,
                                          axis2_char_t *localname,
                                          axis2_char_t *value,
                                          axis2_char_t *namespace_uri,
                                          axis2_char_t *prefix)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespace_uri, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, prefix , AXIS2_FAILURE);
}
                                          
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_namespace(axis2_xml_writer_t *writer,
                                             axis2_env_t **env,
                                             axis2_char_t *prefix,
                                             axis2_char_t *namespace_uri)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespace_uri, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, prefix , AXIS2_FAILURE);
}


axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_default_namespace
                                            (axis2_xml_writer_t *writer,
                                             axis2_env_t **env,
                                             axis2_char_t *namespace_uri)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespace_uri, AXIS2_FAILURE);
}

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_comment(axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *value)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
}

                                           
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_processing_instruction(
                                           axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *target)
{   
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, target, AXIS2_FAILURE);
}    
                                           
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_processing_instruction_data(
                                           axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *target,
                                           axis2_char_t *data)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, target, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, data, AXIS2_FAILURE);
}
                                           
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_cdata(axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *data)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, data, AXIS2_FAILURE);
}    

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_dtd(axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *dtd)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, dtd, AXIS2_FAILURE);
}

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_entity_ref(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *name)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, name, AXIS2_FAILURE);
}    

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_start_document(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env)
{
     AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
}    

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_start_document_with_version(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *version)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, version, AXIS2_FAILURE); 
}    

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_start_document_with_version_encoding(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *encoding,
                                         axis2_char_t *version)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, encoding, AXIS2_FAILURE); 
    AXIS2_PARAM_CHECK((*env)->error, version, AXIS2_FAILURE); 
    
}

                                         
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_characters(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *text)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, text, AXIS2_FAILURE); 
}    

axis2_char_t* AXIS2_CALL
axis2_libxml2_writer_wrapper_get_prefix(  axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *uri)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, uri, NULL);
}

axis2_status_t AXIS2_CALL 
axis2_libxml2_writer_wrapper_set_prefix( axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *prefix,
                                         axis2_char_t *uri)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, prefix, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, uri, AXIS2_FAILURE);
}
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_set_default_prefix(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *uri)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, uri, AXIS2_FAILURE);
}    

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_encoded(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *text,
                                         int in_attr)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, text, AXIS2_FAILURE);
}
