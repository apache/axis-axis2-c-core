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

#include <stdio.h>
#include <axis2_defines.h>
#include <axis2_error.h>
#include <axis2_env.h>
#include <axis2_utils.h>
#include <rampart_callback.h>
#include <axis2_string.h>
#include <axis2_svc_skeleton.h>
#include <axis2_string.h>

#if 0
axis2_char_t* AXIS2_CALL
callback_get_sample_password_from_file(rampart_callback_t *rcb,
        const axis2_env_t *env,
        const axis2_char_t *username,
        void *param)
{
    axis2_char_t * password = NULL;
    FILE *file = NULL;
    axis2_char_t *filename = "samples/rampart/data/passwords.txt";

    file = fopen ( filename, "r" );
    if ( file != NULL )
    {
       axis2_char_t line [ 128 ];
       axis2_char_t ch = 0;
       axis2_char_t *res = NULL;
       axis2_char_t *un = NULL;
       axis2_char_t *pw = NULL;

       while ( fgets ( line, sizeof line, file ) != NULL )
       {
          res = axis2_strstr(line, ":");
          ch = res[0];
          res[0] = '\0';
          un = (axis2_char_t *) axis2_strdup(line, env);
          res[0] = ch;
          if(0 == axis2_strcmp(un, username)){
             pw = (axis2_char_t *) axis2_strdup(&(res[1]), env);
             password = axis2_strndup(pw, axis2_strlen(pw)-1, env); /*We need to remove the end of line character*/

             break;
          }
       }
       AXIS2_FREE(env->allocator, un);
       AXIS2_FREE(env->allocator, pw);
       fclose ( file );
    }else{
       AXIS2_LOG_INFO(env->log, "Cannot load the password file %s ", filename);
       perror ( filename );
    }
    return password;
}

#endif

axis2_char_t* AXIS2_CALL
get_sample_password(rampart_callback_t *rcb,
        const axis2_env_t *env, 
        const axis2_char_t *username,
        void *param)
{
    /*First set pf password are for sample usernames*/
    axis2_char_t * pw = NULL;
    if (0 == axis2_strcmp(username, "Raigama"))
    {
        pw = "RaigamaPW" ;
    }
    else if (0 == axis2_strcmp(username, "Gampola"))
    {
        pw = "GampolaPW";
    }
    else if (0 == axis2_strcmp(username, "alice"))
    {
        pw = "password";
    }
    else if (0 == axis2_strcmp(username, "bob"))
    {
        pw = "bobPW";
    }
    /*These are for sample keystores*/
    else if (0 == axis2_strcmp(username, "a"))
    {
        pw = "a12345";
    }
    else if (0 == axis2_strcmp(username, "b"))
    {
        pw = "b12345";
    }
    else if (0 == axis2_strcmp(username, "x"))
    {
        pw = "x12345";
    }
    else if (0 == axis2_strcmp(username, "y"))
    {
        pw = "y12345";
    }
    else
    {
        /*Append 12345 for any name not specified above*/
        /*sprintf(pw, "%s%s",  username, "12345");*/
    }
    return pw;
};

/**
 * Following block distinguish the exposed part of the dll.
 */
AXIS2_EXPORT int
axis2_get_instance(rampart_callback_t **inst,
        const axis2_env_t *env)
{
    rampart_callback_t* rcb = NULL;

    rcb = AXIS2_MALLOC(env->allocator,
            sizeof(rampart_callback_t));

    rcb->ops = AXIS2_MALLOC(
                env->allocator, sizeof(rampart_callback_ops_t));

    /*assign function pointers*/

    rcb->ops->callback_password = get_sample_password;

    *inst = rcb;

    if (!(*inst))
    {
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int
axis2_remove_instance(rampart_callback_t *inst,
        const axis2_env_t *env)
{
    axis2_status_t status = AXIS2_FAILURE;
    if (inst)
    {
        status = AXIS2_SVC_SKELETON_FREE(inst, env);
    }
    return status;
}

