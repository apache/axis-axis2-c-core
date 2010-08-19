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

#include <axiom_xml_reader.h>
#include <axutil_env.h>
#include <axutil_utils.h>
#include <guththila.h>

/**************** function prototypes ******************************************/

int AXIS2_CALL
guththila_xml_reader_wrapper_next(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env);

void AXIS2_CALL
guththila_xml_reader_wrapper_free(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env);

int AXIS2_CALL
guththila_xml_reader_wrapper_get_attribute_count(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env);

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_attribute_name_by_number(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env,
    int i);

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_attribute_prefix_by_number(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env,
    int i);

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_attribute_value_by_number(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env,
    int i);

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_attribute_namespace_by_number(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env,
    int i);

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_value(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env);

int AXIS2_CALL
guththila_xml_reader_wrapper_get_namespace_count(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env);

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_namespace_uri_by_number(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env,
    int i);

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_namespace_prefix_by_number(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env,
    int i);

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_prefix(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env);

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_name(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env);

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_pi_target(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env);

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_pi_data(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env);

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_dtd(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env);

void AXIS2_CALL
guththila_xml_reader_wrapper_xml_free(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env,
    void *data);

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_char_set_encoding(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env);

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_namespace_uri(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env);

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_namespace_uri_by_prefix(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env,
    axis2_char_t * prefix);

void *AXIS2_CALL
guththila_xml_reader_wrapper_get_context(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env);

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_current_buffer(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env);

/*********** guththila_xml_reader_wrapper_impl_t wrapper struct   *******************/

typedef struct guththila_xml_reader_wrapper_impl
{
    axiom_xml_reader_t parser;
    guththila_t *guththila_parser;
    guththila_reader_t *reader;
    int event_map[10];
    void *context;

} guththila_xml_reader_wrapper_impl_t;

/********************************* Macro ***************************************/

#define AXIS2_INTF_TO_IMPL(p) ((guththila_xml_reader_wrapper_impl_t*)p)

#define AXIS2_IMPL_TO_INTF(p) &(p->parser)

/**********End macros  --  event_map initializing function *********************/

static void
guththila_xml_reader_wrapper_init_map(
    guththila_xml_reader_wrapper_impl_t * parser)
{
    parser->event_map[GUTHTHILA_START_DOCUMENT] = AXIOM_XML_READER_START_DOCUMENT;
    parser->event_map[GUTHTHILA_START_ELEMENT] = AXIOM_XML_READER_START_ELEMENT;
    parser->event_map[GUTHTHILA_END_ELEMENT] = AXIOM_XML_READER_END_ELEMENT;
    parser->event_map[GUTHTHILA_SPACE] = AXIOM_XML_READER_SPACE;
    parser->event_map[GUTHTHILA_EMPTY_ELEMENT] = AXIOM_XML_READER_EMPTY_ELEMENT;
    parser->event_map[GUTHTHILA_CHARACTER] = AXIOM_XML_READER_CHARACTER;
    parser->event_map[GUTHTHILA_ENTITY_REFERANCE] = AXIOM_XML_READER_ENTITY_REFERENCE;
    parser->event_map[GUTHTHILA_COMMENT] = AXIOM_XML_READER_COMMENT;
}

static const axiom_xml_reader_ops_t axiom_xml_reader_ops_var = {
    guththila_xml_reader_wrapper_next,
    guththila_xml_reader_wrapper_free,
    guththila_xml_reader_wrapper_get_attribute_count,
    guththila_xml_reader_wrapper_get_attribute_name_by_number,
    guththila_xml_reader_wrapper_get_attribute_prefix_by_number,
    guththila_xml_reader_wrapper_get_attribute_value_by_number,
    guththila_xml_reader_wrapper_get_attribute_namespace_by_number,
    guththila_xml_reader_wrapper_get_value,
    guththila_xml_reader_wrapper_get_namespace_count,
    guththila_xml_reader_wrapper_get_namespace_uri_by_number,
    guththila_xml_reader_wrapper_get_namespace_prefix_by_number,
    guththila_xml_reader_wrapper_get_prefix,
    guththila_xml_reader_wrapper_get_name,
    guththila_xml_reader_wrapper_get_pi_target,
    guththila_xml_reader_wrapper_get_pi_data,
    guththila_xml_reader_wrapper_get_dtd,
    guththila_xml_reader_wrapper_xml_free,
    guththila_xml_reader_wrapper_get_char_set_encoding,
    guththila_xml_reader_wrapper_get_namespace_uri,
    guththila_xml_reader_wrapper_get_namespace_uri_by_prefix,
    guththila_xml_reader_wrapper_get_context,
    guththila_xml_reader_wrapper_get_current_buffer};

/********************************************************************************/

static axiom_xml_reader_t *AXIS2_CALL
axiom_xml_reader_create_basic(
    const axutil_env_t *env,
    guththila_reader_t *reader,
    void *ctx)
{
    guththila_xml_reader_wrapper_impl_t *guththila_impl = NULL;
    guththila_t *guththila = NULL;

    if(!reader)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Invalid guththila reader");
        return NULL;
    }

    guththila_impl = AXIS2_MALLOC(env->allocator, sizeof(guththila_xml_reader_wrapper_impl_t));
    if(!guththila_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        guththila_reader_free(reader, env);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Insufficient memory to create xml reader wrapper");
        return NULL;
    }

    guththila = (guththila_t *)AXIS2_MALLOC(env->allocator, sizeof(guththila_t));
    if(!guththila)
    {
        AXIS2_FREE(env->allocator, guththila_impl);
        guththila_reader_free(reader, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Insufficient memory to create guththila");
        return NULL;
    }

    if(guththila_init(guththila, reader, env) != GUTHTHILA_SUCCESS)
    {
        AXIS2_FREE(env->allocator, guththila_impl);
        AXIS2_FREE(env->allocator, guththila);
        guththila_reader_free(reader, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_CREATING_XML_STREAM_READER, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Guththila initialization failed");
        return NULL;
    }

    guththila_impl->guththila_parser = guththila;
    guththila_impl->reader = reader;
    guththila_impl->context = ctx;
    guththila_xml_reader_wrapper_init_map(guththila_impl);
    guththila_impl->parser.ops = &axiom_xml_reader_ops_var;
    return &(guththila_impl->parser);
}

AXIS2_EXTERN axiom_xml_reader_t *AXIS2_CALL
axiom_xml_reader_create_for_file(
    const axutil_env_t * env,
    char *filename,
    const axis2_char_t * encoding)
{
    guththila_reader_t *reader = guththila_reader_create_for_file(filename, env);
    return axiom_xml_reader_create_basic(env, reader, NULL);
}

axiom_xml_reader_t *AXIS2_CALL
axiom_xml_reader_create_for_io(
    const axutil_env_t * env,
    AXIS2_READ_INPUT_CALLBACK read_input_callback,
    AXIS2_CLOSE_INPUT_CALLBACK close_input_callback,
    void *ctx,
    const char *encoding)
{
    guththila_reader_t *reader = guththila_reader_create_for_io(read_input_callback, ctx, env);
    return axiom_xml_reader_create_basic(env, reader, ctx);
}

AXIS2_EXTERN axiom_xml_reader_t *AXIS2_CALL
axiom_xml_reader_create_for_memory(
    const axutil_env_t * env,
    void *buffer,
    int size,
    const char *encoding,
    int type)
{
    guththila_reader_t *reader = guththila_reader_create_for_memory(buffer, size, env);
    return axiom_xml_reader_create_basic(env, reader, NULL);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_xml_reader_init()
{
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_xml_reader_cleanup()
{
    return AXIS2_SUCCESS;
}

int AXIS2_CALL
guththila_xml_reader_wrapper_next(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env)
{
    int i = -1;
    i = guththila_next(AXIS2_INTF_TO_IMPL(parser)->guththila_parser, env);
    return i == -1 ? -1 : AXIS2_INTF_TO_IMPL(parser)->event_map[i];
}

void AXIS2_CALL
guththila_xml_reader_wrapper_free(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env)
{
    guththila_xml_reader_wrapper_impl_t *parser_impl = NULL;
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    if(parser_impl->reader)
    {
        guththila_reader_free(parser_impl->reader, env);
    }

    if(parser_impl->guththila_parser)
    {
        guththila_un_init(parser_impl->guththila_parser, env);
    }

    AXIS2_FREE(env->allocator, parser_impl);
}

int AXIS2_CALL
guththila_xml_reader_wrapper_get_attribute_count(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env)
{
    return guththila_get_attribute_count(AXIS2_INTF_TO_IMPL(parser)-> guththila_parser, env);
}

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_attribute_name_by_number(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env,
    int i)
{
    return guththila_get_attribute_name_by_number(
        AXIS2_INTF_TO_IMPL(parser)-> guththila_parser, i, env);
}

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_attribute_prefix_by_number(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env,
    int i)
{
    return guththila_get_attribute_prefix_by_number(
        AXIS2_INTF_TO_IMPL(parser)-> guththila_parser, i, env);
}

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_attribute_value_by_number(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env,
    int i)
{
    return guththila_get_attribute_value_by_number(
        AXIS2_INTF_TO_IMPL(parser)-> guththila_parser, i, env);
}

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_attribute_namespace_by_number(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env,
    int i)
{
    return guththila_get_attribute_namespace_by_number(
        AXIS2_INTF_TO_IMPL(parser)-> guththila_parser, i, env);
}

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_value(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env)
{
    return guththila_get_value(AXIS2_INTF_TO_IMPL(parser)->guththila_parser, env);
}

int AXIS2_CALL
guththila_xml_reader_wrapper_get_namespace_count(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env)
{
    return guththila_get_namespace_count(AXIS2_INTF_TO_IMPL(parser)-> guththila_parser, env);
}

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_namespace_uri_by_number(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env,
    int i)
{
    return guththila_get_namespace_uri_by_number(
        AXIS2_INTF_TO_IMPL(parser)-> guththila_parser, i, env);
}

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_namespace_prefix_by_number(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env,
    int i)
{
    return guththila_get_namespace_prefix_by_number(
        AXIS2_INTF_TO_IMPL(parser)-> guththila_parser, i, env);
}

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_prefix(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env)
{
    return guththila_get_prefix(AXIS2_INTF_TO_IMPL(parser)->guththila_parser, env);
}

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_name(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env)
{
    return guththila_get_name(AXIS2_INTF_TO_IMPL(parser)->guththila_parser, env);
}

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_pi_target(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env)
{
    /* guththila_does not support pi's yet */
    return NULL;
}

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_pi_data(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env)
{
    /* guththila_dose not support yet */
    return NULL;
}

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_dtd(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env)
{
    printf("not implemented in guththila");
    return NULL;
}

void AXIS2_CALL
guththila_xml_reader_wrapper_xml_free(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env,
    void *data)
{
    if(data)
        AXIS2_FREE(env->allocator, data);
}

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_char_set_encoding(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env)
{
    return guththila_get_encoding(AXIS2_INTF_TO_IMPL(parser)->guththila_parser, env);
}

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_namespace_uri(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env)
{

    return (axis2_char_t *)NULL;
}

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_namespace_uri_by_prefix(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env,
    axis2_char_t * prefix)
{
    return (axis2_char_t *)NULL;
}

void *AXIS2_CALL
guththila_xml_reader_wrapper_get_context(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env)
{
    guththila_xml_reader_wrapper_impl_t* parser_impl = NULL;
    parser_impl = AXIS2_INTF_TO_IMPL(parser);
    return parser_impl->context;
}

axis2_char_t *AXIS2_CALL
guththila_xml_reader_wrapper_get_current_buffer(
    axiom_xml_reader_t * parser,
    const axutil_env_t * env)
{
    return guththila_get_current_buffer(AXIS2_INTF_TO_IMPL(parser)->guththila_parser, env);
}

