/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
/*
 * This example simply prints out most of the WSDL elements encountered
 * demonstrates the WSDL pull API
 */
#include <axis2_wsdl4c_parser.h>
#include <axis2_wsdl4c_operation.h>
#include <axis2_wsdl4c_port_type.h>
#include <axis2_wsdl4c_message.h>
#include <axutil_array_list.h>
#include <axutil_env.h>
#include <axutil_allocator.h>
#include <stdio.h>

extern const void *
    axis2_wsdl4c_parser_get_port_type(void *parser);

extern int
    axis2_wsdl4c_port_type_get_num_ops(void *port_type);

extern axutil_array_list_t *
    axis2_wsdl4c_port_type_get_operations(void *port_type);

extern const void *
    axis2_wsdl4c_operation_get_message(void *operation,
            axis2_wsdl4c_msg_type_t type);

extern axutil_array_list_t *
    axis2_wsdl4c_operation_get_faults(void *operation);

extern char *
    axis2_wsdl4c_port_type_get_name(void *operation);

extern char *
    axis2_wsdl4c_operation_get_name(void *operation);

extern char *
    axis2_wsdl4c_msg_get_name(void *message);

int
main(int argc, char *argv[])
{
    axutil_allocator_t *allocator = axutil_allocator_init(NULL);
    const axutil_env_t *env = axutil_env_create(allocator);
    if (argc <= 1)
    {
        printf("Usage: print <wsdl_File_Name>\n");
        return 1;
    }
    void *wp = axis2_wsdl4c_parser_create(argv[1], "");
    while (axis2_wsdl4c_parser_get_event_type(wp) != AXIS2_WSDL4C_PARSER_END)
    {
        switch (axis2_wsdl4c_parser_get_next_element(wp))
        {
                axutil_array_list_t *msg_list = NULL;
                void * port_type = NULL;
                int i = 0;
                int size = 0;
                void *message = NULL;

            case AXIS2_WSDL4C_PARSER_DOCUMENTATION:
                printf("%s\n", axis2_wsdl4c_parser_get_documentation(wp));
                break;
            case AXIS2_WSDL4C_PARSER_TYPES:
                printf("%d schema(s) found \n", axis2_wsdl4c_parser_get_num_schemas(wp));
                break;
            case AXIS2_WSDL4C_PARSER_MESSAGE:
                message = axis2_wsdl4c_parser_get_message(wp);
                printf("Message  :%s\n", axis2_wsdl4c_msg_get_name(message));
                break;
            case AXIS2_WSDL4C_PARSER_PORT_TYPE:
                port_type = axis2_wsdl4c_parser_get_port_type(wp);
                printf("Port Type %s ", axis2_wsdl4c_port_type_get_name(port_type));
                printf("has :%d operations \n", axis2_wsdl4c_port_type_get_num_ops(port_type));
                msg_list = axis2_wsdl4c_port_type_get_operations(port_type);
                size = axutil_array_list_size(msg_list, env);
                printf("size:%d\n", size);
                for (i = 0; i < size; i++)
                {
                    const char *op_name = NULL;
                    const char *in_msg_name = NULL;
                    const char *out_msg_name = NULL;
                    axutil_array_list_t *fault_list = NULL;

                    void *op = axutil_array_list_get(msg_list, env, i);
                    if (!op)
                    {
                        return -1;
                    }
                    void *in_msg = axis2_wsdl4c_operation_get_message(op, AXIS2_WSDL4C_INPUT);
                    void *out_msg = axis2_wsdl4c_operation_get_message(op, AXIS2_WSDL4C_OUTPUT);
                    fault_list = axis2_wsdl4c_operation_get_faults(op);

                    op_name = axis2_wsdl4c_operation_get_name(op);
                    printf("Operation Name:%s\n", op_name);
                    if (in_msg)
                    {
                        in_msg_name = axis2_wsdl4c_msg_get_name(in_msg);
                        printf("Input:%s\n", in_msg_name);
                    }
                    if (out_msg)
                    {
                        out_msg_name = axis2_wsdl4c_msg_get_name(out_msg);
                        printf("Output:%s\n", out_msg_name);
                    }
                    if (fault_list)
                    {
                        int j = 0, sizej = 0;

                        sizej = axutil_array_list_size(fault_list, env);
                        printf("sizej:%d\n", sizej);
                        for (j = 0; j < sizej; j++)
                        {
                            char *fault_msg_name = NULL;

                            void *fault_msg = axutil_array_list_get(fault_list, env, j);
                            /*fault_msg_name = axis2_wsdl4c_message_get_name(fault_msg);
                            printf("Fault message name:%s\n", fault_msg_name);*/
                        }
                    }
                }
                break;
        }
    }
    if (!axis2_wsdl4c_parser_status(wp))
        return 1;
    return 0;
}


