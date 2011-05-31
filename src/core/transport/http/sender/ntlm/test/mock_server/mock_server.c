/* tcpserver.c */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <axutil_string.h>
#include <axis2_util.h>

int main()
{
    int port = 8080;
    int sock, connected, bytes_recieved , true = 1;  
    char recv_data[10024];       
    char *send_data = NULL;
    const axutil_env_t *env = NULL;
    int flip = 0;

    env = axutil_env_create_all("mock_server.log", AXIS2_LOG_LEVEL_TRACE);
    struct sockaddr_in server_addr,client_addr;    
    unsigned int sin_size;
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }

    if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int)) == -1) {
        perror("Setsockopt");
        exit(1);
    }
    
    server_addr.sin_family = AF_INET;         
    server_addr.sin_port = htons(port);     
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    bzero(&(server_addr.sin_zero),8); 

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))
                                                                   == -1) {
        perror("Unable to bind");
        exit(1);
    }

    if (listen(sock, 5) == -1) {
        perror("Listen");
        exit(1);
    }
        
    printf("\nTCPServer Waiting for client on port %d \n", port);
        fflush(stdout);


    while(1)
    {  
        sin_size = sizeof(struct sockaddr_in);

        connected = accept(sock, (struct sockaddr *)&client_addr, &sin_size);

        printf("\nGot a connection from (%s , %d) \n",
               inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

        while (1)
        {
            int mark = 0;
            bytes_recieved = recv(connected,recv_data,10024,0);
            recv_data[bytes_recieved] = '\0';
            if(strstr(recv_data, "GET")) 
            {
                mark = 1;
            }
            if( mark == 1 && strstr(recv_data, "Authorization: NTLM "))
            {
                if(flip == 0)
                {
                    send_data = "HTTP/1.1 401 Unauthorized\n"\
                        "Content-Type: text/html; charset=us-ascii\n"\
                        "Server: Microsoft-HTTPAPI/2.0\n"\
                        "WWW-Authenticate: NTLM TlRMTVNTUAACAAAADAAMADAAAAABAgEAf39/f39/f38AAAAAAAAAAAAAAAA8AAAARABPAE0AQQBJAE4A\n"\
                        "Date: Thu, 31 Mar 2011 17:41:03 GMT\n"\
                        "Content-Length: 341\n\n";
                    flip = 1;
                }
                else if(flip == 1) 
                {
                    send_data = "HTTP/1.1 200 OK\n"\
                        "Cache-Control: private, max-age=0\n"\
                        "Content-Type: text/xml; charset=utf-8\n"\
                        "Server: Microsoft-IIS/7.5\n"\
                        "X-AspNet-Version: 2.0.50727\n"\
                        "Persistent-Auth: true\n"\
                        "X-Powered-By: ASP.NET\n"\
                        "Date: Thu, 31 Mar 2011 17:41:04 GMT\n"\
                        "Content-Length: 2827\n\n";
                    flip = 0;
                }
                printf("sent:%s\n", send_data);
                fflush(stdout);
                send(connected, send_data,strlen(send_data), 0);  
            }
            else if(mark == 1)
            {
                send_data = "HTTP/1.1 401 Unauthorized\n"\
                    "Content-Type: text/html\n"\
                    "Server: Microsoft-IIS/7.5\n"\
                    "WWW-Authenticate: NTLM\n"\
                    "X-Powered-By: ASP.NET\n"\
                    "Date: Thu, 31 Mar 2011 17:41:03 GMT\n"\
                    "Content-Length: 1293\n\n";
                printf("sent:%s\n", send_data);
                fflush(stdout);
                send(connected, send_data,strlen(send_data), 0);  
            }
            else 
            {
                break;
            }
        }
    }       

      close(sock);
      return 0;
} 

