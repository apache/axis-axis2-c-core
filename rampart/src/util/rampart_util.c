/*
 *   Copyright 2003-2004 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

/* Rampart_crypto_util will contain crypto functionalities of the rampart model
 * 
 */
#include <stdio.h>
#include <rampart/rampart_util.h>
#include <axis2_util.h>
#include <axis2_base64.h>
#include <time.h>

/*Calculate the hash of concatenated string of 
 * nonce, created and the password.
 * 
 */
#define SIZE 256
#define SIZE_HASH 32
#define SIZE_NONCE 24

AXIS2_EXTERN axis2_char_t* AXIS2_CALL rampart_generate_nonce(const axis2_env_t *env)
{
	int num;
	char* rand_str = NULL;
	axis2_char_t* encoded_str=NULL;
	num=rand();
	rand_str= AXIS2_MALLOC(env->allocator,sizeof(char)*16);
	sprintf(rand_str, "%16d", num);


    encoded_str = AXIS2_MALLOC(env->allocator, sizeof(char)*SIZE_NONCE);
    axis2_base64_encode(encoded_str,rand_str,AXIS2_STRLEN(rand_str));
    AXIS2_FREE(env->allocator, rand_str);
	return encoded_str;
}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL rampart_generate_time(const axis2_env_t *env, int ttl)
{
	char buffer[SIZE];
	time_t curtime;
	struct tm *loctime;
	axis2_char_t* created_str=NULL;
 
	curtime = time (NULL) + ttl;
	loctime = localtime (&curtime);
  	strftime (buffer, SIZE, "%Y-%m-%dT%H:%M:%SZ\n", loctime);
    created_str = AXIS2_STRDUP(buffer, env);
    
	return created_str;
}

AXIS2_EXTERN int AXIS2_CALL 
rampart_format_date_zulu_to_mili(const axis2_env_t *env, axis2_char_t *str)
{
    /*TODO Get the time in mili sec from the string*/

   
 
    return 1000000;/*TODO*/
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
rampart_print_info(const axis2_env_t *env, axis2_char_t* info)
{
    /*printf("[rampart]: %s\n", info);*/
    return AXIS2_SUCCESS;
}
