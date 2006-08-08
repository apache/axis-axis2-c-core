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


AXIS2_EXTERN guththila_writer_t *
guththila_writer_create_for_file (axis2_env_t *env, char *fp)
{

  if (!fp)
    return NULL;

  guththila_writer_impl_t *wt = NULL;

  wt = (guththila_writer_impl_t *) AXIS2_MALLOC (env->allocator, sizeof (guththila_writer_impl_t));
 
  wt->outputstream = fopen (fp, "w");
 
  if (!wt->outputstream)
    return NULL;

 wt->writer.guththila_writer_type = GUTHTHILA_WRITER_CREATE_FOR_FILE;
 return &(wt->writer);
}

AXIS2_EXTERN void
guththila_writer_free (axis2_env_t *env, guththila_writer_t *wt)
{
  if (wt)
    {
      if (wt->guththila_writer_type == GUTHTHILA_WRITER_CREATE_FOR_FILE)
	{
	  if (((guththila_writer_impl_t *)wt)->outputstream)
	    fclose (((guththila_writer_impl_t *)wt)->outputstream);
	  ((guththila_writer_impl_t *)wt)->outputstream = NULL;
	}
      AXIS2_FREE (env->allocator, (guththila_writer_t *)wt);
      wt = NULL;
    }
}

AXIS2_EXTERN int 
guththila_writer_write (axis2_env_t *env, char *buffer, int offset, int length, guththila_writer_t *wt)
{
 
  if (wt->guththila_writer_type == GUTHTHILA_WRITER_CREATE_FOR_FILE)
    {
      int c;
      c = fwrite (buffer+offset, 1, length, ((guththila_writer_impl_t *)wt)->outputstream);
      return c;
    }
  else 
    return 0;
}
