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
 
#ifndef AXIS2_HTTP_OUT_TRANSPORT_INFO_H
#define AXIS2_HTTP_OUT_TRANSPORT_INFO_H

/**
 * @file axis2_http_out_transport_info.h
 * @brief axis2 HTTP Out Transport Info
 */

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_http_simple_response.h>


#ifdef __cplusplus
extern "C" 
{
#endif
/** 
  * @ingroup axis2_core_transport_http
  * @{
  */

typedef struct axis2_http_out_transport_info_ops 
               axis2_http_out_transport_info_ops_t;
typedef struct axis2_http_out_transport_info axis2_http_out_transport_info_t;
    
    
/** 
 * @brief HTTP Out Transport Info ops struct
 * Encapsulator struct for ops of axis2_http_out_transport_info
 */
AXIS2_DECLARE_DATA struct axis2_http_out_transport_info_ops
{
    int (AXIS2_CALL *set_content_type) (axis2_http_out_transport_info_t *info, 
               const axis2_env_t *env, 
               const axis2_char_t *content_type);
    
    axis2_status_t (AXIS2_CALL *set_char_encoding) 
                    (axis2_http_out_transport_info_t *info, const axis2_env_t *env,
               const axis2_char_t *encoding);
    
    axis2_status_t (AXIS2_CALL *free) 
               (axis2_http_out_transport_info_t *out_transport_info, 
               const axis2_env_t *env);
};

/**
 * @brief HTTP out transport info
 * Axis2 HTTP out transport info
 */
AXIS2_DECLARE_DATA struct axis2_http_out_transport_info
{
    axis2_http_out_transport_info_ops_t *ops;
};


AXIS2_EXTERN axis2_http_out_transport_info_t * AXIS2_CALL 
axis2_http_out_transport_info_create(const axis2_env_t *env,
               axis2_http_simple_response_t *response);

/**
 * Free http_out_transport_info passed as void pointer. This will be
 * cast into appropriate type and then pass the cast object
 * into the http_out_transport_info structure's free method
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL 
axis2_http_out_transport_info_free_void_arg (void *transport_info,
                                             const axis2_env_t *env);

/************************* Start of function macros    ***************************/

#define AXIS2_HTTP_OUT_TRANSPORT_INFO_SET_CONTENT_TYPE(out_transport_info, \
               env, content_type) ((out_transport_info)->ops->\
               set_content_type (out_transport_info, env, content_type))
#define AXIS2_HTTP_OUT_TRANSPORT_INFO_SET_CHAR_ENCODING(out_transport_info,\
               env, encoding) ((out_transport_info)->ops->set_char_encoding \
               (out_transport_info, env, encoding))
#define AXIS2_HTTP_OUT_TRANSPORT_INFO_FREE(out_transport_info, env)\
                    ((out_transport_info)->ops->free(out_transport_info, env))

/************************* End of function macros *****************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_HTTP_OUT_TRANSPORT_INFO_H */
