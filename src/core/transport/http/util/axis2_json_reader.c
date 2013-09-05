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

#include <axutil_stream.h>
#include <axiom_node.h>
#include <axiom_element.h>
#include <axiom_attribute.h>
#include <json.h>
#include "axis2_json_reader.h"

#define AXIS2_JSON_XSI_URI "http://www.w3.org/2001/XMLSchema-instance"

struct axis2_json_reader
{
    json_object* json_obj;
    axiom_node_t* axiom_node;
};

const char* json_tokener_error_to_str(enum json_tokener_error error)
{
    switch (error)
    {
    case json_tokener_success:
        return "success";
    case json_tokener_continue:
        return "continue";
    case json_tokener_error_depth:
        return "error_depth";
    case json_tokener_error_parse_eof:
        return "error_parse_eof";
    case json_tokener_error_parse_unexpected:
        return "error_parse_unexpected";
    case json_tokener_error_parse_null:
        return "error_parse_null";
    case json_tokener_error_parse_boolean:
        return "error_parse_boolean";
    case json_tokener_error_parse_number:
        return "error_parse_number";
    case json_tokener_error_parse_array:
        return "error_parse_array";
    case json_tokener_error_parse_object_key_name:
        return "error_parse_object_key_name";
    case json_tokener_error_parse_object_key_sep:
        return "error_parse_object_key_sep";
    case json_tokener_error_parse_object_value_sep:
        return "error_parse_object_value_sep";
    case json_tokener_error_parse_string:
        return "error_parse_string";
    case json_tokener_error_parse_comment:
        return "error_parse_comment";
    }
    return "UNKNOWN";
}

axis2_status_t
axis2_json_read_node(
        json_object* parent,
        const char* name,
        axiom_node_t** om_node,
        const axutil_env_t* env);


axis2_status_t
axis2_json_read_child_node(
        json_object* child_object,
        const char* child_name,
        axiom_node_t* om_node,
        const axutil_env_t* env)
{
    axiom_node_t* child_node = NULL;

    switch (json_object_get_type(child_object))
    {
    case json_type_object:
    {
        if (axis2_json_read_node(child_object, child_name, &child_node, env) != AXIS2_SUCCESS)
            return AXIS2_FAILURE;
        if (axiom_node_add_child(om_node, env, child_node) != AXIS2_SUCCESS)
            return AXIS2_FAILURE;
        break;
    }

    case json_type_array:
    {
        int i;
        int array_len = json_object_array_length(child_object);
        for (i = 0; i < array_len; ++i)
        {
            if (axis2_json_read_child_node(json_object_array_get_idx(child_object, i),
                                       child_name, om_node, env) != AXIS2_SUCCESS)
                return AXIS2_FAILURE;
        }
        break;
    }

    case json_type_int:
    case json_type_boolean:
    case json_type_double:
    case json_type_string:
    {
        axiom_node_t* om_child_node = NULL;
        axiom_node_t* om_text_node = NULL;

        if (!axiom_element_create(env, NULL, child_name, NULL, &om_child_node))
            return AXIS2_FAILURE;

        if (!axiom_text_create(env, om_child_node, json_object_get_string(child_object),
                          &om_text_node))
                return AXIS2_FAILURE;

        if (axiom_node_add_child(om_node, env, om_child_node) != AXIS2_SUCCESS)
        {
            axiom_node_free_tree(om_text_node, env);
            return AXIS2_FAILURE;
        }

        break;
    }

    case json_type_null:
    {
        axiom_element_t* om_child_elem = NULL;
        axiom_node_t* om_child_node = NULL;

        /* handle as nillable */
        axiom_attribute_t* attr;
        axiom_namespace_t* ns =
                axiom_namespace_create(env, AXIS2_JSON_XSI_URI, "xsi");

        if (!ns)
            return AXIS2_FAILURE;

        om_child_elem = axiom_element_create(env, NULL, child_name, NULL, &om_child_node);
        if (!om_child_elem)
        {
            axiom_namespace_free(ns, env);
            return AXIS2_FAILURE;
        }

        if (axiom_node_add_child(om_node, env, om_child_node) != AXIS2_SUCCESS)
        {
            axiom_namespace_free(ns, env);
            axiom_node_free_tree(om_child_node, env);
            return AXIS2_FAILURE;
        }

        attr = axiom_attribute_create(env, "nil", "true", ns);
        if (!attr)
        {
            axiom_attribute_free(attr, env);
            return AXIS2_FAILURE;
        }

        if (axiom_element_add_attribute(om_child_elem, env, attr,
                                        om_child_node) != AXIS2_SUCCESS)
        {
            axiom_attribute_free(attr, env);
            return AXIS2_FAILURE;
        }

        break;
    }
    }

    return AXIS2_SUCCESS;
}

axis2_status_t
axis2_json_read_node(
        json_object* parent,
        const char* name,
        axiom_node_t** om_node,
        const axutil_env_t* env)
{
    if (!json_object_is_type(parent, json_type_object))
        return AXIS2_FAILURE;

    axiom_element_create(env, NULL, name, NULL, om_node);

    json_object_object_foreach(parent, child_name, child_object)
    {
        if (axis2_json_read_child_node(child_object, child_name, *om_node, env) != AXIS2_SUCCESS)
            return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_json_reader_t* AXIS2_CALL
axis2_json_reader_create_for_stream(
        const axutil_env_t* env,
        axutil_stream_t* stream)
{
    axis2_json_reader_t* reader =
            (axis2_json_reader_t*)AXIS2_MALLOC(env->allocator,
                                               sizeof(struct axis2_json_reader));
    if (reader)
    {
        axis2_char_t buffer[512];
        int readed;
        struct json_tokener* tokener = json_tokener_new();
        enum json_tokener_error error;
        json_object* json_obj = NULL;

        reader->json_obj = NULL;
        reader->axiom_node = NULL;
        do
        {
            readed = axutil_stream_read(stream, env, &buffer, sizeof(buffer));
            if (readed < 0)
                break;

            json_obj = json_tokener_parse_ex(tokener, buffer, readed);

        } while ((error = json_tokener_get_error(tokener)) == json_tokener_continue);

        if (error != json_tokener_success)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_STATE_PARAM, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to parse JSON request: %s",
                            json_tokener_error_to_str(tokener->err));
            if (json_obj)
                json_object_put(json_obj);
            json_tokener_free(tokener);
            free(reader);
            return NULL;
        }

        reader->json_obj = json_obj;

        json_tokener_free(tokener);
    }

    return reader;
}


AXIS2_EXTERN axis2_json_reader_t* AXIS2_CALL
axis2_json_reader_create_for_memory(
        const axutil_env_t* env,
        const axis2_char_t* json_string,
        int json_string_size)
{
    axis2_json_reader_t* reader =
            (axis2_json_reader_t*)AXIS2_MALLOC(env->allocator,
                                               sizeof(struct axis2_json_reader));
    if (reader)
    {
        struct json_tokener* tokener = json_tokener_new();

        reader->axiom_node = NULL;
        reader->json_obj = json_tokener_parse_ex(tokener, json_string, json_string_size);
        if (tokener->err != json_tokener_success)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_STATE_PARAM, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to parse JSON request: %s",
                            json_tokener_error_to_str(tokener->err));
            if (reader->json_obj)
                json_object_put(reader->json_obj);
            json_tokener_free(tokener);
            free(reader);
            return NULL;
        }
        json_tokener_free(tokener);
    }

    return reader;
}


AXIS2_EXTERN void AXIS2_CALL
axis2_json_reader_free(
        axis2_json_reader_t* reader,
        const axutil_env_t* env)
{
    if (reader->json_obj)
        json_object_put(reader->json_obj);
    AXIS2_FREE(env->allocator, reader);
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_json_reader_read(
        axis2_json_reader_t* reader,
        const axutil_env_t* env)
{
    json_object* json_root = NULL;
    const char* json_root_name = NULL;

    /* free existing om tree */
    if (reader->axiom_node)
        axiom_node_free_tree(reader->axiom_node, env);
    reader->axiom_node = NULL;

    /* get first child */
    json_object_object_foreach(reader->json_obj, key, value)
    {
        json_root = value;
        json_root_name = key;
        break;
    }

    if (!json_root || !json_root_name)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_STATE_PARAM, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed find root JSON node");
        return AXIS2_FAILURE;
    }

    return axis2_json_read_node(json_root, json_root_name, &reader->axiom_node, env);
}


AXIS2_EXTERN axiom_node_t* AXIS2_CALL
axis2_json_reader_get_root_node(
        axis2_json_reader_t* reader,
        const axutil_env_t* env)
{
    (void)env;
    return reader->axiom_node;
}
