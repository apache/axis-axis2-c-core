#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    char *hostname = "localhost";
    char *port = "9090";
    char *filename = "soap_req";
    char* endpoint = "/axis2/services/echo/echo";
    char c;
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

	write_to_socket(hostname, port, filename);
    return 0;
}

int write_to_socket(char *address, char* port, char* filename)
{
    int sockfd, portno, n, i;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    struct stat buf;
    char *buffer;
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
    int fd = open(filename, O_RDONLY, 0);
    if (fd == -1)
    {
        printf("can't open file soap_req\n");
        return;
    }
    else
        printf("opened file soap_req\n");
	
    while((i = read(fd, buffer, bufsize - 1)) > 0)
    {
        buffer[i] = '\0';
    	n = write(sockfd,buffer,strlen(buffer));
	    printf("buffer = %s %d...", buffer, i);
    	if (n < 0) 
       	    error("ERROR writing to socket");
    }

    printf("Done writing to server\n");

/*    bzero(buffer,2000);*/
    buffer[0] = '\0';
	
    while((n = read(sockfd, buffer, bufsize -1)) > 0)
    {
        buffer[i] = '\0';
        printf("%s\n", buffer);
    }

    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
}

