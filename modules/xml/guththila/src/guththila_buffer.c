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

#include "guththila_environment.h"
#include "guththila_buffer.h"

GUTHTHILA_DECLARE(guththila_buffer_t *) 
guththila_buffer_create (guththila_environment_t *environment,int size)
{
  guththila_buffer_t *name = guththila_malloc (environment->allocator , sizeof(guththila_buffer_t));
  name->size = size;
  name->offset = 0;
  name->last = 0;
  name->next = 0;
  name->buff = NULL;
  if (size != 0)
    name->buff = (guththila_char_t *) guththila_malloc (environment->allocator,size);
  return name;
}


GUTHTHILA_DECLARE(void) 
guththila_buffer_free (guththila_environment_t *environment, guththila_buffer_t *name)
{
  if (name)
    {
      if (name->buff)
	  {
	        guththila_free (environment->allocator,name->buff);
	  }
      free (name);
    }
}


GUTHTHILA_DECLARE(guththila_buffer_t *)
guththila_buffer_grow (guththila_environment_t *environment,guththila_buffer_t *name)
{
  guththila_buffer_t *x;
  name->size <<= 1;
  x = (guththila_buffer_t *)guththila_realloc (environment->allocator,name, name->size);
  if (x)
    name = x; 
  else
    return NULL;
  return name;
}
