/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include <stdlib.h>
#include <axis2_stream.h>
#include <platforms/axis2_platform_auto_sense.h>
/** 
 * @brief Stream struct impl
 *   Axis2 Stream impl  
 */
typedef struct axis2_stream_impl axis2_stream_impl_t;  
  
struct axis2_stream_impl
{
   axis2_stream_t stream;
   axis2_stream_type_t stream_type;
   int len;
   int max_len;
   /* Only one of these is used for a perticlar
    * instance depending on the type
    */
   axis2_char_t *buffer;
   FILE *fp;
   int socket;   
};

#define AXIS2_INTF_TO_IMPL(stream) ((axis2_stream_impl_t *)(stream))

/********************************Function headers******************************/
axis2_status_t AXIS2_CALL 
axis2_stream_free (axis2_stream_t *stream, const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_stream_free_void_arg (void *stream,
                            const axis2_env_t *env);

/** basic stream operatons **/
int AXIS2_CALL
axis2_stream_write_basic(axis2_stream_t *stream, const axis2_env_t *env, 
                  const void *buffer, size_t count);
int AXIS2_CALL 
axis2_stream_read_basic (axis2_stream_t *stream, const axis2_env_t *env, 
                  void *buffer, size_t count);
int AXIS2_CALL 
axis2_stream_get_len_basic (axis2_stream_t *stream, const axis2_env_t *env);

int AXIS2_CALL 
axis2_stream_skip_basic (axis2_stream_t *stream, const axis2_env_t *env, int count);

/** file stream operations **/
int AXIS2_CALL
axis2_stream_write_file(axis2_stream_t *stream, const axis2_env_t *env, 
                  const void *buffer, size_t count);
int AXIS2_CALL 
axis2_stream_read_file (axis2_stream_t *stream, const axis2_env_t *env, 
                  void *buffer, size_t count);
int AXIS2_CALL 
axis2_stream_get_len_file (axis2_stream_t *stream, const axis2_env_t *env);

int AXIS2_CALL 
axis2_stream_skip_file (axis2_stream_t *stream, const axis2_env_t *env, int count);

/** socket stream operations **/
int AXIS2_CALL
axis2_stream_write_socket(axis2_stream_t *stream, const axis2_env_t *env, 
                  const void *buffer, size_t count);
int AXIS2_CALL 
axis2_stream_read_socket (axis2_stream_t *stream, const axis2_env_t *env, 
                  void *buffer, size_t count);
int AXIS2_CALL 
axis2_stream_get_len_socket (axis2_stream_t *stream, const axis2_env_t *env);

int AXIS2_CALL 
axis2_stream_skip_socket (axis2_stream_t *stream, const axis2_env_t *env, int count);

/************************* End of function headers ****************************/
/*
 * Internal function. Not exposed to outside
 */
AXIS2_EXTERN axis2_stream_t * AXIS2_CALL
axis2_stream_create_internal (const axis2_env_t *env)
{
    axis2_stream_impl_t *stream_impl = NULL;
   AXIS2_ENV_CHECK(env, NULL);
       
   stream_impl = (axis2_stream_impl_t *)AXIS2_MALLOC(
                  env->allocator, sizeof(axis2_stream_impl_t));
   
   if(NULL == stream_impl)
   {
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
   }
   stream_impl->buffer = NULL;
   stream_impl->fp = NULL;   
   stream_impl->socket = -1;
   stream_impl->stream.ops = (axis2_stream_ops_t *) AXIS2_MALLOC (
                  env->allocator, sizeof (axis2_stream_ops_t));
   if (NULL == stream_impl->stream.ops)
   {
      axis2_stream_free(&(stream_impl->stream), env);
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
      return NULL;
   }
    stream_impl->stream.axis2_eof = EOF;
   
   stream_impl->stream.ops->free_fn = axis2_stream_free;
   stream_impl->stream.ops->free_void_arg = axis2_stream_free_void_arg; 
   return &(stream_impl->stream);
}


axis2_status_t AXIS2_CALL
axis2_stream_free (axis2_stream_t *stream, const axis2_env_t *env)
{
    axis2_stream_impl_t *stream_impl = NULL;
   AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   
   stream_impl = AXIS2_INTF_TO_IMPL(stream);
   
   switch (stream_impl->stream_type)
   {
      case AXIS2_STREAM_BASIC:
      {
         if(NULL != stream_impl->buffer)
         {
            AXIS2_FREE(env->allocator, stream_impl->buffer);
         }
         stream_impl->buffer = NULL;
         stream_impl->len = -1;
         break;
      }
      case AXIS2_STREAM_FILE:
      {
         stream_impl->fp = NULL;
         stream_impl->len = -1;
         break;
      }
      case AXIS2_STREAM_SOCKET:
      {
         if(NULL != stream_impl->fp)
         {
            fclose(stream_impl->fp);
         }
         stream_impl->socket = -1;
         stream_impl->len = -1;
         break;
      }
        default: break;
   }
      
   if (NULL != stream_impl->stream.ops)
    {
        AXIS2_FREE (env->allocator, stream_impl->stream.ops);
    }
      AXIS2_FREE(env->allocator, stream_impl);
   
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_stream_free_void_arg (void *stream,
                            const axis2_env_t *env)
{
    axis2_stream_t *stream_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    stream_l = (axis2_stream_t *) stream;
    return axis2_stream_free(stream_l, env);
}

/************************ Basic Stream Operations *****************************/
AXIS2_EXTERN axis2_stream_t * AXIS2_CALL
axis2_stream_create_basic (const axis2_env_t *env)
{
   axis2_stream_t *def_stream = NULL;
   axis2_stream_impl_t *stream_impl = NULL;
   
   AXIS2_ENV_CHECK(env, NULL);
   def_stream = axis2_stream_create_internal(env);
   if(NULL == def_stream)
   {
      /*
       * We leave the error returned by the 
       * axis2_stream_create_internal intact
       */
      return NULL;
   }
   stream_impl = AXIS2_INTF_TO_IMPL(def_stream);
   stream_impl->stream_type = AXIS2_STREAM_BASIC;
   stream_impl->stream.ops->read = axis2_stream_read_basic;
   stream_impl->stream.ops->write = axis2_stream_write_basic;
   stream_impl->stream.ops->get_len = axis2_stream_get_len_basic;
   stream_impl->stream.ops->skip = axis2_stream_skip_basic;
   stream_impl->buffer = (axis2_char_t*)AXIS2_MALLOC(env->allocator, 
                  AXIS2_STREAM_DEFAULT_BUF_SIZE*sizeof(axis2_char_t));
   stream_impl->len = 0;
   stream_impl->max_len =    AXIS2_STREAM_DEFAULT_BUF_SIZE;
   
   if(NULL == stream_impl->buffer)
   {
      axis2_stream_free(def_stream, env);
      return NULL;   
   }
   return def_stream;
}


int AXIS2_CALL 
axis2_stream_read_basic (axis2_stream_t *stream, const axis2_env_t *env, 
                  void *buffer, size_t count)
{
   int len = 0;
   char *buf = NULL;
   
   AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
   
   buf = AXIS2_INTF_TO_IMPL(stream)->buffer;
   if(NULL == buf)
   {
      return -1;
   }
    if (NULL == buffer)
   {
        return -1;
   }
   if((count - 1) > AXIS2_INTF_TO_IMPL(stream)->len)
   {
      len = AXIS2_INTF_TO_IMPL(stream)->len;
   }
   else
   {
      len = count - 1;
   }
   memcpy(buffer, buf, len);
    /*
    * Finally we need to remove the read bytes from the stream
    * adjust the length of the stream.
    */
   AXIS2_INTF_TO_IMPL(stream)->len -= len;
   memmove(buf, buf + len * sizeof(axis2_char_t), 
                  AXIS2_INTF_TO_IMPL(stream)->len * sizeof(axis2_char_t));
   ((axis2_char_t *) buffer)[len] = '\0';
    return len;
}

int AXIS2_CALL
axis2_stream_write_basic(axis2_stream_t *stream, const axis2_env_t *env, 
                  const void *buffer, size_t count)
{
   axis2_stream_impl_t *stream_impl = NULL;
   int new_len = 0;
   
   AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
   if (NULL == buffer)
      return -1;
   
   stream_impl = AXIS2_INTF_TO_IMPL(stream);
   new_len = stream_impl->len + count;
   if(new_len > stream_impl->max_len)
   {
      axis2_char_t *tmp = (axis2_char_t *)AXIS2_MALLOC(env->allocator,
                  sizeof(axis2_char_t)*(new_len + 
                  AXIS2_STREAM_DEFAULT_BUF_SIZE));
      if(NULL == tmp)
      {
         AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, 
                  AXIS2_FAILURE);
         return -1;
      }
      /* 
       * pre allocation: extra AXIS2_STREAM_DEFAULT_BUF_SIZE more bytes 
       * allocated 
       */
      stream_impl->max_len = new_len + AXIS2_STREAM_DEFAULT_BUF_SIZE;
      memcpy(tmp, stream_impl->buffer, sizeof(axis2_char_t)*stream_impl->len);
      AXIS2_FREE(env->allocator, stream_impl->buffer);
      stream_impl->buffer = tmp;
   }
   memcpy(stream_impl->buffer + (stream_impl->len * sizeof(axis2_char_t)), 
                  buffer, count);
   stream_impl->len += count;   
    return count;
}


int AXIS2_CALL 
axis2_stream_get_len_basic (axis2_stream_t *stream, const axis2_env_t *env)
{
   AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
   return AXIS2_INTF_TO_IMPL(stream)->len;
}

int AXIS2_CALL 
axis2_stream_skip_basic (axis2_stream_t *stream, const axis2_env_t *env, int count)
{
   axis2_stream_impl_t *stream_impl = NULL;
   int del_len = 0;
   AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
   
   stream_impl = AXIS2_INTF_TO_IMPL(stream);
   if(count > 0)
   {
      if(count <= stream_impl->len)
      {
         del_len= count;
      }
      else
      {
         del_len = stream_impl->len;
      }
      stream_impl->len -= del_len;
      memmove(stream_impl->buffer, stream_impl->buffer + 
               del_len * sizeof(axis2_char_t), 
               stream_impl->len * sizeof(axis2_char_t));
      return del_len;
   }
   return -1;
}


/********************* End of Basic Stream Operations *************************/

/************************** File Stream Operations ****************************/
AXIS2_EXTERN axis2_stream_t * AXIS2_CALL
axis2_stream_create_file (const axis2_env_t *env, FILE *fp)
{
   axis2_stream_t *def_stream = NULL;
   axis2_stream_impl_t *stream_impl = NULL;
   
   AXIS2_ENV_CHECK(env, NULL);
   def_stream = axis2_stream_create_internal(env);
   if(NULL == def_stream)
   {
      /*
       * We leave the error returned by the 
       * axis2_stream_create_internal intact
       */
      return NULL;
   }
   stream_impl = AXIS2_INTF_TO_IMPL(def_stream);
   stream_impl->stream_type = AXIS2_STREAM_FILE;
   stream_impl->fp = fp;
   
   stream_impl->stream.ops->read = axis2_stream_read_file;
   stream_impl->stream.ops->write = axis2_stream_write_file;
   stream_impl->stream.ops->get_len = axis2_stream_get_len_file;
   stream_impl->stream.ops->skip = axis2_stream_skip_file;
   
   return def_stream;
}


int AXIS2_CALL 
axis2_stream_read_file (axis2_stream_t *stream, const axis2_env_t *env, 
                  void *buffer, size_t count)
{
   FILE *fp = NULL;
   AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
   
   if(NULL == AXIS2_INTF_TO_IMPL(stream)->fp)
   {
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_FD, AXIS2_FAILURE);
      return -1;
   }
   fp = AXIS2_INTF_TO_IMPL(stream)->fp;
    if (NULL == buffer)
   {
        return -1;
   }
   return fread(buffer, sizeof(axis2_char_t), count, fp);   
}

int AXIS2_CALL
axis2_stream_write_file(axis2_stream_t *stream, const axis2_env_t *env, 
                  const void *buffer, size_t count)
{
    int len = 0;
   FILE *fp = NULL;
   
   if(NULL == AXIS2_INTF_TO_IMPL(stream)->fp)
   {
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_FD, AXIS2_FAILURE);
      return -1;
   }
   fp = AXIS2_INTF_TO_IMPL(stream)->fp;
   AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
   if (NULL == buffer)
      return -1;
   len = fwrite(buffer, sizeof(axis2_char_t), count, fp);
   return len;
}


int AXIS2_CALL 
axis2_stream_get_len_file (axis2_stream_t *stream, const axis2_env_t *env)
{
   AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
   return -1;
}

int AXIS2_CALL 
axis2_stream_skip_file (axis2_stream_t *stream, const axis2_env_t *env, int count)
{
   axis2_stream_impl_t *stream_impl = NULL;
   int c = -1;
   int i = count;
   AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
   if(NULL == AXIS2_INTF_TO_IMPL(stream)->fp)
   {
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_FD, AXIS2_FAILURE);
      return -1;
   }
   while(EOF != (c = fgetc(stream_impl->fp)) && i > 0)
   {
      i--;
   }
   return count - i;
}

/********************** End of File Stream Operations *************************/

/************************** Socket Stream Operations **************************/
AXIS2_EXTERN axis2_stream_t * AXIS2_CALL
axis2_stream_create_socket (const axis2_env_t *env, int socket)
{
   axis2_stream_t *def_stream = NULL;
   axis2_stream_impl_t *stream_impl = NULL;
      
   AXIS2_ENV_CHECK(env, NULL);
   def_stream = axis2_stream_create_internal(env);
   if(NULL == def_stream)
   {
      /*
       * We leave the error returned by the 
       * axis2_stream_create_internal intact
       */
      return NULL;
   }
   
   stream_impl = AXIS2_INTF_TO_IMPL(def_stream);
   stream_impl->stream.ops->read = axis2_stream_read_socket;
   stream_impl->stream.ops->write = axis2_stream_write_socket;
   stream_impl->stream.ops->get_len = axis2_stream_get_len_socket;
   stream_impl->stream.ops->skip = axis2_stream_skip_socket;
   
   stream_impl->stream_type = AXIS2_STREAM_SOCKET;
   stream_impl->socket = socket;
   stream_impl->fp = NULL;
   
   return def_stream;
}


int AXIS2_CALL 
axis2_stream_read_socket (axis2_stream_t *stream, const axis2_env_t *env, 
                  void *buffer, size_t count)
{
   int len = 0;
#ifdef AXIS2_TCPMON
    axis2_char_t *temp = NULL;
#endif
    
   AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
   
   if(-1 == AXIS2_INTF_TO_IMPL(stream)->socket)
   {
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_SOCKET, 
                  AXIS2_FAILURE);
      return -1;
   }
    if (NULL == buffer)
   {
        return -1;
   }
    
    len = recv(AXIS2_INTF_TO_IMPL(stream)->socket, buffer, count, 0);
    
#ifdef AXIS2_TCPMON
    if (len > 1)
    {
        temp = (axis2_char_t *)AXIS2_MALLOC(env->allocator, 
                (len + 1) * sizeof(axis2_char_t));
        if (temp)
        {
            memcpy(temp, buffer, len * sizeof(axis2_char_t));
            temp[len] = '\0';
            fprintf (stderr, "%s", temp);
            AXIS2_FREE(env->allocator, temp);
            temp = NULL;
        }
    }
#endif
    return len;
}

int AXIS2_CALL
axis2_stream_write_socket(axis2_stream_t *stream, const axis2_env_t *env, 
                  const void *buffer, size_t count)
{
    int len = 0;
#ifdef AXIS2_TCPMON
    axis2_char_t *temp = NULL;
#endif
         
   AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
   
   if(-1 == AXIS2_INTF_TO_IMPL(stream)->socket)
   {
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_SOCKET, 
                  AXIS2_FAILURE);
      return -1;
   }
   if (NULL == buffer)
      return -1;
   len = send(AXIS2_INTF_TO_IMPL(stream)->socket, buffer, count, 0);
#ifdef AXIS2_TCPMON
    if (len > 0)
    {
        temp = (axis2_char_t *)AXIS2_MALLOC(env->allocator, 
                (len + 1) * sizeof(axis2_char_t));
        if (temp)
        {
            memcpy(temp, buffer, len * sizeof(axis2_char_t));
            temp[len] = '\0';
            fprintf (stderr, "%s", temp);
            AXIS2_FREE(env->allocator, temp);
            temp = NULL;
        }
    }
#endif
   return len;
   
}


int AXIS2_CALL 
axis2_stream_get_len_socket (axis2_stream_t *stream, const axis2_env_t *env)
{
   AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
   return -1;
}

int AXIS2_CALL 
axis2_stream_skip_socket (axis2_stream_t *stream, const axis2_env_t *env, int count)
{
   int len = 0;
   char buffer[2];
   AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);

   
   if(-1 == AXIS2_INTF_TO_IMPL(stream)->socket)
   {
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_SOCKET, 
                  AXIS2_FAILURE);
      return -1;
   }
   while(len < count)
   {
      len += recv(AXIS2_INTF_TO_IMPL(stream)->socket, buffer, 1, 0);
   }
   return len;
}

/********************** End of Socket Stream Operations ***********************/
