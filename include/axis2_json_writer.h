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

#ifndef AXIS2_JSON_WRITER_H
#define AXIS2_JSON_WRITER_H

#include <axutil_utils.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axiom_node axiom_node_t;
typedef struct axutil_env axutil_env_t;
typedef struct axis2_json_writer axis2_json_writer_t;

/**
 * @brief Creates JSON writer
 * @param env Environment
 */
AXIS2_EXTERN axis2_json_writer_t* AXIS2_CALL
axis2_json_writer_create(
        const axutil_env_t* env);


/**
 * @brief Destroys writer
 * @param writer JSON writer
 * @param env Environment
 */
AXIS2_EXTERN void AXIS2_CALL
axis2_json_writer_free(
        axis2_json_writer_t* writer,
        const axutil_env_t* env);


/**
 * @brief Starts writing to JSON
 * @param writer JSON writer
 * @param node Source OM node
 * @param env Environment
 */
AXIS2_EXTERN void AXIS2_CALL
axis2_json_writer_write(
        axis2_json_writer_t* writer,
        const axiom_node_t* node,
        const axutil_env_t* env);


/**
 * @brief Gets resulting JSON string
 * @param writer JSON writer
 * @param env Environment
 */
AXIS2_EXTERN const axis2_char_t* AXIS2_CALL
axis2_json_writer_get_json_string(
        axis2_json_writer_t* writer,
        const axutil_env_t* env,
        int* json_string_length);

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

#ifndef AXIS2_JSON_WRITER_H
#define AXIS2_JSON_WRITER_H

#include <axutil_utils_defines.h>
#include <axiom_node.h>
#include <axutil_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_json_writer axis2_json_writer_t;

/**
 * @brief Creates JSON writer
 * @param env Environment
 */
AXIS2_EXTERN axis2_json_writer_t* AXIS2_CALL
axis2_json_writer_create(
        const axutil_env_t* env);


/**
 * @brief Destroys writer
 * @param writer JSON writer
 * @param env Environment
 */
AXIS2_EXTERN void AXIS2_CALL
axis2_json_writer_free(
        axis2_json_writer_t* writer,
        const axutil_env_t* env);


/**
 * @brief Starts writing to JSON
 * @param writer JSON writer
 * @param node Source OM node
 * @param env Environment
 */
AXIS2_EXTERN void AXIS2_CALL
axis2_json_writer_write(
        axis2_json_writer_t* writer,
        const axiom_node_t* node,
        const axutil_env_t* env);


/**
 * @brief Gets resulting JSON string
 * @param writer JSON writer
 * @param env Environment
 */
AXIS2_EXTERN const axis2_char_t* AXIS2_CALL
axis2_json_writer_get_json_string(
        axis2_json_writer_t* writer,
        const axutil_env_t* env,
        int* json_string_length);

#ifdef __cplusplus
}
#endif

#endif
