
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

#include <axutil_utils.h>
#include <axutil_uuid_gen.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axutil_thread_pool.h>
#include <tcpmon_session.h>
#include <tcpmon_entry.h>
#include <tcpmon_util.h>
#include <signal.h>
#include <stdio.h>
#include <axutil_stream.h>
#include <axutil_network_handler.h>
#include <axis2_http_transport.h>
#include <axutil_version.h>

#include <tcpmon_util.h>

#define SIZE 1024
axis2_char_t *tcpmon_traffic_log = "tcpmon_traffic.log";
axutil_env_t *system_env = NULL;
tcpmon_session_t *session = NULL;
char *target_host = NULL;

int on_new_entry(
    const axutil_env_t * env,
    tcpmon_entry_t * entry,
    int status);

int on_new_entry_to_file(
    const axutil_env_t * env,
    tcpmon_entry_t * entry,
    int status);

int on_error_func(
    const axutil_env_t * env,
    char *error_message);


void sig_handler(
    int signal);

int resend_request(
    const axutil_env_t * env,
    int status);

int
main(
    int argc,
    char **argv)
{
    axutil_env_t *env = NULL;
    int c;
    int listen_port = 9099,
        target_port = 9090; /* the default port simple axis2 server is run on 9090  */
    int test_bit = 0;
    int format_bit = 0;  /* pretty print the request/response SOAP messages */
    int ii = 1;

    if (!axutil_strcmp(argv[1], "-h"))
    {
        printf
            ("Usage : %s [-lp LISTEN_PORT] [-tp TARGET_PORT] [-th TARGET_HOST] [-f LOG_FILE] [options]\n",
             argv[0]);
        fprintf(stdout, " Options :\n");
        fprintf(stdout,
                "\t-lp LISTEN_PORT \tport number to listen on, default is 9099\n");
        fprintf(stdout,
                "\t-tp TARGET_PORT \tport number to connect and re-direct messages, default is 9090\n");
        fprintf(stdout,
                "\t-th TARGET_HOST \ttarget host to connect, default is localhost\n");
        fprintf(stdout,
                "\t-f  LOG_FILE    \tfile to write the messages to, default is %s\n",
                tcpmon_traffic_log);
        fprintf(stdout,
                "\t--format        \tenable xml formatting\n");
        fprintf(stdout,
                "\t--test          \tenable testing last request/response by logging it separately\n");
        fprintf(stdout, " Help :\n\t-h              \tdisplay this help screen.\n\n");
        return 0;
    }

    env = axutil_env_create_all("axis2_tcpmon.log", AXIS2_LOG_LEVEL_DEBUG);

    signal(SIGINT, sig_handler);
    system_env = env;
#ifndef WIN32
    signal(SIGPIPE, sig_handler);
#endif

    target_host = axutil_strdup(env, "localhost"); 

    while (ii < argc)
    {
        if (!strcmp("-lp", argv[ii]))
        {
            ii++;
            if (argv[ii])
            {
                listen_port = atoi(argv[ii++]);
                if (listen_port == 0)
                {
                    printf("INVALID value for listen port\n");
                    printf("Use -h for help\n");
                    AXIS2_FREE(env->allocator, target_host);
                    if (env)
                    {
                        axutil_env_free((axutil_env_t *) env);
                        env = NULL;
                    }
                    return 0;
                }
            }
        }
        else if (!strcmp("-tp", argv[ii]))
        {
            ii++;
            if (argv[ii])
            {
                target_port = atoi(argv[ii++]);
                if (target_port == 0)
                {
                    printf("INVALID value for target port\n");
                    printf("Use -h for help\n");
                    AXIS2_FREE(env->allocator, target_host);
                    if (env)
                    {
                        axutil_env_free((axutil_env_t *) env);
                        env = NULL;
                    }
                    return 0;
                }
            }
        }
        else if (!strcmp("-th", argv[ii]))
        {
            ii++;
            if (argv[ii])
            {
                AXIS2_FREE(env->allocator, target_host);
                target_host = (char *) axutil_strdup(env, argv[ii++]);
            }
        }
        else if (!strcmp("--test", argv[ii]))
        {
            ii++;
            test_bit = 1;
        }
        else if (!strcmp("--format", argv[ii]))
        {
            ii++;
            format_bit = 1;
        }
        else if (!strcmp("-f", argv[ii]))
        {
            ii++;
            if (argv[ii])
            {
                tcpmon_traffic_log = argv[ii++];
            }
        }
        else
        {
            printf("INVALID value for tcpmon \n");
            printf("Use -h for help\n");
            AXIS2_FREE(env->allocator, target_host);
            if (env)
            {
                axutil_env_free((axutil_env_t *) env);
                env = NULL;
            }
            return 0;
        }
    }

    if (!(listen_port && target_port && target_host))
    {
        printf("ERROR: essential argument missing \n");
        printf
            ("Please recheck values of listen_port (-lp), target_port(-tp) and target_host (-th)\n");
        AXIS2_FREE(env->allocator, target_host);
        if (env)
        {
            axutil_env_free((axutil_env_t *) env);
            env = NULL;
        }
        return 0;
    }

    printf("Listen port : %d Target port : %d Target host: %s\n",
           listen_port, target_port, target_host);
    session = tcpmon_session_create(env);

    TCPMON_SESSION_SET_LISTEN_PORT(session, env, listen_port);
    TCPMON_SESSION_SET_TARGET_PORT(session, env, target_port);
    TCPMON_SESSION_SET_TARGET_HOST(session, env, target_host);
    TCPMON_SESSION_ON_TRANS_FAULT(session, env, on_error_func);

    TCPMON_SESSION_ON_NEW_ENTRY(session, env, on_new_entry_to_file);

    TCPMON_SESSION_SET_TEST_BIT(session, env, test_bit);
    TCPMON_SESSION_SET_FORMAT_BIT(session, env, format_bit);
    TCPMON_SESSION_START(session, env);

    do
    {
        c = getchar();
        if (c == 'f')
        {
            format_bit = format_bit ? 0 : 1;
            TCPMON_SESSION_SET_FORMAT_BIT(session, env, format_bit);
        }
        else if (c == 'r')
        {
            resend_request(env, 0);
        }
    }
    while (c != 'q');
    printf("\n\n");

    TCPMON_SESSION_STOP(session, env);
    TCPMON_SESSION_FREE(session, env);
    AXIS2_FREE(env->allocator, target_host);
    if (env)
    {
        axutil_env_free((axutil_env_t *) env);
        env = NULL;
    }
    return 0;
}

int
on_new_entry_to_file(
    const axutil_env_t * env,
    tcpmon_entry_t * entry,
    int status)
{
    char *plain_buffer = NULL;
    char *formated_buffer = NULL;
    int format = 0;
    FILE *file;
    char *convert = NULL;
    char *uuid = NULL;
    int resend = 0;

    file = fopen(tcpmon_traffic_log, "ab");

    if (NULL == file)
    {
        printf("\ncould not create or open log-file\n");
        return -1;
    }

    fprintf(file,
            "\n= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =\n");

    format = TCPMON_ENTRY_GET_FORMAT_BIT(entry, env);

    if (status == 0)
    {
        if (strstr(TCPMON_ENTRY_SENT_HEADERS(entry, env), AXIS2_HTTP_HEADER_USER_AGENT
                ": " AXIS2_HTTP_HEADER_SERVER_AXIS2C "TCPMon\r\n"))
        {
            resend = 1;
        }
        plain_buffer = TCPMON_ENTRY_SENT_DATA(entry, env);
        if (plain_buffer)       /* this can be possible as no xml present */
        {
            if (TCPMON_ENTRY_GET_SENT_DATA_LENGTH(entry, env) !=
                (int)strlen(TCPMON_ENTRY_SENT_HEADERS(entry, env)) +
                (int)strlen(plain_buffer) + 4)
            {
                format = 0; /* mtom scenario */
            }
            formated_buffer = tcpmon_util_format_as_xml
                (env, plain_buffer, format);
        }
        else
        {
            formated_buffer = "";
        }
        /* 2 screen */
        printf("\n\n%s\n", resend ? "RESENDING DATA..": "SENDING DATA..");
        printf("/* sending time = %s*/\n", TCPMON_ENTRY_SENT_TIME(entry, env));
        uuid = axutil_uuid_gen(env);
        printf("/* message uuid = %s*/\n", uuid);
        printf("---------------------\n");

        if (format || TCPMON_ENTRY_GET_SENT_DATA_LENGTH(entry, env) ==
            (int)strlen(TCPMON_ENTRY_SENT_HEADERS(entry, env)) +
            (int)strlen(formated_buffer) + 4)
        {
            printf("%s\n\n%s\n\n", TCPMON_ENTRY_SENT_HEADERS(entry, env),
                   formated_buffer);
        }
        else
        {
            int count = 0;
            int printed = 0;
            axis2_char_t *formated_buffer_temp = formated_buffer;
            printf("%s\n\n", TCPMON_ENTRY_SENT_HEADERS(entry, env));
            count = TCPMON_ENTRY_GET_SENT_DATA_LENGTH(entry, env) - 4 -
                    (int)strlen(TCPMON_ENTRY_SENT_HEADERS(entry, env));
            while (count > printed)
            {
                int plen = 0;
                plen = ((int)strlen(formated_buffer) + 1);
                if (plen != 1)
                {
                    printf("%s", formated_buffer);
                }
                printed += plen;
                if (count > printed)
                {
                    printf("%c", '\0');
                    formated_buffer += plen;
                }
            }
            formated_buffer = formated_buffer_temp;
        }

        /* 2 file */
        fprintf(file, "%s\n", resend ? "RESENDING DATA..": "SENDING DATA..");
        fprintf(file, "/* sending time = %s*/\n",
                TCPMON_ENTRY_SENT_TIME(entry, env));
        fprintf(file, "/* message uuid = %s*/\n", uuid);
        AXIS2_FREE(env->allocator, uuid);
        fprintf(file, "---------------------\n");

        convert = axutil_strdup(env, TCPMON_ENTRY_SENT_HEADERS(entry, env));
        convert = tcpmon_util_str_replace(env, convert, "; ", ";\n\t");
        fprintf(file, "%s\r\n\r\n", convert);
        if (convert)
        {
            AXIS2_FREE(env->allocator, convert);
            convert = NULL;
        }
        if (strcmp(formated_buffer, "") != 0)
        {
            if (format || TCPMON_ENTRY_GET_SENT_DATA_LENGTH(entry, env) ==
                (int)strlen(TCPMON_ENTRY_SENT_HEADERS(entry, env)) +
                (int)strlen(formated_buffer) + 4)
            {
                convert = axutil_strdup(env, formated_buffer);
                convert = tcpmon_util_str_replace(env, convert, "><", ">\n<");
                fprintf(file, "%s", convert);
                if (convert)
                {
                    AXIS2_FREE(env->allocator, convert);
                    convert = NULL;
                }
            }
            else
            {
                int count = 0;
                int printed = 0;
                count = TCPMON_ENTRY_GET_SENT_DATA_LENGTH(entry, env) - 4 -
                        (int)strlen(TCPMON_ENTRY_SENT_HEADERS(entry, env));
                while (count > printed)
                {
                    int plen = 0;
                    plen = ((int)strlen(formated_buffer) + 1);
                    if (plen != 1)
                    {
                        fprintf(file, "%s", formated_buffer);
                    }
                    printed += plen;
                    if (count > printed)
                    {
                        fprintf(file, "%c", '\0');
                        formated_buffer += plen;
                    }
                }
            }
        }
    }
    if (status == 1)
    {
        plain_buffer = TCPMON_ENTRY_ARRIVED_DATA(entry, env);
        if (plain_buffer)       /* this can be possible as no xml present */
        {
            if (TCPMON_ENTRY_GET_ARRIVED_DATA_LENGTH(entry, env) !=
                (int)strlen(TCPMON_ENTRY_ARRIVED_HEADERS(entry, env)) +
                (int)strlen(plain_buffer) + 4)
            {
                format = 0; /* mtom scenario */
            }
            formated_buffer = tcpmon_util_format_as_xml
                (env, plain_buffer, format);
        }
        else
        {
            formated_buffer = "";
        }
        /* 2 screen */
        printf("\n\n%s\n", "RETRIEVING DATA..");
        printf("/* retrieving time = %s*/\n",
               TCPMON_ENTRY_ARRIVED_TIME(entry, env));
        printf("/* time throughput = %s*/\n",
               TCPMON_ENTRY_TIME_DIFF(entry, env));
        printf("---------------------\n");

        if (format || TCPMON_ENTRY_GET_ARRIVED_DATA_LENGTH(entry, env) ==
            (int)strlen(TCPMON_ENTRY_ARRIVED_HEADERS(entry, env)) +
            (int)strlen(formated_buffer) + 4)
        {
            printf("%s\n\n%s\n\n", TCPMON_ENTRY_ARRIVED_HEADERS(entry, env),
                   formated_buffer);
        }
        else
        {
            int count = 0;
            int printed = 0;
            axis2_char_t *formated_buffer_temp = formated_buffer;
            printf("%s\n\n", TCPMON_ENTRY_ARRIVED_HEADERS(entry, env));
            count = TCPMON_ENTRY_GET_ARRIVED_DATA_LENGTH(entry, env) - 4 -
                    (int)strlen(TCPMON_ENTRY_ARRIVED_HEADERS(entry, env));
            while (count > printed)
            {
                int plen = 0;
                plen = ((int)strlen(formated_buffer) + 1);
                if (plen != 1)
                {
                    printf("%s", formated_buffer);
                }
                printed += plen;
                if (count > printed)
                {
                    printf("%c", '\0');
                    formated_buffer += plen;
                }
            }
            formated_buffer = formated_buffer_temp;
        }

        /* 2 file */
        fprintf(file, "%s\n", "RETRIEVING DATA..");
        fprintf(file, "/* retrieving time = %s*/\n",
                TCPMON_ENTRY_ARRIVED_TIME(entry, env));
        fprintf(file, "/* time throughput = %s*/\n",
                TCPMON_ENTRY_TIME_DIFF(entry, env));
        fprintf(file, "---------------------\n");

        convert = axutil_strdup(env, TCPMON_ENTRY_ARRIVED_HEADERS(entry, env));
        convert = tcpmon_util_str_replace(env, convert, "; ", ";\n\t");
        fprintf(file, "%s\r\n\r\n", convert);
        if (convert)
        {
            AXIS2_FREE(env->allocator, convert);
            convert = NULL;
        }
        if (strcmp(formated_buffer, "") != 0)
        {
            if (format || TCPMON_ENTRY_GET_ARRIVED_DATA_LENGTH(entry, env) ==
                (int)strlen(TCPMON_ENTRY_ARRIVED_HEADERS(entry, env)) +
                (int)strlen(formated_buffer) + 4)
            {
                convert = axutil_strdup(env, formated_buffer);
                convert = tcpmon_util_str_replace(env, convert, "><", ">\n<");
                fprintf(file, "%s", convert);
                if (convert)
                {
                    AXIS2_FREE(env->allocator, convert);
                    convert = NULL;
                }
            }
            else
            {
                int count = 0;
                int printed = 0;
                count = TCPMON_ENTRY_GET_ARRIVED_DATA_LENGTH(entry, env) - 4 -
                        (int)strlen(TCPMON_ENTRY_ARRIVED_HEADERS(entry, env));
                while (count > printed)
                {
                    int plen = 0;
                    plen = ((int)strlen(formated_buffer) + 1);
                    if (plen != 1)
                    {
                        fprintf(file, "%s", formated_buffer);
                    }
                    printed += plen;
                    if (count > printed)
                    {
                        fprintf(file, "%c", '\0');
                        formated_buffer += plen;
                    }
                }
            }
        }
    }
    fclose(file);
    return 0;
}

int 
resend_request(
    const axutil_env_t * env,
    int status)
{
    FILE *file;
    axis2_char_t *uuid = NULL;
    axis2_char_t *buffer = NULL;
    int read_len = 0;

    if (status == 0)
    {
        int c;
        int i = 0;
        do
        {
            c = getchar();
        }
        while (c == ' ');
        uuid = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * 37);
        for (i = 0; i < 36; i++)
        {
            if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || c == '-')
            {
                uuid[i] = (axis2_char_t)c;
            }
            else if (c >= 'A' && c <= 'F')
            {
                uuid[i] = (axis2_char_t)(c + 32);
            }
            else
            {
                return 0;
            }
            c = getchar();
        }
        uuid[i] = '\0';
    }

    file = fopen(tcpmon_traffic_log, "rb");

    if (!file)
    {
        printf("\ncould not create or open log-file\n");
        return -1;
    }

    buffer = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * SIZE);
    if (!buffer)
    {
        printf("\ngimme more memory\n");
        return -1;
    }
    buffer[SIZE - 1] = '\0';

    read_len = (int)fread(buffer, sizeof(char), SIZE - 1, file);

    while(read_len)
    {
        axis2_char_t *search = "/* message uuid = ";
        axis2_char_t *tmp1 = NULL;
        axis2_char_t *tmp2 = NULL;
        axis2_char_t *tmp3 = NULL;
        axis2_char_t *uuid_match = NULL;
        int offset = 0;
        int loop_state = 1;
        int end_reached = 0;
        int rounds = 0;

        offset = (int)strlen(search);
        tmp3 = buffer;
        if (read_len >= SIZE)
        {
            AXIS2_FREE(env->allocator, buffer);
            AXIS2_FREE(env->allocator, uuid);
            printf("\nbuffer overflow\n");
            return -1;
        }
        if (read_len < SIZE - 1)
        {   
            end_reached = 1;
        }
        while (loop_state)
        {
            int temp_len = 0;
            tmp1 = strstr(tmp3, search);
            temp_len = (int)strlen(tmp3) + 1;
            /* loop below is for mtom cases */
            while (!tmp1 && (read_len - rounds > temp_len))
            {
                tmp3 += (int)strlen(tmp3) + 1;
                tmp1 = strstr(tmp3, search);
                temp_len += (int)strlen(tmp3) + 1;
            }
            if (!tmp1)
            {
                if (end_reached)
                {
                    break;
                }
                memmove(buffer, buffer + (SIZE - 1 - offset), offset);
                read_len = (int)fread(buffer + offset, sizeof(char),
                                 SIZE - 1 - offset, file) + offset;
                break;
            }
            else
            {
                rounds = (int)(tmp1 - tmp3) + offset + 36;
                tmp3 = tmp1 + offset + 36;
            }
            if (read_len - offset - 36 < (int)(tmp1 - buffer))
            {
                if (end_reached)
                {
                    break;
                }
                offset += 36;
                memmove(buffer, buffer + (SIZE - 1 - offset), offset);
                read_len = (int)fread(buffer + offset, sizeof(char),
                                 SIZE - 1 - offset, file) + offset;
                break;
            }
            tmp2 = tmp1 + offset;
            uuid_match = AXIS2_MALLOC(env->allocator,
                                      sizeof(axis2_char_t) * 37);
            if (!uuid_match)
            {
                printf("\ngimme more memory\n");
                return -1;
            }
            memcpy(uuid_match, tmp2, 36);
            uuid_match[36] = '\0';
            if (!axutil_strcasecmp(uuid_match, uuid))
            {
                axis2_char_t *header_str = "*/\n---------------------\n";
                axis2_char_t *footer_str =
                    "\n= = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =";
                int seek_len = 0;
                int has_raw_binary = 0;
                axis2_char_t *request_buffer = NULL;
                AXIS2_FREE(env->allocator, uuid_match);
                AXIS2_FREE(env->allocator, uuid);
                end_reached = 1;
                tmp2 += 36;
                offset = (int)strlen(header_str);
                if (read_len - offset < (int)(tmp2 - buffer))
                {
                    seek_len = (int)(tmp2 - buffer) + offset - read_len;
                    if (seek_len > 0)
                    {
                        read_len = fread(buffer, sizeof(char), seek_len, file);
                    }
                    seek_len = 0;
                }
                else
                {
                    seek_len = read_len - (int)(tmp2 - buffer) - offset;
                }
                request_buffer = AXIS2_MALLOC(env->allocator,
                                              sizeof(axis2_char_t) * (48 * 1024 + 1));
                if (!request_buffer)
                {
                    printf("\ngimme more memory\n");
                    return -1;
                }
                if (seek_len > 0)
                {
                    memcpy(request_buffer, buffer + (read_len - seek_len), seek_len);
                }
                read_len = (int)fread(request_buffer + seek_len,
                                 sizeof(char), 48 * 1024 - seek_len, file) + seek_len;
                tmp1 = NULL;
                tmp3 = request_buffer;
                tmp1 = strstr(tmp3, footer_str);
                temp_len = (int)strlen(tmp3) + 1;
                /* loop below is for mtom cases */
                while (!tmp1 && (read_len > temp_len))
                {
                    if (!has_raw_binary)
                    {
                        has_raw_binary = 1;
                    }
                    tmp3 += (int)strlen(tmp3) + 1;
                    tmp1 = strstr(tmp3, footer_str);
                    temp_len += (int)strlen(tmp3) + 1;
                }
                if (tmp1)
                {
                    axis2_char_t *req_header = NULL;
                    axis2_char_t *req_payload = NULL;
                    int req_content_len = 0;
                    req_content_len = (int)(tmp1 - request_buffer) - 4;
                    *tmp1 = '\0';
                    tmp1 = NULL;
                    tmp1 = strstr(request_buffer, "\r\n\r\n");
                    if (tmp1)
                    {
                        axis2_char_t *listen_host = "localhost";
                        int write_socket = -1;
                        axutil_stream_t *write_stream = NULL;
                        tmp1 += 2;
                        *tmp1 = '\0';
                        req_payload = tmp1 + 2;
                        tmp1 = axutil_strdup(env, request_buffer);
                        req_content_len -= (int)strlen(tmp1);
                        tmp1 = tcpmon_util_str_replace(env, tmp1, ";\n\t", "; ");
                        req_header = tmp1;
                        tmp2 = strstr(req_header, AXIS2_HTTP_HEADER_USER_AGENT ":");
                        if (tmp2)
                        {
                            tmp3 = strstr(tmp2, "\r\n");
                            if (tmp3)
                            {
                                int header_len = 0;
                                axis2_char_t *user_agent = AXIS2_HTTP_HEADER_USER_AGENT 
                                    ": " AXIS2_HTTP_HEADER_SERVER_AXIS2C " TCPMon";
                                header_len = (int)(tmp3 - tmp2) + 2;
                                tmp1 = AXIS2_MALLOC(env->allocator,
                                                    sizeof(axis2_char_t) * header_len + 1);
                                memcpy(tmp1, tmp2, header_len);
                                tmp1[header_len] = '\0';
                                header_len = 2 + (int)strlen(user_agent);
                                tmp2 = AXIS2_MALLOC(env->allocator,
                                                    sizeof(axis2_char_t) * (header_len + 1));
                                sprintf(tmp2, "%s\r\n", user_agent);
                                req_header = tcpmon_util_str_replace(env, req_header, tmp1, tmp2);
                                AXIS2_FREE(env->allocator, tmp1);
                                AXIS2_FREE(env->allocator, tmp2);
                                tmp1 = NULL;
                                tmp2 = NULL;
                            }
                        }
                        if (!has_raw_binary)
                        {
                            tmp2 = strstr(req_header, AXIS2_HTTP_HEADER_CONTENT_LENGTH ":");
                            if (tmp2)
                            {
                                tmp3 = strstr(tmp2, "\r\n");
                                if (tmp3)
                                {
                                    int header_len = 0;
                                    header_len = (int)(tmp3 - tmp2) + 2;
                                    tmp1 = AXIS2_MALLOC(env->allocator,
                                                        sizeof(axis2_char_t) * header_len + 1);
                                    memcpy(tmp1, tmp2, header_len);
                                    tmp1[header_len] = '\0';
                                    tmp2 = AXIS2_MALLOC(env->allocator,
                                                        sizeof(axis2_char_t) * (header_len + 2));
                                    req_content_len = (int)strlen(req_payload);
                                    sprintf(tmp2, "%s%d\r\n", AXIS2_HTTP_HEADER_CONTENT_LENGTH 
                                        ": ", req_content_len);
                                    req_header = tcpmon_util_str_replace(env, req_header, tmp1, tmp2);
                                    AXIS2_FREE(env->allocator, tmp1);
                                    AXIS2_FREE(env->allocator, tmp2);
                                    tmp1 = NULL;
                                    tmp2 = NULL;
                                }
                            }
                        }
                        tmp2 = strstr(req_header, AXIS2_HTTP_HEADER_HOST ":");
                        if (tmp2)
                        {
                            tmp3 = strstr(tmp2, "\r\n");
                            if (tmp3)
                            {
                                int header_len = 0;
                                header_len = (int)(tmp3 - tmp2) + 2;
                                tmp1 = AXIS2_MALLOC(env->allocator,
                                                    sizeof(axis2_char_t) * header_len + 1);
                                memcpy(tmp1, tmp2, header_len);
                                tmp1[header_len] = '\0';
                                header_len = 16 + (int)strlen(target_host);
                                tmp2 = AXIS2_MALLOC(env->allocator,
                                                    sizeof(axis2_char_t) * (header_len + 1));
                                sprintf(tmp2, "%s%s:%d\r\n", AXIS2_HTTP_HEADER_HOST ": ", target_host,
                                        TCPMON_SESSION_GET_LISTEN_PORT(session, env));
                                req_header = tcpmon_util_str_replace(env, req_header, tmp1, tmp2);
                                AXIS2_FREE(env->allocator, tmp1);
                                AXIS2_FREE(env->allocator, tmp2);
                                tmp1 = NULL;
                                tmp2 = NULL;
                            }
                        }
                        write_socket =
                           (int)axutil_network_handler_open_socket(env, listen_host, 
                            TCPMON_SESSION_GET_LISTEN_PORT(session, env));
                        if (write_socket == -1)
                        {
                            printf("\nerror in creating socket\n");
                        }
                        else
                        {
                            write_stream = axutil_stream_create_socket(env, write_socket);
                        }
                        if (!write_stream)
                        {
                            printf("\nerror in creating stream\n");
                        }
                        else
                        {
                            axutil_stream_write(write_stream, env, req_header, strlen(req_header));
                            axutil_stream_write(write_stream, env, "\r\n", 2);
                            axutil_stream_write(write_stream, env, req_payload, req_content_len);
                            axutil_stream_free(write_stream, env);
                            axutil_network_handler_close_socket(env, write_socket);
                        }
                        AXIS2_FREE(env->allocator, req_header);
                    }
                }
                else if (read_len == 48 * 1024)
                {
                    printf("\nrequest size greater than buffer\n");
                }
                AXIS2_FREE(env->allocator, request_buffer);
                break;
            }
            AXIS2_FREE(env->allocator, uuid_match);
        }
        if (end_reached)
        {
            break;
        }
    }
    AXIS2_FREE(env->allocator, buffer);

    fclose(file);
    return 0;
}

int
on_new_entry(
    const axutil_env_t * env,
    tcpmon_entry_t * entry,
    int status)
{
    char *plain_buffer = NULL;
    char *formated_buffer = NULL;
    int format = 0;
    char *uuid = NULL;
    int resend = 0;

    format = TCPMON_ENTRY_GET_FORMAT_BIT(entry, env);

    if (status == 0)
    {
        if (strstr(TCPMON_ENTRY_SENT_HEADERS(entry, env), 
                   AXIS2_HTTP_HEADER_USER_AGENT ": "\
                   AXIS2_HTTP_HEADER_SERVER_AXIS2C " TCPMon\r\n"))
        {
            resend = 1;
        }
        plain_buffer = TCPMON_ENTRY_SENT_DATA(entry, env);
        if (plain_buffer)       /* this can be possible as no xml present */
        {
            if (TCPMON_ENTRY_GET_SENT_DATA_LENGTH(entry, env) ==
                (int)strlen(TCPMON_ENTRY_SENT_HEADERS(entry, env)) +
                (int)strlen(plain_buffer) + 4)
            {
                format = 0; /* mtom scenario */
            }
            formated_buffer = tcpmon_util_format_as_xml
                (env, plain_buffer, format);
        }
        else
        {
            formated_buffer = "";
        }
        printf("\n\n%s\n", resend ? "RESENDING DATA..": "SENDING DATA..");
        printf("/* sending time = %s*/\n", TCPMON_ENTRY_SENT_TIME(entry, env));
        uuid = axutil_uuid_gen(env);
        printf("/* message uuid = %s*/\n", uuid);
        AXIS2_FREE(env->allocator, uuid);
        printf("---------------------\n");

        if (format || TCPMON_ENTRY_GET_SENT_DATA_LENGTH(entry, env) ==
            (int)strlen(TCPMON_ENTRY_SENT_HEADERS(entry, env)) +
            (int)strlen(formated_buffer) + 4)
        {
            printf("%s\n\n%s\n\n", TCPMON_ENTRY_SENT_HEADERS(entry, env),
                   formated_buffer);
        }
        else
        {
            int count = 0;
            int printed = 0;
            axis2_char_t *formated_buffer_temp = formated_buffer;
            printf("%s\n", TCPMON_ENTRY_SENT_HEADERS(entry, env));
            count = TCPMON_ENTRY_GET_SENT_DATA_LENGTH(entry, env) - 4 -
                    (int)strlen(TCPMON_ENTRY_SENT_HEADERS(entry, env));
            while (count > printed)
            {
                int plen = 0;
                plen = ((int)strlen(formated_buffer) + 1);
                if (plen != 1)
                {
                    printf("%s", formated_buffer);
                }
                printed += plen;
                if (count > printed)
                {
                    printf("%c", '\0');
                    formated_buffer += plen;
                }
            }
            formated_buffer = formated_buffer_temp;
        }
    }
    if (status == 1)
    {
        plain_buffer = TCPMON_ENTRY_ARRIVED_DATA(entry, env);
        if (plain_buffer)       /* this can be possible as no xml present */
        {
            if (TCPMON_ENTRY_GET_ARRIVED_DATA_LENGTH(entry, env) ==
                (int)strlen(TCPMON_ENTRY_ARRIVED_HEADERS(entry, env)) +
                (int)strlen(plain_buffer) + 4)
            {
                format = 0; /* mtom scenario */
            }
            formated_buffer = tcpmon_util_format_as_xml
                (env, plain_buffer, format);
        }
        else
        {
            formated_buffer = "";
        }
        printf("\n\n%s\n", "RETRIEVING DATA..");
        printf("/* retrieving time = %s*/\n",
               TCPMON_ENTRY_ARRIVED_TIME(entry, env));
        printf("/* time throughput = %s*/\n",
               TCPMON_ENTRY_TIME_DIFF(entry, env));
        printf("---------------------\n");

        if (format || TCPMON_ENTRY_GET_ARRIVED_DATA_LENGTH(entry, env) ==
            (int)strlen(TCPMON_ENTRY_ARRIVED_HEADERS(entry, env)) +
            (int)strlen(formated_buffer) + 4)
        {
            printf("%s\n\n%s\n\n", TCPMON_ENTRY_ARRIVED_HEADERS(entry, env),
                   formated_buffer);
        }
        else
        {
            int count = 0;
            int printed = 0;
            axis2_char_t *formated_buffer_temp = formated_buffer;
            printf("%s\n", TCPMON_ENTRY_ARRIVED_HEADERS(entry, env));
            count = TCPMON_ENTRY_GET_ARRIVED_DATA_LENGTH(entry, env) - 4 -
                    (int)strlen(TCPMON_ENTRY_ARRIVED_HEADERS(entry, env));
            while (count > printed)
            {
                int plen = 0;
                plen = ((int)strlen(formated_buffer) + 1);
                if (plen != 1)
                {
                    printf("%s", formated_buffer);
                }
                printed += plen;
                if (count > printed)
                {
                    printf("%c", '\0');
                    formated_buffer += plen;
                }
            }
            formated_buffer = formated_buffer_temp;
        }
    }
    return 0;
}

int
on_error_func(
    const axutil_env_t * env,
    char *error_message)
{
    fprintf(stderr, "ERROR: %s\n", error_message);
    return 0;
}


/**
 * Signal handler
 */
void
sig_handler(
    int signal)
{

    if (!system_env)
    {
        AXIS2_LOG_ERROR(system_env->log, AXIS2_LOG_SI,
                        "Received signal %d, unable to proceed system_env is NULL,\
                         system exit with -1", signal);
        _exit (-1);
    }

    switch (signal)
    {
    case SIGINT:
        {
            AXIS2_LOG_INFO(system_env->log, "Received signal SIGINT. Utility "
                           "shutting down");
            printf("\n\n");
            TCPMON_SESSION_STOP(session, system_env);
            TCPMON_SESSION_FREE(session, system_env);
            AXIS2_FREE(system_env->allocator, target_host);
            if (system_env)
            {
                axutil_env_free(system_env);
            }
            exit(0);
        }
#ifndef WIN32
    case SIGPIPE:
        {
            AXIS2_LOG_INFO(system_env->log, "Received signal SIGPIPE. Operation "
                           "aborted");
            return;
        }
#endif
    case SIGSEGV:
        {
            fprintf(stderr, "Received deadly signal SIGSEGV. Terminating\n");
            _exit(-1);
        }
    }
}

