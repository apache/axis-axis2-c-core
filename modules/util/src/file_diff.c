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

void *expected_file, *actual_file; /* File pointers */

/* private function header */
axis2_status_t axis2_file_diff_clean(axis2_env_t *env);

axis2_status_t axis2_file_diff(axis2_env_t *env
		, axis2_char_t* expected_file_name, axis2_char_t* actual_file_name)
{
    int ch1, ch2;
    int j = 0, k = 0;
   	axis2_char_t *buffer1, *buffer2;
   	int flag1 = 0, flag2 = 0;
	expected_file = AXIS2_STREAM_FILE_OPEN (env->stream
		, expected_file_name, "rt");
	 
	actual_file = AXIS2_STREAM_FILE_OPEN (env->stream, actual_file_name
		, "rt");
	
	if( expected_file == NULL || actual_file == NULL ) {
		/*printf("Unable to open one of datafile %s, %s\n", expected_file_name
			, actual_file_name );*/
		axis2_file_diff_clean(env);
		return AXIS2_ERROR_COULD_NOT_OPEN_FILE;
	}
	
   	
   
   	buffer1 = (axis2_char_t*) AXIS2_MALLOC(env->allocator, 8096 * sizeof(axis2_char_t));
   	buffer2 = (axis2_char_t*) AXIS2_MALLOC(env->allocator, 8096 * sizeof(axis2_char_t));
   	ch1 = AXIS2_STREAM_FILE_GET_CHAR (env->stream, expected_file);
   	ch2 = AXIS2_STREAM_FILE_GET_CHAR (env->stream, actual_file);
	
   	
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
            AXIS2_STREAM_FILE_UNGET_CHAR(env->stream, ch2, actual_file);
            j++;
            buffer1[j] = ch1;
        }
        if(flag1 == 0 && flag2 == 1)
        {
            AXIS2_STREAM_FILE_UNGET_CHAR(env->stream, ch1, expected_file);
            k++;
            buffer2[k] = ch2;
        }
        if(flag1 == 0 && flag2 == 0)
        {
            if(ch1 != ch2)
            {
				AXIS2_STREAM_WRITE(env->stream
					, "\n****************************** TEST FAILED *********" \
					"**************************\n", 80);
                
                *buffer1 = '\n';
                *buffer2 = '\n';
				AXIS2_STREAM_WRITE(env->stream
					, "\n---------------------- Expected file read upto: ----" \
					"--------------------------\n", 80);
                
                 /*printf("%s\n", buffer1); */
				AXIS2_STREAM_WRITE(env->stream, buffer1, j);
				AXIS2_STREAM_WRITE(env->stream
					, "\n---------------------- Actual file read upto: ------" \
					"--------------------------\n", 80);
                
                /* printf("%s\n", buffer2); */
				AXIS2_STREAM_WRITE(env->stream, buffer2, k);
				AXIS2_STREAM_WRITE(env->stream, "\n", 1);

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

        
        if(ch1 == env->stream->axis2_eof || ch2 == env->stream->axis2_eof)
            break;
        ch1 = AXIS2_STREAM_FILE_GET_CHAR(env->stream, expected_file );
		ch2 = AXIS2_STREAM_FILE_GET_CHAR(env->stream, actual_file);
   	}
   	
   	AXIS2_STREAM_WRITE(env->stream
		, "\n************************* TEST PASSED **************************" \
		"**************\n", 80);
	
   	return AXIS2_SUCCESS;
}

axis2_status_t axis2_file_diff_clean (axis2_env_t *env)
{
	if(expected_file != NULL )
    	AXIS2_STREAM_FILE_CLOSE(env->stream, expected_file);
   	if(actual_file != NULL )
    	AXIS2_STREAM_FILE_CLOSE(env->stream, actual_file);
	return AXIS2_SUCCESS;
}
