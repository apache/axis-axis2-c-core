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
 
#include <axis2_http_out_transport_info.h>
#include <axis2_string.h>
#include <axis2_http_transport.h>
#include <axis2_string.h>


/** 
 * @brief HTTP Header struct impl
 *   Axis2 HTTP Header impl  
 */
typedef struct axis2_http_out_transport_info_impl 
            axis2_http_out_transport_info_impl_t;  
  
struct axis2_http_out_transport_info_impl
{
   axis2_http_out_transport_info_t out_transport_info;
   axis2_http_simple_response_t *response;
   axis2_char_t *encoding;
};

#define AXIS2_INTF_TO_IMPL(out_transport_info) \
                ((axis2_http_out_transport_info_impl_t *)(out_transport_info))

/***************************** Function headers *******************************/
axis2_status_t AXIS2_CALL 
axis2_http_out_transport_info_set_content_type 
            (axis2_http_out_transport_info_t *info, const axis2_env_t *env, 
            axis2_char_t *content_type);
    
axis2_status_t AXIS2_CALL 
axis2_http_out_transport_info_set_char_encoding 
            (axis2_http_out_transport_info_t *info, const axis2_env_t *env,
            axis2_char_t *encoding);
    
axis2_status_t AXIS2_CALL 
axis2_http_out_transport_info_free 
            (axis2_http_out_transport_info_t *out_transport_info, 
            const axis2_env_t *env);

/***************************** End of function headers ************************/

axis2_http_out_transport_info_t * AXIS2_CALL 
axis2_http_out_transport_info_create(const axis2_env_t *env,
               axis2_http_simple_response_t *response)
{
    axis2_http_out_transport_info_impl_t *info_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
        
    info_impl = (axis2_http_out_transport_info_impl_t *)AXIS2_MALLOC 
                        (env->allocator, sizeof(
                        axis2_http_out_transport_info_impl_t));
   
    if(NULL == info_impl)
   {
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
   }
    info_impl->response = response;
     info_impl->encoding = NULL;  
     
    info_impl->out_transport_info.ops = AXIS2_MALLOC(env->allocator,
                    sizeof(axis2_http_out_transport_info_ops_t));
    if(NULL == info_impl->out_transport_info.ops)
   {
      axis2_http_out_transport_info_free((axis2_http_out_transport_info_t*)
                         info_impl, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
      return NULL;
   }

    info_impl->out_transport_info.ops->set_content_type = 
                  axis2_http_out_transport_info_set_content_type;
    info_impl->out_transport_info.ops->set_char_encoding = 
                  axis2_http_out_transport_info_set_char_encoding;
    info_impl->out_transport_info.ops->free = 
                  axis2_http_out_transport_info_free;
                        
   return &(info_impl->out_transport_info);
}


axis2_status_t AXIS2_CALL 
axis2_http_out_transport_info_free (axis2_http_out_transport_info_t *info, 
                  const axis2_env_t *env)
{
    axis2_http_out_transport_info_impl_t *info_impl = NULL;
   AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    info_impl = AXIS2_INTF_TO_IMPL(info);
   
   info_impl->response = NULL; /* response doesn't belong to info */
    if(NULL != info_impl->encoding)
    {
        AXIS2_FREE(env->allocator, info_impl->encoding);
        info_impl->encoding = NULL;
    }
    if(NULL != info->ops)
        AXIS2_FREE(env->allocator, info->ops);
    
   AXIS2_FREE(env->allocator, info_impl);
   return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_http_out_transport_info_free_void_arg (void *transport_info,
                                             const axis2_env_t *env)
{
    axis2_http_out_transport_info_t *transport_info_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    transport_info_l = (axis2_http_out_transport_info_t *) transport_info;
    return axis2_http_out_transport_info_free(transport_info_l, env);
}

axis2_status_t AXIS2_CALL 
axis2_http_out_transport_info_set_content_type 
            (axis2_http_out_transport_info_t *info, const axis2_env_t *env, 
            axis2_char_t *content_type)
{
    axis2_char_t *tmp1 = NULL;
   axis2_char_t *tmp2 = NULL;
   axis2_http_out_transport_info_impl_t *info_impl = NULL;
   
   AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, content_type, AXIS2_FAILURE);
   
   info_impl = AXIS2_INTF_TO_IMPL(info);
   
   if(NULL != info_impl->encoding)
   {
      
      tmp1 = AXIS2_STRACAT(content_type, ";charset=", env);
      tmp2 = AXIS2_STRACAT(tmp1, info_impl->encoding, env);
      AXIS2_HTTP_SIMPLE_RESPONSE_SET_HEADER(info_impl->response, env, 
            axis2_http_header_create(env, AXIS2_HTTP_HEADER_CONTENT_TYPE, 
            tmp2));
      AXIS2_FREE(env->allocator, tmp1);
      AXIS2_FREE(env->allocator, tmp2);      
   }
   else
   {
      AXIS2_HTTP_SIMPLE_RESPONSE_SET_HEADER(info_impl->response, env, 
            axis2_http_header_create(env, AXIS2_HTTP_HEADER_CONTENT_TYPE, 
            content_type));
   }
   return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_http_out_transport_info_set_char_encoding 
            (axis2_http_out_transport_info_t *info, const axis2_env_t *env,
            axis2_char_t *encoding)
{
    axis2_http_out_transport_info_impl_t *info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK(env->error, encoding, AXIS2_FAILURE);
   
    info_impl = AXIS2_INTF_TO_IMPL(info);
   
   if(NULL != info_impl->encoding)
   {
      AXIS2_FREE(env->allocator, info_impl->encoding);
   }
   info_impl->encoding = AXIS2_STRDUP(encoding, env);
   
   return AXIS2_SUCCESS;
}
