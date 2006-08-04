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


void
guththila_xml_pull_parser_createXmlStreamWriter (axis2_env_t *env, guththila_xml_pull_parser_t *p, char *file)
{
  if (p && file)
    {
      p->xsw = (guththila_xml_writer_t *) AXIS2_MALLOC (env->allocator, sizeof (guththila_xml_writer_t));
      p->xsw->writer = guththila_writer_create_for_file (env, file);
      p->xsw->writer_buffer = guththila_buffer_create (env, 1024);
      p->xsw->element = axis2_stack_create (env);
      p->xsw->attribute = axis2_stack_create (env);
      p->xsw->namespace = axis2_stack_create (env);
      p->xsw->depth = axis2_stack_create (env);
      p->xsw->next = 0;
      p->xsw->offset = 0;
      p->xsw->last = 1024;   /* size of the buffer */
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
}


AXIS2_EXTERN void
guththila_xml_pull_parser_freeXmlStreamWriter (axis2_env_t *env, guththila_xml_pull_parser_t *p)
{
  guththila_xml_pull_parser_checkXmlStreamWriter (env, p);
  if (p->xsw->writer)
    free (p->xsw->writer);
  if (p->xsw->writer_buffer)
    free (p->xsw->writer_buffer);
  if (p->xsw)
    free (p->xsw);
}


int
guththila_xml_pull_parser_checkXmlStreamWriter (axis2_env_t *env, guththila_xml_pull_parser_t *p)
{
  if (!p->xsw->writer)
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_WRITER);
  else 
    return 1;
  return 0;
}


AXIS2_EXTERN void
guththila_xml_pull_parser_writeStartDocument (axis2_env_t *env, guththila_xml_pull_parser_t *p)
{
  guththila_xml_pull_parser_checkXmlStreamWriter (env, p); 
  char *sd = "<?xml version = \"1.0\" encoding = \"utf-8\" ?>";
  guththila_xml_pull_parser_writeToBuffer (env, p, sd);
}


AXIS2_EXTERN void
guththila_xml_pull_parser_writeEndElement (axis2_env_t *env, guththila_xml_pull_parser_t *p)
{
  guththila_xml_pull_parser_checkXmlStreamWriter (env, p);
  void *element;
  element = AXIS2_STACK_POP (p->xsw->element, env);
  if (p->xsw->empty_element_open)
    {
      guththila_xml_pull_parser_closeStartElement (env, p);
    }
  else
    {
      guththila_xml_pull_parser_closeStartElement (env, p);
      if (element)
   {
     guththila_xml_pull_parser_writeToBuffer (env, p, "</");
     guththila_xml_pull_parser_writeToBuffer (env, p, element);
     guththila_xml_pull_parser_writeToBuffer (env, p, ">");
     guththila_xml_pull_parser_close_depth_element (env, p);
   }
      else
   guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
    }
}


AXIS2_EXTERN void
guththila_xml_pull_parser_flush (axis2_env_t *env, guththila_xml_pull_parser_t *p)
{
  guththila_xml_pull_parser_checkXmlStreamWriter (env, p);
  int c;
  int ii = strlen (p->xsw->writer_buffer->buff);
  c = guththila_writer_write (env, p->xsw->writer_buffer->buff, 0, ii, p->xsw->writer);
  p->xsw->writer_buffer->buff[0] = 0;
  p->xsw->writer_buffer->buff[1] = 0;
  p->xsw->next = 0;
  p->xsw->offset = 0;
}


AXIS2_EXTERN void
guththila_xml_pull_parser_writeStartElement (axis2_env_t *env, guththila_xml_pull_parser_t *p, char *start_element)
{
  int size;
  void *element;
  if (!p || !start_element)
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
  else
    {
      guththila_xml_pull_parser_checkXmlStreamWriter (env, p);
      guththila_xml_pull_parser_closeStartElement (env, p);
      size = AXIS2_STACK_SIZE (p->xsw->element, env);
      if (size)
   {
     element = AXIS2_STACK_GET_AT (p->xsw->element, env, 0);
     if (!strcmp ((char *)element, start_element))
       guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
   }
      guththila_xml_pull_parser_checkNameValidity (env, p,start_element);
      p->xsw->start_element_open = 1;

      if (!p->xsw->empty_element_open)
   AXIS2_STACK_PUSH (p->xsw->element, env, start_element);
      guththila_xml_pull_parser_writeToBuffer (env, p,"<");
      guththila_xml_pull_parser_writeToBuffer  (env, p,start_element);
    }
}


AXIS2_EXTERN void
guththila_xml_pull_parser_writeToBuffer (axis2_env_t *env, guththila_xml_pull_parser_t *p, const char *buff)
{
  if (buff)
    {
      guththila_xml_pull_parser_checkXmlStreamWriter (env, p);
      if (p->xsw->next >= p->xsw->last)
   {
     guththila_xml_pull_parser_flush (env, p);
     p->xsw->next = 0;
     p->xsw->offset = 0;
   }
      else
   {
     int length;
     length = strlen (buff);
     strcat ((p->xsw->writer_buffer->buff) + (p->xsw->next), buff);
     p->xsw->offset = p->xsw->next;
     p->xsw->next += length;
   }
    }
}


AXIS2_EXTERN void
guththila_xml_pull_parser_writeCharacters (axis2_env_t *env, guththila_xml_pull_parser_t *p, const char *buff)
{
  guththila_xml_pull_parser_checkXmlStreamWriter (env, p);
  guththila_xml_pull_parser_closeStartElement (env, p);
  
  if (buff)
    {
      guththila_xml_pull_parser_writeToBuffer (env, p,buff);
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_INVALID_BUFFER);
}


AXIS2_EXTERN void
guththila_xml_pull_parser_closeStartElement (axis2_env_t *env, guththila_xml_pull_parser_t *p)
{
  if (p->xsw->start_element_open && p->xsw->empty_element_open)
    {
      p->xsw->start_element_open = 0;
      p->xsw->empty_element_open = 0;
      guththila_xml_pull_parser_writeToBuffer (env, p,"/>");
   /*    Stack_clear (p->xsw->attribute); */
    }
  
  if (p->xsw->start_element_open && !p->xsw->empty_element_open)
    {
      p->xsw->start_element_open = 0;
      guththila_xml_pull_parser_writeToBuffer (env, p,">");
/*       Stack_clear (p->xsw->attribute); */
    }

  guththila_xml_pull_parser_open_depth_element (env, p);
}


AXIS2_EXTERN void
guththila_xml_pull_parser_checkNameValidity (axis2_env_t *env, guththila_xml_pull_parser_t *p, char *name)
{
  int length;
  int ii;
  length = strlen (name);
  /* ideograhic characters are also valid for name_starting charactes
   * need to add them to here isIdeograpic function doesn't
   * functionning correctly yet*/
  if  (!(isalpha (name[0]) || name[0] == '_' || name[0] == ':'))
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_INVALID_CHAR_IN_NAME);

  /* xml in any case combination isn't allow */
  if ((name[0] == 'x' || name[0] == 'X')
      && (name[1] == 'm' || name[1] == 'M')
      && (name[2] == 'l' || name[2] == 'L'))
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_XML_STRING_IN_NAME);
  
  /* some punctuation's not allowed */
  for (ii = 1; ii < length; ii++)
    {
      if ((name[ii] == '$' || name[ii] == '^' || name[ii] == '%'
      || name[ii] == ';' || name[ii] == '\'' || name[ii] == '"'
      || name[ii] == '&' || name[ii] == '<' || name[ii] == '>'
      || isspace(name[ii])))
   guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_INVALID_CHAR_IN_NAME);
    }
}


int
guththila_xml_pull_parser_isIdeographic (char *id)
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


AXIS2_EXTERN void
guththila_xml_pull_parser_writeComment (axis2_env_t *env, guththila_xml_pull_parser_t *p, const char *buff)
{
  guththila_xml_pull_parser_checkXmlStreamWriter (env, p);
  guththila_xml_pull_parser_closeStartElement (env, p);
  char *s = strchr (buff, '-');
  if (s && s[1] == '-')
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EXCESS_HYPENS_IN_COMMENT);

  guththila_xml_pull_parser_writeToBuffer (env, p,"<!--");
  guththila_xml_pull_parser_writeToBuffer (env, p,buff);
  guththila_xml_pull_parser_writeToBuffer (env, p,"-->");
}


AXIS2_EXTERN void
guththila_xml_pull_parser_writeEscapeCharacter (axis2_env_t *env, guththila_xml_pull_parser_t *p, const char *buff)
{
  guththila_xml_pull_parser_closeStartElement (env, p);
  if (buff)
    {
      switch (buff[0])
   {
   case '>':
     {
       guththila_xml_pull_parser_writeToBuffer (env, p,"&gt;");
     }
     break;
   case '<':
     {
       guththila_xml_pull_parser_writeToBuffer (env, p,"&lt;");
     }
     break;
   case '\'':
     {
       guththila_xml_pull_parser_writeToBuffer (env, p,"&apos;");
     }
     break;
   case '"':
     {
       guththila_xml_pull_parser_writeToBuffer (env, p,"&quot;");
     }
     break;
   case '&':
     {
       guththila_xml_pull_parser_writeToBuffer (env, p,"&amp;");
     }
     break;
   };
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_INVALID_BUFFER);
}


AXIS2_EXTERN void
guththila_xml_pull_parser_writeAttribute (axis2_env_t *env, guththila_xml_pull_parser_t *p, const char *local_name, const char *value)
{
  int size = 0;
  void *element;
  guththila_attribute_t *attr;

  if (!p->xsw->start_element_open)
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);

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
         if (!strcmp((char *)attr->name, local_name))
      guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
         else
      {
        guththila_xml_pull_parser_doWriteAttribute (env, p,local_name, value);
        break;
      }
       }
   }
       
    }
  else
    {
      guththila_xml_pull_parser_doWriteAttribute (env, p,local_name, value);
    }
}


AXIS2_EXTERN void
guththila_xml_pull_parser_doWriteAttribute (axis2_env_t *env, guththila_xml_pull_parser_t *p, const char *local_name, const char *value)
{
 guththila_attribute_t *attr = (guththila_attribute_t *) AXIS2_MALLOC (env->allocator, sizeof (guththila_attribute_t));
  
  if (local_name)
    attr->name = (guththila_token_t *)local_name;
  if (value)
    attr->value = (guththila_token_t *)value;
  if (local_name && value)
    {
      AXIS2_STACK_PUSH (p->xsw->attribute, env, (AXIS2_EXTERN void *)attr);
    }

  guththila_xml_pull_parser_checkNameValidity (env, p,(char *)local_name);
  
  if (strrchr (value, '&') 
      || strrchr (value, '<') 
      || strrchr (value, '\''))
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_INVALID_CHAR_IN_ATTRIBUTE);

  guththila_xml_pull_parser_writeToBuffer (env, p," ");
  guththila_xml_pull_parser_writeToBuffer (env, p,local_name);
  guththila_xml_pull_parser_writeToBuffer (env, p," = \'");
  guththila_xml_pull_parser_writeToBuffer (env, p,value);
  guththila_xml_pull_parser_writeToBuffer (env, p,"\'");
}


AXIS2_EXTERN void
guththila_xml_pull_parser_writeEmptyElement (axis2_env_t *env, guththila_xml_pull_parser_t *p, char *empty_element)
{
  guththila_xml_pull_parser_checkXmlStreamWriter (env, p);
  guththila_xml_pull_parser_closeStartElement (env, p);
  p->xsw->start_element_open = 0;
  p->xsw->empty_element_open = 1;
  guththila_xml_pull_parser_writeStartElement (env, p,empty_element);
}


AXIS2_EXTERN void
guththila_xml_pull_parser_writeDefaultNamespace (axis2_env_t *env, guththila_xml_pull_parser_t *p, char *namespace_uri)
{
  if (!p->xsw->start_element_open)
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
  else
    {
     
      if (guththila_xml_pull_parser_checkDefaultNamespace (env, p,namespace_uri))
   {
     guththila_xml_pull_parser_doWriteDefaultNamespace (env, p,namespace_uri);
   }
    }
}


int
guththila_xml_pull_parser_checkDefaultNamespace (axis2_env_t *env, guththila_xml_pull_parser_t *p, char *ns_uri)
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
          guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
      }
       }
   }
      return 1;

    }
  else
    return 1;
}


AXIS2_EXTERN void
guththila_xml_pull_parser_doWriteDefaultNamespace (axis2_env_t *env, guththila_xml_pull_parser_t *p, char *ns_uri)
{
guththila_namespace_t *ns = (guththila_namespace_t *) AXIS2_MALLOC (env->allocator, sizeof (guththila_namespace_t));
  ns->name = NULL;
  ns->length = 0;
  ns->uri = ns_uri;
  ns->lengthuri = strlen (ns_uri);
  AXIS2_STACK_PUSH (p->xsw->namespace, env, (AXIS2_EXTERN void *)ns);

  guththila_xml_pull_parser_writeToBuffer (env, p," ");
  guththila_xml_pull_parser_writeToBuffer (env, p,"xmlns");
  guththila_xml_pull_parser_writeToBuffer (env, p," = \'");
  guththila_xml_pull_parser_writeToBuffer (env, p,ns_uri);
  guththila_xml_pull_parser_writeToBuffer (env, p,"\'");
}


AXIS2_EXTERN void
guththila_xml_pull_parser_writeNamespace (axis2_env_t *env, guththila_xml_pull_parser_t *p, char *prefix, char *uri)
{
  if (!prefix || !strcmp(prefix, "xmlns"))
    guththila_xml_pull_parser_doWriteDefaultNamespace (env, p,uri);
  
  if (!p->xsw->start_element_open)
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
  else
    {
      if (guththila_xml_pull_parser_checkPrefixValidity (env, p,prefix, uri))
   guththila_xml_pull_parser_doWriteNamespace (env, p,prefix, uri);
    }

}


int 
guththila_xml_pull_parser_checkPrefixValidity (axis2_env_t *env, guththila_xml_pull_parser_t *p, char *prefix, char *uri)
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
         if (ns->name)
      {
        if (!strcmp (ns->name, prefix))
          guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
      }
         if (uri)
      {
        if (ns->uri)
          {
            if (!strcmp (ns->uri, uri))
         guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
          }
      }
       }
   }
      return 1;
    }
  else
    return 1;
}


AXIS2_EXTERN void
guththila_xml_pull_parser_doWriteNamespace (axis2_env_t *env, guththila_xml_pull_parser_t *p, char *prefix, char *uri)
{
  guththila_namespace_t *ns = (guththila_namespace_t *) AXIS2_MALLOC (env->allocator, sizeof (guththila_namespace_t));
  ns->name = prefix;
  ns->length = strlen (prefix);
  ns->uri = uri;
  ns->lengthuri = strlen (uri);
  AXIS2_STACK_PUSH (p->xsw->namespace, env, (AXIS2_EXTERN void *)ns);
  
  guththila_xml_pull_parser_writeToBuffer (env, p," ");
  guththila_xml_pull_parser_writeToBuffer (env, p,"xmlns:");
  guththila_xml_pull_parser_writeToBuffer (env, p,prefix);
  guththila_xml_pull_parser_writeToBuffer (env, p," = \'");
  guththila_xml_pull_parser_writeToBuffer (env, p,uri);
  guththila_xml_pull_parser_writeToBuffer (env, p,"\'");
}


AXIS2_EXTERN void
guththila_xml_pull_parser_writeAttribute_with_prefix_and_namespace (axis2_env_t *env, guththila_xml_pull_parser_t *p,
                      const char *prefix, const char *namespace,
                      const char *local_name, const char *value)
{
  int size = 0;
  void *element;
  guththila_attribute_t *attr;
  
  if (!p->xsw->start_element_open)
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);

  if (prefix && namespace)
    guththila_xml_pull_parser_writeNamespace (env, p,(char *)prefix, (char *)namespace);

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
          guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
        else
          {
            guththila_xml_pull_parser_doWriteAttribute_with_prefix_and_namespace (env, p,prefix, namespace, local_name, value);
            break;
          }
      }
         else
      {
        /* since att->prefix is null im going to write it here */
        guththila_xml_pull_parser_doWriteAttribute_with_prefix_and_namespace (env, p,prefix, namespace, local_name, value);
        break;
      }
       }
   }
      
    }
  else
    {
      guththila_xml_pull_parser_doWriteAttribute_with_prefix_and_namespace (env, p,prefix, namespace, local_name, value);
    }

}


AXIS2_EXTERN void
guththila_xml_pull_parser_doWriteAttribute_with_prefix_and_namespace (axis2_env_t *env, guththila_xml_pull_parser_t *p, 
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
      AXIS2_STACK_PUSH (p->xsw->attribute, env, (AXIS2_EXTERN void *)attr);
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);

  guththila_xml_pull_parser_checkNameValidity (env, p,(char *)local_name);
  
  if (strrchr (value, '&') 
      || strrchr (value, '<') 
      || strrchr (value, '\''))
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_INVALID_CHAR_IN_ATTRIBUTE);

  guththila_xml_pull_parser_writeToBuffer (env, p," ");
  if (prefix)
    {
      guththila_xml_pull_parser_writeToBuffer (env, p,prefix);
      guththila_xml_pull_parser_writeToBuffer (env, p,":");
    }
  guththila_xml_pull_parser_writeToBuffer (env, p,local_name);
  guththila_xml_pull_parser_writeToBuffer (env, p," = \'");
  guththila_xml_pull_parser_writeToBuffer (env, p,value);
  guththila_xml_pull_parser_writeToBuffer (env, p,"\'");
}


AXIS2_EXTERN void
guththila_xml_pull_parser_writeAttribute_with_prefix (axis2_env_t *env, guththila_xml_pull_parser_t *p, const char *prefix,
                  const char *local_name, const char *value)
{
  int size = 0;
  void *element;
  guththila_attribute_t *attr;

  if (!p->xsw->start_element_open)
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
  /* actually here we want exsisting prefix , that's why we check it
     exsists or not. */
  if (guththila_xml_pull_parser_isExsistingPrefix (env, p,prefix))
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
         guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
            else
         {
           guththila_xml_pull_parser_doWriteAttribute_with_prefix (env, p,prefix, local_name, value);
           break;
         }
          }
        else
          {
            /* since att->prefix is null im going to write it here */
            guththila_xml_pull_parser_doWriteAttribute_with_prefix (env, p,prefix, local_name, value);
            break;
          }
      }
       }
      
   }
      else
   {
     guththila_xml_pull_parser_doWriteAttribute_with_prefix (env, p,prefix, local_name, value);
   }
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_EXSISTING_PREFIX);

}


AXIS2_EXTERN void
guththila_xml_pull_parser_doWriteAttribute_with_prefix(axis2_env_t *env, guththila_xml_pull_parser_t *p, const char *prefix,
                   const char *local_name, const char *value)
{
  guththila_xml_pull_parser_doWriteAttribute_with_prefix_and_namespace (env, p,prefix, NULL, local_name, value);
}


int
guththila_xml_pull_parser_isExsistingPrefix (axis2_env_t *env, guththila_xml_pull_parser_t *p, const char *prefix)
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
guththila_xml_pull_parser_isExsistingNamespaceUri (axis2_env_t *env, guththila_xml_pull_parser_t *p, const char *uri)
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


AXIS2_EXTERN void
guththila_xml_pull_parser_writeAttribute_with_namespace (axis2_env_t *env, guththila_xml_pull_parser_t *p, const char *namespace,
                     const char *local_name, const char *value)
{
  int size = 0;
  void *element;
  guththila_attribute_t *attr;
  if (!p->xsw->start_element_open)
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
  /* actually here we want exsisting namespace , that's why we check it
     exsists or not. */
  if (guththila_xml_pull_parser_isExsistingNamespaceUri (env, p,namespace))
    {
      char *prefix;
      prefix = guththila_xml_pull_parser_getPrefixForNamespace (env, p, namespace);
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
         guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
            else
         {
           guththila_xml_pull_parser_doWriteAttribute_with_prefix (env, p,prefix, local_name, value);
           break;
         }
          }
        else
          {
            /* since att->prefix is null im going to write it here */
            guththila_xml_pull_parser_doWriteAttribute_with_prefix (env, p,prefix, local_name, value);
            break;
          }
      }
       }
      
   }
      else
   {
     guththila_xml_pull_parser_doWriteAttribute_with_prefix (env, p,prefix, local_name, value);
   }
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_EXSISTING_URI);
}


char *
guththila_xml_pull_parser_getPrefixForNamespace (axis2_env_t *env, guththila_xml_pull_parser_t *p, const char *namespace)
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


AXIS2_EXTERN void
guththila_xml_pull_parser_writeStartElement_with_prefix_and_namespace (axis2_env_t *env, guththila_xml_pull_parser_t *p, const char *prefix,
                         const char *namespace_uri, const char *local_name)
{
  int size;
  void *element;
  char *start_element = (char *) calloc ((strlen (prefix) + strlen (local_name) +2), 1);
  if (!p || !local_name)
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
  else
    {
      guththila_xml_pull_parser_checkXmlStreamWriter (env, p);
      guththila_xml_pull_parser_closeStartElement (env, p);
     
      if (prefix)
   {
     strcat (start_element, prefix);
     strcat (start_element, ":");
     strcat (start_element, local_name);
   } 
      size = AXIS2_STACK_SIZE (p->xsw->element, env);
      if (size)
   {
     element = AXIS2_STACK_GET_AT (p->xsw->element, env, size);
     if (!strcmp ((char *)element, start_element))
       guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
   }

      guththila_xml_pull_parser_checkNameValidity (env, p,start_element);
      p->xsw->start_element_open = 1;
      
      if (!p->xsw->empty_element_open)
   AXIS2_STACK_PUSH (p->xsw->element, env, start_element);
      guththila_xml_pull_parser_writeToBuffer (env, p,"<");
      guththila_xml_pull_parser_writeToBuffer  (env, p,start_element);
      guththila_xml_pull_parser_writeNamespace (env, p,(char *)prefix, (char *)namespace_uri);
    }
}


AXIS2_EXTERN void
guththila_xml_pull_parser_writeStartElement_with_namespace (axis2_env_t *env, guththila_xml_pull_parser_t *p, const char *namespace_uri, const char *local_name)
{
  int size;
  void *element;
  char *start_element;
  if (guththila_xml_pull_parser_isExsistingNamespaceUri (env, p,namespace_uri))
    {
      if (!p || !local_name)
   guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
      else
   {
     guththila_xml_pull_parser_checkXmlStreamWriter (env, p);
     guththila_xml_pull_parser_closeStartElement (env, p);
     char *prefix = guththila_xml_pull_parser_getPrefixForNamespace (env, p,namespace_uri);
     if (prefix)
       start_element = (char *) calloc ((strlen (prefix) + strlen (local_name) +2), 1);
     else
       start_element = (char *) calloc ((strlen (local_name) +2), 1);
     
     if (prefix)
       {
         strcat (start_element, prefix);
         strcat (start_element, ":");
         strcat (start_element, local_name);
       } 
     else
       strcat (start_element, local_name);
     size = AXIS2_STACK_SIZE (p->xsw->element, env);
     if (size)
       {
         element = AXIS2_STACK_GET_AT (p->xsw->element, env, size);
         if (!strcmp ((char *)element, start_element))
      guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
       }

     guththila_xml_pull_parser_checkNameValidity (env, p,start_element);
     p->xsw->start_element_open = 1;
      
     if (!p->xsw->empty_element_open)
       AXIS2_STACK_PUSH (p->xsw->element, env, start_element);
     guththila_xml_pull_parser_writeToBuffer (env, p,"<");
     guththila_xml_pull_parser_writeToBuffer  (env, p,start_element);
   }
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_EXSISTING_URI);
}


AXIS2_EXTERN void
guththila_xml_pull_parser_writeStartElement_with_prefix (axis2_env_t *env, guththila_xml_pull_parser_t *p, const char *prefix, const char *local_name)
{
  int size;
  void *element;
  char *start_element = NULL;
  if (guththila_xml_pull_parser_isExsistingPrefix (env, p,prefix))
    {
      if (!p || !local_name)
   guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_EMPTY_ARGUMENTS);
      else
   {
     guththila_xml_pull_parser_checkXmlStreamWriter (env, p);
     guththila_xml_pull_parser_closeStartElement (env, p);
     
     if (prefix)
       start_element = (char *) calloc ((strlen (prefix) + strlen (local_name) +2), 1);
          
     if (prefix)
       {
         strcat (start_element, prefix);
         strcat (start_element, ":");
         strcat (start_element, local_name);
       } 
     size = AXIS2_STACK_SIZE (p->xsw->element, env);
     if (size)
       {
         element = AXIS2_STACK_GET_AT (p->xsw->element, env, size);
         if (!strcmp ((char *)element, start_element))
      guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_MATCHING_ELEMENTS);
       }

     guththila_xml_pull_parser_checkNameValidity (env, p, start_element);
     p->xsw->start_element_open = 1;
      
     if (!p->xsw->empty_element_open)
       AXIS2_STACK_PUSH (p->xsw->element, env, start_element);
     guththila_xml_pull_parser_writeToBuffer (env, p,"<");
     guththila_xml_pull_parser_writeToBuffer  (env, p,start_element);
   }
    }
  else
    guththila_xml_pull_parser_exception (p_FILE, LINE, GUTHTHILA_WRITER_ERROR_NON_EXSISTING_PREFIX);
}


AXIS2_EXTERN void
guththila_xml_pull_parser_writeEmptyElement_with_prefix_and_namespace (axis2_env_t *env, guththila_xml_pull_parser_t *p,const char *prefix,
                         const char *namespace_uri, const char *empty_element)
{
  guththila_xml_pull_parser_checkXmlStreamWriter (env, p);
  guththila_xml_pull_parser_closeStartElement (env, p);
  p->xsw->start_element_open = 0;
  p->xsw->empty_element_open = 1;
  guththila_xml_pull_parser_writeStartElement_with_prefix_and_namespace (env, p,prefix, namespace_uri, empty_element);
}


AXIS2_EXTERN void
guththila_xml_pull_parser_writeEmptyElement_with_namespace (axis2_env_t *env, guththila_xml_pull_parser_t *p, const char *namespace_uri, const char *empty_element)
{
  guththila_xml_pull_parser_checkXmlStreamWriter (env, p);
  guththila_xml_pull_parser_closeStartElement (env, p);
  p->xsw->start_element_open = 0;
  p->xsw->empty_element_open = 1;
  guththila_xml_pull_parser_writeStartElement_with_namespace (env, p,namespace_uri, empty_element);
}


AXIS2_EXTERN void
guththila_xml_pull_parser_writeEmptyElement_with_prefix (axis2_env_t *env, guththila_xml_pull_parser_t *p, const char *prefix, const char *empty_element)
{
  guththila_xml_pull_parser_checkXmlStreamWriter (env, p);
  guththila_xml_pull_parser_closeStartElement (env, p);
  p->xsw->start_element_open = 0;
  p->xsw->empty_element_open = 1;
  guththila_xml_pull_parser_writeStartElement_with_prefix(env, p,prefix, empty_element);
}

AXIS2_EXTERN void 
guththila_xml_pull_parser_open_depth_element (axis2_env_t *env, guththila_xml_pull_parser_t *p)
{
  int size = AXIS2_STACK_SIZE (p->xsw->depth, env);
  guththila_depth_t *d = (guththila_depth_t *) AXIS2_MALLOC (env->allocator, sizeof (guththila_depth_t));
  if (size)
    {
      void *e = NULL;
      e = AXIS2_STACK_GET (p->xsw->depth, env);
      guththila_depth_t *l = (guththila_depth_t *)e;
      d->total = AXIS2_STACK_SIZE (p->xsw->namespace, env);
      d->first = l->first + l->count;
      d->count = d->total - l->total;
    }
  else
    {
      d->first = 0;
      d->total = AXIS2_STACK_SIZE (p->xsw->namespace, env);
      d->count = d->total;
      AXIS2_STACK_PUSH (p->xsw->depth, env, (void *)d);
    }
}

AXIS2_EXTERN void
guththila_xml_pull_parser_close_depth_element (axis2_env_t *env, guththila_xml_pull_parser_t *p)
{
  void *e = AXIS2_STACK_POP (p->xsw->depth, env);
  guththila_depth_t *d = (guththila_depth_t *)e;
  void *elem = NULL;
  guththila_namespace_t *ns = NULL;
  if (d->count)
    {
      for (; d->count > 0; d->count --)
   {
     elem = AXIS2_STACK_POP (p->xsw->namespace, env);
     ns = elem;
     if (ns)
       free (ns);
   }
    }
}

AXIS2_EXTERN void
guththila_xml_pull_parser_writeEndDocument (axis2_env_t *env, guththila_xml_pull_parser_t *p)
{
  if (p->xsw->start_element_open || p->xsw->empty_element_open)
    guththila_xml_pull_parser_closeStartElement (env, p);
  int ii = AXIS2_STACK_SIZE (p->xsw->element, env);
  for (; ii > 0; ii --)
    guththila_xml_pull_parser_writeEndElement (env, p);
  guththila_xml_pull_parser_flush (env, p);
}

AXIS2_EXTERN void 
guththila_xml_pull_parser_close (axis2_env_t *env, guththila_xml_pull_parser_t *p)
{
  guththila_xml_pull_parser_flush (env, p);
  fclose (((guththila_writer_impl_t *)p->xsw->writer)->outputstream);
}

AXIS2_EXTERN void
guththila_xml_pull_parser_writeLine (axis2_env_t *env, guththila_xml_pull_parser_t *p, char *local_name, char *characters)
{
  guththila_xml_pull_parser_writeStartElement (env, p,local_name);
  guththila_xml_pull_parser_writeCharacters (env, p,characters);
  guththila_xml_pull_parser_writeEndElement (env, p);
  guththila_xml_pull_parser_writeCharacters (env, p,"\n");
}
