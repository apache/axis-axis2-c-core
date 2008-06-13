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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <axutil_network_handler.h>
#include <fcntl.h>

#define AXUTIL_NETWORK_HANDLER_ERRBUFSIZE 256 

#ifdef WIN32
static void AXIS2_CALL
axutil_network_handler_get_win32_error_message(const axutil_env_t *env, 
											   axis2_char_t *buff,
											   unsigned int buf_size);
#endif

#if defined(WIN32)
/* fix for an older version of winsock2.h */
#if !defined(SO_EXCLUSIVEADDRUSE)
#define SO_EXCLUSIVEADDRUSE ((int)(~SO_REUSEADDR))
#endif
#endif

#if defined(WIN32)
static int is_init_socket = 0;
axis2_bool_t axis2_init_socket(
    );
#endif

AXIS2_EXTERN axis2_socket_t AXIS2_CALL
axutil_network_handler_open_socket(
    const axutil_env_t *env,
    char *server,
    int port)
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

    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
    AXIS2_PARAM_CHECK(env->error, server, AXIS2_INVALID_SOCKET);

#ifndef WIN32
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        /*AF_INET is not defined in sys/socket.h but PF_INET */
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SOCKET_ERROR, AXIS2_FAILURE);
        return AXIS2_INVALID_SOCKET;
    }
#else
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
        /* In Win 32 if the socket creation failed it return 0 not a negative value */
    {
		char buf[AXUTIL_NETWORK_HANDLER_ERRBUFSIZE]; 
		/* Get the detailed error message */
		axutil_network_handler_get_win32_error_message(env, buf, 
			AXUTIL_NETWORK_HANDLER_ERRBUFSIZE);	

		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, buf); 
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SOCKET_ERROR, AXIS2_FAILURE);
        return AXIS2_INVALID_SOCKET;
    }
#endif 

    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(server);  /*arpa/inet.d */

    if (sock_addr.sin_addr.s_addr == AXIS2_INADDR_NONE) /*netinet/in.h */
    {
        /*
         * server may be a host name
         */
        struct hostent *lphost = NULL;
        lphost = gethostbyname(server);

        if (lphost)
        {
            sock_addr.sin_addr.s_addr =
                ((struct in_addr *) lphost->h_addr)->s_addr;
        }
        else
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_ADDRESS,
                            AXIS2_FAILURE);
            return AXIS2_INVALID_SOCKET;
        }
    }

    sock_addr.sin_port = htons((axis2_unsigned_short_t) port);

    /* Connect to server */
    if (connect(sock, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0)
    {
        AXIS2_CLOSE_SOCKET(sock);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SOCKET_ERROR, AXIS2_FAILURE);
        return AXIS2_INVALID_SOCKET;
    }
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *) &nodelay,
               sizeof(nodelay));
    ll.l_onoff = 1;
    ll.l_linger = 5;
    setsockopt(sock, SOL_SOCKET, SO_LINGER, (const char *) &ll,
               sizeof(struct linger));
    return sock;
}

AXIS2_EXTERN axis2_socket_t AXIS2_CALL
axutil_network_handler_create_server_socket(
    const axutil_env_t *env,
    int port)
{
    axis2_socket_t sock = AXIS2_INVALID_SOCKET;
    axis2_socket_t i = 0;
    struct sockaddr_in sock_addr;

    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
#if defined(WIN32)
    if (is_init_socket == 0)
    {
        axis2_init_socket();
        is_init_socket = 1;
    }
#endif
    sock = socket(AF_INET, SOCK_STREAM, 0);

#ifndef WIN32
    if (sock < 0)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SOCKET_ERROR, AXIS2_FAILURE);
        return AXIS2_INVALID_SOCKET;
    }
#else
	if (sock == INVALID_SOCKET)
	{
		axis2_char_t buf[AXUTIL_NETWORK_HANDLER_ERRBUFSIZE];
		axutil_network_handler_get_win32_error_message(env, buf, AXUTIL_NETWORK_HANDLER_ERRBUFSIZE);
		AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SOCKET_ERROR, AXIS2_FAILURE);
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, buf); 
        return AXIS2_INVALID_SOCKET;
	}
#endif
    /* Address re-use */
    i = 1;
#if defined(WIN32)
    setsockopt(sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char *) &i, sizeof(axis2_socket_t));    /*casted 4th param to char* */
#else
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &i, sizeof(axis2_socket_t));    /*casted 4th param to char* */
#endif

    /* Exec behaviour */
    AXIS2_CLOSE_SOCKET_ON_EXIT(sock) memset(&sock_addr, 0, sizeof(sock_addr));

    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons((axis2_unsigned_short_t) port);

    /* Bind the socket to our port number */
    if (bind(sock, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SOCKET_BIND_FAILED,
                        AXIS2_FAILURE);
        return AXIS2_INVALID_SOCKET;
    }
    if (listen(sock, 50) < 0)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SOCKET_LISTEN_FAILED,
                        AXIS2_FAILURE);
        return AXIS2_INVALID_SOCKET;
    }
    return sock;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_network_handler_close_socket(
    const axutil_env_t *env,
    axis2_socket_t socket)
{
    int i = 0;
    char buf[32];
    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
    if (socket < 0)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_SOCKET, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    shutdown(socket, AXIS2_SHUT_WR);
    axutil_network_handler_set_sock_option(env, socket, SO_RCVTIMEO, 1);
    i = recv(socket, buf, 32, 0);
    AXIS2_CLOSE_SOCKET(socket);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_network_handler_set_sock_option(
    const axutil_env_t *env,
    axis2_socket_t socket,
    int option,
    int value)
{
    if (option == SO_RCVTIMEO || option == SO_SNDTIMEO)
    {
#if defined(WIN32)
        DWORD tv = value;       /* windows expects milliseconds in a DWORD */
#else
        struct timeval tv;
        /* we deal with milliseconds */
        tv.tv_sec = value / 1000;
        tv.tv_usec = (value % 1000) * 1000;
#endif
        setsockopt(socket, SOL_SOCKET, option, (char *) &tv, sizeof(tv));
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}

AXIS2_EXTERN axis2_socket_t AXIS2_CALL
axutil_network_handler_svr_socket_accept(
    const axutil_env_t *env,
    axis2_socket_t svr_socket)
{
    struct sockaddr cli_addr;
    struct linger ll;
    int nodelay = 1;
    axis2_socket_len_t cli_len = 0;
    axis2_socket_t cli_socket = AXIS2_INVALID_SOCKET;
    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);

    cli_len = sizeof(cli_addr);
    cli_socket = accept(svr_socket, (struct sockaddr *) &cli_addr, &cli_len);
#ifndef WIN32
    if (cli_socket < 0)
	{
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "[Axis2][network_handler] Socket accept \
                failed");
	}
#else
	if (cli_socket == INVALID_SOCKET)
	{
		axis2_char_t buf[AXUTIL_NETWORK_HANDLER_ERRBUFSIZE];
		axutil_network_handler_get_win32_error_message(env, buf, AXUTIL_NETWORK_HANDLER_ERRBUFSIZE);
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, buf); 
	}
#endif 

    setsockopt(svr_socket, IPPROTO_TCP, TCP_NODELAY, (const char *)&nodelay, (int)sizeof(nodelay));
    /* We are sure that the difference lies within the int range */
    ll.l_onoff = 1;
    ll.l_linger = 5;
    setsockopt(cli_socket, SOL_SOCKET, SO_LINGER, (const char *)&ll, (int)sizeof(struct linger));
    /* We are sure that the difference lies within the int range */
    return cli_socket;
}

#if defined (WIN32)
axis2_bool_t
axis2_init_socket(
    )
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);

    if (err != 0)
        return 0;               /* WinSock 2.2 not found */

    /* Confirm that the WinSock DLL supports 2.2. 
     * Note that if the DLL supports versions greater 
     * than 2.2 in addition to 2.2, it will still return 
     * 2.2 in wVersion since that is the version we 
     * requested.
     */

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        WSACleanup();
        return 0;               /* WinSock 2.2 not supported */
    }
    return 1;
}
#endif

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_network_handler_get_svr_ip(
    const axutil_env_t *env,
    axis2_socket_t socket)
{
    struct sockaddr_in addr;
    axis2_socket_len_t len = sizeof(addr);
    char *ret = NULL;
    memset(&addr, 0, sizeof(addr));
    if (getsockname(socket, (struct sockaddr *) &addr, &len) < 0)
    {
        return NULL;
    }
    ret = inet_ntoa(addr.sin_addr);
    return ret;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_network_handler_get_peer_ip(
    const axutil_env_t *env,
    axis2_socket_t socket)
{
    struct sockaddr_in addr;
    axis2_socket_len_t len = sizeof(addr);
    char *ret = NULL;
    memset(&addr, 0, sizeof(addr));
    if (getpeername(socket, (struct sockaddr *) &addr, &len) < 0)
    {
        return NULL;
    }
    ret = inet_ntoa(addr.sin_addr);
    return ret;
}

#ifdef WIN32
static void AXIS2_CALL
axutil_network_handler_get_win32_error_message(const axutil_env_t *env, 
											   axis2_char_t *buf, 
											   unsigned int buf_size)
{
	LPVOID lpMsgBuf;	
	int rc = WSAGetLastError();
	sprintf( buf, "Winsock error %d: ", rc );
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				  NULL,
	              rc,
	              MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
				  (LPTSTR) &lpMsgBuf,
				  0,
				  NULL
				);

	strncat( buf, (char*)lpMsgBuf, buf_size - strlen( buf ) - 1 );
	LocalFree( lpMsgBuf );
}
#endif