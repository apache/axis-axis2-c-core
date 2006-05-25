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

#ifndef AXIS2_MIME_PARSER_H
#define AXIS2_MIME_PARSER_H

/**
 * @file axis2_mime_parser.h
 * @brief axis2 mime_parser interface
 */

#include <axis2_utils.h>
#include <axis2_error.h>
#include <axis2_utils_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_hash.h>
#include <axis2_mime_const.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_mime_parser_ops axis2_mime_parser_ops_t;
typedef struct axis2_mime_parser axis2_mime_parser_t;	
	

/** @defgroup axis2_mime_parser Flow
  * @ingroup axis2_mime_parser
  * @{
  */

/** 
 * @brief Flow ops struct
 * Encapsulator struct for ops of axis2_mime_parser
 */
struct axis2_mime_parser_ops
{
    axis2_hash_t* (AXIS2_CALL *
    parse)(
        axis2_mime_parser_t *mime_parser,
        axis2_env_t **env, 
        AXIS2_READ_INPUT_CALLBACK,
        void *callback_ctx,
        axis2_char_t *mime_boundary);
    
    axis2_hash_t* (AXIS2_CALL *
    get_mime_parts_map)(
        axis2_mime_parser_t *mime_parser, 
        axis2_env_t **env);
    
	/** Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free)(
        axis2_mime_parser_t *mime_parser,
        axis2_env_t **env);
    
    int (AXIS2_CALL *
    get_soap_body_len)(
        axis2_mime_parser_t *mime_parser, 
        axis2_env_t **env);
};

/** 
 * @brief Flow struct
 *	Flow  
 */ 
struct axis2_mime_parser
{
	axis2_mime_parser_ops_t *ops;
};

/**
 * Creates mime_parser struct
 * @return pointer to newly created mime_parser
 */
AXIS2_DECLARE(axis2_mime_parser_t *) 
axis2_mime_parser_create (axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_MIME_PARSER_FREE(mime_parser, env) \
((mime_parser)->ops->free (mime_parser, env))

#define AXIS2_MIME_PARSER_PARSE(mime_parser, env, callback) \
((mime_parser)->ops->parse(mime_parser, env, callback, callback_ctx, mime_boundary))

#define AXIS2_MIME_PARSER_GET_MIME_PARTS_MAP(mime_parser, env) \
((mime_parser)->ops->get_mime_parts_map(mime_parser, env))

#define AXIS2_MIME_PARSER_GET_SOAP_BODY_LENGTH(mime_parser, env) \
((mime_parser)->ops->get_soap_body_len(mime_parser, env))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_MIME_PARSER_H */
