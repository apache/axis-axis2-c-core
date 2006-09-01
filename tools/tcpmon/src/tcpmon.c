#include <axis2_utils.h>
#include <axis2_error_default.h>
#include <axis2_log_default.h>
#include <axis2_thread_pool.h>
#include <tcpmon_session.h>
#include <tcpmon_entry.h>
#include <tcpmon_util.h>
#include <platforms/axis2_platform_auto_sense.h>
#include <axis2_string.h>

#define SIZE 1024

int on_new_entry( const axis2_env_t *env,
                   tcpmon_entry_t* entry,
                   int status);
int on_error_func ( const axis2_env_t *env,
                    char* error_message );



int main(int argc, char** argv )
{
    axis2_env_t* env = NULL;
    axis2_allocator_t* allocator = NULL;
    axis2_error_t *error = NULL;
    axis2_log_t *log = NULL;
    axis2_thread_pool_t *thread_pool = NULL;
    tcpmon_session_t* session = NULL;
    int c;
    int listen_port,target_port;
    char *target_host;

    allocator = axis2_allocator_init(NULL);

    error = axis2_error_create(allocator);
    log = axis2_log_create(allocator, NULL, "axis2_tcpmon.log");
    thread_pool = axis2_thread_pool_init(allocator);    

    env = axis2_env_create_with_error_log_thread_pool(allocator, error, log,
                  thread_pool);

    if ( (argc == 2 && AXIS2_STRCMP(argv[1], "-h") ) ||
            !(argc == 3 || argc == 4) )
    {
        printf("Usage : %s LISTEN_PORT TARGET_PORT [TARGET_HOST]\n", argv[0]);
        printf("use -h for help\n");
        return 0;
    }
    if ( argc > 1 )
    {
        listen_port = atoi(argv[1]);
        if ( listen_port == 0 )
        {
            printf( "INVALID value for listen port\n");
            printf("use -h for help\n");
            return 0;
        }
    }
    if ( argc > 2 )
    {
        target_port = atoi(argv[2]);
        if ( target_port == 0 )
        {
            printf( "INVALID value for target port\n");
            printf("use -h for help\n");
            return 0;
        }
    }
    /* if the target_host is not given fill with the default */
    if ( argc > 3)
    {
        target_host = (char*)AXIS2_STRDUP( argv[3], env);
    }
    else
    {
        target_host = (char*)AXIS2_STRDUP( "localhost", env);
    }
    session = tcpmon_session_create (env );

    TCPMON_SESSION_SET_LISTEN_PORT( session, env, listen_port);
    TCPMON_SESSION_SET_TARGET_PORT ( session, env, target_port);
    TCPMON_SESSION_SET_TARGET_HOST ( session, env, target_host);
    TCPMON_SESSION_ON_TRANS_FAULT ( session, env, on_error_func);
    TCPMON_SESSION_ON_NEW_ENTRY ( session, env, on_new_entry);

    TCPMON_SESSION_START( session, env );
    
    do
    {
        c = getchar();
    }while ( c != 'q' );
    
    TCPMON_SESSION_STOP( session, env );
    TCPMON_SESSION_FREE ( session, env );
    AXIS2_FREE( env-> allocator, target_host);
    axis2_allocator_free(allocator);
    axis2_env_free ( env );
    return 0;
}

int on_new_entry( const axis2_env_t *env,
                   tcpmon_entry_t* entry,
                   int status)
{
    char* plain_buffer = NULL;
    char* formated_buffer = NULL;
    if ( status == 0 )
    {
        plain_buffer = TCPMON_ENTRY_SENT_DATA (entry, env );
        if (plain_buffer) /* this can be possible as no xml present */
        {
            formated_buffer = tcpmon_util_format_as_xml 
                                 ( env, plain_buffer);
        }
        else
        {
            formated_buffer = "";
        }
        printf ("%s\n", "SENDING DATA..");
        printf ("/* sending time = %s*/\n", TCPMON_ENTRY_SENT_TIME ( entry, env ) );
        printf ("---------------------\n");
 
        printf ("%s\n\n%s\n\n", TCPMON_ENTRY_SENT_HEADERS ( entry, env ),
                            formated_buffer); 
    }
    if ( status == 1 )
    {
        plain_buffer = TCPMON_ENTRY_ARRIVED_DATA (entry, env );
        if (plain_buffer) /* this can be possible as no xml present */
        {
            formated_buffer = tcpmon_util_format_as_xml 
                                 ( env, plain_buffer);
        }
        else
        {
            formated_buffer = "";
        }
        printf ("%s\n", "RETRIEVING DATA..");
        printf ("/* retrieving time = %s*/\n", TCPMON_ENTRY_ARRIVED_TIME ( entry, env ) );
        printf ("/* time throughput = %s*/\n", TCPMON_ENTRY_TIME_DIFF( entry, env ) );
        printf ("---------------------\n");
 
        printf ("%s\n\n%s\n\n", TCPMON_ENTRY_ARRIVED_HEADERS ( entry, env ),
                            formated_buffer);
    }
    return 0;
}

int on_error_func ( const axis2_env_t *env,
                    char* error_message )
{
    fprintf (stderr, "ERROR: %s\n", error_message );
    return 0;
}
