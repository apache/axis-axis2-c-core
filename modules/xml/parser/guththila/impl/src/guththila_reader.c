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
#include "guththila_defines.h"


typedef struct guththila_reader_impl_t
{
    guththila_reader_t reader;
    FILE *fp;
    int (*input_read_callback)(char *buffer,int size);
}guththila_reader_impl_t;





GUTHTHILA_DECLARE (guththila_reader_t *)
guththila_reader_create_for_file (guththila_environment_t * environment,
                                  char *filename)
{
    if(!filename)
        return NULL;
    guththila_reader_impl_t *file_reader = 
            (guththila_reader_impl_t *) GUTHTHILA_MALLOC ( environment->allocator,
                                                 sizeof (guththila_reader_impl_t));
    if(!file_reader)
        return NULL;
    
    file_reader->fp  = fopen(filename,"r");
    
    if(!(file_reader->fp ))
    {
        GUTHTHILA_FREE(environment->allocator,file_reader);
        return NULL;
    }                                                            
    
    file_reader->reader.guththila_reader_type = GUTHTHILA_FILE_READER;
    
    return &(file_reader->reader);
}


GUTHTHILA_DECLARE(guththila_reader_t *)
guththila_reader_create_for_memory(
                guththila_environment_t *environment,
                int (*input_read_callback)(char *buffer,int size))
{
    guththila_reader_impl_t *memory_reader = 
        (guththila_reader_impl_t *) GUTHTHILA_MALLOC (environment->allocator,
                                            sizeof (guththila_reader_impl_t));
    if(!memory_reader)
    {
        return NULL;
    }
    
    memory_reader->input_read_callback  = input_read_callback;
    memory_reader->reader.guththila_reader_type = GUTHTHILA_IN_MEMORY_READER;
    
    return &(memory_reader->reader);
}


GUTHTHILA_DECLARE (void)
guththila_reader_free (guththila_environment_t * environment,
                       guththila_reader_t * r)
{

    if (!r)
        return;
    
    if(r->guththila_reader_type == GUTHTHILA_FILE_READER)
    {   
        if(((guththila_reader_impl_t*)r)->fp)
            fclose(((guththila_reader_impl_t*)r)->fp);
    }
    GUTHTHILA_FREE(environment->allocator, (guththila_reader_impl_t*)r); 
   return;    
}

        
GUTHTHILA_DECLARE (int)
guththila_reader_read (guththila_environment_t * environment,
                       guththila_char_t * buffer,
                       int offset,
                       int length,
                       guththila_reader_t * r)
{
    
    if(r->guththila_reader_type == GUTHTHILA_FILE_READER)
    {
       return (int)fread (buffer + offset, 1, length,((guththila_reader_impl_t*)r)->fp);
    }
    else if(r->guththila_reader_type == GUTHTHILA_IN_MEMORY_READER)
        return ((guththila_reader_impl_t*)r)->input_read_callback((buffer + offset), length);
 
    return GUTHTHILA_FAILURE;       
}
