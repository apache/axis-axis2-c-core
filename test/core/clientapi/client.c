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
    int sockfd, portno, n, i;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    struct stat buf;


    char *buffer;
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
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

    int tmp;
    tmp = stat("soap_req", &buf);
    printf("%d\n", buf.st_size);
    buffer = (char *) malloc(buf.st_size* sizeof(char));
    int fd = open("soap_req", O_RDONLY,0);
    if (fd == -1)
    {
        printf("can't open file soap_req\n");
        return;
    }
    else
        printf("opened file soap_req\n");
	
    while((i = read(fd, buffer, BUFSIZ)) > 0)
    {
    	n = write(sockfd,buffer,strlen(buffer));
	printf("%s", buffer);
    	if (n < 0) 
       	    error("ERROR writing to socket");
    }

/*    bzero(buffer,2000);*/
    buffer = '\0';
    n = read(sockfd,buffer,BUFSIZ);
    printf("%s\n", buffer);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);

    return 0;
}
