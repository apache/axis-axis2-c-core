
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <guththila.h>

#define GUTHTHILA_VALIDATION_PARSER

/*
 * Read the next char from the reader and return it.
 */
static int
guththila_next_char(
    guththila_t * m,
    const axutil_env_t * env);

/* part of guththila_next_char method. this was included as macro for performance. 99% of the time
 * following will be called, so having it as next_char method is very expensive (method calling
 * overhead is higher) so, common case is checked as part of the macro and if not satisfied, method
 * is called
 */
#define GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c)\
{\
    int result_found = 0;\
    if(!buffer)\
    {\
        if(m->reader->type == GUTHTHILA_MEMORY_READER)\
        {\
            buffer = m->buffer.buff[0];\
            data_size = m->buffer.data_size[0];\
            previous_size = 0;\
        }\
        else\
        {\
            if(m->buffer.cur_buff != -1)\
            {\
                buffer = m->buffer.buff[m->buffer.cur_buff];\
                data_size = GUTHTHILA_BUFFER_CURRENT_DATA_SIZE(m->buffer);\
                previous_size = GUTHTHILA_BUFFER_PRE_DATA_SIZE(m->buffer);\
            }\
            else\
            {\
                c = guththila_next_char(m, env);\
                if(c < 0)\
                    return -1;\
                result_found  = 1;\
            }\
        }\
    }\
\
    if(!result_found)\
    {\
        size_t index = m->next++ - previous_size;\
        if(index < data_size)\
        {\
            c = buffer[index];\
        }\
        else\
        {\
            buffer = NULL;\
            data_size = -1;\
            --(m->next);\
            if(m->reader->type == GUTHTHILA_MEMORY_READER)\
            {\
                return -1;\
            }\
            else\
            {\
                c = guththila_next_char(m, env);\
                if(c < 0)\
                    return -1;\
            }\
        }\
    }\
}

/*
 * Read the specified number of characters at once.
 */
static int
guththila_next_no_char(
    guththila_t * m,
    int eof,
    guththila_char_t *bytes,
    size_t no,
    const axutil_env_t * env);

/*
 * Close a token that is opened previously. 
 */
static void
guththila_token_close(
    guththila_t * m,
    guththila_token_t * tok,
    int tok_type,
    int referer,
    const axutil_env_t * env);

/* 
 * Process the XMl declaration part of a XML document.
 */
static int
guththila_process_xml_dec(
    guththila_t * m,
    const axutil_env_t * env);

/*
 * Return non zero value if the given argument is a space. (c < 0x21) is added to improve the
 * performance. common case is printable characters. and if given character is printable, we can
 * return false immediately.
 */
#define GUTHTHILA_IS_SPACE(c) ((c < 0x21) && (c == 0x20 || c == 0xD || c == 0xA || c == 0x9))

/* 
 * Read characters until all the white spaces are read.
 */
#define GUTHTHILA_SKIP_SPACES(m, c, buffer, data_size, previous_size, _env)while(GUTHTHILA_IS_SPACE(c)){GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, _env, c);}


#define GUTHTHILA_XML_NAME "xml"

#define GUTHTHILA_XML_URI "http://www.w3.org/XML/1998/namespace"

/*
 * Open a token. When we open a token we don't know it's type. We only 
 * set the starting values.
 */
#define GUTHTHILA_TOKEN_OPEN(m, tok, _env)					\
    m->temp_tok = guththila_tok_list_get_token(&m->tokens, _env); \
    m->temp_tok->type = _Unknown; \
    m->temp_tok->_start = (int)m->next; \
    m->last_start = (int)m->next - 1;
/* We are sure that the difference lies within the int range */

/*
 * Read until we met a = character.
 */
#define GUTHTHILA_PROCESS_EQU(m, c, ic, buffer, data_size, previous_size, _env)\
{\
    GUTHTHILA_SKIP_SPACES(m, c, buffer, data_size, previous_size, _env); \
    if (c == '=')\
    { \
        GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, _env, ic); \
        GUTHTHILA_SKIP_SPACES(m, ic, buffer, data_size, previous_size, _env); \
    }\
}

/*
 * Initialize a attribute to the values given.
 */
#define GUTHTHILA_ATTRIBUTE_INITIALIZE(_attr, _pref, _name, _val)	\
    (_attr->pref = (_pref)); \
    (_attr->name = (_name)); \
    (_attr->val = (_val));

/*
 * Initialize namespace to the values given.
 */
#define GUTHTHILA_NAMESPACE_INITIALIZE(_namesp, _name, _uri)	\
    (_namesp->name = _name); \
    (_namesp->uri = _uri);

/*
 * Determine weahter a given character is a valid starting char for a xml name.
 */
#define GUTHTHILA_IS_VALID_STARTING_CHAR(c) (isalpha(c) || '_' == c || ':' == c)

/*
 * Initialize the variables in the guththila_t structure.
 */
#define GUTHTHILA_VARIABLE_INITIALZE(m) \
    m->temp_prefix = NULL; \
    m->temp_name = NULL; \
    m->temp_tok = NULL; \
    if (m->value) guththila_tok_list_release_token(&m->tokens, m->value, env); \
    m->name = NULL; \
    m->prefix = NULL; \
    m->value = NULL;

/*
 * Initialize the guththila_t structure with the reader. 
 * All the values will be set to default values. 
 */
GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_init(
    guththila_t * m,
    void *reader,
    const axutil_env_t * env)
{
    guththila_token_t* temp_name = NULL;
    guththila_token_t* temp_tok = NULL;
    guththila_elem_namesp_t* e_namesp = NULL;

    if(!((guththila_reader_t *)reader))
        return GUTHTHILA_FAILURE;
    m->reader = (guththila_reader_t *)reader;
    if(!guththila_tok_list_init(&m->tokens, env))
    {
        return GUTHTHILA_FAILURE;
    }
    if(m->reader->type == GUTHTHILA_MEMORY_READER)
    {
        guththila_buffer_init_for_buffer(&m->buffer, m->reader->buff, m->reader->buff_size, env);
    }
    else if(m->reader->type == GUTHTHILA_FILE_READER || m->reader->type == GUTHTHILA_IO_READER)
    {
        guththila_buffer_init(&m->buffer, 0, env);
    }
    guththila_stack_init(&m->elem, env);
    guththila_stack_init(&m->attrib, env);
    guththila_stack_init(&m->namesp, env);
    temp_name = guththila_tok_list_get_token(&m->tokens, env);
    temp_tok = guththila_tok_list_get_token(&m->tokens, env);
    if(temp_tok && temp_name)
    {
        guththila_set_token(temp_name, GUTHTHILA_XML_NAME, 0, (int)strlen(GUTHTHILA_XML_NAME), 1,
            0, 0, env);

        guththila_set_token(temp_tok, GUTHTHILA_XML_URI, 0, (int)strlen(GUTHTHILA_XML_URI), 1, 0,
            0, env);

    }

    e_namesp = (guththila_elem_namesp_t *)AXIS2_MALLOC(env->allocator,
        sizeof(guththila_elem_namesp_t));
    if(e_namesp && temp_tok && temp_name)
    {
        e_namesp->namesp = (guththila_namespace_t *)AXIS2_MALLOC(env->allocator,
            sizeof(guththila_namespace_t) * GUTHTHILA_NAMESPACE_DEF_SIZE);
    }
    if(e_namesp->namesp)
    {
        e_namesp->no = 1;
        e_namesp->size = GUTHTHILA_NAMESPACE_DEF_SIZE;
        e_namesp->namesp[0].name = temp_name;
        e_namesp->namesp[0].uri = temp_tok;
        guththila_stack_push(&m->namesp, e_namesp, env);
    }
    else
    {
        if(temp_name)
        {
            AXIS2_FREE(env->allocator, temp_name);
            temp_name = NULL;
        }
        if(temp_tok)
        {
            AXIS2_FREE(env->allocator, temp_tok);
            temp_tok = NULL;
        }
        if(e_namesp)
        {
            AXIS2_FREE(env->allocator, e_namesp);
            e_namesp = NULL;
        }
        return GUTHTHILA_FAILURE;
    }
    m->name = NULL;
    m->prefix = NULL;
    m->value = NULL;
    m->status = S_1;
    m->guththila_event = -1;
    m->next = 0;
    m->last_start = -1;
    m->temp_name = NULL;
    m->temp_prefix = NULL;
    m->temp_tok = NULL;
    return GUTHTHILA_SUCCESS;
}

/*
 * Uninitialize a guththila_t structure. This method deallocates all the 
 * resources that are held in the guththila_t structure.
 */
GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_un_init(
    guththila_t * m,
    const axutil_env_t * env)
{
    int size = 0, i = 0, j = 0;
    guththila_attr_t * attr = NULL;
    guththila_element_t* elem = NULL;
    guththila_elem_namesp_t * e_namesp = NULL;

    if(m->prefix)
    {
        guththila_tok_list_release_token(&m->tokens, m->prefix, env);
    }
    if(m->name)
    {
        guththila_tok_list_release_token(&m->tokens, m->name, env);
    }
    if(m->value)
    {
        guththila_tok_list_release_token(&m->tokens, m->value, env);
    }
    if(m->temp_tok)
    {
        guththila_tok_list_release_token(&m->tokens, m->temp_tok, env);
    }
    if(m->temp_name)
    {
        guththila_tok_list_release_token(&m->tokens, m->temp_name, env);
    }
    if(m->temp_prefix)
    {
        guththila_tok_list_release_token(&m->tokens, m->temp_prefix, env);
    }
    size = GUTHTHILA_STACK_SIZE(m->attrib);
    for(i = 0; i < size; i++)
    {
        attr = (guththila_attr_t *)guththila_stack_pop(&m->attrib, env);
        if(attr)
        {
            if(attr->name)
                guththila_tok_list_release_token(&m->tokens, attr->name, env);
            if(attr->pref)
                guththila_tok_list_release_token(&m->tokens, attr->pref, env);
            AXIS2_FREE(env->allocator, attr);
        }
    }
    guththila_stack_un_init(&m->attrib, env);

#ifndef GUTHTHILA_VALIDATION_PARSER
    guththila_namespace_t * namesp = NULL;

    size = GUTHTHILA_STACK_SIZE(m->namesp);
    for (i = 0; i < size; i++)
    {
        namesp =
        (guththila_namespace_t *) guththila_stack_pop(&m->namesp, env);
        if (namesp)
        {
            if (namesp->name)
            guththila_tok_list_release_token(&m->tokens, namesp->name, env);
            if (namesp->uri)
            guththila_tok_list_release_token(&m->tokens, namesp->uri, env);
            AXIS2_FREE(env->allocator, namesp);
        }
    }
#else 

    size = GUTHTHILA_STACK_SIZE(m->namesp);
    for(i = 0; i < size; i++)
    {
        e_namesp = (guththila_elem_namesp_t *)guththila_stack_pop(&m->namesp, env);
        if(e_namesp)
        {
            for(j = 0; j < e_namesp->no; j++)
            {
                if(e_namesp->namesp[j].name)
                {
                    guththila_tok_list_release_token(&m->tokens, e_namesp->namesp[j].name, env);
                }
                if(e_namesp->namesp[j].uri)
                {
                    guththila_tok_list_release_token(&m->tokens, e_namesp->namesp[j].uri, env);
                }
            }
            AXIS2_FREE(env->allocator, e_namesp->namesp);
            AXIS2_FREE(env->allocator, e_namesp);
        }
    }
#endif  

    size = GUTHTHILA_STACK_SIZE(m->elem);
    for(i = 0; i < size; i++)
    {
        elem = (guththila_element_t *)guththila_stack_pop(&m->elem, env);
        if(elem)
        {
            if(elem->name)
                guththila_tok_list_release_token(&m->tokens, elem->name, env);
            if(elem->prefix)
                guththila_tok_list_release_token(&m->tokens, elem->prefix, env);
            AXIS2_FREE(env->allocator, elem);
        }
    }
    guththila_stack_un_init(&m->elem, env);
    guththila_stack_un_init(&m->namesp, env);
    guththila_tok_list_free_data(&m->tokens, env);
    guththila_buffer_un_init(&m->buffer, env);
    AXIS2_FREE(env->allocator, m);
    return GUTHTHILA_SUCCESS;
}

/*static int
guththila_utf8_bytes_len(
    unsigned int c)
{
    if (c < 0x80)
        return 1;
    else if (c < 0x800)
        return 2;
    else if (c < 0x10000)
        return 3;
    else if (c < 0x200000)
        return 4;
    else if (c < 0x4000000)
        return 5;
    else
        return 6;
}*/

/*
 * Stores the given unicode char as UTF-8
 */
static guththila_char_t*
guththila_utf8_bytes(
    unsigned int c,
    guththila_char_t *start)
{
    int first;
    guththila_char_t *end, *p;

    if(c <           0x80)
    {
        *start = c;
        return start + 1;
    }
    if(c <          0x800)
    {
        first = 0xc0;
        end = start + 2;
    }
    else if(c <   0x10000)
    {
        first = 0xe0;
        end = start + 3;
    }
    else if(c <  0x200000)
    {
        first = 0xf0;
        end = start + 4;
    }
    else if(c < 0x4000000)
    {
        first = 0xf8;
        end = start + 5;
    }
    else
    {
        first = 0xfc;
        end = start + 6;
    }

    p = end;
    while(--p > start)
    {
        *p = (c & 0x3f) | 0x80;
        c >>= 6;
    }
    *start = c | first;
    return end;
}


/*
 * Replace the references with the corresponding actual values.
 */
static void
guththila_token_evaluate_references(
    guththila_token_t * tok)
{
    guththila_char_t *start = tok->start;
    guththila_char_t *end = start + tok->size;
    guththila_char_t *p = start;
    guththila_char_t *q = NULL;
    guththila_char_t *entity = NULL;
    size_t entity_len = 0;

    while(p < end && *p != '&')
    {
        p++;
    }
    q = p;
    while(p < end)
    {
        /* Copy characters until the next ampersand */
        if(*p != '&')
        {
            *q++ = *p++;
            continue;
        }
        entity = ++p;
        /* Find the end of the entity, marked by ';' */
        while(p < end && *p != ';')
        {
            p++;
        }
        if(p == end)
        {
            break; /* Drop unterminated entity */
        }
        entity_len = p - entity;
        if(entity_len == 2 && entity[1] == 't')
        {
            if(entity[0] == 'g')
                *q++ = '>';
            else if(entity[0] == 'l')
                *q++ = '<';
            /* else drop */
        }
        else if(entity_len == 3 && entity[0] == 'a' && entity[1] == 'm' && entity[2] == 'p')
        {
            *q++ = '&';
        }
        else if(entity_len == 4 && entity[2] == 'o')
        {
            if(entity[0] == 'q' && entity[1] == 'u' && entity[3] == 't')
            {
                *q++ = '"';
            }
            else if(entity[0] == 'a' && entity[1] == 'p' && entity[3] == 's')
            {
                *q++ = '\'';
            }
        }
        else if(entity_len >= 2 && entity[0] == '#')
        {
            /* p points to the ';' */
            int c = 0;
            guththila_char_t b;
            guththila_char_t *digit = entity + 1;
            if(*digit == 'x')
            { /* &#x...; */
                while(++digit < p)
                {
                    b = *digit;
                    if(b >= '0' && b <= '9')
                        c = c << 4 | (b - '0');
                    else if(b >= 'A' && b <= 'F')
                        c = c << 4 | (b - 'A' + 10);
                    else if(b >= 'a' && b <= 'f')
                        c = c << 4 | (b - 'a' + 10);
                    else
                        break; /* stop and drop */
                }
            }
            else
            { /* &#...; */
                while(digit < p)
                {
                    b = *digit;
                    if(b >= '0' && b <= '9')
                        c = c * 10 + (b - '0');
                    else
                        break; /* stop and drop */
                    digit++;
                }
            }
            if(digit == p && c != 0)
            { /* drop null char or unparsable entity */
                /* Replace the entity with the UTF-8 representation */
                q = guththila_utf8_bytes(c, q);
            }
        } /* else drop unknown entity */
        p++; /* go over ';' */
    }
    tok->size = q - start;
}

/*
 * Close a token. This method accepts the type of the token as a parameter.
 */
static void
guththila_token_close(
    guththila_t * m,
    guththila_token_t * tok,
    int tok_type,
    int referer,
    const axutil_env_t * env)
{
    guththila_attr_t * attr = NULL;
    guththila_element_t * elem = NULL;
    guththila_elem_namesp_t * e_namesp = NULL;
    guththila_namespace_t * namesp;
    int i = 0;
    /* We are sure that the difference lies within the short range */
    m->temp_tok->type = (short)tok_type;
    m->temp_tok->size = m->next - m->temp_tok->_start;
    m->temp_tok->start = GUTHTHILA_BUF_POS(m->buffer, m->next - 1) - m->temp_tok->size;
    m->temp_tok->ref = referer;
    m->last_start = -1;
    switch(tok_type)
    {
        case _attribute_name:
            m->temp_name = m->temp_tok;
            m->temp_tok = NULL;
            break;
        case _char_data:
            m->value = m->temp_tok;
            m->temp_tok = NULL;
            break;
        case _text_data:
            guththila_token_evaluate_references(m->temp_tok);
            m->value = m->temp_tok;
            m->temp_tok = NULL;
            break;
        case _attribute_value:
            guththila_token_evaluate_references(m->temp_tok);
            /* Chech weather we are at a xml namespace declaration */
            if((m->temp_prefix && (guththila_tok_str_cmp(m->temp_prefix, "xmlns", 5u, env) == 0))
                || (guththila_tok_str_cmp(m->temp_name, "xmlns", 5u, env) == 0))
            /*checks inside the m->temp_name to parse the default namespace*/
            /*checks inside the m->temp_prefix to parse namespace with prefix*/
            {
#ifndef GUTHTHILA_VALIDATION_PARSER
                namesp =
                (guththila_namespace_t *)
                AXIS2_MALLOC(sizeof(guththila_namespace_t));
                GUTHTHILA_NAMESPACE_INITIALIZE(namesp, m->temp_name, m->temp_tok);
                guththila_stack_push(&m->namesp, namesp);
#else   
                elem = (guththila_element_t *)guththila_stack_peek(&m->elem, env);
                /* This is the first namespace */
                if(elem && !elem->is_namesp)
                {
                    e_namesp = (guththila_elem_namesp_t *)AXIS2_MALLOC(env->allocator,
                        sizeof(guththila_elem_namesp_t));
                    if(e_namesp)
                    {
                        e_namesp->namesp = (guththila_namespace_t *)AXIS2_MALLOC(env->allocator,
                            sizeof(guththila_namespace_t) * GUTHTHILA_NAMESPACE_DEF_SIZE);
                        if(e_namesp->namesp)
                        {
                            e_namesp->no = 1;
                            e_namesp->size = GUTHTHILA_NAMESPACE_DEF_SIZE;
                            e_namesp->namesp[0].name = m->temp_name;
                            e_namesp->namesp[0].uri = m->temp_tok;
                            guththila_stack_push(&m->namesp, e_namesp, env);
                            elem->is_namesp = 1;
                        }
                        else
                        {
                            AXIS2_FREE(env->allocator, e_namesp);
                            e_namesp = NULL;
                        }
                    }
                }
                /* Already there is a namespace */
                else if(elem && elem->is_namesp)
                {
                    e_namesp = (guththila_elem_namesp_t *)guththila_stack_peek(&m->namesp, env);
                    /* if we have enough space allocated */
                    if(e_namesp->no < e_namesp->size)
                    {
                        e_namesp->namesp[e_namesp->no].name = m->temp_name;
                        e_namesp->namesp[e_namesp->no].uri = m->temp_tok;
                        e_namesp->no++;
                    }
                    else
                    {
                        namesp = (guththila_namespace_t *)AXIS2_MALLOC(env->allocator,
                            sizeof(guththila_namespace_t) * e_namesp->size * 2);
                        if(namesp)
                        {
                            for(i = 0; i < e_namesp->no; i++)
                            {
                                namesp[i].name = e_namesp->namesp[i].name;
                                namesp[i].uri = e_namesp->namesp[i].uri;
                            }
                            AXIS2_FREE(env->allocator, e_namesp->namesp);
                            e_namesp->namesp = namesp;
                            e_namesp->size *= 2;

                            e_namesp->namesp[e_namesp->no].name = m->temp_name;
                            e_namesp->namesp[e_namesp->no].uri = m->temp_tok;
                            e_namesp->no++;
                        }
                    }
                }
#endif  
            }
            else
            {
                /* It is just a attribute */
                attr = (guththila_attr_t *)AXIS2_MALLOC(env->allocator, sizeof(guththila_attr_t));

                GUTHTHILA_ATTRIBUTE_INITIALIZE(attr,
                    m->temp_prefix,
                    m->temp_name,
                    m->temp_tok);
                guththila_stack_push(&m->attrib, attr, env);
            }
            m->temp_prefix = NULL;
            m->temp_name = NULL;
            m->temp_tok = NULL;
            break;
        case _prefix:
            m->temp_prefix = m->temp_tok;
            m->temp_tok = NULL;
            break;
        default:
            m->prefix = m->temp_prefix;
            m->name = m->temp_tok;
            m->temp_tok = NULL;
            m->temp_prefix = NULL;
            break;
    }
}

int GUTHTHILA_CALL
guththila_validate_namespaces(
    guththila_t *m,
    const axutil_env_t *env)
{
    int size = 0, i = 0, nmsp_no = 0, j = 0, k = 0;
    int namesp_found = GUTHTHILA_FALSE;
    guththila_elem_namesp_t *e_namesp = NULL;

    size = GUTHTHILA_STACK_SIZE(m->attrib);
    /* Loop through all the attributes */
    for(i = 0; i < size; i++)
    {
        guththila_attr_t *attr = (guththila_attr_t *)guththila_stack_get_by_index(&m->attrib, i,
            env);
        if(attr && attr->pref)
        {
            /* We have a attribute prefix. Need to validate the prefix */
            nmsp_no = GUTHTHILA_STACK_SIZE(m->namesp);
            for(j = nmsp_no - 1; j >= 0; j--)
            {
                e_namesp = (guththila_elem_namesp_t *)guththila_stack_get_by_index(&m->namesp, j,
                    env);
                for(k = 0; k < e_namesp->no; k++)
                {
                    if(!guththila_tok_tok_cmp(e_namesp->namesp[k].name, attr->pref, env))
                    {
                        namesp_found = GUTHTHILA_TRUE;
                        j = -1; /* force exit from second for loop */
                        break;
                    }
                }
            }
            if(!namesp_found)
                return GUTHTHILA_FAILURE;
        }
    }
    /* If the element has a prefix. Need to validate the prefix*/
    if(m->prefix)
    {
        namesp_found = AXIS2_FALSE;
        nmsp_no = GUTHTHILA_STACK_SIZE(m->namesp);
        for(j = nmsp_no - 1; j >= 0; j--)
        {
            e_namesp = (guththila_elem_namesp_t *)guththila_stack_get_by_index(&m->namesp, j, env);
            for(k = 0; k < e_namesp->no; k++)
            {
                if(!guththila_tok_tok_cmp(e_namesp->namesp[k].name, m->prefix, env))
                {
                    namesp_found = GUTHTHILA_TRUE;
                    j = -1; /* force exit from outer loop */
                    break;
                }
            }
        }
        if(!namesp_found)
            return AXIS2_FAILURE;
    }
    return GUTHTHILA_SUCCESS;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_next(
    guththila_t * m,
    const axutil_env_t * env)
{
    guththila_element_t * elem = NULL;
    guththila_elem_namesp_t * nmsp = NULL;
    guththila_token_t * tok = NULL;
    int quote = 0, ref = 0;
    int c = -1;
    guththila_attr_t * attr = NULL;
    int size = 0, i = 0, nmsp_counter, loop = 0, white_space = 0;
    size_t data_size = -1;
    size_t previous_size = -1;
    guththila_char_t *buffer = NULL;

    /* Need to release the resources for attributes */
    size = GUTHTHILA_STACK_SIZE(m->attrib);
    for(i = 0; i < size; i++)
    {
        attr = (guththila_attr_t *)guththila_stack_pop(&m->attrib, env);
        if(attr)
        {
            if(attr->name)
                guththila_tok_list_release_token(&m->tokens, attr->name, env);
            if(attr->pref)
                guththila_tok_list_release_token(&m->tokens, attr->pref, env);
            AXIS2_FREE(env->allocator, attr);
        }
    }

#ifdef GUTHTHILA_VALIDATION_PARSER    
    if(m->guththila_event == GUTHTHILA_END_ELEMENT && m->name)
    {
        guththila_tok_list_release_token(&m->tokens, m->name, env);
        if(m->prefix)
        {
            guththila_tok_list_release_token(&m->tokens, m->prefix, env);
        }
    }
    /* If the previous event was a empty element we need to do some clean up */
    else if(m->guththila_event == GUTHTHILA_EMPTY_ELEMENT)
    {
        elem = (guththila_element_t *)guththila_stack_pop(&m->elem, env);
        if(elem->is_namesp)
        {
            nmsp = (guththila_elem_namesp_t *)guththila_stack_pop(&m->namesp, env);
            for(nmsp_counter = 0; nmsp_counter < nmsp->no; nmsp_counter++)
            {
                if(nmsp->namesp[nmsp_counter].name)
                    guththila_tok_list_release_token(&m->tokens, nmsp->namesp[nmsp_counter]. name,
                        env);
                if(nmsp->namesp[nmsp_counter].uri)
                    guththila_tok_list_release_token(&m->tokens, nmsp->namesp[nmsp_counter]. uri,
                        env);
            }
            AXIS2_FREE(env->allocator, nmsp->namesp);
            AXIS2_FREE(env->allocator, nmsp);
        }
        if(elem->name)
            guththila_tok_list_release_token(&m->tokens, elem->name, env);
        if(elem->prefix)
            guththila_tok_list_release_token(&m->tokens, elem->prefix, env);
        AXIS2_FREE(env->allocator, elem);
    }
    GUTHTHILA_VARIABLE_INITIALZE(m);
#endif  
    /* Actual XML parsing logic */
    do
    {
        loop = 0;
        GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
        if(m->status == S_1)
        {
            while(isspace(c))
            {
                GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
            }
            if('<' == c)
            {
                m->status = S_2;
            }
            else
            {
                return -1;
            }
        }
        if(m->status != S_2)
        {
            return -1;
        }
        if(c == '<')
        {
            GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
            if(c != '?' && c != '!' && c != '/')
            {
                /* We are at the beginning of a xml element */
                if(GUTHTHILA_IS_VALID_STARTING_CHAR(c))
                {
                    GUTHTHILA_TOKEN_OPEN(m, tok, env);
                    GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                    while(!GUTHTHILA_IS_SPACE(c) && c != '>' && c != '/')
                    {
                        if(c != ':')
                        {
                            GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                        }
                        else
                        {
                            /* We know for sure that this is a prefix */
                            guththila_token_close(m, tok, _prefix, 0, env);
                            GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                            GUTHTHILA_TOKEN_OPEN(m, tok, env);
                        }
                    }
                    /* XML element name */
                    guththila_token_close(m, tok, _name, 0, env);
#ifdef GUTHTHILA_VALIDATION_PARSER		
                    elem = (guththila_element_t *)AXIS2_MALLOC(env->allocator,
                        sizeof(guththila_element_t));
                    elem->name = m->name;
                    elem->prefix = m->prefix;
                    elem->is_namesp = 0;
                    guththila_stack_push(&m->elem, elem, env);
#endif  
                }
                GUTHTHILA_SKIP_SPACES(m, c, buffer, data_size, previous_size, env);
                /* Process the attributes */
                for(;;)
                {
                    /* Empty element */
                    if(c == '/')
                    {
                        GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                        if(c == '>')
                        {
                            m->guththila_event = GUTHTHILA_EMPTY_ELEMENT;
                            if(!guththila_validate_namespaces(m, env))
                                return -1;
                            else
                                return GUTHTHILA_EMPTY_ELEMENT;
                        }
                        else
                        {
                            return -1;
                        }
                    }
                    /* Normal element */
                    else if(c == '>')
                    {
                        m->guththila_event = GUTHTHILA_START_ELEMENT;
                        if(!guththila_validate_namespaces(m, env))
                            return -1;
                        else
                            return GUTHTHILA_START_ELEMENT;
                    }
                    /* We are in the middle of a element */
                    else
                    {
                        /* Process the attributes */
                        if(GUTHTHILA_IS_VALID_STARTING_CHAR(c))
                        {
                            GUTHTHILA_TOKEN_OPEN(m, tok, env);

                            GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                            while(!GUTHTHILA_IS_SPACE(c) && c != '=')
                            {
                                if(c != ':')
                                {
                                    GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                                }
                                else if(c == ':')
                                {
                                    /* Prefix */
                                    guththila_token_close(m, tok, _prefix, 0, env);
                                    GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                                    GUTHTHILA_TOKEN_OPEN(m, tok, env);
                                }
                            }
                            /* Attribute name*/
                            guththila_token_close(m, tok, _attribute_name, 0, env);
                        }
                        else
                        {
                            return -1;
                        }
                        /* Attribute Value */
                        GUTHTHILA_PROCESS_EQU(m, c, quote, buffer, data_size, previous_size, env);
                        if(quote == '\'' || quote == '\"')
                        {
                            GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                            GUTHTHILA_TOKEN_OPEN(m, tok, env);
                            while(c != quote)
                            {
                                GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                            }
                            guththila_token_close(m, tok, _attribute_value, 0, env);
                            GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                            GUTHTHILA_SKIP_SPACES(m, c, buffer, data_size, previous_size, env);
                        }
                        else
                        {
                            return -1;
                        }
                    }
                } /* for(;;) */
            }
            else if(c == '/')
            {
                /* End Element */
                m->guththila_event = GUTHTHILA_END_ELEMENT;
                GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                if(GUTHTHILA_IS_VALID_STARTING_CHAR(c))
                {
                    GUTHTHILA_TOKEN_OPEN(m, tok, env);
                    GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                    while(!GUTHTHILA_IS_SPACE(c) && c != '>')
                    {
                        if(c != ':')
                        {
                            GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                        }
                        else
                        {
                            /* Prefix */
                            guththila_token_close(m, tok, _prefix, 0, env);
                            GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                            GUTHTHILA_TOKEN_OPEN(m, tok, env);
                        }
                    }
                    /* name */
                    guththila_token_close(m, tok, _name, 0, env);

#ifdef GUTHTHILA_VALIDATION_PARSER	
                    elem = (guththila_element_t *)guththila_stack_pop(&m->elem, env);
                    if(!elem || (!elem->prefix && m->prefix) || (elem->prefix && !m->prefix))
                        return -1;
                    if(guththila_tok_tok_cmp(m->name, elem->name, env))
                    {
                        return -1;
                    }
                    if(elem->prefix && m->prefix && guththila_tok_tok_cmp(m->prefix, elem->prefix,
                        env))
                    {
                        return -1;
                    }
                    /* Releasing the namespace related resources */
                    if(elem->is_namesp)
                    {
                        nmsp = (guththila_elem_namesp_t *)guththila_stack_pop(&m->namesp, env);
                        for(nmsp_counter = 0; nmsp_counter < nmsp->no; nmsp_counter++)
                        {
                            if(nmsp->namesp[nmsp_counter].name)
                                guththila_tok_list_release_token(&m->tokens,
                                    nmsp-> namesp[nmsp_counter]. name, env);
                            if(nmsp->namesp[nmsp_counter].uri)
                                guththila_tok_list_release_token(&m->tokens,
                                    nmsp-> namesp[nmsp_counter]. uri, env);
                        }
                        AXIS2_FREE(env->allocator, nmsp->namesp);
                        AXIS2_FREE(env->allocator, nmsp);
                    }
                    /* Release the tokens */
                    if(elem->name)
                        guththila_tok_list_release_token(&m->tokens, elem->name, env);
                    if(elem->prefix)
                        guththila_tok_list_release_token(&m->tokens, elem->prefix, env);
                    AXIS2_FREE(env->allocator, elem);
#endif  
                    GUTHTHILA_SKIP_SPACES(m, c, buffer, data_size, previous_size, env);
                    if(c != '>')
                        return -1;
                    return GUTHTHILA_END_ELEMENT;
                }
                return -1;
            }
            else if(c == '!')
            {
                /* Comment */
                guththila_char_t c_arra[16] = { 0 };
                if(2 == guththila_next_no_char(m, 0, c_arra, 2, env) && '-' == c_arra[0] && '-'
                    == c_arra[1])
                {
                    int loop_state = 1;
                    GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                    GUTHTHILA_TOKEN_OPEN(m, tok, env);
                    while(loop_state)
                    {
                        GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                        if('-' == c)
                        {
                            if(2 == guththila_next_no_char(m, 0, c_arra, 2, env) && '-'
                                == c_arra[0])
                            {
                                if('>' == c_arra[1])
                                {
                                    m->guththila_event = GUTHTHILA_COMMENT;
                                    /* position after first hyphen, as if we just scanned it */
                                    m->next = m->next - 2;
                                    guththila_token_close(m, tok, _char_data, 0, env);
                                    m->next = m->next + 2;
                                    return GUTHTHILA_COMMENT;
                                }
                                else
                                {
                                    return -1;
                                }
                            }
                        }
                    }
                }
                else
                {
                    GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                    while('<' != c)
                    {
                        GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                    }
                }
            }
            else if(c == '?')
            {
                /* XML declaration */
                c = guththila_process_xml_dec(m, env);
                if(c != -1)
                    return GUTHTHILA_START_DOCUMENT;
                else
                    return -1;
            }
        }
        else
        {
            /* Text */
            m->guththila_event = GUTHTHILA_CHARACTER;
            if(!GUTHTHILA_IS_SPACE(c))
                white_space = 0;
            else
                white_space = 1;
            GUTHTHILA_TOKEN_OPEN(m, tok, env);

            /* code given below is having two do-while loop wrapped by another do-while loop.
             * This is done to improve the performance for big messages. Most of the cases, the
             * content will be not whitespace, so checking for whitespace even if we already found
             * that we have some valid characters is big overhead. Hence better to do the looping
             * separately to find white_space = false and normal case
             */
            do
            {
                if(white_space)
                {
                    do
                    {
                        GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                        if(!GUTHTHILA_IS_SPACE(c) && c != '<')
                        {
                            white_space = 0;
                            break;
                        }
                    }
                    while(c != '<');
                }
                else
                {
                    do
                    {
                        if(buffer)
                        {
                            guththila_char_t *pos = NULL;
                            size_t index = m->next - previous_size;
                            pos = (guththila_char_t*)memchr(buffer + index, '<', data_size - index);
                            if(pos)
                            {
                                m->next += pos - (buffer + index);
                            }
                            else
                            {
                                m->next = previous_size + data_size;
                            }

                            GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                        }
                        else
                        {
                            GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                        }
                    }while(c != '<');
                }
            }
            while(c != '<');
            guththila_token_close(m, tok, _text_data, ref, env);
            m->next--;
            if(white_space)
            {
#ifndef GUTHTHILA_IGNORE_SPACES
                m->guththila_event = GUTHTHILA_SPACE;
                return GUTHTHILA_SPACE;
#else
                loop = 1;
                if (m->value)
                {
                    guththila_tok_list_release_token(&m->tokens, m->value,
                        env);
                    m->value = NULL;
                }
#endif
            }

            else
                return GUTHTHILA_CHARACTER;
        }
    }
    while(loop);
    return c;
}

/* Process the XML declaration */
static int
guththila_process_xml_dec(
    guththila_t * m,
    const axutil_env_t * env)
{
    guththila_token_t * tok = NULL;
    guththila_char_t c_arra[16] = { 0 };
    int c = -1;
    int quote = -1;
    int nc = -1;
    size_t data_size = -1;
    size_t previous_size = -1;
    guththila_char_t *buffer = NULL;
    if(3 == guththila_next_no_char(m, GUTHTHILA_EOF, c_arra, 3, env) && 'x' == c_arra[0] && 'm'
        == c_arra[1] && 'l' == c_arra[2])
    {
        GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
        GUTHTHILA_SKIP_SPACES(m, c, buffer, data_size, previous_size, env);
        if(c == 'v')
        {
            GUTHTHILA_TOKEN_OPEN(m, tok, env);
            if(6 == guththila_next_no_char(m, 0, c_arra, 6, env) && 'e' == c_arra[0] && 'r'
                == c_arra[1] && 's' == c_arra[2] && 'i' == c_arra[3] && 'o' == c_arra[4] && 'n'
                == c_arra[5])
            {
                GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                guththila_token_close(m, tok, _attribute_name, 0, env);
                GUTHTHILA_PROCESS_EQU(m, c, quote, buffer, data_size, previous_size, env);
                GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, nc);
                GUTHTHILA_TOKEN_OPEN(m, tok, env);
                while(nc != quote)
                {
                    GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, nc);
                }
                guththila_token_close(m, tok, _attribute_value, 0, env);
                GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                GUTHTHILA_SKIP_SPACES(m, c, buffer, data_size, previous_size, env);
            }
            else
            {
                return -1;
            }
        }
        if(c == 'e')
        {
            GUTHTHILA_TOKEN_OPEN(m, tok, env);
            if(7 == guththila_next_no_char(m, 0, c_arra, 7, env) && 'n' == c_arra[0] && 'c'
                == c_arra[1] && 'o' == c_arra[2] && 'd' == c_arra[3] && 'i' == c_arra[4] && 'n'
                == c_arra[5] && 'g' == c_arra[6])
            {
                GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                guththila_token_close(m, tok, _attribute_name, 0, env);
                GUTHTHILA_PROCESS_EQU(m, c, quote, buffer, data_size, previous_size, env);
                GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, nc);
                GUTHTHILA_TOKEN_OPEN(m, tok, env);
                while(nc != quote)
                {
                    GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, nc);
                }
                guththila_token_close(m, tok, _attribute_value, 0, env);
                GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                GUTHTHILA_SKIP_SPACES(m, c, buffer, data_size, previous_size, env);
            }
        }
        if(c == 's')
        {
            GUTHTHILA_TOKEN_OPEN(m, tok, env);
            if(9 == guththila_next_no_char(m, 0, c_arra, 9, env) && 't' == c_arra[0] && 'a'
                == c_arra[1] && 'n' == c_arra[2] && 'd' == c_arra[3] && 'a' == c_arra[4] && 'l'
                == c_arra[5] && 'o' == c_arra[6] && 'n' == c_arra[7] && 'e' == c_arra[8])
            {
                GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                guththila_token_close(m, tok, _attribute_name, 0, env);
                GUTHTHILA_PROCESS_EQU(m, c, quote, buffer, data_size, previous_size, env);
                GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, nc);
                GUTHTHILA_TOKEN_OPEN(m, tok, env);
                while(nc != quote)
                {
                    GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, nc);
                }
                guththila_token_close(m, tok, _attribute_value, 0, env);
                GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, c);
                GUTHTHILA_SKIP_SPACES(m, c, buffer, data_size, previous_size, env);
            }
        }
        if(c == '?')
        {
            int nc;
            GUTHTHILA_NEXT_CHAR(m, buffer, data_size, previous_size, env, nc);
            if('>' == nc)
            {
                m->guththila_event = GUTHTHILA_START_DOCUMENT;
            }
            else
            {
                return -1;
            }
        }
    }
    return c;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_get_attribute_count(
    guththila_t * m,
    const axutil_env_t * env)
{
    return GUTHTHILA_STACK_SIZE(m->attrib);
}

GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL
guththila_get_attribute_name(
    guththila_t * m,
    guththila_attr_t * att,
    const axutil_env_t * env)
{
    guththila_char_t *str = NULL;
    if(att->name)
    {
        GUTHTHILA_TOKEN_TO_STRING(att->name, str, env);
        return str;
    }
    return NULL;
}

GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL
guththila_get_attribute_value(
    guththila_t * m,
    guththila_attr_t * att,
    const axutil_env_t * env)
{
    guththila_char_t *str = NULL;
    if(att->val)
    {
        GUTHTHILA_TOKEN_TO_STRING(att->val, str, env);
        return str;
    }
    return NULL;
}

GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL
guththila_get_attribute_prefix(
    guththila_t * m,
    guththila_attr_t * att,
    const axutil_env_t * env)
{
    guththila_char_t *str = NULL;
    if(att->pref)
    {
        GUTHTHILA_TOKEN_TO_STRING(att->pref, str, env);
        return str;
    }
    return NULL;
}

GUTHTHILA_EXPORT guththila_attr_t *GUTHTHILA_CALL
guththila_get_attribute(
    guththila_t * m,
    const axutil_env_t * env)
{
    return (guththila_attr_t *)guththila_stack_pop(&m->attrib, env);
}

GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL
guththila_get_attribute_name_by_number(
    guththila_t * m,
    int i,
    const axutil_env_t *env)
{
    guththila_char_t *str = NULL;
    guththila_attr_t * attr = (guththila_attr_t *)guththila_stack_get_by_index(&m->attrib, i - 1,
        env);
    if(attr->name)
    {
        GUTHTHILA_TOKEN_TO_STRING(attr->name, str, env);
        return str;
    }
    return NULL;
}

GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL
guththila_get_attribute_value_by_number(
    guththila_t * m,
    int i,
    const axutil_env_t *env)
{
    guththila_char_t *str = NULL;
    guththila_attr_t * attr = (guththila_attr_t *)guththila_stack_get_by_index(&m->attrib, i - 1,
        env);
    if(attr->val)
    {
        GUTHTHILA_TOKEN_TO_STRING(attr->val, str, env);
        return str;
    }
    return NULL;
}

GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL
guththila_get_attribute_prefix_by_number(
    guththila_t * m,
    int i,
    const axutil_env_t *env)
{
    guththila_char_t *str = NULL;
    guththila_attr_t * attr = (guththila_attr_t *)guththila_stack_get_by_index(&m->attrib, i - 1,
        env);
    if(attr && attr->pref)
    {
        GUTHTHILA_TOKEN_TO_STRING(attr->pref, str, env);
        return str;
    }
    return NULL;
}

GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL
guththila_get_name(
    guththila_t * m,
    const axutil_env_t * env)
{
    guththila_char_t *str = NULL;
    if(m->name)
    {
        GUTHTHILA_TOKEN_TO_STRING(m->name, str, env);
        return str;
    }
    return NULL;
}

GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL
guththila_get_prefix(
    guththila_t * m,
    const axutil_env_t * env)
{
    guththila_char_t *str = NULL;
    if(m->prefix)
    {
        GUTHTHILA_TOKEN_TO_STRING(m->prefix, str, env);
        return str;
    }
    return NULL;
}

GUTHTHILA_EXPORT guththila_char_t * GUTHTHILA_CALL
guththila_get_value(
    guththila_t * m,
    const axutil_env_t * env)
{
    guththila_char_t *str = NULL;
    if(m->value)
    {
        GUTHTHILA_TOKEN_TO_STRING(m->value, str, env);
        return str;
    }
    return NULL;
}

GUTHTHILA_EXPORT guththila_namespace_t *GUTHTHILA_CALL
guththila_get_namespace(
    guththila_t * m,
    const axutil_env_t * env)
{

#ifndef GUTHTHILA_VALIDATION_PARSER
    return (guththila_namespace_t *) guththila_stack_pop(&m->namesp, env);
#else  
    return NULL;
#endif  
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_get_namespace_count(
    guththila_t * m,
    const axutil_env_t * env)
{

#ifndef GUTHTHILA_VALIDATION_PARSER
    return GUTHTHILA_STACK_SIZE(m->namesp);
#else   
    guththila_elem_namesp_t * nmsp = NULL;
    if(((guththila_element_t *)guththila_stack_peek(&m->elem, env))->is_namesp)
    {
        nmsp = (guththila_elem_namesp_t *)guththila_stack_peek(&m->namesp, env);
        return nmsp->no;
    }
    return 0;

#endif  
}

GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL
guththila_get_namespace_uri(
    guththila_t * m,
    guththila_namespace_t * ns,
    const axutil_env_t * env)
{
    guththila_char_t *str = NULL;
    if(ns->uri)
    {
        GUTHTHILA_TOKEN_TO_STRING(ns->uri, str, env);
        return str;
    }
    return NULL;
}

GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL
guththila_get_namespace_prefix(
    guththila_t * m,
    guththila_namespace_t * ns,
    const axutil_env_t * env)
{
    guththila_char_t *str = NULL;
    if(ns->name)
    {
        GUTHTHILA_TOKEN_TO_STRING(ns->name, str, env);
        return str;
    }
    return NULL;
}

GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL
guththila_get_namespace_prefix_by_number(
    guththila_t * m,
    int i,
    const axutil_env_t *env)
{
    guththila_char_t *str = NULL;

#ifndef GUTHTHILA_VALIDATION_PARSER
    if (GUTHTHILA_STACK_SIZE(m->namesp) >= i)
    {
        namesp = guththila_stack_get_by_index(&m->namesp, i - 1, env);
        if (namesp && namesp->name)
        {
            GUTHTHILA_TOKEN_TO_STRING(namesp->name, str, env);
            return str;
        }
    }

#else   
    guththila_elem_namesp_t * nmsp = NULL;
    if(((guththila_element_t *)guththila_stack_peek(&m->elem, env))->is_namesp)
    {
        nmsp = (guththila_elem_namesp_t *)guththila_stack_peek(&m->namesp, env);
        if(nmsp && nmsp->no >= i)
        {
            GUTHTHILA_TOKEN_TO_STRING(nmsp->namesp[i - 1].name, str, env);
            return str;
        }
    }
#endif  /*  */
    return NULL;
}

GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL
guththila_get_namespace_uri_by_number(
    guththila_t * m,
    int i,
    const axutil_env_t *env)
{
    guththila_char_t *str = NULL;
#ifndef GUTHTHILA_VALIDATION_PARSER
    if (GUTHTHILA_STACK_SIZE(m->namesp) >= i)
    {
        namesp = guththila_stack_get_by_index(&m->namesp, i - 1, env);
        if (namesp && namesp->uri)
        {
            GUTHTHILA_TOKEN_TO_STRING(namesp->uri, str, env);
            return str;
        }
    }
#else  
    guththila_elem_namesp_t * nmsp = NULL;
    if(((guththila_element_t *)guththila_stack_peek(&m->elem, env))->is_namesp)
    {
        nmsp = (guththila_elem_namesp_t *)guththila_stack_peek(&m->namesp, env);
        if(nmsp && nmsp->no >= i)
        {
            GUTHTHILA_TOKEN_TO_STRING(nmsp->namesp[i - 1].uri, str, env);
            return str;
        }
    }
#endif  
    return NULL;
}

GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL
guththila_get_attribute_namespace_by_number(
    guththila_t * m,
    int i,
    const axutil_env_t *env)
{

#ifndef GUTHTHILA_VALIDATION_PARSER
    return NULL;

#else  
    guththila_attr_t * attr = NULL;
    guththila_char_t* str = NULL;
    int j = 0, k = 0, count = 0;
    guththila_elem_namesp_t * nmsp = NULL;
    if(i <= GUTHTHILA_STACK_SIZE(m->attrib))
    {
        attr = (guththila_attr_t *)guththila_stack_get_by_index(&m->attrib, i - 1, env);
        if(attr && attr->pref)
        {
            count = GUTHTHILA_STACK_SIZE(m->namesp);
            for(j = count - 1; j >= 0; j--)
            {
                nmsp = (guththila_elem_namesp_t *)guththila_stack_get_by_index(&m->namesp, j, env);
                for(k = 0; k < nmsp->no; k++)
                {
                    if(!guththila_tok_tok_cmp(nmsp->namesp[k].name, attr->pref, env))
                    {
                        GUTHTHILA_TOKEN_TO_STRING(nmsp->namesp[k].uri, str, env);
                        return str;
                    }
                }
            }
        }
    }
    return NULL;

#endif 
}

GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL
guththila_get_encoding(
    guththila_t * m,
    const axutil_env_t * env)
{
    return "UTF-8";
}

/* Return the next character */
static int
guththila_next_char(
    guththila_t * m,
    const axutil_env_t * env)
{
    int c;
    size_t data_move, i;
    int temp;
    guththila_char_t **temp1;
    size_t * temp2, *temp3;

    /* we have a buffered reader. Easiest case just fetch the character from 
     * the buffer. Here we have a single buffer. 
     * */
    if(m->reader->type == GUTHTHILA_MEMORY_READER)
    {
        size_t index = m->next++;
        if(index < m->buffer.data_size[0])
        {
            return m->buffer.buff[0][index];
        }
    }
    else
    {
        /* comlex stuff. We have a array of buffers */
        if(m->buffer.cur_buff != -1 && m->next < GUTHTHILA_BUFFER_PRE_DATA_SIZE(m->buffer)
            + GUTHTHILA_BUFFER_CURRENT_DATA_SIZE(m->buffer))
        {
            /* What we are looking for is already in the buffer */
            c = m->buffer.buff[m->buffer.cur_buff][m->next++ - GUTHTHILA_BUFFER_PRE_DATA_SIZE(
                m->buffer)];
            return c;
        }
        else if(m->buffer.cur_buff != -1 && m->next >= GUTHTHILA_BUFFER_PRE_DATA_SIZE(m->buffer)
            + GUTHTHILA_BUFFER_CURRENT_DATA_SIZE(m->buffer))
        {
            /* if we have don't have enough space in current buffer, have to create new buffer */
            if(m->buffer.buffs_size[m->buffer.cur_buff] <
                GUTHTHILA_BUFFER_CURRENT_DATA_SIZE(m->buffer) + GUTHTHILA_BUFFER_DEF_MIN_SIZE)
            {
                if(m->buffer.cur_buff == (int)m->buffer.no_buffers - 1)
                {
                    /* we are out of allocated buffers. Need to allocate more buffers */
                    temp = m->buffer.no_buffers * 2;
                    temp1 = (guththila_char_t **)AXIS2_MALLOC(env->allocator,
                        sizeof(guththila_char_t *) * temp);
                    temp2 = (size_t *)AXIS2_MALLOC(env->allocator, sizeof(size_t) * temp);
                    temp3 = (size_t *)AXIS2_MALLOC(env->allocator, sizeof(size_t) * temp);
                    if(!temp1 || !temp2 || !temp3)
                        return (-1);
                    for(i = 0; i < m->buffer.no_buffers; i++)
                    {
                        temp1[i] = m->buffer.buff[i];
                        temp2[i] = m->buffer.buffs_size[i];
                        temp3[i] = m->buffer.data_size[i];
                    }
                    AXIS2_FREE(env->allocator, m->buffer.buff);
                    AXIS2_FREE(env->allocator, m->buffer.data_size);
                    AXIS2_FREE(env->allocator, m->buffer.buffs_size);
                    m->buffer.buff = temp1;
                    m->buffer.buffs_size = temp2;
                    m->buffer.data_size = temp3;
                    m->buffer.no_buffers *= 2;
                }
                m->buffer.buff[m->buffer.cur_buff + 1] = (guththila_char_t *)AXIS2_MALLOC(
                    env->allocator,
                    sizeof(guththila_char_t) * m->buffer.buffs_size[m->buffer.cur_buff] * 2);
                if(!m->buffer.buff[m->buffer.cur_buff + 1])
                    return -1;
                m->buffer.cur_buff++;
                m->buffer.buffs_size[m->buffer.cur_buff] =
                    m->buffer.buffs_size[m->buffer.cur_buff - 1] * 2;
                m->buffer.data_size[m->buffer.cur_buff] = 0;
                /* We need to have the content for one token in a single buffer.
                 * So if the space is not sufficient we have to move first part
                 * of the token to the next buffer */
                if(m->last_start != -1)
                {
                    data_move = m->buffer.data_size[m->buffer.cur_buff - 1] -
                        (m->last_start - m->buffer.pre_tot_data);
                    memcpy(m->buffer.buff[m->buffer.cur_buff], m->buffer.buff[m->buffer.cur_buff - 1]
                        + m->buffer.data_size[m->buffer.cur_buff - 1] - data_move, data_move);
                    m->buffer.data_size[m->buffer.cur_buff - 1] -= data_move;
                    m->buffer.data_size[m->buffer.cur_buff] += data_move;
                }
                m->buffer.pre_tot_data += m->buffer.data_size[m->buffer.cur_buff - 1];
            }
            temp = guththila_reader_read(m->reader, GUTHTHILA_BUFFER_CURRENT_BUFF(m->buffer), 0,
                (int)GUTHTHILA_BUFFER_CURRENT_BUFF_SIZE(m->buffer), env);
            if(temp > 0)
            {
                m->buffer.data_size[m->buffer.cur_buff] += temp;
            }
            else
            {
                return -1;
            }
            c = m->buffer.buff[m->buffer.cur_buff][m->next++ - GUTHTHILA_BUFFER_PRE_DATA_SIZE(
                m->buffer)];
            return c;
        }
        /* Initial stage. We dont' have the array of buffers allocated*/
        else if(m->buffer.cur_buff == -1)
        {
            m->buffer.buff[0] = (guththila_char_t *)AXIS2_MALLOC(env->allocator,
                sizeof(guththila_char_t) * GUTHTHILA_BUFFER_DEF_SIZE);
            m->buffer.buffs_size[0] = GUTHTHILA_BUFFER_DEF_SIZE;
            m->buffer.cur_buff = 0;
            temp = guththila_reader_read(m->reader, m->buffer.buff[0], 0,
                GUTHTHILA_BUFFER_DEF_SIZE, env);
            m->buffer.data_size[0] = temp;
            c = m->buffer.buff[0][m->next++];
            return c;
        }
    }
    return -1;
}

/* Same functionality as the guththila_next_char. But insted of reading 
 * one character this function reads several characters at once 
 * */
static int
guththila_next_no_char(
    guththila_t * m,
    int eof,
    guththila_char_t *bytes,
    size_t no,
    const axutil_env_t * env)
{
    int temp, data_move;
    size_t i;
    guththila_char_t **temp1;
    size_t * temp2, *temp3;
    if(m->reader->type == GUTHTHILA_MEMORY_READER && m->next + no - 1
        < GUTHTHILA_BUFFER_CURRENT_DATA_SIZE(m->buffer) && m->buffer.cur_buff != -1)
    {
        for(i = 0; i < no; i++)
        {
            bytes[i] = m->buffer.buff[0][m->next++];
        }
        return (int)no;
        /* We are sure that the difference lies within the int range */
    }
    else if(m->reader->type == GUTHTHILA_IO_READER || m->reader->type == GUTHTHILA_FILE_READER)
    {
        if(m->next < GUTHTHILA_BUFFER_PRE_DATA_SIZE(m->buffer)
            + GUTHTHILA_BUFFER_CURRENT_DATA_SIZE(m->buffer) + no && m->buffer.cur_buff != -1)
        {
            for(i = 0; i < no; i++)
            {
                bytes[i] = m->buffer.buff[m->buffer.cur_buff][m->next++
                    - GUTHTHILA_BUFFER_PRE_DATA_SIZE(m->buffer)];
            }
            return (int)no;
            /* We are sure that the difference lies within the int range */
        }
        else if(m->next >= GUTHTHILA_BUFFER_PRE_DATA_SIZE(m->buffer)
            + GUTHTHILA_BUFFER_CURRENT_DATA_SIZE(m->buffer) + no && m->buffer.cur_buff != -1)
        {
            /* We are sure that the difference lies within the int range */
            if(m->buffer.cur_buff == (int)m->buffer.no_buffers - 1)
            {
                temp = m->buffer.no_buffers * 2;
                temp1 = (guththila_char_t **)AXIS2_MALLOC(env->allocator,
                    sizeof(guththila_char_t *) * temp);
                temp2 = (size_t *)AXIS2_MALLOC(env->allocator, sizeof(size_t) * temp);
                temp3 = (size_t *)AXIS2_MALLOC(env->allocator, sizeof(size_t) * temp);
                if(!temp1 || !temp2 || !temp3)
                    return (-1);
                for(i = 0; i < m->buffer.no_buffers; i++)
                {
                    temp1[i] = m->buffer.buff[i];
                    temp2[i] = m->buffer.buffs_size[i];
                    temp3[i] = m->buffer.data_size[i];
                }
                AXIS2_FREE(env->allocator, m->buffer.buff);
                AXIS2_FREE(env->allocator, m->buffer.data_size);
                AXIS2_FREE(env->allocator, m->buffer.buffs_size);
                m->buffer.buff = temp1;
                m->buffer.buffs_size = temp2;
                m->buffer.data_size = temp3;
                m->buffer.no_buffers *= 2;
            }
            m->buffer.buff[m->buffer.cur_buff + 1] = (guththila_char_t *)AXIS2_MALLOC(
                env->allocator, sizeof(guththila_char_t) * m->buffer.data_size[m->buffer.cur_buff]
                    * 2);
            if(!m->buffer.buff[m->buffer.cur_buff + 1])
                return -1;
            m->buffer.cur_buff++;
            m->buffer.buffs_size[m->buffer.cur_buff] = m->buffer.buffs_size[m->buffer.cur_buff - 1]
                * 2;
            m->buffer.data_size[m->buffer.cur_buff] = 0;
            data_move = (int)m->next;
            /* We are sure that the difference lies within the int range */
            if((m->last_start != -1) && (m->last_start < data_move))
                data_move = m->last_start;
            data_move = (int)m->buffer.data_size[m->buffer.cur_buff - 1] - (data_move
                - (int)m->buffer.pre_tot_data);
            /* We are sure that the difference lies within the int range */
            if(data_move)
            {
                memcpy(m->buffer.buff[m->buffer.cur_buff], m->buffer.buff[m->buffer.cur_buff - 1]
                    + m->buffer.data_size[m->buffer.cur_buff - 1] - data_move, data_move);
                m->buffer.data_size[m->buffer.cur_buff - 1] -= data_move;
                m->buffer.data_size[m->buffer.cur_buff] += data_move;
            }
            m->buffer.pre_tot_data += m->buffer.data_size[m->buffer.cur_buff - 1];
            temp = guththila_reader_read(m->reader, GUTHTHILA_BUFFER_CURRENT_BUFF(m->buffer), 0,
                (int)GUTHTHILA_BUFFER_CURRENT_BUFF_SIZE(m-> buffer), env);
            /* We are sure that the difference lies within the int range */
            if(temp > 0)
            {
                m->buffer.data_size[m->buffer.cur_buff] += temp;
            }
            else
            {
                return -1;
            }
            for(i = 0; i < no; i++)
            {
                bytes[i] = m->buffer.buff[m->buffer.cur_buff][m->next++
                    - GUTHTHILA_BUFFER_PRE_DATA_SIZE(m->buffer)];
            }
            return (int)no;
            /* We are sure that the difference lies within the int range */
        }
        else if(m->buffer.cur_buff == -1)
        {
            m->buffer.buff[0] = (guththila_char_t *)AXIS2_MALLOC(env->allocator,
                sizeof(guththila_char_t) * GUTHTHILA_BUFFER_DEF_SIZE);
            m->buffer.buffs_size[0] = GUTHTHILA_BUFFER_DEF_SIZE;
            m->buffer.cur_buff = 0;
            temp = guththila_reader_read(m->reader, m->buffer.buff[0], 0,
                GUTHTHILA_BUFFER_DEF_SIZE, env);
            m->buffer.data_size[0] = temp;
            for(i = 0; i < no; i++)
            {
                bytes[i] = m->buffer.buff[m->buffer.cur_buff][m->next++
                    - GUTHTHILA_BUFFER_PRE_DATA_SIZE(m->buffer)];
            }
            return (int)no;
            /* We are sure that the difference lies within the int range */
        }
    }
    return -1;
}

GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL
guththila_get_current_buffer(
    guththila_t * m,
    const axutil_env_t * env)
{
    return guththila_buffer_get(&m->buffer, env);
}

