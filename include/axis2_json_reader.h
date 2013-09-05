/*
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef AXIS2_JSON_READER_H
#define AXIS2_JSON_READER_H

#include <axutil_utils.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axutil_stream axutil_stream_t;
typedef struct axiom_node axiom_node_t;
typedef struct axutil_env axutil_env_t;
typedef struct axis2_json_reader axis2_json_reader_t;

/**
 * @brief Creates JSON reader to read data from stream
 * @param env Environment
 * @param stream Stream to read data from
 */
AXIS2_EXTERN axis2_json_reader_t* AXIS2_CALL
axis2_json_reader_create_for_stream(
        const axutil_env_t* env,
        axutil_stream_t* stream);


/**
 * @brief Creates JSON reader to read data from string
 * @param env Environment
 * @param json_string JSON string
 * @param json_string_size JSON string
 */
AXIS2_EXTERN axis2_json_reader_t* AXIS2_CALL
axis2_json_reader_create_for_memory(
        const axutil_env_t* env,
        const axis2_char_t* json_string,
        int json_string_size);


/**
 * @brief Destroys reader
 *  does not free root node.
 *  user must free axiom tree using axiom_node_free_tree()
 * @param reader JSON reader
 * @param env Environment
 */
AXIS2_EXTERN void AXIS2_CALL
axis2_json_reader_free(
        axis2_json_reader_t* reader,
        const axutil_env_t* env);


/**
 * @brief Starts reading from string or stream
 * @param reader JSON reader
 * @param env Environment
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_json_reader_read(
        axis2_json_reader_t* reader,
        const axutil_env_t* env);


/**
 * @brief Gets root node of parsed data
 * @param reader JSON reader
 * @param env Environment
 */
AXIS2_EXTERN axiom_node_t* AXIS2_CALL
axis2_json_reader_get_root_node(
        axis2_json_reader_t* reader,
        const axutil_env_t* env);

#ifdef __cplusplus
}
#endif

#endif
/*
 *  Copyright 2013 Utkin Dmitry
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef AXIS2_JSON_READER_H
#define AXIS2_JSON_READER_H

#include <axutil_utils_defines.h>
#include <axutil_stream.h>
#include <axiom_node.h>
#include <axutil_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_json_reader axis2_json_reader_t;

/**
 * @brief Creates JSON reader to read data from stream
 * @param env Environment
 * @param stream Stream to read data from
 */
AXIS2_EXTERN axis2_json_reader_t* AXIS2_CALL
axis2_json_reader_create_for_stream(
        const axutil_env_t* env,
        axutil_stream_t* stream);


/**
 * @brief Creates JSON reader to read data from string
 * @param env Environment
 * @param json_string JSON string
 * @param json_string_size JSON string
 */
AXIS2_EXTERN axis2_json_reader_t* AXIS2_CALL
axis2_json_reader_create_for_memory(
        const axutil_env_t* env,
        const axis2_char_t* json_string,
        int json_string_size);


/**
 * @brief Destroys reader
 *  does not free root node.
 *  user must free axiom tree using axiom_node_free_tree()
 * @param reader JSON reader
 * @param env Environment
 */
AXIS2_EXTERN void AXIS2_CALL
axis2_json_reader_free(
        axis2_json_reader_t* reader,
        const axutil_env_t* env);


/**
 * @brief Starts reading from string or stream
 * @param reader JSON reader
 * @param env Environment
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_json_reader_read(
        axis2_json_reader_t* reader,
        const axutil_env_t* env);


/**
 * @brief Gets root node of parsed data
 * @param reader JSON reader
 * @param env Environment
 */
AXIS2_EXTERN axiom_node_t* AXIS2_CALL
axis2_json_reader_get_root_node(
        axis2_json_reader_t* reader,
        const axutil_env_t* env);

#ifdef __cplusplus
}
#endif

#endif
