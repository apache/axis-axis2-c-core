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
#include <axis2_om_text.h>
#include <axis2_soap_const.h>
#include <axis2_array_list.h>
#include <axis2_uuid_gen.h>
#include <axis2_mime_output.h>

#define AXIS2_DEFAULT_CHAR_SET_ENCODING  "UTF-8"
/** also defined in axis2_soap.h */

/** max args for om_output_write function */
#define MAX_ARGS  4

/****************************** impl struct ***********************************/

typedef struct axis2_om_output_impl_t
{
    axis2_om_output_t om_output;

    /** axis2_xml_writer. any xml writer which 
        implemet axis2_xml_writer.h interface  */
    axis2_xml_writer_t *xml_writer;

    axis2_bool_t do_optimize;

    axis2_char_t *mime_boundary;

    axis2_char_t *root_content_id;
    
    int next_id;
    
    axis2_char_t *next_content_id;
    
    axis2_bool_t is_soap11;

    axis2_char_t *char_set_encoding;

    axis2_char_t *xml_version;
    
    axis2_bool_t ignore_xml_declaration;
  
    axis2_array_list_t *binary_node_list;    

    axis2_mime_output_t *mime_output;
    
    axis2_char_t *mime_boundry;

}axis2_om_output_impl_t;

/************************ Macro ***********************************************/
#define AXIS2_INTF_TO_IMPL(output) ((axis2_om_output_impl_t*)output)

/************************ function prototypes *********************************/

axis2_status_t AXIS2_CALL
axis2_om_output_free(axis2_om_output_t *om_output,
                     const axis2_env_t *env);
                     
                                             
axis2_bool_t AXIS2_CALL 
axis2_om_output_is_soap11(axis2_om_output_t *om_output,
                          const axis2_env_t *env); 
                                                                                    
axis2_bool_t AXIS2_CALL 
axis2_om_output_is_ignore_xml_declaration(axis2_om_output_t *om_output,
                                          const axis2_env_t *env);                               

axis2_status_t AXIS2_CALL 
axis2_om_output_set_ignore_xml_declaration
                           (axis2_om_output_t *om_output,
                            const axis2_env_t *env,
                            axis2_bool_t ignore_xml_dec); 
                            
axis2_status_t AXIS2_CALL 
axis2_om_output_set_soap11(axis2_om_output_t *om_output,
                           const axis2_env_t *env,
                           axis2_bool_t soap11);
                            
axis2_status_t AXIS2_CALL 
axis2_om_output_set_xml_version(axis2_om_output_t *om_output,
                                const axis2_env_t *env,
                                axis2_char_t *xml_version);
                                
axis2_char_t* AXIS2_CALL 
axis2_om_output_get_xml_version(axis2_om_output_t *om_output,
                                const axis2_env_t *env);  
                            
axis2_status_t AXIS2_CALL 
axis2_om_output_set_char_set_encoding(axis2_om_output_t *om_output,
                                      const axis2_env_t *env,
                                      axis2_char_t *char_set_encoding);
                                
axis2_char_t* AXIS2_CALL 
axis2_om_output_get_char_set_encoding(axis2_om_output_t *om_output,
                                      const axis2_env_t *env); 
                            
axis2_status_t AXIS2_CALL 
axis2_om_output_set_do_optimize(axis2_om_output_t *om_output,
                                const axis2_env_t *env,
                                axis2_bool_t optimize); 

axis2_bool_t AXIS2_CALL
axis2_om_output_is_optimized(axis2_om_output_t *om_output,
                             const axis2_env_t *env);
                            
axis2_xml_writer_t* AXIS2_CALL
axis2_om_output_get_xml_writer(axis2_om_output_t *om_output,
                               const axis2_env_t *env);  
                                                                                          
axis2_status_t AXIS2_CALL
axis2_om_output_write_xml_version_encoding(axis2_om_output_t *om_output,
                                           const axis2_env_t *env);

axis2_char_t* AXIS2_CALL
axis2_om_output_get_next_content_id(axis2_om_output_t *om_output,
                                    const axis2_env_t *env);

axis2_char_t* AXIS2_CALL
axis2_om_output_get_root_content_id(axis2_om_output_t *om_output,
                                    const axis2_env_t *env);
                                    
axis2_char_t* AXIS2_CALL
axis2_om_output_get_mime_boundry(axis2_om_output_t *om_output,
                                 const axis2_env_t *env);
                                                                        
axis2_byte_t* AXIS2_CALL 
axis2_om_output_flush(axis2_om_output_t *om_output,
                      const axis2_env_t *env,
                      axis2_byte_t **output_stream,
                      int *output_stream_size);

axis2_char_t* AXIS2_CALL 
axis2_om_output_get_content_type(axis2_om_output_t *om_output,
                                 const axis2_env_t *env);                     

axis2_status_t AXIS2_CALL
axis2_om_output_write_optimized(axis2_om_output_t *om_output,
                                const axis2_env_t *env, 
                                axis2_om_text_t *om_text);
                                
                                
                        
/*********************** end function prototypes ******************************/                      

AXIS2_EXTERN axis2_om_output_t * AXIS2_CALL
axis2_om_output_create (const axis2_env_t *env, 
                        axis2_xml_writer_t *xml_writer)
{
    axis2_om_output_impl_t *om_output_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    
    om_output_impl = (axis2_om_output_impl_t *) AXIS2_MALLOC (
                        env->allocator,
                        sizeof (axis2_om_output_impl_t));

    if (!om_output_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    om_output_impl->xml_writer = NULL;
    om_output_impl->xml_writer = xml_writer;   
    om_output_impl->do_optimize = AXIS2_FALSE;    
    om_output_impl->mime_boundary = NULL;    
    om_output_impl->root_content_id = NULL;    
    om_output_impl->next_content_id = NULL;    
    om_output_impl->next_id = 0;    
    om_output_impl->is_soap11 = AXIS2_TRUE;
    om_output_impl->char_set_encoding = NULL;
    om_output_impl->xml_version = NULL;
    om_output_impl->ignore_xml_declaration = AXIS2_TRUE;
    om_output_impl->binary_node_list = NULL;
    om_output_impl->mime_output = NULL;
    om_output_impl->mime_boundry = NULL;
    om_output_impl->om_output.ops = NULL;
    
    om_output_impl->char_set_encoding = AXIS2_STRDUP(
        AXIS2_DEFAULT_CHAR_SET_ENCODING, env); 
        
    if(!(om_output_impl->char_set_encoding))
    {
        AXIS2_FREE(env->allocator, om_output_impl);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }        
    
    om_output_impl->om_output.ops = (axis2_om_output_ops_t *)AXIS2_MALLOC(
                                        env->allocator,
                                        sizeof(axis2_om_output_ops_t));
    if(!(om_output_impl->om_output.ops))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE(env->allocator, om_output_impl);
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
    
    om_output_impl->om_output.ops->is_optimized =
        axis2_om_output_is_optimized;
    
    om_output_impl->om_output.ops->is_ignore_xml_declaration = 
        axis2_om_output_is_ignore_xml_declaration;
        
    om_output_impl->om_output.ops->set_soap11 =
        axis2_om_output_set_soap11;  

    om_output_impl->om_output.ops->write_xml_version_encoding =
        axis2_om_output_write_xml_version_encoding;
    
    om_output_impl->om_output.ops->get_next_content_id = axis2_om_output_get_next_content_id;
    
    om_output_impl->om_output.ops->get_content_type =
        axis2_om_output_get_content_type;        
    
    om_output_impl->om_output.ops->get_root_content_id =
        axis2_om_output_get_root_content_id;
        
    om_output_impl->om_output.ops->get_mime_boundry =
        axis2_om_output_get_mime_boundry;
                
    om_output_impl->om_output.ops->flush =
        axis2_om_output_flush;

    return &(om_output_impl->om_output);
}

 axis2_status_t AXIS2_CALL
 axis2_om_output_free(axis2_om_output_t *om_output,
                      const axis2_env_t *env)
{
    axis2_om_output_impl_t *om_output_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    om_output_impl = AXIS2_INTF_TO_IMPL(om_output);
    
    if(NULL != om_output_impl->xml_version)
    {
        AXIS2_FREE(env->allocator, om_output_impl->xml_version);
        om_output_impl->xml_version = NULL;
    }        
    if(NULL != om_output_impl->char_set_encoding)
    {
        AXIS2_FREE(env->allocator, om_output_impl->char_set_encoding);
        om_output_impl->char_set_encoding = NULL;
    }
    if(NULL != om_output_impl->mime_boundary)
    {
        AXIS2_FREE(env->allocator, om_output_impl->mime_boundary);
        om_output_impl->mime_boundary = NULL;
    }
    if(NULL != om_output_impl->next_content_id)
    {
        AXIS2_FREE(env->allocator, om_output_impl->next_content_id);
        om_output_impl->next_content_id = NULL;
    }
    if(NULL != om_output_impl->root_content_id)
    {
        AXIS2_FREE(env->allocator, om_output_impl->root_content_id);
        om_output_impl->root_content_id = NULL;
    }
    
    if(NULL != om_output_impl->xml_writer)
    {
        AXIS2_XML_WRITER_FREE(om_output_impl->xml_writer, env);
        om_output_impl->xml_writer = NULL;
    }        
    
    if (om_output_impl->binary_node_list)
    {
        AXIS2_ARRAY_LIST_FREE(om_output_impl->binary_node_list, env);
        om_output_impl->binary_node_list = NULL;
    }

    if(NULL != om_output->ops)
    {
        AXIS2_FREE(env->allocator, om_output->ops);
        om_output->ops = NULL;
    }
    AXIS2_FREE (env->allocator, om_output_impl);
    om_output_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL 
axis2_om_output_is_soap11(axis2_om_output_t *om_output,
                          const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(om_output)->is_soap11;
} 
                                                                                    
axis2_bool_t AXIS2_CALL 
axis2_om_output_is_ignore_xml_declaration
                            (axis2_om_output_t *om_output,
                             const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(om_output)->ignore_xml_declaration;
}                               

axis2_status_t AXIS2_CALL 
axis2_om_output_set_ignore_xml_declaration
                            (axis2_om_output_t *om_output,
                            const axis2_env_t *env,
                            axis2_bool_t ignore_xml_dec)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_output)->ignore_xml_declaration = ignore_xml_dec;
    return AXIS2_SUCCESS;
} 
                            
axis2_status_t AXIS2_CALL 
axis2_om_output_set_soap11(axis2_om_output_t *om_output,
                           const axis2_env_t *env,
                           axis2_bool_t soap11)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_output)->is_soap11 = soap11;
    return AXIS2_SUCCESS;
}
                            
axis2_status_t AXIS2_CALL 
axis2_om_output_set_xml_version(axis2_om_output_t *om_output,
                                const axis2_env_t *env,
                                axis2_char_t *xml_version)
{
    axis2_om_output_impl_t *output_impl = NULL; 
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    AXIS2_PARAM_CHECK(env->error, xml_version, AXIS2_FAILURE); 
    output_impl = AXIS2_INTF_TO_IMPL(om_output);
    
    if(NULL !=  output_impl->xml_version)
    {
        AXIS2_FREE(env->allocator,  output_impl->xml_version);
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
                                const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_output)->xml_version;
}  
                            
axis2_status_t AXIS2_CALL 
axis2_om_output_set_char_set_encoding
                            (axis2_om_output_t *om_output,
                            const axis2_env_t *env,
                            axis2_char_t *char_set_encoding)
{
    axis2_om_output_impl_t *output_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, char_set_encoding, AXIS2_FAILURE); 
    output_impl = AXIS2_INTF_TO_IMPL(om_output);
    
    if(NULL != output_impl->char_set_encoding)
    {
        AXIS2_FREE(env->allocator, output_impl->char_set_encoding);
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
                            const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_output)->char_set_encoding;
} 
                            
axis2_status_t AXIS2_CALL 
axis2_om_output_set_do_optimize
                            (axis2_om_output_t *om_output,
                            const axis2_env_t *env,
                            axis2_bool_t optimize)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(om_output)->do_optimize = optimize;
    return AXIS2_SUCCESS;    
} 
                            
axis2_xml_writer_t* AXIS2_CALL
axis2_om_output_get_xml_writer
                            (axis2_om_output_t *om_output,
                            const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_output)->xml_writer;
}                                                                                         
                                                                                    
axis2_bool_t AXIS2_CALL
axis2_om_output_is_optimized(axis2_om_output_t *om_output,
                             const axis2_env_t *env)
{
    return AXIS2_INTF_TO_IMPL(om_output)->do_optimize;
}

axis2_char_t* AXIS2_CALL 
axis2_om_output_get_content_type(axis2_om_output_t *om_output,
                                 const axis2_env_t *env)
{
    axis2_om_output_impl_t *output_impl = NULL;
    axis2_char_t *soap_content_type = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    output_impl = AXIS2_INTF_TO_IMPL(om_output);    
    if(AXIS2_TRUE == output_impl->do_optimize)
    {
        if(AXIS2_TRUE == output_impl->is_soap11)
        {
            soap_content_type = AXIS2_SOAP11_CONTENT_TYPE;
        }
        else
        {
            soap_content_type = AXIS2_SOAP12_CONTENT_TYPE;
        }    

        return AXIS2_MIME_OUTPUT_GET_CONTENT_TYPE_FOR_MIME(
            output_impl->mime_output, 
            env, output_impl->mime_boundry,
            output_impl->root_content_id, output_impl->char_set_encoding, 
            soap_content_type);
    }
    else
    {
        if(AXIS2_TRUE == output_impl->is_soap11)
        {
            return AXIS2_SOAP11_CONTENT_TYPE;
        }
        else
        {
            return AXIS2_SOAP12_CONTENT_TYPE;
        }
    }
    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_om_output_write_optimized(axis2_om_output_t *om_output, 
                                const axis2_env_t *env, 
                                axis2_om_text_t *om_text)
{
    axis2_om_output_impl_t *om_output_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output_impl = AXIS2_INTF_TO_IMPL(om_output);
    if(NULL != om_output_impl->binary_node_list)
    {
        AXIS2_ARRAY_LIST_ADD(om_output_impl->binary_node_list, env, om_text);
    }
    else
    {
        om_output_impl->binary_node_list = axis2_array_list_create(env, 5);
        if(!(om_output_impl->binary_node_list))
            return AXIS2_FAILURE;
        AXIS2_ARRAY_LIST_ADD(om_output_impl->binary_node_list, env, om_text);    
    }  
    return AXIS2_SUCCESS;
}


axis2_char_t* AXIS2_CALL
axis2_om_output_get_next_content_id(axis2_om_output_t *om_output,
                        const axis2_env_t *env)
{
    axis2_om_output_impl_t *om_output_impl = NULL;
    axis2_char_t *uuid = NULL;
    axis2_char_t *temp_str = NULL;
    axis2_char_t *temp_str1 = NULL;
    axis2_char_t id[256];
    AXIS2_ENV_CHECK(env, NULL);
    
    om_output_impl = AXIS2_INTF_TO_IMPL(om_output);
    
    om_output_impl->next_id++;
    
    /** free existing id */
    if(NULL != om_output_impl->next_content_id)
    {
        AXIS2_FREE(env->allocator, om_output_impl->next_content_id);
        om_output_impl->next_content_id = NULL;
    }
    
    uuid = axis2_uuid_gen(env);
    if(!uuid)
        return NULL;

    sprintf(id, "%d", om_output_impl->next_id);                
    
    temp_str = AXIS2_STRACAT(id, ".", env);
    temp_str1 = AXIS2_STRACAT(temp_str, uuid, env);
    om_output_impl->next_content_id = AXIS2_STRACAT(temp_str1, "@apache.org", env);
    if(NULL != temp_str)
    {
        AXIS2_FREE(env->allocator, temp_str);
        temp_str = NULL;
    }          
    if(NULL != temp_str1)
    {
        AXIS2_FREE(env->allocator, temp_str1);
        temp_str1 = NULL;
    }
    if(NULL != uuid)
    {
        AXIS2_FREE(env->allocator, uuid);
        uuid = NULL;
    }
    return om_output_impl->next_content_id; 
}


axis2_char_t* AXIS2_CALL
axis2_om_output_get_root_content_id(axis2_om_output_t *om_output,
                                    const axis2_env_t *env)
{
    axis2_om_output_impl_t *om_output_impl = NULL;
    axis2_char_t *temp_str = NULL;
    axis2_char_t *uuid = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    
    om_output_impl = AXIS2_INTF_TO_IMPL(om_output);
    if(NULL == om_output_impl->root_content_id)
    {
        uuid = axis2_uuid_gen(env);

        temp_str = AXIS2_STRACAT("0.", uuid, env);

        om_output_impl->root_content_id = 
                AXIS2_STRACAT(temp_str, "@apache.org", env);        
        
        if(NULL != temp_str)
        {
            AXIS2_FREE(env->allocator, temp_str);
            temp_str = NULL;
        }                   
        if(NULL != uuid)
        {
            AXIS2_FREE(env->allocator, uuid);
            uuid = NULL;
        }            
    }
    
    return om_output_impl->root_content_id;
}                                    
                                    
axis2_char_t* AXIS2_CALL
axis2_om_output_get_mime_boundry(axis2_om_output_t *om_output,
                                 const axis2_env_t *env)
{
    axis2_om_output_impl_t *om_output_impl = NULL;
    axis2_char_t *uuid = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    om_output_impl = AXIS2_INTF_TO_IMPL(om_output);
    
    if(NULL == om_output_impl->mime_boundary)
    {
        uuid = axis2_uuid_gen(env);
        
        om_output_impl->mime_boundary = AXIS2_STRACAT("MIMEBoundary", uuid, env);
        if(NULL != uuid)
        {
            AXIS2_FREE(env->allocator, uuid);
            uuid = NULL;
        }
    }
    return om_output_impl->mime_boundary;
}                                 

/******************************************************************************/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_om_output_write (axis2_om_output_t * om_output,
                       const axis2_env_t *env,
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
                                           const axis2_env_t *env)
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

axis2_byte_t* AXIS2_CALL 
axis2_om_output_flush(axis2_om_output_t *om_output,
                      const axis2_env_t *env,
                      axis2_byte_t **output_stream,
                      int *output_stream_size)
{
    axis2_om_output_impl_t *output_impl = NULL;
    axis2_char_t *soap_content_type = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    output_impl = AXIS2_INTF_TO_IMPL(om_output);

    if (output_impl->do_optimize)
    {
        axis2_byte_t* byte_stream = NULL;
        axis2_char_t *root_content_id = NULL;
        axis2_char_t *buffer = (axis2_char_t*)AXIS2_XML_WRITER_GET_XML(output_impl->xml_writer, env);
        int stream_size = 0;
        if (output_impl->is_soap11)
        {
            soap_content_type = AXIS2_SOAP11_CONTENT_TYPE;
        }
        else
        {
            soap_content_type = AXIS2_SOAP12_CONTENT_TYPE;
        }
        output_impl->mime_output = axis2_mime_output_create(env);
        output_impl->mime_boundry = axis2_om_output_get_mime_boundry(om_output, env);
        root_content_id = axis2_om_output_get_root_content_id(om_output, env);
        AXIS2_MIME_OUTPUT_COMPLETE(output_impl->mime_output, env, &byte_stream, &stream_size,
            buffer, output_impl->binary_node_list, output_impl->mime_boundry, 
            output_impl->root_content_id, output_impl->char_set_encoding,
            soap_content_type); 

        *output_stream = byte_stream;
        *output_stream_size = stream_size;

        return byte_stream;
    }
    return NULL;
}
