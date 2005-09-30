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


#ifndef __STACK_INCLUDED__
#define __STACK_INCLUDED__
#include "token.h"
#include "attribute.h"
#include "namespace.h"
#include "depth.h"

enum element_types
  {
    token_element,
    attribute_element,
    namespace_element
  };

typedef struct element ELEMENT;

struct element
{
  TOKEN *token;
  ATTRIBUTE *attribute;
  NAMESPACE *namespace;
  DEPTH *depth;
  ELEMENT *prev;
};

typedef struct stack
{
  enum element_types ELEMENT_TYPE; 
  int pointer;
  int current_pos;
  ELEMENT *tail;
  ELEMENT *current;
}STACK;

/* stack implementation */
STACK *Stack_createStack ();
int Stack_push (STACK *st, TOKEN *tok, ATTRIBUTE *attr);
int Stack_size (STACK *st);
void Stack_free (STACK *st);
void Stack_free_rec (STACK *st, ELEMENT *el);
ELEMENT *Stack_last (STACK *st);
ELEMENT *Stack_pull (STACK *st);
int Stack_push_namespace (STACK *st, NAMESPACE *ns);
ELEMENT *Stack_pull_current (STACK *st);
int Stack_push_depth (STACK *st, DEPTH *d);
void Stack_clear (STACK *st);
ELEMENT *Stack_get (STACK *st, int i);


#endif /* end of__STACK_INCLUDED__ */
