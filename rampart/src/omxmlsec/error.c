/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License")" "
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

#include <oxs_error.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

/*Table to map error codes with the error message*/
#define OXS_ERRORS_MAX_NUMBER 64
static oxs_error_description oxs_errors_table[OXS_ERRORS_MAX_NUMBER] = {
    {OXS_ERROR_DEFAULT, "oxs defualt error"},
    {OXS_ERROR_ENCRYPT_FAILED ,"encryption failed" },
    {OXS_ERROR_DECRYPT_FAILED , "decryption failed"},
    {OXS_ERROR_INVALID_DATA , "invalid data"},
    {OXS_ERROR_INVALID_SIZE , "invalid size"},
    {OXS_ERROR_INVALID_FORMAT , "invalid format"},
};

AXIS2_EXTERN const char* AXIS2_CALL
oxs_errors_get_msg_by_code(int code)
{
    const char* error_msg = NULL;
    unsigned int i;

    for(i = 0; (i < OXS_ERRORS_MAX_NUMBER) && (oxs_errors_get_msg(i) != NULL); ++i) {
            if(oxs_errors_get_code(i) == code) 
            {
                error_msg = oxs_errors_get_msg(i);
                break;
            }
    }
    return error_msg;

}

AXIS2_EXTERN int AXIS2_CALL
oxs_errors_get_code(unsigned int pos) {
    if(pos < sizeof(oxs_errors_table) / sizeof(oxs_errors_table[0])) {
	    return(oxs_errors_table[pos].code);
    }
    return(0);
}

AXIS2_EXTERN const char* AXIS2_CALL
oxs_errors_get_msg(unsigned int pos) {
    if(pos < sizeof(oxs_errors_table) / sizeof(oxs_errors_table[0])) {
	    return(oxs_errors_table[pos].message);
    }
    return(NULL);
}

AXIS2_EXTERN void	AXIS2_CALL
oxs_error(const char* file, int line, const char* func, 
  	    int code, const char* msg, ...) 
{
    const char* error_msg = NULL;
    
    error_msg = oxs_errors_get_msg_by_code(code);	    
    /*TODO Log instead of printf*/
    printf("\nERROR [%s:%d in %s] %s , %s\n", file, line, func, error_msg, msg);
    
}
