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
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <sys/time.h>
#include <axis2_uuid_gen_unix.h>

	
/* We need these static variables to track throughout the program execution */
static axis2_bool_t axis2_uuid_gen_is_first = AXIS2_TRUE;
static struct axis2_uuid_st axis2_uuid_static;


axis2_uuid_t* AXIS2_CALL
axis2_uuid_gen_v1()
{
    struct timeval time_now;
    struct timeval tv;
    unsigned long long time_val;
	unsigned long long time_val2;
	unsigned short int clck = 0;
	axis2_uuid_t *ret_uuid = NULL;
	unsigned short int time_high_version = 0;
		
    if(AXIS2_TRUE == axis2_uuid_gen_is_first)
    {
        char *mac_addr = axis2_uuid_get_mac_addr();
        memcpy(axis2_uuid_static.mac, mac_addr, 6);
        axis2_uuid_static.time_seq = 0;
        axis2_uuid_static.clock = 0;
		free(mac_addr);
		axis2_uuid_gen_is_first = AXIS2_FALSE;
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

    time_val = (unsigned long long)time_now.tv_sec * 10000000ull;
    time_val += (unsigned long long)time_now.tv_usec * 10ull;
	
	ret_uuid = malloc(sizeof(axis2_uuid_t));
	
    time_val += UUID_TIMEOFFSET;
    /* compensate for low resolution system clock by adding
       the time/tick sequence counter */
    if (axis2_uuid_static.time_seq > 0)
        time_val += (unsigned long long)axis2_uuid_static.time_seq;
	
	time_val2 = time_val;
	ret_uuid->time_low = (unsigned long)time_val2;
	time_val2 >>= 32;
	ret_uuid->time_mid = (unsigned short int)time_val2;
	time_val2 >>= 16;
	time_high_version = (unsigned short int)time_val2;
	
	/* store the 60 LSB of the time in the UUID and make version 1*/
	time_high_version <<= 4;
	time_high_version &= 0xFFF0;
	time_high_version |= 0x0001;
	ret_uuid->time_high_version = time_high_version;
	
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
	    
	if(NULL == ret_uuid)
	{
		return NULL;
	}
	ret_uuid->clock_variant = clck;
	memcpy(ret_uuid->mac_addr, axis2_uuid_static.mac, 6);
	return ret_uuid;
}


axis2_char_t* AXIS2_CALL
axis2_platform_uuid_gen()
{
	axis2_uuid_t *uuid_struct = NULL;
	axis2_char_t *uuid_str = NULL;
	unsigned char mac[7];
	char mac_hex[13];
	
	uuid_struct = axis2_uuid_gen_v1();
	if(NULL == uuid_struct)
	{
		return NULL;
	}
	uuid_str = malloc(sizeof(axis2_char_t)*40);
	if(NULL == uuid_str)	
	{
		return NULL;
	}
	memcpy(mac, uuid_struct->mac_addr, 6);
	sprintf(mac_hex, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3]
						, mac[4], mac[5]);
	sprintf(uuid_str, "%08x-%04x-%04x-%04x-%s", uuid_struct->time_low, 
						uuid_struct->time_mid, uuid_struct->time_high_version, 
						uuid_struct->clock_variant, mac_hex);
	return uuid_str;	
}

char * AXIS2_CALL
axis2_uuid_get_mac_addr()
{
	struct ifreq ifr;
	struct sockaddr *sa;
	int s = 0;
	int i = 0;
	char *buffer = NULL;
	
		
	if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
		return NULL;
	sprintf(ifr.ifr_name, "eth0");
	if (ioctl(s, SIOCGIFHWADDR, &ifr) < 0) 
	{
		close(s);
		return NULL;
	}
	buffer = (char*)malloc(6*sizeof(char));
	
	sa = (struct sockaddr *)&ifr.ifr_addr;
	for (i = 0; i < 6; i++)
		buffer[i] = (unsigned char)(sa->sa_data[i] & 0xff);
	close(s);
	return buffer;	
}
