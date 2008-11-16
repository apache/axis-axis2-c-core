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

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_parse_rest_url_for_params(
    const axutil_env_t *env,
    const axis2_char_t *tmpl,
    const axis2_char_t *url,
    int *match_count,
    axis2_char_t ****matches)
{
    axis2_char_t ***ret = NULL;
    axis2_char_t *tmp1 = NULL;
    axis2_char_t **tmp2 = NULL;
    axis2_char_t ***tmp3 = NULL;
    axis2_char_t *tmp4 = NULL;
    axis2_char_t *resource = NULL;
    axis2_char_t *query = NULL;
    axis2_char_t *url_tmp = NULL;
    axis2_char_t *url_resource = NULL;
    axis2_char_t *url_query = NULL;
    axis2_bool_t finished = AXIS2_FALSE;
    axis2_status_t status = AXIS2_FAILURE;
    int ret_count = 0;
    int i = 0;
    int j = 0;
    axis2_bool_t in_tok = AXIS2_FALSE;

    tmp2 = AXIS2_MALLOC(env->allocator, 2 * (sizeof(axis2_char_t *)));
    memset(tmp2, 0, 2 * sizeof(axis2_char_t *));

    if (tmpl[0] == '/')
    {
        tmp1 = (axis2_char_t *) tmpl;
        tmp1++;
        resource = axutil_strdup(env, tmp1);
    }
    else
    {
        resource = axutil_strdup(env, tmpl);
    }
    i = (int)strlen(resource);
    /* We are sure that the difference lies within the int range */
    if (resource[i] == '/')
    {
        resource[i] = '\0';
    }
    tmp1 = strchr(resource, '?');
    if (tmp1)
    {
        axis2_char_t *tmp4 = NULL;
        
        tmp4 = tmp1;
        tmp1++;
        resource[tmp4 - resource] = '\0';
        if (*tmp1 && ((tmp1 - resource) < (int)strlen(resource) - 1))
            /* We are sure that the difference lies within the int range */
        {
            query = tmp1;
            /* 
             * Query String based matching is not implemented. This is
             * reserved for future implementations.
             */
        }
    }

    /* Validation of Template */
    i = (int)strlen(resource);
    /* We are sure that the difference lies within the int range */

    if (!strchr(resource, '{') && !strchr(resource, '}'))
    {
        i = 0;
    }
    for (j = 0; j < i; j++)
    {
        if (!in_tok)
        {
            if (resource[j] == '}')
            {
                AXIS2_FREE(env->allocator, resource);
                return AXIS2_FAILURE;
            }
            else if (resource[j] == '{')
            {
                if (j + 1 == i || resource[j + 1] == '}')
                {
                    AXIS2_FREE(env->allocator, resource);
                    return AXIS2_FAILURE;
                }
                else if (resource[j + 1] == '{')
                {
                    j++;
                }
                else
                {
                    in_tok = AXIS2_TRUE;
                }
            }
        }
        else
        {
            if (resource[j] == '{')
            {
                AXIS2_FREE(env->allocator, resource);
                return AXIS2_FAILURE;
            }
            else if (resource[j] == '}')
            {
                if (j + 1 < i && resource[j + 1] == '}')
                {
                    j++;
                }
                else
                {
                    in_tok = AXIS2_FALSE;
                }
            }
        }
    }
    if (in_tok)
    {
        AXIS2_FREE(env->allocator, resource);
        return AXIS2_FAILURE;
    }

    /* Validity of template guaranteed if not returned */

    if (url[0] == '/')
    {
        tmp1 = (axis2_char_t *) url;
        tmp1++;
        url_resource = axutil_strdup(env, tmp1);
    }
    else
    {
        url_resource = axutil_strdup(env, url);
    }
    i = (int)strlen(url_resource);

    /* We are sure that the difference lies within the int range */

    if (url_resource[i] == '/')
    {
        url_resource[i] = '\0';
    }
    i = 0;
    url_tmp = url_resource;
    tmp1 = strchr(url_resource, '?');
    if (tmp1)
    {
        axis2_char_t *tmp4 = NULL;
        
        tmp4 = tmp1;
        tmp1++;
        url_resource[tmp4 - url_resource] = '\0';
        if (*tmp1 && ((tmp1 - url_resource) < (int)strlen(url_resource) - 1))
            /* We are sure that the difference lies within the int range */
        {
            url_query = tmp1;
        }
    }
    tmp1 = resource;

    /* Simplest case match */

    if (!strchr(resource, '{'))
    {
        if (strcmp(resource, url_resource) == 0)
        {
            finished = AXIS2_TRUE;
        }
    }

    while (!finished)
    {
        tmp4 = strchr(tmp1, '{');
        if (tmp4 && tmp4[1])
        {
            if (tmp4[1] != '{')
            {
                axis2_char_t *tmp5 = NULL;
                axis2_char_t *tmp6 = NULL;
                axis2_char_t *tmp7 = NULL;
                axis2_char_t *tmp8 = NULL;
                axis2_char_t *tmp9 = NULL;

                /* Logic for finding out constant portion to match */
                i = (int)(tmp4 - tmp1);
                tmp2[0] = AXIS2_MALLOC(env->allocator, (i + 1) * sizeof(char));
                strncpy(tmp2[0], tmp1, i);
                tmp2[0][i] = '\0';
                if (url_tmp && *url_tmp)
                {
                    tmp6 = url_tmp;
                    tmp5 = strstr(tmp6, tmp2[0]);
                    if (tmp5)
                    {
                        tmp5 += strlen(tmp2[0]);
                        tmp7 = tmp4;
                        tmp8 = tmp4;
                        tmp7++;
                        if (*tmp7)
                        {
                            axis2_bool_t finished_tmp = AXIS2_FALSE;
                            while (!finished_tmp)
                            {
                                tmp6 = strchr(tmp8, '}');
                                if (tmp6 && *tmp6)
                                {
                                    if (tmp6[1] != '}')
                                    {
                                        tmp8 = tmp6 + 1;
                                        break;
                                    }
                                }
                                else
                                {
                                    finished_tmp = AXIS2_TRUE;
                                }
                            }
                            if (!finished_tmp && !strchr(tmp8, '{'))
                            {
                                tmp7 = tmp8 + strlen(tmp8);
                            }
                            else
                            {
                                while (!finished_tmp)
                                {
                                    tmp6 = strchr(tmp8, '{');
                                    if (tmp6 && tmp6[1])
                                    {
                                        if (tmp6[1] != '{')
                                        {
                                            tmp7 = tmp6;
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        finished_tmp = AXIS2_TRUE;
                                    }
                                }
                            }
                            if (!finished_tmp)
                            {
                                i = (int)(tmp7 - tmp8);
                                tmp9 = AXIS2_MALLOC(env->allocator, (i + 1) * sizeof(char));
                                strncpy(tmp9, tmp8, i);
                                tmp9[i] = '\0';
                            }
                        }
                        if (tmp9 && *tmp9)
                        {
                            tmp6 = strstr(tmp5, tmp9);
                            AXIS2_FREE (env->allocator, tmp9);
                            tmp9 = NULL;
                        }
                        else
                        {
                            tmp6 = strchr(tmp5, '/');
                        }
                        /* Logic for saving the match */
                        if (tmp6 && tmp6 != tmp5)
                        {
                            i = (int)(tmp6 - tmp5);
                            url_tmp = tmp6;
                            tmp2[1] = AXIS2_MALLOC(env->allocator, (i + 1) * sizeof(char));
                            strncpy(tmp2[1], tmp5, i);
                            tmp2[1][i] = '\0';
                        }
                        else
                        {
                            i = (int)strlen(tmp5);
                            /* We are sure that the difference lies within the int range */
                            tmp2[1] = AXIS2_MALLOC(env->allocator, (i + 1) * sizeof(char));
                            strncpy(tmp2[1], tmp5, i);
                            tmp2[1][i] = '\0';
                            url_tmp = NULL;
                        }
                    }
                }
                else
                {
                    break;
                }
                while (!finished)
                {
                    tmp1 = tmp4 + 1;
                    tmp4 = strchr(tmp1, '}');
                    if (tmp4 && *tmp4)
                    {
                        if (tmp4[1] != '}')
                        {
                            /* Logic for saving the key for the match */
                            i = (int)(tmp4 - tmp1);
                            if (tmp2[0])
                            {
                                AXIS2_FREE(env->allocator, tmp2[0]);
                            }
                            tmp2[0] = AXIS2_MALLOC(env->allocator, (i + 1) * sizeof(char));
                            strncpy(tmp2[0], tmp1, i);
                            tmp2[0][i] = '\0';
                            tmp3 = ret;
                            ret_count++;
                            ret = AXIS2_MALLOC(env->allocator, ret_count * 2 * (sizeof(axis2_char_t *)));
                            memset(ret, 0, ret_count * 2 * sizeof(axis2_char_t *));
                            for(i = 0; i < ret_count - 1; i++)
                            {
                                ret[i] = tmp3[i];
                            }
                            ret[i] = tmp2;
                            tmp2 = AXIS2_MALLOC(env->allocator, 2 * (sizeof(axis2_char_t *)));
                            memset(tmp2, 0, 2 * sizeof(axis2_char_t *));
                            tmp3 = NULL;
                            break;
                        }
                        else
                        {
                            tmp4++;
                        }
                    }
                    else
                    {
                        finished = AXIS2_TRUE;
                    }
                }
            }
            else
            {
                tmp4++;
            }
        }
        else
        {
            /* Result of mismatch at the simplest case */
            if (!strchr(resource, '{'))
            {
                finished = AXIS2_FALSE;
                break;
            }
            finished = AXIS2_TRUE;
        }
        tmp1 = tmp4 + 1;
    }
    if (resource)
    {
        AXIS2_FREE(env->allocator, resource);
    }
    if (url_resource)
    {
        AXIS2_FREE(env->allocator, url_resource);
    }
    if (tmp2)
    {
        if (tmp2[0])
        {
            AXIS2_FREE(env->allocator, tmp2[0]);
        }
        if (tmp2[1])
        {
            AXIS2_FREE(env->allocator, tmp2[1]);
        }
        AXIS2_FREE(env->allocator, tmp2);
    }
    if (finished)
    {
        status = AXIS2_SUCCESS;
    }
    *match_count = ret_count;
    *matches = ret;
    return status;
}

AXIS2_EXTERN axis2_char_t **AXIS2_CALL
axutil_parse_request_url_for_svc_and_op(
    const axutil_env_t *env,
    const axis2_char_t *request)
{
    axis2_char_t **ret = NULL;
    axis2_char_t *service_str = NULL;
    axis2_char_t *tmp = NULL;
    int i = 0;
    ret = AXIS2_MALLOC(env->allocator, 2 * (sizeof(axis2_char_t *)));
    memset(ret, 0, 2 * sizeof(axis2_char_t *));
    tmp = (axis2_char_t *) request;

    tmp = strstr(tmp, axis2_request_url_prefix);
    if (tmp)
    {
        service_str = tmp;
        tmp += axutil_strlen(axis2_request_url_prefix);
        /*break stop on first prefix as user may have prefix in service name */
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
                i = (int)(tmp - service_str);
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
                        i = (int)(tmp - service_str);
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
    const axutil_env_t *env,
    const axis2_char_t *s,
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
    const axutil_env_t *env,
    axis2_char_t *dest,
    axis2_char_t *src)
{
    AXIS2_PARAM_CHECK(env->error, dest, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, src, AXIS2_FAILURE);

    for (; *src != '\0'; ++dest, ++src)
    {
        if (src[0] == '%' && isxdigit(src[1]) && isxdigit(src[2]))
        {
            *dest = (axis2_char_t)(axutil_hexit(src[1]) * 16 + axutil_hexit(src[2]));
            /* We are sure that the conversion is safe */
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

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_char_2_byte(
    const axutil_env_t *env,
    axis2_char_t *char_buffer,
    axis2_byte_t **byte_buffer,
    int *byte_buffer_size)
{
    int length = 0;
    int i = 0;
    axis2_byte_t *bytes = NULL;

    length = (int) axutil_strlen(char_buffer);
    bytes = (axis2_byte_t *)
        AXIS2_MALLOC(env->allocator, length * sizeof(axis2_byte_t));

    if (!bytes)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No memory. Cannot create byte buffer");
        return AXIS2_FAILURE;
    }

    for (i = 0; i < length; i++)
    {
        bytes[i] = (axis2_byte_t) char_buffer[i];
    }
    *byte_buffer = bytes;
    *byte_buffer_size = length;
    return AXIS2_SUCCESS;
}

