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
#include <platforms/axis2_platform_auto_sense.h>
#include <stdio.h>
#include <stdlib.h>
#include <axis2_error_default.h>
#include <axis2_log_default.h>
#include <axis2_thread_pool.h>
#include <signal.h>
#include <axis2_types.h>
#include <ctype.h>
#include <axis2_xml_reader.h>

axis2_env_t *system_env = NULL;
axis2_transport_receiver_t *server = NULL;
/***************************** Function headers *******************************/
axis2_env_t* init_syetem_env(axis2_allocator_t *allocator, 
    const axis2_char_t *log_file);
void system_exit(axis2_env_t *env, int status);
void usage(axis2_char_t* prog_name);
void sig_handler(int signal);
/***************************** End of function headers ************************/
axis2_env_t* init_syetem_env(axis2_allocator_t *allocator, 
    const axis2_char_t *log_file)
{
   axis2_error_t *error = axis2_error_create(allocator);
   axis2_log_t *log = axis2_log_create(allocator, NULL, log_file);
   axis2_thread_pool_t *thread_pool = axis2_thread_pool_init(allocator);
    /* We need to init the parser in main thread before spawning child 
     * threads
     */
    axis2_xml_reader_init(); 
   return axis2_env_create_with_error_log_thread_pool(allocator, error, log, 
                  thread_pool);
}

void system_exit(axis2_env_t *env, int status)
{
    axis2_allocator_t *allocator = NULL;
    if(NULL != server)
    {
       AXIS2_TRANSPORT_RECEIVER_FREE(server,  system_env);
    }
   if(NULL != env)
   {
        allocator = env->allocator;
      axis2_env_free(env);
   }
    axis2_allocator_free(allocator);
    axis2_xml_reader_cleanup();
   _exit(status);
}

int main(int argc, char *argv[])
{
   axis2_allocator_t *allocator = NULL;
   axis2_env_t *env = NULL;
   extern char *optarg;
   extern int optopt;
   int c;
   axis2_log_levels_t log_level = AXIS2_LOG_LEVEL_DEBUG;
   const axis2_char_t *log_file = "axis2.log";
   int port = 9090;
   const axis2_char_t *repo_path = "../";
   
   axis2_http_socket_read_timeout = AXIS2_HTTP_DEFAULT_SO_TIMEOUT;

    while ((c = AXIS2_GETOPT(argc, argv, ":p:r:ht:l:f:")) != -1)
    {
        
        switch(c)
        {
            case 'p':
                port = AXIS2_ATOI(optarg);
                break;
            case 'r':
                repo_path = optarg;
                break;
            case 't':
                axis2_http_socket_read_timeout = AXIS2_ATOI(optarg) * 1000;
                break;
            case 'l':
                log_level = AXIS2_ATOI(optarg);
                if (log_level < AXIS2_LOG_LEVEL_CRITICAL)
                    log_level = AXIS2_LOG_LEVEL_CRITICAL;
                if (log_level > AXIS2_LOG_LEVEL_TRACE)
                    log_level = AXIS2_LOG_LEVEL_TRACE;
                break;
            case 'f':
                log_file = optarg;
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
      system_exit(NULL, -1);
   }
   
    env = init_syetem_env(allocator, log_file);
    env->log->level = log_level;
   
    axis2_error_init();
   system_env = env;
   
#ifndef WIN32
    signal(SIGINT, sig_handler);
   signal(SIGPIPE, sig_handler); 
#endif   

   AXIS2_LOG_INFO(env->log, "Starting Axis2 HTTP server....");
   AXIS2_LOG_INFO(env->log, "Server port : %d", port);
   AXIS2_LOG_INFO(env->log, "Repo location : %s", repo_path);
   AXIS2_LOG_INFO(env->log, "Read Timeout : %d ms", axis2_http_socket_read_timeout);
   
   server = axis2_http_server_create(env, repo_path, port);
   if(NULL == server)
   {
       AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Server creation failed: Error code:"
                  " %d :: %s", env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
      system_exit(env, -1);
      
   }
   printf("Started Simple Axis2 HTTP Server ...\n");
   if(AXIS2_TRANSPORT_RECEIVER_START(server, env) == AXIS2_FAILURE)
   {
      AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Server start failed: Error code:"
                  " %d :: %s", env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
      system_exit(env, -1);
   }
   return 0;
}

void usage(axis2_char_t* prog_name)
{
    fprintf(stdout, "\n Usage : %s", prog_name );
    fprintf(stdout, " [-p PORT]");
    fprintf(stdout, " [-t TIMEOUT]");
    fprintf(stdout, " [-r REPO_PATH]");
    fprintf(stdout, " [-l LOG_LEVEL]");
    fprintf(stdout, " [-f LOG_FILE]\n");
    fprintf(stdout, " Options :\n");
    fprintf(stdout, "\t-p PORT \t use the port number PORT. The default port is"
                  " 9090\n");
    fprintf(stdout, "\t-r REPO_PATH \t use the repository path REPO_PATH. The"
                  " default repository path is ../\n");
    fprintf(stdout, "\t-t SOCKET_READ_TIMEOUT\t set socket read timeout to "
                  "SOCKET_READ_TIMEOUT. Default timeout is 30 seconds\n");
    fprintf(stdout, "\t-l LOG_LEVEL\t set log level to LOG_LEVEL. Available "
                  "log levels range from 0(critical only) to 4(debug)."
                  "\n\t\t\t Default log level is 4(debug).\n");
    fprintf(stdout, "\t-f LOG_FILE\t set log file to LOG_FILE. Default is /dev/stderr\n");
    fprintf(stdout, " Help :\n\t-h \t display this help screen.\n\n");
}

/**
 * Signal handler
 */
#ifndef WIN32
 
void sig_handler(int signal)
{
   switch(signal)
   {
      case SIGINT : 
      {
         AXIS2_LOG_INFO(system_env->log, "Received signal SIGINT. Server "
                  "shutting down");
         axis2_http_server_stop(server, system_env);
         AXIS2_LOG_INFO(system_env->log, "Shutdown complete ...");
         system_exit(system_env, 0);         
      }
      case SIGPIPE :
      {
         AXIS2_LOG_INFO(system_env->log, "Received signal SIGPIPE.  Client "
                  "request serve aborted");
         return;
      }
      case SIGSEGV :
      {
         fprintf(stderr, "Received deadly signal SIGSEGV. Terminating\n");
         _exit(-1);
      }
   }
}
#endif
