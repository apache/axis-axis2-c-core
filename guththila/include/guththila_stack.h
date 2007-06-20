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
#ifndef GUTHTHILA_STACK_H
#define GUTHTHILA_STACK_H

#include <stdio.h>
#include <stdlib.h>

#include <guththila_defines.h>

#define GUTHTHILA_STACK_DEFAULT	16

EXTERN_C_START()

typedef struct guththila_stack_s
{
	/* Number of Items in the stack*/
	int top;
	/* Max number of Items that can be hold in data*/
	int max;
	void **data;			
} guththila_stack_t;

#ifndef GUTHTHILA_STACK_SIZE
#define GUTHTHILA_STACK_SIZE(_stack) ((_stack).top)
#endif

#ifndef GUTHTHILA_STACK_TOP_INDEX
#define GUTHTHILA_STACK_TOP_INDEX(_stack) (((_stack).top - 1))
#endif

guththila_stack_t* GUTHTHILA_CALL guththila_stack_create();
int GUTHTHILA_CALL guththila_stack_init(guththila_stack_t *stack);
void GUTHTHILA_CALL guththila_stack_free(guththila_stack_t *stack);
void GUTHTHILA_CALL guththila_stack_free_data(guththila_stack_t *stack);
void * GUTHTHILA_CALL guththila_stack_pop(guththila_stack_t *stack);
int GUTHTHILA_CALL guththila_stack_push(guththila_stack_t *stack, void *data);
void * GUTHTHILA_CALL guththila_stack_peek(guththila_stack_t *stack);
void * GUTHTHILA_CALL guththila_stack_get_by_index(guththila_stack_t *stack, int index);
int GUTHTHILA_CALL guththila_stack_del_top(guththila_stack_t *stack);
int GUTHTHILA_CALL guththila_stack_is_empty(guththila_stack_t *stack);

EXTERN_C_END()

#endif

