/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <axiom_output.h>
#include <stdarg.h>
#include <axis2_string.h>
#include <axiom_xml_writer.h>
#include <axiom_text.h>
#include <axiom_soap_const.h>
#include <axis2_array_list.h>
#include <axis2_uuid_gen.h>
#include <axiom_mime_output.h>

#define AXIS2_DEFAULT_CHAR_SET_ENCODING  "UTF-8"
/** also defined in axiom_soap.h */

/** max args for om_output_write function */
#define MAX_ARGS  4


struct axiom_output
{
    /** axiom_xml_writer. any xml writer which
        implemet axiom_xml_writer.h interface  */
    axiom_xml_writer_t *xml_writer;

    axis2_bool_t do_optimize;

    axis2_char_t *mime_boundary;

    axis2_char_t *root_content_id;

    int next_id;

    axis2_char_t *next_content_id;

    axis2_bool_t is_soap11;

    axis2_char_t *char_set_encoding;

    axis2_char_t *xml_version;

    axis2_bool_t ignore_xml_declaration;

    axis2_array_list_t *binary_node_list;

    axiom_mime_output_t *mime_output;

    axis2_char_t *mime_boundry;

};

AXIS2_EXTERN axiom_output_t *AXIS2_CALL
axiom_output_create(const axis2_env_t *env,
    axiom_xml_writer_t *xml_writer)
{
    axiom_output_t *om_output = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    om_output = (axiom_output_t *) AXIS2_MALLOC(
        env->allocator,
        sizeof(axiom_output_t));

    if (!om_output)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    om_output->xml_writer = NULL;
    om_output->xml_writer = xml_writer;
    om_output->do_optimize = AXIS2_FALSE;
    om_output->mime_boundary = NULL;
    om_output->root_content_id = NULL;
    om_output->next_content_id = NULL;
    om_output->next_id = 0;
    om_output->is_soap11 = AXIS2_TRUE;
    om_output->char_set_encoding = AXIS2_DEFAULT_CHAR_SET_ENCODING;
    om_output->xml_version = NULL;
    om_output->ignore_xml_declaration = AXIS2_TRUE;
    om_output->binary_node_list = NULL;
    om_output->mime_output = NULL;
    om_output->mime_boundry = NULL;

    return om_output;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_output_free(axiom_output_t *om_output,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (om_output->xml_version)
    {
        AXIS2_FREE(env->allocator, om_output->xml_version);
    }
    if (om_output->mime_boundary)
    {
        AXIS2_FREE(env->allocator, om_output->mime_boundary);
    }
    if (om_output->next_content_id)
    {
        AXIS2_FREE(env->allocator, om_output->next_content_id);
    }
    if (om_output->root_content_id)
    {
        AXIS2_FREE(env->allocator, om_output->root_content_id);
    }

    if (om_output->xml_writer)
    {
        AXIOM_XML_WRITER_FREE(om_output->xml_writer, env);
    }

    if (om_output->binary_node_list)
    {
        axis2_array_list_free(om_output->binary_node_list, env);
    }

    if (om_output->mime_output)
    {
        AXIOM_MIME_OUTPUT_FREE(om_output->mime_output, env);
    }

    AXIS2_FREE(env->allocator, om_output);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axiom_output_is_soap11(axiom_output_t *om_output,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return om_output->is_soap11;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axiom_output_is_ignore_xml_declaration(axiom_output_t *om_output,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return om_output->ignore_xml_declaration;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_output_set_ignore_xml_declaration(axiom_output_t *om_output,
    const axis2_env_t *env,
    axis2_bool_t ignore_xml_dec)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output->ignore_xml_declaration = ignore_xml_dec;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_output_set_soap11(axiom_output_t *om_output,
    const axis2_env_t *env,
    axis2_bool_t soap11)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output->is_soap11 = soap11;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_output_set_xml_version(axiom_output_t *om_output,
    const axis2_env_t *env,
    axis2_char_t *xml_version)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    AXIS2_PARAM_CHECK(env->error, xml_version, AXIS2_FAILURE);

    if (om_output->xml_version)
    {
        AXIS2_FREE(env->allocator,  om_output->xml_version);
        om_output->xml_version = NULL;
    }

    om_output->xml_version = axis2_strdup(xml_version, env);
    if (!om_output->xml_version)
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axiom_output_get_xml_version(axiom_output_t *om_output,
    const axis2_env_t *env)
{
    return om_output->xml_version;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_output_set_char_set_encoding(axiom_output_t *om_output,
    const axis2_env_t *env,
    axis2_char_t *char_set_encoding)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output->char_set_encoding = char_set_encoding;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axiom_output_get_char_set_encoding(axiom_output_t *om_output,
    const axis2_env_t *env)
{
    return om_output->char_set_encoding;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_output_set_do_optimize(axiom_output_t *om_output,
    const axis2_env_t *env,
    axis2_bool_t optimize)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    om_output->do_optimize = optimize;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axiom_xml_writer_t *AXIS2_CALL
axiom_output_get_xml_writer(axiom_output_t *om_output,
    const axis2_env_t *env)
{
    return om_output->xml_writer;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axiom_output_is_optimized(axiom_output_t *om_output,
    const axis2_env_t *env)
{
    return om_output->do_optimize;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axiom_output_get_content_type(axiom_output_t *om_output,
    const axis2_env_t *env)
{
    const axis2_char_t *soap_content_type = NULL;
    if (AXIS2_TRUE == om_output->do_optimize)
    {
        if (AXIS2_TRUE == om_output->is_soap11)
        {
            soap_content_type = AXIOM_SOAP11_CONTENT_TYPE;
        }
        else
        {
            soap_content_type = AXIOM_SOAP12_CONTENT_TYPE;
        }

        return axiom_mime_output_get_content_type_for_mime(om_output->mime_output,
            env, om_output->mime_boundry,
            om_output->root_content_id, om_output->char_set_encoding,
            soap_content_type);
    }
    else
    {
        if (AXIS2_TRUE == om_output->is_soap11)
        {
            return AXIOM_SOAP11_CONTENT_TYPE;
        }
        else
        {
            return AXIOM_SOAP12_CONTENT_TYPE;
        }
    }
    return NULL;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_output_write_optimized(axiom_output_t *om_output,
    const axis2_env_t *env,
    axiom_text_t *om_text)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (om_output->binary_node_list)
    {
        axis2_array_list_add(om_output->binary_node_list, env, om_text);
    }
    else
    {
        om_output->binary_node_list = axis2_array_list_create(env, 5);
        if (!(om_output->binary_node_list))
        {
            return AXIS2_FAILURE;
        }
        axis2_array_list_add(om_output->binary_node_list, env, om_text);
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axiom_output_get_next_content_id(axiom_output_t *om_output,
    const axis2_env_t *env)
{
    axis2_char_t *uuid = NULL;
    axis2_char_t *temp_str = NULL;
    axis2_char_t *temp_str1 = NULL;
    axis2_char_t id[256];
    om_output->next_id++;

    /** free existing id */
    if (om_output->next_content_id)
    {
        AXIS2_FREE(env->allocator, om_output->next_content_id);
        om_output->next_content_id = NULL;
    }

    uuid = axis2_uuid_gen(env);
    if (!uuid)
    {
        return NULL;
    }

    sprintf(id, "%d", om_output->next_id);

    temp_str = axis2_stracat(id, ".", env);
    temp_str1 = axis2_stracat(temp_str, uuid, env);
    om_output->next_content_id = axis2_stracat(temp_str1, "@apache.org", env);
    if (temp_str)
    {
        AXIS2_FREE(env->allocator, temp_str);
        temp_str = NULL;
    }
    if (temp_str1)
    {
        AXIS2_FREE(env->allocator, temp_str1);
        temp_str1 = NULL;
    }
    if (uuid)
    {
        AXIS2_FREE(env->allocator, uuid);
        uuid = NULL;
    }
    return om_output->next_content_id;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axiom_output_get_root_content_id(axiom_output_t *om_output,
    const axis2_env_t *env)
{
    axis2_char_t *temp_str = NULL;
    axis2_char_t *uuid = NULL;
    
    if (!om_output->root_content_id)
    {
        uuid = axis2_uuid_gen(env);

        temp_str = axis2_stracat("0.", uuid, env);

        om_output->root_content_id =
            axis2_stracat(temp_str, "@apache.org", env);

        if (temp_str)
        {
            AXIS2_FREE(env->allocator, temp_str);
            temp_str = NULL;
        }
        if (uuid)
        {
            AXIS2_FREE(env->allocator, uuid);
            uuid = NULL;
        }
    }

    return om_output->root_content_id;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axiom_output_get_mime_boundry(axiom_output_t *om_output,
    const axis2_env_t *env)
{
    axis2_char_t *uuid = NULL;
    if (!om_output->mime_boundary)
    {
        uuid = axis2_uuid_gen(env);

        om_output->mime_boundary = axis2_stracat("MIMEBoundary", uuid, env);
        if (uuid)
        {
            AXIS2_FREE(env->allocator, uuid);
            uuid = NULL;
        }
    }
    return om_output->mime_boundary;
}

/******************************************************************************/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_output_write(axiom_output_t * om_output,
    const axis2_env_t *env,
    axiom_types_t type,
    int no_of_args, ...)
{
    int status = AXIS2_SUCCESS;
    axis2_char_t *args_list[MAX_ARGS];
    int i = 0;
    va_list ap;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    va_start(ap, no_of_args);
    for (i = 0; i < no_of_args; i++)
    {
        args_list[i] = va_arg(ap, axis2_char_t *);
    }
    va_end(ap);


    if (type == AXIOM_ELEMENT)
    {
        if (no_of_args == 0)
        {
            status =
                AXIOM_XML_WRITER_WRITE_END_ELEMENT(om_output->xml_writer,
                    env);
        }
        else if (no_of_args == 1)
        {
            status =
                AXIOM_XML_WRITER_WRITE_START_ELEMENT(om_output->xml_writer,
                    env, args_list[0]);
        }
        else if (no_of_args == 2)
        {
            status =
                AXIOM_XML_WRITER_WRITE_START_ELEMENT_WITH_NAMESPACE(
                    om_output->xml_writer,env,
                    args_list[0],
                    args_list[1]);
        }
        else if (no_of_args == 3)
        {
            status =
                AXIOM_XML_WRITER_WRITE_START_ELEMENT_WITH_NAMESPACE_PREFIX(
                    om_output->xml_writer,
                    env,
                    args_list[0],
                    args_list[1],
                    args_list[2]);
        }
    }
    else if (type == AXIOM_DATA_SOURCE)
    {
        status = AXIOM_XML_WRITER_WRITE_RAW(om_output->xml_writer,
            env, args_list[0]); 
    }
    else if (type == AXIOM_ATTRIBUTE)
    {
        if (no_of_args == 2)
        {
            status =
                AXIOM_XML_WRITER_WRITE_ATTRIBUTE(om_output->xml_writer,
                    env,
                    args_list[0],
                    args_list[1]);
        }
        else if (no_of_args == 3)
        {
            status = AXIOM_XML_WRITER_WRITE_ATTRIBUTE_WITH_NAMESPACE(
                om_output->xml_writer, env,
                args_list[0],
                args_list[1],
                args_list[2]);
        }
        else if (no_of_args == 4)
        {
            status =
                AXIOM_XML_WRITER_WRITE_ATTRIBUTE_WITH_NAMESPACE_PREFIX(
                    om_output->xml_writer, env,
                    args_list[0],
                    args_list[1],
                    args_list[2],
                    args_list[3]);
        }
    }
    else if (type == AXIOM_NAMESPACE)
    {
        status = AXIOM_XML_WRITER_WRITE_NAMESPACE(om_output->xml_writer,
            env,
            args_list[0],
            args_list[1]);
    }
    else if (type == AXIOM_TEXT)
    {
        status = AXIOM_XML_WRITER_WRITE_CHARACTERS(om_output->xml_writer,
            env,
            args_list[0]);
    }
    else if (type == AXIOM_COMMENT)
    {
        status = AXIOM_XML_WRITER_WRITE_COMMENT(om_output->xml_writer,
            env,
            args_list[0]);
    }
    else if (type == AXIOM_PROCESSING_INSTRUCTION)
    {
        if (no_of_args == 1)
        {
            status =
                AXIOM_XML_WRITER_WRITE_PROCESSING_INSTRUCTION(
                    om_output->xml_writer,
                    env, args_list[0]);
        }
        else if (no_of_args == 2)
        {
            status = AXIOM_XML_WRITER_WRITE_PROCESSING_INSTRUCTION_DATA(
                om_output->xml_writer,
                env,
                args_list[0],
                args_list[1]);
        }
    }
    else if (type == AXIOM_DOCTYPE)
    {
        status = AXIOM_XML_WRITER_WRITE_DTD(om_output->xml_writer,
            env,
            args_list[0]);
    }

    if (status == AXIS2_SUCCESS)
    {
        return AXIS2_SUCCESS;
    }
    else
        return AXIS2_FAILURE;
}

axis2_status_t AXIS2_CALL
axiom_output_write_xml_version_encoding(axiom_output_t *om_output,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIOM_XML_WRITER_WRITE_START_DOCUMENT_WITH_VERSION_ENCODING(     om_output->xml_writer,
        env,
        om_output->xml_version,
        om_output->char_set_encoding);

}

axis2_byte_t *AXIS2_CALL
axiom_output_flush(axiom_output_t *om_output,
    const axis2_env_t *env,
    axis2_byte_t **output_stream,
    int *output_stream_size)
{
    const axis2_char_t *soap_content_type = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (om_output->do_optimize)
    {
        axis2_byte_t* byte_stream = NULL;
        axis2_char_t *root_content_id = NULL;
        axis2_char_t *buffer =
            (axis2_char_t*)AXIOM_XML_WRITER_GET_XML(om_output->xml_writer, env);
        int stream_size = 0;
        if (om_output->is_soap11)
        {
            soap_content_type = AXIOM_SOAP11_CONTENT_TYPE;
        }
        else
        {
            soap_content_type = AXIOM_SOAP12_CONTENT_TYPE;
        }
        om_output->mime_output = axiom_mime_output_create(env);
        om_output->mime_boundry = axiom_output_get_mime_boundry(om_output, env);
        root_content_id = axiom_output_get_root_content_id(om_output, env);
        AXIOM_MIME_OUTPUT_COMPLETE(om_output->mime_output,
            env, &byte_stream, &stream_size,
            buffer, om_output->binary_node_list, om_output->mime_boundry,
            om_output->root_content_id, om_output->char_set_encoding,
            soap_content_type);

        *output_stream = byte_stream;
        *output_stream_size = stream_size;

        return byte_stream;
    }
    return NULL;
}

