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

#include <axis2_string.h>
#include <stdlib.h>
#include <string.h>

axis2_status_t check_validity(axis2_string_t *string);

void *
axis2_string_ops_strdup (const void *ptr)
{
    if (ptr)
    {
        return (void *) strdup (ptr);
    }
    else
    {
        return NULL;
    }
}

int
axis2_string_ops_strcmp (const axis2_char_t * s1, const axis2_char_t * s2)
{
    if (s1 && s2)
        return strcmp (s1, s2);
    else
        return -1;
}

axis2_string_t *
axis2_string_create (axis2_allocator_t * allocator, axis2_string_t * string)
{
    if (string && AXIS2_SUCCESS == check_validity(string))
	{
		return string;	
	}
	else if(string)
	{
		string->axis2_string_strdup = axis2_string_ops_strdup;
        string->axis2_string_strcmp = axis2_string_ops_strcmp;	
		return string;
	}
	else if (allocator)
	{
        string =
            (axis2_string_t *) axis2_malloc (allocator,
                                             sizeof (axis2_string_t));
        if (string)
        {
            string->axis2_string_strdup = axis2_string_ops_strdup;
            string->axis2_string_strcmp = axis2_string_ops_strcmp;
            return string;
        }
    }
    return NULL;
}

/** This will check whether the passed string is properly initialized
  * @param string to be validated
  * @return status code
  */
axis2_status_t check_validity(axis2_string_t *string)
{
	if(!string)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	if(!string->axis2_string_strdup) return AXIS2_ERROR_STRUCT_NOT_INITIALIZED;
		
	if(!string->axis2_string_strcmp) return AXIS2_ERROR_STRUCT_NOT_INITIALIZED;
		
	return AXIS2_SUCCESS;
}
