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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <axis2_network_handler.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>

int AXIS2_CALL
axis2_network_handler_open_socket(axis2_env_t **env, char *server, int port)
{
	int sock = -1;
	struct sockaddr_in sock_addr;
	
    AXIS2_PARAM_CHECK((*env)->error, server, AXIS2_CRTICAL_FAILURE);
	
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SOCKET_ERROR, AXIS2_FAILURE);
		return -1;
	}
	
    memset(&sock_addr,0,sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(server);

    if (sock_addr.sin_addr.s_addr == (in_addr_t)-1)
    {
        /**
         * server may be a host name
         */
        struct hostent* lphost = NULL;
        lphost = gethostbyname(server);

        if (NULL != lphost)
            sock_addr.sin_addr.s_addr = ((struct in_addr*)lphost->h_addr)->s_addr;
        else
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_ADDRESS, 
						AXIS2_FAILURE);
            return -1;
        }
    }

    sock_addr.sin_port = htons((uint16_t)port);
	/** Connect to server */
    if (connect(sock, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0)
    {
        close(sock);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SOCKET_ERROR, AXIS2_FAILURE);
        return -1;
    }
	return sock;
}

int AXIS2_CALL
axis2_network_handler_create_server_socket(axis2_env_t **env, int port)
{
	int sock = -1;
	unsigned int i = 0;
	struct sockaddr_in sock_addr;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SOCKET_ERROR, AXIS2_FAILURE);
		return -1;
	}
	/** Address re-use */
	i = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR ,&i, sizeof(int));
	/** Exec behaviour */
	fcntl(sock, F_SETFD, FD_CLOEXEC);
	
	
    memset(&sock_addr,0,sizeof(sock_addr));

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons((uint16_t)port);

    /* Bind the socket to our port number */
    if (bind(sock, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SOCKET_BIND_FAILED, 
						AXIS2_FAILURE);
		return -1;
	}
	return sock;
}

axis2_status_t AXIS2_CALL
axis2_network_handler_close_socket (axis2_env_t **env, int socket)
{
	if(socket < 0)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_SOCKET,
							AXIS2_FAILURE);
		return AXIS2_FAILURE;
	}
	if(0 != close(socket))
	{
		return AXIS2_FAILURE;
	}
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_network_handler_set_sock_option(axis2_env_t **env, int socket, 
						int option, int value)
{
	if(option == SO_RCVTIMEO || option == SO_SNDTIMEO)
	{
		struct timeval tv;
		/* we deal with milliseconds */
		tv.tv_sec = value/1000;
		tv.tv_usec = (value%1000) * 1000;
		setsockopt(socket, SOL_SOCKET, option, &tv, sizeof(tv));
		return AXIS2_SUCCESS;
	}
	return AXIS2_FAILURE;    
}
