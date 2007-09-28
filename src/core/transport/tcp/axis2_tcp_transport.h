
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

#ifndef AXIS2_TCP_TRANSPORT_H
#define AXIS2_TCP_TRANSPORT_H

#include <axis2_const.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** @defgroup axis2_core_trans_tcp tcp transport
     * @ingroup axis2_transport
     * Description.
     * @{
     */

    /**
     * @defgroup axis2_core_transport_tcp core tcp transport
     * @ingroup axis2_core_trans_tcp
     * @{
     */

    /**
     * @brief TCP protocol and message context constants.
     *
     */
#define AXIS2_TCP_OUT_TRANSPORT_INFO "TCPOutTransportInfo"

    /**
     * PROTOCOL_VERSION
     */
#define AXIS2_TCP_PROTOCOL_VERSION "PROTOCOL"

    /**
     * SOCKET
     */
#define AXIS2_SOCKET "SOCKET"

    /**
     * HEADER_HOST
     */
#define AXIS2_TCP_HOST "Host"

    /**
     * SO_TIMEOUT
     */
#define AXIS2_TCP_SO_TIMEOUT "SO_TIMEOUT"

    /**
     * CONNECTION_TIMEOUT
     */
#define AXIS2_TCP_CONNECTION_TIMEOUT "CONNECTION_TIMEOUT"

    /**
     * DEFAULT_SO_TIMEOUT
     */
#define AXIS2_TCP_DEFAULT_SO_TIMEOUT 60000

    /**
     * DEFAULT_CONNECTION_TIMEOUT
     */
#define AXIS2_TCP_DEFAULT_CONNECTION_TIMEOUT 60000

    /**
     * Field TRANSPORT_TCP
     */
#define AXIS2_TRANSPORT_TCP "tcp"

#ifdef __cplusplus
}
#endif
#endif                          /* AXIS2_TCP_TRANSPORT_H */
