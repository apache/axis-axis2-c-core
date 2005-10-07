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


#include "reader.h"


READER *
Reader_createReader (FILE *fp)
{
  READER *reader = (READER *) malloc (sizeof(READER));
  if (fp)
    reader->fp = fp;
  return reader;
}


void
Reader_freeReader (READER *r)
{
  if (r)
    free (r);
}


int
Reader_read (char *buffer, int offset, int length, READER *r)
{
  return (int)fread (buffer+offset, 1, length, r->fp);
}


int 
Reader_setInputStream (READER *r, FILE *fp)
{
  if (fp)
    {
      r->fp = fp;
      return 1;
    }
  else
    return 0;
}
