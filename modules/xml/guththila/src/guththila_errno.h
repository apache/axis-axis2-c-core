/*
 *   Copyright 2004,2005 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 */

#ifndef GUTHTHILA_ERRNO_H
#define GUTHTHILA_ERRNO_H

/**
 * @file guththila_errno.h
 * @brief Guththila Error Codes
 */

/**
 * GUTHTHILA_START_ERROR is where the Guththila specific error values start.
 */

#define GUTHTHILA_START_ERROR    20000

#define GUTHTHILA_ERROR_SPACE    50000

#define GUTHTHILA_START_STATUS         GUTHTHILA_START_ERROR + GUTHTHILA_ERROR_SPACE

/*
 * Error Codes
 */
#define GUTHTHILA_STREAM_WRITER_ERROR_NOT_IN_START_guththila_element_t        (GUTHTHILA_START_ERROR + 1)
#define GUTHTHILA_STREAM_WRITER_ERROR_WRITING_TO_STREAM        (GUTHTHILA_START_ERROR + 2)
#define GUTHTHILA_STREAM_WRITER_ERROR_STREAM_STRUCT_NULL        (GUTHTHILA_START_ERROR + 3)
#define GUTHTHILA_STREAM_WRITER_ERROR_LOCAL_NAME_NULL        (GUTHTHILA_START_ERROR + 4)
#define GUTHTHILA_STREAM_WRITER_ERROR_guththila_namespace_t_NULL        (GUTHTHILA_START_ERROR + 5)
#define GUTHTHILA_STREAM_WRITER_ERROR_PREFIX_NULL        (GUTHTHILA_START_ERROR + 6)
#define GUTHTHILA_STREAM_WRITER_ERROR_guththila_namespace_t_NOT_DECLARED        (GUTHTHILA_START_ERROR + 7)
#define GUTHTHILA_STREAM_WRITER_ERROR_guththila_element_t_guththila_stack_t_EMPTY        (GUTHTHILA_START_ERROR + 8)
#define GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_STATE        (GUTHTHILA_START_ERROR + 9)
#define GUTHTHILA_STREAM_WRITER_ERROR_COMMENT_NULL        (GUTHTHILA_START_ERROR + 10)
#define GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_COMMENT        (GUTHTHILA_START_ERROR + 11)
#define GUTHTHILA_STREAM_WRITER_ERROR_PROCESSING_INSTRUCTION_TARGET_NULL        (GUTHTHILA_START_ERROR + 12)
#define GUTHTHILA_STREAM_WRITER_ERROR_CDATA_NULL        (GUTHTHILA_START_ERROR + 13)
#define GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_CDATA        (GUTHTHILA_START_ERROR + 14)
#define GUTHTHILA_STREAM_WRITER_ERROR_DTD_NULL        (GUTHTHILA_START_ERROR + 15)
#define GUTHTHILA_STREAM_WRITER_ERROR_ENTITY_REF_NULL        (GUTHTHILA_START_ERROR + 16)
#define GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_XML_VERSION        (GUTHTHILA_START_ERROR + 17)
#define GUTHTHILA_STREAM_WRITER_ERROR_TEXT_NULL        (GUTHTHILA_START_ERROR + 18)
#define GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_PREFIX        (GUTHTHILA_START_ERROR + 19)
#define GUTHTHILA_STREAM_WRITER_ERROR_OUT_OF_MEMORY        (GUTHTHILA_START_ERROR + 20)

/*
 * Status Codes
 */
#define GUTHTHILA_SUCCESS        (GUTHTHILA_START_STATUS + 1)
#define GUTHTHILA_FAILURE       (APR_OS_START_STATUS + 2)

#endif /* GUTHTHILA_ERRNO_H */
