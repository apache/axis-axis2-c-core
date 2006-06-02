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
 
#include <axis2_url.h>
#include <axis2_string.h>
#include <axis2_string.h>
#include <axis2_file_handler.h>
#include <axis2_network_handler.h>
#include <axis2_types.h>


/** 
 * @brief URL impl
 *	Axis2 URL impl  
 */
typedef struct axis2_url_impl axis2_url_impl_t;  
  
struct axis2_url_impl
{
	axis2_url_t url;
	axis2_char_t *protocol;
    axis2_char_t *server;
	int port;
	axis2_char_t *path;
};

#define AXIS2_INTF_TO_IMPL(url) ((axis2_url_impl_t *)(url))

/***************************** Function headers *******************************/
axis2_char_t* AXIS2_CALL 
axis2_url_to_external_form (axis2_url_t *url, const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
axis2_url_set_protocol(axis2_url_t *url, const axis2_env_t *env, 
						axis2_char_t *protocol);
axis2_char_t* AXIS2_CALL 
axis2_url_get_protocol (axis2_url_t *url, const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
axis2_url_set_server(axis2_url_t *url,const axis2_env_t *env, 
                        axis2_char_t *server);

axis2_char_t* AXIS2_CALL 
axis2_url_get_server(axis2_url_t *url, const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
axis2_url_set_port(axis2_url_t *url, const axis2_env_t *env, int port);

int AXIS2_CALL 
axis2_url_get_port (axis2_url_t *url, const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
axis2_url_set_path(axis2_url_t *url, const axis2_env_t *env, 
                        axis2_char_t *path);

axis2_char_t* AXIS2_CALL 
axis2_url_get_path(axis2_url_t *url, const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
axis2_url_free (axis2_url_t *url, const axis2_env_t *env);								

axis2_status_t AXIS2_CALL
axis2_url_open (axis2_url_t *url, const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_url_close (axis2_url_t *url, const axis2_env_t *env);

axis2_url_t* AXIS2_CALL
axis2_url_clone(
        axis2_url_t *url,
        const axis2_env_t *env);
/***************************** End of function headers ************************/

AXIS2_EXTERN axis2_url_t * AXIS2_CALL 
axis2_url_create (const axis2_env_t *env, axis2_char_t *protocol, 
						axis2_char_t *server, int port, axis2_char_t *path)
{
    axis2_url_impl_t *url_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, protocol, NULL);
	
    url_impl =  (axis2_url_impl_t *)AXIS2_MALLOC (env->allocator, 
						sizeof(axis2_url_impl_t));
	
    if(NULL == url_impl)
	{
		AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
	url_impl->protocol = AXIS2_STRDUP(protocol, env);
	url_impl->server = NULL;
	url_impl->path = NULL;
	url_impl->port = port;

	if(NULL != server)
	{
    	url_impl->server = (axis2_char_t *)AXIS2_STRDUP(server, env);
	}
    /** if the path is not starting with / we have to make it so
     */
	if(NULL != path)
	{
        if(path[0] == '/')
        {
            url_impl->path = (axis2_char_t *)AXIS2_STRDUP(path, env);
        }
        else
        {
            url_impl->path = AXIS2_STRACAT("/", path, env);
        }
    	
	}
	     
    url_impl->url.ops = AXIS2_MALLOC(env->allocator,sizeof(axis2_url_ops_t));
    if(NULL == url_impl->url.ops)
	{
		axis2_url_free((axis2_url_t*)url_impl, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    url_impl->url.ops->to_external_form = axis2_url_to_external_form;
   	url_impl->url.ops->set_protocol = axis2_url_set_protocol;
	url_impl->url.ops->get_protocol = axis2_url_get_protocol;
	url_impl->url.ops->set_server = axis2_url_set_server;
	url_impl->url.ops->get_server = axis2_url_get_server;
	url_impl->url.ops->set_port = axis2_url_set_port;
	url_impl->url.ops->get_port = axis2_url_get_port;
	url_impl->url.ops->set_path = axis2_url_set_path;
	url_impl->url.ops->get_path = axis2_url_get_path;
    url_impl->url.ops->clone = axis2_url_clone;
    url_impl->url.ops->free = axis2_url_free;
                        
	return &(url_impl->url);
}

axis2_url_t * AXIS2_CALL
axis2_url_parse_string(const axis2_env_t *env, axis2_char_t *str_url)
{
	/**
	 * Only accepted format is : 
	 * protocol://server:port/path
     * Added file:///path
	 * port is optional and the default port is assumed
	 * if path is not present / (root) is assumed
	 */
	axis2_char_t *tmp_url_str = NULL;
	axis2_url_t *ret = NULL;
	axis2_char_t *protocol = NULL;
	axis2_char_t *path = NULL;
	axis2_char_t *port_str = NULL;
	axis2_char_t *server = NULL;
	axis2_char_t *params = NULL;
	int port = -1;
	
	AXIS2_ENV_CHECK(env, NULL);
	AXIS2_PARAM_CHECK(env->error, str_url, NULL);
	
	tmp_url_str = AXIS2_STRDUP(str_url, env);
	if(NULL == tmp_url_str)
	{
		return NULL;
	}
	protocol = tmp_url_str;
	server = strstr(tmp_url_str, "://");
	if(NULL == server)
	{
		AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_ADDRESS, 
						AXIS2_FAILURE);
		AXIS2_FREE(env->allocator, tmp_url_str);
		return NULL;
	}
	if(AXIS2_STRLEN(server) < 3 * sizeof(axis2_char_t))
	{
		AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_ADDRESS, 
						AXIS2_FAILURE);
		AXIS2_FREE(env->allocator, tmp_url_str);
		return NULL;
	}
	*server = '\0';
	server += 3 * sizeof(axis2_char_t); /* skip "://" part */
	if(AXIS2_STRLEN(server) <= 0)
	{
		AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_ADDRESS, 
						AXIS2_FAILURE);
		AXIS2_FREE(env->allocator, tmp_url_str);
		return NULL;
	}
    /* if the url is file:// thing we need the protocol and
     * path only
     */
    if(0 == AXIS2_STRCASECMP(protocol, "file"))
    {
        ret = axis2_url_create(env, protocol, NULL, 0, server);
        AXIS2_FREE(env->allocator, tmp_url_str);
        return ret;
    }
	port_str = strchr(server, ':');
	if(NULL == port_str)
	{
		if(0 == AXIS2_STRCASECMP(protocol, "http"))
		{
			port = 80;
		}
		if(0 == AXIS2_STRCASECMP(protocol, "ftp"))
		{
			port = 20;
		}
		if(0 == AXIS2_STRCASECMP(protocol, "smtp"))
		{
			port = 25;
		}
		path = strchr(server, '/');
		if(NULL == path)
		{
			/* No path - assume def path ('/') */
			params = strchr(server, '?');
			if(NULL != params)
			{
				*params = '\0';
			}
			/* here we have protocol + server + def port + def path */
			ret = axis2_url_create(env, protocol, server, port, "/");
			AXIS2_FREE(env->allocator, tmp_url_str);
			return ret;			
		}
		else
		{
			*path++ = '\0';
			params = strchr(path, '?');
			if(NULL != params)
			{
				*params = '\0';
			}
			/* here we have protocol + server + def port + path */
			ret = axis2_url_create(env, protocol, server, port, path);
			AXIS2_FREE(env->allocator, tmp_url_str);
			return ret;
		}
	}
	else
	{
		*port_str++ = '\0';
		path = strchr(port_str, '/');
		if(NULL == path)
		{
			params = strchr(port_str, '?');
			if(NULL != params)
			{
				*params = '\0';
			}
			port = AXIS2_ATOI(port_str);
			/* here we have protocol + server + port + def path */
			ret = axis2_url_create(env, protocol, server, port, "/");
			AXIS2_FREE(env->allocator, tmp_url_str);
			return ret;			
		}
		else
		{
			*path++ = '\0';
			port = AXIS2_ATOI(port_str);
			params = strchr(path, '?');
			if(NULL != params)
			{
				*params = '\0';
			}
			if(AXIS2_STRLEN(path) > 0)
			{
				/* here we have protocol + server + port + path */
				ret = axis2_url_create(env, protocol, server, port, path);
				AXIS2_FREE(env->allocator, tmp_url_str);
				return ret;
			}
			else
			{
				/* here we have protocol + server + port + def path */
				ret = axis2_url_create(env, protocol, server, port, "/");
				AXIS2_FREE(env->allocator, tmp_url_str);
				return ret;
			}
		}
	}
}

axis2_status_t AXIS2_CALL 
axis2_url_free (axis2_url_t *url, const axis2_env_t *env)
{
    axis2_url_impl_t *url_impl = NULL;
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    url_impl = AXIS2_INTF_TO_IMPL(url);
    if(NULL != url_impl->protocol)
    {
        AXIS2_FREE(env->allocator, url_impl->protocol);
        url_impl->protocol = NULL;
    }
    if(NULL != url_impl->server)
    {
        AXIS2_FREE(env->allocator, url_impl->server);
        url_impl->server = NULL;
    }
	if(NULL != url_impl->path)
    {
        AXIS2_FREE(env->allocator, url_impl->path);
        url_impl->path = NULL;
    }
	url_impl->port = -1;
    if(NULL != url->ops)
        AXIS2_FREE(env->allocator, url->ops);
    
	AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(url));
	return AXIS2_SUCCESS;
}


axis2_char_t* AXIS2_CALL 
axis2_url_to_external_form (axis2_url_t *url, 
                const axis2_env_t *env)
{
    axis2_url_impl_t *url_impl = NULL;
    axis2_char_t *external_form = NULL;
    axis2_ssize_t len = 0;
    axis2_char_t port_str[8];
    AXIS2_ENV_CHECK(env, NULL);
    url_impl = AXIS2_INTF_TO_IMPL(url);
    sprintf(port_str, "%d", url_impl->port); 
    len = AXIS2_STRLEN(url_impl->protocol) + 
            AXIS2_STRLEN(url_impl->server) + AXIS2_STRLEN(url_impl->path) + 
				strlen(port_str) + 7; 
    external_form = (axis2_char_t*) AXIS2_MALLOC(env->allocator,
                len);
    sprintf(external_form, "%s://%s:%s%s", url_impl->protocol, url_impl->server,
                port_str, url_impl->path);
    return external_form;
}


axis2_status_t AXIS2_CALL 
axis2_url_set_protocol(axis2_url_t *url, const axis2_env_t *env, 
						axis2_char_t *protocol)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK(env->error, protocol, AXIS2_FAILURE);
    if(NULL != AXIS2_INTF_TO_IMPL(url)->protocol)
	{
		AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(url)->protocol);
		AXIS2_INTF_TO_IMPL(url)->protocol = NULL;
	}
	AXIS2_INTF_TO_IMPL(url)->protocol = AXIS2_STRDUP(protocol, env);
	return AXIS2_SUCCESS;
}


axis2_char_t* AXIS2_CALL 
axis2_url_get_protocol (axis2_url_t *url, const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(url)->protocol;
}

axis2_status_t AXIS2_CALL 
axis2_url_set_server(axis2_url_t *url, const axis2_env_t *env, 
						axis2_char_t *server)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK(env->error, server, AXIS2_FAILURE);
    if(NULL != AXIS2_INTF_TO_IMPL(url)->server)
	{
		AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(url)->server);
		AXIS2_INTF_TO_IMPL(url)->server = NULL;
	}
	AXIS2_INTF_TO_IMPL(url)->server = AXIS2_STRDUP(server, env);
	return AXIS2_SUCCESS;
}


axis2_char_t* AXIS2_CALL 
axis2_url_get_server (axis2_url_t *url, const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(url)->server;
}

axis2_status_t AXIS2_CALL 
axis2_url_set_port(axis2_url_t *url, const axis2_env_t *env, 
						int port)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(url)->port = port;
	return AXIS2_SUCCESS;
}


int AXIS2_CALL 
axis2_url_get_port (axis2_url_t *url, const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_CRTICAL_FAILURE);
    return AXIS2_INTF_TO_IMPL(url)->port;
}

axis2_status_t AXIS2_CALL 
axis2_url_set_path(axis2_url_t *url, const axis2_env_t *env, 
						axis2_char_t *path)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK(env->error, path, AXIS2_FAILURE);
    if(NULL != AXIS2_INTF_TO_IMPL(url)->path)
	{
		AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(url)->path);
		AXIS2_INTF_TO_IMPL(url)->path = NULL;
	}
	AXIS2_INTF_TO_IMPL(url)->path = AXIS2_STRDUP(path, env);
	return AXIS2_SUCCESS;
}


axis2_char_t* AXIS2_CALL 
axis2_url_get_path (axis2_url_t *url, const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(url)->path;
}

axis2_url_t* AXIS2_CALL
axis2_url_clone(
        axis2_url_t *url,
        const axis2_env_t *env)
{
    axis2_url_impl_t *url_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    url_impl = AXIS2_INTF_TO_IMPL(url);

    return axis2_url_create(env, url_impl->protocol,
                        url_impl->server,
                        url_impl->port,
                        url_impl->path);
}


