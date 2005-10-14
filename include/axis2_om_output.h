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

#include <axis2_defines.h>
#include <axis2_environment.h>
#include <axis2_om_node.h>



static const char* DEFAULT_CHAR_SET_ENCODING = "utf-8";

struct axis2_om_output {
    void *xml_writer;
    axis2_bool_t do_optimize;
    axis2_char_t *mime_boundary;
    axis2_char_t *root_content_id;
    int next_id;
    axis2_bool_t is_soap_11;
    axis2_char_t *char_set_encoding;
    axis2_char_t *xml_version;
    axis2_bool_t ignore_xml_declaration;
};

axis2_om_output_t* axis2_create_om_output(axis2_environment_t *environment, void* xml_writer);

axis2_status_t axis2_om_output_write(axis2_environment_t *environment, axis2_om_output_t *om_output, axis2_om_types_t type, int no_of_args, ... );

#endif /* AXIS2_OM_OUTPUT_H */
