#include <stdio.h>
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

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    axis2_char_t *hostname = "localhost";
    axis2_char_t *port = "9090";
    axis2_char_t *filename = "soap_req";
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
    if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    stat(filename, &buf);
    bufsize = buf.st_size* sizeof(char);
    buffer = (char *) malloc(bufsize);
	sprintf(buffer_l, "POST %s HTTP/1.1\r\nUser-Agent: Axis/2.0/C\r\nConnection: Keep-Alive\r\nHost: ", endpoint);
	strcat(buffer_l, address);
	strcat(buffer_l, ":");
	strcat(buffer_l, port);
	strcat(buffer_l, "\r\n");
	strcat(buffer_l, "Content-Length: ");
	sprintf(tmpstr, "%d", buf.st_size - 1);
	strcat(buffer_l, tmpstr);
	strcat(buffer_l, "\r\n");
	strcat(buffer_l, "Content-Type: application/soap+xml;\r\n");
	strcat(buffer_l, "\r\n");

    int fd = open(filename, O_RDONLY, 0);
    if (fd == -1)
    {
        printf("can't open file %s\n", filename);
        return;
    }
    else
        printf("opened file %s\n", filename);
	
    printf("Writing buffer_1...\n%s", buffer_l);
	n = write(sockfd, buffer_l, strlen(buffer_l));

    /*while((i = read(fd, buffer, bufsize - 1)) > 0)
    {*/
    i = read(fd, buffer, bufsize - 1);
        buffer[i] = '\0';
        printf("%s...\n", buffer);
    	n = write(sockfd,buffer,strlen(buffer));
    	if (n < 0) 
       	    error("ERROR writing to socket");
    	/*n = write(sockfd,"\r\n",2);
    	if (n < 0) 
       	    error("ERROR writing to socket");*/
    /*}*/

    printf("Done writing to server\n");

    buffer[0] = '\0';
	
    while((n = read(sockfd, buffer, bufsize - 1)) > 0)
    {
        buffer[n] = '\0';
        printf("%s %d\n", buffer, n);
    }

    if (n < 0)
    {
        error("ERROR reading from socket");
        buffer[0] = '\0';
    }
    printf("%s\n",buffer);
	free(buffer);
}

