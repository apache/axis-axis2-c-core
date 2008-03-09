
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

char *str_replace(
    char *str,
    const char *search,
    const char *replace);

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
    int listen_port = 9090,
        target_port = 8080;
    int test_bit = 0;
    int format_bit = 0;
    int ii = 1;

    if (!axutil_strcmp(argv[1], "-h"))
    {
        printf
            ("Usage : %s [-lp LISTEN_PORT] [-tp TARGET_PORT] [-th TARGET_HOST] [-f LOG_FILE] [options]\n",
             argv[0]);
        fprintf(stdout, " Options :\n");
        fprintf(stdout,
                "\t-lp LISTEN_PORT \tport number to listen on, default is 9090\n");
        fprintf(stdout,
                "\t-tp TARGET_PORT \tport number to connect and re-direct messages, default is 8080\n");
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
        else if (!strcmp("-tp", argv[ii]))
        {
            ii++;
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
        else if (!strcmp("-th", argv[ii]))
        {
            ii++;
            AXIS2_FREE(env->allocator, target_host);
            target_host = (char *) axutil_strdup(env, argv[ii++]);
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
            tcpmon_traffic_log = argv[ii++];
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

    file = fopen(tcpmon_traffic_log, "a+");

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
        plain_buffer = TCPMON_ENTRY_SENT_DATA(entry, env);
        if (plain_buffer)       /* this can be possible as no xml present */
        {
            if (TCPMON_ENTRY_GET_SENT_DATA_LENGTH(entry, env) !=
                strlen(TCPMON_ENTRY_SENT_HEADERS(entry, env)) +
                strlen(plain_buffer) + 4)
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
        printf("\n\n%s\n", "SENDING DATA..");
        printf("/* sending time = %s*/\n", TCPMON_ENTRY_SENT_TIME(entry, env));
        uuid = axutil_uuid_gen(env);
        printf("/* message uuid = %s*/\n", uuid);
        printf("---------------------\n");

        if (format || TCPMON_ENTRY_GET_SENT_DATA_LENGTH(entry, env) ==
            strlen(TCPMON_ENTRY_SENT_HEADERS(entry, env)) +
            strlen(formated_buffer) + 4)
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
                    strlen(TCPMON_ENTRY_SENT_HEADERS(entry, env));
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
        fprintf(file, "%s\n", "SENDING DATA..");
        fprintf(file, "/* sending time = %s*/\n",
                TCPMON_ENTRY_SENT_TIME(entry, env));
        fprintf(file, "/* message uuid = %s*/\n", uuid);
        AXIS2_FREE(env->allocator, uuid);
        fprintf(file, "---------------------\n");

        convert = axutil_strdup(env, TCPMON_ENTRY_SENT_HEADERS(entry, env));
        convert = str_replace(convert, "; ", ";\n\t");
        fprintf(file, "%s\r\n\r\n", convert);
        if (convert)
        {
            free(convert);
        }
        if (strcmp(formated_buffer, "") != 0)
        {
            if (format || TCPMON_ENTRY_GET_SENT_DATA_LENGTH(entry, env) ==
                strlen(TCPMON_ENTRY_SENT_HEADERS(entry, env)) +
                strlen(formated_buffer) + 4)
            {
                convert = axutil_strdup(env, formated_buffer);
                convert = str_replace(convert, "><", ">\n<");
                fprintf(file, "%s", convert);
                if (convert)
                {
                    free(convert);
                }
            }
            else
            {
                int count = 0;
                int printed = 0;
                count = TCPMON_ENTRY_GET_SENT_DATA_LENGTH(entry, env) - 4 -
                        strlen(TCPMON_ENTRY_SENT_HEADERS(entry, env));
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
                strlen(TCPMON_ENTRY_ARRIVED_HEADERS(entry, env)) +
                strlen(plain_buffer) + 4)
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
            strlen(TCPMON_ENTRY_ARRIVED_HEADERS(entry, env)) +
            strlen(formated_buffer) + 4)
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
                    strlen(TCPMON_ENTRY_ARRIVED_HEADERS(entry, env));
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
        convert = str_replace(convert, "; ", ";\n\t");
        fprintf(file, "%s\r\n\r\n", convert);
        if (convert)
        {
            free(convert);
        }
        if (strcmp(formated_buffer, "") != 0)
        {
            if (format || TCPMON_ENTRY_GET_ARRIVED_DATA_LENGTH(entry, env) ==
                strlen(TCPMON_ENTRY_ARRIVED_HEADERS(entry, env)) +
                strlen(formated_buffer) + 4)
            {
                convert = axutil_strdup(env, formated_buffer);
                convert = str_replace(convert, "><", ">\n<");
                fprintf(file, "%s", convert);
                if (convert)
                {
                    free(convert);
                }
            }
            else
            {
                int count = 0;
                int printed = 0;
                count = TCPMON_ENTRY_GET_ARRIVED_DATA_LENGTH(entry, env) - 4 -
                        strlen(TCPMON_ENTRY_ARRIVED_HEADERS(entry, env));
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

    file = fopen(tcpmon_traffic_log, "r");

    if (NULL == file)
    {
        printf("\ncould not create or open log-file\n");
        return -1;
    }

    buffer = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * SIZE);
    if (!buffer)
    {
        return -1;
    }

    read_len = fread(buffer, sizeof(char), SIZE - 1, file);

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

        offset = strlen(search);
        tmp3 = buffer;
        if (read_len >= SIZE)
        {
            AXIS2_FREE(env->allocator, buffer);
            AXIS2_FREE(env->allocator, uuid);
            return -1;
        }
        if (read_len < SIZE - 1)
        {   
            end_reached = 1;
        }
        while (loop_state)
        {
            tmp1 = strstr(tmp3, search);
            if (!tmp1)
            {
                if (end_reached)
                {
                    break;
                }
                memmove(buffer, buffer + (SIZE - 1 - offset), offset);
                read_len = fread(buffer + offset, sizeof(char),
                                 SIZE - 1 - offset, file) + offset;
                break;
            }
            if (read_len - offset - 36 < (int)(tmp1 - buffer))
            {
                if (end_reached)
                {
                    break;
                }
                offset += 36;
                memmove(buffer, buffer + (SIZE - 1 - offset), offset);
                read_len = fread(buffer + offset, sizeof(char),
                                 SIZE - 1 - offset, file) + offset;
                break;
            }
            tmp2 = tmp1 + offset;
            uuid_match = AXIS2_MALLOC(env->allocator,
                                      sizeof(axis2_char_t) * 37);
            if (!uuid_match)
            {
                return -1;
            }
            memcpy(uuid_match, tmp2, 36);
            uuid_match[36] = '\0';
            if (!strcasecmp(uuid_match, uuid))
            {
                AXIS2_FREE(env->allocator, uuid_match);
                AXIS2_FREE(env->allocator, uuid);
                end_reached = 1;
                break;
            }
            AXIS2_FREE(env->allocator, uuid_match);
            tmp3 += offset + 36;
        }
        if (end_reached)
        {
            break;
        }
    }
    AXIS2_FREE(env->allocator, buffer);

/*    while ((SIZE - 1 - offset) == fread(buffer + offset, sizeof(char), SIZE - 1 - offset, file))
    {
        axis2_char_t *search = "* message uuid = ";
        axis2_char_t *tmp2 = NULL;
        axis2_char_t *uuid_match = NULL;
        tmp1 = NULL;
        offset = 0;
        tmp2 = buffer;
        do
        {
            tmp1 = strstr(tmp2, search);
            if (tmp1 && *tmp1 + 1)
            {
                int len = strlen(search);
                axis2_char_t *tmp3 = NULL;
                if (SIZE - 1 - (int)(tmp1 - tmp2) <= (int)strlen(search) + 36)
                {
                    len = SIZE - 1 - (int)(tmp1 - tmp2);
                    tmp3 = AXIS2_MALLOC(env->allocator,
                                        sizeof(axis2_char_t) * len);
                    memcpy(tmp3, tmp1, len);
                    offset += len;
                    memcpy(buffer, tmp3, len);
                    AXIS2_FREE(env->allocator, tmp3);
                    break;
                }
                tmp3 = tmp1 + strlen(search);
                if (tmp3)
                {
                    int read = 36;
                    uuid_match = AXIS2_MALLOC(env->allocator,
                                              sizeof(axis2_char_t) * 37);
                    if (!uuid_match)
                    {
                        return -1;
                    }
                    if (read > SIZE - 1 - (int)(tmp3 - tmp2))
                    {
                        read = SIZE - 1 - (int)(tmp3 - tmp2);
                    }
                    memcpy(uuid_match, tmp3, read);
                    len = 36 - read;
                    if (len)
                    {
                        if (len == fread(buffer, sizeof(char), len, file))
                        {
                            memcpy(uuid_match + len, buffer, len);
                            uuid_match[36] = '\0';
                            if (!strcasecmp(uuid_match, uuid))
                            {
                                printf("match"); 
                            }
                            offset += len;
                        }
                        else
                        {
                            AXIS2_FREE(env->allocator, uuid_match);
                            break;
                        }
                    }
                    uuid_match[36] = '\0';
                    if (!strcasecmp(uuid_match, uuid))
                    {
                        printf("match");      
                    }
                    AXIS2_FREE(env->allocator, uuid_match);
                }
            }
            tmp2 = tmp1 + strlen(search);
            if (offset)
            {
                break;
            }
        }
        while(tmp1 != NULL);
        if (offset)
        {
            continue;
        }
        if (!tmp1)
        {
            int len = strlen(search);
            int inc = 0;
            axis2_char_t *tmp3 = NULL;
            tmp2 = AXIS2_MALLOC(env->allocator,
                sizeof(axis2_char_t) * (len * 2 + 1));
            if (!tmp2)
            {
                return -1;
            }
            memcpy(tmp2, buffer + (SIZE - 1 - len), len);
            if (len == fread(buffer, sizeof(char), len, file))
            {
                memcpy(tmp2 + len, buffer, len);
                inc += len;
            }
            else
            {
                AXIS2_FREE(env->allocator, tmp2);
                break;
            }
            tmp2[len * 2] = '\0';
            tmp3 = strstr(tmp2, search);
            if (tmp3)
            {
                tmp3 += strlen(search);
            }
            if (tmp3)
            {
                int read = 36;
                tmp3 += strlen(search);
                uuid_match = AXIS2_MALLOC(env->allocator,
                                          sizeof(axis2_char_t) * 37);
                if (!uuid_match)
                {
                    return -1;
                }
                if (read > len * 2 - (int)(tmp3 - tmp2))
                {
                    read = len * 2 - (int)(tmp3 - tmp2);
                }
                memcpy(uuid_match, tmp3, read);
                len = 36 - read;
                if (len)
                {
                    if (len == fread(buffer, sizeof(char), len, file))
                    {
                        memcpy(uuid_match + len, buffer, len);               
                        inc += len;
                    }
                    else
                    {
                        AXIS2_FREE(env->allocator, tmp2);
                        AXIS2_FREE(env->allocator, uuid_match);
                        break;
                    }
                }
                uuid_match[36] = '\0';
                if (!strcasecmp(uuid_match, uuid))
                {
                    printf("match");   
                }
                AXIS2_FREE(env->allocator, uuid_match);
            }
            AXIS2_FREE(env->allocator, tmp2);
            if ((SIZE - 1 - inc) != fread(buffer + inc, sizeof(char), SIZE - 1 - inc, file))
            {
                break;
            }
        }
    }
    AXIS2_FREE(env->allocator, buffer); 
*/
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

    format = TCPMON_ENTRY_GET_FORMAT_BIT(entry, env);

    if (status == 0)
    {
        plain_buffer = TCPMON_ENTRY_SENT_DATA(entry, env);
        if (plain_buffer)       /* this can be possible as no xml present */
        {
            if (TCPMON_ENTRY_GET_SENT_DATA_LENGTH(entry, env) ==
                strlen(TCPMON_ENTRY_SENT_HEADERS(entry, env)) +
                strlen(plain_buffer) + 4)
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
        printf("\n\n%s\n", "SENDING DATA..");
        printf("/* sending time = %s*/\n", TCPMON_ENTRY_SENT_TIME(entry, env));
        uuid = axutil_uuid_gen(env);
        printf("/* message uuid = %s*/\n", uuid);
        AXIS2_FREE(env->allocator, uuid);
        printf("---------------------\n");

        if (format || TCPMON_ENTRY_GET_SENT_DATA_LENGTH(entry, env) ==
            strlen(TCPMON_ENTRY_SENT_HEADERS(entry, env)) +
            strlen(formated_buffer) + 4)
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
                    strlen(TCPMON_ENTRY_SENT_HEADERS(entry, env));
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
                strlen(TCPMON_ENTRY_ARRIVED_HEADERS(entry, env)) +
                strlen(plain_buffer) + 4)
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
            strlen(TCPMON_ENTRY_ARRIVED_HEADERS(entry, env)) +
            strlen(formated_buffer) + 4)
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
                    strlen(TCPMON_ENTRY_ARRIVED_HEADERS(entry, env));
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

char *
str_replace(
    char *str,
    const char *search,
    const char *replace)
{
    int size = ((int)strlen(str)) * 2;
    /* We are sure that the difference lies within the int range */
    int addmem = size;
    int diff = (int)(strlen(replace) - strlen(search));
    /* We are sure that the difference lies within the int range */

    char *str_return = (char *) malloc(size * sizeof(char));
    char *str_tmp = (char *) malloc(size * sizeof(char));
    char *str_relic;

    if (str_return == NULL || str_tmp == NULL)
    {
        free(str_return);
        free(str_tmp);
        return "function str_replace : gimme more memory";
    }

    strcpy(str_return, str);

    while ((str_relic = strstr(str_return, search)) != NULL)
    {
        if ((int)strlen(str_return) + diff >= addmem)
            /* We are sure that the difference lies within the int range */
        {
            str_return = (char *) realloc(str_return, addmem += size);
            str_tmp = (char *) realloc(str_tmp, addmem);

            if (str_return == NULL || str_tmp == NULL)
            {
                free(str_return);
                free(str_tmp);
                return "function str_replace : gimme more memory";
            }
        }

        strcpy(str_tmp, replace);
        strcat(str_tmp, (str_relic + strlen(search)));
        *str_relic = '\0';

        strcat(str_return, str_tmp);
    }

    free(str_tmp);
    free(str);
    return (str_return);
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

