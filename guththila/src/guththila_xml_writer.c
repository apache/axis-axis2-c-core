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

#include "guththila.h"


AXIS2_EXTERN void AXIS2_CALL
guththila_create_xml_stream_writer (axis2_env_t *env, guththila_t *p, char *file)
{
  if (p || file)
    {
      p->xsw = (guththila_xml_writer_t *) AXIS2_MALLOC (env->allocator, sizeof (guththila_xml_writer_t));
      p->xsw->writer = guththila_writer_create_for_file (env, file);
      p->xsw->writer_buffer = guththila_buffer_create (env, 1024);
      p->xsw->writer_buffer->buff[0] = 0;
      p->xsw->element = axis2_stack_create (env);
      p->xsw->attribute = axis2_stack_create (env);
      p->xsw->namespace = axis2_stack_create (env);
      p->xsw->depth = axis2_stack_create (env);
      p->xsw->next = 0;
      p->xsw->offset = 0;
      p->xsw->last = 1024;	/* size of the buffer */
      p->xsw->start_element_open = -1;
      p->xsw->empty_element_open = -1;
    }
  else
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
}


AXIS2_EXTERN void AXIS2_CALL
guththila_create_xml_stream_writer_for_memory (axis2_env_t *env, guththila_t *p)
{
  if (p)
    {
      p->xsw = (guththila_xml_writer_t *) AXIS2_MALLOC (env->allocator, sizeof (guththila_xml_writer_t));
      p->xsw->writer = guththila_writer_create_for_memory (env);
      p->xsw->writer_buffer = guththila_buffer_create (env, 1024);
      p->xsw->writer_buffer->buff[0] = 0;
      p->xsw->element = axis2_stack_create (env);
      p->xsw->attribute = axis2_stack_create (env);
      p->xsw->namespace = axis2_stack_create (env);
      p->xsw->depth = axis2_stack_create (env);
      p->xsw->next = 0;
      p->xsw->offset = 0;
      p->xsw->last = 1024;	/* size of the buffer */
      p->xsw->start_element_open = -1;
      p->xsw->empty_element_open = -1;
    }
  else
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
}

AXIS2_EXTERN void AXIS2_CALL
guththila_xml_writer_free(axis2_env_t *env, guththila_t *p)
{

  int size;
  size = 0;

  guththila_check_xml_stream_writer (env, p);
  /*   guththila_flush (env, p); */
  guththila_write_end_document (env, p);
  if (p->xsw->writer_buffer)
    {
      guththila_buffer_free (env, p->xsw->writer_buffer);
      p->xsw->writer_buffer = NULL;
    }

  if (p->xsw->element)
    {
      void *element;
      size = 0;
      
      size = AXIS2_STACK_SIZE (p->xsw->element, env);
      if (size)
	{
	  for (; size > 0; size--)
	    {
	      element = AXIS2_STACK_POP (p->xsw->element, env);
	      AXIS2_FREE (env->allocator, element);
	      element = NULL;
	    }
	}
      AXIS2_STACK_FREE (p->xsw->element, env);
      p->xsw->element = NULL;
    }


  if (p->xsw->attribute)
    {
      size = 0;
      size = AXIS2_STACK_SIZE (p->xsw->attribute, env);
      if (size)
	{
	  guththila_attribute_t* att;
	  for (;size > 0; size--)
	    {
	      att = (guththila_attribute_t *)AXIS2_STACK_POP (p->xsw->attribute, env);
	      AXIS2_FREE (env->allocator, att);
	      att = NULL;
	    }
	}
      AXIS2_STACK_FREE (p->xsw->attribute, env);
      p->xsw->attribute = NULL;
    }


  if (p->xsw->namespace)
    {
      size = 0;
      size = AXIS2_STACK_SIZE (p->xsw->namespace, env);
      if (size)
	{
	  guththila_namespace_t* ns;
	  for (;size > 0; size--)
	    {
	      ns = (guththila_namespace_t *)AXIS2_STACK_POP (p->xsw->namespace, env);
	      AXIS2_FREE (env->allocator, ns);
	      ns = NULL;
	    }
	  AXIS2_STACK_FREE (p->xsw->namespace, env);
	  p->xsw->namespace = NULL;
	}
    }


  if (p->xsw->depth)
    {
      size = 0;
      size = AXIS2_STACK_SIZE (p->xsw->depth, env);
      if (size)
	{
	  guththila_depth_t* depth;
	  for (;size > 0; size--)
	    {
	      depth = (guththila_depth_t *)AXIS2_STACK_POP (p->xsw->depth, env);
	      AXIS2_FREE (env->allocator, depth);
	      depth = NULL;
	    }
	  AXIS2_STACK_FREE (p->xsw->depth, env);
	  p->xsw->depth = NULL;
	}
    }

  if (p->xsw->writer)
    {
      guththila_writer_free (env, p->xsw->writer);
      p->xsw->writer = NULL;
    }

 
  if (p->xsw)
    {
      AXIS2_FREE (env->allocator, p->xsw);
      p->xsw = NULL;
    }
}


int AXIS2_CALL
guththila_check_xml_stream_writer (axis2_env_t *env, guththila_t *p)
{
  if (!p->xsw->writer)
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_WRITER);
  else 
    return 1;
  return 0;
}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_start_document(axis2_env_t *env, 
			       guththila_t *p)
{
  char *sd =  NULL;    
  guththila_check_xml_stream_writer (env, p); 
  sd  = "<?xml version = \"1.0\" encoding = \"utf-8\" ?>";
  guththila_write_to_buffer (env, p, sd);
}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_end_element (axis2_env_t *env, guththila_t *p)
{
  void *element = NULL;
  guththila_check_xml_stream_writer (env, p);
  element = AXIS2_STACK_POP (p->xsw->element, env);
  if(p->xsw->empty_element_open)
    {
      guththila_close_start_element (env, p);
    }
  else
    {
      guththila_close_start_element (env, p);
      if (element)
	{
	  guththila_write_to_buffer (env, p, "</");
	  guththila_write_to_buffer (env, p, element);
	  guththila_write_to_buffer (env, p, ">");
	  guththila_close_depth_element (env, p);
	}
      else
	guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
    }
}


AXIS2_EXTERN void AXIS2_CALL
guththila_flush (axis2_env_t *env, guththila_t *p)
{
  int c;
  int ii = 0;
  guththila_check_xml_stream_writer (env, p);
  if(p->xsw->writer_buffer->buff)
    {
      ii = strlen (p->xsw->writer_buffer->buff);
      c = guththila_writer_write (env, p->xsw->writer_buffer->buff, 0, ii, p->xsw->writer);
      p->xsw->writer_buffer->buff[0] = 0;
      p->xsw->writer_buffer->buff[1] = 0;
      p->xsw->next = 0;
      p->xsw->offset = 0;
    }
}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_start_element (axis2_env_t *env, guththila_t *p, char *start_element)
{
  int size;
  void *element;
  if (!p || !start_element)
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
  else
    {
      guththila_check_xml_stream_writer (env, p);
      guththila_close_start_element (env, p);
      guththila_open_depth_element (env, p);
      size = AXIS2_STACK_SIZE (p->xsw->element, env);
 
      if (size)
	{
	  element = AXIS2_STACK_GET_AT (p->xsw->element, env, 0);
	  if (!strcmp ((char *)element, start_element))
	    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
	}

      guththila_check_name_validity (env, p,start_element);
      p->xsw->start_element_open = 1;

      if (!p->xsw->empty_element_open)
	AXIS2_STACK_PUSH (p->xsw->element, env, start_element);
      guththila_write_to_buffer (env, p,"<");
      guththila_write_to_buffer  (env, p,start_element);
    }
}


void  AXIS2_CALL
guththila_write_to_buffer (axis2_env_t *env, guththila_t *p, const char *buff)
{
  if (buff)
    {
      guththila_check_xml_stream_writer (env, p);
      int length = 0;
      length = strlen (buff);

      if ((p->xsw->next + length) >= p->xsw->last)
	{
	  guththila_flush (env, p);
	  p->xsw->next = 0;
	  p->xsw->offset = 0;
	}
	 
      if (p->xsw->writer_buffer->buff && p->xsw->next > -1)
	{
	  strcat ((p->xsw->writer_buffer->buff) + (p->xsw->next), buff);
	  p->xsw->offset = p->xsw->next;
	  p->xsw->next += length;
	}
    }
}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_characters (axis2_env_t *env, guththila_t *p, const char *buff)
{
  guththila_check_xml_stream_writer (env, p);
  guththila_close_start_element (env, p);
  
  if (buff)
    {
      guththila_write_to_buffer (env, p,buff);
    }
  else
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_INVALID_BUFFER);
}


void AXIS2_CALL
guththila_close_start_element (axis2_env_t *env, guththila_t *p)
{
  int stack_size = 0;
  if (p->xsw->start_element_open != -1 && p->xsw->empty_element_open != -1)
    {
      if (p->xsw->start_element_open && p->xsw->empty_element_open)
	{
	  p->xsw->start_element_open = 0;
	  p->xsw->empty_element_open = 0;
	  guththila_write_to_buffer (env, p,"/>");
	}
      
      if (p->xsw->start_element_open && !p->xsw->empty_element_open)
	{
	  p->xsw->start_element_open = 0;
	  guththila_write_to_buffer (env, p,">");
	}
      
      stack_size = AXIS2_STACK_SIZE (p->xsw->attribute, env);
      if (stack_size)
	{
	  guththila_attribute_t* att = NULL;
	  for (;stack_size > 0; stack_size--)
	    {
	      att = (guththila_attribute_t *)AXIS2_STACK_POP (p->xsw->attribute, env);
	      AXIS2_FREE (env->allocator, att);
	      att = NULL;
	    }
	}

    }
  else
    {
      p->xsw->start_element_open = 0;
      p->xsw->empty_element_open = 0;
    }

  /*   guththila_open_depth_element (env, p); */
}


void  AXIS2_CALL
guththila_check_name_validity (axis2_env_t *env, guththila_t *p, char *name)
{
  int length;
  int ii;
  length = strlen (name);
  /* ideograhic characters are also valid for name_starting charactes
   * need to add them to here isIdeograpic function doesn't
   * functionning correctly yet*/
  if  (!(isalpha (name[0]) || name[0] == '_' || name[0] == ':'))
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_INVALID_CHAR_IN_NAME);

  /* xml in any case combination isn't allow */
  if ((name[0] == 'x' || name[0] == 'X')
      && (name[1] == 'm' || name[1] == 'M')
      && (name[2] == 'l' || name[2] == 'L'))
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_XML_STRING_IN_NAME);
  
  /* some punctuation's not allowed */
  for (ii = 1; ii < length; ii++)
    {
      if ((name[ii] == '$' || name[ii] == '^' || name[ii] == '%'
	   || name[ii] == ';' || name[ii] == '\'' || name[ii] == '"'
	   || name[ii] == '&' || name[ii] == '<' || name[ii] == '>'
	   || isspace(name[ii])))
	guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_INVALID_CHAR_IN_NAME);
    }
}


int AXIS2_CALL
guththila_is_ideographic (char *id)
{
  int ii;
  short *xy = (short *)id;
  for (ii = 0x4e00 ; ii < 0x9fa5; ii ++)
    {
      if (ii == xy[0])
	return 1;
    }

  if (0x3007 == xy[0])
    return 1;
  
  for (ii = 0x3021 ; ii <  0x3029; ii ++)
    {
      if (ii == xy[0])
	return 1;
    }
  return 0;
}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_comment (axis2_env_t *env, guththila_t *p, const char *buff)
{
  char *s = NULL;
  guththila_check_xml_stream_writer (env, p);
  guththila_close_start_element (env, p);
  s   = strchr (buff, '-');
  if(s && s[1] == '-')
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EXCESS_HYPENS_IN_COMMENT);
  guththila_write_to_buffer (env, p,"<!--");
  guththila_write_to_buffer (env, p,buff);
  guththila_write_to_buffer (env, p,"-->");
}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_escape_character (axis2_env_t *env, guththila_t *p, const char *buff)
{
  guththila_close_start_element (env, p);
  if(buff)
    {
      switch (buff[0])
	{
	case '>':
	  {
	    guththila_write_to_buffer (env, p,"&gt;");
	  }
	  break;
	case '<':
	  {
	    guththila_write_to_buffer (env, p,"&lt;");
	  }
	  break;
	case '\'':
	  {
	    guththila_write_to_buffer (env, p,"&apos;");
	  }
	  break;
	case '"':
	  {
	    guththila_write_to_buffer (env, p,"&quot;");
	  }
	  break;
	case '&':
	  {
	    guththila_write_to_buffer (env, p,"&amp;");
	  }
	  break;
	};
    }
  else
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_INVALID_BUFFER);
}


AXIS2_EXTERN void
AXIS2_CALL guththila_write_attribute (axis2_env_t *env, guththila_t *p, const char *local_name, const char *value)
{
  int size = 0;
  void *element;
  guththila_attribute_t *attr;

  if (!p->xsw->start_element_open)
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);

  size = AXIS2_STACK_SIZE (p->xsw->attribute, env);
  if (size)
    {
      int ii;
      for (ii = 0; ii <= size; ii++)
	{
	  element = AXIS2_STACK_GET_AT (p->xsw->attribute, env, size - ii);
	  if (element)
	    {
	      attr = (guththila_attribute_t *)element;
	      if (strcmp((char *)attr->name, local_name))
		{
		  guththila_do_write_attribute (env, p,local_name, value);
		  break;
		}
	      else
		guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_SAME_ATTRIBUTE_REPEAT);
	    }	}

	    
    }
  else
    {
      guththila_do_write_attribute (env, p,local_name, value);
    }
}


AXIS2_EXTERN void AXIS2_CALL
guththila_do_write_attribute (axis2_env_t *env, 
			      guththila_t *p, 
			      const char *local_name, 
			      const char *value)
{
  guththila_attribute_t *attr = (guththila_attribute_t *) AXIS2_MALLOC (env->allocator, sizeof (guththila_attribute_t));
  
  if (local_name)
    attr->name = (guththila_token_t *)local_name;
  if (value)
    attr->value = (guththila_token_t *)value;
  if (local_name && value)
    {
      AXIS2_STACK_PUSH (p->xsw->attribute, env, (  void *)attr);
    }

  guththila_check_name_validity (env, p,(char *)local_name);
  
  if (strrchr (value, '&') 
      || strrchr (value, '<') 
      || strrchr (value, '\"'))
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_INVALID_CHAR_IN_ATTRIBUTE);

  guththila_write_to_buffer (env, p," ");
  guththila_write_to_buffer (env, p,local_name);
  guththila_write_to_buffer (env, p," = \"");
  guththila_write_to_buffer (env, p,value);
  guththila_write_to_buffer (env, p,"\"");
}


void AXIS2_CALL
guththila_write_empty_element (axis2_env_t *env, guththila_t *p, char *empty_element)
{
  guththila_check_xml_stream_writer (env, p);
  guththila_close_start_element (env, p);
  p->xsw->start_element_open = 0;
  p->xsw->empty_element_open = 1;
  guththila_write_start_element (env, p,empty_element);
}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_default_namespace (axis2_env_t *env, guththila_t *p, char *namespace_uri)
{
  if (!p->xsw->start_element_open)
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
  else
    {
     
      if (guththila_check_default_namespace (env, p,namespace_uri))
	{
	  guththila_do_write_default_namespace (env, p,namespace_uri);
	}
    }
}


AXIS2_EXTERN int AXIS2_CALL
guththila_check_default_namespace (axis2_env_t *env, guththila_t *p, char *ns_uri)
{
  int size;
  int ii;
  void *element;
  guththila_namespace_t *ns;
  size = AXIS2_STACK_SIZE (p->xsw->namespace, env);
  if (size)
    {
      for (ii = 0; ii <= size; ii++)
	{
	  element = AXIS2_STACK_GET_AT (p->xsw->namespace, env, size - ii);
	  if (element)
	    {
	      ns = (guththila_namespace_t *)element;
	      if (ns)
		{
		  if (!ns->length || !strcmp (ns->uri, ns_uri))
		    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
		}
	    }
	}
      return 1;

    }
  else
    return 1;
}


void AXIS2_CALL
guththila_do_write_default_namespace (axis2_env_t *env, guththila_t *p, char *ns_uri)
{
  guththila_namespace_t *ns = (guththila_namespace_t *) AXIS2_MALLOC (env->allocator, sizeof (guththila_namespace_t));
  ns->name = NULL;
  ns->length = 0;
  ns->uri = ns_uri;
  ns->lengthuri = strlen (ns_uri);
  AXIS2_STACK_PUSH (p->xsw->namespace, env, (  void *)ns);

  guththila_write_to_buffer (env, p," ");
  guththila_write_to_buffer (env, p,"xmlns");
  guththila_write_to_buffer (env, p," = \'");
  guththila_write_to_buffer (env, p,ns_uri);
  guththila_write_to_buffer (env, p,"\'");
}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_namespace (axis2_env_t *env, guththila_t *p, char *prefix, char *uri)
{
  if (!p->xsw->start_element_open)
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);

  if (!prefix || !strcmp(prefix, ""))
    guththila_do_write_default_namespace (env, p,uri);
  else
    {
      if (guththila_check_prefix_validity (env, p,prefix, uri))
	guththila_do_write_namespace (env, p,prefix, uri);
    }
  

}


AXIS2_EXTERN int AXIS2_CALL
guththila_check_prefix_validity (axis2_env_t *env, guththila_t *p, char *prefix, char *uri)
{
  int size = 0;
  int ii = 0;
  void *element = NULL;
  guththila_namespace_t *ns = NULL;
  size = AXIS2_STACK_SIZE (p->xsw->namespace, env);
  if (size)
    {
      for (ii = 0; ii <= size; ii++)
	{
	  element = AXIS2_STACK_GET_AT (p->xsw->namespace, env, size - ii);
	  if (element)
	    {
	      ns = (guththila_namespace_t *)element;
	      if (ns->name && prefix)
		{
		  if (!strcmp (ns->name, prefix))
		    {
		      if (ns->uri && uri)
			{
			  if (strcmp (ns->uri, uri))
			    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
			  else
			    return 0;
			}

		    }
		}
	    }
	}
      return 1;
    }
  else
    return 1;
}


void AXIS2_CALL
guththila_do_write_namespace (axis2_env_t *env, guththila_t *p, char *prefix, char *uri)
{
  guththila_namespace_t *ns = (guththila_namespace_t *) AXIS2_MALLOC (env->allocator, sizeof (guththila_namespace_t));
  ns->name = prefix;
  ns->length = strlen (prefix);
  ns->uri = uri;
  ns->lengthuri = strlen (uri);
  AXIS2_STACK_PUSH (p->xsw->namespace, env, (  void *)ns);
  
  guththila_write_to_buffer (env, p," ");
  guththila_write_to_buffer (env, p,"xmlns:");
  guththila_write_to_buffer (env, p,prefix);
  guththila_write_to_buffer (env, p," = \'");
  guththila_write_to_buffer (env, p,uri);
  guththila_write_to_buffer (env, p,"\'");
}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_attribute_with_prefix_and_namespace (axis2_env_t *env, guththila_t *p,
						     const char *prefix, const char *namespace,
						     const char *local_name, const char *value)
{
  int size = 0;
  void *element;
  guththila_attribute_t *attr;
  
  if (!p->xsw->start_element_open)
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);

  if (prefix && namespace)
    guththila_write_namespace (env, p,(char *)prefix, (char *)namespace);

  size = AXIS2_STACK_SIZE (p->xsw->attribute, env);
  if (size)
    {
      int ii;
      for (ii = 0; ii <= size; ii++)
	{
	  element = AXIS2_STACK_GET_AT (p->xsw->attribute, env, size - ii);
	  if (element)
	    {
	      attr = (guththila_attribute_t *)element;
	      /* We want to make sure that out checking attribute has
		 the prefix otherwise we don't wan't to worry */
	      if (attr->name && attr->prefix) 
		{
		  if (!strcmp((char *)attr->name, local_name) && !strcmp((char *)attr->prefix, prefix))
		    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
		  else
		    {
		      guththila_do_write_attribute_with_prefix_and_namespace (env, p,prefix, namespace, local_name, value);
		      break;
		    }
		}
	      else
		{
		  /* since att->prefix is null im going to write it here */
		  guththila_do_write_attribute_with_prefix_and_namespace (env, p,prefix, namespace, local_name, value);
		  break;
		}
	    }
	}
      
    }
  else
    {
      guththila_do_write_attribute_with_prefix_and_namespace (env, p,prefix, namespace, local_name, value);
    }

}


void AXIS2_CALL
guththila_do_write_attribute_with_prefix_and_namespace (axis2_env_t *env, guththila_t *p, 
							const char *prefix, const char *namespace_uri,
							const char *local_name, const char *value)
{
  guththila_attribute_t *attr = (guththila_attribute_t *) AXIS2_MALLOC (env->allocator, sizeof (guththila_attribute_t));
  
  if (prefix)
    attr->prefix = (guththila_token_t *)prefix;
  if (namespace_uri)
    attr->namespace_uri = (guththila_token_t *)namespace_uri;
  if (local_name)
    attr->name = (guththila_token_t *)local_name;
  if (value)
    attr->value = (guththila_token_t *)value;

  if (local_name && value)
    {
      AXIS2_STACK_PUSH (p->xsw->attribute, env, (  void *)attr);
    }
  else
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);

  guththila_check_name_validity (env, p,(char *)local_name);
  
  if (strrchr (value, '&') 
      || strrchr (value, '<') 
      || strrchr (value, '\''))
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_INVALID_CHAR_IN_ATTRIBUTE);

  guththila_write_to_buffer (env, p," ");
  if (prefix)
    {
      guththila_write_to_buffer (env, p,prefix);
      guththila_write_to_buffer (env, p,":");
    }
  guththila_write_to_buffer (env, p,local_name);
  guththila_write_to_buffer (env, p," = \'");
  guththila_write_to_buffer (env, p,value);
  guththila_write_to_buffer (env, p,"\'");

}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_attribute_with_prefix (axis2_env_t *env, guththila_t *p, const char *prefix,
				       const char *local_name, const char *value)
{
  int size = 0;
  void *element;
  guththila_attribute_t *attr;

  if (!p->xsw->start_element_open)
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
  /* actually here we want exsisting prefix , that's why we check it
     exsists or not. */
  if (guththila_is_exsisting_prefix (env, p,prefix))
    {
      size = AXIS2_STACK_SIZE (p->xsw->attribute, env);
      if (size)
	{
	  int ii;
	  for (ii = 0; ii <= size; ii++)
	    {
	      element = AXIS2_STACK_GET_AT (p->xsw->attribute, env,  ii);
	      if (element)
		{
		  attr = (guththila_attribute_t *)element;
		  /* We want to make sure that out checking attribute has
		     the prefix otherwise we don't wan't to worry */
		  if (attr->name && attr->prefix) 
		    {
		      if (!strcmp((char *)attr->name, local_name) && !strcmp((char *)attr->prefix, prefix))
			guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
		      else
			{
			  guththila_do_write_attribute_with_prefix (env, p,prefix, local_name, value);
			  break;
			}
		    }
		  else
		    {
		      /* since att->prefix is null im going to write it here */
		      guththila_do_write_attribute_with_prefix (env, p,prefix, local_name, value);
		      break;
		    }
		}
	    }
      
	}
      else
	{
	  guththila_do_write_attribute_with_prefix (env, p,prefix, local_name, value);
	}
    }
  else
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_EXSISTING_PREFIX);

}


void AXIS2_CALL
guththila_do_write_attribute_with_prefix(axis2_env_t *env, guththila_t *p, const char *prefix,
					 const char *local_name, const char *value)
{
  guththila_do_write_attribute_with_prefix_and_namespace (env, p,prefix, NULL, local_name, value);
}


AXIS2_EXTERN int AXIS2_CALL
guththila_is_exsisting_prefix (axis2_env_t *env, guththila_t *p, const char *prefix)
{
  int size;
  int ii;
  void *element;
  guththila_namespace_t *ns;
  size = AXIS2_STACK_SIZE (p->xsw->namespace, env);
  if (size)
    {
      for (ii = 0; ii <= size; ii++)
	{
	  element = AXIS2_STACK_GET_AT (p->xsw->namespace, env, ii);
	  if (element)
	    {
	      ns = (guththila_namespace_t *)element;
	      if (ns->name)
		{
		  if (!strcmp (ns->name, prefix))
		    return 1;
		}
	    }
	}
      return 0;
    }
  else
    return 0;
}


int
guththila_is_exsisting_namespace_uri (axis2_env_t *env, guththila_t *p, const char *uri)
{
  int size;
  int ii;
  void *element;
  guththila_namespace_t *ns;
  size = AXIS2_STACK_SIZE (p->xsw->namespace, env);
  if (size)
    {
      for (ii = 0; ii <= size; ii++)
	{
	  element = AXIS2_STACK_GET_AT (p->xsw->namespace, env , size - ii);
	  if (element)
	    {
	      ns = (guththila_namespace_t *)element;
	      if (ns->uri)
		{
		  if (!strcmp (ns->uri, uri))
		    return 1;
		}
	    }
	}
      return 0;
    }
  else
    return 0;
}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_attribute_with_namespace (axis2_env_t *env, guththila_t *p, const char *namespace,
					  const char *local_name, const char *value)
{
  int size = 0;
  void *element;
  guththila_attribute_t *attr;
  if (!p->xsw->start_element_open)
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
  /* actually here we want exsisting namespace , that's why we check it
     exsists or not. */
  if (guththila_is_exsisting_namespace_uri (env, p,namespace))
    {
      char *prefix;
      prefix = guththila_get_prefix_for_namespace (env, p, namespace);
      size = AXIS2_STACK_SIZE (p->xsw->attribute, env);
      if (size)
	{
	  int ii;
	  for (ii = 0; ii <= size; ii++)
	    {
	      element = AXIS2_STACK_GET_AT (p->xsw->attribute, env, size - ii);
	      if (element)
		{
		  attr = (guththila_attribute_t *)element;
		  /* We want to make sure that out checking attribute has
		     the namespace otherwise we don't wan't to worry */
		  if (attr->name && attr->prefix) 
		    {
		      if (!strcmp((char *)attr->name, local_name) && !strcmp((char *)attr->prefix, prefix))
			guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
		      else
			{
			  guththila_do_write_attribute_with_prefix (env, p,prefix, local_name, value);
			  break;
			}
		    }
		  else
		    {
		      /* since att->prefix is null im going to write it here */
		      guththila_do_write_attribute_with_prefix (env, p,prefix, local_name, value);
		      break;
		    }
		}
	    }
      
	}
      else
	{
	  guththila_do_write_attribute_with_prefix (env, p,prefix, local_name, value);
	}
    }
  else
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_EXSISTING_URI);
}


AXIS2_EXTERN char* AXIS2_CALL
guththila_get_prefix_for_namespace (axis2_env_t *env, guththila_t *p, const char *namespace)
{
  int size;
  int ii;
  void *element;
  guththila_namespace_t *ns;
  size = AXIS2_STACK_SIZE (p->xsw->namespace, env);
  if (size)
    {
      for (ii = 0; ii <= size; ii++)
	{
	  element = AXIS2_STACK_GET_AT (p->xsw->namespace, env, size - ii);
	  if (element)
	    {
	      ns = (guththila_namespace_t *)element;
	      if (ns->uri)
		{
		  if (!strcmp (ns->uri, namespace))
		    return (char *)ns->name;
		}
	    }
	}
      return 0;
    }
  else
    return 0;
}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_start_element_with_prefix_and_namespace (axis2_env_t *env, guththila_t *p, const char *prefix,
							 const char *namespace_uri, const char *local_name)
{
  int size;
  void *element;
  char *start_element = NULL;
  if (prefix && local_name)
    start_element = (char *) calloc ((strlen (prefix) + strlen (local_name) +2), 1);

  if (!p || !local_name)
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
  else
    {
      guththila_check_xml_stream_writer (env, p);
      guththila_close_start_element (env, p);
      guththila_open_depth_element (env, p);
     
      if (prefix)
	{
	  strcat (start_element, prefix);
	  strcat (start_element, ":");
	  strcat (start_element, local_name);
	}
      else 
	start_element = (char *)local_name; 

      size = AXIS2_STACK_SIZE (p->xsw->element, env);
      if (size)
	{
	  element = AXIS2_STACK_GET_AT (p->xsw->element, env, size);
	  if (start_element && element)
	    {
	      if (!strcmp ((char *)element, start_element))
		guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
	    }
	}

      guththila_check_name_validity (env, p,start_element);
      p->xsw->start_element_open = 1;
      
      if (!p->xsw->empty_element_open)
	AXIS2_STACK_PUSH (p->xsw->element, env, start_element);
      guththila_write_to_buffer (env, p,"<");
      guththila_write_to_buffer  (env, p,start_element);
      guththila_write_namespace (env, p,(char *)prefix, (char *)namespace_uri);
    }
}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_start_element_with_namespace (axis2_env_t *env, guththila_t *p, const char *namespace_uri, const char *local_name)
{
  int size;
  void *element;
  char *start_element;
  if (guththila_is_exsisting_namespace_uri (env, p,namespace_uri))
    {
      if (!p || !local_name)
	guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
      else
	{
	  char *prefix = NULL;
	  guththila_check_xml_stream_writer (env, p);
	  guththila_close_start_element (env, p);
	  prefix = guththila_get_prefix_for_namespace (env, p,namespace_uri);

	  if(prefix)
	    start_element = (char *) calloc ((strlen (prefix) + strlen (local_name) +2), 1);
	  else
	    start_element = (char *) calloc ((strlen (local_name) +2), 1);
     
	  if(prefix)
	    {
	      strcat (start_element, prefix);
	      strcat (start_element, ":");
	      strcat (start_element, local_name);
	    } 
	  else
	    strcat (start_element, local_name);

	  size = AXIS2_STACK_SIZE (p->xsw->element, env);

	  if(size)
	    {
	      element = AXIS2_STACK_GET_AT (p->xsw->element, env, size);
	      if (!strcmp ((char *)element, start_element))
		guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
	    }

	  guththila_check_name_validity (env, p,start_element);
	  p->xsw->start_element_open = 1;
      
	  if (!p->xsw->empty_element_open)
	    AXIS2_STACK_PUSH (p->xsw->element, env, start_element);
	  guththila_write_to_buffer (env, p,"<");
	  guththila_write_to_buffer  (env, p,start_element);
	}
    }
  else
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_EXSISTING_URI);
}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_start_element_with_prefix (axis2_env_t *env, guththila_t *p, const char *prefix, const char *local_name)
{
  int size;
  void *element;
  char *start_element = NULL;
  if (guththila_is_exsisting_prefix (env, p,prefix))
    {
      if (!p || !local_name)
	guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
      else
	{
	  guththila_check_xml_stream_writer (env, p);
	  guththila_close_start_element (env, p);
	  guththila_open_depth_element (env, p);

	  if (prefix)
	    {
	      start_element = (char *) AXIS2_MALLOC (env->allocator, (strlen (prefix) + strlen (local_name) +2));
	      memset (start_element, 1, strlen (start_element));
	    }
	       
	  if (prefix)
	    {
	      strcat (start_element, prefix);
	      strcat (start_element, ":");
	      strcat (start_element, local_name);
	    } 
	  size = AXIS2_STACK_SIZE (p->xsw->element, env);
	  if (size)
	    {
	      element = AXIS2_STACK_GET_AT (p->xsw->element, env, size-1);
	      if (!strcmp ((char *)element, start_element))
		guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
	    }

	  guththila_check_name_validity (env, p, start_element);
	  p->xsw->start_element_open = 1;
      
	  if (!p->xsw->empty_element_open)
	    AXIS2_STACK_PUSH (p->xsw->element, env, start_element);
	  guththila_write_to_buffer (env, p,"<");
	  guththila_write_to_buffer  (env, p,start_element);
	}
    }
  else
    guththila_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_EXSISTING_PREFIX);
}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_empty_element_with_prefix_and_namespace (axis2_env_t *env, guththila_t *p,const char *prefix,
							 const char *namespace_uri, const char *empty_element)
{
  guththila_check_xml_stream_writer (env, p);
  guththila_close_start_element (env, p);
  p->xsw->start_element_open = 0;
  p->xsw->empty_element_open = 1;
  guththila_write_start_element_with_prefix_and_namespace (env, p,prefix, namespace_uri, empty_element);
}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_empty_element_with_namespace (axis2_env_t *env, guththila_t *p, const char *namespace_uri, const char *empty_element)
{
  guththila_check_xml_stream_writer (env, p);
  guththila_close_start_element (env, p);
  p->xsw->start_element_open = 0;
  p->xsw->empty_element_open = 1;
  guththila_write_start_element_with_namespace (env, p,namespace_uri, empty_element);
}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_empty_element_with_prefix (axis2_env_t *env, guththila_t *p, const char *prefix, const char *empty_element)
{
  guththila_check_xml_stream_writer (env, p);
  guththila_close_start_element (env, p);
  p->xsw->start_element_open = 0;
  p->xsw->empty_element_open = 1;
  guththila_write_start_element_with_prefix(env, p,prefix, empty_element);
}


void AXIS2_CALL 
guththila_open_depth_element (axis2_env_t *env, guththila_t *p)
{
  int size = AXIS2_STACK_SIZE (p->xsw->depth, env);
  guththila_depth_t *d = (guththila_depth_t *) AXIS2_MALLOC (env->allocator, sizeof (guththila_depth_t)); 

  if(size)
    {
      void *e = NULL;
      guththila_depth_t *l = NULL;
      e = AXIS2_STACK_GET (p->xsw->depth, env);
      l = (guththila_depth_t *)e;
      d->total = AXIS2_STACK_SIZE (p->xsw->namespace, env);
      d->first = l->first + l->count;
      d->count = d->total - l->total;
      AXIS2_STACK_PUSH (p->xsw->depth, env, (void *)d);
    }
  else
    {
      d->first = 0;
      d->total = AXIS2_STACK_SIZE (p->xsw->namespace, env);
      d->count = d->total;
      AXIS2_STACK_PUSH (p->xsw->depth, env, (void *)d);
    }
}


void AXIS2_CALL
guththila_close_depth_element (axis2_env_t *env, guththila_t *p)
{
  void *e = AXIS2_STACK_POP (p->xsw->depth, env);
  guththila_depth_t *d = (guththila_depth_t *)e;
  void *elem = NULL;
  if (d->count)
    {
      for (; d->count > 0; d->count --)
	{
	  elem = AXIS2_STACK_POP (p->xsw->namespace, env);
	  if (elem)
	    AXIS2_FREE (env->allocator, elem);
	}
    }
}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_end_document (axis2_env_t *env, guththila_t *p)
{
  int ii = 0;
  if (p->xsw->start_element_open || p->xsw->empty_element_open)
    guththila_close_start_element (env, p);
  ii = AXIS2_STACK_SIZE (p->xsw->element, env);
  for (; ii > 0; ii --)
    guththila_write_end_element (env, p);
  guththila_flush (env, p);
}


AXIS2_EXTERN void AXIS2_CALL 
guththila_close (axis2_env_t *env, 
		 guththila_t *p)
{
  guththila_flush (env, p);
  fclose (((guththila_writer_impl_t *)p->xsw->writer)->outputstream);
}


AXIS2_EXTERN void AXIS2_CALL
guththila_write_line (axis2_env_t *env, guththila_t *p, char *local_name, char *characters)
{
  guththila_write_start_element (env, p,local_name);
  guththila_write_characters (env, p,characters);
  guththila_write_end_element (env, p);
  guththila_write_characters (env, p,"\n");
}

AXIS2_EXTERN char* AXIS2_CALL
guththila_get_memory_buffer (axis2_env_t *env, guththila_t *p)
{
  char *buffer = NULL;
  guththila_flush (env, p);
  if (p->xsw)
    buffer = guththila_writer_get_buffer (env, p->xsw->writer);

  return buffer;
}
