/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *mime_parser
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <axis2_mime_parser.h>
#include <axis2_string.h>
#include <axis2_data_handler.h>

typedef struct axis2_mime_parser_impl
{
	axis2_mime_parser_t mime_parser;
    axis2_hash_t *mime_parts_map;
    int soap_body_len;
    axis2_char_t* soap_body_str;
} axis2_mime_parser_impl_t;

#define AXIS2_INTF_TO_IMPL(mime_parser) ((axis2_mime_parser_impl_t *)(mime_parser))

#define AXIS2_MIME_PARSER_BUFFER_SIZE (1024 * 1024)

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axis2_mime_parser_free (axis2_mime_parser_t *mime_parser, axis2_env_t **env);

axis2_hash_t* AXIS2_CALL
axis2_mime_parser_parse(axis2_mime_parser_t *mime_parser,
    axis2_env_t **env, 
    AXIS2_READ_INPUT_CALLBACK,
    void *callback_ctx,
    axis2_char_t *mime_boundary);

axis2_hash_t* AXIS2_CALL
axis2_mime_parser_get_mime_parts_map(axis2_mime_parser_t *mime_parser, 
    axis2_env_t **env);

int AXIS2_CALL
axis2_mime_parser_get_soap_body_len(axis2_mime_parser_t *mime_parser, 
    axis2_env_t **env);

axis2_char_t* AXIS2_CALL
axis2_mime_parser_get_soap_body_str(axis2_mime_parser_t *mime_parser, 
    axis2_env_t **env);


/************************** End of Function headers ************************/

AXIS2_DECLARE(axis2_mime_parser_t *)
axis2_mime_parser_create (axis2_env_t **env)
{
    axis2_mime_parser_impl_t *mime_parser_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	mime_parser_impl = (axis2_mime_parser_impl_t *) AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_mime_parser_impl_t));
		
	if(NULL == mime_parser_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
	
    mime_parser_impl->mime_parser.ops = NULL;
    mime_parser_impl->mime_parts_map = NULL;
    mime_parser_impl->soap_body_len = 0;
    mime_parser_impl->soap_body_str = NULL; /* shallow copy */
    
    mime_parser_impl->mime_parts_map = axis2_hash_make(env);
    if (!(mime_parser_impl->mime_parts_map))
    {
        axis2_mime_parser_free(&(mime_parser_impl->mime_parser), env);
        return NULL;
    }
    
	mime_parser_impl->mime_parser.ops = AXIS2_MALLOC ((*env)->allocator, 
        sizeof(axis2_mime_parser_ops_t));
	if(NULL == mime_parser_impl->mime_parser.ops)
    {
        axis2_mime_parser_free(&(mime_parser_impl->mime_parser), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	mime_parser_impl->mime_parser.ops->free =  axis2_mime_parser_free;
    mime_parser_impl->mime_parser.ops->parse = axis2_mime_parser_parse;
    mime_parser_impl->mime_parser.ops->get_mime_parts_map = axis2_mime_parser_get_mime_parts_map;
	mime_parser_impl->mime_parser.ops->get_soap_body_len = axis2_mime_parser_get_soap_body_len;
	mime_parser_impl->mime_parser.ops->get_soap_body_str = axis2_mime_parser_get_soap_body_str;
    
	return &(mime_parser_impl->mime_parser);
}

/*************************** Start of op impls *************************/

axis2_status_t AXIS2_CALL    

axis2_mime_parser_free (axis2_mime_parser_t *mime_parser, axis2_env_t **env)
{
    axis2_mime_parser_impl_t *mime_parser_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    mime_parser_impl = AXIS2_INTF_TO_IMPL(mime_parser);
    
    if (mime_parser_impl->mime_parts_map)
    {
        axis2_hash_free(mime_parser_impl->mime_parts_map, env);
        mime_parser_impl->mime_parts_map = NULL;
    }
    
    if(NULL != mime_parser->ops)
    {
        AXIS2_FREE((*env)->allocator, mime_parser->ops);
        mime_parser->ops = NULL;
    }
    
    if(mime_parser_impl)
    {
        AXIS2_FREE((*env)->allocator, mime_parser_impl);
        mime_parser_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}


axis2_hash_t* AXIS2_CALL
axis2_mime_parser_parse(axis2_mime_parser_t *mime_parser,
    axis2_env_t **env, 
    AXIS2_READ_INPUT_CALLBACK callback, 
    void *callback_ctx,
    axis2_char_t *mime_boundary)
{
    axis2_mime_parser_impl_t *mime_parser_impl = NULL;
    axis2_char_t *buffer = NULL;
    int size = AXIS2_MIME_PARSER_BUFFER_SIZE;
    int len = 0;
    axis2_char_t *root_mime = NULL;
    int root_mime_len = 0;
    axis2_char_t *soap_body_str = NULL;
    int soap_body_len = 0;
    axis2_char_t *body_mime = NULL;
    int body_mime_len = 0;
    axis2_char_t *mime_binary = NULL;
    int mime_binary_len = 0;
    axis2_char_t *pos = NULL;
    axis2_bool_t end_of_mime = AXIS2_FALSE;
    
    AXIS2_ENV_CHECK(env, NULL);
    mime_parser_impl = AXIS2_INTF_TO_IMPL(mime_parser);
    
    buffer = AXIS2_MALLOC((*env)->allocator, sizeof(axis2_char_t) * (size + 1));
   
    do 
    {    
        len = callback(buffer, size, (void*)callback_ctx);
        if (len > 0)
        {
            axis2_char_t *temp_root_mime = root_mime;
            root_mime = AXIS2_MALLOC((*env)->allocator, 
                sizeof(char) * (root_mime_len + len + 1));
            if (root_mime)
            {
                if (temp_root_mime)
                {
                    memcpy(root_mime, temp_root_mime, root_mime_len);
                    AXIS2_FREE((*env)->allocator, temp_root_mime);
                    temp_root_mime = NULL;
                }
                
                memcpy(root_mime + root_mime_len, buffer, len);
                root_mime_len += len;
                root_mime[root_mime_len] = '\0';

                pos = AXIS2_STRSTR(root_mime, "\r\n\r\n");
                if (pos)
                {
                    if (root_mime_len > (pos - root_mime + 4))
                    {
                        soap_body_len = root_mime_len - (pos - root_mime + 4);
                        soap_body_str = AXIS2_MALLOC((*env)->allocator,
                                        sizeof(char) * (soap_body_len + 1));
                        memcpy(soap_body_str, pos + 4, soap_body_len);
                        soap_body_str[soap_body_len] = '\0';
                        *pos = '\0';
                    }
                }
            }
        }
        
    } while(!pos && len > 0);

    pos = NULL;
    len = 0;

    do 
    {
        if (soap_body_str)
        {
            pos = AXIS2_STRSTR(soap_body_str, mime_boundary);
        }
        
        if (pos)
        {
            pos -= 2;
            body_mime_len = soap_body_len - (pos - soap_body_str);
            body_mime = AXIS2_MALLOC((*env)->allocator,
                            sizeof(char) * (body_mime_len + 1));
            memcpy(body_mime, pos, body_mime_len);
            body_mime[body_mime_len] = '\0';
            
            *(pos)  = '\0';
            soap_body_len = (pos - soap_body_str);
        }
        else
        {
            len = callback(buffer, size, (void*)callback_ctx);
            if (len > 0)
            {
                axis2_char_t *temp_soap_body = soap_body_str;
                soap_body_str = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(char) * (soap_body_len + len + 1));
                if (soap_body_str)
                {
                    if (temp_soap_body)
                    {
                        memcpy(soap_body_str, temp_soap_body, soap_body_len);
                        AXIS2_FREE((*env)->allocator, temp_soap_body);
                        temp_soap_body = NULL;
                    }
                    
                    memcpy(soap_body_str + soap_body_len, buffer, len);
                    soap_body_len += len;
                    soap_body_str[soap_body_len] = '\0';
                }
             }
         }
    } while(!pos && len > 0);
    
    if (soap_body_str) 
    {
        mime_parser_impl->soap_body_len = soap_body_len;
        mime_parser_impl->soap_body_str = soap_body_str;
    }

    while (!end_of_mime)
    {
        axis2_char_t *temp_body_mime = NULL;
        int temp_body_mime_len = 0;
        pos = NULL;
        len = 0;
    
        do 
        {
            if (body_mime)
            {
                pos = AXIS2_STRSTR(body_mime, "\r\n\r\n");
            }
            
            if (pos)
            {
                if (body_mime_len > (pos - body_mime + 4))
                {
                    mime_binary_len = body_mime_len - (pos - body_mime + 4);
                    mime_binary = AXIS2_MALLOC((*env)->allocator,
                                    sizeof(char) * (mime_binary_len + 1));
                    memcpy(mime_binary, pos + 4, mime_binary_len);
                    mime_binary[mime_binary_len] = '\0';
                    *pos = '\0';
                }
            }
            else
            {
                len = callback(buffer, size, (void*)callback_ctx);
                if (len > 0)
                {
                    temp_body_mime = body_mime;
                    body_mime = AXIS2_MALLOC((*env)->allocator, 
                        sizeof(char) * (body_mime_len + len + 1));
                    if (body_mime)
                    {
                        if (temp_body_mime)
                        {
                            memcpy(body_mime, temp_body_mime, body_mime_len);
                            AXIS2_FREE((*env)->allocator, temp_body_mime);
                            temp_body_mime = NULL;
                        }
                        
                        memcpy(body_mime + body_mime_len, buffer, len);
                        body_mime_len += len;
                        body_mime[body_mime_len] = '\0';
                    }
                 }
            }
        } while(!pos && len > 0);

        pos = NULL;
        len = 0;
        do 
        {
            axis2_char_t *old_pos = NULL;
            int old_mime_binary_len = 0;
            if (mime_binary)
            {
                axis2_char_t *temp_pos = NULL;
                old_pos = mime_binary;
                
                do
                {
                    pos = memchr(old_pos, AXIS2_MIME_BOUNDARY_BYTE, 
                        (mime_binary_len - (old_pos - mime_binary)));
                    if (!pos)
                        break;
                    else
                    {
                        old_pos = pos + 1;
                        if (old_pos - mime_binary > mime_binary_len)
                        {
                            pos = NULL;
                            break;
                        }
                        temp_pos = AXIS2_STRSTR(pos + 1, mime_boundary);
                    }
                } while ( *(pos + 1) != AXIS2_MIME_BOUNDARY_BYTE || temp_pos != pos + 2);
    
                if ( pos && *(pos + 1) == AXIS2_MIME_BOUNDARY_BYTE && temp_pos == pos + 2)
                {
                    old_mime_binary_len = mime_binary_len;
                    mime_binary_len = pos - mime_binary;
                }
                else
                    pos = NULL;
            }
            
            if (!pos)
            {
                len = callback(buffer, size, (void*)callback_ctx);
                if (len > 0)
                {
                    axis2_char_t *temp_mime_binary = mime_binary;
                    mime_binary = AXIS2_MALLOC((*env)->allocator, 
                        sizeof(char) * (mime_binary_len + len + 1));
                    if (mime_binary)
                    {
                        if (temp_mime_binary)
                        {
                            memcpy(mime_binary, temp_mime_binary, mime_binary_len);
                            AXIS2_FREE((*env)->allocator, temp_mime_binary);
                            temp_mime_binary = NULL;
                        }
                        
                        memcpy(mime_binary + mime_binary_len, buffer, len);
                        mime_binary_len += len;
                        mime_binary[mime_binary_len] = '\0';
                    }
                 }
            }
            else
            {
                axis2_char_t *temp_pos = NULL;
                temp_pos = pos + 2 + AXIS2_STRLEN(mime_boundary);

                end_of_mime = (AXIS2_MIME_BOUNDARY_BYTE == *(temp_pos)) &&
                    (AXIS2_MIME_BOUNDARY_BYTE == *(temp_pos + 1));
                
                /* capture the next mime part */
                temp_body_mime = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(char) * (old_mime_binary_len - mime_binary_len + 1));
                if (temp_body_mime)
                {
                    memcpy(temp_body_mime, pos, old_mime_binary_len - mime_binary_len);
                    temp_body_mime_len = old_mime_binary_len - mime_binary_len;
                    temp_body_mime[temp_body_mime_len] = '\0';
                }
            }
        } while(!pos && len > 0);
    
    
        if (mime_parser_impl->mime_parts_map)
        {
            /* get MIME ID */
            axis2_char_t *id = NULL;
            if (body_mime)
            {
                id = AXIS2_STRSTR(body_mime, "content-id");
            }
            if (id)
            {
                id += AXIS2_STRLEN("content-id");
                while (id && *id && *id != ':')
                    id++;
                if (id)
                {
                    while (id && *id && *id != '<')
                        id++;
                    id++;
                    if (id)
                    {
                        axis2_char_t *pos = NULL;
                        pos = AXIS2_STRSTR(id, ">");
                        if (pos)
                        {
                            axis2_char_t *mime_id = NULL;
                            int mime_id_len = 0;
                            mime_id_len = pos - id;
                            mime_id = AXIS2_MALLOC((*env)->allocator, 
                                sizeof(axis2_char_t) * mime_id_len + 1);
                            if (mime_id)
                            {
                                
                                axis2_data_handler_t *data_handler = NULL;
                                memcpy(mime_id, id, mime_id_len);
                                mime_id[mime_id_len] = '\0';
                                data_handler = axis2_data_handler_create(env, NULL, NULL);
                                AXIS2_DATA_HANDLER_SET_BINARY_DATA(data_handler, env,
                                    mime_binary, mime_binary_len);                                        
                                axis2_hash_set(mime_parser_impl->mime_parts_map, mime_id,
                                    AXIS2_HASH_KEY_STRING, data_handler);                                        
                            }
                        }
                    }
                }
            }
            
            body_mime = temp_body_mime;
            body_mime_len = temp_body_mime_len;
            
        }/*if (mime_parser_impl->mime_parts_map)*/
    }/* end while (!end_of_mime) */
    
    return mime_parser_impl->mime_parts_map;
}

axis2_hash_t* AXIS2_CALL
axis2_mime_parser_get_mime_parts_map(axis2_mime_parser_t *mime_parser, 
    axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(mime_parser)->mime_parts_map;
}

int AXIS2_CALL
axis2_mime_parser_get_soap_body_len(axis2_mime_parser_t *mime_parser, 
    axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(mime_parser)->soap_body_len;
}

axis2_char_t* AXIS2_CALL
axis2_mime_parser_get_soap_body_str(axis2_mime_parser_t *mime_parser, 
    axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(mime_parser)->soap_body_str;
}
