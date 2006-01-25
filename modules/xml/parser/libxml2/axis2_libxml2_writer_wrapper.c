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
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_xml_writer.h>
#include <axis2_string.h>
#include <string.h>

/*******************************************************************************/

/** TODO namespace validation in element scope */
/******************************************************************************/
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

axis2_char_t* AXIS2_CALL
axis2_libxml2_writer_wrapper_get_xml(axis2_xml_writer_t *writer,
                                     axis2_env_t **env);

static axis2_bool_t 
axis2_libxml2_writer_wrapper_validate_namespace(axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *ns_uri,
                                         axis2_char_t *prefix);
static axis2_status_t 
axis2_libxml2_writer_wrapper_reset(axis2_xml_writer_t *writer,
                                   axis2_env_t **env);

                                   
                                                                            


/**************************** end function pointers ****************************/
typedef struct axis2_qname_array
{
    int allocated;
    int current_no;
    axis2_char_t **prefix;
    axis2_char_t **uri;
}axis2_qname_array_t;



typedef struct axis2_libxml2_writer_wrapper_impl
{
    axis2_xml_writer_t writer;
    xmlTextWriterPtr xml_writer;
    xmlBufferPtr buffer;
    int writer_type;
    axis2_char_t *encoding;
    int is_prefix_defaulting;
    int compression;
    axis2_qname_array_t qname_array;
    
}axis2_libxml2_writer_wrapper_impl_t;


/***************************** Macros ******************************************/

#define AXIS2_INTF_TO_IMPL(p) ((axis2_libxml2_writer_wrapper_impl_t*)p)

/****************************** End macro **************************************/


AXIS2_DECLARE(axis2_xml_writer_t *)
axis2_xml_writer_create(axis2_env_t **env,
                        axis2_char_t *filename,
                        axis2_char_t *encoding,
                        int is_prefix_default,
                        int compression)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    writer_impl = (axis2_libxml2_writer_wrapper_impl_t *)AXIS2_MALLOC((*env)->allocator,
                   sizeof(axis2_libxml2_writer_wrapper_impl_t));
    if(!writer_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    writer_impl->xml_writer = xmlNewTextWriterFilename(filename, compression);
    
    if(!(writer_impl->xml_writer))
    {
        AXIS2_FREE((*env)->allocator, writer_impl);
        AXIS2_ERROR_SET((*env)->error,
                   AXIS2_ERROR_CREATING_XML_STREAM_WRITER, AXIS2_FAILURE);
        return NULL;
                
    }
    writer_impl->buffer = NULL;
    writer_impl->encoding = NULL;
    
    
    if(encoding)
        writer_impl->encoding = AXIS2_STRDUP(encoding , env);
    else
        writer_impl->encoding = AXIS2_STRDUP(ENCODING, env);
     
    /********** allocate qname array 10 default size ******************************/
    writer_impl->qname_array.prefix = AXIS2_MALLOC((*env)->allocator,
                                        sizeof(axis2_char_t*)*10);
    writer_impl->qname_array.uri = AXIS2_MALLOC((*env)->allocator,
                                        sizeof(axis2_char_t*)*10);                                        
    if(!(writer_impl->qname_array.uri) || !(writer_impl->qname_array.prefix))
    {
        axis2_libxml2_writer_wrapper_free(&(writer_impl->writer), env);
    
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }                                       
    
    writer_impl->qname_array.allocated = 10;
    writer_impl->qname_array.current_no = 0;
    
      
    /*************************************************************************/  
    writer_impl->compression = compression;
    /* writer type */
    writer_impl->writer_type = AXIS2_LIBXML2_WRITER_FILE;   
    
    writer_impl->writer.ops = NULL;
    writer_impl->writer.ops = (axis2_xml_writer_ops_t*)AXIS2_MALLOC((*env)->allocator,
                                    sizeof(axis2_xml_writer_ops_t));
    
    if(!(writer_impl->writer.ops))
    {
        AXIS2_FREE((*env)->allocator,writer_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    /* ops */
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
    writer_impl->writer.ops->get_xml =
            axis2_libxml2_writer_wrapper_get_xml;            
    return &(writer_impl->writer);
}

/*********************** writer create func for file ***************************/

AXIS2_DECLARE(axis2_xml_writer_t *)
axis2_xml_writer_create_for_memory(axis2_env_t **env,
                                   axis2_char_t *encoding, 
                                   int is_prefix_default,
                                   int compression)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    writer_impl = (axis2_libxml2_writer_wrapper_impl_t *)AXIS2_MALLOC((*env)->allocator,
                   sizeof(axis2_libxml2_writer_wrapper_impl_t));
    if(!writer_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    writer_impl->writer.ops = NULL;
    writer_impl->encoding = NULL;
    writer_impl->buffer = NULL;
    writer_impl->qname_array.prefix = NULL;
    writer_impl->qname_array.uri = NULL;
    writer_impl->writer_type = AXIS2_LIBXML2_WRITER_MEMORY;
    
    
    writer_impl->buffer = xmlBufferCreate();
    if(writer_impl->buffer == NULL)
    {
        axis2_libxml2_writer_wrapper_free(&(writer_impl->writer), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    writer_impl->xml_writer = xmlNewTextWriterMemory(writer_impl->buffer, 0);
    if(writer_impl->xml_writer == NULL)
    {
        axis2_libxml2_writer_wrapper_free(&(writer_impl->writer), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_CREATING_XML_STREAM_WRITER , AXIS2_FAILURE);
        return NULL;
    }

     if(encoding)
        writer_impl->encoding = AXIS2_STRDUP(encoding , env);
    else
        writer_impl->encoding = AXIS2_STRDUP(ENCODING, env);
     
    /********** allocate qname array 10 default size ******************************/
    writer_impl->qname_array.prefix = AXIS2_MALLOC((*env)->allocator,
                                        sizeof(axis2_char_t*)*10);
    writer_impl->qname_array.uri = AXIS2_MALLOC((*env)->allocator,
                                        sizeof(axis2_char_t*)*10);                                        
    if(!(writer_impl->qname_array.uri) || !(writer_impl->qname_array.prefix))
    {
        axis2_libxml2_writer_wrapper_free(&(writer_impl->writer), env);
    
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }                                       
    
    writer_impl->qname_array.allocated = 10;
    writer_impl->qname_array.current_no = 0;
    /*************************************************************************/  
    
    writer_impl->compression = compression;
    
    
    writer_impl->writer.ops = (axis2_xml_writer_ops_t*)AXIS2_MALLOC((*env)->allocator,
                                    sizeof(axis2_xml_writer_ops_t));
    if(!(writer_impl->writer.ops))
    {
        axis2_libxml2_writer_wrapper_free(&(writer_impl->writer), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    /* ops */
    writer_impl->writer.ops->free = axis2_libxml2_writer_wrapper_free;
    writer_impl->writer.ops->write_start_element = 
            axis2_libxml2_writer_wrapper_write_start_element;
    writer_impl->writer.ops->write_start_element_with_namespace =
            axis2_libxml2_writer_wrapper_write_start_element_with_namespace;
    writer_impl->writer.ops->write_start_element_with_namespace_prefix = 
            axis2_libxml2_writer_wrapper_write_start_element_with_namespace_prefix;
    
    writer_impl->writer.ops->write_empty_element =
            axis2_libxml2_writer_wrapper_write_empty_element;
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
    writer_impl->writer.ops->get_xml =
            axis2_libxml2_writer_wrapper_get_xml;                 
    return &(writer_impl->writer);
}

/*******************************************************************************/
axis2_status_t AXIS2_CALL 
axis2_libxml2_writer_wrapper_free(axis2_xml_writer_t *writer,
                                  axis2_env_t **env)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    writer_impl = AXIS2_INTF_TO_IMPL(writer);

   
 
           
    if(writer_impl->encoding)
    {
        AXIS2_FREE((*env)->allocator, writer_impl->encoding);
        writer_impl->encoding = NULL;
    }
    if(writer_impl->qname_array.current_no >0)
        axis2_libxml2_writer_wrapper_reset(writer, env);
    
    AXIS2_FREE((*env)->allocator, writer_impl->qname_array.prefix);
    AXIS2_FREE((*env)->allocator, writer_impl->qname_array.uri);
    writer_impl->qname_array.prefix = NULL;
    writer_impl->qname_array.prefix = NULL;

    
    if(writer->ops)
    {
        AXIS2_FREE((*env)->allocator, writer->ops);
        writer->ops = NULL;
    }
    AXIS2_FREE((*env)->allocator, writer_impl);
    writer_impl = NULL;
    return AXIS2_SUCCESS;
}
        
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_start_element(
                                    axis2_xml_writer_t *writer,
                                    axis2_env_t **env,
                                    axis2_char_t *localname)
{
    int status = 0;
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    
    axis2_libxml2_writer_wrapper_reset(writer, env);
    
    status =  xmlTextWriterStartElement(writer_impl->xml_writer,(xmlChar *)localname);
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_WRITING_START_ELEMENT, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_end_start_element(axis2_xml_writer_t *writer,
                                               axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    /* nothing to do , 
       it is automatically taken care by the libxml2 writer */
    return AXIS2_SUCCESS;
}
        
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_start_element_with_namespace(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname,
                                        axis2_char_t *namespace_uri)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    int status = 0;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error,namespace_uri, AXIS2_FAILURE);
    
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    axis2_libxml2_writer_wrapper_reset(writer, env);

    axis2_libxml2_writer_wrapper_validate_namespace(writer, env, namespace_uri, NULL); 

    status = xmlTextWriterStartElementNS(writer_impl->xml_writer
                , NULL, BAD_CAST localname, BAD_CAST namespace_uri);
    
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error,
            AXIS2_ERROR_WRITING_START_ELEMENT_WITH_NAMESPACE, AXIS2_FAILURE);
        return AXIS2_FAILURE;            
    }   
    return AXIS2_SUCCESS;    
}
        
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_start_element_with_namespace_prefix(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname,
                                        axis2_char_t *namespace_uri,
                                        axis2_char_t *prefix)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    int status = 0;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname,  AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespace_uri, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, prefix, AXIS2_FAILURE);
    
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    axis2_libxml2_writer_wrapper_reset(writer, env);

    /**
        we intentionally write a null namespace_uri 
        to avoid namespace duplication since wrapper does not
        do namespace validation
    */

   /* axis2_libxml2_writer_wrapper_validate_namespace(writer, env, namespace_uri, prefix);
    */
    status = xmlTextWriterStartElementNS(writer_impl->xml_writer,
                                        BAD_CAST prefix,
                                        BAD_CAST localname, 
                                        BAD_CAST NULL);
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error,
            AXIS2_ERROR_WRITING_START_ELEMENT_WITH_NAMESPACE_PREFIX, AXIS2_FAILURE);
        return AXIS2_FAILURE;            
    }                                        
    return AXIS2_SUCCESS;       
}
                                    
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_empty_element(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname)
{
    int status = 0;
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    writer_impl = AXIS2_INTF_TO_IMPL(writer);

    axis2_libxml2_writer_wrapper_reset(writer, env);
    status =  xmlTextWriterStartElement(writer_impl->xml_writer,(xmlChar *)localname);

    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_WRITING_EMPTY_ELEMENT, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    status =  xmlTextWriterEndElement(writer_impl->xml_writer);
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_WRITING_EMPTY_ELEMENT, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}
        
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_empty_element_with_namespace(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname,
                                        axis2_char_t *namespace_uri)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    int status = 0;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error,namespace_uri, AXIS2_FAILURE);
    
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    axis2_libxml2_writer_wrapper_reset(writer, env);
    axis2_libxml2_writer_wrapper_validate_namespace(writer, env, namespace_uri, NULL);
    
    status = xmlTextWriterStartElementNS(writer_impl->xml_writer
                , NULL, BAD_CAST localname, BAD_CAST namespace_uri);

    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error,
            AXIS2_ERROR_WRITING_EMPTY_ELEMENT_WITH_NAMESPACE, AXIS2_FAILURE);
        return AXIS2_FAILURE;            
    }
    status =  xmlTextWriterEndElement(writer_impl->xml_writer);
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error,
            AXIS2_ERROR_WRITING_EMPTY_ELEMENT_WITH_NAMESPACE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
      
    return AXIS2_SUCCESS;    
}

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_empty_element_with_namespace_prefix(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname,
                                        axis2_char_t *namespace_uri,
                                        axis2_char_t *prefix)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    int status = 0;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname,  AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespace_uri, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, prefix, AXIS2_FAILURE);
    
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    axis2_libxml2_writer_wrapper_reset(writer, env);
    /**
        we intentionally write a null namespace uri to avoid namespace
        duplication 
    axis2_libxml2_writer_wrapper_validate_namespace(writer, env, namespace_uri, prefix);
    */
    status = xmlTextWriterStartElementNS(writer_impl->xml_writer,
                                        BAD_CAST prefix,
                                        BAD_CAST localname, 
                                        BAD_CAST NULL);
                                        
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error,
            AXIS2_ERROR_WRITING_START_ELEMENT_WITH_NAMESPACE_PREFIX, AXIS2_FAILURE);
        return AXIS2_FAILURE;            
    }                                        
    status =  xmlTextWriterEndElement(writer_impl->xml_writer);
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error,
            AXIS2_ERROR_WRITING_EMPTY_ELEMENT_WITH_NAMESPACE_PREFIX, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_end_element(axis2_xml_writer_t *writer,
                                               axis2_env_t **env)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    int status = 0;
    
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    status = xmlTextWriterFullEndElement(writer_impl->xml_writer);
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error,
                     AXIS2_ERROR_WRITING_END_ELEMENT,
                     AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;    
}

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_end_document(axis2_xml_writer_t *writer,
                                               axis2_env_t **env)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    int status = 0;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
  
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    status =  xmlTextWriterEndDocument(writer_impl->xml_writer);
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error,
                        AXIS2_ERROR_WRITING_END_DOCUMENT,
                        AXIS2_FAILURE);
                        
        return AXIS2_FAILURE;    
    }
    return AXIS2_SUCCESS;    
}

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_attribute(axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *localname,
                                           axis2_char_t *value)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    int status = 0;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
    
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    status = xmlTextWriterWriteAttribute(writer_impl->xml_writer,
                      BAD_CAST localname, BAD_CAST value);
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error,
                        AXIS2_ERROR_WRITING_ATTRIBUTE,  AXIS2_FAILURE);
                        
        return AXIS2_FAILURE;    
    }
    return AXIS2_SUCCESS;                     
}
                                               
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_attribute_with_namespace(
                                          axis2_xml_writer_t *writer,
                                          axis2_env_t **env,
                                          axis2_char_t *localname,
                                          axis2_char_t *value,
                                          axis2_char_t *namespace_uri)
{   
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    int status = 0;
    axis2_bool_t exists = AXIS2_FALSE;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname,  AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespace_uri, AXIS2_FAILURE);
    
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    
    exists = axis2_libxml2_writer_wrapper_validate_namespace(writer, env, namespace_uri, NULL);
    if(exists == AXIS2_TRUE)
        status = xmlTextWriterWriteAttributeNS(writer_impl->xml_writer,
                        NULL, BAD_CAST localname, BAD_CAST NULL,
                        BAD_CAST value);
    else                   
        status = xmlTextWriterWriteAttributeNS(writer_impl->xml_writer,
                        NULL, BAD_CAST localname, BAD_CAST namespace_uri,
                        BAD_CAST value);
                    
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error,
             AXIS2_ERROR_WRITING_ATTRIBUTE_WITH_NAMESPACE,  AXIS2_FAILURE);
                        
        return AXIS2_FAILURE;    
    }
    return AXIS2_SUCCESS;                   
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
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    int status = 0;
    
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname,  AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespace_uri, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, prefix, AXIS2_FAILURE);
        
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    /*
    we intentionally write null namespace uri to avoid namespace duplication
    
    exists = axis2_libxml2_writer_wrapper_validate_namespace(writer, env, namespace_uri, prefix);
    if(exists == AXIS2_TRUE)
            status = xmlTextWriterWriteAttributeNS(writer_impl->xml_writer,
                    BAD_CAST prefix , BAD_CAST localname, 
                        BAD_CAST NULL , BAD_CAST value);
    else                        
        status = xmlTextWriterWriteAttributeNS(writer_impl->xml_writer,
                    BAD_CAST prefix , BAD_CAST localname, 
                        BAD_CAST namespace_uri , BAD_CAST value);
     */
    status = xmlTextWriterWriteAttributeNS(writer_impl->xml_writer,
                    BAD_CAST prefix , BAD_CAST localname, 
                        BAD_CAST NULL , BAD_CAST value);
                  
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error,
             AXIS2_ERROR_WRITING_ATTRIBUTE_WITH_NAMESPACE_PREFIX,  AXIS2_FAILURE);
                        
        return AXIS2_FAILURE;    
    }
    return AXIS2_SUCCESS;                   
}

/** need to work on this */                                         
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_namespace(axis2_xml_writer_t *writer,
                                             axis2_env_t **env,
                                             axis2_char_t *prefix,
                                             axis2_char_t *namespace_uri)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    int status = 0;
    int exists = AXIS2_FALSE;
    char *xmlnsprefix = NULL;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespace_uri, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, prefix , AXIS2_FAILURE);
    
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    exists = axis2_libxml2_writer_wrapper_validate_namespace(writer, env, 
                                namespace_uri, prefix);

    if(exists == AXIS2_TRUE)
        return AXIS2_SUCCESS;                                
    
    xmlnsprefix = (axis2_char_t*)AXIS2_MALLOC((*env)->allocator,
                    (sizeof(char)* (strlen(prefix) +7)));
    sprintf(xmlnsprefix,"xmlns:%s",prefix);
    
    
    status = xmlTextWriterWriteAttribute(writer_impl->xml_writer,
                   BAD_CAST xmlnsprefix, BAD_CAST namespace_uri);
    AXIS2_FREE((*env)->allocator, xmlnsprefix);
    xmlnsprefix = NULL;
                       
    
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error,
             AXIS2_ERROR_WRITING_ATTRIBUTE_WITH_NAMESPACE_PREFIX,  AXIS2_FAILURE);
                        
        return AXIS2_FAILURE;    
    }
    
    AXIS2_FREE((*env)->allocator, xmlnsprefix);
    xmlnsprefix = NULL;
    return AXIS2_SUCCESS;   
}

/** should be implemented */
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_default_namespace
                                            (axis2_xml_writer_t *writer,
                                             axis2_env_t **env,
                                             axis2_char_t *namespace_uri)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    int status = 0;
    int exists = AXIS2_FALSE;
    axis2_char_t *xmlns = NULL;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespace_uri, AXIS2_FAILURE)
    
    exists = axis2_libxml2_writer_wrapper_validate_namespace(writer, env, namespace_uri, NULL);
    if(exists == AXIS2_TRUE)
    {
        /* namespace already declared , nothing to do */
        return AXIS2_SUCCESS;
    }
    
    xmlns = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_char_t)*(strlen("xmlns")+1));
    sprintf(xmlns,"xmlns");
    
    status = xmlTextWriterWriteAttribute(writer_impl->xml_writer,
                   NULL , BAD_CAST namespace_uri);
                   
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_WRITING_DEFAULT_NAMESPACE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }                   
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_comment(axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *value)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    int status = 0;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
    
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    status = xmlTextWriterWriteComment(writer_impl->xml_writer,BAD_CAST value);
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error,
             AXIS2_ERROR_WRITING_COMMENT,  AXIS2_FAILURE);
                        
        return AXIS2_FAILURE;    
    }
    return AXIS2_SUCCESS;    
}

                                           
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_processing_instruction(
                                           axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *target)
{   
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    int status = 0;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, target, AXIS2_FAILURE);
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    
    status = xmlTextWriterStartPI(writer_impl->xml_writer, BAD_CAST target);
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error,
            AXIS2_ERROR_WRITING_PROCESSING_INSTRUCTION, AXIS2_FAILURE);
        return AXIS2_FAILURE;            
    }
    return AXIS2_SUCCESS;    
}    
                                           
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_processing_instruction_data(
                                           axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *target,
                                           axis2_char_t *data)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    int status = 0;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, target, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, data, AXIS2_FAILURE);
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    
    status = xmlTextWriterWritePI(writer_impl->xml_writer,
                                  BAD_CAST target,
                                  BAD_CAST data);
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error,
            AXIS2_ERROR_WRITING_PROCESSING_INSTRUCTION, AXIS2_FAILURE);
        return AXIS2_FAILURE;            
    }
    return AXIS2_SUCCESS; 
}
                                           
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_cdata(axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *data)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    int status = 0;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, data, AXIS2_FAILURE);
    
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    
    status = xmlTextWriterWriteCDATA(writer_impl->xml_writer, BAD_CAST data);
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_WTRING_CDATA, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;    
}    

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_dtd(axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *dtd)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    int status = 0;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, dtd, AXIS2_FAILURE);
    
    status = xmlTextWriterStartDTD(writer_impl->xml_writer,
                    BAD_CAST dtd, NULL, NULL);
    if(status < 0)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_WRITING_DTD, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_entity_ref(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *name)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, name, AXIS2_FAILURE);
    printf("not implemented ");
    return AXIS2_FAILURE;
}    

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_start_document(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env)
{
    axis2_libxml2_writer_wrapper_impl_t *wrapper_impl = NULL;
    int status = 0;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    
    wrapper_impl = AXIS2_INTF_TO_IMPL(writer);
    status = xmlTextWriterStartDocument(wrapper_impl->xml_writer,
                             NULL, NULL, NULL);
    if(status < 0)
    {   AXIS2_ERROR_SET((*env)->error,
            AXIS2_ERROR_WRITING_START_DOCUMENT,AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;        
}    

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_start_document_with_version(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *version)
{   
    axis2_libxml2_writer_wrapper_impl_t *wrapper_impl = NULL;
    int status = 0;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, version, AXIS2_FAILURE);
    
    wrapper_impl = AXIS2_INTF_TO_IMPL(writer);
    status = xmlTextWriterStartDocument(wrapper_impl->xml_writer,
                             version , NULL, NULL);
    if(status < 0)
    {   AXIS2_ERROR_SET((*env)->error,
            AXIS2_ERROR_WRITING_START_DOCUMENT, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}    

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_start_document_with_version_encoding(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *version,
                                         axis2_char_t *encoding)
{
    axis2_libxml2_writer_wrapper_impl_t *wrapper_impl = NULL;
    int status = 0;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    
    wrapper_impl = AXIS2_INTF_TO_IMPL(writer);
    status = xmlTextWriterStartDocument(wrapper_impl->xml_writer,
                              version , encoding , NULL);
    if(status < 0)
    {   AXIS2_ERROR_SET((*env)->error,
            AXIS2_ERROR_WRITING_START_DOCUMENT, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

                                         
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_characters(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *text)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    int status = 0;
    
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, text, AXIS2_FAILURE); 
    
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    
    status = xmlTextWriterWriteString(writer_impl->xml_writer,
                                      BAD_CAST text);
    if(status < 0)
    {   AXIS2_ERROR_SET((*env)->error,
            AXIS2_ERROR_WRITING_START_DOCUMENT, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}    

axis2_char_t* AXIS2_CALL
axis2_libxml2_writer_wrapper_get_prefix(  axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *uri)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(writer, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, uri, NULL);
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    printf("not implemented ");
    return NULL;    
}

axis2_status_t AXIS2_CALL 
axis2_libxml2_writer_wrapper_set_prefix( axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *prefix,
                                         axis2_char_t *uri)
{   
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, prefix, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, uri, AXIS2_FAILURE);
    
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    printf(" not implemented ");
    return AXIS2_FAILURE;
}
axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_set_default_prefix(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *uri)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, uri, AXIS2_FAILURE);
    
    printf(" not implemented ");
    return AXIS2_FAILURE;
}    

axis2_status_t AXIS2_CALL
axis2_libxml2_writer_wrapper_write_encoded(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *text,
                                         int in_attr)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, text, AXIS2_FAILURE);
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    printf(" not implemented ");
    return AXIS2_FAILURE;    
}


static axis2_status_t 
axis2_libxml2_writer_wrapper_validate_namespace(axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *ns_uri,
                                         axis2_char_t *prefix)
{
    int i = 0;
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    if(!prefix && !ns_uri)
        return AXIS2_FAILURE;
    
    /* default namespace handling */
    if(!prefix && ns_uri)
    {
        for(i =0; i < writer_impl->qname_array.current_no; i++)
        {
            if(AXIS2_STRCMP(writer_impl->qname_array.uri[i], ns_uri) == 0)
            {
                return AXIS2_TRUE;
            }        
        }
        if(writer_impl->qname_array.current_no == writer_impl->qname_array.allocated)
        {
           writer_impl->qname_array.prefix = AXIS2_REALLOC((*env)->allocator,
                                                writer_impl->qname_array.prefix,
                                                sizeof(axis2_char_t *)*10);
           writer_impl->qname_array.uri = AXIS2_REALLOC((*env)->allocator,
                                                writer_impl->qname_array.prefix,
                                                sizeof(axis2_char_t *)*10);
           writer_impl->qname_array.allocated += 10;
            
        }
        writer_impl->qname_array.prefix[writer_impl->qname_array.current_no] = 
                AXIS2_STRDUP("",env);
        writer_impl->qname_array.uri[writer_impl->qname_array.current_no] =
                AXIS2_STRDUP(ns_uri, env);
        writer_impl->qname_array.current_no++;                
        return AXIS2_FALSE;
    }
  
    for(i =0; i < writer_impl->qname_array.current_no; i++)
    {
        if(AXIS2_STRCMP(writer_impl->qname_array.prefix[i], prefix) == 0)
        {
            return AXIS2_TRUE;
        }        
    }
    if(writer_impl->qname_array.current_no == writer_impl->qname_array.allocated)
    {
        writer_impl->qname_array.prefix = AXIS2_REALLOC((*env)->allocator,
                                            writer_impl->qname_array.prefix,
                                            sizeof(axis2_char_t *)*10);
        writer_impl->qname_array.uri = AXIS2_REALLOC((*env)->allocator,
                                            writer_impl->qname_array.prefix,
                                            sizeof(axis2_char_t *)*10);
        writer_impl->qname_array.allocated += 10;
        
    }
    writer_impl->qname_array.prefix[writer_impl->qname_array.current_no] = 
            AXIS2_STRDUP(prefix ,env);
    writer_impl->qname_array.uri[writer_impl->qname_array.current_no] =
            AXIS2_STRDUP(ns_uri, env);
    writer_impl->qname_array.current_no++;                
return AXIS2_FALSE;

}

static axis2_status_t 
axis2_libxml2_writer_wrapper_reset(axis2_xml_writer_t *writer,
                                   axis2_env_t **env)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    int i = 0;
    
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    
        for( i = 0; i < writer_impl->qname_array.current_no; i++)
        {
            AXIS2_FREE((*env)->allocator, writer_impl->qname_array.prefix[i]);
            AXIS2_FREE((*env)->allocator, writer_impl->qname_array.uri[i]);
            writer_impl->qname_array.prefix[i] = NULL;
            writer_impl->qname_array.uri[i] = NULL;
        }
        
        writer_impl->qname_array.current_no = 0;    
    return AXIS2_SUCCESS;
}


axis2_char_t* AXIS2_CALL
axis2_libxml2_writer_wrapper_get_xml(axis2_xml_writer_t *writer,
                                     axis2_env_t **env)
{
    axis2_libxml2_writer_wrapper_impl_t *writer_impl = NULL;
    axis2_char_t *output = NULL;
    writer_impl = AXIS2_INTF_TO_IMPL(writer);
    if(writer_impl->xml_writer)
    {
        xmlFreeTextWriter(writer_impl->xml_writer);
        writer_impl->xml_writer = NULL;
    }
    if(writer_impl->writer_type == AXIS2_LIBXML2_WRITER_MEMORY)
    {

        if(writer_impl->buffer != NULL)
        {
           output = AXIS2_MALLOC((*env)->allocator,     
                    sizeof(axis2_char_t)*(
                        strlen((const axis2_char_t*)(writer_impl->buffer->content))+1));
            sprintf(output, 
                    ((const axis2_char_t*)(writer_impl->buffer->content)));
    	    xmlBufferFree(writer_impl->buffer);
    	    writer_impl->buffer = NULL;
        }
    }
    else if(writer_impl->writer_type == AXIS2_LIBXML2_WRITER_FILE)
    {
        printf(" This is not supported for this type of writer");
    }

    return output;   
}
