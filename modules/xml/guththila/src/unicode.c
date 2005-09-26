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


#include "unicode.h"
#include <stdlib.h>
#include <string.h>
#ifdef UNICODE_OUT  /* This variable for Obtain UTF-16 Output */

/*! In order to determine length of UTF-16 Buffer 
 * Basicall char *p should be a Utf-16 Buffer  */

UTF8_char 
strlenUnicode (char *p)
{
  UTF8_char len = 0;
  UTF8_char d;
  UTF16_char c ;
  char* s = p;
  c = *((UTF16_char *)&s[len]);
  if (c)
    {
      while (c)
	{
	  c = *((UTF16_char *)&s[len]);
	  len +=2;
	  d = s[len];
	  if(!d)
	    {
	      return len;
	    break;
	    }
	}
      return (len-2);
    }
  else 
    return 0;
}

/*!To Duplicate UTF-16 String  */
char * 
strdupUnicode (char* p, int length)
{
  char *s;
  s = (char *)calloc (length+1, 1);
  s[length] = 0;
  return (char *)memcpy (s, p, length);
}
#endif /*end of UNICODE_OUT definition  */
