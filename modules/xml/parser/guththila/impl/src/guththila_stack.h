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
 *	
 * @author Dinesh Premalal (xydinesh@gmail.com, premalwd@cse.mrt.ac.lk)	
 */


#ifndef GUTHTHILA_STACK_H
#define GUTHTHILA_STACK_H
#include "guththila_environment.h"
#include "guththila_token.h"
#include "guththila_attribute.h"
#include "guththila_namespace.h"
#include "guththila_depth.h"

enum guththila_element_types
{
    token_element,
    attribute_element,
    namespace_element
};

typedef struct guththila_element_s guththila_element_t;

struct guththila_element_s
{
    guththila_token_t *token;
    guththila_attribute_t *attribute;
    guththila_namespace_t *namespace;
    guththila_depth_t *depth;
    guththila_element_t *prev;
};

typedef struct guththila_stack_s
{
    enum guththila_element_types element_type;
    int pointer;
    int current_pos;
    guththila_element_t *tail;
    guththila_element_t *current;
} guththila_stack_t;

/* stack implementation */
GUTHTHILA_DECLARE (guththila_stack_t *)
guththila_stack_create (guththila_environment_t * environment);
GUTHTHILA_DECLARE (int)
guththila_stack_push (guththila_environment_t * environment,
                      guththila_stack_t * st, guththila_token_t * tok,
                      guththila_attribute_t * attr);
GUTHTHILA_DECLARE (int)
guththila_stack_size (guththila_environment_t * environment,
                      guththila_stack_t * st);
GUTHTHILA_DECLARE (void)
guththila_stack_free (guththila_environment_t * environment,
                      guththila_stack_t * st);
GUTHTHILA_DECLARE (void)
guththila_stack_free_rec (guththila_environment_t * environment,
                          guththila_stack_t * st, guththila_element_t * el);
GUTHTHILA_DECLARE (guththila_element_t *)
guththila_stack_last (guththila_environment_t * environment,
                      guththila_stack_t * st);
GUTHTHILA_DECLARE (guththila_element_t *)
guththila_stack_pull (guththila_environment_t * environment,
                      guththila_stack_t * st);
GUTHTHILA_DECLARE (int)
guththila_stack_push_namespace (guththila_environment_t * environment,
                                guththila_stack_t * st,
                                guththila_namespace_t * ns);
GUTHTHILA_DECLARE (guththila_element_t *)
guththila_stack_pull_current (guththila_environment_t * environment,
                              guththila_stack_t * st);
GUTHTHILA_DECLARE (int)
guththila_stack_push_depth (guththila_environment_t * environment,
                            guththila_stack_t * st, guththila_depth_t * d);
GUTHTHILA_DECLARE (void)
guththila_stack_clear (guththila_environment_t * environment,
                       guththila_stack_t * st);
GUTHTHILA_DECLARE (guththila_element_t *)
guththila_stack_get (guththila_environment_t * environment,
                     guththila_stack_t * st, int i);

#endif /* GUTHTHILA_STACK_H */
