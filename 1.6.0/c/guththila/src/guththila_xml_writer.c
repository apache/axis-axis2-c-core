
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
    
#include <guththila_xml_writer.h>
    
#define GUTHTHILA_WRITER_SD_DECLARATION  "<?xml version=\"1.0\" encoding=\"utf-8\" ?>"
    
#ifndef GUTHTHILA_XML_WRITER_TOKEN
#ifndef GUTHTHILA_WRITER_ELEM_FREE
#define GUTHTHILA_WRITER_ELEM_FREE(wr, elem, _env)		\
    if ((elem)->prefix) AXIS2_FREE(env->allocator, (elem)->prefix); \
    if ((elem)->name) AXIS2_FREE(env->allocator, (elem)->name); \
    AXIS2_FREE(env->allocator, elem);
#endif 
#else  
#ifndef GUTHTHILA_WRITER_ELEM_FREE
#define GUTHTHILA_WRITER_ELEM_FREE(wr, elem, _env)		\
    if ((elem)->prefix) guththila_tok_list_release_token(&wr->tok_list, (elem)->prefix, _env); \
    if ((elem)->name) guththila_tok_list_release_token(&wr->tok_list, (elem)->name, _env); \
    AXIS2_FREE(env->allocator, elem);
#endif 
#endif 
    
#ifndef GUTHTHILA_XML_WRITER_TOKEN
#ifndef GUTHTHILA_WRITER_CLEAR_NAMESP 
#define GUTHTHILA_WRITER_CLEAR_NAMESP(wr, stack_namesp, _no, counter, _namesp, j, _env)		\
    for (counter = GUTHTHILA_STACK_TOP_INDEX(*stack_namesp); counter >= _no; counter--) {\
    _namesp = (guththila_xml_writer_namesp_t *) guththila_stack_pop(stack_namesp, _env); \
    if (_namesp) { \
        for (j = 0; j < _namesp->no - 1; j++) { \
            if (_namesp->name[j]) AXIS2_FREE(env->allocator, _namesp->name[j]); \
            if (_namesp->uri[j]) AXIS2_FREE(env->allocator, _namesp->uri[j]); \
        } \
        AXIS2_FREE(env->allocator, _namesp->name); \
        AXIS2_FREE(env->allocator, _namesp->uri); \
        AXIS2_FREE(env->allocator, _namesp); \
    } \
    _namesp = NULL; \
}
#endif 
#else  
#ifndef GUTHTHILA_WRITER_CLEAR_NAMESP 
#define GUTHTHILA_WRITER_CLEAR_NAMESP(wr, stack_namesp, _no, counter, _namesp, j, _env)		\
    for (counter = GUTHTHILA_STACK_TOP_INDEX(*stack_namesp); counter >= _no; counter--) { \
    _namesp = (guththila_xml_writer_namesp_t *) guththila_stack_pop(stack_namesp, _env); \
    if (_namesp) { \
        for (j = 0; j < _namesp->no - 1; j++) { \
            guththila_tok_list_release_token(&wr->tok_list, _namesp->name[j], _env); \
            guththila_tok_list_release_token(&wr->tok_list, _namesp->uri[j], _env); \
        } \
        AXIS2_FREE(env->allocator, _namesp->name); \
        AXIS2_FREE(env->allocator, _namesp->uri); \
        AXIS2_FREE(env->allocator, _namesp); \
    } \
    _namesp = NULL; \
}
#endif 
#endif 
    
#ifndef GUTHTHILA_WRITER_INIT_ELEMENT
#define GUTHTHILA_WRITER_INIT_ELEMENT_WITH_PREFIX(wr, _elem, _name_start, _name_size, _pref_start, _pref_size) \
    _elem->name = guththila_tok_list_get_token(&wr->tok_list); \
    _elem->prefix = = guththila_tok_list_get_token(&wr->tok_list); \
    _elem->name->start = _name_start; \
    _elem->name->size = _name_size; \
    _elem->prefix->start = _pref_start; \
    _elem->prrefix->size = pref_size;
#endif 
    
#ifndef GUTHTHILA_WRITER_INIT_ELEMENT
#define GUTHTHILA_WRITER_INIT_ELEMENT_WITHOUT_PREFIX(wr, _elem, _name_start, _name_size) \
    _elem->name = guththila_tok_list_get_token(&(wr)->tok_list); \
    _elem->name->start = _name_start; \
    _elem->name->size = _name_size; \
    _elem->prefix->NULL;
#endif 
    

/*
#ifndef guththila_write(_wr, _buff, _buff_size)
#define guththila_write(_wr, _buff, _buff_size)	\
	if (_wr->type == GUTHTHILA_WRITER_MEMORY){	\
		if (_wr->buffer.size > _wr->buffer.next + _buff_size) {\
			memcpy (_wr->buffer.buff + _wr->buffer.next, _buff, _buff_size);\
			_wr->buffer.next += (int)_buff_size;			\
		} else {\
		_wr->buffer.buff = realloc(_wr->buffer.buff, _wr->buffer.size * 2);\
		_wr->buffer.size = _wr->buffer.size * 2; \
		memcpy (_wr->buffer.buff + _wr->buffer.next, _buff, _buff_size);\
		_wr->buffer.next += (int)_buff_size;			\
		}\
	} 
#endif*/ 

/* 
 * Write the contents of the buff in to the guththila_xml_writer buffer. 
 * len indicates the number of items in the buff.
 */
int GUTHTHILA_CALL guththila_write(
    guththila_xml_writer_t * wr,
    char *buff,
    size_t buff_size,
    const axutil_env_t * env);

/* 
 * Same functionality as the guththila_write only difference is here we are given 
 * a token to write, not a buffer.
 */
int GUTHTHILA_CALL guththila_write_token(
    guththila_xml_writer_t * wr,
    guththila_token_t * tok,
    const axutil_env_t * env);

int GUTHTHILA_CALL guththila_write_xtoken(
    guththila_xml_writer_t * wr,
    char *buff,
    size_t buff_len,
    const axutil_env_t * env);

/*
 * Private function for free the contents of a empty element.
 */
int GUTHTHILA_CALL guththila_free_empty_element(
        guththila_xml_writer_t *wr,
        const axutil_env_t *env);

GUTHTHILA_EXPORT guththila_xml_writer_t * GUTHTHILA_CALL 
guththila_create_xml_stream_writer(guththila_char_t *file_name,
                                   const axutil_env_t * env) 
{
    guththila_xml_writer_t * wr =
        AXIS2_MALLOC(env->allocator, sizeof(guththila_xml_writer_t));
    if (!wr)
        return NULL;
    wr->out_stream = fopen(file_name, "w");
    if (!wr->out_stream)
    {
        AXIS2_FREE(env->allocator, wr);
        return NULL;
    }
    if (!guththila_stack_init(&wr->element, env))
    {
        fclose(wr->out_stream);
        AXIS2_FREE(env->allocator, wr);
        return NULL;
    }
    if (!guththila_stack_init(&wr->namesp, env))
    {
        guththila_stack_un_init(&wr->element, env);
        fclose(wr->out_stream);
        AXIS2_FREE(env->allocator, wr);
        return NULL;
    }
    wr->type = GUTHTHILA_WRITER_FILE;
    wr->status = BEGINING;
    wr->next = 0;
    return wr;
}

GUTHTHILA_EXPORT guththila_xml_writer_t * GUTHTHILA_CALL 
guththila_create_xml_stream_writer_for_memory(const axutil_env_t * env) 
{
    guththila_xml_writer_t * wr =
        AXIS2_MALLOC(env->allocator, sizeof(guththila_xml_writer_t));
    if (!wr)
        return NULL;
    if (!guththila_buffer_init(&wr->buffer, GUTHTHILA_BUFFER_DEF_SIZE, env))
    {
        AXIS2_FREE(env->allocator, wr);
        return NULL;
    }
    if (!guththila_stack_init(&wr->element, env))
    {
        guththila_buffer_un_init(&wr->buffer, env);
        AXIS2_FREE(env->allocator, wr);
        return NULL;
    }
    if (!guththila_stack_init(&wr->namesp, env))
    {
        guththila_buffer_un_init(&wr->buffer, env);
        guththila_stack_un_init(&wr->element, env);
        AXIS2_FREE(env->allocator, wr);
        return NULL;
    }
    
#ifdef GUTHTHILA_XML_WRITER_TOKEN
    if (!guththila_tok_list_init(&wr->tok_list, env))
    {
        guththila_buffer_un_init(&wr->buffer, env);
        guththila_stack_un_init(&wr->element, env);
        guththila_stack_un_init(&wr->namesp, env);
        AXIS2_FREE(env->allocator, wr);
        return NULL;
    }
#endif 
    wr->type = GUTHTHILA_WRITER_MEMORY;
    wr->status = BEGINING;
    wr->next = 0;
    return wr;
}

GUTHTHILA_EXPORT void GUTHTHILA_CALL 
guththila_xml_writer_free(
    guththila_xml_writer_t * wr,
    const axutil_env_t * env) 
{
    if (wr->type == GUTHTHILA_WRITER_MEMORY)
    {
        guththila_buffer_un_init(&wr->buffer, env);
    }
    else if (wr->type == GUTHTHILA_WRITER_FILE)
    {
        fclose(wr->out_stream);
    }
    
#ifdef GUTHTHILA_XML_WRITER_TOKEN
    guththila_tok_list_free_data(&wr->tok_list, env);    
#endif  
    guththila_stack_un_init(&wr->element, env);
    guththila_stack_un_init(&wr->namesp, env);
    AXIS2_FREE(env->allocator,wr);
}

int GUTHTHILA_CALL
guththila_write(guththila_xml_writer_t * wr, guththila_char_t *buff,
                size_t buff_len, const axutil_env_t * env) 
{
    size_t remain_len = 0;
    size_t temp = 0;
    size_t * temp1 = NULL, *temp2 = NULL;
    guththila_char_t **temp3 = NULL;
    int i = 0;
    if (wr->type == GUTHTHILA_WRITER_MEMORY)
    {
        remain_len = wr->buffer.buffs_size[wr->buffer.cur_buff] -
                     wr->buffer.data_size[wr->buffer.cur_buff];
        /* We have space */
        if (buff_len < remain_len)
        {
            memcpy(wr->buffer.buff[wr->buffer.cur_buff] +
                    wr->buffer.data_size[wr->buffer.cur_buff], buff, buff_len);
            wr->buffer.data_size[wr->buffer.cur_buff] += buff_len;
            wr->next += (int)buff_len;
            /* We are sure that the difference lies within the int range */
            return (int) buff_len;
        }
        else
        {
            if (remain_len != 0)
            {
                memcpy(wr->buffer.buff[wr->buffer.cur_buff] +
                        wr->buffer.data_size[wr->buffer.cur_buff], buff, remain_len);
                wr->buffer.data_size[wr->buffer.cur_buff] += remain_len;
            }
            /* We are sure that the difference lies within the int range */
            if (((int)wr->buffer.no_buffers - 1) == wr->buffer.cur_buff)
            {
                /* Out of allocated array buffers. Need to allocate*/
                wr->buffer.no_buffers = wr->buffer.no_buffers * 2;
                temp3 = (guththila_char_t **) AXIS2_MALLOC(env->allocator,
                         sizeof(guththila_char_t *) * wr->buffer.no_buffers);
                temp1 = (size_t *) AXIS2_MALLOC(env->allocator,
                         sizeof(size_t) * wr->buffer.no_buffers);
                temp2 = (size_t *) AXIS2_MALLOC(env->allocator,
                         sizeof(size_t) * wr->buffer.no_buffers);
                for (i = 0; i <= wr->buffer.cur_buff; i++)
                {
                    temp3[i] = wr->buffer.buff[i];
                    temp1[i] = wr->buffer.data_size[i];
                    temp2[i] = wr->buffer.buffs_size[i];
                }
                AXIS2_FREE(env->allocator, wr->buffer.data_size);
                AXIS2_FREE(env->allocator, wr->buffer.buffs_size);
                AXIS2_FREE(env->allocator, wr->buffer.buff);
                wr->buffer.buff = temp3;
                wr->buffer.buffs_size = temp2;
                wr->buffer.data_size = temp1;
            }
            wr->buffer.cur_buff++;
            temp = wr->buffer.buffs_size[wr->buffer.cur_buff - 1] * 2;
            while (temp < (buff_len - remain_len))
            {
                temp = temp * 2;
            }
            /* Create a be buffer */
            wr->buffer.buff[wr->buffer.cur_buff] =
                (guththila_char_t *) AXIS2_MALLOC(env->allocator, sizeof(guththila_char_t) * temp);
            wr->buffer.buffs_size[wr->buffer.cur_buff] = temp;
            memcpy(wr->buffer.buff[wr->buffer.cur_buff], buff + remain_len,
                   buff_len - remain_len);
            wr->buffer.data_size[wr->buffer.cur_buff] = buff_len - remain_len;
            wr->buffer.pre_tot_data += wr->buffer.data_size[wr->buffer.cur_buff - 1];
            wr->next += (int)buff_len;
            /* We are sure that the difference lies within the int range */
            return (int) buff_len;
        } 
    }
    else if (wr->type == GUTHTHILA_WRITER_FILE)
    {
        return (int) fwrite(buff, 1, buff_len, wr->out_stream);
    }
    return GUTHTHILA_FAILURE;
}


int GUTHTHILA_CALL
guththila_write_token(guththila_xml_writer_t * wr, guththila_token_t * tok,
                        const axutil_env_t * env) 
{
    int i;
    size_t remain_len = 0;
    size_t temp = 0;
    size_t * temp1 = NULL, *temp2 = NULL;
    guththila_char_t **temp3 = NULL;
    if (wr->type == GUTHTHILA_WRITER_MEMORY)
    {
        remain_len = wr->buffer.buffs_size[wr->buffer.cur_buff] -
                     wr->buffer.data_size[wr->buffer.cur_buff];
        if (tok->size < remain_len)
        {
            memcpy(wr->buffer.buff[wr->buffer.cur_buff] +
                    wr->buffer.data_size[wr->buffer.cur_buff], 
                    tok->start,
                    tok->size);

            wr->buffer.data_size[wr->buffer.cur_buff] += tok->size;
            wr->next += (int)tok->size;
            /* We are sure that the difference lies within the int range */
            return (int) tok->size;
        }
        else
        {
            if (remain_len != 0)
            {
                memcpy(wr->buffer.buff[wr->buffer.cur_buff] +
                       wr->buffer.data_size[wr->buffer.cur_buff], 
                       tok->start,
                       remain_len);

                wr->buffer.data_size[wr->buffer.cur_buff] += remain_len;
            }
            /* We are sure that the difference lies within the int range */
            if (((int)wr->buffer.no_buffers - 1) == wr->buffer.cur_buff)
            {
                wr->buffer.no_buffers = wr->buffer.no_buffers * 2;
                temp3 = (guththila_char_t **) AXIS2_MALLOC(env->allocator,
                         sizeof(guththila_char_t *) * wr->buffer.no_buffers);
                temp1 = (size_t *) AXIS2_MALLOC(env->allocator,
                         sizeof(size_t) * wr->buffer.no_buffers);
                temp2 = (size_t *) AXIS2_MALLOC(env->allocator,
                         sizeof(size_t) * wr->buffer.no_buffers);
                for (i = 0; i <= wr->buffer.cur_buff; i++)
                {
                    temp3[i] = wr->buffer.buff[i];
                    temp1[i] = wr->buffer.data_size[i];
                    temp2[i] = wr->buffer.buffs_size[i];
                }
                AXIS2_FREE(env->allocator, wr->buffer.data_size);
                AXIS2_FREE(env->allocator, wr->buffer.buffs_size);
                AXIS2_FREE(env->allocator, wr->buffer.buff);
                wr->buffer.buff = temp3;
                wr->buffer.buffs_size = temp2;
                wr->buffer.data_size = temp1;
            }
            wr->buffer.cur_buff++;
            temp = wr->buffer.buffs_size[wr->buffer.cur_buff - 1] * 2;
            while (temp < (tok->size - remain_len))
            {
                temp = temp * 2;
            }
            wr->buffer.buff[wr->buffer.cur_buff] =
                (guththila_char_t *) AXIS2_MALLOC(env->allocator, 
                                         sizeof(guththila_char_t) * temp);

            wr->buffer.buffs_size[wr->buffer.cur_buff] = temp;
            memcpy(wr->buffer.buff[wr->buffer.cur_buff],
                    tok->start + remain_len, tok->size - remain_len);
            wr->buffer.data_size[wr->buffer.cur_buff] = tok->size - remain_len;
            wr->buffer.pre_tot_data += wr->buffer.data_size[wr->buffer.cur_buff - 1];
            wr->next += (int)tok->size;
            /* We are sure that the difference lies within the int range */
            return (int) tok->size;
        } 
    }
    else if (wr->type == GUTHTHILA_WRITER_FILE)
    {
        return (int) fwrite(tok->start, 1, tok->size, wr->out_stream);
    }
    return GUTHTHILA_FAILURE;
}

int GUTHTHILA_CALL
guththila_write_xtoken(guththila_xml_writer_t * wr, guththila_char_t *buff,
                        size_t buff_len, const axutil_env_t * env) 
{
    int i;
    size_t temp = 0;
    size_t remain_len = 0;
    size_t * temp1 = NULL, *temp2 = NULL;
    guththila_char_t **temp3 = NULL;
    if (wr->type == GUTHTHILA_WRITER_MEMORY)
    {
        remain_len = wr->buffer.buffs_size[wr->buffer.cur_buff] -
                     wr->buffer.data_size[wr->buffer.cur_buff];
        if (buff_len < remain_len)
        {
            memcpy(wr->buffer.buff[wr->buffer.cur_buff] +
                    wr->buffer.data_size[wr->buffer.cur_buff], buff, buff_len);
            wr->buffer.data_size[wr->buffer.cur_buff] += buff_len;
            wr->next += (int)buff_len;
            /* We are sure that the difference lies within the int range */
            return (int) buff_len;
        }
        else
        {
            /* We are sure that the difference lies within the int range */
            if (((int)wr->buffer.no_buffers - 1) == wr->buffer.cur_buff)
            {
                wr->buffer.no_buffers = wr->buffer.no_buffers * 2;
                temp3 = (guththila_char_t **) AXIS2_MALLOC(env->allocator,
                                           sizeof(guththila_char_t *) *
                                           wr->buffer.no_buffers);
                temp1 = (size_t *) AXIS2_MALLOC(env->allocator,
                                            sizeof(size_t) *
                                            wr->buffer.no_buffers);
                temp2 = (size_t *) AXIS2_MALLOC(env->allocator,
                                            sizeof(size_t) *
                                            wr->buffer.no_buffers);
                for (i = 0; i <= wr->buffer.cur_buff; i++)
                {
                    temp3[i] = wr->buffer.buff[i];
                    temp1[i] = wr->buffer.data_size[i];
                    temp2[i] = wr->buffer.buffs_size[i];
                }
                AXIS2_FREE(env->allocator, wr->buffer.data_size);
                AXIS2_FREE(env->allocator, wr->buffer.buffs_size);
                AXIS2_FREE(env->allocator, wr->buffer.buff);
                wr->buffer.buff = temp3;
                wr->buffer.buffs_size = temp2;
                wr->buffer.data_size = temp1;
            }
            temp = wr->buffer.buffs_size[wr->buffer.cur_buff] * 2;
            while (temp < (buff_len))
            {
                temp = temp * 2;
            }
            wr->buffer.cur_buff++;
            wr->buffer.buff[wr->buffer.cur_buff] =
                (guththila_char_t *) AXIS2_MALLOC(env->allocator, 
                                    sizeof(guththila_char_t) * temp);

            wr->buffer.buffs_size[wr->buffer.cur_buff] = temp;
            memcpy(wr->buffer.buff[wr->buffer.cur_buff], buff, buff_len);
            wr->buffer.data_size[wr->buffer.cur_buff] = buff_len;
            wr->buffer.pre_tot_data +=
                wr->buffer.data_size[wr->buffer.cur_buff - 1];
            wr->next += (int)buff_len;
            /* We are sure that the difference lies within the int range */
            return (int) buff_len;
        } 
    }
    else if (wr->type == GUTHTHILA_WRITER_FILE)
    {
        return (int) fwrite(buff, 1, buff_len, wr->out_stream);
    }
    return GUTHTHILA_FAILURE;
}

int GUTHTHILA_CALL 
guththila_free_empty_element(
        guththila_xml_writer_t *wr,
        const axutil_env_t *env)
{
    guththila_xml_writer_element_t * elem = NULL;
    guththila_xml_writer_namesp_t * namesp = NULL;
    int i = 0, j = 0;
    elem =
        (guththila_xml_writer_element_t *) guththila_stack_pop(&wr->element, env);
    if (elem)
    {
        wr->status = BEGINING;
        if (elem->name_sp_stack_no != -1)
        {
            GUTHTHILA_WRITER_CLEAR_NAMESP(wr, &wr->namesp,
                                           elem->name_sp_stack_no, i,
                                           namesp, j, env);
        }
        GUTHTHILA_WRITER_ELEM_FREE(wr, elem, env);
        return GUTHTHILA_SUCCESS;
    }
    else
    {
        return GUTHTHILA_FAILURE;
    }    
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_start_document(guththila_xml_writer_t * wr,
                               const axutil_env_t * env,
                               char *encoding,
                               char *version) 
{
    char *tmp1 = NULL;
    char *tmp2 = GUTHTHILA_WRITER_SD_DECLARATION;

    tmp1 = strchr(tmp2, '\"');
    tmp1++;
    guththila_write(wr, tmp2, (int)(tmp1 - tmp2), env);
    tmp2 = strchr(tmp1, '\"');
    if (version)
    {
        guththila_write(wr, version, (int)strlen(version), env);
    }
    else
    {
        guththila_write(wr, tmp1, (int)(tmp2 - tmp1), env);
    }
    tmp2++;
    tmp1 = strchr(tmp2, '\"');
    tmp2--;
    tmp1++;
    guththila_write(wr, tmp2, (int)(tmp1 - tmp2), env);
    tmp2 = strchr(tmp1, '\"');
    if (encoding)
    {
        guththila_write(wr, encoding, (int)strlen(encoding), env);
    }
    else
    {
        guththila_write(wr, tmp1, (int)(tmp2 - tmp1), env);
    }
    guththila_write(wr, tmp2, (int)strlen(tmp2), env);

    return GUTHTHILA_SUCCESS;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_start_element(guththila_xml_writer_t * wr, 
                              guththila_char_t *start_element,
                              const axutil_env_t * env) 
{
    int cur_pos = 0;
    size_t len = 0;
    guththila_xml_writer_element_t * element =
        (guththila_xml_writer_element_t *) AXIS2_MALLOC(env->allocator,
                                   sizeof(guththila_xml_writer_element_t));
    len = strlen(start_element);
    if (wr->status == START)
    {   
        /* If we are in a start we need to close and start */
        guththila_write(wr, "><", 2u, env);
        cur_pos = wr->next;
        guththila_write_xtoken(wr, start_element, len, env);
    }
    else if (wr->status == START_EMPTY)
    {
        /* We need to close and start */
        guththila_free_empty_element(wr, env);
        guththila_write(wr, "/><", 3u, env);
        cur_pos = wr->next;
        guththila_write_xtoken(wr, start_element, len, env);
    }
    else if (wr->status == BEGINING)
    {
        /* We can start rightaway*/
        guththila_write(wr, "<", 1u, env);
        cur_pos = wr->next;
        guththila_write_xtoken(wr, start_element, len, env);
    }
    else
    {
        return GUTHTHILA_FAILURE;
    }
    wr->status = START;
    
#ifndef GUTHTHILA_XML_WRITER_TOKEN
    element->name = strdup(start_element);
    element->prefix = NULL;    
#else  
    element->name = guththila_tok_list_get_token(&wr->tok_list, env);
    element->name->start = GUTHTHILA_BUF_POS(wr->buffer, cur_pos);
    element->name->size = len;
    element->prefix = NULL;
    
#endif 
    element->name_sp_stack_no = -1;
    return guththila_stack_push(&wr->element, element, env);
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_end_element(guththila_xml_writer_t * wr, const axutil_env_t * env) 
{
    guththila_xml_writer_element_t * elem = NULL;
    guththila_xml_writer_namesp_t * namesp = NULL;
    int i = 0, j = 0;
    if (wr->status == START)
    {
        guththila_write(wr, "></", 3u, env);
        /* Write the contents of the element at the top */
        elem =
            (guththila_xml_writer_element_t *) guththila_stack_pop(&wr->element, env);
        if (elem)
        {
            if (elem->prefix)
            {
                
#ifndef GUTHTHILA_XML_WRITER_TOKEN
                guththila_write(wr, elem->prefix, strlen(elem->prefix), env);                
#else   
                guththila_write_token(wr, elem->prefix, env);                
#endif  
                guththila_write(wr, ":", 1u, env);
            }
            
#ifndef GUTHTHILA_XML_WRITER_TOKEN
            guththila_write(wr, elem->name, strlen(elem->name), env);            
#else   
            guththila_write_token(wr, elem->name, env);            
#endif  
            guththila_write(wr, ">", 1u, env);
            wr->status = BEGINING;
            if (elem->name_sp_stack_no != -1)
            {
                GUTHTHILA_WRITER_CLEAR_NAMESP(wr, &wr->namesp,
                                               elem->name_sp_stack_no, i,
                                               namesp, j, env);
            }
            GUTHTHILA_WRITER_ELEM_FREE(wr, elem, env);
            return GUTHTHILA_SUCCESS;
        }
        else
        {
            return GUTHTHILA_FAILURE;
        }
    }
    else if (wr->status == START_EMPTY)
    {
        guththila_write(wr, "/>", 2u, env);
        elem =
            (guththila_xml_writer_element_t *) guththila_stack_pop(&wr->element, env);
        if (elem)
        {
            wr->status = BEGINING;
            if (elem->name_sp_stack_no != -1)
            {
                GUTHTHILA_WRITER_CLEAR_NAMESP(wr, &wr->namesp,
                                               elem->name_sp_stack_no, i,
                                               namesp, j, env);
            }
            GUTHTHILA_WRITER_ELEM_FREE(wr, elem, env);
            return GUTHTHILA_SUCCESS;
        }
        else
        {
            return GUTHTHILA_FAILURE;
        }
    }
    else if (wr->status == BEGINING)
    {
        guththila_write(wr, "</", 2u, env);
        elem =
            (guththila_xml_writer_element_t *) guththila_stack_pop(&wr->element,
                                                                   env);
        if (elem)
        {
            if (elem->prefix)
            {
                
#ifndef GUTHTHILA_XML_WRITER_TOKEN
                guththila_write(wr, elem->prefix, strlen(elem->prefix),
                                env);                
#else  
                guththila_write_token(wr, elem->prefix, env);                
#endif 
                guththila_write(wr, ":", 1u, env);
            }
            
#ifndef GUTHTHILA_XML_WRITER_TOKEN
            guththila_write(wr, elem->name, strlen(elem->name), env);        
#else  
            guththila_write_token(wr, elem->name, env);            
#endif 
            guththila_write(wr, ">", 1u, env);
            wr->status = BEGINING;
            if (elem->name_sp_stack_no != -1)
            {
                GUTHTHILA_WRITER_CLEAR_NAMESP(wr, &wr->namesp,
                                               elem->name_sp_stack_no, i,
                                               namesp, j, env);
            }
            GUTHTHILA_WRITER_ELEM_FREE(wr, elem, env);
            return GUTHTHILA_SUCCESS;
        }
        else
        {
            return GUTHTHILA_FAILURE;
        }
    }
    return GUTHTHILA_FAILURE;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_close(guththila_xml_writer_t * wr, const axutil_env_t * env) 
{
    return GUTHTHILA_FAILURE;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_characters(guththila_xml_writer_t * wr, guththila_char_t *buff,
                           const axutil_env_t * env) 
{
    size_t i = 0;
    size_t len = strlen(buff);
    guththila_char_t ch = 0;
    if (wr->status == START)
    {
        wr->status = BEGINING;
        guththila_write(wr, ">", 1u, env);
    }
    else if (wr->status == START_EMPTY)
    {
        guththila_free_empty_element(wr, env);
        wr->status = BEGINING;
        guththila_write(wr, "/>", 2u, env);
    }
    else if (wr->status != BEGINING)
    {
        return GUTHTHILA_FAILURE;
    }
    while (len > 0)
    {
        /* scan buffer until the next special character */
        for (i = 0; 
            (i < len) && ((ch = buff[i]) != '&') && (ch != '<') && (ch != '>') && (ch != '\"') && (ch != '\'');
            i++)
            ;
        /* write everything until the special character */
        if (i > 0)
        {
            guththila_write(wr, buff, i, env);
            buff += i;
            len -= i;
        }
        /* replace the character with the appropriate sequence */
        if (len > 0)
        {
            if (AXIS2_SUCCESS != guththila_write_escape_character(wr, buff, env))
                return GUTHTHILA_FAILURE;
            /* skip the character */
            buff++;
            len--;
        }
    }
    return GUTHTHILA_SUCCESS;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_comment(
    guththila_xml_writer_t * wr,
    guththila_char_t *buff,
    const axutil_env_t * env) 
{
    if (wr->status == START)
    {
        wr->status = BEGINING;
        guththila_write(wr, "><!--", 5u, env);
        guththila_write(wr, buff, strlen(buff), env);
        guththila_write(wr, "-->", 3u, env);
        return GUTHTHILA_SUCCESS;
    }
    else if (wr->status == START_EMPTY)
    { 
        guththila_free_empty_element(wr, env);
        wr->status = BEGINING;
        guththila_write(wr, "/><!--", 6u, env);
        guththila_write(wr, buff, strlen(buff), env);
        guththila_write(wr, "-->", 3u, env);
        return GUTHTHILA_SUCCESS;
    }
    else if (wr->status == BEGINING)
    {
        guththila_write(wr, "<!--", 4u, env);
        guththila_write(wr, buff, strlen(buff), env);
        guththila_write(wr, "-->", 3u, env);
        return GUTHTHILA_SUCCESS;
    }
    return GUTHTHILA_FAILURE;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_escape_character(
    guththila_xml_writer_t * wr,
    guththila_char_t *buff,
    const axutil_env_t * env) 
{
    if (buff)
    {
        switch (buff[0])
        {
        case '>':
            guththila_write(wr, "&gt;", 4u, env);
            break;
        case '<':
            guththila_write(wr, "&lt;", 4u, env);
            break;
        case '\'':
            guththila_write(wr, "&apos;", 6u, env);
            break;
        case '"':
            guththila_write(wr, "&quot;", 6u, env);
            break;
        case '&':
            guththila_write(wr, "&amp;", 5u, env);
            break;
        default:
            return GUTHTHILA_FAILURE;
        };
    }
    return GUTHTHILA_SUCCESS;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_empty_element(guththila_xml_writer_t * wr, 
                              guththila_char_t *start_element,
                              const axutil_env_t * env) 
{
    int cur_pos = 0;
    size_t len = 0;
    guththila_xml_writer_element_t * element =
        (guththila_xml_writer_element_t *) AXIS2_MALLOC(env->allocator,
                                   sizeof(guththila_xml_writer_element_t));
    len = strlen(start_element);
    if (wr->status == START)
    {
        guththila_write(wr, "><", 2u, env);
        cur_pos = wr->next;
        guththila_write_xtoken(wr, start_element, len, env);
    }
    else if (wr->status == START_EMPTY)
    {
        guththila_free_empty_element(wr, env);
        wr->status = BEGINING;
        guththila_write(wr, "/><", 3u, env);
        cur_pos = wr->next;
        guththila_write_xtoken(wr, start_element, len, env);
    }
    else if (wr->status == BEGINING)
    {
        guththila_write(wr, "<", 1u, env);
        cur_pos = wr->next;
        guththila_write_xtoken(wr, start_element, len, env);
    }
    else
    {
        return GUTHTHILA_FAILURE;
    }
    wr->status = START_EMPTY;
    
#ifndef GUTHTHILA_XML_WRITER_TOKEN
    element->name = strdup(start_element);
    element->prefix = NULL;    
#else  
    element->name = guththila_tok_list_get_token(&wr->tok_list, env);
    element->name->start = GUTHTHILA_BUF_POS(wr->buffer, cur_pos);
    element->name->size = len;
    element->prefix = NULL;
    
#endif 
    element->name_sp_stack_no = -1;
    return guththila_stack_push(&wr->element, element, env);
}


GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_default_namespace(
    guththila_xml_writer_t * wr,
    guththila_char_t *namespace_uri,
    const axutil_env_t * env) 
{
    if (wr->status == START || wr->status == START_EMPTY)
    {
        guththila_write(wr, " xmlns=\"", 8u, env);
        guththila_write(wr, namespace_uri, strlen(namespace_uri), env);
        guththila_write(wr, "\"", 1u, env);
        return GUTHTHILA_SUCCESS;
    }
    return GUTHTHILA_FAILURE;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_namespace(
    guththila_xml_writer_t * wr,
    guththila_char_t *prefix,
    guththila_char_t *uri,
    const axutil_env_t * env) 
{
    int i, j, temp, nmsp_found = GUTHTHILA_FALSE, stack_size;
    guththila_xml_writer_namesp_t * namesp = NULL;
    guththila_xml_writer_element_t * elem = NULL;
    int pref_start = 0, uri_start = 0;
    guththila_xml_writer_namesp_t * writer_namesp = NULL;
    guththila_token_t ** tok_name = NULL, **tok_uri = NULL;
    size_t pref_len = strlen(prefix), uri_len = strlen(uri);
    stack_size = GUTHTHILA_STACK_SIZE(wr->namesp);
    /* Check weather we have met the namespace before */
    for (i = stack_size - 1; i >= 0; i--)
    {
        writer_namesp =
            (guththila_xml_writer_namesp_t *) guththila_stack_get_by_index(&wr->namesp,
                                                                           i,
                                                                           env);
        temp = writer_namesp->no;
        for (j = 0; j < temp; j++)
        {
            
#ifndef GUTHTHILA_XML_WRITER_TOKEN
            if (!strcmp(prefix, writer_namesp->name[j]))
            {
                
#else  
            if (!guththila_tok_str_cmp(writer_namesp->name[j], prefix, pref_len, env))
            {                
#endif  
                nmsp_found = GUTHTHILA_TRUE;
            }
        }
    }
    /* Proceed if we didn't find the namespace */    
    if (!nmsp_found && (wr->status == START || wr->status == START_EMPTY))
    {
        guththila_write(wr, " xmlns:", 7u, env);
        pref_start = wr->next;
        guththila_write_xtoken(wr, prefix, pref_len, env);
        guththila_write(wr, "=\"", 2u, env);
        uri_start = wr->next;
        guththila_write_xtoken(wr, uri, uri_len, env);
        guththila_write(wr, "\"", 1u, env);
        elem = guththila_stack_peek(&wr->element, env);
        if (elem && elem->name_sp_stack_no == -1)
        {
            namesp =
                (guththila_xml_writer_namesp_t *) AXIS2_MALLOC(env->allocator,
                                                               sizeof
                                                               (guththila_xml_writer_namesp_t));
            if (namesp)
            {
                
#ifndef GUTHTHILA_XML_WRITER_TOKEN
                namesp->name =
                    (guththila_char_t **) AXIS2_MALLOC(env->allocator,
                                           sizeof(guththila_char_t *) *
                                           GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE);
                namesp->uri =
                    (guththila_char_t **) AXIS2_MALLOC(env->allocator,
                                           sizeof(guththila_char_t *) *
                                           GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE);
                namesp->name[0] = strdup(prefix);
                namesp->uri[0] = strdup(uri);
                
#else   
                namesp->name =
                    (guththila_token_t **) AXIS2_MALLOC(env->allocator,
                                                        sizeof(guththila_token_t
                                                               *) *
                                                        GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE);
                namesp->uri =
                    (guththila_token_t **) AXIS2_MALLOC(env->allocator,
                                                        sizeof(guththila_token_t
                                                               *) *
                                                        GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE);
                namesp->name[0] =
                    guththila_tok_list_get_token(&wr->tok_list, env);
                namesp->name[0]->start =
                    GUTHTHILA_BUF_POS(wr->buffer, pref_start);
                namesp->name[0]->size = pref_len;
                namesp->uri[0] =
                    guththila_tok_list_get_token(&wr->tok_list, env);
                namesp->uri[0]->start =
                    GUTHTHILA_BUF_POS(wr->buffer, uri_start);
                namesp->uri[0]->size = uri_len;
                
#endif  
                namesp->no = 1;
                namesp->size = GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE;
                guththila_stack_push(&wr->namesp, namesp, env);
                elem->name_sp_stack_no = GUTHTHILA_STACK_TOP_INDEX(wr->namesp);
            }
            else
            {
                return GUTHTHILA_FAILURE;
            }
        }
        else if (elem)
        {
            namesp = guththila_stack_peek(&wr->namesp, env);
            if (namesp->no < namesp->size)
            {
                
#ifndef GUTHTHILA_XML_WRITER_TOKEN
                namesp->name[++(namesp->no) - 1] = strdup(prefix);
                namesp->uri[namesp->no - 1] = strdup(uri);
                
#else   
                namesp->name[++(namesp->no) - 1] =
                    guththila_tok_list_get_token(&wr->tok_list, env);
                namesp->uri[namesp->no - 1] =
                    guththila_tok_list_get_token(&wr->tok_list, env);
                namesp->name[namesp->no - 1]->start =
                    GUTHTHILA_BUF_POS(wr->buffer, pref_start);
                namesp->name[namesp->no - 1]->size = pref_len;
                namesp->uri[namesp->no - 1]->start =
                    GUTHTHILA_BUF_POS(wr->buffer, uri_start);
                namesp->uri[namesp->no - 1]->size = uri_len;
                
#endif  
            }
            else
            {
                
#ifndef GUTHTHILA_XML_WRITER_TOKEN
                namesp->name =
                    (guththila_char_t **) realloc(namesp->name,
                                      sizeof(guththila_char_t *) *
                                      (GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE +
                                       namesp->size));
                namesp->uri =
                    (guththila_char_t **) realloc(namesp->name,
                                      sizeof(guththila_char_t *) *
                                      (GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE +
                                       namesp->size));
                namesp->size = GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE + namesp->size;
                namesp->name[++(namesp->no) - 1] = strdup(prefix);
                namesp->uri[namesp->no - 1] = strdup(uri);
                
#else               
                tok_name =
                    (guththila_token_t **) AXIS2_MALLOC(env->allocator,
                                                sizeof(guththila_token_t *) *
                                                (GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE
                                                 + namesp->size));
                tok_uri =
                    (guththila_token_t **) AXIS2_MALLOC(env->allocator,
                                                sizeof(guththila_token_t *) *
                                                (GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE
                                                 + namesp->size));
                for (i = 0; i < namesp->no; i++)
                {
                    tok_name[i] = namesp->name[i];
                    tok_uri[i] = namesp->uri[i];
                }
                AXIS2_FREE(env->allocator, namesp->name);
                AXIS2_FREE(env->allocator, namesp->uri);
                namesp->name = tok_name;
                namesp->uri = tok_uri;
                namesp->size = GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE + namesp->size;

                namesp->name[namesp->no] =
                    guththila_tok_list_get_token(&wr->tok_list, env);
                namesp->uri[namesp->no] =
                    guththila_tok_list_get_token(&wr->tok_list, env);

                namesp->name[namesp->no ]->start =
                    GUTHTHILA_BUF_POS(wr->buffer, pref_start);
                namesp->name[namesp->no ]->size = pref_len;
                namesp->uri[namesp->no ]->start =
                    GUTHTHILA_BUF_POS(wr->buffer, uri_start);
                namesp->uri[namesp->no ]->size = uri_len;
                namesp->no ++;
#endif  
            }
        }
        return GUTHTHILA_SUCCESS;
    }
    if (nmsp_found)
        return GUTHTHILA_SUCCESS;
    return GUTHTHILA_FAILURE;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_attribute(
    guththila_xml_writer_t * wr,
    guththila_char_t *localname,
    guththila_char_t *value,
    const axutil_env_t * env) 
{
    if (wr->status == START || wr->status == START_EMPTY)
    {
        guththila_write(wr, " ", 1u, env);
        guththila_write(wr, localname, strlen(localname), env);
        guththila_write(wr, "=\"", 2u, env);
        guththila_write(wr, value, strlen(value), env);
        guththila_write(wr, "\"", 1u, env);
        return GUTHTHILA_SUCCESS;
    }
    return GUTHTHILA_FAILURE;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_attribute_with_prefix_and_namespace(
    guththila_xml_writer_t * wr,
    guththila_char_t *prefix,
    guththila_char_t *namespace_uri,
    guththila_char_t *localname,
    guththila_char_t *value,
    const axutil_env_t * env) 
{
    return guththila_write_namespace(wr, prefix, namespace_uri, env) &&
        guththila_write_attribute_with_prefix(wr, prefix, localname, value,
                                              env);
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_attribute_with_prefix(
    guththila_xml_writer_t * wr,
    guththila_char_t *prefix,
    guththila_char_t *localname,
    guththila_char_t *value,
    const axutil_env_t * env) 
{
    int i, j;
    int stack_size = GUTHTHILA_STACK_SIZE(wr->namesp);
    int temp;
    size_t pref_len = strlen(prefix);
    guththila_xml_writer_namesp_t * writer_namesp = NULL;
    if (wr->status == START || wr->status == START_EMPTY)
    {   
        /* We need to make sure that there is a namespace defined with the 
         * given prefix as the name */
        for (i = stack_size - 1; i >= 0; i--)
        {
            writer_namesp =
                (guththila_xml_writer_namesp_t *)
                guththila_stack_get_by_index(&wr->namesp, i, env);
            temp = writer_namesp->no;
            for (j = 0; j < temp; j++)
            {
                
#ifndef GUTHTHILA_XML_WRITER_TOKEN
                if (!strcmp(prefix, writer_namesp->name[j]))
                {
                    
#else   
                if (!guththila_tok_str_cmp
                        (writer_namesp->name[j], prefix, pref_len, env))
                {
                    
#endif  
                    guththila_write(wr, " ", 1u, env);
                    guththila_write(wr, prefix, pref_len, env);
                    guththila_write(wr, ":", 1u, env);
                    guththila_write(wr, localname, strlen(localname), env);
                    guththila_write(wr, "=\"", 2u, env);
                    guththila_write(wr, value, strlen(value), env);
                    guththila_write(wr, "\"", 1u, env);
                    return GUTHTHILA_SUCCESS;
                }
            }
        }
    }
    return GUTHTHILA_FAILURE;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_attribute_with_namespace(
    guththila_xml_writer_t * wr,
    guththila_char_t *namesp,
    guththila_char_t *loc_name,
    guththila_char_t *value,
    const axutil_env_t * env) 
{
    int i, j;
    int stack_size = GUTHTHILA_STACK_SIZE(wr->namesp);
    int temp;
    guththila_xml_writer_namesp_t * writer_namesp = NULL;
    if (wr->status == START || wr->status == START_EMPTY)
    {
        /* We need to make sure that the namespace is previously declared */
        for (i = stack_size - 1; i >= 0; i--)
        {
            writer_namesp = (guththila_xml_writer_namesp_t *)
                guththila_stack_get_by_index(&wr->namesp, i, env);
            temp = writer_namesp->no;
            for (j = 0; j < temp; j++)
            {                
#ifndef GUTHTHILA_XML_WRITER_TOKEN
                if (!strcmp(namesp, writer_namesp->uri[j]))
                {
                    guththila_write(wr, " ", 1, env);
                    guththila_write(wr, writer_namesp->name[j],
                                     strlen(writer_namesp->name[j]), env);                    
#else   
                if (!guththila_tok_str_cmp
                        (writer_namesp->uri[j], namesp, strlen(namesp), env))
                {
                    guththila_write(wr, " ", 1u, env);
                    guththila_write_token(wr, writer_namesp->name[j], env);                    
#endif  
                    guththila_write(wr, ":", 1u, env);
                    guththila_write(wr, loc_name, strlen(loc_name), env);
                    guththila_write(wr, "=\"", 2u, env);
                    guththila_write(wr, value, strlen(value), env);
                    guththila_write(wr, "\"", 1u, env);
                    return GUTHTHILA_SUCCESS;
                }
            }
        }
    }
    return GUTHTHILA_FAILURE;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_start_element_with_prefix_and_namespace(
                                    guththila_xml_writer_t * wr,
                                    guththila_char_t *prefix,
                                    guththila_char_t *namespace_uri,
                                    guththila_char_t *local_name,
                                    const axutil_env_t * env) 
{
    int i, j, temp, stack_size, nmsp_found = GUTHTHILA_FALSE;
    guththila_xml_writer_namesp_t * namesp = NULL;
    guththila_xml_writer_element_t * elem = NULL;
    int uri_start = 0, pref_start = 0, elem_start = 0, elem_pref_start = 0;
    size_t uri_len = 0;
    size_t pref_len = 0;
    size_t elem_len = 0;
    guththila_xml_writer_namesp_t * writer_namesp = NULL;

    elem =
        (guththila_xml_writer_element_t *) AXIS2_MALLOC(env->allocator,
                                                    sizeof
                                                    (guththila_xml_writer_element_t));
    uri_len = strlen(namespace_uri);
    pref_len = strlen(prefix);
    elem_len = strlen(local_name);
    stack_size = GUTHTHILA_STACK_SIZE(wr->namesp);
    /* We have to determine weather we have seen the namespace before */
    for (i = stack_size - 1; i >= 0; i--)
    {
        writer_namesp =
            (guththila_xml_writer_namesp_t *) guththila_stack_get_by_index(&wr->namesp,
                                                                           i,
                                                                           env);
        temp = writer_namesp->no;
        for (j = 0; j < temp; j++)
        {           
#ifndef GUTHTHILA_XML_WRITER_TOKEN
            if (!strcmp(uri, writer_namesp->uri[j]))
            {
                
#else   
            if (!guththila_tok_str_cmp
                    (writer_namesp->name[j], prefix, pref_len, env))
            {
                
#endif  
                nmsp_found = GUTHTHILA_TRUE;
            }
        }
    }
    if (elem)
    {
        elem->name_sp_stack_no = -1;
        if (wr->status == START)
        {
            guththila_write(wr, "><", 2u, env);
            elem_pref_start = wr->next;
            guththila_write_xtoken(wr, prefix, pref_len, env);
            guththila_write(wr, ":", 1u, env);
            elem_start = wr->next;
            guththila_write_xtoken(wr, local_name, elem_len, env);
            if (!nmsp_found)
            {
                guththila_write(wr, " ", 1u, env);
                guththila_write(wr, "xmlns:", 6u, env);
                pref_start = wr->next;
                guththila_write_xtoken(wr, prefix, pref_len, env);
                guththila_write(wr, "=\"", 2u, env);
                uri_start = wr->next;
                guththila_write_xtoken(wr, namespace_uri, uri_len, env);
                guththila_write(wr, "\"", 1u, env);
            }
        }
        else if (wr->status == START_EMPTY)
        {
            guththila_free_empty_element(wr, env);
            guththila_write(wr, "/><", 2u, env);
            elem_pref_start = wr->next;
            guththila_write_xtoken(wr, prefix, pref_len, env);
            guththila_write(wr, ":", 1u, env);
            elem_start = wr->next;
            guththila_write_xtoken(wr, local_name, elem_len, env);
            if (!nmsp_found)
            {
                guththila_write(wr, " ", 1u, env);
                guththila_write(wr, "xmlns:", 6u, env);
                pref_start = wr->next;
                guththila_write_xtoken(wr, prefix, pref_len, env);
                guththila_write(wr, "=\"", 2u, env);
                uri_start = wr->next;
                guththila_write_xtoken(wr, namespace_uri, uri_len, env);
                guththila_write(wr, "\"", 1u, env);
            }
            wr->status = START;
        }
        else if (wr->status == BEGINING)
        {
            guththila_write(wr, "<", 1u, env);
            elem_pref_start = wr->next;
            guththila_write_xtoken(wr, prefix, pref_len, env);
            guththila_write(wr, ":", 1u, env);
            elem_start = wr->next;
            guththila_write_xtoken(wr, local_name, elem_len, env);
            if (!nmsp_found)
            {
                guththila_write(wr, " ", 1u, env);
                guththila_write(wr, "xmlns:", 6u, env);
                pref_start = wr->next;
                guththila_write_xtoken(wr, prefix, pref_len, env);
                guththila_write(wr, "=\"", 2u, env);
                uri_start = wr->next;
                guththila_write_xtoken(wr, namespace_uri, uri_len, env);
                guththila_write(wr, "\"", 1u, env);
            }
            wr->status = START;
        }
        else
        {
            return GUTHTHILA_FAILURE;
        }
        if (!nmsp_found)
        {
            /* If this namespace not defined previously we need to add it */
	        namesp =
        	    (guththila_xml_writer_namesp_t *) AXIS2_MALLOC(env->allocator,
                                                   sizeof
                                                   (guththila_xml_writer_namesp_t));	
            
#ifndef GUTHTHILA_XML_WRITER_TOKEN
            namesp->name =
                (guththila_char_t **) AXIS2_MALLOC(env->allocator,
                                       sizeof(guththila_char_t *) *
                                       GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE);
            namesp->uri =
                (guththila_char_t **) AXIS2_MALLOC(env->allocator,
                                       sizeof(guththila_char_t *) *
                                       GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE);
            namesp->name[0] = strdup(prefix);
            namesp->uri[0] = strdup(namespace_uri);
            
#else   
            namesp->name =
                (guththila_token_t **) AXIS2_MALLOC(env->allocator,
                                                    sizeof(guththila_token_t *)
                                                    *
                                                    GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE);
            namesp->uri =
                (guththila_token_t **) AXIS2_MALLOC(env->allocator,
                                                    sizeof(guththila_token_t *)
                                                    *
                                                    GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE);
            namesp->name[0] = guththila_tok_list_get_token(&wr->tok_list, env);
            namesp->name[0]->start = GUTHTHILA_BUF_POS(wr->buffer, pref_start);
            namesp->name[0]->size = pref_len;
            namesp->uri[0] = guththila_tok_list_get_token(&wr->tok_list, env);
            namesp->uri[0]->start = GUTHTHILA_BUF_POS(wr->buffer, uri_start);
            namesp->uri[0]->size = uri_len;            
#endif  
            namesp->no = 1;
            namesp->size = GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE;
            guththila_stack_push(&wr->namesp, namesp, env);
            elem->name_sp_stack_no = GUTHTHILA_STACK_TOP_INDEX(wr->namesp);
        }
        
#ifndef GUTHTHILA_XML_WRITER_TOKEN
        elem->name = strdup(local_name);
        elem->prefix = strdup(prefix);        
#else   
        elem->name = guththila_tok_list_get_token(&wr->tok_list, env);
        elem->prefix = guththila_tok_list_get_token(&wr->tok_list, env);
        elem->name->start = GUTHTHILA_BUF_POS(wr->buffer, elem_start);
        elem->name->size = elem_len;
        elem->prefix->start = GUTHTHILA_BUF_POS(wr->buffer, elem_pref_start);
        elem->prefix->size = pref_len;        
#endif           
        guththila_stack_push(&wr->element, elem, env);
    }
    else
    {
        return GUTHTHILA_FAILURE;
    }
    return GUTHTHILA_SUCCESS;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_start_element_with_namespace(
                        guththila_xml_writer_t * wr,
                        guththila_char_t *namespace_uri,
                        guththila_char_t *local_name,
                        const axutil_env_t * env) 
{
    int i = 0, j = 0;
    int stack_size = GUTHTHILA_STACK_SIZE(wr->namesp);
    int temp = 0;
    int elem_start = 0;
    size_t elem_len = 0;
    guththila_xml_writer_namesp_t * writer_namesp = NULL;
    guththila_xml_writer_element_t * element;

    elem_len = strlen(local_name);
    element =
        (guththila_xml_writer_element_t *) AXIS2_MALLOC(env->allocator,
                                                        sizeof (guththila_xml_writer_element_t));
    if (!element)
        return GUTHTHILA_FAILURE;

    for (i = stack_size - 1; i >= 0; i--)
    {
        writer_namesp =
            (guththila_xml_writer_namesp_t *) guththila_stack_get_by_index(&wr->namesp,
                                                                           i,
                                                                           env);
        temp = writer_namesp->no;
        for (j = 0; j < temp; j++)
        {           
#ifndef GUTHTHILA_XML_WRITER_TOKEN
            if (!strcmp(namespace_uri, writer_namesp->uri[j]))
#else
            if (!guththila_tok_str_cmp
                    (writer_namesp->uri[j], namespace_uri,
                     strlen(namespace_uri), env))
#endif
            {
                i = 0;                  /* force exit from outer loop as well */
                break;
            }
        }
    }

    /* Close off any preceding element and start a new element */
    if (wr->status == START)
    {
        guththila_write(wr, "><", 2u, env);       
    }
    else if (wr->status == START_EMPTY)
    {
        guththila_free_empty_element(wr, env);
        guththila_write(wr, "/><", 2u, env);       
    }
    else if (wr->status == BEGINING)
    {
        guththila_write(wr, "<", 1u, env);        
    }
    else
    {
        return GUTHTHILA_FAILURE;
    }
    /* If there is a prefix, include it. */
    if (writer_namesp && (j < writer_namesp->no))
    {
#ifndef GUTHTHILA_XML_WRITER_TOKEN
        guththila_write(wr, writer_namesp->name[j], strlen(writer_namesp->name[j]), env);       
#else
        guththila_write_token(wr, writer_namesp->name[j], env);    
#endif
        guththila_write(wr, ":", 1u, env);
    }
    elem_start = wr->next;
    guththila_write_xtoken(wr, local_name, elem_len, env);

    /* Remember this element's name and prefix, so the closing tag can be written later. */
#ifndef GUTHTHILA_XML_WRITER_TOKEN
    element->name = strdup(local_name);
    if (writer_namesp && (j < writer_namesp->no))
    {
        element->prefix = strdup(writer_namesp->name[j]);
    }
    else
    {
        element->prefix = NULL;
    }
    
#else
    element->name =
        guththila_tok_list_get_token(&wr->tok_list, env);
    element->name->size = elem_len;
    element->name->start =
        GUTHTHILA_BUF_POS(wr->buffer, elem_start);
    if (writer_namesp && (j < writer_namesp->no))
    {
        element->prefix =
            guththila_tok_list_get_token(&wr->tok_list, env);
        element->prefix->size = writer_namesp->name[j]->size;
        element->prefix->start = writer_namesp->name[j]->start;
    }
    else
    {
        element->prefix = NULL;
    }
#endif

    element->name_sp_stack_no = -1;
    wr->status = START;
    return guththila_stack_push(&wr->element, element, env);
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_start_element_with_prefix(
    guththila_xml_writer_t * wr,
    guththila_char_t *prefix,
    guththila_char_t *local_name,
    const axutil_env_t * env) 
{
    int i, j;
    int stack_size = GUTHTHILA_STACK_SIZE(wr->namesp);
    int temp;
    int elem_start = 0;
    size_t elem_len = 0, pref_len = 0;
    guththila_xml_writer_namesp_t * writer_namesp = NULL;
    elem_len = strlen(local_name);
    pref_len = strlen(prefix);
    for (i = stack_size - 1; i >= 0; i--)
    {
        writer_namesp =
            (guththila_xml_writer_namesp_t *) guththila_stack_get_by_index(&wr->
                                                                           namesp,
                                                                           i,
                                                                           env);
        temp = writer_namesp->no;
        for (j = 0; j < temp; j++)
        {
            /* if we found a namespace with the given prefix we can proceed */
#ifndef GUTHTHILA_XML_WRITER_TOKEN
            if (!strcmp(prefix, writer_namesp->name[j]))
            {                
#else   
            if (!guththila_tok_str_cmp(writer_namesp->name[j], 
                                       prefix, pref_len, env))
            {                
#endif  
                guththila_xml_writer_element_t * element =
                    (guththila_xml_writer_element_t *) AXIS2_MALLOC(env->
                                                                    allocator,
                                                                    sizeof
                                                                    (guththila_xml_writer_element_t));
                if (wr->status == START)
                {
                    guththila_write(wr, "><", 2u, env);
                    guththila_write_xtoken(wr, prefix, pref_len, env);
                    guththila_write(wr, ":", 1u, env);
                    elem_start = wr->next;
                    guththila_write_xtoken(wr, local_name, strlen(local_name),
                                            env);
                }
                else if (wr->status == START_EMPTY)
                {
                    guththila_free_empty_element(wr, env);
                    guththila_write(wr, "/><", 3u, env);
                    guththila_write_xtoken(wr, prefix, pref_len, env);
                    guththila_write(wr, ":", 1u, env);
                    elem_start = wr->next;
                    guththila_write_xtoken(wr, local_name, strlen(local_name),
                                            env);
                }
                else if (wr->status == BEGINING)
                {
                    guththila_write(wr, "<", 1u, env);
                    guththila_write_xtoken(wr, prefix, pref_len, env);
                    guththila_write(wr, ":", 1u, env);
                    elem_start = wr->next;
                    guththila_write_xtoken(wr, local_name, strlen(local_name),
                                            env);
                }
                else
                {
                    return GUTHTHILA_FAILURE;
                }
                
#ifndef GUTHTHILA_XML_WRITER_TOKEN
                element->name = strdup(local_name);
                element->prefix = strdup(prefix);                
#else   
                element->name =
                    guththila_tok_list_get_token(&wr->tok_list, env);
                element->name->size = elem_len;
                element->name->start =
                    GUTHTHILA_BUF_POS(wr->buffer, elem_start);
                element->prefix =
                    guththila_tok_list_get_token(&wr->tok_list, env);
                element->prefix->size = writer_namesp->name[j]->size;
                element->prefix->start = writer_namesp->name[j]->start;                
#endif  
                wr->status = START;
                element->name_sp_stack_no = -1;
                return guththila_stack_push(&wr->element, element, env);
            }
        }
    }
    return GUTHTHILA_FAILURE;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_empty_element_with_prefix_and_namespace(
                                    guththila_xml_writer_t * wr,
                                    guththila_char_t *prefix,
                                    guththila_char_t *namespace_uri,
                                    guththila_char_t *local_name,
                                    const axutil_env_t * env) 
{
    int i, j, temp, stack_size, nmsp_found = GUTHTHILA_FALSE;
    guththila_xml_writer_namesp_t * namesp = NULL;
    guththila_xml_writer_element_t * elem = NULL;
    int uri_start = 0, pref_start = 0, elem_start = 0, elem_pref_start = 0;
    size_t uri_len = 0;
    size_t pref_len = 0;
    size_t elem_len = 0;
    guththila_xml_writer_namesp_t * writer_namesp = NULL;

    namesp =
        (guththila_xml_writer_namesp_t *) AXIS2_MALLOC(env->allocator,
                                                   sizeof
                                                   (guththila_xml_writer_namesp_t));
    elem =
        (guththila_xml_writer_element_t *) AXIS2_MALLOC(env->allocator,
                                                    sizeof
                                                    (guththila_xml_writer_element_t));
    uri_len = strlen(namespace_uri);
    pref_len = strlen(prefix);
    elem_len = strlen(local_name);
    stack_size = GUTHTHILA_STACK_SIZE(wr->namesp);
    /* Chech weather we have defined this namespace before */
    for (i = stack_size - 1; i >= 0; i--)
    {
        writer_namesp =
            (guththila_xml_writer_namesp_t *) guththila_stack_get_by_index(&wr->namesp,
                                                                           i,
                                                                           env);
        temp = writer_namesp->no;
        for (j = 0; j < temp; j++)
        {           
#ifndef GUTHTHILA_XML_WRITER_TOKEN
            if (!strcmp(uri, writer_namesp->uri[j]))
            {
                
#else   
            if (!guththila_tok_str_cmp
                    (writer_namesp->name[j], prefix, pref_len, env))
            {
                
#endif  
                nmsp_found = GUTHTHILA_TRUE;
            }
        }
    }
    if (namesp && elem)
    {
        elem->name_sp_stack_no = -1;
        if (wr->status == START)
        {
            guththila_write(wr, "><", 2u, env);
            elem_pref_start = wr->next;
            guththila_write_xtoken(wr, prefix, pref_len, env);
            guththila_write(wr, ":", 1u, env);
            elem_start = wr->next;
            guththila_write_xtoken(wr, local_name, elem_len, env);
            if (!nmsp_found)
            {
                guththila_write(wr, " ", 1u, env);
                guththila_write(wr, "xmlns:", 6u, env);
                pref_start = wr->next;
                guththila_write_xtoken(wr, prefix, pref_len, env);
                guththila_write(wr, "=\"", 2u, env);
                uri_start = wr->next;
                guththila_write_xtoken(wr, namespace_uri, uri_len, env);
                guththila_write(wr, "\"", 1u, env);
            }
            wr->status = START_EMPTY;
        }
        else if (wr->status == START_EMPTY)
        {
            guththila_free_empty_element(wr, env);
            guththila_write(wr, "/><", 2u, env);
            elem_pref_start = wr->next;
            guththila_write_xtoken(wr, prefix, pref_len, env);
            guththila_write(wr, ":", 1u, env);
            elem_start = wr->next;
            guththila_write_xtoken(wr, local_name, elem_len, env);
            if (!nmsp_found)
            {
                guththila_write(wr, " ", 1u, env);
                guththila_write(wr, "xmlns:", 6u, env);
                pref_start = wr->next;
                guththila_write_xtoken(wr, prefix, pref_len, env);
                guththila_write(wr, "=\"", 2u, env);
                uri_start = wr->next;
                guththila_write_xtoken(wr, namespace_uri, uri_len, env);
                guththila_write(wr, "\"", 1u, env);
            }
        }
        else if (wr->status == BEGINING)
        {
            guththila_write(wr, "<", 1u, env);
            elem_pref_start = wr->next;
            guththila_write_xtoken(wr, prefix, pref_len, env);
            guththila_write(wr, ":", 1u, env);
            elem_start = wr->next;
            guththila_write_xtoken(wr, local_name, elem_len, env);
            if (!nmsp_found)
            {
                guththila_write(wr, " ", 1u, env);
                guththila_write(wr, "xmlns:", 6u, env);
                pref_start = wr->next;
                guththila_write_xtoken(wr, prefix, pref_len, env);
                guththila_write(wr, "=\"", 2u, env);
                uri_start = wr->next;
                guththila_write_xtoken(wr, namespace_uri, uri_len, env);
                guththila_write(wr, "\"", 1u, env);
            }
            wr->status = START_EMPTY;
        }
        else
        {
            return GUTHTHILA_FAILURE;
        }
        if (!nmsp_found)
        {
            /* If the namespace is not defined we need to remember it for later*/
#ifndef GUTHTHILA_XML_WRITER_TOKEN
            namesp->name =
                (guththila_char_t **) AXIS2_MALLOC(env->allocator,
                                       sizeof(guththila_char_t *) *
                                       GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE);
            namesp->uri =
                (guththila_char_t **) AXIS2_MALLOC(env->allocator,
                                       sizeof(guththila_char_t *) *
                                       GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE);
            namesp->name[0] = strdup(prefix);
            namesp->uri[0] = strdup(namespace_uri);
            
#else   
            namesp->name =
                (guththila_token_t **) AXIS2_MALLOC(env->allocator,
                                                    sizeof(guththila_token_t *)
                                                    *
                                                    GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE);
            namesp->uri =
                (guththila_token_t **) AXIS2_MALLOC(env->allocator,
                                                    sizeof(guththila_token_t *)
                                                    *
                                                    GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE);
            namesp->name[0] = guththila_tok_list_get_token(&wr->tok_list, env);
            namesp->name[0]->start = GUTHTHILA_BUF_POS(wr->buffer, pref_start);
            namesp->name[0]->size = pref_len;
            namesp->uri[0] = guththila_tok_list_get_token(&wr->tok_list, env);
            namesp->uri[0]->start = GUTHTHILA_BUF_POS(wr->buffer, uri_start);
            namesp->uri[0]->size = uri_len;            
#endif  
            namesp->no = 1;
            namesp->size = GUTHTHILA_XML_WRITER_NAMESP_DEF_SIZE;
            guththila_stack_push(&wr->namesp, namesp, env);
            elem->name_sp_stack_no = GUTHTHILA_STACK_TOP_INDEX(wr->namesp);
        }
        
#ifndef GUTHTHILA_XML_WRITER_TOKEN
        elem->name = strdup(local_name);
        elem->prefix = strdup(prefix);        
#else   
        elem->name = guththila_tok_list_get_token(&wr->tok_list, env);
        elem->prefix = guththila_tok_list_get_token(&wr->tok_list, env);
        elem->name->start = GUTHTHILA_BUF_POS(wr->buffer, elem_start);
        elem->name->size = elem_len;
        elem->prefix->start = GUTHTHILA_BUF_POS(wr->buffer, elem_pref_start);
        elem->prefix->size = pref_len;        
#endif           
        guththila_stack_push(&wr->element, elem, env);
    }
    return GUTHTHILA_SUCCESS;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_empty_element_with_namespace(
                        guththila_xml_writer_t * wr,
                        guththila_char_t *namespace_uri,
                        guththila_char_t *local_name,
                        const axutil_env_t * env) 
{
    int i = 0, j = 0;
    int stack_size = GUTHTHILA_STACK_SIZE(wr->namesp);
    int temp = 0;
    int elem_start = 0;
    size_t elem_len = 0;
    guththila_xml_writer_namesp_t * writer_namesp = NULL;
    guththila_xml_writer_element_t * element;

    elem_len = strlen(local_name);
    element =
        (guththila_xml_writer_element_t *) AXIS2_MALLOC(env->allocator,
                                                        sizeof (guththila_xml_writer_element_t));
    if (!element)
        return GUTHTHILA_FAILURE;

    for (i = stack_size - 1; i >= 0; i--)
    {
        writer_namesp =
            (guththila_xml_writer_namesp_t *) guththila_stack_get_by_index(&wr->namesp,
                                                                           i,
                                                                           env);
        temp = writer_namesp->no;
        for (j = 0; j < temp; j++)
        {           
#ifndef GUTHTHILA_XML_WRITER_TOKEN
            if (!strcmp(namespace_uri, writer_namesp->uri[j]))
#else
            if (!guththila_tok_str_cmp
                    (writer_namesp->uri[j], namespace_uri,
                     strlen(namespace_uri), env))
#endif
            {
                i = 0;                  /* force exit from outer loop as well */
                break;
            }
        }
    }

    /* Close off any preceding element and start a new element */
    if (wr->status == START)
    {
        guththila_write(wr, "><", 2u, env);       
    }
    else if (wr->status == START_EMPTY)
    {   
        guththila_free_empty_element(wr, env);
        guththila_write(wr, "/><", 2u, env);       
    }
    else if (wr->status == BEGINING)
    {
        guththila_write(wr, "<", 1u, env);        
    }
    else
    {
        return GUTHTHILA_FAILURE;
    }
    /* If there is a prefix, include it. */
    if (writer_namesp && (j < writer_namesp->no))
    {
#ifndef GUTHTHILA_XML_WRITER_TOKEN
        guththila_write(wr, writer_namesp->name[j], strlen(writer_namesp->name[j]), env);       
#else
        guththila_write_token(wr, writer_namesp->name[j], env);    
#endif
        guththila_write(wr, ":", 1u, env);
    }
    elem_start = wr->next;
    guththila_write_xtoken(wr, local_name, elem_len, env);

    /* Remember this element's name and prefix, so the closing tag can be written later. */
#ifndef GUTHTHILA_XML_WRITER_TOKEN
    element->name = strdup(local_name);
    if (writer_namesp && (j < writer_namesp->no))
    {
        element->prefix = strdup(writer_namesp->name[j]);
    }
    else
    {
        element->prefix = NULL;
    }
    
#else
    element->name =
        guththila_tok_list_get_token(&wr->tok_list, env);
    element->name->size = elem_len;
    element->name->start =
        GUTHTHILA_BUF_POS(wr->buffer, elem_start);
    if (writer_namesp && (j < writer_namesp->no))
    {
        element->prefix =
            guththila_tok_list_get_token(&wr->tok_list, env);
        element->prefix->size = writer_namesp->name[j]->size;
        element->prefix->start = writer_namesp->name[j]->start;
    }
    else
    {
        element->prefix = NULL;
    }
#endif

    element->name_sp_stack_no = -1;
    wr->status = START_EMPTY;
    return guththila_stack_push(&wr->element, element, env);
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_empty_element_with_prefix(
    guththila_xml_writer_t * wr,
    guththila_char_t *prefix,
    guththila_char_t *local_name,
    const axutil_env_t * env) 
{
    int i, j;
    int stack_size = GUTHTHILA_STACK_SIZE(wr->namesp);
    int temp;
    int elem_start = 0;
    size_t elem_len = 0, pref_len = 0;
    guththila_xml_writer_namesp_t * writer_namesp = NULL;
    elem_len = strlen(local_name);
    pref_len = strlen(prefix);
    for (i = stack_size - 1; i >= 0; i--)
    {
        writer_namesp =
            (guththila_xml_writer_namesp_t *) guththila_stack_get_by_index(&wr->
                                                                           namesp,
                                                                           i,
                                                                           env);
        temp = writer_namesp->no;
        for (j = 0; j < temp; j++)
        {
            /* Proceed if we found the namespace */
#ifndef GUTHTHILA_XML_WRITER_TOKEN
            if (!strcmp(prefix, writer_namesp->name[j]))
            {                
#else   
            if (!guththila_tok_str_cmp(writer_namesp->name[j], 
                                       prefix, pref_len, env))
            {                
#endif  
                guththila_xml_writer_element_t * element =
                    (guththila_xml_writer_element_t *) AXIS2_MALLOC(env->
                                                                    allocator,
                                                                    sizeof
                                                                    (guththila_xml_writer_element_t));
                if (wr->status == START)
                {
                    guththila_write(wr, "><", 2u, env);
                    guththila_write_xtoken(wr, prefix, pref_len, env);
                    guththila_write(wr, ":", 1u, env);
                    elem_start = wr->next;
                    guththila_write_xtoken(wr, local_name, strlen(local_name),
                                            env);
                }
                else if (wr->status == START_EMPTY)
                {
                    guththila_free_empty_element(wr, env);
                    guththila_write(wr, "/><", 3u, env);
                    guththila_write_xtoken(wr, prefix, pref_len, env);
                    guththila_write(wr, ":", 1u, env);
                    elem_start = wr->next;
                    guththila_write_xtoken(wr, local_name, strlen(local_name),
                                            env);
                }
                else if (wr->status == BEGINING)
                {
                    guththila_write(wr, "<", 1u, env);
                    guththila_write_xtoken(wr, prefix, pref_len, env);
                    guththila_write(wr, ":", 1u, env);
                    elem_start = wr->next;
                    guththila_write_xtoken(wr, local_name, strlen(local_name),
                                            env);
                }
                else
                {
                    return GUTHTHILA_FAILURE;
                }
                
#ifndef GUTHTHILA_XML_WRITER_TOKEN
                element->name = strdup(local_name);
                element->prefix = strdup(prefix);                
#else   
                element->name =
                    guththila_tok_list_get_token(&wr->tok_list, env);
                element->name->size = elem_len;
                element->name->start =
                    GUTHTHILA_BUF_POS(wr->buffer, elem_start);
                element->prefix =
                    guththila_tok_list_get_token(&wr->tok_list, env);
                element->prefix->size = writer_namesp->name[j]->size;
                element->prefix->start = writer_namesp->name[j]->start;                
#endif  
                wr->status = START_EMPTY;
                element->name_sp_stack_no = -1;
                /* remember the element */
                return guththila_stack_push(&wr->element, element, env);
            }
        }
    }
    return GUTHTHILA_FAILURE;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_end_document(
    guththila_xml_writer_t * wr,
    const axutil_env_t * env) 
{
    int i = 0;
    int size = GUTHTHILA_STACK_SIZE(wr->element);
    if (wr->status == START_EMPTY)
        guththila_write_end_element(wr, env);
    /* For all the open elements in the element stack close them */
    for (i = 0; i < size; i++)
    {
        if (!guththila_write_end_element(wr, env))
        {
            return GUTHTHILA_FAILURE;
        }
    }
    return GUTHTHILA_SUCCESS;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_line(
    guththila_xml_writer_t * wr,
    guththila_char_t *element_name,
    guththila_char_t *characters,
    const axutil_env_t * env) 
{
    guththila_write_start_element(wr, element_name, env);
    guththila_write_characters(wr, characters, env);
    guththila_write_end_element(wr, env);
    guththila_write_characters(wr, "\n", env);
    return GUTHTHILA_FAILURE;
}

GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL 
guththila_get_memory_buffer(
    guththila_xml_writer_t * wr,
    const axutil_env_t * env) 
{
    if (wr->type == GUTHTHILA_WRITER_MEMORY)
    {
        return (guththila_char_t *) guththila_buffer_get(&wr->buffer, env);
    }
    return NULL;
}

GUTHTHILA_EXPORT unsigned int GUTHTHILA_CALL 
guththila_get_memory_buffer_size(
    guththila_xml_writer_t * wr,
    const axutil_env_t * env) 
{
    if (wr->type == GUTHTHILA_WRITER_MEMORY)
    {
        return (unsigned int)(wr->buffer.pre_tot_data +
            wr->buffer.data_size[wr->buffer.cur_buff]);
    }
    return 0;
}

GUTHTHILA_EXPORT guththila_char_t *GUTHTHILA_CALL 
guththila_get_prefix_for_namespace(
    guththila_xml_writer_t * wr,
    guththila_char_t *nmsp,
    const axutil_env_t * env) 
{
    int i, j;
    int stack_size = GUTHTHILA_STACK_SIZE(wr->namesp);
    int temp;
    guththila_char_t *str = NULL;
    guththila_xml_writer_namesp_t * writer_namesp = NULL;
    for (i = stack_size - 1; i >= 0; i--)
    {
        writer_namesp =
            (guththila_xml_writer_namesp_t *) guththila_stack_get_by_index(&wr->
                                                                           namesp,
                                                                           i,
                                                                           env);
        temp = writer_namesp->no;
        for (j = 0; j < temp; j++)
        {
            if (!guththila_tok_str_cmp
                 (writer_namesp->uri[j], nmsp, strlen(nmsp), env))
            {
                GUTHTHILA_TOKEN_TO_STRING(writer_namesp->uri[j], str, env);
                return str;
            }
        }
    }
    return NULL;
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL 
guththila_write_to_buffer(
    guththila_xml_writer_t * wr,
    guththila_char_t *buff,
    int size,
    const axutil_env_t * env) 
{
    /* Just write what ever given. But need to close things before */
    if(wr->status == START)
    {
        guththila_write(wr,">", 1u, env);
    }
    if(wr->status == START_EMPTY)
    {
        guththila_write(wr, "/>", 2u, env);      
    }
    guththila_write(wr, buff, size, env);
    wr->status = BEGINING;
    return GUTHTHILA_SUCCESS;
}



