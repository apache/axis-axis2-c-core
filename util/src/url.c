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

#include <axutil_url.h>
#include <axutil_string.h>
#include <axutil_string.h>
#include <axutil_file_handler.h>
#include <axutil_network_handler.h>
#include <axutil_types.h>

struct axutil_url
{
    axis2_char_t *protocol;
    axis2_char_t *host;
    int port;
    axis2_char_t *path;
    axis2_char_t *query;
    axis2_char_t *server;
};

static int is_safe_or_unreserve (char c);

AXIS2_EXTERN axutil_url_t *AXIS2_CALL
axutil_url_create(
    const axutil_env_t *env,
    const axis2_char_t *protocol,
    const axis2_char_t *host,
    const int port,
    const axis2_char_t *path)
{
    axutil_url_t *url = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, protocol, NULL);

    if (!protocol || !*protocol ||
        strstr(protocol, "://") ||
        (host && strchr(host, '/')))
    {
        return NULL;
    }

    url = (axutil_url_t *) AXIS2_MALLOC(env->allocator, sizeof(axutil_url_t));

    if (!url)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    url->protocol = axutil_strdup(env, protocol);
    url->host = NULL;
    url->path = NULL;
    url->server = NULL;
    url->query = NULL;

    if (host)
    {
        url->host = (axis2_char_t *) axutil_strdup(env, host);
        url->port = port;
    }
    else
    {
        url->port = 0;
    }

    /** if the path is not starting with / we have to make it so
     */
    if (path)
    {
        axis2_char_t *params = NULL;
        axis2_char_t *temp = NULL;
        if (path[0] == '/')
        {
            temp = (axis2_char_t *) axutil_strdup(env, path);
        }
        else
        {
            temp = axutil_stracat(env, "/", path);
        }
        params = strchr(temp, '?');
        if (!params)
        {
            params = strchr(temp, '#');
        }
        if (params)
        {
            url->query = (axis2_char_t *) axutil_strdup(env, params);
            *params = '\0';
        }
        url->path = (axis2_char_t *) axutil_strdup(env, temp);
        AXIS2_FREE(env->allocator, temp);
    }

    return url;
}

AXIS2_EXTERN axutil_url_t *AXIS2_CALL
axutil_url_parse_string(
    const axutil_env_t *env,
    const axis2_char_t *str_url)
{
    /**
     * Only accepted format is : 
     * protocol://host:port/path
      * Added file:///path
     * port is optional and the default port is assumed
     * if path is not present / (root) is assumed
     */
    axis2_char_t *tmp_url_str = NULL;
    axutil_url_t *ret = NULL;
    const axis2_char_t *protocol = NULL;
    axis2_char_t *path = NULL;
    axis2_char_t *port_str = NULL;
    axis2_char_t *host = NULL;
    int port = 0;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, str_url, NULL);

    tmp_url_str = axutil_strdup(env, str_url);
    if (!tmp_url_str)
    {
        return NULL;
    }
    protocol = tmp_url_str;
    host = strstr(tmp_url_str, "://");
    if (!host)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_ADDRESS, AXIS2_FAILURE);
  
        AXIS2_FREE(env->allocator, tmp_url_str);
        return NULL;
    }
    if (axutil_strlen(host) < 3 * sizeof(axis2_char_t))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_ADDRESS, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Invalid IP or hostname");
        AXIS2_FREE(env->allocator, tmp_url_str);
        return NULL;
    }
    *host = '\0';
    host += 3 * sizeof(axis2_char_t); /* skip "://" part */
    if (axutil_strlen(host) <= 0)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_ADDRESS, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Invalid IP or hostname");
        AXIS2_FREE(env->allocator, tmp_url_str);
        return NULL;
    }
    /* if the url is file:// thing we need the protocol and
     * path only
     */
    if (0 == axutil_strcasecmp(protocol, (const axis2_char_t *) "file"))
    {
        ret = axutil_url_create(env, protocol, NULL, 0, host);
        AXIS2_FREE(env->allocator, tmp_url_str);
        return ret;
    }

    port_str = strchr(host, ':');
    if (!port_str)
    {
        path = strchr(host, '/');
        if (!path)
        {
            path = strchr(host, '?');
        }
        else
        {
            *path++ = '\0';
        }
        if (!path)
        {
            path = strchr(host, '#');
        }
        if (!path)
        {
            /* No path - assume def path ('/') */
            /* here we have protocol + host + def port + def path */
            ret = axutil_url_create(env, protocol, host, port, "/");
            AXIS2_FREE(env->allocator, tmp_url_str);
            return ret;
        }
        else
        {
            axis2_char_t *path_temp = NULL;

            path_temp = axutil_strdup(env, path);
            *path = '\0';
            /* here we have protocol + host + def port + path */
            ret = axutil_url_create(env, protocol, host, port, path_temp);
            AXIS2_FREE(env->allocator, tmp_url_str);
            AXIS2_FREE(env->allocator, path_temp);
            return ret;
        }
    }
    else
    {
        *port_str++ = '\0';
        path = strchr(port_str, '/');
        if (!path)
        {
            path = strchr(port_str, '?');
            if (path)
            {
                *path = '\0';
                port = AXIS2_ATOI(port_str);
                *path = '?';
            }
        }
        else
        {
            *path++ = '\0';
            port = AXIS2_ATOI(port_str);
        }
        if (!path)
        {
            path = strchr(port_str, '#');
            if (path)
            {
                *path = '\0';
                port = AXIS2_ATOI(port_str);
                *path = '#';
            }
        }
        if (!path)
        {
            port = AXIS2_ATOI(port_str);
            /* here we have protocol + host + port + def path */
            ret = axutil_url_create(env, protocol, host, port, "/");
            AXIS2_FREE(env->allocator, tmp_url_str);
            return ret;
        }
        else
        {
            if (axutil_strlen(path) > 0)
            {
                axis2_char_t *path_temp = NULL;

                path_temp = axutil_strdup(env, path);
                *path = '\0';
                /* here we have protocol + host + port + path */
                ret = axutil_url_create(env, protocol, host, port, path_temp);
                AXIS2_FREE(env->allocator, tmp_url_str);
                AXIS2_FREE(env->allocator, path_temp);
                return ret;
            }
            else
            {
                /* here we have protocol + host + port + def path */
                ret = axutil_url_create(env, protocol, host, port, "/");
                AXIS2_FREE(env->allocator, tmp_url_str);
                return ret;
            }
        }
    }
}

AXIS2_EXTERN void AXIS2_CALL
axutil_url_free(
    axutil_url_t *url,
    const axutil_env_t *env)
{
    if (url->protocol)
    {
        AXIS2_FREE(env->allocator, url->protocol);
        url->protocol = NULL;
    }
    if (url->host)
    {
        AXIS2_FREE(env->allocator, url->host);
        url->host = NULL;
    }
    if (url->server)
    {
        AXIS2_FREE(env->allocator, url->server);
        url->server = NULL;
    }
    if (url->path)
    {
        AXIS2_FREE(env->allocator, url->path);
        url->path = NULL;
    }
    if (url->query)
    {
        AXIS2_FREE(env->allocator, url->query);
        url->query = NULL;
    }
    AXIS2_FREE(env->allocator, url);
    return;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_url_to_external_form(
    axutil_url_t *url,
    const axutil_env_t *env)
{
    axis2_char_t *external_form = NULL;
    axis2_ssize_t len = 0;
    axis2_char_t port_str[8];
    axis2_bool_t print_port = AXIS2_FALSE;
    AXIS2_PARAM_CHECK(env->error, url, NULL);

    if (!url->protocol)
    {
        return NULL;
    }

    if (url->port != 0 &&
        url->port != axutil_uri_port_of_scheme(url->protocol))
    {
        print_port = AXIS2_TRUE;
        sprintf(port_str, "%d", url->port);
    }

    len = axutil_strlen(url->protocol) + 6;
    if (url->host)
    {
        len += axutil_strlen(url->host);
    }
    if (url->path)
    {
        len += axutil_strlen(url->path);
    }
    if (url->query)
    {
        len += axutil_strlen(url->query);
    }
    if (print_port)
    {
        len += axutil_strlen(port_str) + 1;
    }
    external_form = (axis2_char_t *) AXIS2_MALLOC(env->allocator, len);
    sprintf(external_form, "%s://%s%s%s%s%s", url->protocol,
            (url->host) ? url->host : "",
            (print_port) ? ":" : "",
            (print_port) ? port_str : "",
            (url->path) ? url->path : "",
            (url->query) ? url->query : "");
    return external_form;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_url_set_protocol(
    axutil_url_t *url,
    const axutil_env_t *env,
    axis2_char_t *protocol)
{
    AXIS2_PARAM_CHECK(env->error, protocol, AXIS2_FAILURE);
    if (url->protocol)
    {
        AXIS2_FREE(env->allocator, url->protocol);
        url->protocol = NULL;
    }
    url->protocol = axutil_strdup(env, protocol);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_url_get_protocol(
    axutil_url_t *url,
    const axutil_env_t *env)
{
    return url->protocol;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_url_set_host(
    axutil_url_t *url,
    const axutil_env_t *env,
    axis2_char_t *host)
{
    axis2_ssize_t len = 0;
    axis2_char_t port_str[8];
    axis2_bool_t print_port = AXIS2_FALSE;
    AXIS2_PARAM_CHECK(env->error, host, AXIS2_FAILURE);

    if (url->host)
    {
        AXIS2_FREE(env->allocator, url->host);
    }
    url->host = axutil_strdup(env, host);
    if (url->server)
    {
        AXIS2_FREE(env->allocator, url->server);
    }
    if (!url->host)
    {
        return AXIS2_SUCCESS;
    }
    len += axutil_strlen(url->host);
    if (url->port != 0 && (!url->protocol ||
        url->port != axutil_uri_port_of_scheme(url->protocol)))
    {
        print_port = AXIS2_TRUE;
        sprintf(port_str, "%d", url->port);
        len += axutil_strlen(port_str) + 1;
    }
    url->server = (axis2_char_t *) AXIS2_MALLOC(env->allocator, len);
    sprintf(url->server, "%s%s%s", url->host,
            (print_port) ? ":" : "",
            (print_port) ? port_str : "");   
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_url_get_host(
    axutil_url_t *url,
    const axutil_env_t *env)
{
    return url->host;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_url_set_server(
    axutil_url_t *url,
    const axutil_env_t *env,
    axis2_char_t *server)
{
    axis2_char_t *temp = NULL;
    axis2_char_t *port_str = NULL;
    AXIS2_PARAM_CHECK(env->error, server, AXIS2_FAILURE);

    temp = axutil_strdup(env, server);
    if (temp && *temp == ':')
    {
        AXIS2_FREE(env->allocator, temp);
        return AXIS2_FAILURE;
    }

    if (strchr(temp, '/'))
    {
        AXIS2_FREE(env->allocator, temp);
        return AXIS2_FAILURE;
    }

    if (url->server)
    {
        AXIS2_FREE(env->allocator, url->server);
    }
    if (url->host)
    {
        AXIS2_FREE(env->allocator, url->host);
    }
    url->port = 0;

    url->server = axutil_strdup(env, server);

    port_str = strchr(temp, ':');
    if (port_str)
    {
        *port_str++ = '\0';
        url->port = AXIS2_ATOI(port_str);
    }

    url->host = axutil_strdup(env, temp);
    AXIS2_FREE(env->allocator, temp);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_url_get_server(
    axutil_url_t *url,
    const axutil_env_t *env)
{
    axis2_ssize_t len = 0;
    axis2_char_t port_str[8];
    axis2_bool_t print_port = AXIS2_FALSE;

    if (!url->server && !url->host)
    {
        return NULL;
    }
    else if (!url->host)
    {
        AXIS2_FREE(env->allocator, url->server);
        return NULL;
    }
    else if (url->server)
    {
        return url->server;
    }
    len += axutil_strlen(url->host);
    if (url->port != 0 && (!url->protocol ||
        url->port != axutil_uri_port_of_scheme(url->protocol)))
    {
        print_port = AXIS2_TRUE;
        sprintf(port_str, "%d", url->port);
        len += axutil_strlen(port_str) + 1;
    }
    url->server = (axis2_char_t *) AXIS2_MALLOC(env->allocator, len);
    sprintf(url->server, "%s%s%s", url->host,
            (print_port) ? ":" : "",
            (print_port) ? port_str : "");
    return url->server;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_url_set_port(
    axutil_url_t *url,
    const axutil_env_t *env,
    int port)
{
    axis2_ssize_t len = 0;
    axis2_char_t port_str[8];
    axis2_bool_t print_port = AXIS2_FALSE;
    if (url->port == port)
    {
        return AXIS2_SUCCESS;
    }
    url->port = port;
    if (url->server)
    {
        AXIS2_FREE(env->allocator, url->server);
    }
    if (!url->host)
    {
        return AXIS2_SUCCESS;
    }
    len += axutil_strlen(url->host);
    if (url->port != 0 && (!url->protocol ||
        url->port != axutil_uri_port_of_scheme(url->protocol)))
    {
        print_port = AXIS2_TRUE;
        sprintf(port_str, "%d", url->port);
        len += axutil_strlen(port_str) + 1;
    }
    url->server = (axis2_char_t *) AXIS2_MALLOC(env->allocator, len);
    sprintf(url->server, "%s%s%s", url->host,
            (print_port) ? ":" : "",
            (print_port) ? port_str : "");   
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN int AXIS2_CALL
axutil_url_get_port(
    axutil_url_t *url,
    const axutil_env_t *env)
{
    if (!url->port)
    {
       return axutil_uri_port_of_scheme(url->protocol);
    }
    return url->port;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_url_set_path(
    axutil_url_t *url,
    const axutil_env_t *env,
    axis2_char_t * path)
{
    AXIS2_PARAM_CHECK(env->error, path, AXIS2_FAILURE);
    if (url->path)
    {
        AXIS2_FREE(env->allocator, url->path);
    }
    url->path = axutil_strdup(env, path);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_url_get_path(
    axutil_url_t *url,
    const axutil_env_t *env)
{
    return url->path;
}

AXIS2_EXTERN axutil_url_t *AXIS2_CALL
axutil_url_clone(
    axutil_url_t *url,
    const axutil_env_t *env)
{
    axis2_char_t *temp = NULL;
    axutil_url_t *ret = NULL;

    if (url->path && url->query)
    {
        temp = axutil_stracat(env, url->path, url->query);
    }
    else if (url->path)
    {
        temp = axutil_strdup(env, url->path);
    }
    else if (url->query)
    {
        temp = axutil_strdup(env, url->query);
    }

    ret = axutil_url_create(env, url->protocol, url->host, url->port,
                            url->path);
    if (temp)
    {
        AXIS2_FREE(env->allocator, temp);
    }
    return ret;
}

AXIS2_EXTERN axutil_uri_t *AXIS2_CALL
axutil_url_to_uri(
    axutil_url_t *url,
    const axutil_env_t *env)
{
    axis2_char_t *url_str = NULL;
    axutil_uri_t *uri = NULL;
    url_str = axutil_url_to_external_form(url, env);
    uri = axutil_uri_parse_string(env, url_str);
    return uri;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL 
axutil_url_encode (
    const axutil_env_t *env,
    axis2_char_t *dest,
    axis2_char_t *buff, int len)
{
    axis2_char_t string[4];
    axis2_char_t *expand_buffer = NULL;
	axis2_char_t *temp = NULL;
    int i;
    for (i = 0; i < len && buff[i]; i++)
    {
        if (isalnum (buff[i]) || is_safe_or_unreserve (buff[i]))
        {
            sprintf (string, "%c", buff[i]);
        }
        else
        {
            /* %%%x is to print % mark with the hex value */
            sprintf (string, "%%%x", buff[i]);
        }

        if (((int)strlen (dest) + 4) > len)
        {
            expand_buffer =
                (axis2_char_t *) AXIS2_MALLOC (env->allocator, len * 2);
            memset (expand_buffer, 0, len * 2);
            len *= 2;
            temp = memmove (expand_buffer, dest, strlen (dest));
			if(dest)
			{
				AXIS2_FREE(env->allocator, dest);
				dest = NULL;
			}
			dest = temp;
        }
        strcat (dest, string);
    }
    return dest;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_url_get_query(
		axutil_url_t *url,
		const axutil_env_t *env)
{
	return url->query;
}


static int
is_safe_or_unreserve (char c)
{
    char safe[] = { '-', '_', '.', '~'};
    char reserve[] = { ';', '/', '?', ':', '@', '&', '=', '#', '[', ']', '!', 
                       '$', '\'', '(', ')', '*', '+', ','};

/* These are reserved and safe charaters , got from RFC
 * 
 * reserved       = ";" | "/" | "?" | ":" | "@" | "&" | "="
 * safe           = "$" | "-" | "_" | "." | "+" 
 */

    int flag = 0;
    int i = 0;

    int size = sizeof (safe) / sizeof (safe[0]);
    for (i = 0; i < size; i++)
    {
        if (c == safe[i])
        {
            flag = 1;
            return flag;
        }
    }

    size = sizeof (reserve) / sizeof (reserve[0]);
    for (i = 0; i < size; i++)
    {
        if (c == reserve[i])
        {
            flag = 0;
            return flag;
        }
    }
    return flag;
}
