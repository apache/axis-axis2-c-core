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
 
#ifndef AXIS2_HTTP_STATUS_LINE_H
#define AXIS2_HTTP_STATUS_LINE_H

/**
 * @file axis2_http_status_line.h
 * @brief axis2 HTTP Status Line
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

typedef struct axis2_http_status_line_ops axis2_http_status_line_ops_t;
typedef struct axis2_http_status_line axis2_http_status_line_t;
    
    
/** 
 * @brief HTTP Status Line ops struct
 * Encapsulator struct for ops of axis2_http_status_line
 */
AXIS2_DECLARE_DATA struct axis2_http_status_line_ops
{
    int (AXIS2_CALL *
   get_status_code)(axis2_http_status_line_t *status_line, 
                     const axis2_env_t *env);
    
    axis2_char_t* (AXIS2_CALL *
   get_http_version)(axis2_http_status_line_t *status_line, 
                 const axis2_env_t *env);
    
    axis2_char_t* (AXIS2_CALL *
   get_reason_phrase)(axis2_http_status_line_t *status_line, 
                  const axis2_env_t *env);
    
    axis2_bool_t (AXIS2_CALL *
   starts_with_http)(axis2_http_status_line_t *status_line, 
                    const axis2_env_t *env);
    
    axis2_char_t* (AXIS2_CALL *
   to_string)(axis2_http_status_line_t *status_line, 
             const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
   free) (axis2_http_status_line_t *status_line, 
           const axis2_env_t *env);
};

/**
 * @brief HTTP Status Line struct
 * Axis2 HTTP Status Line
 */
AXIS2_DECLARE_DATA struct axis2_http_status_line
{
    axis2_http_status_line_ops_t *ops;
};


AXIS2_EXTERN axis2_http_status_line_t * AXIS2_CALL 
axis2_http_status_line_create(const axis2_env_t *env, 
                              axis2_char_t *str);

/************************* Start of function macros    ***************************/

#define AXIS2_HTTP_STATUS_LINE_GET_STATUS_CODE(status_line, env)\
                    ((status_line)->ops->get_status_code (status_line, env))
#define AXIS2_HTTP_STATUS_LINE_GET_HTTP_VERSION(status_line, env)\
                    ((status_line)->ops->get_http_version (status_line, env))
#define AXIS2_HTTP_STATUS_LINE_GET_REASON_PHRASE(status_line, env)\
                    ((status_line)->ops->get_reason_phrase (status_line, env))
#define AXIS2_HTTP_STATUS_LINE_STARTS_WITH_HTTP(status_line, env)\
                    ((status_line)->ops->starts_with_http (status_line, env))
#define AXIS2_HTTP_STATUS_LINE_TO_STRING(status_line, env)\
                    ((status_line)->ops->to_string (status_line, env))
#define AXIS2_HTTP_STATUS_LINE_FREE(status_line, env)\
                    ((status_line)->ops->free(status_line, env))

/************************* End of function macros *****************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_HTTP_STATUS_LINE_H */
