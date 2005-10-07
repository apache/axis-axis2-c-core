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
  guththila_xml_pull_parser_t *parser = guththila_xml_pull_parser_create (red);
  guththila_xml_pull_parser_read (parser);
  int c;
  while ((c = guththila_xml_pull_parser_next (parser)) != -1)
    {
      switch (c)
	{
	case START_DOCUMENT :
	  {
	    printf ("<?xml ");
	    int ix;
	    ix = guththila_xml_pull_parser_get_attribute_count (parser);
	    for (; ix > 0; ix--)
	      {
		guththila_attribute_t *a;
		char *p;
		a = guththila_xml_pull_parser_get_attribute (parser);
		p = guththila_xml_pull_parser_get_attribute_name (parser, a);
		printf ("%s=\"", p);
		free (p);
		p = guththila_xml_pull_parser_get_attribute_value (parser, a);
		printf ("%s\" ", p);
		free (p);
	      }
	    printf ("?>");
	  }
	  break;
	case START_guththila_element_t:
	case EMPTY_guththila_element_t:
	  {
	    printf ("<");
	    int ia;
	    int d;
	    char *p;
	     p = guththila_xml_pull_parser_get_prefix (parser);
	    if (p)
	      {
		printf ("%s:", p);
		free (p);
	      }
	    p = guththila_xml_pull_parser_get_name (parser);
	    printf ("%s", p);
	    free (p);
	    guththila_element_t *e;
	    ia = guththila_xml_pull_parser_get_attribute_count (parser);
	    for ( ; ia > 0; ia--)
	      {
		/* p = guththila_xml_pull_parser_get_attribute_prefix_by_number
		   (parser, ia); */
		p = guththila_xml_pull_parser_get_attribute_namespace_by_number (parser, ia);
		if (p)
		  {
		    printf (" %s:", p);
		    free (p);
		    p = guththila_xml_pull_parser_get_attribute_name_by_number (parser, ia);
		    printf ("%s=\"", p);
		    free (p);
		    p = guththila_xml_pull_parser_get_attribute_value_by_number (parser, ia);
		    printf ("%s\"", p);
		    free (p);
		  }
		else
		  {
		    p = guththila_xml_pull_parser_get_attribute_name_by_number (parser, ia);
		    printf (" %s=\"", p);
		    free (p);
		    p = guththila_xml_pull_parser_get_attribute_value_by_number (parser, ia);
		    printf ("%s\"", p);
		    free (p);
		  }
	      }
	    e = guththila_stack_last (parser->dep);
	    d = e->depth->count;
	    guththila_namespace_t *ns ;
	    for (; d > 0; d--)
	      {
		p = guththila_xml_pull_parser_get_namespace_prefix_by_number (parser, d);
		if (strncmp (p, "xmlns", 5))
		  printf (" xmlns:");
		printf ("%s=\"", p);
		free (p);
		p = guththila_xml_pull_parser_get_namespace_uri_by_number (parser, d);
		printf ("%s\" ", p);
		free (p);
		}
	    if (EVENT == START_guththila_element_t)
	      printf (">");
	    else
	      printf ("/>");
	  }
	  break;
	  case END_guththila_element_t:
	  {
	    printf ("</");
	    char *p;
	    p = guththila_xml_pull_parser_get_prefix (parser);
	    if (p)
	      {
		printf ("%s:", p);
		free (p);
	      }
	    p = guththila_xml_pull_parser_get_name (parser);
	    printf ("%s", p);
	    free (p);
	    printf (">");
	  }
	  break;
	case CHARACTER:
	  {
	  char *p;
	  p = guththila_xml_pull_parser_get_value (parser);
	  printf (p);
	  free (p);
	  }
	  break;
	case COMMENT:
	  break;
	};
    }
  guththila_xml_pull_parser_free (parser);
  return 0;
}

