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


#include "guththila_reader.h"


guththila_reader_t *
guththila_reader_create (FILE *fp)
{
  guththila_reader_t *reader = (guththila_reader_t *) malloc (sizeof(guththila_reader_t));
  if (fp)
    reader->fp = fp;
  return reader;
}


void
guththila_reader_free (guththila_reader_t *r)
{
  if (r)
    free (r);
}


int
guththila_reader_read (char *buffer, int offset, int length, guththila_reader_t *r)
{
  return (int)fread (buffer+offset, 1, length, r->fp);
}


int 
guththila_reader_set_input_stream (guththila_reader_t *r, FILE *fp)
{
  if (fp)
    {
      r->fp = fp;
      return 1;
    }
  else
    return 0;
}
