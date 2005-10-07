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


#ifndef __BUFFER__INCLUDED__
#define __BUFFER__INCLUDED__
#include <stdlib.h>

struct Buffer
{
  int size;
  int next;
  int last;
  int offset;
  char *buff;
};

typedef struct Buffer BUFFER;
BUFFER *Buffer_createBuffer (int size);
void Buffer_freeBuffer (BUFFER *name);
BUFFER *Buffer_grow (BUFFER *name);

#endif
