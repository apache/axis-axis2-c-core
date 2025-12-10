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

/* Revolutionary HTTP/2 JSON Architecture - AXIOM-free stub implementation */
#include <json-c/json.h>
#include <string.h>
#include "axis2_json_writer.h"

struct axis2_json_writer
{
    json_object* json_obj;
};

/* Revolutionary AXIOM-free stub implementation */
AXIS2_EXTERN axis2_json_writer_t* AXIS2_CALL
axis2_json_writer_create(const axutil_env_t* env)
{
    /* Revolutionary: Stub implementation - actual JSON processing handled by JsonRpcMessageReceiver */
    axis2_json_writer_t* writer = (axis2_json_writer_t*)AXIS2_MALLOC(env->allocator,
                                                                     sizeof(struct axis2_json_writer));
    if (writer) {
        writer->json_obj = json_object_new_object();
    }
    return writer;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_json_writer_free(
        axis2_json_writer_t* writer,
        const axutil_env_t* env)
{
    /* Revolutionary: Minimal cleanup */
    if (writer) {
        if (writer->json_obj) {
            json_object_put(writer->json_obj);
        }
        AXIS2_FREE(env->allocator, writer);
    }
}

AXIS2_EXTERN void AXIS2_CALL
axis2_json_writer_write(
        axis2_json_writer_t* writer,
        const axiom_node_t* node,
        const axutil_env_t* env)
{
    /* Revolutionary: Stub - actual JSON processing done by JsonRpcMessageReceiver */
    /* Compatibility shim for existing tests */
    return;
}

AXIS2_EXTERN const axis2_char_t* AXIS2_CALL
axis2_json_writer_get_json_string(
        axis2_json_writer_t* writer,
        const axutil_env_t* env,
        int* json_string_length)
{
    /* Revolutionary: Return empty JSON object */
    if (writer && writer->json_obj) {
        const char* json_str = json_object_to_json_string(writer->json_obj);
        if (json_string_length) {
            *json_string_length = (int)strlen(json_str);
        }
        return json_str;
    }
    if (json_string_length) {
        *json_string_length = 2;
    }
    return "{}";
}