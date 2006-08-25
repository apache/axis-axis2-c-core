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
#include <rampart_util.h>
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

#define PRINTINFO 1 
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
    /* AXIS2_FREE(env->allocator, rand_str); */
   return encoded_str;
}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL rampart_generate_time(const axis2_env_t *env, int ttl)
{
   char buffer[SIZE];
   time_t curtime;
   struct tm *loctime;
   axis2_char_t *created_str = NULL;
 
   curtime = time (NULL) + ttl;
   loctime = localtime (&curtime);
     strftime (buffer, SIZE, "%Y-%m-%dT%H:%M:%SZ\n", loctime);
    created_str = AXIS2_STRDUP(buffer, env);
    
   return created_str;
}

/**
    We expect dt1_str < dt2_str/ Otherwise FAILURE
*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_compare_date_time(const axis2_env_t *env, axis2_char_t *dt1_str, axis2_char_t *dt2_str)
{
    axis2_status_t status = AXIS2_FAILURE;
    axis2_date_time_t *dt1 = NULL;
    axis2_date_time_t *dt2 = NULL;
    int yyyy1, mm1, dd1, hh1, mi1, ss1;
    int yyyy2, mm2, dd2, hh2, mi2, ss2;

    
    dt1 = axis2_date_time_create(env);
    dt2 = axis2_date_time_create(env);
    
    status = AXIS2_DATE_TIME_DESERIALIZE_DATE_TIME(dt1, env, dt1_str);
    if(status == AXIS2_FAILURE){
        return AXIS2_FAILURE;
    }

    /*printf("\n DT1 = %s",AXIS2_DATE_TIME_SERIALIZE_DATE_TIME(dt1, env)); */
    yyyy1=AXIS2_DATE_TIME_GET_YEAR( dt1, env);
    mm1 = AXIS2_DATE_TIME_GET_MONTH( dt1, env);
    dd1 = AXIS2_DATE_TIME_GET_DATE( dt1, env);
    hh1 = AXIS2_DATE_TIME_GET_HOUR( dt1, env);
    mi1 = AXIS2_DATE_TIME_GET_MINUTE( dt1, env);
    ss1 = AXIS2_DATE_TIME_GET_SECOND( dt1, env);

    status = AXIS2_DATE_TIME_DESERIALIZE_DATE_TIME(dt2, env, dt2_str);
    if(status == AXIS2_FAILURE){
        return AXIS2_FAILURE;
    }
  
    /*printf("\n DT2 = %s",AXIS2_DATE_TIME_SERIALIZE_DATE_TIME(dt2, env)); */
    
    yyyy2=AXIS2_DATE_TIME_GET_YEAR( dt2, env);
    mm2 = AXIS2_DATE_TIME_GET_MONTH( dt2, env);
    dd2 = AXIS2_DATE_TIME_GET_DATE( dt2, env);
    hh2 = AXIS2_DATE_TIME_GET_HOUR( dt2, env);
    mi2 = AXIS2_DATE_TIME_GET_MINUTE( dt2, env);
    ss2 = AXIS2_DATE_TIME_GET_SECOND( dt2, env);
    
    if(yyyy1 > yyyy2){
        return AXIS2_FAILURE;
    }
    if(mm1 > mm2){
        return AXIS2_FAILURE;
    }
    if(dd1 > dd2){
        return AXIS2_FAILURE;
    }
    if(hh1 > hh2){
        return AXIS2_FAILURE;
    }
    if(mi1 > mi2){
        return AXIS2_FAILURE;
    }
    if(ss1 > ss2){
        return AXIS2_FAILURE;
    }
   /* 
    if(AXIS2_DATE_TIME_GET_YEAR( dt1, env) > AXIS2_DATE_TIME_GET_YEAR( dt2, env)){
        return AXIS2_FAILURE;
    }
    if(AXIS2_DATE_TIME_GET_MONTH( dt1, env) > AXIS2_DATE_TIME_GET_MONTH( dt2, env)){
        return AXIS2_FAILURE;
    }
    if(AXIS2_DATE_TIME_GET_DATE( dt1, env) > AXIS2_DATE_TIME_GET_DATE( dt2, env)){
        return AXIS2_FAILURE;
    }
    if(AXIS2_DATE_TIME_GET_HOUR( dt1, env) > AXIS2_DATE_TIME_GET_HOUR( dt2, env)){
        return AXIS2_FAILURE;
    }
    if(AXIS2_DATE_TIME_GET_MINUTE( dt1, env) > AXIS2_DATE_TIME_GET_MINUTE( dt2, env)){
        return AXIS2_FAILURE;
    }
    if(AXIS2_DATE_TIME_GET_SECOND( dt1, env) > AXIS2_DATE_TIME_GET_SECOND( dt2, env)){
        return AXIS2_FAILURE;
    }

    */

    /*AXIS2_DATE_TIME_FREE(dt1, env);
    AXIS2_DATE_TIME_FREE(dt2, env);*/

    return AXIS2_SUCCESS; 
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
rampart_print_info(const axis2_env_t *env, axis2_char_t* info)
{
    #ifdef PRINTINFO
    printf("[rampart]: %s\n", info);
    #endif
    return AXIS2_SUCCESS;
}
