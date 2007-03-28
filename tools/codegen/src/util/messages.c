/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <w2c_messages.h>
#include <axis2_property.h>
#include <axis2_hash.h>
#include <axis2_string.h>
#include <platforms/axis2_platform_auto_sense.h>

#define W2C_MESSAGES_FILENAME "resource.properties"

AXIS2_EXTERN axis2_properties_t* AXIS2_CALL
w2c_messages_get_message_properties(
    const axis2_env_t *env )
{
    axis2_properties_t *props = NULL;
    axis2_char_t *axis2c_home = NULL;
    axis2_char_t *filename = NULL;
    axis2_char_t *filepath= NULL;
    axis2_char_t *inter_path= NULL;
    int path_len = 0;
    /*
    FILE *f = NULL;
   */
    axis2c_home = getenv( "AXIS2C_HOME");
    if ( axis2c_home == NULL )
    {
        fprintf(stderr,"failed to pick AXIS2C_HOME\n" );
        return NULL;
    }
    inter_path = W2C_MESSAGES_INTER_PATH;
    filename = W2C_MESSAGES_FILENAME;
    
    path_len = axis2_strlen ( filename ) + axis2_strlen (axis2c_home )
               + axis2_strlen ( inter_path ) 
               + 2; /* for seperator and null char */
    filepath = AXIS2_MALLOC ( env-> allocator, path_len* sizeof(axis2_char_t) );
    sprintf ( filepath, "%s%s%s", axis2c_home, inter_path, filename );
  
    props = axis2_properties_create(env);
	axis2_properties_load( props, env, filepath); 

    return props;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
w2c_messages_get_message( const axis2_env_t *env, axis2_char_t *key )
{
     axis2_properties_t *props = NULL;
     axis2_char_t *message = NULL;
     
     props = w2c_messages_get_message_properties( env);
     message = axis2_properties_get_property( props, env, key);
     if ( message )
     {
         message = axis2_strdup (env, message);
     }
     axis2_properties_free( props, env);

     return  message;
}

AXIS2_EXTERN void AXIS2_CALL
w2c_messages_print_n_log_error( const axis2_env_t *env, axis2_char_t *key )
{
     axis2_char_t *msg = NULL;
     msg = w2c_messages_get_message( env, key );
     if ( msg )
     {
         fprintf ( stderr, "%s\n", msg );
         AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                         msg);
         AXIS2_FREE( env->allocator, msg);
     }
}

AXIS2_EXTERN void AXIS2_CALL
w2c_messages_print_n_log_error_from_properties( const axis2_env_t *env, 
                                          axis2_properties_t *props,
                                          axis2_char_t *key )
{
     axis2_char_t *msg = NULL;
     msg = axis2_properties_get_property( props, env, key);
     if ( msg )
     {
         fprintf ( stderr, "%s\n", msg );
         AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                         msg);
     }
}

