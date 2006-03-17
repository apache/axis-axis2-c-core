#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <axis2.h>
#include <stdlib.h>

/* Function prototypes */
int write_to_socket(char *address, char* port, char* filename, char* endpoint);
/* End of function prototypes */

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    axis2_char_t *hostname = "localhost";
    axis2_char_t *port = "9090";
    axis2_char_t *filename = "echo.xml";
    axis2_char_t *endpoint = "/axis2/services/echo/echo";
    int c;
    extern char *optarg;
	
    while ((c = getopt(argc, argv, ":h:p:f:e:")) != -1) 
    {
        switch(c) 
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
	
	write_to_socket(hostname, port, filename, endpoint);
    return 0;
}

int write_to_socket(char *address, char* port, char* filename, char* endpoint)
{
	axis2_char_t buffer_l[4999];
    int sockfd, portno, n, i;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    struct stat buf;
    axis2_char_t *buffer;
	axis2_char_t tmpstr[10];
    int bufsize = 0;
	
	portno = atoi(port);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(address);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    /* Read from file */
    stat(filename, &buf);
    bufsize = (buf.st_size + 1) * sizeof(char);
    buffer = (char *) malloc(bufsize);
    int fd = open(filename, O_RDONLY, 0);
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
	sprintf(buffer_l, "POST %s HTTP/1.1\r\nUser-Agent: Axis/2.0/C\r\nConnection: Keep-Alive\r\nHost: ", endpoint);
	strcat(buffer_l, address);
	strcat(buffer_l, ":");
	strcat(buffer_l, port);
	strcat(buffer_l, "\r\n");
	strcat(buffer_l, "Content-Length: ");
	sprintf(tmpstr, "%d", strlen(buffer));
	strcat(buffer_l, tmpstr);
	strcat(buffer_l, "\r\n");
	/*strcat(buffer_l, "SOAPAction: http://localhost:9090/axis2/services/echo/echo\r\n");*/
	strcat(buffer_l, "Content-Type: application/soap+xml;\r\n");
	strcat(buffer_l, "\r\n");

	
    printf("Writing buffer_1...\n%s", buffer_l);
	n = write(sockfd, buffer_l, strlen(buffer_l));

    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
       	error("ERROR writing to socket");

    printf("Done writing to server\n");

    buffer[0] = '\0';
	
    printf("Reading the reply from server :\n");
    while((n = read(sockfd, buffer, bufsize - 1)) > 0)
    {
        buffer[n] = '\0';
        printf("%s", buffer);
    }
    printf("\nReading from server done ...\n");

    if (n < 0)
    {
        error("ERROR reading from socket");
        buffer[0] = '\0';
    }
	free(buffer);
    return 0;
}

