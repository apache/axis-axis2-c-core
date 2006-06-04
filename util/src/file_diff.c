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
 
#include <axis2_file_diff.h>
#include <axis2_file_handler.h>
#include <axis2_stream.h>
#include <axis2_utils_defines.h>

void *expected_file, *actual_file; /* File pointers */
axis2_stream_t *expected_file_stream, *actual_file_stream; /* Streams corresponding to above files*/
axis2_stream_t *output_stream;
/* private function header */
axis2_status_t axis2_file_diff_clean(const axis2_env_t *env);

axis2_status_t AXIS2_CALL axis2_file_diff(const axis2_env_t *env
      , axis2_char_t* expected_file_name, axis2_char_t* actual_file_name)
{
    int ch1, ch2;
    int j = 0, k = 0;
      axis2_char_t *buffer1, *buffer2;
      int flag1 = 0, flag2 = 0;
   expected_file = axis2_file_handler_open(expected_file_name,"rt");
    
   actual_file = axis2_file_handler_open(actual_file_name,"rt"); 
   
   if( expected_file == NULL || actual_file == NULL ) {
      /*printf("Unable to open one of datafile %s, %s\n", expected_file_name
         , actual_file_name );*/
      axis2_file_diff_clean(env);
      return AXIS2_ERROR_COULD_NOT_OPEN_FILE;
   }
   
    expected_file_stream = axis2_stream_create_file(env, expected_file);
    actual_file_stream = axis2_stream_create_file(env, actual_file); 
    output_stream = axis2_stream_create_file(env, stdout);
    
    if( expected_file_stream == NULL || actual_file_stream == NULL ) {
        /*printf("Unable to open one of datafile %s, %s\n", expected_file_name
            , actual_file_name );*/
        axis2_file_diff_clean(env);
        return AXIS2_FAILURE;
    }   
   
      buffer1 = (axis2_char_t*) AXIS2_MALLOC(env->allocator, 8096 * sizeof(axis2_char_t));
      buffer2 = (axis2_char_t*) AXIS2_MALLOC(env->allocator, 8096 * sizeof(axis2_char_t));
      ch1 = AXIS2_STREAM_GET_CHAR (expected_file_stream, env);
      ch2 = AXIS2_STREAM_GET_CHAR (actual_file_stream, env);
   
      
      while(1)
      {
        
      if(ch1 == ' ' || ch1 == '\n' )
        {
            flag1 = 1;    
        }
        if(ch2 == ' ' || ch2 == '\n')
        {
            flag2 = 1;
        }
        if(ch1 != ' ' && ch1 != '\n' && ch2 != ' ' && ch2 != '\n')
        {
            flag1 = 0;
            flag2 = 0;
        }
            
        if(flag1 == 1 && flag2 == 0)
        {
            AXIS2_STREAM_UNGET_CHAR(actual_file_stream, env, ch2);
            j++;
            buffer1[j] = ch1;
        }
        if(flag1 == 0 && flag2 == 1)
        {
            AXIS2_STREAM_UNGET_CHAR(expected_file_stream, env, ch1);
            k++;
            buffer2[k] = ch2;
        }
        if(flag1 == 0 && flag2 == 0)
        {
            if(ch1 != ch2)
            {
            AXIS2_STREAM_WRITE(output_stream, env
               , "\n****************************** TEST FAILED *********" \
               "**************************\n", 80);
                
                *buffer1 = '\n';
                *buffer2 = '\n';
            AXIS2_STREAM_WRITE(output_stream, env
               , "\n---------------------- Expected file read upto: ----" \
               "--------------------------\n", 80);
                
                 /*printf("%s\n", buffer1); */
            AXIS2_STREAM_WRITE(output_stream, env, buffer1, j);
            AXIS2_STREAM_WRITE(output_stream, env
               , "\n---------------------- Actual file read upto: ------" \
               "--------------------------\n", 80);
                
                /* printf("%s\n", buffer2); */
            AXIS2_STREAM_WRITE(output_stream, env, buffer2, k);
            AXIS2_STREAM_WRITE(output_stream, env, "\n", 1);

                axis2_file_diff_clean(env);
            }
            j++;k++;
            buffer1[j] = ch1;
            buffer2[k] = ch2;
        }
        if(flag1 == 1 && flag2 == 1)
        {
            flag1 = 0;
            flag2 = 0;
            j++; k++;
            buffer1[j] = ch1;
            buffer2[k] = ch2;
        }

        
        if(ch1 == expected_file_stream->axis2_eof || ch2 == actual_file_stream->axis2_eof)
            break;
        ch1 = AXIS2_STREAM_GET_CHAR(expected_file_stream, env);
      ch2 = AXIS2_STREAM_GET_CHAR(actual_file_stream, env);
      }
      
      AXIS2_STREAM_WRITE(output_stream, env
      , "\n************************* TEST PASSED **************************" \
      "**************\n", 80);
   
      return AXIS2_SUCCESS;
}

axis2_status_t axis2_file_diff_clean (const axis2_env_t *env)
{
    if(NULL != expected_file_stream)
    {
        AXIS2_STREAM_FREE(expected_file_stream, env);
        expected_file_stream = NULL;
    }
    if(NULL != actual_file_stream)
    {
        AXIS2_STREAM_FREE(actual_file_stream, env);
        actual_file_stream = NULL;
    }
    if(NULL != output_stream)
    {
        AXIS2_STREAM_FREE(output_stream, env);
        output_stream = NULL;
    }

   if(expected_file != NULL )
       axis2_file_handler_close(expected_file);
      if(actual_file != NULL )
       axis2_file_handler_close(actual_file);
   return AXIS2_SUCCESS;
}
