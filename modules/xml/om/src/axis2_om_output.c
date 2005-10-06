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


axis2_om_output_t *
axis2_create_om_output (FILE * stream)
{
    axis2_om_output_t *om_output =
        (axis2_om_output_t *) malloc (sizeof (axis2_om_output_t));

    if (!om_output)
        return 0;

    om_output->out_stream = stream;
    om_output->xml_writer =
        guththila_create_xml_stream_writer (om_output->out_stream,
                                            DEFAULT_CHAR_SET_ENCODING, 1);
    om_output->do_optimize = 0;
    om_output->mime_boundary = 0;
    om_output->root_content_id = 0;
    om_output->next_id = 0;
    om_output->is_soap_11 = 1;
    om_output->char_set_encoding = 0;
    om_output->xml_version = 0;
    om_output->ignore_xml_declaration = 0;

    return om_output;
}

int
axis2_om_output_write (axis2_om_output_t * om_output, axis2_om_types_t type,
                       int no_of_args, ...)
{
    int status = AXIS2_SUCCESS;
    char *args_list[no_of_args];
    int i = 0;
    va_list ap;

    va_start (ap, no_of_args);
    for (i = 0; i < no_of_args; i++)
    {
        args_list[i] = va_arg (ap, char *);
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
        /*case AXIS2_OM_DOCUMENT:
           b= 10,
           = 20,
           AXIS2_OM_DOCTYPE = 30,
           AXIS2_OM_COMMENT = 40,

           AXIS2_OM_NAMESPACE = 60,
           AXIS2_OM_PROCESSING_INSTRUCTION = 70,
           AXIS2_OM_TEXT = 80 */
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
