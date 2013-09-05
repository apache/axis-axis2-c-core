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

#include <axiom_node.h>
#include <axiom_element.h>
#include <axiom_attribute.h>
#include <json.h>
#include "axis2_json_writer.h"

#define AXIS2_JSON_XSI_URI "http://www.w3.org/2001/XMLSchema-instance"

struct axis2_json_writer
{
    json_object* json_obj;
};

axis2_bool_t axis2_json_element_is_nil(axiom_element_t* om_element, const axutil_env_t* env)
{
    axiom_attribute_t* attr = NULL;
    axutil_hash_index_t* index;
    axutil_hash_t* attr_hash = axiom_element_get_all_attributes(om_element, env);

    if (!attr_hash)
        return AXIS2_FALSE;

    for (index = axutil_hash_first(attr_hash, env);
         index; index = axutil_hash_next(env, index))
    {
        axutil_hash_this(index, NULL, NULL, (void**)&attr);
        if (attr && !strcmp(axiom_attribute_get_localname(attr, env), "nil"))
        {
            /* found some "nil" attribute, check it namespace */
            axutil_qname_t* qname =
                    axiom_attribute_get_qname(attr, env);
            if (qname && !strcmp(axutil_qname_get_uri(qname, env), AXIS2_JSON_XSI_URI))
            {
                axis2_char_t* attr_value =
                        axiom_attribute_get_value(attr, env);
                return (!strcmp(attr_value, "true") || !strcmp(attr_value, "1")) ?
                            AXIS2_TRUE : AXIS2_FALSE;
            }
        }
    }

    return AXIS2_FALSE;
}

void axis2_json_write_node(json_object* parent, axiom_node_t* om_node, const axutil_env_t* env)
{
    axiom_element_t* elem;
    axiom_node_t* child_node;
    const axis2_char_t* local_name;
    json_object* obj;
    json_object* array = NULL;

    if (!om_node || axiom_node_get_node_type(om_node, env) != AXIOM_ELEMENT)
        return;

    elem = (axiom_element_t*)axiom_node_get_data_element(om_node, env);
    local_name = axiom_element_get_localname(elem, env);

    child_node = axiom_node_get_first_element(om_node, env);

    /* find existing object */
    if (json_object_object_get_ex(parent, local_name, &obj))
    {
        /* check that object is array? */
        if (!json_object_is_type(obj, json_type_array))
        {
            /* convert to array */
            obj = json_object_get(obj);
            array = json_object_new_array();
            json_object_array_add(array, obj);
            json_object_object_del(parent, local_name);
            json_object_object_add(parent, local_name, array);
        }
        else
            array = obj;
    }

    if (!child_node)
    {
        /* this is a leaf node */
        json_object* json_value = NULL;

        /* check for nillable */
        if (!axis2_json_element_is_nil(elem, env))
        {
            const axis2_char_t* value =
                    axiom_element_get_text(elem, env, om_node);
            json_value = json_object_new_string(value ? value : "");
        }

        if (array)
            json_object_array_add(array, json_value);
        else
            json_object_object_add(parent, local_name, json_value);
        return;
    }

    /* iterate through children elements */
    obj = json_object_new_object();
    if (array)
        json_object_array_add(array, obj);
    else
        json_object_object_add(parent, local_name, obj);

    for (; child_node; child_node = axiom_node_get_next_sibling(child_node, env))
        if (axiom_node_get_node_type(child_node, env) == AXIOM_ELEMENT)
            axis2_json_write_node(obj, child_node, env);
}


AXIS2_EXTERN axis2_json_writer_t* AXIS2_CALL
axis2_json_writer_create(
        const axutil_env_t* env)
{
    axis2_json_writer_t* writer = (axis2_json_writer_t*)AXIS2_MALLOC(env->allocator,
                                                                     sizeof(struct axis2_json_writer));
    if (writer)
        writer->json_obj = NULL;

    return writer;
}


AXIS2_EXTERN void AXIS2_CALL
axis2_json_writer_free(
        axis2_json_writer_t* writer,
        const axutil_env_t* env)
{
    json_object_put(writer->json_obj);
    AXIS2_FREE(env->allocator, writer);
}


AXIS2_EXTERN void AXIS2_CALL
axis2_json_writer_write(
        axis2_json_writer_t* writer,
        const axiom_node_t* node,
        const axutil_env_t* env)
{
    /* free existing object */
    if (writer->json_obj)
        json_object_put(writer->json_obj);

    writer->json_obj = json_object_new_object();
    axis2_json_write_node(writer->json_obj, (axiom_node_t*)node, env);
}


AXIS2_EXTERN const axis2_char_t* AXIS2_CALL
axis2_json_writer_get_json_string(
        axis2_json_writer_t* writer,
        const axutil_env_t* env,
        int* json_string_length)
{
    (void)env;
    const axis2_char_t* result =
            (const axis2_char_t*)json_object_to_json_string_ext(writer->json_obj,
                                                               JSON_C_TO_STRING_PLAIN);
    if (json_string_length)
        *json_string_length = strlen(result);

    return result;
}

