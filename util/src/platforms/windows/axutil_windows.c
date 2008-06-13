
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

 

#include <windows/axutil_windows.h>
#include <stdio.h>
    

/*

std::string* getPlatformErrorMessage(long errorNumber)
{
   std::string* returningString = new std::string();
    LPVOID lpMsgBuf;

   FormatMessage(
   FORMAT_MESSAGE_ALLOCATE_BUFFER |
   FORMAT_MESSAGE_FROM_SYSTEM,
   NULL,
   errorNumber,
   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
   (LPTSTR) &lpMsgBuf,
   0, NULL );

   returningString->append((LPTSTR)lpMsgBuf);
    LocalFree(lpMsgBuf);

    return returningString;
}
*/ 
AXIS2_EXTERN HMODULE AXIS2_CALL 
callLoadLib(char *lib) 
{
	/* Disable display of the critical-error-handler message box */
    SetErrorMode(SEM_FAILCRITICALERRORS); 
    return LoadLibraryEx(lib, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
}


AXIS2_EXTERN struct tm *AXIS2_CALL 
axis2_win_gmtime(
    const time_t * timep,
    struct tm *result) 
{
    return gmtime(timep);
}

AXIS2_EXTERN void AXIS2_CALL 
axutil_win32_get_last_error(axis2_char_t *buf, 
							unsigned int buf_size)
{
	LPVOID lpMsgBuf;	
	int rc = GetLastError();
	sprintf( buf, "DLL Load Error %d: ", rc );
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				  NULL,
	              rc,
	              0, 
				  (LPTSTR) &lpMsgBuf,
				  0,
				  NULL
				);
	if (lpMsgBuf)
	{
		strncat( buf, (char*)lpMsgBuf, buf_size - strlen( buf ) - 1 );
	}
	LocalFree( lpMsgBuf );
}

AXIS2_EXTERN void AXIS2_CALL
axutil_win32_get_last_wsa_error(axis2_char_t *buf, 
								unsigned int buf_size)
{
	LPVOID lpMsgBuf;	
	int rc = WSAGetLastError();
	sprintf( buf, "Winsock error %d: ", rc );
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				  NULL,
	              rc,
	              0, 
				  (LPTSTR) &lpMsgBuf,
				  0,
				  NULL
				);
	if (lpMsgBuf)
	{
		strncat( buf, (char*)lpMsgBuf, buf_size - strlen( buf ) - 1 );
	}
	LocalFree( lpMsgBuf );
}
