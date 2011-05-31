/* tcpclient.c */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <axutil_base64.h>
#include <axutil_string.h>
#include <axis2_util.h>
#include <axis2_ntlm.h>


/*axis2_char_t *hostname = "172.16.176.132";*/
axis2_char_t *hostname = "localhost";
int port = 8080;
static char *
create_type1_header(const axutil_env_t *env, axis2_char_t *encoded)
{
    char *type1_header = NULL;
    char *header1 = NULL;

    type1_header = axutil_strcat(env, "Authorization: NTLM " ,encoded, NULL);

    header1 = "HEAD /myservice/Service1.asmx HTTP/1.1\n"\
            "Host: 172.16.176.132:8080\n"\
            "User-Agent: Axis2C/1.7.0\n";
    header1 = axutil_strcat(env, header1, type1_header, "\n\n", NULL);
    AXIS2_FREE(env->allocator, type1_header);

    return header1;
}

static axis2_char_t *
create_type3_header(const axutil_env_t *env, 
        char *encoded)
{
    char *header3 = NULL;

    char *temp_header = strdup("GET /myservice/Service1.asmx?WSDL HTTP/1.1\n"\
            "Host: 172.16.176.132:8080\n"\
            "User-Agent: Axis2C/1.7.0\n"\
            "Authorization: NTLM ");

    int len1 = axutil_strlen(encoded);
    int len2 = axutil_strlen(temp_header);
    header3 = AXIS2_MALLOC(env->allocator, (len1 + len2 + 5));
    memset(header3, 0, sizeof(header3));
    strcat(header3, temp_header);
    strcat(header3, encoded);
    strcat(header3, "\n\n");

    return header3;

}

int main()
{
    int sock, bytes_recieved;  
    char recv_data[10024];
    struct hostent *host;
    struct sockaddr_in server_addr;
    const axis2_char_t *header = NULL;
    axis2_char_t *header_value = NULL;
    char *type3_header = NULL;
    char *type1_header = NULL;
    const char *user = "nandika",
	            *domain = "mydomain",
	            *password = "nandika",
	            *workstation = "workstation";
    axis2_status_t status = AXIS2_FAILURE;
    const axutil_env_t *env = NULL;
    env = axutil_env_create_all("mock_client.log", AXIS2_LOG_LEVEL_TRACE);

    host = gethostbyname(hostname);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;     
    server_addr.sin_port = htons(port);   
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(server_addr.sin_zero),8); 
  
    /* Create a normal message */
    char *init_send = axutil_strdup(env, "HEAD /myservice/Service1.asmx HTTP/1.1\n"\
                        "Host: 172.16.176.132:8080\n"\
                        "User-Agent: Axis2C/1.7.0\n\n");

    if (connect(sock, (struct sockaddr *)&server_addr,
                sizeof(struct sockaddr)) == -1) 
    {
        perror("Connect");
        exit(1);
    }
    /* Send a normal message */
    send(sock, init_send, strlen(init_send), 0); 

    while(1)
    {
        bytes_recieved=recv(sock,recv_data,10024,0);
        recv_data[bytes_recieved] = '\0';
        header = axutil_strstr(recv_data, "WWW-Authenticate: NTLM ");
        
        /* Process the challange */
        if(header)
        {
            int i = 22;
            while(header[i] && isspace((unsigned char) header[i]))
            {
                i++;
            }
            if (header[i] != '\0') 
            {
                int len = axutil_strlen(&header[i]);
                if (len == 0)
                {
                    printf("invalid Negotiate token\n");
                }
                else
                {
                    header_value = axutil_strdup(env, &header[i]);
                }
            }
            if(header_value)
            {
                axis2_char_t *temp = strstr(header_value, "==");
                temp = temp + 3;
                *temp = '\0';
                header_value = axutil_strdup(env, header_value);
            }
        }
        if(!header_value) /* printf("unauth_header:\n%s\n", unauth_header); */
        {
            axis2_char_t *encoded = NULL;
            int elen = 0;
            int flags = 0;
            axis2_ntlm_t *ntlm = NULL;
            ntlm = axis2_ntlm_create(env);
            status = axis2_ntlm_auth_create_type1_message(ntlm, env, &encoded, &elen, user, 
                    password, flags, domain);
            if(AXIS2_SUCCESS != status)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "axis2_ntlm_auth_create_type1_message call failed");
                printf("axis2_ntlm_auth_create_type1_message call failed\n");
                return 1;
            }

            /* Ceate type 1(negotiation) header  message from the recieved header */
            type1_header = create_type1_header(env, encoded);
            /* Send netotiation message */
            send(sock, type1_header,strlen(type1_header), 0);
            printf("sent:\n%s\n", type1_header);
            free(type1_header);
            continue;
        }
        if(header_value)
        {
            /*printf("header_value:\n***%s***\n", header_value);*/
            int elen = 0;
            axis2_char_t *encoded = NULL;
            axis2_char_t *header_value = NULL;
            axis2_ntlm_t *ntlm = NULL;
            ntlm = axis2_ntlm_create(env);
            status = axis2_ntlm_auth_create_type3_message(ntlm, env, header_value, &encoded, 
                    &elen, user, password, domain, workstation);
            if(AXIS2_SUCCESS != status)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "axis2_ntlm_auth_create_type3_message call failed");
                printf("axis2_ntlm_auth_create_type3_message call failed\n");
                return 1;
            }

            /* Create Type3 (authentication) header */
            type3_header = create_type3_header(env, encoded);
            break;
        }
    }
    /* Send Type3(authentication) message */
    send(sock, type3_header, strlen(type3_header), 0); 
    bytes_recieved=recv(sock,recv_data,10024,0);
    recv_data[bytes_recieved] = '\0';
    printf("\nRecieved data:\n%s \n" , recv_data);
    close(sock);
    
    return 0;
}

