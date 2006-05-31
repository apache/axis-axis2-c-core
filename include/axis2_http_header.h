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

#ifndef AXIS2_HTTP_HEADER_H
#define AXIS2_HTTP_HEADER_H


/**
  * @file axis2_http_header.h
  * @brief axis2 HTTP Header name:value pair implementation
  */
  
#include <axis2_const.h>
#include <axis2_defines.h>
#include <axis2_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @ingroup axis2_core_transport_http
 * @{
 */

typedef struct axis2_http_header_ops axis2_http_header_ops_t;
typedef struct axis2_http_header axis2_http_header_t;

    
/** 
 * @brief HTTP Header ops struct
 * Encapsulator struct for ops of axis2_http_header
 */  
AXIS2_DECLARE_DATA struct axis2_http_header_ops
{
    axis2_char_t* (AXIS2_CALL *to_external_form) (axis2_http_header_t *header, 
                    const axis2_env_t *env);
    
    axis2_char_t* (AXIS2_CALL *get_name) (axis2_http_header_t *header, 
                    const axis2_env_t *env);
    
    axis2_char_t* (AXIS2_CALL *get_value) (axis2_http_header_t *header, 
                    const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *free) (axis2_http_header_t *header, 
                    const axis2_env_t *env);
};

/** 
 * @brief HTTP Header struct
 *    Axis2 HTTP Header
 */
AXIS2_DECLARE_DATA struct axis2_http_header
{
    axis2_http_header_ops_t *ops;    
};


AXIS2_DECLARE(axis2_http_header_t *) 
axis2_http_header_create (const axis2_env_t *env, axis2_char_t *name, 
                            axis2_char_t *value);
AXIS2_DECLARE(axis2_http_header_t *) AXIS2_CALL
axis2_http_header_create_by_str (const axis2_env_t *env, axis2_char_t *str);
/************************** Start of function macros **************************/

#define AXIS2_HTTP_HEADER_TO_EXTERNAL_FORM(header, env) \
                                ((header)->ops->to_external_form(header, env))
#define AXIS2_HTTP_HEADER_GET_NAME(header, env) \
                                ((header)->ops->get_name(header, env))
#define AXIS2_HTTP_HEADER_GET_VALUE(header, env) \
                                ((header)->ops->get_value(header, env))
#define AXIS2_HTTP_HEADER_FREE(header, env) \
                                ((header)->ops->free(header, env))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_HTTP_HEADER_H */
