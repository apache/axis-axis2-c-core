/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <w2c_xslt_utils.h>
#include <w2c_string.h>
#include <platforms/axis2_platform_auto_sense.h>
#include <string.h>
#include <stdlib.h>


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
w2c_file_writer_create_classfile_name (
    const axis2_env_t *env,
    axis2_char_t *root_location,
    axis2_char_t *package_name,
    axis2_char_t *filename,
    axis2_char_t *extension )
{
    axis2_char_t *full_path = NULL;
    axis2_char_t *cp = NULL;
    axis2_bool_t end = AXIS2_FALSE;
    int error = 0;
    int length = 0;
        
    if ( ! root_location )
    {
        full_path= axis2_strdup ( ".", env);
    }
    else
    {
        full_path = axis2_strdup ( root_location,env );
        for ( end = AXIS2_FALSE, cp = full_path; 1; cp ++)
        {
            if ( *cp == '\0' )
            {
                end = AXIS2_TRUE;
            }
            else if ( *cp != AXIS2_PATH_SEP_CHAR)
            {
                continue;
            }
            *cp = '\0';
            /* TODO: this should be placed in platform */
            error = AXIS2_MKDIR ( full_path, 0755 ); 
            if ( end )
            {
                if ( cp != full_path && *(cp-1) == AXIS2_PATH_SEP_CHAR )
                {
                    *(cp-1) = '\0';
                }
                break;
            }
            else
            {
                *cp = AXIS2_PATH_SEP_CHAR;
            }
        }
    }
    /* to seperate root location and package path */
    full_path = w2c_string_add_string ( full_path, AXIS2_PATH_SEP_STR, env );

    /* this is related to adding folder hiearchy corrosponding to the package */
    if ( package_name && *package_name != '\0')
    {
        length = axis2_strlen ( full_path );
        full_path = w2c_string_add_string ( full_path, package_name, env);

        /* the loop wil start from the package name */
        for ( end = AXIS2_FALSE, cp = full_path + length; 1; cp ++)
        {
            if ( *cp == '\0' )
            {
                end = AXIS2_TRUE;
            }
            else if ( *cp !='.' )
            {
                continue;
            }
            *cp = '\0';
            /* TODO:this should be placed in platform */
            error = AXIS2_MKDIR ( full_path, 0755 ); 
            if ( end )
            {
                break;
            }
            else
            {
                *cp = AXIS2_PATH_SEP_CHAR;
            }
        }
        full_path = w2c_string_add_string ( full_path, AXIS2_PATH_SEP_STR, env );
    }
    full_path = w2c_string_add_string ( full_path, filename, env);
    full_path = w2c_string_add_string ( full_path, extension, env );/* extension has the .*/

    return full_path;
}

