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


#ifndef AXIS2_WINDOWS_H
#define AXIS2_WINDOWS_H

/**
  * @file axis2_unix.h
  * @brief axis2 unix platform specific interface
  */

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_unix Platform Specific 
 * @ingroup axis2_platforms_unix
 * @{
 */

#define _WINSOCKAPI_ /* Prevent inclusion of winsock.h in windows.h */
#include <windows.h>

/***************************************************************
 * Default paths to shared library/DLLs and files
 ***************************************************************
 */
 
#define AXIS2_PLATFORM_DEFAULT_DEPLOY_PATH ""

#define AXIS2_PLATFORM_XMLPARSER_PATH      "axis2_parser.dll"
#define AXIS2_PLATFORM_TRANSPORTHTTP_PATH  "http_transport.dll"
#define AXIS2_PLATFORM_CHANNEL_PATH        "http_channel.dll"
#define AXIS2_PLATFORM_SSLCHANNEL_PATH     "Unknown"

#define AXIS2_PLATFORM_LOG_PATH            ""
#define AXIS2_PLATFORM_CLIENTLOG_PATH      ""
#define AXIS2_PLATFORM_CONFIG_PATH         ""
#define AXIS2_PLATFORM_SECUREINFO			 ""


// =============================================================
// Library loading and procedure resolution
// =============================================================
#define DLHandler HINSTANCE
#define RTLD_LAZY 0    // not sure this is needed?

#define AXIS2_PLATFORM_LOADLIBINIT()
#define AXIS2_PLATFORM_LOADLIB(_lib)     callLoadLib(_lib)
#define AXIS2_PLATFORM_UNLOADLIB         FreeLibrary
#define AXIS2_PLATFORM_GETPROCADDR       GetProcAddress
#define AXIS2_PLATFORM_LOADLIBEXIT()
#define AXIS2_PLATFORM_LOADLIB_ERROR     ""

// =============================================================
// National Language Support
// =============================================================

// STRTOASC is to translate single byte 'native' character representation to ASCII
// ASCTOSTR is to translate single byte ascii representation to 'native' character
// CANNOT be used with constants
#define AXIS2_PLATFORM_STRTOASC( x ) ( x )
#define AXIS2_PLATFORM_ASCTOSTR( x ) ( x )

// =============================================================
// Miscellaneous
// =============================================================
#define AXIS2_PLATFORM_SLEEP(x) Sleep(0);

/**
 * Get the last error code from the system.
 * Please ensure that this is a thread safe implementation
 * and that it returns a long
 * @return long the lsat error message for this thread
 */
#define AXIS2_GETLASTERROR GetLastError();

/**
 * From the last error number get a sensible std::string representing it
 * @param errorNumber the error Number you are trying to get a message for
 * @return the error message. NOTE: The caller is responsible for deleting the returned string
 */
#define AXIS2_PLATFORM_GET_ERROR_MESSAGE(errorNumber) getPlatformErrorMessage(errorNumber);

/**
 * type to be used for 64bit integers
 */
#define AXIS2_LONGLONG __int64

/**
 * Format string to be used in printf for 64bit integers
 */
#define AXIS2_PRINTF_LONGLONG_FORMAT_SPECIFIER "%I64d"
#define AXIS2_PRINTF_LONGLONG_FORMAT_SPECIFIER_CHARS "I64d"

HMODULE callLoadLib(LPCTSTR lib);

/**
 * Platform specific path separator char
 */
#define AXIS2_PATH_SEP_CHAR "/"

/**
 * Platform specific time
 */
#define AXIS2_TIME_T time_t
/**
 * Platform specific method to obtain current time in milli seconds
 */
#define AXIS2_PLATFORM_GET_TIME_IN_MILLIS _ftime
#define AXIS2_PLATFORM_TIMEB timeb

/**
 * Platform specific file handling
 */
#define AXIS2_R_OK R_OK /* test for read permission */
#define AXIS2_W_OK W_OK /* test for write permission */
#define AXIS2_X_OK X_OK /* test for execute or search permission */
#define AXIS2_F_OK F_OK /* test whether the directories leading to the file can be 
                      searched and the file exists */

/** @} */
#ifdef __cplusplus
}
#endif
#endif /*  AXIS2_WINDOWS_H */


