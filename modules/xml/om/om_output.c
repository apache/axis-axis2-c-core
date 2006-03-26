/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <axis2_om_output.h>
#include <stdarg.h>
#include <axis2_string.h>
#include <axis2_xml_writer.h>
#include <axis2_mime_output.h>
#include <axis2_om_output_format.h>
#include <axis2_om_text.h>
#include <axis2_array_list.h>
/*#define AXIS2_DEFAULT_CHAR_SET_ENCODING  "UTF-8"*/
/*#define AXIS2_DEFAULT_CHAR_SET_ENCODING  "ISO-8859-1"*/
#define MAX_ARGS  4
/****************************** impl struct ***********************************/

typedef struct axis2_om_output_impl_t
{
    axis2_om_output_t om_output;
    /** axis2_xml_writer. any xml writer which 
        implemet axis2_xml_writer.h interface  */
    axis2_xml_writer_t *xml_writer;

    /* following fields are not used currently but will be used in the future */
    axis2_bool_t do_optimize;
    axis2_char_t *mime_boundary;
    axis2_char_t *root_content_id;
    int next_id;
    axis2_bool_t is_soap11;
    axis2_char_t *char_set_encoding;
    /* xml version */
    axis2_char_t *xml_version;
    axis2_bool_t ignore_xml_declaration;
  
    axis2_array_list_t *binary_node_list;    
}axis2_om_output_impl_t;

/************************ Macro ***********************************************/
#define AXIS2_INTF_TO_IMPL(output) ((axis2_om_output_impl_t*)output)

/************************ function prototypes *********************************/

axis2_status_t AXIS2_CALL
axis2_om_output_free(axis2_om_output_t *om_output,
                     axis2_env_t **env);
                     
                                             
axis2_bool_t AXIS2_CALL 
axis2_om_output_is_soap11(axis2_om_output_t *om_output,
                          axis2_env_t **env); 
                                                                                    
axis2_bool_t AXIS2_CALL 
axis2_om_output_is_ignore_xml_declaration
                            (axis2_om_output_t *om_output,
                            axis2_env_t **env);                               

axis2_status_t AXIS2_CALL 
axis2_om_output_set_ignore_xml_declaration
                            (axis2_om_output_t *om_output,
                            axis2_env_t **env,
                            axis2_bool_t ignore_xml_dec); 
                            
axis2_status_t AXIS2_CALL 
axis2_om_output_set_soap11(axis2_om_output_t *om_output,
                           axis2_env_t **env,
                           axis2_bool_t soap11);
                            
axis2_status_t AXIS2_CALL 
axis2_om_output_set_xml_version(axis2_om_output_t *om_output,
                                axis2_env_t **env,
                                axis2_char_t *xml_version);
                                
axis2_char_t* AXIS2_CALL 
axis2_om_output_get_xml_version(axis2_om_output_t *om_output,
                                axis2_env_t **env);  
                            
axis2_status_t AXIS2_CALL 
axis2_om_output_set_char_set_encoding
                            (axis2_om_output_t *om_output,
                            axis2_env_t **env,
                            axis2_char_t *char_set_encoding);
                                
axis2_char_t* AXIS2_CALL 
axis2_om_output_get_char_set_encoding
                            (axis2_om_output_t *om_output,
                            axis2_env_t **env); 
                            
axis2_status_t AXIS2_CALL 
axis2_om_output_set_do_optimize
                            (axis2_om_output_t *om_output,
                            axis2_env_t **env,
                            axis2_bool_t optimize); 

axis2_bool_t AXIS2_CALL
axis2_om_output_is_optimized(axis2_om_output_t *om_output,axis2_env_t **env);
                            
axis2_xml_writer_t* AXIS2_CALL
axis2_om_output_get_xml_writer
                            (axis2_om_output_t *om_output,
                            axis2_env_t **env);                                                                                                                                                                             
                                                                    
axis2_status_t AXIS2_CALL
axis2_om_output_write_xml_version_encoding(axis2_om_output_t *om_output,
                                           axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_om_output_flush(axis2_om_output_t *om_output,
                                           axis2_env_t **env);
axis2_char_t* AXIS2_CALL
axis2_om_output_get_next_content_id(axis2_om_output_t *om_output,
                        axis2_env_t **env);
                      

axis2_status_t AXIS2_CALL
axis2_om_output_write_optimized(axis2_om_output_t *om_output,
                        axis2_env_t **env, axis2_om_text_t *om_text);
/*********************** end function prototypes ******************************/                      

AXIS2_DECLARE(axis2_om_output_t *)
axis2_om_output_create (axis2_env_t **env, axis2_xml_writer_t *xml_writer)
{
    axis2_om_output_impl_t *om_output_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    
    om_output_impl = (axis2_om_output_impl_t *) AXIS2_MALLOC (
                        (*env)->allocator,
                        sizeof (axis2_om_output_impl_t));

    if (!om_output_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    om_output_impl->xml_writer = NULL;
    om_output_impl->xml_writer = xml_writer;
   
    om_output_impl->do_optimize = AXIS2_FALSE;
    om_output_impl->mime_boundary = 0;
    om_output_impl->root_content_id = 0;
    om_output_impl->next_id = 0;
    om_output_impl->is_soap11 = AXIS2_TRUE;
    om_output_impl->char_set_encoding = NULL;
    om_output_impl->xml_version = NULL;
    om_output_impl->ignore_xml_declaration = AXIS2_TRUE;
    om_output_impl->binary_node_list = NULL;
    
    om_output_impl->om_output.ops = NULL;
    
    om_output_impl->om_output.ops = (axis2_om_output_ops_t *)AXIS2_MALLOC(
                                        (*env)->allocator,
                                        sizeof(axis2_om_output_ops_t));
    if(!(om_output_impl->om_output.ops))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE((*env)->allocator, om_output_impl);
        return NULL;
    }
    om_output_impl->om_output.ops->free_fn = 
        axis2_om_output_free;
        
    om_output_impl->om_output.ops->is_soap11 =
        axis2_om_output_is_soap11;
        
    om_output_impl->om_output.ops->get_xml_version =
        axis2_om_output_get_xml_version;
    
    om_output_impl->om_output.ops->set_xml_version =
        axis2_om_output_set_xml_version;
    
    om_output_impl->om_output.ops->get_char_set_encoding =
        axis2_om_output_get_char_set_encoding;
    
    om_output_impl->om_output.ops->set_char_set_encoding =
        axis2_om_output_set_char_set_encoding;
    
    om_output_impl->om_output.ops->get_xml_writer =
        axis2_om_output_get_xml_writer;
    
    om_output_impl->om_output.ops->set_do_optimize =
        axis2_om_output_set_do_optimize;
    
    om_output_impl->om_output.ops->is_ignore_xml_declaration = 
        axis2_om_output_is_ignore_xml_declaration;
        
    om_output_impl->om_output.ops->set_soap11 =
        axis2_om_output_set_soap11;  

    om_output_impl->om_output.ops->write_xml_version_encoding =
        axis2_om_output_write_xml_version_encoding;
    
    om_output_impl->om_output.ops->get_next_content_id = axis2_om_output_get_next_content_id;
        
    om_output_impl->char_set_encoding = AXIS2_STRDUP(
        AXIS2_DEFAULT_CHAR_SET_ENCODING, env);        
    return &(om_output_impl->om_output);
}

 axis2_status_t AXIS2_CALL
 axis2_om_output_free(axis2_om_output_t *om_output,
                      axis2_env_t **env)
{
    axis2_om_output_impl_t *om_output_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    om_output_impl = AXIS2_INTF_TO_IMPL(om_output);
    
    if(NULL != om_output_impl->xml_version)
    {
        AXIS2_FREE((*env)->allocator, om_output_impl->xml_version);
        om_output_impl->xml_version = NULL;
    }        
    if(NULL != om_output_impl->char_set_encoding)
    {
        AXIS2_FREE((*env)->allocator, om_output_impl->char_set_encoding);
        om_output_impl->char_set_encoding = NULL;
    }        
    if(NULL != om_output_impl->xml_writer)
    {
        AXIS2_XML_WRITER_FREE(om_output_impl->xml_writer, env);
        om_output_impl->xml_writer = NULL;
    }        
    if(NULL != om_output->ops)
    {
        AXIS2_FREE((*env)->allocator, om_output->ops);
        om_output->ops = NULL;
    }
    AXIS2_FREE ((*env)->allocator, om_output_impl);
    om_output_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL 
axis2_om_output_is_soap11(axis2_om_output_t *om_output,
                          axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(om_output)->is_soap11;
} 
                                                                                    
axis2_bool_t AXIS2_CALL 
axis2_om_output_is_ignore_xml_declaration
                            (axis2_om_output_t *om_output,
                             axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(om_output)->ignore_xml_declaration;

}                               

axis2_status_t AXIS2_CALL 
axis2_om_output_set_ignore_xml_declaration
                            (axis2_om_output_t *om_output,
                            axis2_env_t **env,
                            axis2_bool_t ignore_xml_dec)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_output)->ignore_xml_declaration = ignore_xml_dec;
    return AXIS2_SUCCESS;
} 
                            
axis2_status_t AXIS2_CALL 
axis2_om_output_set_soap11(axis2_om_output_t *om_output,
                           axis2_env_t **env,
                           axis2_bool_t soap11)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_output)->is_soap11 = soap11;
    return AXIS2_SUCCESS;
}
                            
axis2_status_t AXIS2_CALL 
axis2_om_output_set_xml_version(axis2_om_output_t *om_output,
                                axis2_env_t **env,
                                axis2_char_t *xml_version)
{
    axis2_om_output_impl_t *output_impl = NULL; 
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    AXIS2_PARAM_CHECK((*env)->error, xml_version, AXIS2_FAILURE); 
    output_impl = AXIS2_INTF_TO_IMPL(om_output);
    
    if(NULL !=  output_impl->xml_version)
    {
        AXIS2_FREE((*env)->allocator,  output_impl->xml_version);
        output_impl->xml_version = NULL;
    }

    output_impl->xml_version = AXIS2_STRDUP(xml_version, env);
    if(!output_impl->xml_version)
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}
                                
axis2_char_t* AXIS2_CALL 
axis2_om_output_get_xml_version(axis2_om_output_t *om_output,
                                axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_output)->xml_version;
}  
                            
axis2_status_t AXIS2_CALL 
axis2_om_output_set_char_set_encoding
                            (axis2_om_output_t *om_output,
                            axis2_env_t **env,
                            axis2_char_t *char_set_encoding)
{
    axis2_om_output_impl_t *output_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, char_set_encoding, AXIS2_FAILURE); 
    output_impl = AXIS2_INTF_TO_IMPL(om_output);
    
    if(NULL != output_impl->char_set_encoding)
    {
        AXIS2_FREE((*env)->allocator, output_impl->char_set_encoding);
        output_impl->char_set_encoding = NULL;
    }
    
    output_impl->char_set_encoding = AXIS2_STRDUP(char_set_encoding, env);
    if(!output_impl->char_set_encoding)
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}
                                
axis2_char_t* AXIS2_CALL 
axis2_om_output_get_char_set_encoding
                            (axis2_om_output_t *om_output,
                            axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_output)->char_set_encoding;
} 
                            
axis2_status_t AXIS2_CALL 
axis2_om_output_set_do_optimize
                            (axis2_om_output_t *om_output,
                            axis2_env_t **env,
                            axis2_bool_t optimize)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_output)->do_optimize = optimize;
    return AXIS2_SUCCESS;    
} 
                            
axis2_xml_writer_t* AXIS2_CALL
axis2_om_output_get_xml_writer
                            (axis2_om_output_t *om_output,
                            axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_output)->xml_writer;

}                                                                                                                                                                             


/******************************************************************************/

AXIS2_DECLARE(axis2_status_t)
axis2_om_output_write (axis2_om_output_t * om_output,
                       axis2_env_t **env,
                       axis2_om_types_t type,
                       int no_of_args, ...)
{
    int status = AXIS2_SUCCESS;
    axis2_char_t *args_list[MAX_ARGS];
    axis2_om_output_impl_t *om_output_impl = NULL;
    int i = 0;
    va_list ap;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output_impl = AXIS2_INTF_TO_IMPL(om_output);

    va_start (ap, no_of_args);
    for (i = 0; i < no_of_args; i++)
    {
        args_list[i] = va_arg (ap, axis2_char_t *);
    }
    va_end (ap);


    switch (type)
    {
    case AXIS2_OM_ELEMENT:


        switch (no_of_args)
        {
        case 0:
            status =
                AXIS2_XML_WRITER_WRITE_END_ELEMENT(om_output_impl->xml_writer,
                                                   env);
            break;
        case 1:
            status = 
                AXIS2_XML_WRITER_WRITE_START_ELEMENT(om_output_impl->xml_writer,
                                                     env, args_list[0]);
        break;
        case 2:
            status = AXIS2_XML_WRITER_WRITE_START_ELEMENT_WITH_NAMESPACE(
                                                om_output_impl->xml_writer,
                                                env, 
                                                args_list[0],
                                                args_list[1]);
        break;
        case 3:
            status = 
                AXIS2_XML_WRITER_WRITE_START_ELEMENT_WITH_NAMESPACE_PREFIX(
                                                om_output_impl->xml_writer,
                                                env,
                                                args_list[0],
                                                args_list[1],
                                                args_list[2]);
            break;
        }
        break;

    case AXIS2_OM_ATTRIBUTE:
        switch (no_of_args)
        {
        case 2:
            status = 
                AXIS2_XML_WRITER_WRITE_ATTRIBUTE(om_output_impl->xml_writer,
                                                env,
                                                args_list[0],
                                                args_list[1]);
        break;
        case 3:
            status = AXIS2_XML_WRITER_WRITE_ATTRIBUTE_WITH_NAMESPACE(
                                                om_output_impl->xml_writer,env,
                                                args_list[0],
                                                args_list[1],
                                                args_list[2]);
        break;
        case 4:
            status = 
                AXIS2_XML_WRITER_WRITE_ATTRIBUTE_WITH_NAMESPACE_PREFIX(
                                                om_output_impl->xml_writer, env,
                                                args_list[0],
                                                args_list[1],
                                                args_list[2],
                                                args_list[3]);

         break;

        }
        break;

    case AXIS2_OM_NAMESPACE:
        status = AXIS2_XML_WRITER_WRITE_NAMESPACE(om_output_impl->xml_writer,
                                                  env,
                                                  args_list[0],
                                                  args_list[1]);
        break;
    case AXIS2_OM_TEXT:
        status = AXIS2_XML_WRITER_WRITE_CHARACTERS(om_output_impl->xml_writer,
                                                   env,
                                                   args_list[0]);
        break;
    case AXIS2_OM_COMMENT:
        status = AXIS2_XML_WRITER_WRITE_COMMENT(om_output_impl->xml_writer,
                                                env,
                                                args_list[0]);
        break;
    case AXIS2_OM_PROCESSING_INSTRUCTION:
        switch(no_of_args)
        {
         case 1:
            
            status = 
                    AXIS2_XML_WRITER_WRITE_PROCESSING_INSTRUCTION(
                                                om_output_impl->xml_writer,
                                                env, args_list[0]);
            break;
         case 2:
             
            status = AXIS2_XML_WRITER_WRITE_PROCESSING_INSTRUCTION_DATA(
                                                om_output_impl->xml_writer, 
                                                env, 
                                                args_list[0], 
                                                args_list[1]);        
            break;
        }
         break;
    case AXIS2_OM_DOCTYPE:
        status = AXIS2_XML_WRITER_WRITE_DTD(om_output_impl->xml_writer, 
                                            env, 
                                            args_list[0]);
        break;
    
    default:
        break;
    };

    if (status == AXIS2_SUCCESS)
    {
        return AXIS2_SUCCESS;
    }
    else
        return AXIS2_FAILURE;
}

axis2_status_t AXIS2_CALL 
axis2_om_output_write_xml_version_encoding(axis2_om_output_t *om_output,
                                           axis2_env_t **env)
{
    axis2_om_output_impl_t *output_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    output_impl = AXIS2_INTF_TO_IMPL(om_output);
    return AXIS2_XML_WRITER_WRITE_START_DOCUMENT_WITH_VERSION_ENCODING(
                                output_impl->xml_writer, 
                                env, 
                                output_impl->xml_version, 
                                output_impl->char_set_encoding);
    
}

/*=================================================*/
axis2_bool_t AXIS2_CALL
axis2_om_output_is_optimized(axis2_om_output_t *om_output,axis2_env_t **env)
{
    axis2_bool_t is_optimized = AXIS2_FALSE;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    axis2_om_output_format_t *om_output_format = axis2_om_output_format_create(env);
    is_optimized = AXIS2_OM_OUTPUT_FORMAT_IS_OPTIMIZED(om_output_format, env);    
    return is_optimized;
}

/*@TODO Following method method must be implemented*/
axis2_status_t AXIS2_CALL 
axis2_om_output_flush(axis2_om_output_t *om_output,
                                           axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    return AXIS2_SUCCESS;
}
/*
    public void flush() throws XMLStreamException {
        xmlWriter.flush();
        String SOAPContentType;
        if (format.isOptimized()) {
            if (format.isSOAP11()) {
                SOAPContentType = SOAP11Constants.SOAP_11_CONTENT_TYPE;
            } else {
                SOAPContentType = SOAP12Constants.SOAP_12_CONTENT_TYPE;
            }
            MIMEOutputUtils.complete(
                    outStream,
                    bufferedSOAPBody,
                    binaryNodeList,
                    format.getMimeBoundary(),
                    format.getRootContentId(),
                    format.getCharSetEncoding(), SOAPContentType);
        }
    }
*/
axis2_char_t* AXIS2_CALL
axis2_om_output_get_next_content_id(axis2_om_output_t *om_output,
                        axis2_env_t **env)
{
    axis2_om_output_format_t *om_output_format = NULL;
    axis2_char_t *next_content_id = NULL;
    AXIS2_ENV_CHECK(env,NULL);
    om_output_format = axis2_om_output_format_create(env);
    next_content_id = AXIS2_OM_OUTPUT_FORMAT_GET_NEXT_CONTENT_ID(om_output_format, env);
    return next_content_id;
}


axis2_status_t AXIS2_CALL
axis2_om_output_write_optimized(axis2_om_output_t *om_output, axis2_env_t **env, axis2_om_text_t *om_text)
{
    axis2_om_output_impl_t *om_output_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output_impl = AXIS2_INTF_TO_IMPL(om_output);
    om_output_impl->binary_node_list  = axis2_array_list_create(env, 5);
    AXIS2_ARRAY_LIST_ADD(om_output_impl->binary_node_list, env, (void *)om_text);
    return AXIS2_SUCCESS;
}
