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
#include <stdlib.h>
#include <string.h>
#include <guththila_reader.h>

GUTHTHILA_EXPORT guththila_reader_t * GUTHTHILA_CALL
guththila_reader_create_for_file (char* file_name)
{
	guththila_reader_t *reader = NULL;
	FILE *f = NULL;
	if (!file_name)
		return NULL;
	reader = (guththila_reader_t *) malloc(sizeof(guththila_reader_t));
	if (!reader)
		return NULL;
	f = fopen(file_name, "r");

	if (!f) {
		free(reader);
		return NULL;
	}
	reader->fp = f;
	reader->type = GUTHTHILA_FILE_READER;
	/*guththila_buffer_init(&reader->buffer, 0);*/
	/*reader->next = 0;*/
	/*reader->last_start = -1;*/
	return reader;
}

GUTHTHILA_EXPORT guththila_reader_t * GUTHTHILA_CALL
guththila_reader_create_for_memory(void *buffer, int size)
{
	guththila_reader_t *reader = (guththila_reader_t *) malloc(sizeof(guththila_reader_t));	
	if (reader) {
		reader->type = GUTHTHILA_MEMORY_READER;
		/*guththila_buffer_init_for_buffer(&reader->buffer, buffer, size);	*/
		reader->buff = buffer;
		/*memcpy(reader->buff, buffer, size);*/
		reader->buff_size = size;
		reader->fp = NULL;
		/*reader->next = 0;
		reader->last_start = -1;*/
		return reader;
	}
	return NULL;
}

GUTHTHILA_EXPORT guththila_reader_t * GUTHTHILA_CALL 
guththila_reader_create_for_io(GUTHTHILA_READ_INPUT_CALLBACK input_read_callback, void *ctx)
{
	guththila_reader_t *reader = (guththila_reader_t *)malloc(sizeof(guththila_reader_t));
    if (reader){
		reader->input_read_callback = input_read_callback;
		reader->context = ctx;
		reader->type = GUTHTHILA_IO_READER;
		/*reader->next = 0;*/
		/*guththila_buffer_init(&reader->buffer, 0);*/
		/*reader->last_start = -1;*/
		return reader;
    }
    return NULL;
}

GUTHTHILA_EXPORT void GUTHTHILA_CALL
guththila_reader_free (guththila_reader_t * r)
{
	if (r->type == GUTHTHILA_FILE_READER && r->fp){
		fclose(r->fp);
	}	
	/*guththila_buffer_un_init(&r->buffer);*/
	free(r);
}

GUTHTHILA_EXPORT int GUTHTHILA_CALL
guththila_reader_read (guththila_reader_t *r, guththila_char *buffer, int offset, int length)
{
	int rt = r->type;
	switch (rt) {
		case GUTHTHILA_FILE_READER :	
			return (int)fread(buffer + offset, 1, length, r->fp);		
		case GUTHTHILA_IO_READER :
			return r->input_read_callback((buffer + offset), length, r->context);
		default:	
			return 0;	
	}
	return 0;
}


                       
