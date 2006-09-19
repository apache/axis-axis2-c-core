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
 */

#include "guththila_writer.h"


AXIS2_EXTERN guththila_writer_t * AXIS2_CALL
guththila_writer_create_for_file (axis2_env_t *env, char *fp)
{
      guththila_writer_impl_t *wt = NULL;
      if(!fp)
        return NULL;
      wt = (guththila_writer_impl_t *) AXIS2_MALLOC (env->allocator, sizeof (guththila_writer_impl_t));
      wt->outputstream = fopen (fp, "w");
      if (!wt->outputstream)
        return NULL;
     wt->writer.guththila_writer_type = GUTHTHILA_WRITER_FILE;
    return &(wt->writer);
}

AXIS2_EXTERN guththila_writer_t * AXIS2_CALL
guththila_writer_create_for_memory (axis2_env_t *env)
{
      guththila_writer_impl_t *wt = NULL;

      wt = (guththila_writer_impl_t *) AXIS2_MALLOC (env->allocator, sizeof (guththila_writer_impl_t));
      wt->outputstream = NULL;

      wt->buffer = guththila_buffer_create (env, 1024);
      memset ((void *)wt->buffer->buff, 0, 1024);
      if (!wt->buffer)
	return NULL;
      
      wt->writer.guththila_writer_type = GUTHTHILA_WRITER_MEMORY;
      return &(wt->writer);
}

AXIS2_EXTERN void AXIS2_CALL
guththila_writer_free (axis2_env_t *env, guththila_writer_t *wt)
{
  if (wt)
    {
      if (wt->guththila_writer_type == GUTHTHILA_WRITER_FILE)
	{
	  if (((guththila_writer_impl_t *)wt)->outputstream)
	    fclose (((guththila_writer_impl_t *)wt)->outputstream);
	  ((guththila_writer_impl_t *)wt)->outputstream = NULL;
	}
      AXIS2_FREE (env->allocator, (guththila_writer_t *)wt);
      wt = NULL;
    }
}

AXIS2_EXTERN int AXIS2_CALL
guththila_writer_write (axis2_env_t *env, 
    char *buffer, int offset, 
    int length, guththila_writer_t *wt)
{
  int c = 0;
  guththila_writer_impl_t *writer_impl = NULL;
    if (wt->guththila_writer_type == GUTHTHILA_WRITER_FILE)
    {
        c = fwrite (buffer+offset, 1, length, ((guththila_writer_impl_t *)wt)->outputstream);
    }
    else if (wt->guththila_writer_type == GUTHTHILA_WRITER_MEMORY)
      {
	int size = 0;
	writer_impl = (guththila_writer_impl_t *)wt;

	if (writer_impl->buffer->buff)
	  size = strlen ((const char *)writer_impl->buffer->buff);

	if ((size + length) > writer_impl->buffer->size)
	  {
	    writer_impl->buffer = guththila_buffer_grow (env, writer_impl->buffer);
	  }
	strcat (writer_impl->buffer->buff, buffer);
	c = length;
      }
    return c;
}


AXIS2_EXTERN char * AXIS2_CALL
guththila_writer_get_buffer (axis2_env_t *env, 
			     guththila_writer_t *wt)
{
  guththila_writer_impl_t *writer_impl = NULL;
  writer_impl = (guththila_writer_impl_t *)wt;
  
  if (writer_impl->buffer->buff)
    return (char *)writer_impl->buffer->buff;
  else 
    return (char *)NULL;
}
