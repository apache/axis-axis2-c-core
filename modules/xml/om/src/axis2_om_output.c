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
#include <axis2_xml_writer.h>

#define   DEFAULT_CHAR_SET_ENCODING  "utf-8"
#define MAX_ARGS  4

struct axis2_om_output
{
    /** axis2_xml_writer. any xml writer which 
        implemet axis2_xml_writer.h interface  */
    axis2_xml_writer_t *xml_writer;

    /* following fields are not used currently but will be used in the future */
    axis2_bool_t do_optimize;
    axis2_char_t *mime_boundary;
    axis2_char_t *root_content_id;
    int next_id;
    axis2_bool_t is_soap_11;
    axis2_char_t *char_set_encoding;
    /* xml version */
    axis2_char_t *xml_version;
    axis2_bool_t ignore_xml_declaration;
};


AXIS2_DECLARE(axis2_om_output_t *)
axis2_om_output_create (axis2_env_t **env, axis2_xml_writer_t *xml_writer)
{
    axis2_om_output_t *om_output = NULL;
    AXIS2_FUNC_PARAM_CHECK(xml_writer, env, NULL);
    
    om_output = (axis2_om_output_t *) AXIS2_MALLOC ((*env)->allocator,
                                                    sizeof (axis2_om_output_t));

    if (!om_output)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);

    om_output->xml_writer = NULL;
    om_output->xml_writer = xml_writer;
   
    om_output->do_optimize = AXIS2_FALSE;
    om_output->mime_boundary = 0;
    om_output->root_content_id = 0;
    om_output->next_id = 0;
    om_output->is_soap_11 = AXIS2_TRUE;
    om_output->char_set_encoding = NULL;
    om_output->xml_version = NULL;
    om_output->ignore_xml_declaration = AXIS2_TRUE;

    return om_output;
}

AXIS2_DECLARE(int)
axis2_om_output_write (axis2_om_output_t * om_output,
                       axis2_env_t **env,
                       axis2_om_types_t type,
                       int no_of_args, ...)
{
    int status = AXIS2_SUCCESS;
    axis2_char_t *args_list[MAX_ARGS];
    int i = 0;
    va_list ap;

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
                AXIS2_XML_WRITER_WRITE_END_ELEMENT(om_output->xml_writer, env);
            break;
        case 1:
            status = AXIS2_XML_WRITER_WRITE_START_ELEMENT(om_output->xml_writer,
                                                          env, args_list[0]);
        break;
        case 2:
            status = AXIS2_XML_WRITER_WRITE_START_ELEMENT_WITH_NAMESPACE(
                                                          om_output->xml_writer,
                                                          env, 
                                                          args_list[0],
                                                          args_list[1]);
        break;
        case 3:
            status = AXIS2_XML_WRITER_WRITE_START_ELEMENT_WITH_NAMESPACE_PREFIX(
                                                          om_output->xml_writer,
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
            status = AXIS2_XML_WRITER_WRITE_ATTRIBUTE(om_output->xml_writer,
                                                      env,
                                                      args_list[0],
                                                      args_list[1]);
        break;
        case 3:
            status = AXIS2_XML_WRITER_WRITE_ATTRIBUTE_WITH_NAMESPACE(
                                                      om_output->xml_writer,env,
                                                      args_list[0],
                                                      args_list[1],
                                                      args_list[2]);
        break;
        case 4:
            status = AXIS2_XML_WRITER_WRITE_ATTRIBUTE_WITH_NAMESPACE_PREFIX(
                                                     om_output->xml_writer, env,
                                                      args_list[0],
                                                      args_list[1],
                                                      args_list[2],
                                                      args_list[3]);
        
         break;

        }
        break;

    case AXIS2_OM_NAMESPACE:
        status = AXIS2_XML_WRITER_WRITE_NAMESPACE(om_output->xml_writer, env,
                                                  args_list[0],
                                                  args_list[1]);
        break;
    case AXIS2_OM_TEXT:
        status = AXIS2_XML_WRITER_WRITE_CHARACTERS(om_output->xml_writer, env,
                                                  args_list[0]);
        break;
    case AXIS2_OM_COMMENT:
        status = AXIS2_XML_WRITER_WRITE_COMMENT(om_output->xml_writer, env,
                                                args_list[0]);
        break;
    case AXIS2_OM_PROCESSING_INSTRUCTION:
        switch(no_of_args)
        {
         case 1:
            
            status = AXIS2_XML_WRITER_WRITE_PROCESSING_INSTRUCTION(
                        om_output->xml_writer, env, args_list[0]);
            break;
         case 2:
             
            status = AXIS2_XML_WRITER_WRITE_PROCESSING_INSTRUCTION_DATA(
                        om_output->xml_writer, env, args_list[0], args_list[1]);        
            break;
        }
         break;
    case AXIS2_OM_DOCTYPE:
        status = AXIS2_XML_WRITER_WRITE_DTD(om_output->xml_writer, 
                                            env, args_list[0]);
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


 AXIS2_DECLARE(axis2_status_t)
 axis2_om_output_free(axis2_om_output_t *om_output,
                         axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(om_output, env, AXIS2_FAILURE);
    if(om_output->xml_version)
        AXIS2_FREE((*env)->allocator, om_output->xml_version);
    if(om_output->char_set_encoding)
        AXIS2_FREE((*env)->allocator, om_output->char_set_encoding);
    if(om_output->xml_writer)
        AXIS2_XML_WRITER_FREE(om_output->xml_writer, env);
    AXIS2_FREE ((*env)->allocator, om_output);
    return AXIS2_SUCCESS;
}
