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
 
#include <guththila_xml_stream_writer.h>
#include <axis2.h>
#include <axis2_env.h>
#include <axis2_xml_writer.h>
/*********************** function prototypes ***********************************/
axis2_status_t AXIS2_CALL 
guththila_xml_writer_wrapper_free(axis2_xml_writer_t *writer,
                                  axis2_env_t **env);
        
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_start_element(
                                    axis2_xml_writer_t *writer,
                                    axis2_env_t **env,
                                    axis2_char_t *localname);
        
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_end_start_element(axis2_xml_writer_t *writer,
                                               axis2_env_t **env);
        
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_start_element_with_namespace(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname,
                                        axis2_char_t *namespace_uri);
        
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_start_element_with_namespace_prefix(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname,
                                        axis2_char_t *namespace_uri,
                                        axis2_char_t *prefix);       
                                    
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_empty_element(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname);
        
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_empty_element_with_namespace(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname,
                                        axis2_char_t *namespace_uri);

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_empty_element_with_namespace_prefix(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname,
                                        axis2_char_t *namespace_uri,
                                        axis2_char_t *prefix);

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_end_element(axis2_xml_writer_t *writer,
                                               axis2_env_t **env);

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_end_document(axis2_xml_writer_t *writer,
                                               axis2_env_t **env);

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_attribute(axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *localname,
                                           axis2_char_t *value);
                                               
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_attribute_with_namespace(
                                          axis2_xml_writer_t *writer,
                                          axis2_env_t **env,
                                          axis2_char_t *localname,
                                          axis2_char_t *value,
                                          axis2_char_t *namespace_uri);
                                          
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_attribute_with_namespace_prefix(
                                          axis2_xml_writer_t *writer,
                                          axis2_env_t **env,
                                          axis2_char_t *localname,
                                          axis2_char_t *value,
                                          axis2_char_t *namespace_uri,
                                          axis2_char_t *prefix);
                                          
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_namespace(axis2_xml_writer_t *writer,
                                             axis2_env_t **env,
                                             axis2_char_t *prefix,
                                             axis2_char_t *namespace_uri);   


axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_default_namespace
                                            (axis2_xml_writer_t *writer,
                                             axis2_env_t **env,
                                             axis2_char_t *namespace_uri);  

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_comment(axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *value);
                                           
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_processing_instruction(
                                           axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *target);
                                           
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_processing_instruction_data(
                                           axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *target,
                                           axis2_char_t *data);
                                           
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_cdata(axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *data);

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_dtd(axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *dtd); 

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_entity_ref(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *name); 

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_start_document(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env);  

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_start_document_with_version(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *version);   

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_start_document_with_version_encoding(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *encoding,
                                         axis2_char_t *version);
                                         
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_characters(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *text);   

axis2_char_t* AXIS2_CALL
guththila_xml_writer_wrapper_get_prefix(  axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *uri);

axis2_status_t AXIS2_CALL 
guththila_xml_writer_wrapper_set_prefix(  axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *prefix,
                                         axis2_char_t *uri);
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_set_default_prefix(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *uri);                                                 

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_encoded(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *text,
                                         int in_attr);



/**************************** end function pointers ****************************/

typedef struct guththila_xml_writer_wrapper_impl
{
    axis2_xml_writer_t writer;
    guththila_xml_stream_writer_t *guththila_writer;
    guththila_environment_t *guththila_env;
}guththila_xml_writer_wrapper_impl_t;


/***************************** Macros ******************************************/

#define AXIS2_INTF_TO_IMPL(p) ((guththila_xml_writer_wrapper_impl_t*)p)

/****************************** End macro **************************************/

AXIS2_DECLARE(axis2_xml_writer_t *)
axis2_xml_writer_create(axis2_env_t **env,
                        void *stream,
                        axis2_char_t *encoding, int is_prefix_default)
{
    guththila_xml_writer_wrapper_impl_t *writer_impl;
    guththila_allocator_t *allocator;
  
    
    AXIS2_ENV_CHECK(env, NULL);
    
    writer_impl = (guththila_xml_writer_wrapper_impl_t *)AXIS2_MALLOC((*env)->allocator,
                   sizeof(guththila_xml_writer_wrapper_impl_t));
    if(!writer_impl)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    
    
    allocator = guththila_allocator_init(NULL);
    writer_impl->guththila_env = guththila_environment_create(allocator, NULL,  NULL);
    writer_impl->guththila_writer = guththila_create_xml_stream_writer(
                                                    writer_impl->guththila_env,
                                                    stream, encoding , 
                                                    is_prefix_default);
                
    if(!(writer_impl->guththila_writer))
    {
        AXIS2_FREE((*env)->allocator,writer_impl);
        AXIS2_FREE((*env)->allocator,allocator);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
    writer_impl->writer.ops = NULL;
    writer_impl->writer.ops = (axis2_xml_writer_ops_t*)AXIS2_MALLOC((*env)->allocator,
                                    sizeof(axis2_xml_writer_ops_t));
    if(!(writer_impl->writer.ops))
    {
        AXIS2_FREE((*env)->allocator,writer_impl);
        AXIS2_FREE((*env)->allocator,allocator);
        
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    /* operations */
    writer_impl->writer.ops->free = guththila_xml_writer_wrapper_free;
    writer_impl->writer.ops->write_start_element = guththila_xml_writer_wrapper_write_start_element;
    writer_impl->writer.ops->write_start_element_with_namespace =
            guththila_xml_writer_wrapper_write_start_element_with_namespace;
    writer_impl->writer.ops->write_start_element_with_namespace_prefix = 
            guththila_xml_writer_wrapper_write_start_element_with_namespace_prefix;
    
    writer_impl->writer.ops->write_empty_element = guththila_xml_writer_wrapper_write_empty_element;
    writer_impl->writer.ops->write_empty_element_with_namespace  =
            guththila_xml_writer_wrapper_write_empty_element_with_namespace;
    writer_impl->writer.ops->write_empty_element_with_namespace_prefix =
            guththila_xml_writer_wrapper_write_empty_element_with_namespace_prefix;
    
    writer_impl->writer.ops->write_end_element = 
            guththila_xml_writer_wrapper_write_end_element;
    writer_impl->writer.ops->write_end_document = 
            guththila_xml_writer_wrapper_write_end_document;
    
    writer_impl->writer.ops->write_attribute = 
            guththila_xml_writer_wrapper_write_attribute;
    writer_impl->writer.ops->write_attribute_with_namespace =
            guththila_xml_writer_wrapper_write_attribute_with_namespace;
    writer_impl->writer.ops->write_attribute_with_namespace_prefix =
            guththila_xml_writer_wrapper_write_attribute_with_namespace_prefix;
    writer_impl->writer.ops->write_namespace = 
            guththila_xml_writer_wrapper_write_namespace;
    writer_impl->writer.ops->write_default_namespace =
            guththila_xml_writer_wrapper_write_default_namespace;
    writer_impl->writer.ops->write_comment = 
            guththila_xml_writer_wrapper_write_comment;
    writer_impl->writer.ops->write_processing_instruction = 
            guththila_xml_writer_wrapper_write_processing_instruction;
    writer_impl->writer.ops->write_processing_instruction_data = 
            guththila_xml_writer_wrapper_write_processing_instruction_data;
    writer_impl->writer.ops->write_cdata = 
            guththila_xml_writer_wrapper_write_cdata;
    writer_impl->writer.ops->write_dtd = 
            guththila_xml_writer_wrapper_write_dtd;
    writer_impl->writer.ops->write_entity_ref = 
            guththila_xml_writer_wrapper_write_entity_ref;
    writer_impl->writer.ops->write_start_document = 
            guththila_xml_writer_wrapper_write_start_document;
    writer_impl->writer.ops->write_start_document_with_version = 
            guththila_xml_writer_wrapper_write_start_document_with_version;
    writer_impl->writer.ops->write_start_document_with_version_encoding = 
            guththila_xml_writer_wrapper_write_start_document_with_version_encoding;
    writer_impl->writer.ops->write_characters = 
            guththila_xml_writer_wrapper_write_characters;
    writer_impl->writer.ops->get_prefix =
            guththila_xml_writer_wrapper_get_prefix;
    writer_impl->writer.ops->set_prefix = guththila_xml_writer_wrapper_set_prefix;
    writer_impl->writer.ops->set_default_prefix = 
            guththila_xml_writer_wrapper_set_default_prefix;
    writer_impl->writer.ops->write_encoded = 
            guththila_xml_writer_wrapper_write_encoded;    
    
    
    
    
    
    return &(writer_impl->writer);
}

axis2_status_t AXIS2_CALL 
guththila_xml_writer_wrapper_free(axis2_xml_writer_t *writer,
                                  axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    if(AXIS2_INTF_TO_IMPL(writer)->guththila_writer)
        guththila_xml_stream_writer_free(
            AXIS2_INTF_TO_IMPL(writer)->guththila_env, writer);
            
    if(AXIS2_INTF_TO_IMPL(writer)->guththila_env)
        guththila_environment_free(AXIS2_INTF_TO_IMPL(writer)->guththila_env);
    if(writer->ops)
        AXIS2_FREE((*env)->allocator, writer->ops);
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(writer));
    return AXIS2_SUCCESS;
}
        
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_start_element(
                                    axis2_xml_writer_t *writer,
                                    axis2_env_t **env,
                                    axis2_char_t *localname)
{
    int status = AXIS2_SUCCESS;
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    status = guththila_xml_stream_writer_write_start_element(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    localname);
    return status;
}

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_end_start_element(axis2_xml_writer_t *writer,
                                               axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    return guththila_xml_stream_writer_end_start_element(    
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer);
}
        
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_start_element_with_namespace(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname,
                                        axis2_char_t *namespace_uri)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error,namespace_uri, AXIS2_FAILURE);
    return guththila_xml_stream_writer_write_start_element_with_namespace(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    localname,
                    namespace_uri);
}
        
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_start_element_with_namespace_prefix(
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
    return guththila_xml_stream_writer_write_start_element_with_namespace_prefix(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    localname, namespace_uri, prefix);
    
}
                                    
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_empty_element(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    return guththila_xml_stream_writer_write_empty_element(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer, localname);
    
    
}
        
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_empty_element_with_namespace(
                                        axis2_xml_writer_t *writer,
                                        axis2_env_t **env,
                                        axis2_char_t *localname,
                                        axis2_char_t *namespace_uri)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespace_uri, AXIS2_FAILURE);
    return guththila_xml_stream_writer_write_empty_element_with_namespace(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    localname, namespace_uri);
}

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_empty_element_with_namespace_prefix(
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
    
    return guththila_xml_stream_writer_write_empty_element_with_namespace_prefix(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    localname, namespace_uri, prefix);
    
    
}

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_end_element(axis2_xml_writer_t *writer,
                                               axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    return guththila_xml_stream_writer_write_end_element(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer);
}

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_end_document(axis2_xml_writer_t *writer,
                                               axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    return guththila_xml_stream_writer_write_end_document(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer);
    
}

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_attribute(axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *localname,
                                           axis2_char_t *value)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, localname, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
    return guththila_xml_stream_writer_write_attribute(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    localname, value);
                        
    
    
}
                                               
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_attribute_with_namespace(
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
    
    return guththila_xml_stream_writer_write_attribute_with_namespace(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    localname, value, namespace_uri);
}
                                          
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_attribute_with_namespace_prefix(
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
    return guththila_xml_stream_writer_write_attribute_with_namespace_prefix(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    localname, value, namespace_uri, prefix);
    
    
    
}
                                          
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_namespace(axis2_xml_writer_t *writer,
                                             axis2_env_t **env,
                                             axis2_char_t *prefix,
                                             axis2_char_t *namespace_uri)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespace_uri, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, prefix , AXIS2_FAILURE);
    return guththila_xml_stream_writer_write_namespace(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    prefix, namespace_uri);

}


axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_default_namespace
                                            (axis2_xml_writer_t *writer,
                                             axis2_env_t **env,
                                             axis2_char_t *namespace_uri)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespace_uri, AXIS2_FAILURE);
    return guththila_xml_stream_writer_write_default_namespace(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    namespace_uri);
    
}

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_comment(axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *value)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
    return guththila_xml_stream_writer_write_comment(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    value);
    
}

                                           
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_processing_instruction(
                                           axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *target)
{   
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, target, AXIS2_FAILURE);
    return guththila_xml_stream_writer_write_processing_instruction(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    target);
    
    
}    
                                           
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_processing_instruction_data(
                                           axis2_xml_writer_t *writer,
                                           axis2_env_t **env,
                                           axis2_char_t *target,
                                           axis2_char_t *data)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, target, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, data, AXIS2_FAILURE);
    return guththila_xml_stream_writer_write_processing_instruction_data(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    target, data);
    
}
                                           
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_cdata(axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *data)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, data, AXIS2_FAILURE);
    return guththila_xml_stream_writer_write_cdata(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    data);
                        
}    

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_dtd(axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *dtd)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, dtd, AXIS2_FAILURE);
    return guththila_xml_stream_writer_write_dtd(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    dtd);
}

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_entity_ref(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *name)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, name, AXIS2_FAILURE);
    return guththila_xml_stream_writer_write_entity_ref(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    name);
    
}    

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_start_document(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env)
{
     AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
     return guththila_xml_stream_writer_write_start_document(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer);
}    

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_start_document_with_version(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *version)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, version, AXIS2_FAILURE); 
    return guththila_xml_stream_writer_write_start_document_with_version(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    version);
}    

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_start_document_with_version_encoding(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *encoding,
                                         axis2_char_t *version)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, encoding, AXIS2_FAILURE); 
    AXIS2_PARAM_CHECK((*env)->error, version, AXIS2_FAILURE); 
    
    return guththila_xml_stream_writer_write_start_document_with_version_encoding(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    encoding, version);
}

                                         
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_characters(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *text)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, text, AXIS2_FAILURE); 
    return guththila_xml_stream_writer_write_characters(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer,
                    text);
}    

axis2_char_t* AXIS2_CALL
guththila_xml_writer_wrapper_get_prefix(  axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *uri)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, uri, NULL);
    return guththila_xml_stream_writer_get_prefix(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer, uri);
                    
    
    
}

axis2_status_t AXIS2_CALL 
guththila_xml_writer_wrapper_set_prefix( axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *prefix,
                                         axis2_char_t *uri)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, prefix, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, uri, AXIS2_FAILURE);
    return guththila_xml_stream_writer_set_prefix(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer, prefix, uri);
    
}
axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_set_default_prefix(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *uri)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, uri, AXIS2_FAILURE);
    return guththila_xml_stream_writer_set_default_prefix(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer, uri);
}    

axis2_status_t AXIS2_CALL
guththila_xml_writer_wrapper_write_encoded(
                                         axis2_xml_writer_t *writer,
                                         axis2_env_t **env,
                                         axis2_char_t *text,
                                         int in_attr)
{
    AXIS2_FUNC_PARAM_CHECK(writer, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, text, AXIS2_FAILURE);
    return guththila_xml_stream_writer_write_encoded(
                    AXIS2_INTF_TO_IMPL(writer)->guththila_env,
                    AXIS2_INTF_TO_IMPL(writer)->guththila_writer, text, in_attr);
}
