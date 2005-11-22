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

#include "guththila_xml_stream_writer.h"
#include "guththila_array.h"
#include "guththila_hash.h"
#include "guththila_string.h"


struct guththila_xml_stream_writer
{
    FILE *writer;
    guththila_char_t *encoding;
    int prefix_defaulting;
    guththila_array_header_t *element_stack;    /*stack of element names (apr array is FILO) */
    guththila_hash_t *namespace_hash;   /*hash map with key:namespace and value:prefix */
    guththila_hash_t *prefix_hash;  /*hash map with key:prefix and value:namespace */
    int in_start_element;
    int empty_element;
};

GUTHTHILA_DECLARE (guththila_xml_stream_writer_t *)
guththila_create_xml_stream_writer (guththila_environment_t * environment,
                                    guththila_char_t * filename,
                                    guththila_char_t * encoding,
                                    int prefix_defaulting)
{
    /*   guththila_status_t status; */
    guththila_xml_stream_writer_t *stream_writer;
    FILE *fp = NULL;

    stream_writer =
        (guththila_xml_stream_writer_t *) GUTHTHILA_MALLOC (environment->allocator,
                                          sizeof(guththila_xml_stream_writer_t));
    if(filename != NULL)
    {
        fp = fopen(filename, "w");
        if(!fp)
            return GUTHTHILA_STREAM_WRITER_ERROR_FILE_NOT_FOUND;            
        stream_writer->writer = fp;
    }
    else{
      stream_writer->writer = stdout;
    }
    stream_writer->encoding = NULL;
    if (encoding)
        stream_writer->encoding = GUTHTHILA_STRDUP(environment, encoding);
   
        
    stream_writer->prefix_defaulting = prefix_defaulting;




    stream_writer->element_stack =
        guththila_array_make (environment, 64, sizeof (guththila_char_t **));
    stream_writer->namespace_hash = guththila_hash_make (environment);
    stream_writer->prefix_hash = guththila_hash_make (environment);
    stream_writer->in_start_element = 0;
    stream_writer->empty_element = 0;

    return stream_writer;
}

GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_end_start_element 
                            (guththila_environment_t *environment,
                             guththila_xml_stream_writer_t *stream_writer)
{
    guththila_char_t ***del_element;
    if (!stream_writer)
        return GUTHTHILA_STREAM_WRITER_ERROR_STREAM_STRUCT_NULL;
    if (!stream_writer->in_start_element)
        return GUTHTHILA_STREAM_WRITER_ERROR_NOT_IN_GUTHTHILA_START_ELEMENT;
    if (stream_writer->empty_element)
    {
        if (fputs ("/", stream_writer->writer) == EOF)
        {
            return GUTHTHILA_STREAM_WRITER_ERROR_WRITING_TO_STREAM;
        }

        /*pop last element and delete that elelemt */
        del_element =
            (guththila_char_t ***) guththila_array_pop (stream_writer->element_stack);
        if (del_element)
        {
            if ((*del_element)[0])
                GUTHTHILA_FREE (environment->allocator, (*del_element)[0]);
            if ((*del_element)[1])
                GUTHTHILA_FREE (environment->allocator, (*del_element)[1]);
            GUTHTHILA_FREE (environment->allocator, del_element);
        }
        del_element = 0;

        /*end pop element */


        stream_writer->empty_element = 0;
    }
    fputs (">", stream_writer->writer);
    stream_writer->in_start_element = 0;

    return GUTHTHILA_SUCCESS;
}

GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_start_element 
                (guththila_environment_t *environment,
                 guththila_xml_stream_writer_t *stream_writer,
                 guththila_char_t *local_name)
{
    guththila_char_t ***new_element;
    if (!stream_writer)
        return GUTHTHILA_STREAM_WRITER_ERROR_STREAM_STRUCT_NULL;
    if (!local_name)
        return GUTHTHILA_STREAM_WRITER_ERROR_LOCAL_NAME_NULL;
    guththila_xml_stream_writer_end_start_element (environment, stream_writer);
    
    /*namespaces.pushContext(); */
    fputs ("<", stream_writer->writer);
    fputs (local_name, stream_writer->writer);

    /*push element to stack */
    new_element =
        (guththila_char_t ***) guththila_array_push (stream_writer->element_stack);
    *new_element =
        (guththila_char_t **) GUTHTHILA_MALLOC (environment->allocator,
                                        sizeof (guththila_char_t **) *2);
    (*new_element)[0] = 0;
    (*new_element)[1] = GUTHTHILA_STRDUP (environment, local_name);
    /*end push element */

    stream_writer->in_start_element = 1;

    return GUTHTHILA_SUCCESS;
}


GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_start_element_with_namespace
                               (guththila_environment_t * environment,
                                guththila_xml_stream_writer_t * stream_writer,
                                guththila_char_t * local_name,
                                guththila_char_t * namespace_uri)
{
    int is_declared;
    guththila_char_t ***new_element;
    guththila_char_t *prefix;
    if (!stream_writer)
        return GUTHTHILA_STREAM_WRITER_ERROR_STREAM_STRUCT_NULL;
    if (!local_name)
        return GUTHTHILA_STREAM_WRITER_ERROR_LOCAL_NAME_NULL;
    if (!namespace_uri)
        return GUTHTHILA_STREAM_WRITER_ERROR_guththila_namespace_t_NULL;

    guththila_xml_stream_writer_end_start_element (environment,
                                                   stream_writer);

    prefix =
        guththila_xml_stream_writer_get_prefix (environment, stream_writer,
                                                namespace_uri);

    is_declared = (prefix != 0);

    if (!is_declared)
    {
        if (stream_writer->prefix_defaulting)
            prefix = GUTHTHILA_DEFAULT_NS_PREFIX;
        else
            return
                GUTHTHILA_STREAM_WRITER_ERROR_guththila_namespace_t_NOT_DECLARED;
    }

    fputs ("<", stream_writer->writer);

    if (prefix && GUTHTHILA_STRLEN ( prefix) != 0)
    {
        fputs (prefix, stream_writer->writer);
        fputs (":", stream_writer->writer);
    }

    fputs (local_name, stream_writer->writer);

    if (stream_writer->prefix_defaulting && !is_declared)
    {
        if (prefix)
            guththila_xml_stream_writer_write_namespace (environment,
                                                         stream_writer,
                                                         prefix,
                                                         namespace_uri);
    }
    /*push element to stack */
    /*guththila_char_t** new_element = (guththila_char_t**)apr_array_push(stream_writer->element_stack); */
    new_element =
        (guththila_char_t ***) guththila_array_push (stream_writer->
                                                     element_stack);
    *new_element =
        (guththila_char_t **) GUTHTHILA_MALLOC (environment->allocator,
                                                sizeof (guththila_char_t **) *
                                                2);
    if (prefix)
        (*new_element)[0] = GUTHTHILA_STRDUP (environment, prefix);
    else
        (*new_element)[0] = 0;
    (*new_element)[1] = GUTHTHILA_STRDUP (environment, local_name);
    /*end push element */

    stream_writer->in_start_element = 1;

    return GUTHTHILA_SUCCESS;
}

GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_start_element_with_namespace_prefix
                        (guththila_environment_t * environment,
                         guththila_xml_stream_writer_t * stream_writer,
                         guththila_char_t * local_name,
                         guththila_char_t * namespace_uri,
                         guththila_char_t * prefix)
{
    guththila_char_t *current_prefix;
    int is_current = 0;
    guththila_char_t ***new_element;
    if (!stream_writer)
        return GUTHTHILA_STREAM_WRITER_ERROR_STREAM_STRUCT_NULL;
    if (!local_name)
        return GUTHTHILA_STREAM_WRITER_ERROR_LOCAL_NAME_NULL;
    if (!namespace_uri)
        return GUTHTHILA_STREAM_WRITER_ERROR_guththila_namespace_t_NULL;
    if (!prefix)
        return GUTHTHILA_STREAM_WRITER_ERROR_PREFIX_NULL;

    guththila_xml_stream_writer_end_start_element (environment,
                                                   stream_writer);

    current_prefix =
        guththila_xml_stream_writer_get_prefix (environment, stream_writer,
                                                namespace_uri);

    if (current_prefix)
        is_current =
            (GUTHTHILA_STRCMP ( prefix, current_prefix) !=
             0);
    fputs ("<", stream_writer->writer);

    if (prefix && GUTHTHILA_STRLEN ( prefix) != 0)
    {
        fputs (prefix, stream_writer->writer);
        fputs (":", stream_writer->writer);
    }

    fputs (local_name, stream_writer->writer);

    if (stream_writer->prefix_defaulting && !is_current)
    {
        if (prefix)
            guththila_xml_stream_writer_write_namespace (environment,
                                                         stream_writer,
                                                         prefix,
                                                         namespace_uri);
    }

    /*push element to stack */
    /*guththila_char_t** new_element = (guththila_char_t**)apr_array_push(stream_writer->element_stack); */
    new_element =
        (guththila_char_t ***) guththila_array_push (stream_writer->
                                                     element_stack);

    *new_element =
        (guththila_char_t **) GUTHTHILA_MALLOC (environment->allocator,
                                                sizeof (guththila_char_t **) *
                                                2);
    if (prefix)
        (*new_element)[0] = GUTHTHILA_STRDUP (environment, prefix);
    else
        (*new_element)[0] = 0;
    (*new_element)[1] = GUTHTHILA_STRDUP (environment, local_name);
    /*end push element */

    stream_writer->in_start_element = 1;

    return GUTHTHILA_SUCCESS;
}


GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_empty_element 
                        (guththila_environment_t *environment,
                         guththila_xml_stream_writer_t *stream_writer,
                         guththila_char_t *local_name)
{
    int ret =
        guththila_xml_stream_writer_write_start_element (environment,
                                                         stream_writer,
                                                         local_name);

    if (ret != GUTHTHILA_SUCCESS)
        return ret;

    stream_writer->empty_element = 1;

    return GUTHTHILA_SUCCESS;
}

GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_empty_element_with_namespace
                    (guththila_environment_t * environment,
                     guththila_xml_stream_writer_t * stream_writer,
                     guththila_char_t * local_name,
                     guththila_char_t * namespace_uri)
{
    int ret =
        guththila_xml_stream_writer_write_start_element_with_namespace
        (environment, stream_writer, local_name, namespace_uri);

    if (ret != GUTHTHILA_SUCCESS)
        return ret;

    stream_writer->empty_element = 1;

    return GUTHTHILA_SUCCESS;
}

GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_empty_element_with_namespace_prefix
                    (guththila_environment_t * environment, 
                     guththila_xml_stream_writer_t * stream_writer,
                     guththila_char_t * local_name, 
                     guththila_char_t * namespace_uri,
                     guththila_char_t * prefix)
{
    int ret =
        guththila_xml_stream_writer_write_start_element_with_namespace_prefix
        (environment, stream_writer, local_name, namespace_uri, prefix);

    if (ret != GUTHTHILA_SUCCESS)
        return ret;

    stream_writer->empty_element = 1;

    return GUTHTHILA_SUCCESS;
}

GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_end_element 
                        (guththila_environment_t *environment,
                         guththila_xml_stream_writer_t *stream_writer)
{
    guththila_char_t ***element_data;
    guththila_xml_stream_writer_end_start_element (environment,
                                                   stream_writer);

    /* pop element stack */
    element_data =
        (guththila_char_t ***) guththila_array_pop (stream_writer->
                                                    element_stack);
    if (!element_data)
        return
            GUTHTHILA_STREAM_WRITER_ERROR_guththila_element_t_guththila_stack_t_EMPTY;

    /*end pop element stack */

    fputs ("</", stream_writer->writer);

    if ((*element_data)[0]
        && GUTHTHILA_STRLEN ((*element_data)[0]) > 0)
    {
        fputs ((*element_data)[0], stream_writer->writer);
        fputs (":", stream_writer->writer);
        GUTHTHILA_FREE(environment->allocator ,((*element_data)[0]));
    }

    if ((*element_data)[1]
        && GUTHTHILA_STRLEN ((*element_data)[1]) > 0)
    {
        fputs ((*element_data)[1], stream_writer->writer);
        GUTHTHILA_FREE(environment->allocator ,((*element_data)[1]));
    }
    else
        return
            GUTHTHILA_STREAM_WRITER_ERROR_guththila_element_t_guththila_stack_t_EMPTY;

    fputs (">", stream_writer->writer);

    *element_data = 0;

    return GUTHTHILA_SUCCESS;
}


GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_end_document 
                        (guththila_environment_t *environment,
                         guththila_xml_stream_writer_t *stream_writer)
{
    while (!guththila_is_empty_array (stream_writer->element_stack))
    {
        int ret =
            guththila_xml_stream_writer_write_end_element (environment,
                                                           stream_writer);
        if (ret != GUTHTHILA_SUCCESS)
            return ret;

    }
    return GUTHTHILA_SUCCESS;
}


GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_attribute 
                            (guththila_environment_t *environment,
                             guththila_xml_stream_writer_t *stream_writer,
                             guththila_char_t * local_name,
                             guththila_char_t * value)
{
    if (!stream_writer->in_start_element)
        return GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_STATE;

    fputs (" ", stream_writer->writer);
    fputs (local_name, stream_writer->writer);
    fputs ("=\"", stream_writer->writer);
    guththila_xml_stream_writer_write_encoded (environment, stream_writer,
                                               value, 1);
    fputs ("\"", stream_writer->writer);

    return GUTHTHILA_SUCCESS;
}


GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_attribute_with_namespace
                        (guththila_environment_t * environment, 
                         guththila_xml_stream_writer_t * stream_writer,
                         guththila_char_t * local_name,
                         guththila_char_t * value, 
                         guththila_char_t * namespace_uri)
{
    guththila_char_t *prefix;
    if (!stream_writer->in_start_element)
        return GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_STATE;

    prefix =
        guththila_xml_stream_writer_get_prefix (environment, stream_writer,
                                                namespace_uri);
    if (!prefix)
    {
        if (stream_writer->prefix_defaulting)
        {
            prefix = GUTHTHILA_DEFAULT_NS_PREFIX;
            guththila_xml_stream_writer_write_namespace (environment,
                                                         stream_writer,
                                                         prefix,
                                                         namespace_uri);
        }
        else
            return
                GUTHTHILA_STREAM_WRITER_ERROR_guththila_namespace_t_NOT_DECLARED;
    }

    fputs (" ", stream_writer->writer);

    if (prefix && GUTHTHILA_STRLEN (prefix) > 0)
    {
        fputs (prefix, stream_writer->writer);
        fputs (":", stream_writer->writer);
    }
    fputs (local_name, stream_writer->writer);
    fputs ("=\"", stream_writer->writer);
    guththila_xml_stream_writer_write_encoded (environment, stream_writer,
                                               value, 1);
    fputs ("\"", stream_writer->writer);

    return GUTHTHILA_SUCCESS;
}

GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_attribute_with_namespace_prefix
                    (guththila_environment_t * environment, 
                     guththila_xml_stream_writer_t * stream_writer,
                     guththila_char_t * local_name, 
                     guththila_char_t * value, 
                     guththila_char_t * namespace_uri,
                     guththila_char_t * prefix)
{
    guththila_char_t *current_prefix;
    if (!stream_writer->in_start_element)
        return GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_STATE;

    current_prefix =
        guththila_xml_stream_writer_get_prefix (environment, stream_writer,
                                                namespace_uri);
    if (!current_prefix)
    {
        if (stream_writer->prefix_defaulting)
            guththila_xml_stream_writer_write_namespace (environment,
                                                         stream_writer,
                                                         prefix,
                                                         namespace_uri);
        else
            return
                GUTHTHILA_STREAM_WRITER_ERROR_guththila_namespace_t_NOT_DECLARED;
    }
    else if (GUTHTHILA_STRCMP (current_prefix, prefix) !=
             0)
        return
            GUTHTHILA_STREAM_WRITER_ERROR_guththila_namespace_t_NOT_DECLARED;

    fputs (" ", stream_writer->writer);

    if (prefix && GUTHTHILA_STRLEN ( prefix) > 0)
    {
        fputs (prefix, stream_writer->writer);
        fputs (":", stream_writer->writer);
    }
    fputs (local_name, stream_writer->writer);
    fputs ("=\"", stream_writer->writer);
    guththila_xml_stream_writer_write_encoded (environment, stream_writer,
                                               value, 1);
    fputs ("\"", stream_writer->writer);

    return GUTHTHILA_SUCCESS;
}

GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_namespace 
                            (guththila_environment_t *environment,
                             guththila_xml_stream_writer_t *stream_writer,
                             guththila_char_t * prefix,
                             guththila_char_t * namespace_uri)
{
    guththila_char_t *declared_prefix;
    if (!stream_writer->in_start_element)
        return GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_STATE;

    if (!prefix)
        prefix = GUTHTHILA_DEFAULT_NS_PREFIX;

    declared_prefix =
        guththila_xml_stream_writer_get_prefix (environment, stream_writer,
                                                namespace_uri);
    if (declared_prefix
        && GUTHTHILA_STRCMP ( prefix,  declared_prefix) == 0)
        return GUTHTHILA_SUCCESS;
    guththila_xml_stream_writer_set_prefix (environment, stream_writer,
                                            prefix, namespace_uri);

    fputs (" ", stream_writer->writer);
    fputs ("xmlns", stream_writer->writer);

    if (GUTHTHILA_STRCMP ( GUTHTHILA_DEFAULT_NS_PREFIX, prefix) != 0)
    {
        fputs (":", stream_writer->writer);
        fputs (prefix, stream_writer->writer);
    }
    fputs ("=\"", stream_writer->writer);
    fputs (namespace_uri, stream_writer->writer);
    fputs ("\"", stream_writer->writer);
    return GUTHTHILA_SUCCESS;
}

GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_default_namespace 
                        (guththila_environment_t *environment,
                         guththila_xml_stream_writer_t *stream_writer,
                         guththila_char_t *namespace_uri)
{
    int ret =
        guththila_xml_stream_writer_write_namespace (environment,
                stream_writer, GUTHTHILA_DEFAULT_NS_PREFIX, namespace_uri);
    return ret;
}


GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_comment 
                    (guththila_environment_t *environment,
                     guththila_xml_stream_writer_t *stream_writer,
                     guththila_char_t * data)
{
    guththila_char_t *ptr;
    guththila_xml_stream_writer_end_start_element (environment,
                                                   stream_writer);

    if (!data)
        return GUTHTHILA_STREAM_WRITER_ERROR_GUTHTHILA_COMMENT_NULL;

    ptr = data;
    while (*ptr)
    {
        if (*ptr++ == '-')
            if (*ptr == '-')
                return
                    GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_GUTHTHILA_COMMENT;
    }

    fputs ("<!--", stream_writer->writer);

    fputs (data, stream_writer->writer);
    fputs ("-->", stream_writer->writer);
    return GUTHTHILA_SUCCESS;
}


GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_processing_instruction
                        (guththila_environment_t * environment, 
                         guththila_xml_stream_writer_t * stream_writer,
                         guththila_char_t * target)
{
    int ret =
        guththila_xml_stream_writer_write_processing_instruction_data
        (environment, stream_writer, target, 0);
    return ret;
}

GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_processing_instruction_data
                        (guththila_environment_t * environment, 
                         guththila_xml_stream_writer_t * stream_writer,
                         guththila_char_t * target, 
                         guththila_char_t * data)
{
    if (!target)
        return
            GUTHTHILA_STREAM_WRITER_ERROR_PROCESSING_INSTRUCTION_TARGET_NULL;

    guththila_xml_stream_writer_end_start_element (environment,
                                                   stream_writer);

    fputs ("<?", stream_writer->writer);
    fputs (target, stream_writer->writer);

    if (data)
    {
        fputs (" ", stream_writer->writer);
        fputs (data, stream_writer->writer);
    }

    fputs ("?>", stream_writer->writer);

    return GUTHTHILA_SUCCESS;
}


GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_cdata 
                            (guththila_environment_t *environment,
                             guththila_xml_stream_writer_t *stream_writer,
                             guththila_char_t * data)
{
    guththila_char_t *ptr;
    if (!data)
        return GUTHTHILA_STREAM_WRITER_ERROR_CDATA_NULL;

    guththila_xml_stream_writer_end_start_element (environment,
                                                   stream_writer);

    ptr = data;
    while (*ptr)
    {
        if (*ptr++ == ']')
            if (*ptr == ']')
                return GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_CDATA;
    }

    fputs ("<![CDATA[", stream_writer->writer);
    fputs (data, stream_writer->writer);
    fputs ("]]>", stream_writer->writer);

    return GUTHTHILA_SUCCESS;
}


GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_dtd 
                        (guththila_environment_t * environment,
                         guththila_xml_stream_writer_t *stream_writer,
                         guththila_char_t * dtd)
{
    if (!dtd)
        return GUTHTHILA_STREAM_WRITER_ERROR_DTD_NULL;

    fputs ("<!DOCTYPE ", stream_writer->writer);
    fputs (dtd, stream_writer->writer);
    fputs (">", stream_writer->writer);

    return GUTHTHILA_SUCCESS;
}


GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_entity_ref 
                    (guththila_environment_t *environment,
                     guththila_xml_stream_writer_t *stream_writer,
                     guththila_char_t * name)
{
    if (!name)
        return GUTHTHILA_STREAM_WRITER_ERROR_ENTITY_REF_NULL;

    guththila_xml_stream_writer_end_start_element (environment,
                                                   stream_writer);

    fputs ("&", stream_writer->writer);
    fputs (name, stream_writer->writer);
    fputs (";", stream_writer->writer);

    return GUTHTHILA_SUCCESS;
}


GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_start_document 
                            (guththila_environment_t *environment,
                             guththila_xml_stream_writer_t *stream_writer)
{
    int ret =
        guththila_xml_stream_writer_write_start_document_with_version_encoding
        (environment, stream_writer, 0, 0);
    return ret;
}


GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_start_document_with_version
                            (guththila_environment_t * environment, 
                             guththila_xml_stream_writer_t * stream_writer,
                             guththila_char_t * version)
{
    int ret =
        guththila_xml_stream_writer_write_start_document_with_version_encoding
        (environment, stream_writer, 0, version);
    return ret;

}


GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_start_document_with_version_encoding
                            (guththila_environment_t * environment, 
                             guththila_xml_stream_writer_t * stream_writer,
                             guththila_char_t * encoding,
                             guththila_char_t * version)
{
    if (!version)
        version = "1.0";

    encoding = "UTF-8";         /* The parameter is ignored */

    if ((GUTHTHILA_STRCMP ( "1.0", version) != 0)
        && (GUTHTHILA_STRCMP ( "1.1", version) != 0))
        return GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_XML_VERSION;

    fputs ("<?xml version=\"", stream_writer->writer);
    fputs (version, stream_writer->writer);
    fputs ("\" encoding=\"", stream_writer->writer);
    fputs (encoding, stream_writer->writer);
    fputs ("\"?>", stream_writer->writer);
    fputs ("\r\n", stream_writer->writer);

    return GUTHTHILA_SUCCESS;
}

GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_characters 
                        (guththila_environment_t *environment,
                         guththila_xml_stream_writer_t *stream_writer,
                         guththila_char_t * text)
{
    if (!text)
        return GUTHTHILA_STREAM_WRITER_ERROR_TEXT_NULL;

    guththila_xml_stream_writer_end_start_element (environment,
                                                   stream_writer);

    if (text)
        guththila_xml_stream_writer_write_encoded (environment, stream_writer,
                                                   text, 0);

    return GUTHTHILA_SUCCESS;
}

GUTHTHILA_DECLARE (guththila_char_t *)
guththila_xml_stream_writer_get_prefix 
                                (guththila_environment_t * environment,
                                 guththila_xml_stream_writer_t *stream_writer, 
                                 guththila_char_t * uri)
{
    guththila_char_t *prefix;
    if (!uri)
        return 0;

    prefix =
        guththila_hash_get (stream_writer->namespace_hash, uri,
                            GUTHTHILA_HASH_KEY_STRING);
    return prefix;
}

GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_set_prefix 
                            (guththila_environment_t * environment,
                             guththila_xml_stream_writer_t *stream_writer,
                             guththila_char_t * prefix,
                             guththila_char_t * uri)
{
    if (!prefix)
        return GUTHTHILA_STREAM_WRITER_ERROR_PREFIX_NULL;

    if (!uri)
        return GUTHTHILA_STREAM_WRITER_ERROR_guththila_namespace_t_NULL;

    if (GUTHTHILA_STRCMP ( prefix, "xml") == 0
        || GUTHTHILA_STRCMP ( prefix, "xmlns") == 0)
        return GUTHTHILA_STREAM_WRITER_ERROR_ILLEGAL_PREFIX;

    guththila_hash_set (stream_writer->namespace_hash, uri,
                        GUTHTHILA_HASH_KEY_STRING, prefix);
    guththila_hash_set (stream_writer->prefix_hash, prefix, GUTHTHILA_HASH_KEY_STRING, uri);    /*over write another prefix */
    return GUTHTHILA_SUCCESS;
}


GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_set_default_prefix 
                        (guththila_environment_t *environment,
                         guththila_xml_stream_writer_t *stream_writer,
                         guththila_char_t * uri)
{
    return guththila_xml_stream_writer_set_prefix (environment, stream_writer,
                                                   GUTHTHILA_DEFAULT_NS_PREFIX,
                                                   uri);

}

GUTHTHILA_DECLARE (int)
guththila_xml_stream_writer_write_encoded 
                            (guththila_environment_t *environment,
                             guththila_xml_stream_writer_t *stream_writer,
                             guththila_char_t * text,
                             int in_attr)
{
    guththila_char_t *start;
    guththila_char_t *ptr;
    if (!text)
        return GUTHTHILA_STREAM_WRITER_ERROR_TEXT_NULL;

    start = text;
    ptr = text;

    for (ptr = text; *ptr; ptr++)
    {
        guththila_char_t c = *ptr;
        if (c == '<' || c == '>' || c == '&')
        {
            guththila_char_t *to_write =
                GUTHTHILA_STRNDUP (environment,  start,  (guththila_ssize_t) (ptr - start));
            if (to_write)
            {
                fputs (to_write, stream_writer->writer);
                GUTHTHILA_FREE(environment->allocator, to_write);
                /*free (to_write); */
            }
            else
                return GUTHTHILA_STREAM_WRITER_ERROR_OUT_OF_MEMORY;

            if (c == '<')
                fputs ("&lt;", stream_writer->writer);

            else if (c == '>')
                fputs ("&gt;", stream_writer->writer);
            else
                fputs ("&amp;", stream_writer->writer);

            start = ptr + 1;
        }
        else if (in_attr && (c == '"' || c == '\''))
        {
            guththila_char_t *to_write =
                GUTHTHILA_STRNDUP (environment , start,
                                   (guththila_ssize_t) (ptr - start));
            if (to_write)
            {
                fputs (to_write, stream_writer->writer);
                GUTHTHILA_FREE(environment->allocator, to_write);
                /*free (to_write); */
            }
            else
                return GUTHTHILA_STREAM_WRITER_ERROR_OUT_OF_MEMORY;



            if (c == '"')
                fputs ("&quot;", stream_writer->writer);
            else
                fputs ("&apos;", stream_writer->writer);
            start = ptr + 1;

        }
    }

    if (ptr - start)
    {
        guththila_char_t *to_write =
            GUTHTHILA_STRNDUP (environment, start,
                               (guththila_ssize_t) (ptr - start));
        if (to_write)
        {
            fputs (to_write, stream_writer->writer);
            GUTHTHILA_FREE(environment->allocator, to_write);
            /*free (to_write); */
        }
        else
            return GUTHTHILA_STREAM_WRITER_ERROR_OUT_OF_MEMORY;
    }

    return GUTHTHILA_SUCCESS;
}

GUTHTHILA_DECLARE(int)
guththila_xml_stream_writer_free(guththila_environment_t *environment,
                                 guththila_xml_stream_writer_t *writer)
{
    if(!environment) return GUTHTHILA_FAILURE;
    if(!writer) return GUTHTHILA_FAILURE;
   
    if(writer->encoding)
       GUTHTHILA_FREE(environment->allocator, writer->encoding);
    
    if(writer->namespace_hash)
        guththila_hash_free(environment, writer->namespace_hash);
    if(writer->prefix_hash)
        guththila_hash_free(environment, writer->prefix_hash);
    
    if(writer->element_stack)
        guththila_array_free(environment, writer->element_stack);
    
    GUTHTHILA_FREE(environment->allocator, writer);
    return GUTHTHILA_SUCCESS;   
}
