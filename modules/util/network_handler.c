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
#include <axis2_network_handler.h>
#include <fcntl.h>


#if defined(WIN32)
static int is_init_socket = 0;
axis2_bool_t axis2_init_socket();
#endif

AXIS2_DECLARE(axis2_socket_t )
axis2_network_handler_open_socket(axis2_env_t **env, char *server, int port)
{
	axis2_socket_t sock = AXIS2_INVALID_SOCKET;
	struct sockaddr_in sock_addr;
	struct linger ll;
	int nodelay = 1;

#if defined(WIN32)
if (is_init_socket == 0)
{
	axis2_init_socket();
	is_init_socket = 1;
}
#endif

    AXIS2_ENV_CHECK(env, AXIS2_CRTICAL_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, server, AXIS2_CRTICAL_FAILURE);

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		/*nnn AF_INET is not defined in sys/socket.h but PF_INET*/
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SOCKET_ERROR, AXIS2_FAILURE);
		return AXIS2_INVALID_SOCKET;
	}
	
    memset(&sock_addr,0,sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(server);/*nnn arpa/inet.d */

    if (sock_addr.sin_addr.s_addr == AXIS2_INADDR_NONE) /*nnn netinet/in.h */
    {
        /**
         * server may be a host name
         */
        struct hostent* lphost = NULL;
        lphost = gethostbyname(server); /*nnn netdb.h*/

        if (NULL != lphost)
            sock_addr.sin_addr.s_addr = 
						((struct in_addr*)lphost->h_addr)->s_addr;
        else
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_ADDRESS, 
						AXIS2_FAILURE);
            return AXIS2_INVALID_SOCKET;
        }
    }

    sock_addr.sin_port = htons((axis2_unsigned_short_t)port);
	/** Connect to server */
    if (connect(sock, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0)
    {
        AXIS2_CLOSE_SOCKET(sock);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SOCKET_ERROR, AXIS2_FAILURE);
        return AXIS2_INVALID_SOCKET;
    }
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &nodelay, 
						sizeof(nodelay));
	ll.l_onoff = 1;
    ll.l_linger = 5;
    setsockopt(sock, SOL_SOCKET, SO_LINGER , &ll, sizeof(struct linger));
	return sock;
}

AXIS2_DECLARE( axis2_socket_t )
axis2_network_handler_create_server_socket(axis2_env_t **env, int port)
{
	axis2_socket_t sock = AXIS2_INVALID_SOCKET;
	axis2_socket_t i = 0;
	struct sockaddr_in sock_addr;
		
	AXIS2_ENV_CHECK(env, AXIS2_CRTICAL_FAILURE);
#if defined(WIN32)
    if (is_init_socket == 0)
    {
	    axis2_init_socket();
	    is_init_socket = 1;
    }
#endif
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SOCKET_ERROR, AXIS2_FAILURE);
		return AXIS2_INVALID_SOCKET;
	}
	/** Address re-use */
	i = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR ,(char*) &i, 
						sizeof(axis2_socket_t));/*nnn casted 4th param to char* */
	/** Exec behaviour */
	AXIS2_CLOSE_SOCKET_ON_EXIT(sock)
	
    memset(&sock_addr,0,sizeof(sock_addr));

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons((axis2_unsigned_short_t)port);

    /* Bind the socket to our port number */
    if (bind(sock, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SOCKET_BIND_FAILED, 
						AXIS2_FAILURE);
		return AXIS2_INVALID_SOCKET;
	}
    if(listen(sock, 50) < 0)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SOCKET_LISTEN_FAILED,
                        AXIS2_FAILURE);
        return AXIS2_INVALID_SOCKET;
    }
	return sock;
}

AXIS2_DECLARE(axis2_status_t )
axis2_network_handler_close_socket (axis2_env_t **env,  axis2_socket_t socket)
{
	AXIS2_ENV_CHECK(env, AXIS2_CRTICAL_FAILURE);
	if(socket < 0)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_SOCKET,
							AXIS2_FAILURE);
		return AXIS2_FAILURE;
	}
	if(0 != AXIS2_CLOSE_SOCKET(socket))
	{
		return AXIS2_FAILURE;
	}
	return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_status_t)
axis2_network_handler_set_sock_option(axis2_env_t **env, axis2_socket_t socket, 
						int option, int value)
{
	if(option == SO_RCVTIMEO || option == SO_SNDTIMEO)
	{
		struct timeval tv;
		/* we deal with milliseconds */
		tv.tv_sec = value/1000;
		tv.tv_usec = (value%1000) * 1000;
		setsockopt(socket, SOL_SOCKET, option, (char*) &tv, sizeof(tv));
		return AXIS2_SUCCESS;
	}
	return AXIS2_FAILURE;    
}

AXIS2_DECLARE(axis2_socket_t)						
axis2_network_handler_svr_socket_accept(axis2_env_t **env, 
						axis2_socket_t svr_socket)
{
	struct sockaddr cli_addr;
	
	struct linger ll;
	int nodelay = 1;
	axis2_socket_len_t cli_len = 0;
	axis2_socket_t cli_socket = AXIS2_INVALID_SOCKET;

	AXIS2_ENV_CHECK(env, AXIS2_CRTICAL_FAILURE);
	
	cli_len = sizeof(cli_addr);
	cli_socket = accept(svr_socket, (struct sockaddr *)&cli_addr, &cli_len);
    if (cli_socket < 0)
    	AXIS2_LOG_WRITE((*env)->log, "[Axis2][network_handler] Socket accept \
						failed", AXIS2_LOG_LEVEL_ERROR);
	
	setsockopt(cli_socket, IPPROTO_TCP, TCP_NODELAY, &nodelay, 
						sizeof(nodelay));
	ll.l_onoff = 1;
    ll.l_linger = 5;
    setsockopt(cli_socket, SOL_SOCKET, SO_LINGER , &ll, sizeof(struct linger));
    return cli_socket;
}

#if defined (WIN32)
axis2_bool_t axis2_init_socket()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	int sock_opt = SO_SYNCHRONOUS_NONALERT;
 
	wVersionRequested = MAKEWORD( 2, 2 );
 
	err = WSAStartup( wVersionRequested, &wsaData );

	if ( err != 0 ) 
		return 0; //WinSock 2.2 not found
 

	// Confirm that the WinSock DLL supports 2.2.
	// Note that if the DLL supports versions greater    
	// than 2.2 in addition to 2.2, it will still return 
	// 2.2 in wVersion since that is the version we      
	// requested.                                        
 
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
			HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		WSACleanup( );
		return 0; //WinSock 2.2 not supported 
	}
	/**
	 *	Enable the use of sockets as filehandles
	 */
	setsockopt(INVALID_SOCKET, SOL_SOCKET, SO_OPENTYPE, (char *)&sock_opt,
						sizeof(sock_opt));
	return 1;
}
#endif


AXIS2_DECLARE(axis2_char_t*)
axis2_network_handler_get_svr_ip(axis2_env_t **env, 
                                 axis2_socket_t socket)
{
    struct sockaddr_in addr;
    
    axis2_socket_len_t len = sizeof(addr);
    char *ret = NULL;
    memset(&addr, 0, sizeof(addr));
    if(getsockname(socket, (struct sockaddr *)&addr, &len) < 0)
    {
        return NULL;
    }
    ret = inet_ntoa(addr.sin_addr);
    return ret;
}
