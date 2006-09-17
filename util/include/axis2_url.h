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

#ifndef AXIS2_URL_H
#define AXIS2_URL_H


/**
  * @file axis2_url.h
  * @brief axis2 URL container implementation
  */
  
#include <axis2_utils.h>
#include <axis2_utils_defines.h>
#include <axis2_env.h>
#include <axis2_uri.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @ingroup axis2_core_transport_http
 * @{
 */

typedef struct axis2_url_ops axis2_url_ops_t;
typedef struct axis2_url axis2_url_t;

    
/** 
 * @brief URL ops struct
 * Encapsulator struct for ops of axis2_url
 */  
 struct axis2_url_ops
{
    axis2_char_t* (AXIS2_CALL *
   to_external_form) (axis2_url_t *url, 
                       const axis2_env_t *env);
   
    axis2_status_t (AXIS2_CALL *
   set_protocol)(axis2_url_t *url, 
                  const axis2_env_t *env, 
                 axis2_char_t *protocol);
   
   
   axis2_char_t* (AXIS2_CALL *
   get_protocol) (axis2_url_t *url, 
                   const axis2_env_t *env);
   
   axis2_status_t (AXIS2_CALL *
   set_server)(axis2_url_t *url, 
                const axis2_env_t *env, 
            axis2_char_t *server);
   
   
    axis2_char_t* (AXIS2_CALL *
   get_server)(axis2_url_t *url, 
                const axis2_env_t *env);
            
            
   axis2_status_t (AXIS2_CALL *
   set_port)(axis2_url_t *url, 
              const axis2_env_t *env, 
           int port);
           
           
    int (AXIS2_CALL *
   get_port) (axis2_url_t *url, 
               const axis2_env_t *env);
            
            
   axis2_status_t (AXIS2_CALL *
   set_path)(axis2_url_t *url, 
              const axis2_env_t *env, 
           axis2_char_t *path);
           
           
   axis2_char_t* (AXIS2_CALL *
   get_path)(axis2_url_t *url, 
              const axis2_env_t *env);

    axis2_url_t* (AXIS2_CALL *
    clone) (
            axis2_url_t *url,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
   free_fn) (axis2_url_t *url, 
           const axis2_env_t *env);
};

/** 
 * @brief URL struct
 *    Axis2 URL
 */
 struct axis2_url
{
    axis2_url_ops_t *ops;    
};


AXIS2_EXTERN axis2_url_t * AXIS2_CALL 
axis2_url_create (
        const axis2_env_t *env, 
        const axis2_char_t *protocol, 
        const axis2_char_t *server, 
        const int port, 
        const axis2_char_t *path);

AXIS2_EXTERN axis2_url_t * AXIS2_CALL
axis2_url_parse_string(
        const axis2_env_t *env, 
        const axis2_char_t *str_url);

axis2_uri_t *AXIS2_CALL
axis2_url_to_uri(
        axis2_url_t *url,
        const axis2_env_t *env);

/************************** Start of function macros **************************/

#define AXIS2_URL_TO_EXTERNAL_FORM(url, env) \
      ((url)->ops->to_external_form(url, env))

#define AXIS2_URL_SET_PROTOCOL(url, env, protocol) \
        ((url)->ops->set_prtocol(url, env, protocol))
      
#define AXIS2_URL_GET_PROTOCOL(url, env) \
      ((url)->ops->get_protocol(url, env))
      
#define AXIS2_URL_SET_SERVER(url, env, server) \
        ((url)->ops->set_server(url, env, server))
      
#define AXIS2_URL_GET_SERVER(url, env) \
      ((url)->ops->get_server(url, env))
      
#define AXIS2_URL_SET_PORT(url, env, port) \
      ((url)->ops->set_port(url, env, port))
      
#define AXIS2_URL_GET_PORT(url, env) \
      ((url)->ops->get_port(url, env))
      
#define AXIS2_URL_SET_PATH(url, env, path) \
        ((url)->ops->set_path(url, env, path))
      
#define AXIS2_URL_GET_PATH(url, env) \
      ((url)->ops->get_path(url, env))
      
#define AXIS2_URL_CLONE(url, env) \
      ((url)->ops->clone(url, env))
      
#define AXIS2_URL_FREE(url, env) \
      ((url)->ops->free_fn(url, env))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_URL_H */
