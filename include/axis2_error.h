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

#ifndef AXIS2_ERROR_H
#define AXIS2_ERROR_H

#include <axis2_defines.h>
#include <axis2_allocator.h>

#ifdef __cplusplus
extern "C" {
#endif

struct axis2_error;
struct axis2_error_ops;

typedef struct axis2_error_ops {
    axis2_char_t* (*get_message)();
} axis2_error_ops_t;

typedef struct axis2_error {
    struct axis2_error_ops *ops;
    int errorno;
} axis2_error_t;

axis2_error_t *axis2_error_create(axis2_allocator_t* allocator);

#define axis2_error_get_message(error) ((error)->ops->get_message())

typedef enum axis2_status_codes {
    AXIS2_FAILURE = 0,
    AXIS2_SUCCESS
} axis2_status_codes_t;

typedef enum axis2_error_codes {
    AXIS2_ERROR_NONE = 0,
    AXIS2_ERROR_NO_MEMORY,
    AXIS2_ERROR_INVALID_NULL_PARAMETER,
    AXIS2_ERROR_INVALID_ITERATOR_STATE,
    AXIS2_ERROR_INVALID_NODE_TYPE,
	AXIS2_ERROR_PULL_PARSER_ELEMENT_NULL,
	AXIS2_ERROR_PULL_PARSER_VALUE_NULL,
	AXIS2_ERROR_BUILDER_DONE_CANNOT_PULL,
	AXIS2_ERROR_INVALID_BUILDER_STATE_LAST_NODE_NULL,
	AXIS2_ERROR_INVALID_BUILDER_STATE_CANNOT_DISCARD,
	AXIS2_ERROR_INVALID_DOCUMENT_STATE_ROOT_NULL,
	AXIS2_ERROR_INVALID_DOCUMENT_STATE_UNDEFINED_NAMESPACE,
	AXIS2_ERROR_UNALLOCATED_MEMEORY_RELEASE_REQUESTED
} axis2_error_codes_t;

#ifdef __cplusplus
}
#endif


#endif /* AXIS2_ERROR_H */
