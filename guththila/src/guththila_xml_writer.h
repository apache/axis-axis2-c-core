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

#ifndef XML_STREAMWRITER_H
#define XML_STREAMWRITER_H
#include "guththila_writer.h"
#include "guththila_buffer.h"
#include <axis2_env.h>
#include <axis2_stack.h>

typedef struct guththila_xml_writer_s
{
  axis2_stack_t *element;
  axis2_stack_t *attribute;
  axis2_stack_t *namespace;
  axis2_stack_t *depth;
  guththila_writer_t *writer;
  guththila_buffer_t *writer_buffer;
  int next;
  int last;
  int offset;
  int start_element_open;
  int empty_element_open;
}guththila_xml_writer_t;



#endif /* XML_STREAMWRITER_H */
