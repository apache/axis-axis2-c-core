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

#include "guththila_xml_pull_parser.h"

int main (int argc, char *argv[])
{
  guththila_reader_t *red;
  FILE *fp = fopen ("response.xml", "r");
  red = guththila_reader_create (fp);
  XML_PullParser *parser = XML_PullParser_createPullParser (red);
  XML_PullParser_read (parser);
  int c;
  while ((c = XML_PullParser_next (parser)) != -1)
    {
      switch (c)
	{
	case START_DOCUMENT :
	  {
	    printf ("<?xml ");
	    int ix;
	    ix = XML_PullParser_getAttributeCount (parser);
	    for (; ix > 0; ix--)
	      {
		guththila_attribute_t *a;
		char *p;
		a = XML_PullParser_getAttribute (parser);
		p = XML_PullParser_getAttributeName (parser, a);
		printf ("%s=\"", p);
		free (p);
		p = XML_PullParser_getAttributeValue (parser, a);
		printf ("%s\" ", p);
		free (p);
	      }
	    printf ("?>");
	  }
	  break;
	case START_ELEMENT:
	case EMPTY_ELEMENT:
	  {
	    printf ("<");
	    int ia;
	    int d;
	    char *p;
	     p = XML_PullParser_getPrefix (parser);
	    if (p)
	      {
		printf ("%s:", p);
		free (p);
	      }
	    p = XML_PullParser_getName (parser);
	    printf ("%s", p);
	    free (p);
	    ELEMENT *e;
	    ia = XML_PullParser_getAttributeCount (parser);
	    for ( ; ia > 0; ia--)
	      {
		/* p = XML_PullParser_getAttributePrefix_by_number
		   (parser, ia); */
		p = XML_PullParser_getAttributeNamespace_by_number (parser, ia);
		if (p)
		  {
		    printf (" %s:", p);
		    free (p);
		    p = XML_PullParser_getAttributeName_by_number (parser, ia);
		    printf ("%s=\"", p);
		    free (p);
		    p = XML_PullParser_getAttributeValue_by_number (parser, ia);
		    printf ("%s\"", p);
		    free (p);
		  }
		else
		  {
		    p = XML_PullParser_getAttributeName_by_number (parser, ia);
		    printf (" %s=\"", p);
		    free (p);
		    p = XML_PullParser_getAttributeValue_by_number (parser, ia);
		    printf ("%s\"", p);
		    free (p);
		  }
	      }
	    e = Stack_last (parser->dep);
	    d = e->depth->count;
	    guththila_namespace_t *ns ;
	    for (; d > 0; d--)
	      {
		p = XML_PullParser_getNamespacePrefix_by_number (parser, d);
		if (strncmp (p, "xmlns", 5))
		  printf (" xmlns:");
		printf ("%s=\"", p);
		free (p);
		p = XML_PullParser_getNamespaceUri_by_number (parser, d);
		printf ("%s\" ", p);
		free (p);
		}
	    if (EVENT == START_ELEMENT)
	      printf (">");
	    else
	      printf ("/>");
	  }
	  break;
	  case END_ELEMENT:
	  {
	    printf ("</");
	    char *p;
	    p = XML_PullParser_getPrefix (parser);
	    if (p)
	      {
		printf ("%s:", p);
		free (p);
	      }
	    p = XML_PullParser_getName (parser);
	    printf ("%s", p);
	    free (p);
	    printf (">");
	  }
	  break;
	case CHARACTER:
	  {
	  char *p;
	  p = XML_PullParser_getValue (parser);
	  printf (p);
	  free (p);
	  }
	  break;
	case COMMENT:
	  break;
	};
    }
  XML_PullParser_freePullParser (parser);
  return 0;
}

