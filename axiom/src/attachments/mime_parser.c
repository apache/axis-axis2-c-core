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

typedef struct axis2_mime_parser_impl
{
	axis2_mime_parser_t mime_parser;
    axis2_hash_t *mime_parts_map;
} axis2_mime_parser_impl_t;

#define AXIS2_INTF_TO_IMPL(mime_parser) ((axis2_mime_parser_impl_t *)(mime_parser))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axis2_mime_parser_free (axis2_mime_parser_t *mime_parser, axis2_env_t **env);

axis2_hash_t* AXIS2_CALL
axis2_mime_parser_parse(axis2_mime_parser_t *mime_parser,
    axis2_env_t **env, 
    AXIS2_READ_INPUT_CALLBACK);

axis2_hash_t* AXIS2_CALL
axis2_mime_parser_get_mime_parts_map(axis2_mime_parser_t *mime_parser, 
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
    AXIS2_READ_INPUT_CALLBACK callback)
{
    axis2_mime_parser_impl_t *mime_parser_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    mime_parser_impl = AXIS2_INTF_TO_IMPL(mime_parser);
    return mime_parser_impl->mime_parts_map;
}

axis2_hash_t* AXIS2_CALL
axis2_mime_parser_get_mime_parts_map(axis2_mime_parser_t *mime_parser, 
    axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(mime_parser)->mime_parts_map;
}
