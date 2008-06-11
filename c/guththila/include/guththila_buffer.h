
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
#ifndef GUTHTHILA_BUFFER_H
#define GUTHTHILA_BUFFER_H

#include <guththila_defines.h>
#include <axutil_utils.h>

EXTERN_C_START()

typedef enum guththila_buffer_type
{
    GUTHTHILA_SINGLE_BUFFER = 0, /* One buffer */
    GUTHTHILA_MULTIPLE_BUFFER    /* Mulitple buffers in a buff array */
} guththila_buffer_type_t;

typedef struct guththila_buffer_s
{
    /* Required to manupulate multiple buffers */
    size_t *data_size;			/* Array containing filled sizes of buffers */
    size_t *buffs_size;			/* Array containing actual sizes of buffers */
    guththila_char_t **buff;	/* Array of buffers */
    int cur_buff;				/* Current buffer */
    int cur_buff_pos;			/* Position of the current buffer */
    size_t pre_tot_data;		/* All the data in the previous buffers. Not include cur */
    unsigned int no_buffers;	/* No of buffers */
    short type;					/* Buffer type */
    guththila_char_t *xml;		/* All the buffers serialized together */
} guththila_buffer_t;

#define GUTHTHILA_BUFFER_DEF_SIZE 16384
#define GUTHTHILA_BUFFER_NUMBER_OF_BUFFERS 16

#ifndef GUTHTHILA_BUFFER_SIZE
#define GUTHTHILA_BUFFER_SIZE(_buffer) (_buffer.size)
#endif

#ifndef GUTHTHILA_BUFFER_CURRENT_BUFF
#define GUTHTHILA_BUFFER_CURRENT_BUFF(_buffer) ((_buffer).buff[(_buffer).cur_buff] + (_buffer).data_size[(_buffer).cur_buff])
#endif

#ifndef GUTHTHILA_BUFFER_CURRENT_BUFF_SIZE
#define GUTHTHILA_BUFFER_CURRENT_BUFF_SIZE(_buffer) ((_buffer).buffs_size[(_buffer).cur_buff] - (_buffer).data_size[(_buffer).cur_buff])
#endif

#ifndef GUTHTHILA_BUFFER_CURRENT_DATA_SIZE
#define GUTHTHILA_BUFFER_CURRENT_DATA_SIZE(_buffer) ((_buffer).data_size[(_buffer).cur_buff])
#endif

#ifndef GUTHTHILA_BUFFER_PRE_DATA_SIZE
#define GUTHTHILA_BUFFER_PRE_DATA_SIZE(_buffer) ((_buffer).pre_tot_data)
#endif

/*We never consider tokens not in the current buffer*/
#ifndef GUTHTHILA_BUF_POS
#define GUTHTHILA_BUF_POS(_buffer, _pos) ((_buffer).buff[(_buffer).cur_buff] + _pos - (_buffer).pre_tot_data)
#endif

/**
 * This method is the create method of guththila_buffer_s structure
 * @param buffer structure which is going to create
 * @param size size of the buffer which is going to create
 * @param env environment, MUST NOT be NULL.
 * return status of op AXIS2_SUCCESS on success,
 * AXIS2_FAILURE on error
 */
int GUTHTHILA_CALL 
guththila_buffer_init(guththila_buffer_t * buffer,
					  int size,
					  const axutil_env_t * env);

/**
 * This is the free method of guththila_buffer_s structure
 * @param buffer structure which is going to create
 * @param env environment, MUST NOT be NULL.
 * return status of op AXIS2_SUCCESS on success,
 * AXIS2_FAILURE on error
 */
int GUTHTHILA_CALL 
guththila_buffer_un_init(guththila_buffer_t * buffer,
					   const axutil_env_t * env);

/**
 * This method creates a new buffer and copy the content of given
 * data by buffer variable
 * @param mu_buff structure which is going to create
 * @param buffer data to copy in to new buffer
 * @param size size of the buffer to create
 * @param env environment, MUST NOT be NULL.
 * return status of op AXIS2_SUCCESS on success,
 * AXIS2_FAILURE on error
 */
int GUTHTHILA_CALL 
guththila_buffer_init_for_buffer(guththila_buffer_t * mu_buff, 
								 guththila_char_t *buffer, 
								 int size, 
								 const axutil_env_t * env);

void *GUTHTHILA_CALL 
guththila_get_position(guththila_buffer_t * buffer,
					   int pos, 
					   const axutil_env_t * env);

int GUTHTHILA_CALL 
guththila_buffer_next(guththila_buffer_t * buffer, 
					  const axutil_env_t * env);


/**
 * This method create new xml element which is having the 
 * size of cur_buff
 * data by buffer variable
 * @param buffer 
 * @param env environment, MUST NOT be NULL.
 * return xml element of guththila_buffer_s structure 
 */
void *GUTHTHILA_CALL 
guththila_buffer_get(guththila_buffer_t * buffer, 
					 const axutil_env_t * env);


int GUTHTHILA_CALL 
guththila_buffer_shift(guththila_buffer_t * buffer, 
					   int no, const axutil_env_t * env);

int GUTHTHILA_CALL 
guththila_buffer_insert_data(guththila_buffer_t * buffer, 
							 void *buff, size_t buff_len, 
							 const axutil_env_t * env);

EXTERN_C_END()
#endif




