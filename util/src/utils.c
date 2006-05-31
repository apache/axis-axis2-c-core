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

#include <axis2_utils.h>
#include <stdlib.h>
#include <axis2_string.h>
#include <string.h>
#include <platforms/axis2_platform_auto_sense.h>

AXIS2_DECLARE(axis2_char_t**)
axis2_parse_request_url_for_svc_and_op(const axis2_env_t *env, 
                                                axis2_char_t *request)

{
    axis2_char_t **ret = NULL;
    axis2_char_t *service_str = NULL;
    axis2_char_t *tmp = NULL;
    int i = 0;
    AXIS2_ENV_CHECK(env, NULL);
    ret  = AXIS2_MALLOC(env->allocator, 
                                        2*(sizeof(axis2_char_t *)));
    memset(ret, 0, 2*sizeof(axis2_char_t*)); 
    tmp = request;

    
    while(1)
    {
        tmp = strstr(tmp, AXIS2_REQUEST_URL_PREFIX);
        if(NULL == tmp)
            break;
        else
        {
            service_str = tmp;
            tmp += AXIS2_STRLEN(AXIS2_REQUEST_URL_PREFIX);
        }
    }
    if(NULL != service_str)
    {
        service_str += AXIS2_STRLEN(AXIS2_REQUEST_URL_PREFIX);
        if('\0' != *service_str)
        {
            service_str++; /*to remove the leading '/' */
            tmp = strchr(service_str, '/');
            if(NULL != tmp)
            {
                i = tmp - service_str;
                ret[0] = AXIS2_MALLOC(env->allocator, i*sizeof(char)+1);
                strncpy(ret[0], service_str,i);
                ret[0][i] = '\0';

                /* Now search for the op */
                service_str = tmp;
                if('\0' != *service_str)
                {
                    service_str++;
                    tmp = strchr(service_str, '?');
                    if(NULL != tmp)
                    {
                        i = tmp - service_str;
                        ret[1] = AXIS2_MALLOC(env->allocator, 
                                                i*sizeof(char)+1);
                        strncpy(ret[1], service_str,i);
                        ret[1][i] = '\0';
                    }
                    else
                    {
                        ret[1] = AXIS2_STRDUP(service_str, env);
                    }
                }
            }
            else
            {
                ret[0] = AXIS2_STRDUP(service_str, env);
            }
        }
    }
    return ret;
}




