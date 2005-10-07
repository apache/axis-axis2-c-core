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


#include "guththila_buffer.h"

BUFFER * 
Buffer_createBuffer (int size)
{
  BUFFER *name = malloc (sizeof(BUFFER));
  name->size = size;
  name->offset = 0;
  name->last = 0;
  name->next = 0;
  if (name->buff)
    free (name->buff);
  if (size != 0)
    name->buff = (char *) malloc (size);
  return name;
}


void 
Buffer_freeBuffer (BUFFER *name)
{
  if (name)
    {
      if (name->buff)
	free (name->buff);
      free (name);
    }
}


BUFFER *
Buffer_grow (BUFFER *name)
{
  name->size <<= 1;
  BUFFER *x = (BUFFER *) realloc (name, name->size);
  if (x)
    name = x; 
  else
    return NULL;
  return name;
}
