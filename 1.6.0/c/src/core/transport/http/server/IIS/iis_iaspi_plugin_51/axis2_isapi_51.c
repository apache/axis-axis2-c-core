
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
    
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <httpfilt.h>
    
#include <axis2_const.h>
#include <axis2_defines.h>
#include <axutil_env.h>
#include <axutil_stream.h>

#include "..\\axis2_iis_constants.h"

#define REGISTRY_LOC			 "Software\\Apache Axis2c\\IIS ISAPI Redirector"
#define AXIS2_IIS_AXIS2_LOC		 "axis2_location" 
static char *axis2_loc = "/axis2";

static axis2_char_t redirect_word[INTERNET_MAX_URL_LENGTH] = "/axis2/mod_axis2_IIS.dll\?";
/*
 * Search a given uri to find weather it matches a uri for the axis2
 * The uri format for axis2 is of the form
 * scheme://server:port/axis2/other_parts
 * This function search  a give uri for the /axis2/. If a match
 * is found it will replace the /axis2 part of the url with /axis2/mod_iis.dll?
*/
axis2_bool_t AXIS2_CALL 
get_extension_url(char url[], char ret_url[]);

/*
 * This function is called by the IIS server at the server 
 * initialization. So this is the ideal plcae for initializing 
 * axis2c.
*/ 
BOOL WINAPI GetFilterVersion(PHTTP_FILTER_VERSION pVer) 
{
	DWORD type = 0, size = 0;
    LONG lrc = 0;
	char tmpbuf[INTERNET_MAX_URL_LENGTH];
	HKEY hkey;
	ULONG http_filter_revision = HTTP_FILTER_REVISION;
	pVer->dwFilterVersion = pVer->dwServerFilterVersion;
	if (pVer->dwFilterVersion > http_filter_revision)
	{
		pVer->dwFilterVersion = http_filter_revision;
	}

	/*
	Receive notifictions when
	1. Server preprocessed the headers.
	2. Log 
	3. All the request coming in secure and none secure ports.
	*/ 
	pVer->dwFlags =
		(SF_NOTIFY_ORDER_HIGH | 
		SF_NOTIFY_PREPROC_HEADERS |
		SF_NOTIFY_SECURE_PORT | 
		SF_NOTIFY_NONSECURE_PORT |
		SF_NOTIFY_AUTH_COMPLETE 
		);

	/* Give a short discription about the module.*/
	strcpy(pVer->lpszFilterDesc, "axis2c filter");
	/* Get the axis2 location from the registry configuration */
	lrc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REGISTRY_LOC, (DWORD) 0, KEY_READ, &hkey);
    if (ERROR_SUCCESS != lrc)
    {
        return FALSE;
    }
	size = INTERNET_MAX_URL_LENGTH;
    lrc = RegQueryValueEx(hkey, AXIS2_IIS_AXIS2_LOC, (LPDWORD) 0, &type, (LPBYTE) tmpbuf, &size);
    if ((ERROR_SUCCESS == lrc) && (type == REG_SZ))
    {
        tmpbuf[size] = '\0';
		axis2_loc = _strdup(tmpbuf);
    } 
	RegCloseKey(hkey);
	return TRUE;
}

/*
When a notification happens this function is called by the IIS.
*/ 
DWORD WINAPI HttpFilterProc(
		PHTTP_FILTER_CONTEXT pfc,
		DWORD notificationType,
		LPVOID pvNotification) 
{
	DWORD bufferLength = INTERNET_MAX_URL_LENGTH;
	char url[INTERNET_MAX_URL_LENGTH];
	char modified_url[INTERNET_MAX_URL_LENGTH];

	if (notificationType == SF_NOTIFY_PREPROC_HEADERS)
	{
		pfc->GetServerVariable(pfc, "HTTP_URL", url, &bufferLength);
		if (get_extension_url(url, modified_url))
		{
			((PHTTP_FILTER_PREPROC_HEADERS) pvNotification)->SetHeader(pfc,
				"url",
				modified_url);
			return SF_STATUS_REQ_HANDLED_NOTIFICATION;
		}
	}
	return SF_STATUS_REQ_NEXT_NOTIFICATION;
}

axis2_bool_t AXIS2_CALL 
get_extension_url(char url[], char ret_url[]) 
{
	axis2_bool_t is_for_us = AXIS2_FALSE;
	int i = 0;
    /* Should contain "/axis2/"*/
    ret_url[0] = '\0';	
	if (strlen(url) >= strlen(axis2_loc))
    {
		is_for_us = AXIS2_TRUE;	
		while (axis2_loc[i] != '\0')
		{
			if (axis2_loc[i] != (url)[i]) {
				is_for_us = AXIS2_FALSE;
				break;
			}
			i++;
		}
		if (url[i] != '/' && url[i] != '\0')
		{
			is_for_us = AXIS2_FALSE;
		}
	}
	if (is_for_us)
	{
		strcpy(ret_url, redirect_word);
		strcat(ret_url, &url[i]);
	}
    return is_for_us;
}