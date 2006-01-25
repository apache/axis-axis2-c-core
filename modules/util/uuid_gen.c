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

#include <axis2_uuid_gen.h>

/* We need these static variables to track throughout the program execution */
static axis2_bool_t axis2_uuid_gen_is_first = AXIS2_TRUE;
static struct axis2_uuid_st axis2_uuid_static;


axis2_uuid_t* AXIS2_CALL
axis2_uuid_gen_v1(axis2_env_t **env)
{
    struct timeval time_now;
    struct timeval tv;
    AXIS2_LONGLONG time_val;
	short int clck = 0;
	axis2_uuid_t *ret_uuid = NULL;

	AXIS2_ENV_CHECK(env, NULL);	
	
    if(AXIS2_TRUE == axis2_uuid_gen_is_first)
    {
        char *mac_addr = axis2_network_handler_get_mac_addr(env);
        memcpy(axis2_uuid_static.mac, mac_addr, 6);
        axis2_uuid_static.time_seq = 0;
        axis2_uuid_static.clock = 0;
		AXIS2_FREE((*env)->allocator, mac_addr);
    }
    /*
     *  GENERATE TIME
     */

    /* determine current system time and sequence counter */
     if(gettimeofday(&time_now, NULL) == -1)
        return NULL;

    /* check whether system time changed since last retrieve */
    if(!(time_now.tv_sec  == axis2_uuid_static.time_last.tv_sec
                        && time_now.tv_usec == 
						axis2_uuid_static.time_last.tv_usec)) {
        /* reset time sequence counter and continue */
        axis2_uuid_static.time_seq = 0;
     }

    /* until we are out of UUIDs per tick, increment
    the time/tick sequence counter and continue */
	while(axis2_uuid_static.time_seq < UUIDS_PER_TICK)
	{	
     	axis2_uuid_static.time_seq++;
    }
    /* sleep for 1000ns (1us) */
    tv.tv_sec  = 0;
    tv.tv_usec = 1;
    select(0, NULL, NULL, NULL, &tv);

    time_val = time_now.tv_sec * 1000000;
    time_val += time_now.tv_usec * 10;

    time_val += UUID_TIMEOFFSET;

    /* compensate for low resolution system clock by adding
       the time/tick sequence counter */
    if (axis2_uuid_static.time_seq > 0)
        time_val += (int)axis2_uuid_static.time_seq;

    /* store the 60 LSB of the time in the UUID and make version 1*/
	time_val &= AXIS2_UNSIGNED_LONGLONGVALUE(0xFFFFFFFFFFFFFFF0);
	time_val |= AXIS2_UNSIGNED_LONGLONGVALUE(0x0000000000000001);
    /*
     *  GENERATE CLOCK
     */

    /* retrieve current clock sequence */
    clck = axis2_uuid_static.clock;

    /* generate new random clock sequence (initially or if the
       time has stepped backwards) or else just increase it */
    if (clck == 0 || (time_now.tv_sec < axis2_uuid_static.time_last.tv_sec ||
						(time_now.tv_sec == axis2_uuid_static.time_last.tv_sec
                		&& time_now.tv_usec < 
						axis2_uuid_static.time_last.tv_usec)))
	{
		srand(time_now.tv_usec);
		clck = rand();
	}
    else
	{
        clck++;
	}
    clck %= (2<<14);

    /* store back new clock sequence */
    axis2_uuid_static.clock = clck;
	
    clck &= 0x1FFF;
	clck |= 0x2000;
    
    /*
     *  FINISH
     */
    /* remember current system time for next iteration */
    axis2_uuid_static.time_last.tv_sec  = time_now.tv_sec;
    axis2_uuid_static.time_last.tv_usec = time_now.tv_usec;
	
    ret_uuid = AXIS2_MALLOC((*env)->allocator, sizeof(axis2_uuid_t));
	if(NULL == ret_uuid)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
		return NULL;
	}
	ret_uuid->time_stamp_ver = time_val;
	ret_uuid->clock_variant = clck;
	memcpy(ret_uuid->mac_addr, axis2_uuid_static.mac, 6);
	return ret_uuid;
}


axis2_char_t* AXIS2_CALL
axis2_uuid_gen(axis2_env_t **env)
{
	axis2_uuid_t *uuid_struct = NULL;
	axis2_char_t *uuid_str = NULL;
	unsigned char mac[7];
	axis2_char_t llvalue[30];
	char mac_hex[13];
	AXIS2_LONGLONG llno = 0;
	long lno = 0;

	AXIS2_ENV_CHECK(env, NULL);	
	
	uuid_struct = axis2_uuid_gen_v1(env);
	if(NULL == uuid_struct)
	{
		return NULL;
	}
	uuid_str = AXIS2_MALLOC((*env)->allocator, sizeof(axis2_char_t)*35);
	if(NULL == uuid_str)	
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
		AXIS2_FREE((*env)->allocator, uuid_struct);
		return NULL;
	}
	memcpy(mac, uuid_struct->mac_addr, 6);
	sprintf(mac_hex, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3]
						, mac[4], mac[5]);
	sprintf(llvalue, "%llx", uuid_struct->time_stamp_ver);
	
	sprintf(uuid_str, "%s%04x%s", llvalue, uuid_struct->clock_variant, mac_hex);
	return uuid_str;	
}
