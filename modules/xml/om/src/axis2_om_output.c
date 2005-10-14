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
#include <guththila_xml_stream_writer.h>

axis2_om_output_t *
axis2_create_om_output (axis2_environment_t *environment, void* xml_writer)
{
    axis2_om_output_t *om_output =
        (axis2_om_output_t *) axis2_malloc (environment->allocator, sizeof (axis2_om_output_t));

    if (!om_output)
    {
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;        
		return NULL;
    }
	
	om_output->xml_writer = NULL;
	if (xml_writer)
		om_output->xml_writer = xml_writer;
	else
		om_output->xml_writer = guththila_create_xml_stream_writer (stdout,
                                            DEFAULT_CHAR_SET_ENCODING, AXIS2_TRUE);
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

int
axis2_om_output_write (axis2_environment_t *environment, axis2_om_output_t * om_output, axis2_om_types_t type,
                       int no_of_args, ...)
{
    int status = AXIS2_SUCCESS;
    axis2_char_t *args_list[no_of_args];
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
            status = guththila_xml_stream_writer_write_end_element (om_output->
                                                           xml_writer);
            break;
        case 1:
            status = guththila_xml_stream_writer_write_start_element (om_output->
                                                             xml_writer,
                                                             args_list[0]);
            break;
        case 2:
            status = guththila_xml_stream_writer_write_start_element_with_namespace
                (om_output->xml_writer, args_list[0], args_list[1]);
            break;
        case 3:
            status = guththila_xml_stream_writer_write_start_element_with_namespace_prefix
                (om_output->xml_writer, args_list[0], args_list[1],
                 args_list[2]);
            break;
        }
        break;

    case AXIS2_OM_ATTRIBUTE:
        switch (no_of_args)
        {
            case 2:
                status = guththila_xml_stream_writer_write_attribute(om_output->xml_writer, args_list[0], args_list[1]);
                break;
            case 3:
                status = guththila_xml_stream_writer_write_attribute_with_namespace(om_output->xml_writer, args_list[0], args_list[1], args_list[2]);
                break;
            case 4:
                status = guththila_xml_stream_writer_write_attribute_with_namespace_prefix(om_output->xml_writer, args_list[0], args_list[1], args_list[2], args_list[3]);
                break;

        }
        break;

        case AXIS2_OM_NAMESPACE:
            status = guththila_xml_stream_writer_write_namespace(om_output->xml_writer, args_list[0], args_list[1]);
            break;
        case AXIS2_OM_TEXT:
            status = guththila_xml_stream_writer_write_characters(om_output->xml_writer, args_list[0]);
            break;
        case AXIS2_OM_DOCUMENT:
            break;
        case AXIS2_OM_DOCTYPE:
            break;
        case AXIS2_OM_COMMENT:
            break;
        case AXIS2_OM_PROCESSING_INSTRUCTION:
            break;
        default:
            break;
    };

    if (status == GUTHTHILA_SUCCESS)
    {
        return AXIS2_SUCCESS;
    }
    else 
        return AXIS2_FAILURE;
}
