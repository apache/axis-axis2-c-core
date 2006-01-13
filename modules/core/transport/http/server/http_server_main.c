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

/***************************** Function servers *******************************/
axis2_env_t* init_syetem_env(axis2_allocator_t *allocator);
void system_exit(axis2_allocator_t *allocator, axis2_env_t *env, int status);
/***************************** End of function servers ************************/
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
	axis2_http_server_t *server = NULL;
	char tmp_str[512];
	int port = -1;
	
	if(argc != 3)
	{
		printf("%s <port> <repo location>\n", argv[0]);
		exit(0);
	}
	port = atoi(argv[1]);
	allocator = axis2_allocator_init(NULL);
	if(NULL == allocator)
	{
		printf("[Axis2]Startup FAILED due to memory allocation failure\n");
		system_exit(NULL, NULL, -1);
	}
	env = init_syetem_env(allocator);
	
	printf("[Axis2]Starting Axis2 HTTP server....\n");
	AXIS2_LOG_WRITE(env->log, "[Axis2]Starting Axis2 HTTP server....\n", 
						AXIS2_LOG_INFO);
	sprintf(tmp_str, "[Axis2]Server port : %d\n", port);
	printf(tmp_str);
	AXIS2_LOG_WRITE(env->log, tmp_str, AXIS2_LOG_INFO);
	sprintf(tmp_str, "[Axis2]Repo location : %s\n", argv[2]);
	printf(tmp_str);
	AXIS2_LOG_WRITE(env->log, tmp_str, AXIS2_LOG_INFO);
	
	server = axis2_http_server_create(&env, argv[2], port);
	if(NULL == server)
	{
		sprintf(tmp_str, "[Axis2]Server creation failed: Error code: %d\n", 
						env->error->error_number);
		printf(tmp_str);
		AXIS2_LOG_WRITE(env->log, tmp_str, AXIS2_LOG_INFO);
		system_exit(allocator, env, -1);
		
	}
	AXIS2_HTTP_SERVER_START(server, &env);
	return 0;
}
