
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
    
#include "..\\axis2_isapi_plugin.h"
#include "..\\axis2_iis_constants.h"

static axis2_char_t redirect_word[INTERNET_MAX_URL_LENGTH] = "/axis2/mod_axis2_IIS.dll\?";
/*
 * Search a given uri to find weather it matches a uri for the axis2
 * The uri format for axis2 is of the form
 * scheme://server:port/axis2/other_parts
 * This function search  a give uri for the /axis2/. If a match
 * is found it will replace the /axis2 part of the url with /axis2/mod_iis.dll?
*/
axis2_status_t AXIS2_CALL 
get_extension_url(char url[], char ret_url[]);

/*
 * This function is called by the IIS server at the server 
 * initialization. So this is the ideal plcae for initializing 
 * axis2c.
*/ 
BOOL WINAPI GetFilterVersion(PHTTP_FILTER_VERSION pVer) 
{	ULONG http_filter_revision = HTTP_FILTER_REVISION;	pVer->dwFilterVersion = pVer->dwServerFilterVersion;	if (pVer->dwFilterVersion > http_filter_revision)
	{		pVer->dwFilterVersion = http_filter_revision;	}

	/*
	Receive notifictions when
	1. Server preprocessed the headers.
	2. Log 
	3. All the request coming in secure and none secure ports.
	*/ 
	pVer->dwFlags =
		(SF_NOTIFY_ORDER_HIGH | 		SF_NOTIFY_PREPROC_HEADERS |		SF_NOTIFY_SECURE_PORT | 		SF_NOTIFY_NONSECURE_PORT |		SF_NOTIFY_AUTH_COMPLETE 		);

	/* Give a short discription about the module.*/
	strcpy(pVer->lpszFilterDesc, "axis2c filter");
	/* initialize axis2*/
	return TRUE;}

/*
When a notification happens this function is called by the IIS.
*/ 
DWORD WINAPI HttpFilterProc(		PHTTP_FILTER_CONTEXT pfc,		DWORD notificationType,		LPVOID pvNotification) 
{	DWORD bufferLength = INTERNET_MAX_URL_LENGTH;	char url[INTERNET_MAX_URL_LENGTH];	char modified_url[INTERNET_MAX_URL_LENGTH];
	if (notificationType == SF_NOTIFY_PREPROC_HEADERS)
	{		pfc->GetServerVariable(pfc, "HTTP_URL", url, &bufferLength);		if (get_extension_url(url, modified_url))
		{			((PHTTP_FILTER_PREPROC_HEADERS) pvNotification)->SetHeader(pfc,
				"url",
				modified_url);			return SF_STATUS_REQ_HANDLED_NOTIFICATION;		}	}	return SF_STATUS_REQ_NEXT_NOTIFICATION;}

axis2_status_t AXIS2_CALL 
get_extension_url(char url[], char ret_url[]) 
{
    /* Should contain "/axis2/"*/
    ret_url[0] = '\0';

    /* The url should begin with "/axis2/"*/
    if (url[0] == '/' && (url[1] == 'a' || url[1] == 'A') 
        &&(url[2] == 'x' || url[2] == 'X') 
        &&(url[3] == 'i' || url[3] == 'I') 
        &&(url[4] == 's' || url[4] == 'S') 
        &&url[5] == '2' && url[6] == '/')
    {
        /* Append the string to redirect the request*/
        strcpy(ret_url, redirect_word);
        strcat(ret_url, &url[7]);
        return TRUE;
    }
    if (url[0] == '/' && (url[1] == 'a' || url[1] == 'A') 
        &&(url[2] == 'x' || url[2] == 'X') 
        &&(url[3] == 'i' || url[3] == 'I') 
        &&(url[4] == 's' || url[4] == 'S')         &&url[5] == '2' && url[6] == '\0')
    {
        strcpy(ret_url, redirect_word);
        return TRUE;
    }
    return FALSE;
}