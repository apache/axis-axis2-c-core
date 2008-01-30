
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
#include <stdlib.h>
#include <axutil_string.h>
#include <string.h>
#include <platforms/axutil_platform_auto_sense.h>

AXIS2_EXPORT axis2_char_t *axis2_request_url_prefix = "services";

AXIS2_EXTERN axis2_char_t **AXIS2_CALL
axutil_parse_request_url_for_svc_and_op(
    const axutil_env_t * env,
    const axis2_char_t * request)
{
    axis2_char_t **ret = NULL;
    axis2_char_t *service_str = NULL;
    axis2_char_t *tmp = NULL;
    int i = 0;
    AXIS2_ENV_CHECK(env, NULL);
    ret = AXIS2_MALLOC(env->allocator, 2 * (sizeof(axis2_char_t *)));
    memset(ret, 0, 2 * sizeof(axis2_char_t *));
    tmp = (axis2_char_t *) request;

    while (1)
    {
        tmp = strstr(tmp, axis2_request_url_prefix);
        if (!tmp)
        {
            break;
        }
        else
        {
            service_str = tmp;
            tmp += axutil_strlen(axis2_request_url_prefix);
            break;  /* stop on first prefix as user may have prefix in service name */
        }
    }
    if (service_str)
    {
        service_str += axutil_strlen(axis2_request_url_prefix);
        if ('\0' != *service_str)
        {
            if (*service_str == '/')
                service_str++;  /*to remove the leading '/' */
            tmp = strchr(service_str, '/');
            if (tmp)
            {
                i = tmp - service_str;
                ret[0] = AXIS2_MALLOC(env->allocator, i * sizeof(char) + 1);
                strncpy(ret[0], service_str, i);
                ret[0][i] = '\0';

                /* Now search for the op */
                service_str = tmp;
                if ('\0' != *service_str)
                {
                    service_str++;
                    tmp = strchr(service_str, '?');
                    if (tmp)
                    {
                        i = tmp - service_str;
                        ret[1] =
                            AXIS2_MALLOC(env->allocator, i * sizeof(char) + 1);
                        strncpy(ret[1], service_str, i);
                        ret[1][i] = '\0';
                    }
                    else
                    {
                        ret[1] = axutil_strdup(env, service_str);
                    }
                }
            }
            else
            {
                ret[0] = axutil_strdup(env, service_str);
            }
        }
    }
    return ret;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_xml_quote_string(
    const axutil_env_t * env,
    const axis2_char_t * s,
    axis2_bool_t quotes)
{
    const char *scan;
    size_t len = 0;
    size_t extra = 0;
    char *qstr;
    char *qscan;
    char c;

    for (scan = s; (c = *scan) != '\0'; ++scan, ++len)
    {
        if (c == '<' || c == '>')
            extra += 3;         /* &lt; or &gt; */
        else if (c == '&')
            extra += 4;         /* &amp; */
        else if (quotes && c == '"')
            extra += 5;         /* &quot; */
    }

    /* nothing to do */
    if (extra == 0)
        return NULL;

    qstr = AXIS2_MALLOC(env->allocator, len + extra + 1);
    for (scan = s, qscan = qstr; (c = *scan) != '\0'; ++scan)
    {
        if (c == '<')
        {
            *qscan++ = '&';
            *qscan++ = 'l';
            *qscan++ = 't';
            *qscan++ = ';';
        }
        else if (c == '>')
        {
            *qscan++ = '&';
            *qscan++ = 'g';
            *qscan++ = 't';
            *qscan++ = ';';
        }
        else if (c == '&')
        {
            *qscan++ = '&';
            *qscan++ = 'a';
            *qscan++ = 'm';
            *qscan++ = 'p';
            *qscan++ = ';';
        }
        else if (quotes && c == '"')
        {
            *qscan++ = '&';
            *qscan++ = 'q';
            *qscan++ = 'u';
            *qscan++ = 'o';
            *qscan++ = 't';
            *qscan++ = ';';
        }
        else
        {
            *qscan++ = c;
        }
    }

    *qscan = '\0';
    return qstr;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_url_decode(
    const axutil_env_t * env,
    axis2_char_t * dest,
    axis2_char_t * src)
{
    AXIS2_PARAM_CHECK(env->error, dest, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, src, AXIS2_FAILURE);

    for (; *src != '\0'; ++dest, ++src)
    {
        if (src[0] == '%' && isxdigit(src[1]) && isxdigit(src[2]))
        {
            *dest = axutil_hexit(src[1]) * 16 + axutil_hexit(src[2]);
            src += 2;
        }
        else
        {
            *dest = *src;
        }
    }
    *dest = '\0';

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN int AXIS2_CALL
axutil_hexit(axis2_char_t c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f')
    {
        return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'F')
    {
        return c - 'A' + 10;
    }
    return 0;    
}
