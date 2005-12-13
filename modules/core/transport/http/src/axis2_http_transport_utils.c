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
 
#include <axis2_http_transport_utils.h>

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axis2_http_transport_utils_process_http_post_request
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx, 
                        axis2_stream_t *stream, axis2_char_t *content_type, 
                        axis2_char_t *soap_action_header,
                        axis2_char_t *request_uri);
    
axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_process_http_get_request
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx,
                        axis2_stream_t *stream, axis2_char_t *content_type,
                        axis2_char_t *soap_action_header, 
                        axis2_char_t *request_uri, axis2_conf_ctx_t *conf_ctx, 
                        axis2_hash_t *request_params);
    
axis2_soap_envelope_t* AXIS2_CALL 
axis2_http_transport_utils_create_envelope_from_get_request
                        (axis2_env_t **env, axis2_char_t *request_uri,
                        axis2_hash_t *request_params);
    
axis2_om_stax_builder_t* AXIS2_CALL 
axis2_http_transport_utils_select_builder_for_mime
                        (axis2_env_t **env, axis2_char_t *request_uri,
                        axis2_msg_ctx_t *msg_ctx, axis2_stream_t *in_stream,
                        axis2_char_t *content_type);
    
axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_is_optimized(axis2_env_t **env, 
                                        axis2_om_element_t *om_element);
                                                
axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_do_write_mtom(axis2_env_t **env, 
                                        axis2_msg_ctx_t *msg_ctx);
                                                
axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_is_doing_rest(axis2_env_t **env, 
                                        axis2_msg_ctx_t *msg_ctx);
                                                
axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_is_doing_rest_through_post
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx);
                                
/***************************** End of function headers ************************/

axis2_status_t AXIS2_CALL 
axis2_http_transport_utils_process_http_post_request
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx, 
                        axis2_stream_t *stream, axis2_char_t *content_type, 
                        axis2_char_t *soap_action_header,
                        axis2_char_t *request_uri)
{
    /*
        TODO code
    */
    return AXIS2_SUCCESS;
}


axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_process_http_get_request
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx,
                        axis2_stream_t *stream, axis2_char_t *content_type,
                        axis2_char_t *soap_action_header, 
                        axis2_char_t *request_uri, axis2_conf_ctx_t *conf_ctx, 
                        axis2_hash_t *request_params)
{
    return AXIS2_FALSE;
}


axis2_soap_envelope_t* AXIS2_CALL 
axis2_http_transport_utils_create_envelope_from_get_request
                        (axis2_env_t **env, axis2_char_t *request_uri,
                        axis2_hash_t *request_params)
{
    return NULL;
}


axis2_om_stax_builder_t* AXIS2_CALL 
axis2_http_transport_utils_select_builder_for_mime
                        (axis2_env_t **env, axis2_char_t *request_uri,
                        axis2_msg_ctx_t *msg_ctx, axis2_stream_t *in_stream,
                        axis2_char_t *content_type)
{
    return NULL;
}


axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_is_optimized(axis2_env_t **env, 
                                        axis2_om_element_t *om_element)
{
    return AXIS2_FALSE;
}


axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_do_write_mtom(axis2_env_t **env, 
                                        axis2_msg_ctx_t *msg_ctx)
{
   /*
        TODO implement when MTOM support is added
    */
    return AXIS2_FALSE;
}


axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_is_doing_rest(axis2_env_t **env, 
                                        axis2_msg_ctx_t *msg_ctx)
{
    /*
        TODO implement when REST support is added
    */
    return AXIS2_FALSE;
}


axis2_bool_t AXIS2_CALL 
axis2_http_transport_utils_is_doing_rest_through_post
                        (axis2_env_t **env, axis2_msg_ctx_t *msg_ctx)
{
    /*
        TODO implement when REST support is added
    */
    return AXIS2_FALSE;
}
