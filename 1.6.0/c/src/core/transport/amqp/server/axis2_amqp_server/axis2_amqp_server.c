/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      tcp://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axutil_thread_pool.h>
#include <signal.h>
#include <axutil_types.h>
#include <ctype.h>
#include <axiom_xml_reader.h>
#include <axis2_amqp_server.h>
#include <axis2_amqp_receiver.h>
#include <axis2_qpid_receiver_interface.h>

axis2_transport_receiver_t* receiver   = NULL;
axutil_env_t* server_env = NULL;

axutil_env_t*
init_server_env(axutil_allocator_t* allocator, 
		const axis2_char_t* log_file_name)
{
	axutil_error_t* error = axutil_error_create(allocator);
	axutil_log_t* log = axutil_log_create(allocator, NULL, log_file_name);
	axutil_thread_pool_t* thread_pool = axutil_thread_pool_init(allocator);
    axutil_env_t* env = axutil_env_create_with_error_log_thread_pool(allocator,
			error, log, thread_pool);
	
	axiom_xml_reader_init();

	return env;
}


void
server_exit(int status)
{
	if (receiver)
	{
		axis2_transport_receiver_free(receiver, server_env);
	}

	if (server_env)
	{
		axutil_env_free(server_env);
	}

	axiom_xml_reader_cleanup();

	exit(status);
}


void
show_usage(axis2_char_t* prog_name)
{
	fprintf(stdout, "\n Usage : %s", prog_name);
    fprintf(stdout, " [-i QPID_BROKER_IP]");
    fprintf(stdout, " [-p QPID_BROKER_PORT]");
    fprintf(stdout, " [-r REPO_PATH]");
    fprintf(stdout, " [-l LOG_LEVEL]");
    fprintf(stdout, " [-f LOG_FILE]\n");
    fprintf(stdout, " [-s LOG_FILE_SIZE]\n");
    fprintf(stdout, " Options :\n");
    fprintf(stdout, "\t-i QPID_BROKER_IP \t Qpid broker IP, default is 127.0.0.1\n");
    fprintf(stdout, "\t-p QPID_BROKER_PORT \t the port on which the Qpid broker listens, default is 5672\n");
    fprintf(stdout, "\t-r REPO_PATH \t\t repository path, default is ../\n");
    fprintf(stdout,
            "\t-l LOG_LEVEL\t\t log level, available log levels:"
            "\n\t\t\t\t\t 0 - critical    1 - errors 2 - warnings"
            "\n\t\t\t\t\t 3 - information 4 - debug  5- user 6 - trace"
            "\n\t\t\t\t\t Default log level is 4(debug).\n");
#ifndef WIN32
    fprintf(stdout,
            "\t-f LOG_FILE\t\t log file, default is $AXIS2C_HOME/logs/axis2.log"
            "\n\t\t\t\t or axis2.log in current folder if AXIS2C_HOME not set\n");
#else
    fprintf(stdout,
            "\t-f LOG_FILE\t\t log file, default is %%AXIS2C_HOME%%\\logs\\axis2.log"
            "\n\t\t\t\t or axis2.log in current folder if AXIS2C_HOME not set\n");
#endif
    fprintf(stdout,
            "\t-s LOG_FILE_SIZE\t Maximum log file size in mega bytes, default maximum size is 1MB.\n");
    fprintf(stdout, " Help :\n\t-h \t\t\t display this help screen.\n\n");
}

#ifndef WIN32

void
sig_handler(int signal)
{
	switch (signal)
	{
		case SIGINT:
			AXIS2_LOG_INFO(server_env->log, "Received signal SIGINT.Server shutting down");
           	axis2_amqp_receiver_stop(receiver, server_env);
            AXIS2_LOG_INFO(server_env->log, "Shutdown complete ...");
            	
			server_exit(0);

		case SIGPIPE:
			AXIS2_LOG_INFO(server_env->log, "Received signal SIGPIPE.Client request serve aborted");
			return;

		case SIGSEGV:
			fprintf(stderr, "Received deadly signal SIGSEGV. Terminating ...\n");
			_exit(-1);
	}
}

#endif

int 
main(int argc, char* argv[])
{
	axutil_allocator_t* allocator = NULL;
	extern char* optarg;
	extern int optopt;
	int c;
	const axis2_char_t* qpid_broker_ip = NULL;
	int qpid_broker_port = AXIS2_QPID_NULL_CONF_INT;
	const axis2_char_t* repo_path = AXIS2_AMQP_SERVER_REPO_PATH;
	axutil_log_levels_t log_level = AXIS2_LOG_LEVEL_DEBUG;
	const axis2_char_t* log_file_name = AXIS2_AMQP_SERVER_LOG_FILE_NAME;
	int log_file_size = AXUTIL_LOG_FILE_SIZE;

	while ((c = AXIS2_GETOPT(argc, argv, "i:p:r:l:f:s:h")) != -1)
	{
		switch (c)
		{
			case 'i':
				qpid_broker_ip = optarg;
				break;

			case 'p':
				qpid_broker_port = AXIS2_ATOI(optarg);
				break;

			case 'r':
				repo_path = optarg;
				break;

			case 'l':
				log_level = AXIS2_ATOI(optarg);
				if (log_level < AXIS2_LOG_LEVEL_CRITICAL)
					log_level = AXIS2_LOG_LEVEL_CRITICAL;
				if (log_level > AXIS2_LOG_LEVEL_TRACE)
					log_level = AXIS2_LOG_LEVEL_TRACE;
				break;

			case 'f':
				log_file_name = optarg;
				break;

			case 's':
				log_file_size = 1024 * 1024 * AXIS2_ATOI(optarg);
				break;

			case 'h':
				show_usage(argv[0]);
				return 0;

			case ':':
				fprintf(stderr, "\nOption -%c requires an operand\n", optopt);
				show_usage(argv[0]);
				return -1;

			case '?':
				if (isprint(optopt))
					fprintf(stderr, "\nUnknown option `-%c'.\n", optopt);
				 show_usage(argv[0]);
				 return -1;
		}
	}

	allocator = axutil_allocator_init(NULL);
	if (!allocator)
	{
		server_exit(-1);
	}

	server_env = init_server_env(allocator, log_file_name);
	server_env->log->level = log_level;
	server_env->log->size = log_file_size;

	axutil_error_init();

#ifndef WIN32
	signal(SIGINT, sig_handler);
	signal(SIGPIPE, sig_handler);
#endif

	AXIS2_LOG_INFO(server_env->log, "Starting Axis2 AMQP Server ...");
	AXIS2_LOG_INFO(server_env->log, "Repo Location : %s", repo_path);

	receiver = axis2_amqp_receiver_create(server_env, repo_path, 
			qpid_broker_ip, qpid_broker_port);
	if (!receiver)
	{
		AXIS2_LOG_ERROR(server_env->log, AXIS2_LOG_SI,
                        "Server creation failed: Error code:" " %d :: %s",
                        server_env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(server_env->error));
        server_exit(-1);
	}

	if (axis2_amqp_receiver_start(receiver, server_env) == AXIS2_FAILURE)
	{
		AXIS2_LOG_ERROR(server_env->log, AXIS2_LOG_SI,
                        "Server start failed: Error code:" " %d :: %s",
                        server_env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(server_env->error));
        server_exit(-1);
	}

	return 0;
}
