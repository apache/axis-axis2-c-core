/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <axis2_http_server.h>
#include <axis2_http_transport.h>
#include <stdio.h>
#include <stdlib.h>
#include <axis2_error_default.h>
#include <axis2_log_default.h>


/***************************** Function headers *******************************/
axis2_env_t* init_syetem_env(axis2_allocator_t *allocator);
void system_exit(axis2_allocator_t *allocator, axis2_env_t *env, int status);
void usage(axis2_char_t* prog_name);
/***************************** End of function headers ************************/
axis2_env_t* init_syetem_env(axis2_allocator_t *allocator)
{
	axis2_error_t *error = axis2_error_create(allocator);
	axis2_log_t *log = axis2_log_create(allocator, NULL);
	return axis2_env_create_with_error_log(allocator, error, log);
}

void system_exit(axis2_allocator_t *allocator, axis2_env_t *env, int status)
{
	if(NULL != env)
	{
		axis2_env_free(env);
	}
	if(NULL != allocator)
	{
		free(allocator);
	}
	_exit(status);
}

int main(int argc, char *argv[])
{
	axis2_allocator_t *allocator = NULL;
	axis2_env_t *env = NULL;
	axis2_transport_receiver_t *server = NULL;
    extern char *optarg;
    extern int optopt;
	char tmp_str[512];
	int port = 9090;
    axis2_char_t *repo_path = "../";
	axis2_http_socket_read_timeout = AXIS2_HTTP_DEFAULT_SO_TIMEOUT;
    int c;

    while ((c = getopt(argc, argv, ":p:r:ht:")) != -1)
    {
        switch(c)
        {
            case 'p':
                port = atoi(optarg);
                break;
            case 'r':
                repo_path = optarg;
                break;
            case 't':
                axis2_http_socket_read_timeout = atoi(optarg) * 1000;
                break;
            case 'h':
                usage(argv[0]);
                return 0;
            case ':':
                fprintf(stderr, "\nOption -%c requires an operand\n", optopt);
                usage(argv[0]);
	            return -1;
	        case '?':
	            if (isprint (optopt))
	            fprintf (stderr, "\nUnknown option `-%c'.\n", optopt);
	            usage(argv[0]);
	            return -1;
	    }
	}

	allocator = axis2_allocator_init(NULL);
    
	if(NULL == allocator)
	{
		printf("[Axis2]Startup FAILED due to memory allocation failure\n");
		system_exit(NULL, NULL, -1);
	}
	
    env = init_syetem_env(allocator);
    env->log->level = AXIS2_LOG_LEVEL_DEBUG;
	
    axis2_error_init();
    
	printf("[Axis2]Starting Axis2 HTTP server....\n");
	AXIS2_LOG_WRITE(env->log, "[Axis2]Starting Axis2 HTTP server....\n", 
						AXIS2_LOG_LEVEL_INFO);
	sprintf(tmp_str, "[Axis2]Server port : %d", port);
	printf("%s\n",tmp_str);
	AXIS2_LOG_WRITE(env->log, tmp_str, AXIS2_LOG_LEVEL_INFO);
	sprintf(tmp_str, "[Axis2]Repo location : %s", repo_path);
	printf("%s\n",tmp_str);
	AXIS2_LOG_WRITE(env->log, tmp_str, AXIS2_LOG_LEVEL_INFO);
	sprintf(tmp_str, "[Axis2]Read Timeout : %d ms", 
						axis2_http_socket_read_timeout);
	printf("%s\n",tmp_str);
	AXIS2_LOG_WRITE(env->log, tmp_str, AXIS2_LOG_LEVEL_INFO);
	
	server = axis2_http_server_create(&env, repo_path, port);
	if(NULL == server)
	{
		sprintf(tmp_str, "[Axis2]Server creation failed: Error code: %d", 
						env->error->error_number);
		printf("%s\n",tmp_str);
        printf("%s \n", AXIS2_ERROR_GET_MESSAGE(env->error));
		AXIS2_LOG_WRITE(env->log, tmp_str, AXIS2_LOG_LEVEL_INFO);
		system_exit(allocator, env, -1);
		
	}
	AXIS2_TRANSPORT_RECEIVER_START(server, &env);
	return 0;
}

void usage(axis2_char_t* prog_name)
{
    fprintf(stdout, "\n Usage : [-p PORT] axis2_http_server" );
    fprintf(stdout, " [-t TIMEOUT]");
    fprintf(stdout, " [-r REPO_PATH]\n");
    fprintf(stdout, " Options :\n");
    fprintf(stdout, "\t-p PORT \t use port number PORT. The default port is 9090\n");
    fprintf(stdout, "\t-r REPO_PATH \t use the repository path REPO_PATH. The default repository path is ../\n");
    fprintf(stdout, "\t-t SOCKET_READ_TIMEOUT\t set timeout to SOCKET_READ_TIMEOUT. Default timeout is 30 seconds\n");
    fprintf(stdout, " Help :\n\t-h \t display this help screen.\n\n");
}

