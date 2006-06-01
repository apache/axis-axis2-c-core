/*
 *   Copyright 2004-2004 The Apache Software Foundation.
// (c) Copyright IBM Corp. 2004, 2005 All Rights Reserved
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

#include <platforms/windows/axis2_uuid_gen_windows.h>
#include <rpc.h>
#include <stdio.h>
#include <string.h>
#include <axis2_utils_defines.h>


AXIS2_EXTERN axis2_char_t *)  axis2_platform_uuid_gen(char *s AXIS2_CALL
{
	RPC_STATUS			retval;   
    UUID				uuid;
	unsigned char		*str;
	axis2_char_t		*retstr;

    if(NULL == s)
    {
        return NULL;
    }	
    retstr = s;
	retval = UuidCreate(&uuid);
	if (retval == RPC_S_UUID_LOCAL_ONLY)
		printf("warning - unique within computer \n");
	else if (retval == RPC_S_UUID_NO_ADDRESS)
	{
		printf("error\n");
		return NULL;
	}

	retval = UuidToStringA(&uuid,&str);
	if (retval == RPC_S_OK)
	{
		strcpy(retstr, str);
		RpcStringFree(&str);
	}
	else if (retval == RPC_S_OUT_OF_MEMORY)
	{
		printf("system is out of memory \n");
		return NULL;
	}
    return retstr;
}
