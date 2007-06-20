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
#include <guththila_buffer.h>

int GUTHTHILA_CALL guththila_buffer_init(guththila_buffer_t *buffer, int size)
{
	buffer->type = GUTHTHILA_BUFFER_FILE;

	buffer->data_size = (size_t *)malloc(sizeof(size_t) * GUTHTHILA_BUFFER_NUMBER_OF_BUFFERS);
	buffer->buffs_size = (size_t *)malloc(sizeof(size_t) * GUTHTHILA_BUFFER_NUMBER_OF_BUFFERS);
	buffer->buff = (char **)malloc(sizeof(char *) * GUTHTHILA_BUFFER_NUMBER_OF_BUFFERS);
	buffer->cur_buff = -1;
	buffer->pre_tot_data = 0;
	buffer->no_buffers = GUTHTHILA_BUFFER_NUMBER_OF_BUFFERS;
	if (size > 0 ){
		buffer->buff[0] = (guththila_char *)malloc(sizeof(guththila_char) * size);	
		buffer->data_size[0] = 0;
		buffer->buffs_size[0] = size;
		buffer->cur_buff = 0;
	}		
	return GUTHTHILA_SUCCESS;
}

int GUTHTHILA_CALL guththila_buffer_un_init(guththila_buffer_t *buffer) 
{
	int i = 0;	
	if (buffer->type == GUTHTHILA_BUFFER_MEMORY && buffer->buff && buffer->cur_buff == 0) {
		if (buffer->buff[0]) free(buffer->buff[0]);
		if (buffer->buffs_size) free(buffer->buffs_size);
		if (buffer->data_size) free(buffer->data_size);
		free(buffer->buff);
	} else if (buffer->type == GUTHTHILA_BUFFER_MEMORY && buffer->buff){
		for (i = 0; i <= buffer->cur_buff; i++) {
			free(buffer->buff[i]);
		}
		free(buffer->buff);
		if (buffer->data_size) free(buffer->data_size);
		if (buffer->buffs_size) free(buffer->buffs_size);
	}
	return GUTHTHILA_SUCCESS;
}

int GUTHTHILA_CALL guththila_buffer_init_for_buffer (guththila_buffer_t *buffer, char *buff, int size)
{
	buffer->type = GUTHTHILA_BUFFER_MEMORY;	

	buffer->buff = (char **)malloc(sizeof(char *) * GUTHTHILA_BUFFER_DEF_SIZE);
	buffer->buff[0] = buff;
	buffer->cur_buff = 0;
	buffer->buffs_size = (size_t *)malloc(sizeof(size_t) * GUTHTHILA_BUFFER_DEF_SIZE);
	buffer->buffs_size[0] = size;
	buffer->pre_tot_data = 0;
	buffer->data_size = (size_t *)malloc(sizeof(size_t) * GUTHTHILA_BUFFER_DEF_SIZE);
	buffer->data_size[0] = size;
	buffer->no_buffers = 1;
	return GUTHTHILA_SUCCESS;
}


int GUTHTHILA_CALL guththila_buffer_insert_data (guththila_buffer_t * buffer, void *buff, size_t buff_len)
{
	size_t no_written = 0;
	if (buffer->data_size[buffer->cur_buff] + buff_len <= buffer->buffs_size[buffer->cur_buff]) {
		memcpy(buffer->buff[buffer->cur_buff] + buffer->cur_buff_pos, buff, buff_len);
	} else {
		do {
			if (buff_len > no_written + buffer->buffs_size[buffer->cur_buff] - buffer->cur_buff_pos) {
				memcpy(buffer->buff[buffer->cur_buff] + buffer->cur_buff_pos, (char *)buff + no_written, buffer->buffs_size[buffer->cur_buff] - buffer->cur_buff_pos);
				no_written = buffer->buffs_size[buffer->cur_buff] - buffer->cur_buff_pos;			
				buffer->data_size[buffer->cur_buff] = buffer->buffs_size[buffer->cur_buff];
				if (buffer->cur_buff == buffer->no_buffers - 1) {
					buffer->data_size = (size_t *)realloc(buffer->data_size, buffer->no_buffers * 2);
					buffer->buffs_size = (size_t *)realloc(buffer->data_size, buffer->no_buffers * 2);
					buffer->buff = (char **)realloc(buffer->buff, buffer->no_buffers * 2);
					buffer->no_buffers *= 2;
				}
				buffer->buff[buffer->cur_buff + 1] = (char *)malloc(sizeof(char) * buffer->data_size[buffer->cur_buff] * 2);    
				buffer->cur_buff++;
				buffer->buffs_size[buffer->cur_buff] = buffer->data_size[buffer->cur_buff - 1] * 2;
				buffer->data_size[buffer->cur_buff] = 0;
				buffer->pre_tot_data += buffer->data_size[buffer->cur_buff - 1];
			} else {
				memcpy(buffer->buff[buffer->cur_buff] + buffer->cur_buff_pos, (char *)buff + no_written, buff_len - no_written);
				buffer->data_size[buffer->cur_buff] = 0;
				break;
			}			
		} while (1);
	}
	return GUTHTHILA_SUCCESS;
}

int GUTHTHILA_CALL guththila_buffer_shift(guththila_buffer_t * buffer, int no)
{
	size_t i = 0, sum = 0, data_move = 0;
	for (i = 0; i < (size_t)buffer->cur_buff; i++) {
		sum += buffer->data_size[i];							
	}
	data_move = buffer->data_size[buffer->cur_buff] - (no - sum);	
	if (buffer->cur_buff == buffer->no_buffers - 1) {
		buffer->data_size = (size_t *)realloc(buffer->data_size, buffer->no_buffers * 2);
		buffer->buffs_size = (size_t *)realloc(buffer->data_size, buffer->no_buffers * 2);
		buffer->buff = (char **)realloc(buffer->buff, buffer->no_buffers * 2);
		buffer->no_buffers *= 2;
	}
	buffer->buff[buffer->cur_buff + 1] = (char *)malloc(sizeof(char) * buffer->data_size[buffer->cur_buff] * 2);    
	buffer->cur_buff++;
	buffer->buffs_size[buffer->cur_buff] = buffer->data_size[buffer->cur_buff - 1] * 2;
	buffer->data_size[buffer->cur_buff] = data_move;
	memcpy(buffer->buff[buffer->cur_buff], buffer->buff[buffer->cur_buff - 1] + buffer->data_size[buffer->cur_buff] - data_move, data_move); 
	buffer->data_size[buffer->cur_buff] -= data_move; 
	buffer->pre_tot_data += buffer->data_size[buffer->cur_buff - 1];
	return GUTHTHILA_SUCCESS;
}

void * GUTHTHILA_CALL guththila_buffer_get(guththila_buffer_t * buffer)
{
	size_t size = 0, i = 0, current_size = 0;
	char *buff = NULL;
	for (i = 0; i <= (size_t)buffer->cur_buff; i++) {
		size += buffer->data_size[i];
	}
	buff = (char *) malloc(sizeof(char) * size);

	for (i = 0; i <= buffer->cur_buff; i++) {
		memcpy(buff + current_size, buffer->buff[i] , buffer->data_size[i]);
		current_size += buffer->data_size[i];
	}
	return buff;
}

void * GUTHTHILA_CALL guththila_get_position(guththila_buffer_t *buffer, int pos)
{
	if (pos > buffer->pre_tot_data) {
		return buffer->buff[buffer->cur_buff] + pos - buffer->pre_tot_data;
	}
	return NULL;
}

