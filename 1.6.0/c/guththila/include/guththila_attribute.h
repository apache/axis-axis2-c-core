
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
#ifndef GUTHTHILA_ATTRIBUTE_H
#define GUTHTHILA_ATTRIBUTE_H

#include <guththila_defines.h>
#include <guththila_token.h>
#include <axutil_utils.h>

EXTERN_C_START() 

#ifndef GUTHTHILA_ATTR_DEF_SIZE
#define GUTHTHILA_ATTR_DEF_SIZE 16
#endif  

/* Representation of an attribute */
typedef struct guththila_attr_s
{
    guththila_token_t *pref; /* Prefix */
    guththila_token_t *name; /* Name */
    guththila_token_t *val;  /* Value */ 
} guththila_attr_t;

typedef struct guththila_attr_list_s
{
    guththila_attr_t *list;
    guththila_stack_t fr_stack;
    int size;
    int capacity;
} guththila_attr_list_t;

/**
* Create function of guththila_attr_list_t type structure
* @param env environment, MUST NOT be NULL.
* return new pointer to structure guththila_attr_list_s with initializing stack
* fr_stack 
*/
guththila_attr_list_t *
GUTHTHILA_CALL guththila_attr_list_create(const axutil_env_t * env);

/**
 * Initializing function of guththila_attr_list_t type structure,same
 * thing done by the create method
 * @param at_list keeps the list of attributes in this structure using
 * a guththila_stack_t variable
 * @param env environment, MUST NOT be NULL.
 * return status of op AXIS2_SUCCESS on success,
 * AXIS2_FAILURE on error
 */
int GUTHTHILA_CALL
guththila_attr_list_init(
    guththila_attr_list_t * at_list,
    const axutil_env_t * env);

/**
 * @param at_list keeps the list of attributes in this structure using
 * a guththila_stack_t variable
 * @param env environment, MUST NOT be NULL.
 * return the top value of the stack which is inside guththila_attr_list_t
 */
guththila_attr_t *
GUTHTHILA_CALL guththila_attr_list_get(guththila_attr_list_t * at_list,
        const axutil_env_t * env);


/**
 * This method push the given attribute in to the stack which is a
 * member of guththila_attr_list_t
 * @param at_list keeps the list of attributes in this structure using
 * a guththila_stack_t variable
 * @param attr contains attribute with attribute name,value,and prefix
 * @param env environment, MUST NOT be NULL.
 * return status of op AXIS2_SUCCESS on success,
 * AXIS2_FAILURE on error
 */
int GUTHTHILA_CALL
guththila_attr_list_release(
    guththila_attr_list_t * at_list,
    guththila_attr_t * attr,
    const axutil_env_t * env);

/**
 * Free method for the stack which is inside guththila_attr_list_s
 * structure, free the stack and other members
 * @param at_list keeps the list of attributes in this structure using
 * a guththila_stack_t variable
 * @param env environment, MUST NOT be NULL.
 * return status of op AXIS2_SUCCESS on success,
 * AXIS2_FAILURE on error
 */
void GUTHTHILA_CALL
msuila_attr_list_free_data(
    guththila_attr_list_t * at_list,
    const axutil_env_t * env);

/**
 * Free method for guththila_attr_list_s structure,this free at_list too.
 * @param at_list keeps the list of attributes in this structure using
 * a guththila_stack_t variable
 * @param attr contains attribute with attribute name,value,and prefix
 * @param env environment, MUST NOT be NULL.
 * return status of op AXIS2_SUCCESS on success,
 * AXIS2_FAILURE on error
 */
void GUTHTHILA_CALL
guththila_attr_list_free(
    guththila_attr_list_t * at_list,
    const axutil_env_t * env);

EXTERN_C_END() 
#endif  /*  */

