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


#include "guththila_stack.h"

STACK *
Stack_createStack ()
{
  STACK *st = (STACK *) malloc (sizeof (STACK));
  if (st)
    {
      st->pointer = 0;
      st->current_pos = 0;
      st->tail = NULL;
      st->current = NULL;
      return st;
    }
  else
    return NULL;
}


int 
Stack_push (STACK *stack, guththila_token_t *tok, guththila_attribute_t *attr)
{
  if (stack)
    {
      ELEMENT *e = (ELEMENT *) malloc (sizeof (ELEMENT));
      e->token = tok;
      e->attribute = attr;
      if (stack->pointer == 0)
	{
	  e->prev = NULL;
	  stack->tail = e;
	  stack->current = stack->tail;
	  stack->pointer ++;
	  stack->current_pos = stack->pointer;
	}
      else 
	{
	  e->prev = stack->tail;
	  stack->tail = e;
	  stack->current = stack->tail;
	  stack->pointer ++;
	  stack->current_pos = stack->pointer;
	}
    }
  else
    guththila_token_exception ();
  return 0;
}


int
Stack_size (STACK *stack)
{
  if (stack->pointer)
    return stack->pointer;
  else
    return 0;
}


void 
Stack_free (STACK *stack)
{
  if (stack && (stack->pointer > 0))
    {
      ELEMENT *ele = stack->tail;
      Stack_free_rec (stack, ele);
      free (ele);
      free (stack);
    }
}


void
Stack_free_rec (STACK *stack, ELEMENT *elem)
{
  if (elem->prev == NULL)
    {
      free (elem);
    }
  else 
    {
      elem = elem->prev;
      Stack_free_rec (stack, elem);
      free (elem);
    }
}


ELEMENT *
Stack_last (STACK *stack)
{
  if (stack)
    return stack->tail;
  else
    return NULL;
}


ELEMENT *
Stack_pull (STACK *stack)
{
  ELEMENT *e;
  if (stack)
    {
      e = stack->tail;
      if (stack->pointer != 0)
	{
	  if (e->prev)
	    {
	      stack->tail = e->prev;
	      stack->current = stack->tail;
	      e->prev = NULL;
	      stack->pointer --;
	      stack->current_pos = stack->pointer;
	    }
	  else 
	    {
	      stack->tail = e;
	      stack->current = stack->tail;
	      stack->pointer = 0;
	      stack->current_pos = stack->pointer;
	    }
	}
      return e;
    }
  else
    return NULL;
}


int 
Stack_push_namespace (STACK *stack, NAMESPACE *ns)
{
  if (stack)
    {
      ELEMENT *e = (ELEMENT *) malloc (sizeof (ELEMENT));
      e->namespace = ns;
      e->attribute = NULL;
      e->token = NULL;
      if (stack->pointer == 0)
	{
	  e->prev = NULL;
	  stack->tail = e;
	  stack->current = stack->tail;
	  stack->pointer ++;
	  stack->current_pos = stack->pointer;
	}
      else 
	{
	  e->prev = stack->tail;
	  stack->tail = e;
	  stack->current = stack->tail;
	  stack->pointer ++;
	  stack->current_pos = stack->pointer;
	}
    }
  else
    guththila_token_exception ();
  return 0;
}


ELEMENT *
Stack_pull_current (STACK *stack)
{
  ELEMENT *e;
  e = stack->current;
  if (stack->current_pos != 0)
    {
      if (e->prev)
	{
	  stack->current = e->prev;
	  stack->current_pos --;
	}
      else 
	{
	  stack->current = e;
	  stack->current_pos = 0;
	}
    }
  return e;
}


int 
Stack_push_depth (STACK *stack, guththila_depth_t *d)
{
  if (stack)
    {
      ELEMENT *e = (ELEMENT *) malloc (sizeof (ELEMENT));
      e->namespace = NULL;
      e->attribute = NULL;
      e->token = NULL;
      e->depth = d;
      if (stack->pointer == 0)
	{
	  e->prev = NULL;
	  stack->tail = e;
	  stack->current = stack->tail;
	  stack->pointer ++;
	  stack->current_pos = stack->pointer;
	}
      else 
	{
	  e->prev = stack->tail;
	  stack->tail = e;
	  stack->current = stack->tail;
	  stack->pointer ++;
	  stack->current_pos = stack->pointer;
	}
    }
  else
    guththila_token_exception ();
  return 0;
}


void
Stack_clear (STACK *stack)
{
  ELEMENT *e; 
  e = stack->tail;
  if (e)
    {
      Stack_pull (stack);
      e = stack->tail;
    }
  stack->pointer = 0 ;
}


ELEMENT *
Stack_get (STACK *stack , int i)
{
  if (i)
    {
      if ( i == 1)
        return stack->tail;
      else
        {
          ELEMENT *e;
          int ix = stack->pointer;
          e = stack->tail;
          for ( ; ix > ((stack->pointer+1) - i); ix--)
            e = e->prev;
          return e;
        }
    }
  else
    return NULL;
}
