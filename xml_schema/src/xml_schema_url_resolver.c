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

#include <xml_schema/axis2_xml_schema_url_resolver.h>
#include <axis2_url.h>
#include <axis2_file_handler.h>
#include <platforms/axis2_platform_auto_sense.h>

#ifndef S_ISDIR
#   define S_ISDIR(m) ((m & S_IFMT) == S_IFDIR)
#endif


static axis2_bool_t 
is_absolute_url(const axis2_char_t *url);

static axis2_char_t*
get_url(const axis2_env_t *env,
        axis2_char_t *path,
        axis2_char_t *spec);

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_xml_schema_url_resolver_resolve_entity(
        const axis2_env_t *env,
        axis2_char_t *ns,
        axis2_char_t *schema_location,
        axis2_char_t *base_uri)
{
    if(!base_uri)
        return schema_location;
        
    if(!is_absolute_url(schema_location))
    {
        axis2_char_t *path = NULL;
        path = get_url(env, base_uri, schema_location);        
        return path;
    }
    return schema_location;
}        

static axis2_bool_t 
is_absolute_url(const axis2_char_t *url)
{
    if(NULL != url)
    {
        if(strncmp(url, "http://", 7) == 0)
            return AXIS2_TRUE;
    }
    return AXIS2_FALSE;
}

static axis2_char_t*
get_url(const axis2_env_t *env,
        axis2_char_t *path,
        axis2_char_t *spec)
{
    axis2_char_t *modified_path = NULL;
    axis2_char_t *final_path = NULL;
    int length = 0; 
       
    if(NULL != path)
    {
        modified_path = AXIS2_STRDUP(spec, env);
    }
    /** create a path */
    modified_path = AXIS2_REPLACE(env, modified_path, '\\','/');
    
    if(strncmp(modified_path, "/", 1) == 0)
    {
        final_path = AXIS2_STRACAT(path, modified_path, env);
    }
    else
    {
        axis2_char_t *temp_path = NULL;
        temp_path = AXIS2_STRACAT(path, "/",env);
        final_path = AXIS2_STRACAT(temp_path, modified_path, env);
        AXIS2_FREE(env->allocator, modified_path);
    }
        
    if(NULL != final_path)
    {       
        if(strncmp(final_path,"file:///", 8) == 0)
        {
            axis2_char_t *real_path = NULL;
            axis2_char_t *start = NULL;
            start = final_path+8*sizeof(axis2_char_t);
            real_path = AXIS2_STRDUP(start, env);
            AXIS2_FREE(env->allocator, final_path);
            if(axis2_file_handler_access(real_path, AXIS2_F_OK))
                return real_path;
        } 
        else
            if(axis2_file_handler_access(final_path, AXIS2_F_OK))
                return final_path;
    }
    return NULL;
}        
