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

#include <guththila_string.h>
#include <stdlib.h>
#include <string.h>
#include "guththila_unicode.h"
#include "guththila_defines.h"

void* guththila_string_ops_strdup(const void* ptr)
{
    if (ptr)
        return (void*)strdup(ptr);
    else
        return NULL;
}

int guththila_string_ops_strcmp(const guththila_char_t *s1, const guththila_char_t *s2)
{
    if (s1 && s2)
        return strcmp(s1, s2);
    else
        return -1;
}



int guththila_string_ops_strlen(const guththila_char_t *s)
{
    if(s)
    {
    
#ifdef __UNICODE__FUNCTIONS__
       
       return guththila_strlen_unicode(x);
#else 
       return strlen(s);
#endif        

    }
    else
        return -1;
}
guththila_char_t* guththila_string_ops_strndup(const guththila_char_t *s1,size_t len)
{
    if(s1)
    {
#ifdef __UNICODE__FUNCTIONS__    
        
        return guththila_strdup_unicode(x, y);
 
#else 
        register size_t n;
	    register guththila_char_t *dst;

	    n = guththila_string_ops_strlen(s1);
	    if (len < n)
    	        n = len;
	    dst = (guththila_char_t *) malloc(n+1);
	    if (dst)
	    {
	        memcpy(dst, s1, n);
		    dst[n] = '\0';
	    }
	    return dst;
#endif
    }
    else
        return NULL;
}



guththila_string_t *guththila_string_create(guththila_allocator_t *allocator,
                guththila_string_t *string)
{
    if(string)
        return string;

    if (!allocator)
        return NULL;
        
    else
    {
        string = (guththila_string_t*)guththila_malloc(allocator, sizeof(guththila_string_t));
        if(string)
        {
            string->guththila_string_strdup = guththila_string_ops_strdup;
            string->guththila_string_strcmp = guththila_string_ops_strcmp;
            string->guththila_string_strndup = guththila_string_ops_strndup;
            string->guththila_string_strlen = guththila_string_ops_strlen;
            return string;
        }
     }
    return NULL;
}
