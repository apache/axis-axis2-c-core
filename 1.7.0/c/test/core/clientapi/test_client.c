
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

#include <platforms/axutil_platform_auto_sense.h>
#include <axis2_const.h>
#include <cut_defs.h>
#include <cut_http_server.h>

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

void
error(
    const char *msg)
{
    perror(msg);
    exit(-1);
}

void
axis2_test_temp(axutil_env_t *env)
{
    /* To avoid warning of not using cut_ptr_equal */
    CUT_ASSERT_PTR_EQUAL(NULL, NULL, 0);
    /* To avoid warning of not using cut_int_equal */
    CUT_ASSERT_INT_EQUAL(0, 0, 0);
    /* To avoid warning of not using cut_str_equal */
    CUT_ASSERT_STR_EQUAL("", "", 0);
}

int
main(
    int argc,
    char *argv[])
{
    const axis2_char_t *hostname = "localhost";
    const axis2_char_t *port = "9090";
    const axis2_char_t *filename = "echo.xml";
    const axis2_char_t *endpoint = "/axis2/services/echo/echo";
    axutil_env_t *env = NULL;
	
#ifndef WIN32
    int c;
    extern char *optarg;

    while ((c = getopt(argc, argv, ":h:p:f:e:")) != -1)
    {
        switch (c)
        {
        case 'h':
            hostname = optarg;
            break;
        case 'p':
            port = optarg;
            break;
        case 'f':
            filename = optarg;
            break;
        case 'e':
            endpoint = optarg;
            break;
        }
    }
#endif
  env = cut_setup_env("test HTTP server");
  CUT_ASSERT(env != NULL);
  if ( ut_start_http_server(env) != 0 ) return -1;
  write_to_socket(hostname, port, filename, endpoint);
  ut_stop_http_server(env);

  axis2_test_temp(env);
  CUT_RETURN_ON_FAILURE(-1);
  return 0;
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
