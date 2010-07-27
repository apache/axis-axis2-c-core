
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

#include <axiom.h>
#include <stdlib.h>

#include <tcpmon_util.h>

#define START_ELEMENT 1
#define CHAR_VALUE 2
#define END_ELEMENT 3
#define EMPTY_ELEMENT 4

typedef struct tcpmon_util_allocator
{
    int allocated;
    int index;
    axis2_char_t *buffer;
}
tcpmon_util_allocator_t;

/*static void add_string(const axutil_env_t* env,
		       tcpmon_util_allocator_t* allocator,
		       axis2_char_t* string);
*/

/*static void add_axis2_char_t(const axutil_env_t* env,
			     tcpmon_util_allocator_t* allocator,
			     axis2_char_t c,
			     int turns);
*/
axis2_char_t *
tcpmon_util_format_as_xml(
    const axutil_env_t * env,
    axis2_char_t * data,
    int format)
{
    if (format)
    {
        int c;
        int tab_pos = 0;
        int has_value = 0;
        int has_space = 0;
        int start_ele = 0;
        int prev_case = 0;
        int buffer_size = 0;

        axis2_char_t *out;

        axiom_xml_reader_t *xml_reader = NULL;

        buffer_size = 2 * ((int)strlen(data));
        /* We are sure that the difference lies within the int range */
        out = AXIS2_MALLOC(env->allocator, buffer_size * sizeof(axis2_char_t));

        if (data)
        {
            int size = 0;
            size = (int)strlen(data);
            /* We are sure that the difference lies within the int range */
            xml_reader =
                axiom_xml_reader_create_for_memory(env, data, size, "utf-8",
                                                   AXIS2_XML_PARSER_TYPE_BUFFER);
            if (!xml_reader)
                return NULL;
        }

        axiom_xml_reader_init();

        while ((c = axiom_xml_reader_next(xml_reader, env)) != -1)
        {
            switch (c)
            {
            case AXIOM_XML_READER_START_DOCUMENT:
                {
                    int ix;

                    tcpmon_util_strcat(out, "<?xml ", &buffer_size, env);

                    ix = axiom_xml_reader_get_attribute_count(xml_reader, env);
                    for (; ix > 0; ix--)
                    {
                        axis2_char_t *attr_prefix;
                        axis2_char_t *attr_name;
                        axis2_char_t *attr_value;

                        attr_prefix =
                            (axis2_char_t *)
                            axiom_xml_reader_get_attribute_prefix_by_number
                            (xml_reader, env, ix);
                        if (attr_prefix)
                        {
                            tcpmon_util_strcat(out, attr_prefix, &buffer_size, env);
                            tcpmon_util_strcat(out, ":", &buffer_size, env);
                        }

                        attr_name =
                            (axis2_char_t *)
                            axiom_xml_reader_get_attribute_name_by_number
                            (xml_reader, env, ix);
                        if (attr_name)
                        {
                            tcpmon_util_strcat(out, attr_name, &buffer_size, env);
                            tcpmon_util_strcat(out, "=\"", &buffer_size, env);
                        }

                        attr_value =
                            (axis2_char_t *)
                            axiom_xml_reader_get_attribute_value_by_number
                            (xml_reader, env, ix);
                        if (attr_value)
                        {
                            tcpmon_util_strcat(out, attr_value, &buffer_size, env);
                            tcpmon_util_strcat(out, "\"", &buffer_size, env);
                        }
                    }

                    printf("?>");
                }
                break;
            case AXIOM_XML_READER_START_ELEMENT:
                {
                    int i,
                     ix,
                     has_prefix = 0;

                    axis2_char_t *ele_name;
                    axis2_char_t *ele_prefix;

                    prev_case = START_ELEMENT;

                    has_value = 0;
                    has_space = 0;

                    if (start_ele != 0)
                        tcpmon_util_strcat(out, "\n", &buffer_size, env);

                    for (i = 0; i < tab_pos; i++)
                        tcpmon_util_strcat(out, "\t", &buffer_size, env);

                    tcpmon_util_strcat(out, "<", &buffer_size, env);

                    ele_prefix =
                        (axis2_char_t *) axiom_xml_reader_get_prefix(xml_reader,
                                                                     env);
                    if (ele_prefix)
                    {
                        tcpmon_util_strcat(out, ele_prefix, &buffer_size, env);
                        tcpmon_util_strcat(out, ":", &buffer_size, env);
                    }

                    ele_name =
                        (axis2_char_t *) axiom_xml_reader_get_name(xml_reader,
                                                                   env);
                    if (ele_name)
                    {
                        tcpmon_util_strcat(out, ele_name, &buffer_size, env);
                    }

                    ix = axiom_xml_reader_get_attribute_count(xml_reader, env);
                    for (; ix > 0; ix--)
                    {
                        axis2_char_t *attr_prefix;
                        axis2_char_t *attr_name;
                        axis2_char_t *attr_value;

                        attr_prefix =
                            (axis2_char_t *)
                            axiom_xml_reader_get_attribute_prefix_by_number
                            (xml_reader, env, ix);
                        if (attr_prefix)
                        {
                            has_prefix = 1;
                            tcpmon_util_strcat(out, " ", &buffer_size, env);
                            tcpmon_util_strcat(out, attr_prefix, &buffer_size, env);
                            tcpmon_util_strcat(out, ":", &buffer_size, env);
                        }

                        attr_name =
                            (axis2_char_t *)
                            axiom_xml_reader_get_attribute_name_by_number
                            (xml_reader, env, ix);
                        if (attr_name)
                        {
                            if (has_prefix)
                            {
                                tcpmon_util_strcat(out, attr_name, &buffer_size,
                                              env);
                                tcpmon_util_strcat(out, "=\"", &buffer_size, env);
                            }
                            else
                            {
                                tcpmon_util_strcat(out, " ", &buffer_size, env);
                                tcpmon_util_strcat(out, attr_name, &buffer_size,
                                              env);
                                tcpmon_util_strcat(out, "=\"", &buffer_size, env);
                            }

                            has_prefix = 0;
                        }

                        attr_value =
                            (axis2_char_t *)
                            axiom_xml_reader_get_attribute_value_by_number
                            (xml_reader, env, ix);
                        if (attr_value)
                        {
                            tcpmon_util_strcat(out, attr_value, &buffer_size, env);
                            tcpmon_util_strcat(out, "\"", &buffer_size, env);
                        }
                    }

                    tcpmon_util_strcat(out, ">", &buffer_size, env);

                    tab_pos++;
                    start_ele = 1;
                }
                break;
            case AXIOM_XML_READER_CHARACTER:
                {
                    axis2_char_t *ele_value;

                    prev_case = CHAR_VALUE;

                    ele_value = axiom_xml_reader_get_value(xml_reader, env);
                    if (ele_value)
                        tcpmon_util_strcat(out, ele_value, &buffer_size, env);

                    has_value = 1;

                }
                break;
            case AXIOM_XML_READER_EMPTY_ELEMENT:
                {
                    int i,
                     ix,
                     has_prefix = 0;

                    axis2_char_t *ele_name;
                    axis2_char_t *ele_prefix;

                    prev_case = EMPTY_ELEMENT;

                    has_value = 0;
                    has_space = 0;

                    if (start_ele != 0)
                        tcpmon_util_strcat(out, "\n", &buffer_size, env);

                    for (i = 0; i < tab_pos; i++)
                        tcpmon_util_strcat(out, "\t", &buffer_size, env);

                    tcpmon_util_strcat(out, "<", &buffer_size, env);

                    ele_prefix =
                        (axis2_char_t *) axiom_xml_reader_get_prefix(xml_reader,
                                                                     env);
                    if (ele_prefix)
                    {
                        tcpmon_util_strcat(out, ele_prefix, &buffer_size, env);
                        tcpmon_util_strcat(out, ":", &buffer_size, env);
                    }

                    ele_name =
                        (axis2_char_t *) axiom_xml_reader_get_name(xml_reader,
                                                                   env);
                    if (ele_name)
                        tcpmon_util_strcat(out, ele_name, &buffer_size, env);

                    ix = axiom_xml_reader_get_attribute_count(xml_reader, env);
                    for (; ix > 0; ix--)
                    {
                        axis2_char_t *attr_prefix;
                        axis2_char_t *attr_name;
                        axis2_char_t *attr_value;

                        attr_prefix =
                            (axis2_char_t *)
                            axiom_xml_reader_get_attribute_prefix_by_number
                            (xml_reader, env, ix);
                        if (attr_prefix)
                        {
                            has_prefix = 1;
                            tcpmon_util_strcat(out, " ", &buffer_size, env);
                            tcpmon_util_strcat(out, attr_prefix, &buffer_size, env);
                            tcpmon_util_strcat(out, ":", &buffer_size, env);
                        }

                        attr_name =
                            (axis2_char_t *)
                            axiom_xml_reader_get_attribute_name_by_number
                            (xml_reader, env, ix);
                        if (attr_name)
                        {
                            if (has_prefix)
                            {
                                tcpmon_util_strcat(out, attr_name, &buffer_size,
                                              env);
                                tcpmon_util_strcat(out, "=\"", &buffer_size, env);
                            }
                            else
                            {
                                tcpmon_util_strcat(out, " ", &buffer_size, env);
                                tcpmon_util_strcat(out, attr_name, &buffer_size,
                                              env);
                                tcpmon_util_strcat(out, "=\"", &buffer_size, env);
                            }
                            has_prefix = 0;
                        }

                        attr_value =
                            (axis2_char_t *)
                            axiom_xml_reader_get_attribute_value_by_number
                            (xml_reader, env, ix);
                        if (attr_value)
                        {
                            tcpmon_util_strcat(out, attr_value, &buffer_size, env);
                            tcpmon_util_strcat(out, "\"", &buffer_size, env);
                        }
                    }

                    tcpmon_util_strcat(out, "/>", &buffer_size, env);
                    start_ele = 1;
                }
                break;
            case AXIOM_XML_READER_END_ELEMENT:
                {
                    int i;

                    axis2_char_t *ele_prefix;
                    axis2_char_t *ele_name;

                    tab_pos--;

                    if (has_value == 0 && prev_case != START_ELEMENT)
                    {
                        tcpmon_util_strcat(out, "\n", &buffer_size, env);
                        for (i = 0; i < tab_pos; i++)
                            tcpmon_util_strcat(out, "\t", &buffer_size, env);
                    }

                    has_value = 0;

                    tcpmon_util_strcat(out, "</", &buffer_size, env);

                    ele_prefix =
                        (axis2_char_t *) axiom_xml_reader_get_prefix(xml_reader,
                                                                     env);
                    if (ele_prefix)
                    {
                        tcpmon_util_strcat(out, ele_prefix, &buffer_size, env);
                        tcpmon_util_strcat(out, ":", &buffer_size, env);
                    }

                    ele_name =
                        (axis2_char_t *) axiom_xml_reader_get_name(xml_reader,
                                                                   env);
                    if (ele_name)
                    {
                        tcpmon_util_strcat(out, ele_name, &buffer_size, env);
                        tcpmon_util_strcat(out, ">", &buffer_size, env);
                    }
                    prev_case = END_ELEMENT;

                }
                break;
            }
        }
        return out;
    }
    return data;
}

/*void add_string(const axutil_env_t* env,
		tcpmon_util_allocator_t* allocator,
		axis2_char_t* string)
{
  int additional_len = 0;
  void* dest = NULL;
  void* src = NULL;
  int count = 0;

  additional_len = axutil_strlen(string) + 1;
  if (allocator-> index + additional_len  >= allocator-> allocated)
    {
      if (allocator-> allocated == 0)
        {
	  allocator-> buffer =
	    AXIS2_MALLOC(env-> allocator, additional_len);
        }
      else
        {
	  allocator-> buffer =
	    AXIS2_REALLOC(env-> allocator, allocator-> buffer,
			  allocator-> allocated + additional_len);
        }
      allocator-> allocated += additional_len;
    }

  dest = allocator-> buffer + allocator-> index;
  src = string;
  count = additional_len; 
  memcpy(dest, src, count);

  allocator-> index += count - 1;
}
*/

/*void add_axis2_char_t(const axutil_env_t* env,
		      tcpmon_util_allocator_t* allocator,
		      axis2_char_t c,
		      int turns)
{
  int additional_len = 0;

  additional_len = turns + 1;
  if (allocator-> index + additional_len  >= allocator-> allocated)
    {
      if (allocator-> allocated == 0)
        {
	  allocator-> buffer =
	    AXIS2_MALLOC(env-> allocator, additional_len);
        }
      else
        {
	  allocator-> buffer =
	    AXIS2_REALLOC(env-> allocator, allocator-> buffer,
			  allocator-> allocated + additional_len);
        }
      allocator-> allocated += additional_len;
    }

  memset(allocator-> buffer + allocator-> index, c, turns);

  allocator-> index += turns;

} */

axis2_char_t *
tcpmon_util_strcat(
    axis2_char_t * dest,
    axis2_char_t * source,
    int *buff_size,
    const axutil_env_t * env)
{
    int cur_len = 0;
    int source_len = 0;

    axis2_char_t *tmp;
    cur_len = (int)strlen(dest);
    /* We are sure that the difference lies within the int range */
    source_len = (int)strlen(source);
    /* We are sure that the difference lies within the int range */

    if ((*buff_size - cur_len) < source_len)
    {
        *buff_size = *buff_size + (*buff_size * 2);
        tmp =
            (axis2_char_t *) AXIS2_REALLOC(env->allocator, dest,
                                           *buff_size * sizeof(axis2_char_t));
        dest = tmp;
        strcat((char *) dest, (char *) source);
    }
    else
    {
        strcat((char *) dest, (char *) source);
    }

    return dest;
}

char *
tcpmon_util_str_replace(
    const axutil_env_t *env,    
    char *str,
    const char *search,
    const char *replace)
{
    char *str_return = NULL;
    char *str_tmp = NULL;
    char *str_relic = NULL;
    int size = ((int)strlen(str)) * 2;
    /* We are sure that the difference lies within the int range */
    int addmem = size;
    int diff = (int)(strlen(replace) - strlen(search));
    /* We are sure that the difference lies within the int range */

    str_return = (char *) AXIS2_MALLOC(env->allocator, ((size + 1) * sizeof(char)));
    str_tmp = (char *) AXIS2_MALLOC(env->allocator, (size * sizeof(char)));
    

    if (str_return == NULL || str_tmp == NULL)
    {
        AXIS2_FREE(env->allocator, str_return);
        str_return = NULL;
        AXIS2_FREE(env->allocator, str_tmp);
        str_tmp = NULL;
        return "function tcpmon_util_str_replace : give me more memory";
    }
    if (!strcmp(search, replace))
    {
        AXIS2_FREE(env->allocator, str_return);
        str_return = NULL;
        AXIS2_FREE(env->allocator, str_tmp);
        str_tmp = NULL;
        return str;
    }

    strcpy(str_return, str);

    while ((str_relic = strstr(str_return, search)) != NULL)
    {
        if ((int)strlen(str_return) + diff >= addmem)
            /* We are sure that the difference lies within the int range */
        {
            str_return = (char *) realloc(str_return, addmem += size);
            str_tmp = (char *) realloc(str_tmp, addmem);

            if (str_return == NULL || str_tmp == NULL)
            {
                AXIS2_FREE(env->allocator, str_return);
                str_return = NULL;
                AXIS2_FREE(env->allocator, str_tmp);
                str_tmp = NULL;
                return "function tcpmon_str_replace : gimme more memory";
            }
        }

        strcpy(str_tmp, replace);
        strcat(str_tmp, (str_relic + strlen(search)));
        *str_relic = '\0';

        strcat(str_return, str_tmp);
    }

    AXIS2_FREE(env->allocator, str_tmp);
    str_tmp = NULL;
    /* free(str); */ /* we are not allocating memory using str */
    str_return[addmem] = '\0';
    return (str_return);
}


axis2_char_t *
tcpmon_util_read_current_stream(
    const axutil_env_t * env,
    axutil_stream_t * stream,
    int *stream_size,
    axis2_char_t ** header,
    axis2_char_t ** data)
{
    int read_size = 0;
    axis2_char_t *buffer = NULL;
    axis2_char_t *header_ptr = NULL;
    axis2_char_t *body_ptr = NULL;
    int header_found = 0;
    int header_just_finished = 0;
    int read = 0;
    int header_width = 0;
    int current_line_offset = 0;
    int mtom_optimized = 0;
    axis2_char_t *current_line = NULL;
    int line_just_ended = 1;
    axis2_char_t *length_char = 0;
    int length = -1;
    int chunked_encoded = 0;
    int is_get = 0;
    int zero_content_length = 0;

    buffer = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t));
    *data = NULL;
    *header = NULL;
    do
    {
        buffer = AXIS2_REALLOC(env->allocator, buffer,
                               sizeof(axis2_char_t) * (read_size + 1));
        *(buffer + read_size) = '\0';
        read = axutil_stream_read(stream, env, buffer + read_size, 1);

        if (header_just_finished)
        {
            header_just_finished = 0;
            header_width = read_size;
        }

                          /** identify the content lenth*/
        if (!header_found && *(buffer + read_size) == '\r')
        {
            *(buffer + read_size) = '\0';
            current_line = buffer + current_line_offset;
            if (!mtom_optimized && strstr(current_line, "multipart/related"))
                mtom_optimized = 1;
            if (strstr(current_line, "Content-Length"))
            {
                length_char = strstr(current_line, ":");
                if (length_char)
                {
                    length_char++;
                    length = atoi(length_char);
                    if (length == 0)
                    {
                        zero_content_length = 1;
                    }
                }
            }
            if (strstr(current_line, "GET") || strstr(current_line, "HEAD")
                || strstr(current_line, "DELETE"))
            {
                is_get = 1;
                    /** Captures GET style requests */
            }
            *(buffer + read_size) = '\r';
        }
        if (!header_found && line_just_ended)
        {
            line_just_ended = 0;
            current_line_offset = read_size;
        }
        if (!header_found && *(buffer + read_size) == '\n')
        {
            line_just_ended = 1;    /* set for the next loop to read */
        }
        if (header_found)
        {
            length--;
        }
        if (header_found &&
            read_size >= 4 &&
            chunked_encoded == 1 &&
            *(buffer + read_size) == '\n' &&
            *(buffer + read_size - 1) == '\r' &&
            *(buffer + read_size - 2) == '\n' &&
            *(buffer + read_size - 3) == '\r' &&
            *(buffer + read_size - 4) == '0')
        {

            length = 0;          /** this occurs in chunked transfer encoding */
        }

                          /** identify the end of the header */
        if (!header_found &&
            read_size >= 3 &&
            *(buffer + read_size) == '\n' &&
            *(buffer + read_size - 1) == '\r' &&
            *(buffer + read_size - 2) == '\n' &&
            *(buffer + read_size - 3) == '\r')
        {
            header_found = 1;
            *(buffer + read_size - 3) = '\0';
            if (header_ptr)
            {
                AXIS2_FREE(env->allocator, header_ptr);
            }
            header_ptr = (axis2_char_t *) axutil_strdup(env, buffer);
            header_just_finished = 1;
            *(buffer + read_size - 3) = '\r';
            if (is_get && length == -1)
            {
                break;
            }
        }
        read_size++;
        if (!chunked_encoded && length < -1)
        {
            header_width = 0;
            /* break; */

                                  /** this is considered as transfer-encoding = chunked */
            chunked_encoded = 1;
            header_found = 1;
            *(buffer + read_size - 3) = '\0';
            if (header_ptr)
            {
                AXIS2_FREE(env->allocator, header_ptr); 
            }
            header_ptr = (axis2_char_t *) axutil_strdup(env, buffer);
            header_just_finished = 1;
            *(buffer + read_size - 3) = '\r';
        }
        if (!(*(buffer + read_size - 1)))
        {
            if (!mtom_optimized)
            {
                read_size--;
                length = 0;
            }
            else
            {
                /**(buffer + read_size - 1) = ' ';*/
            }
        }
    }
    while (length != 0);
    
    if (is_get)
    {
        read_size++;
    }
    else if (zero_content_length)
    {
        read_size += 3;
    }

    buffer = AXIS2_REALLOC(env->allocator, buffer,
                           sizeof(axis2_char_t) * (read_size + 1));
    *(buffer + read_size) = '\0';
    
    if (header_width != 0)
    {
        body_ptr = buffer + header_width;
        if (body_ptr && *body_ptr)
        {
            if (mtom_optimized)
            {
                int count = read_size - (int)strlen(header_ptr) - 4;
                int copied = 0;
                int plen = 0;
                axis2_char_t *temp = NULL;
                temp = AXIS2_MALLOC(env->allocator,
                              sizeof(axis2_char_t) * count + 1);
                while(count > copied)
                {
                    plen = 0;
                    plen = ((int)strlen(body_ptr) + 1);
                    if (plen != 1)
                    {
                        sprintf(temp, "%s", body_ptr);
                    }
                    copied += plen;
                    if (count > copied)
                    {
                        temp += plen;
                        body_ptr += plen;
                    }
                }
                copied -= plen;
                temp -= copied;
                temp[count] = '\0';
                *data = temp;
            }
            else
            {
                *data = (axis2_char_t *) axutil_strdup(env, body_ptr);
            }
        }
        body_ptr = NULL;    
    }
    else
    {
        /** soap body part is unavailable */
        if (is_get)
        {
            *data = (axis2_char_t *) axutil_strdup(env, "\n");
            *(buffer + read_size - 1) = '\n';
        }
        else if (zero_content_length)
        {
            *data = (axis2_char_t *) axutil_strdup(env, "\n");
            *(buffer + read_size - 3) = '\n';
            *(buffer + read_size - 2) = '\r';
            *(buffer + read_size - 1) = '\n';
        }
        if (header_ptr)
        {
            AXIS2_FREE(env->allocator, header_ptr);
        }
        header_ptr = (axis2_char_t *) axutil_strdup(env, buffer);
    }

    *header = header_ptr;
    *stream_size = read_size;
    return buffer;
}


int
tcpmon_util_write_to_file(
    char *filename,
    char *buffer)
{
    int size = 0;
    if (filename)
    {
        FILE *fp = fopen(filename, "ab");
        size = (int)fwrite(buffer, 1, strlen(buffer), fp);
        /* We are sure that the difference lies within the int range */
        fclose(fp);
    }
    return size;
}

