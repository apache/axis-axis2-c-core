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

#ifndef AXIS2_OM_OUTPUT_H
#define AXIS2_OM_OUTPUT_H

#include <guththila_xml_stream_writer.h>
#include <axis2_om_node.h>

#include <stdarg.h>

static const char* DEFAULT_CHAR_SET_ENCODING = "utf-8";

typedef struct {
    guththila_xml_stream_writer_t* xml_writer;
    int do_optimize;
    FILE* out_stream;
    //apr_array_t binary_node_list;
    //buffered_soap_out_stream;
    char* mime_boundary;
    char*  root_content_id;
    int next_id;
    int is_soap_11;
    char* char_set_encoding;
    char* xml_version;
    int ignore_xml_declaration;
} axis2_om_output_t;

axis2_om_output_t* axis2_create_om_output( FILE* stream);

int axis2_om_output_write(axis2_om_output_t* om_output, axis2_om_types_t type, int no_of_args, ... );

#endif // AXIS2_OM_OUTPUT_H
