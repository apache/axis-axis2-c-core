
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

#include <gtest/gtest.h>

#include <platforms/axutil_platform_auto_sense.h>
#include <axis2_const.h>
#include "../../cutest/include/cut_http_server.h"

#if defined (WIN32)
#define bcopy(s, d, n) 	memcpy(d, s, n)
#define bzero(s, n)  	memset(s, 0, n)
#endif

/* Function prototypes */
int write_to_socket(
    const char *address,
    const char *port,
    const char *filename,
    const char *endpoint);

/* End of function prototypes */


class TestClient: public ::testing::Test
{

    protected:
        void SetUp()
        {

            m_allocator = axutil_allocator_init(NULL);
            m_axis_log = axutil_log_create(m_allocator, NULL, NULL);
            m_error = axutil_error_create(m_allocator);

            m_env = axutil_env_create_with_error_log(m_allocator, m_error, m_axis_log);

        }

        void TearDown()
        {
            axutil_env_free(m_env);
        }


        axutil_allocator_t *m_allocator = NULL;
        axutil_env_t *m_env = NULL;
        axutil_error_t *m_error = NULL;
        axutil_log_t *m_axis_log = NULL;

};

/* Note: This test fails unless you have a deployed axis2c instance running the
 * echo service on the appropriate port, and AXIS2C_HOME defined in your
 * environment */
TEST_F(TestClient, test_client)
{
    const axis2_char_t *hostname = "localhost";
    const axis2_char_t *port = "9090";
    const axis2_char_t *filename = "echo.xml";
    const axis2_char_t *endpoint = "/axis2/services/echo/echo";
    axutil_env_t *env = NULL;
    int server_status;

    server_status = ut_start_http_server(m_env);
    ASSERT_EQ(server_status, 0);
    write_to_socket(hostname, port, filename, endpoint);
    ut_stop_http_server(m_env);

}

void
error(
    const char *msg)
{
    perror(msg);
    exit(-1);
}


#define TEST_BUF_LEN	4999
int
write_to_socket(
    const char *address,
    const char *port,
    const char *filename,
    const char *endpoint)
{
    axis2_char_t buffer_l[TEST_BUF_LEN];
    int sockfd, portno, n, i, fd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    struct stat buf;
    axis2_char_t *buffer;
    axis2_char_t tmpstr[10];
    int bufsize = 0;
    int reste;
    axis2_char_t *pbuf;

    portno = atoi(port);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(address);
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,
          (char *) &serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    /* Read from file */
    stat(filename, &buf);
    bufsize = (buf.st_size + 1) * sizeof(char);
    buffer = (char *) malloc(bufsize);
    fd = open(filename, O_RDONLY, 0);
    if (fd == -1)
    {
        printf("can't open file %s\n", filename);
        return -1;
    }
    else
        printf("opened file %s\n", filename);

    i = read(fd, buffer, bufsize - 1);
    if (i > 0)
    {
        buffer[i] = '\0';
        printf("%s...\n", buffer);
    }
    sprintf(buffer_l,
            "POST %s HTTP/1.1\r\nUser-Agent: Axis/2.0/C\r\nConnection: Keep-Alive\r\nHost: ",
            endpoint);
    strcat(buffer_l, address);
    strcat(buffer_l, ":");
    strcat(buffer_l, port);
    strcat(buffer_l, "\r\n");
    strcat(buffer_l, "Content-Length: ");
    sprintf(tmpstr, "%d", (int) strlen(buffer));
    strcat(buffer_l, tmpstr);
    strcat(buffer_l, "\r\n");
    /*strcat(buffer_l, "SOAPAction: http://localhost:9090/axis2/services/echo/echo\r\n"); */
    strcat(buffer_l, "Content-Type: application/soap+xml;\r\n");
    strcat(buffer_l, "\r\n");

    printf("Writing buffer_1...\n%s", buffer_l);
    n = send(sockfd, buffer_l, strlen(buffer_l), 0);

    n = send(sockfd, buffer, strlen(buffer), 0);
    if (n < 0)
        error("ERROR writing to socket");

    printf("Done writing to server\n");

    buffer[0] = '\0';
    pbuf = buffer_l;
    reste = TEST_BUF_LEN - 1;
 	printf("Read server response %s\n", filename);
    while ((n = recv(sockfd, pbuf, reste, 0)) > 0)
    {
        pbuf += n;
        reste -= n;
    }
    buffer_l[pbuf - buffer_l] = '\0';

    printf("\nReading from server done ...\n");

    if (n < 0)
    {
        error("ERROR reading from socket");
        buffer[0] = '\0';
    }
    
    free(buffer);
    return 0;
}
