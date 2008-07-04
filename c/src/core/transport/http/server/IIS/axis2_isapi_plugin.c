
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
#include <httpext.h>

#include "axis2_iis_constants.h"
#include "axis2_iis_worker.h"

/* Axis headers */ 
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axutil_thread_pool.h>
#include <axiom_xml_reader.h>
#include <axutil_log.h>

#ifndef _WIN32_WINNT 
#define _WIN32_WINNT 0x0500 
#endif 

#define	AXIS2_IIS_LOG_FILE_TAG		 "log_file"
#define AXIS2_IIS_LOG_LEVEL_TAG		 "log_level" 
#define AXIS2_IIS_REPO_PATH_TAG		 "axis2c_home"
#define AXIS2_IIS_EXTENSION_URI_TAG	 "extension_uri"
#define AXIS2_IIS_REDIRECT_WORD_TAG	 "redirect_uri"
#define AXIS2_IIS_AXIS2_LOCATION	 "axis2_location"
#define AXIS2_IIS_SERVICE_URL_PREFIX "services_url_prefix"

#define AXIS2_IIS_LOG_TRACE_VERB	 "trace"
#define AXIS2_IIS_LOG_ERROR_VERB	 "error"
#define AXIS2_IIS_LOG_INFO_VERB		 "info"
#define AXIS2_IIS_LOG_USER_VERB      "user"
#define AXIS2_IIS_LOG_CRITICAL_VERB	 "critical"
#define AXIS2_IIS_LOG_WARN_VERB		 "warning"
#define AXIS2_IIS_LOG_DEBUG_VERB	 "debug"

#define MAX_FILE_PATH				 256
#define REGISTRY_LOCATION			 "Software\\Apache Axis2c\\IIS ISAPI Redirector"

static int is_inited = FALSE;
static axis2_iis_worker_t *axis2_worker		 = NULL;
static const axutil_env_t *axutil_env		 = NULL;

/* Configuration parameters */
axis2_char_t *axis2_location			 = "/axis2";
static axis2_char_t *axis2_service_url_prefix= "/services";
static axis2_char_t repo_path[MAX_FILE_PATH];
static axis2_char_t log_file[MAX_FILE_PATH];
static axutil_log_levels_t log_level		 = AXIS2_LOG_LEVEL_CRITICAL;

/* Path variables */ 
static char szOriginalPath[_MAX_PATH + 1];
static char szPath[_MAX_PATH + 1];

axis2_char_t    general_error[] = "<html>\r\n" 
                        "<head><title> An IIS server error occurred. </title></head>\r\n" 
                        "<h1> An IIS server error occurred </h1>\r\n" 
                        "<hr>\r\n" 
                        "An error occurred in IIS while processing this request.</hr></html>"; 

axis2_char_t	initializing_error[] = "<html>\r\n" 
                        "<head><title> An IIS server error occurred. </title></head>\r\n" 
                        "<h1> An IIS server error occurred </h1>\r\n" 
                        "<hr>\r\n" 
                        "An error occurred while initilizing Axis2/C.</hr></html>"; 


/*
 * This is a utility functipn for reading configuration data from the registery.
 */ 
static axis2_status_t AXIS2_CALL 
read_registery_init_data();

/*
 * Utility function for reading 
 */ 
static axis2_status_t AXIS2_CALL get_registry_config_parameter(
    HKEY hkey,
    const char *tag,
    char *b,
    DWORD sz);

/*
 * Parse the given string and return the corresponding log_level
 */ 
axutil_log_levels_t AXIS2_CALL axis2_iis_parse_log_level(char level[]);

/*
 * Initialize axis. This function is called in the begining of the module loading.
 * It initializes the axis by reading values from the configuration and creating the
 * required structures for the axis2c
*/
axis2_status_t AXIS2_CALL init_axis2();
    
/* 
 * This is the function to be called after the processing 
 * is over for non Axis2 requets
 */
VOID 
WINAPI 
ExecUrlCompletion ( 
    EXTENSION_CONTROL_BLOCK *   pecb, 
    PVOID                       pContext, 
    DWORD                       cbIO, 
    DWORD                       dwError 
    ); 

/* 
 * If somethign went wrong in the IIS server when 
 * we are proccessing we send this 
 */
BOOL 
send_error(		
	EXTENSION_CONTROL_BLOCK * pecb, 
    CHAR error[]); 

axis2_status_t AXIS2_CALL init_axis2();

BOOL 
WINAPI 
GetExtensionVersion(HSE_VERSION_INFO * pVer)  
{     
    pVer->dwExtensionVersion = MAKELONG( HSE_VERSION_MINOR, 
                                         HSE_VERSION_MAJOR); 
    strncpy( pVer->lpszExtensionDesc, 
             "WildCardMap Sample ISAPI Extension", HSE_MAX_EXT_DLL_NAME_LEN ); 

    pVer->lpszExtensionDesc[HSE_MAX_EXT_DLL_NAME_LEN-1] = '\0'; 
	server_version = 5;
	return TRUE; 
} 

DWORD 
WINAPI 
HttpExtensionProc(EXTENSION_CONTROL_BLOCK * pecb) 

{ 
    HSE_EXEC_URL_INFO   ExecUrlInfo; 
    DWORD               cbData = INTERNET_MAX_URL_LENGTH; 
    char				url[INTERNET_MAX_URL_LENGTH]; 
	axis2_bool_t		is_for_us = AXIS2_TRUE;

    /* Get the URL */      
    if ( pecb->GetServerVariable( pecb->ConnID, 
                                  "URL", 
                                  url, 
                                  &cbData ) == FALSE ) 
    { 
        return HSE_STATUS_ERROR;  
    } 

	if (!is_inited) 
	{			
		DWORD dwBufferSize = 0;
		axis2_char_t server_software[256];
		axis2_char_t *version = NULL;

		ZeroMemory(szOriginalPath, sizeof szOriginalPath);
		dwBufferSize = sizeof szOriginalPath;

#if _WIN32_WINNT >= 0x0502
		GetDllDirectory( dwBufferSize, szOriginalPath );
#else
		GetCurrentDirectory( dwBufferSize, szOriginalPath );
#endif
		ZeroMemory(szPath, sizeof szPath);
		dwBufferSize = sizeof szPath;
		/* Get the current physical paht */
		if (pecb->GetServerVariable(pecb->ConnID, "APPL_PHYSICAL_PATH", szPath, &dwBufferSize) == FALSE)
		{
			send_error(pecb, initializing_error);
			return HSE_STATUS_ERROR;				
		}
		/* Retrieve the server version */
		dwBufferSize = 32;
		if (pecb->GetServerVariable(pecb->ConnID, "SERVER_SOFTWARE", server_software, &dwBufferSize) == FALSE)
		{
			send_error(pecb, initializing_error);
			return HSE_STATUS_ERROR;				
		}
		version = axutil_strchr(server_software, '/');	
		if (version)
		{
			server_version = atoi(version + 1);
		}
#if _WIN32_WINNT >= 0x0502
		SetDllDirectory( szPath );
#else
		SetCurrentDirectory( szPath );
#endif
		/* If we haven't initialized axis2/c before initialization failed */
		if (AXIS2_FAILURE == init_axis2())
		{
			send_error(pecb, initializing_error);
			return HSE_STATUS_ERROR;
		}
#if _WIN32_WINNT >= 0x0502
		SetDllDirectory( szOriginalPath );
#else
		SetCurrentDirectory( szOriginalPath );
#endif
	}

	/* Check weather we have a request for Axis2/C */
	if (server_version >= 6 && strlen(url) >= strlen(axis2_location))
    {
		int i = 0;
		is_for_us = AXIS2_TRUE;	
		while (axis2_location[i] != '\0')
		{
			if (axis2_location[i] != (url)[i]) {
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
		/* Windows cannot find the correct dlls unless the path is set*/ 
#if _WIN32_WINNT >= 0x0502
		SetDllDirectory( szPath );
#else
		SetCurrentDirectory( szPath );
#endif
		pecb->dwHttpStatusCode = HTTP_INTERNAL_SERVER_ERROR;
		/* We are sure that worker is not NULL since it is NULL init_axis2 would have failed */
		axis2_iis_worker_process_request(axis2_worker, axutil_env, pecb);

	    /* Windows cannot find the correct dlls unless the dir is set
		 but we want to reset to previous dir after the load */
#if _WIN32_WINNT >= 0x0502
		SetDllDirectory( szOriginalPath );
#else
		SetCurrentDirectory( szOriginalPath );
#endif
		return HSE_STATUS_SUCCESS;
	}
	else if (server_version >= 6)
	{
		/* For IIS 5.1 or earlier this code is never executed. Since the URL is 
		redirected to Axis2/C by the Filter */

		/* If not for Axis2/C let the request go to who ever wants it */
		ExecUrlInfo.pszUrl = NULL;          /* Use original request URL */
		ExecUrlInfo.pszMethod = NULL;       /* Use original request method */
		ExecUrlInfo.pszChildHeaders = NULL; /* Use original request headers */
		ExecUrlInfo.pUserInfo = NULL;       /* Use original request user info */
		ExecUrlInfo.pEntity = NULL;         /* Use original request entity */
		
		/* Provent recursion */
		ExecUrlInfo.dwExecUrlFlags = HSE_EXEC_URL_IGNORE_CURRENT_INTERCEPTOR; 
	     
		/* Associate the completion routine and the current URL with this request. */ 
		if ( pecb->ServerSupportFunction( pecb->ConnID, 
										  HSE_REQ_IO_COMPLETION, 
										  ExecUrlCompletion, 
										  NULL,  
										  NULL) == FALSE ) 
		{        
			return HSE_STATUS_ERROR; 
		} 
	    		
		/* Ok, now that everything is set up, let's call the child request */     
		if ( pecb->ServerSupportFunction( pecb->ConnID, 
										  HSE_REQ_EXEC_URL, 
										  &ExecUrlInfo, 
										  NULL, 
										  NULL ) == FALSE ) 
		{         
			return HSE_STATUS_ERROR; 
		} 
		/* Return pending and let the completion clean up */ 
		return HSE_STATUS_PENDING; 
	} 
	return HSE_STATUS_ERROR;
} 

VOID 
WINAPI 
ExecUrlCompletion ( 
    EXTENSION_CONTROL_BLOCK *   pecb, 
    PVOID                       pContext, 
    DWORD                       cbIO, 
    DWORD                       dwError 
    ) 
{ 
	/* We are done so notify */
    pecb->ServerSupportFunction( 
        pecb->ConnID, 
        HSE_REQ_DONE_WITH_SESSION, 
        NULL, 
        NULL, 
        NULL); 
} 


BOOL 
send_error(		
	EXTENSION_CONTROL_BLOCK * pecb, 
    axis2_char_t error[]) 
{ 
    DWORD   cbData;             
    pecb->dwHttpStatusCode = 500;     
    /* Send headers and response */    
    pecb->ServerSupportFunction( pecb->ConnID, 
                                 HSE_REQ_SEND_RESPONSE_HEADER, 
                                 "500 Server Error", 
                                 NULL, 
                                 (LPDWORD)"Content-Type: text/html\r\n\r\n" ); 

    cbData = axutil_strlen( error ); 
    return pecb->WriteClient( pecb->ConnID, 
                              error, 
                              &cbData, 
                              HSE_IO_SYNC ); 
} 

axis2_status_t AXIS2_CALL read_registery_init_data() 
{
    long rc = 0;
    axis2_status_t ok = TRUE;
    char tmpbuf[INTERNET_MAX_URL_LENGTH];
    HKEY hkey;
	AXIS2_IMPORT extern axis2_char_t *axis2_request_url_prefix;

    rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REGISTRY_LOCATION, (DWORD) 0, KEY_READ, &hkey);
    if (ERROR_SUCCESS != rc)
    {
        return AXIS2_FAILURE;
    }
    if (get_registry_config_parameter(hkey, AXIS2_IIS_REPO_PATH_TAG, tmpbuf, sizeof(repo_path)))
    {
        strcpy(repo_path, tmpbuf);
    }
    else
    {
		return AXIS2_FAILURE;
    }
    if (get_registry_config_parameter(hkey, AXIS2_IIS_LOG_FILE_TAG, tmpbuf, sizeof(log_file)))
    {
        strcpy(log_file, tmpbuf);
    }
    else
    {
		return AXIS2_FAILURE;
    }
    if (get_registry_config_parameter(hkey, AXIS2_IIS_LOG_LEVEL_TAG, tmpbuf, sizeof(tmpbuf)))
    {
        log_level = axis2_iis_parse_log_level(tmpbuf);
    }
	else 
	{
		return AXIS2_FAILURE;
	}
	if (get_registry_config_parameter(hkey, AXIS2_IIS_SERVICE_URL_PREFIX, tmpbuf, sizeof(tmpbuf)))
	{
		axis2_request_url_prefix = _strdup(tmpbuf); 
	}
	if (get_registry_config_parameter(hkey, AXIS2_IIS_AXIS2_LOCATION, tmpbuf, sizeof(tmpbuf)))
	{
		axis2_location = _strdup(tmpbuf); 
	}
    RegCloseKey(hkey);
    return ok;
}

axutil_log_levels_t AXIS2_CALL 
axis2_iis_parse_log_level(char level[]) 
{
    if (0 == AXIS2_STRICMP(level, AXIS2_IIS_LOG_TRACE_VERB))
    {
        return AXIS2_LOG_LEVEL_TRACE;
    }
    if (0 == AXIS2_STRICMP(level, AXIS2_IIS_LOG_DEBUG_VERB))
    {
        return AXIS2_LOG_LEVEL_DEBUG;
    }
    if (0 == AXIS2_STRICMP(level, AXIS2_IIS_LOG_INFO_VERB))
    {
        return AXIS2_LOG_LEVEL_INFO;
    }
    if (0 == AXIS2_STRICMP(level, AXIS2_IIS_LOG_USER_VERB))
    {
        return AXIS2_LOG_LEVEL_USER;
    }
    if (0 == AXIS2_STRICMP(level, AXIS2_IIS_LOG_WARN_VERB))
    {
        return AXIS2_LOG_LEVEL_WARNING;
    }
    if (0 == AXIS2_STRICMP(level, AXIS2_IIS_LOG_ERROR_VERB))
    {
        return AXIS2_LOG_LEVEL_ERROR;
    }
    if (0 == AXIS2_STRICMP(level, AXIS2_IIS_LOG_CRITICAL_VERB))
    {
        return AXIS2_LOG_LEVEL_CRITICAL;
    }
    return AXIS2_LOG_LEVEL_DEBUG;
}

axis2_status_t AXIS2_CALL 
get_registry_config_parameter(HKEY hkey, const char *tag, char *b, DWORD sz) 
{
    DWORD type = 0;
    LONG lrc;

    lrc = RegQueryValueEx(hkey, tag, (LPDWORD) 0, &type, (LPBYTE) b, &sz);
    if ((ERROR_SUCCESS != lrc) || (type != REG_SZ))
    {
        return FALSE;
    }
    b[sz] = '\0';
    return TRUE;
}

/**
 * This method initializes the axis2 engine. All the required variables are set to 
 * their initial values in this method.
*/ 
axis2_status_t AXIS2_CALL init_axis2() 
{
    /*
     * These are the varibles required to initialize axis.
     */ 
    axis2_status_t status = FALSE;
    /* We need to init xml readers before we go into threaded env */     
    if (!is_inited)
    {
        axiom_xml_reader_init();
        status = read_registery_init_data();
		if (status == AXIS2_FAILURE)
		{
			return AXIS2_FAILURE;
		}
        axutil_error_init();
		/* Initialize the environement */
        axutil_env = axutil_env_create_all(log_file, log_level);
        if (!axutil_env)
        {
            return AXIS2_FAILURE;
        }
        axis2_worker = axis2_iis_worker_create(axutil_env, repo_path);
        if (!axis2_worker)
        {
            return AXIS2_FAILURE;
        }
        is_inited = AXIS2_TRUE;
		return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}
