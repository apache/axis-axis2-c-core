
/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain count copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXUTIL_NETWORK_HANDLER_H
#define AXUTIL_NETWORK_HANDLER_H

#include <axutil_utils.h>
#include <axutil_utils_defines.h>
#include <axutil_env.h>
#include <sys/types.h>
#include <platforms/axutil_platform_auto_sense.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup axutil_network_handler network handler
     * @ingroup axis2_util
     * @{
     */

    /**
     * open a socket for a given server
     * @param server ip address or the fqn of the server
     * @param port port of the service
     * @return opened socket
     */
    AXIS2_EXTERN axis2_socket_t AXIS2_CALL

    axutil_network_handler_open_socket(
        const axutil_env_t * env,
        char *server,
        int port);

    /**
     * creates a server socket for a given port
     * @param port port of the socket to be bound
     * @return creates server socket
     */
    AXIS2_EXTERN axis2_socket_t AXIS2_CALL

    axutil_network_handler_create_server_socket(
        const axutil_env_t * env,
        int port);

    /**
     * closes a socket
     * @param opened socket that need to be closed
     * @return status code
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL

    axutil_network_handler_close_socket(
        const axutil_env_t * env,
        axis2_socket_t socket);

    /**
     * used to set up socket options such as timeouts, non-blocking ..etc
     * @param socket valid socket (obtained by socket() or similar call)
     * @param option the name of the option
     * @param value Value to be set
     * @return status of the operations as axis2_status_t
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL

    axutil_network_handler_set_sock_option(
        const axutil_env_t * env,
        axis2_socket_t socket,
        int option,
        int value);

    /**
     * Accepts remote connections for a server socket
     * @param socket valid server socket (obtained by socket() or similar call)
     * @return created socket to handle the incoming client connection
     */
    AXIS2_EXTERN axis2_socket_t AXIS2_CALL

    axutil_network_handler_svr_socket_accept(
        const axutil_env_t * env,
        axis2_socket_t socket);

    /**
     * Returns the ip address of the server associated with the socket
     * @param socket valid socket (obtained by accept() or similar call)
     * @return ip address asoociated with the socket or NULL
     */
    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axutil_network_handler_get_svr_ip(
        const axutil_env_t * env,
        axis2_socket_t socket);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axutil_network_handler_get_peer_ip(
        const axutil_env_t * env,
        axis2_socket_t socket);

	/* 
	 * Create a datagram socket. 
	 * @param env pointer to env
	 * @return a datagram socket
	 */
	AXIS2_EXTERN axis2_socket_t AXIS2_CALL
	axutil_network_handler_open_dgram_socket(const axutil_env_t *env);

	/*
	 * Send a UDP packet to the given source and port address.
	 * Read a incoming UDP packet from the port and server address.
	 * @param env pointer to the env structure
	 * @param socket a datagram socket
	 * @param buffer a buffer containing the data to be sent
	 * @param buf_len length of the buffer
	 * @param addr address of the source field
	 * @param port udp port number
	 * @return success if everything goes well
	 */
	AXIS2_EXTERN axis2_status_t AXIS2_CALL 
	axutil_network_handler_send_dgram(const axutil_env_t *env, axis2_socket_t socket, 
								 axis2_char_t *buff, int *buf_len, 
								 axis2_char_t *addr, int dest_port, int *source_port);

	/* 
	 * Read a incoming UDP packet from the port and server address.
	 * @param env pointer to the env structure
	 * @param socket a datagram socket
	 * @param buffer a buffer allocated and passed to be filled
	 * @param buf_len length of the buffer allocated. In return buffer len 
	                  contains the length of the data read
	 * @param addr address of the sender. This is a return value.
	 * @param port senders port address. Return value
	 * @return if everything goes well return success 
	 */
	AXIS2_EXTERN axis2_status_t AXIS2_CALL
	axutil_network_handler_read_dgram(const axutil_env_t *env, axis2_socket_t socket, 
									 axis2_char_t *buffer, int *buf_len,
									 axis2_char_t **addr, int *port);

	/* 
	 * Create a datagram socket to receive incoming UDP packets.
	 * @param env a pointer to the env structure
	 * @param port udp port to listen
	 * @return AXIS2_SUCCESS if everything goes well
	 */
	AXIS2_EXTERN axis2_socket_t AXIS2_CALL
	axutil_network_handler_create_dgram_svr_socket(
									const axutil_env_t *env, 
									int port);

	/* 
	 * Bind a socket to the specified address
	 * @param env a pointer to the env structure
	 * @param sock socket
	 * @param port port number to bind to
	 * @return AXIS2_SUCCESS if binding is performed
	 */
	AXIS2_EXTERN axis2_status_t AXIS2_CALL
	axutil_network_handler_bind_socket(const axutil_env_t *env, 
									axis2_socket_t sock, int port);

	/* 
	 * Create a multicast socket for listening on the given port. 
	 * @param env a pointer to the env structure
	 * @param port udp port to listen
	 * @param mul_addr multicast address to join. The address should be valid and in dotted format.
	 * @param ttl TTL value. 
	 * @return AXIS2_SUCCESS if everything goes well.s
	 */
	AXIS2_EXTERN axis2_socket_t AXIS2_CALL
	axutil_network_hadler_create_multicast_svr_socket(const axutil_env_t *env, 
									int port, axis2_char_t *mul_addr);

    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_NETWORK_HANDLER_H */
