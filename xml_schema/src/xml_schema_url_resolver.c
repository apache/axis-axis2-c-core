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

#include <xml_schema_url_resolver.h>
#include <axis2_url.h>
#include <axis2_file_handler.h>
#include <platforms/axis2_platform_auto_sense.h>

/*
static axis2_bool_t
is_absolute_url(const axis2_char_t *url);
*/

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
get_file_url(const axis2_env_t *env,
        axis2_char_t *path);

AXIS2_EXTERN xml_schema_input_source_t* AXIS2_CALL
xml_schema_url_resolver_resolve_entity(
    const axis2_env_t *env,
    axis2_char_t *ns,
    axis2_char_t *schema_location,
    axis2_char_t *base_uri)
{
    if (base_uri && NULL != schema_location)
    {
        axis2_uri_t *uri1 = NULL;
        axis2_uri_t *uri2 = NULL;
        axis2_char_t *abs_path = NULL;
        axis2_char_t *ref = NULL;
        /*
        if(axis2_file_handler_access(base_uri, AXIS2_F_OK))
        {
             abs_path = AXIS2_GETCWD(base_uri);
            uri1 = axis2_uri_parse_string(env, get_file_url(env, bas));

        }
        */
        abs_path = get_file_url(env, base_uri);

        uri1 = axis2_uri_parse_string(env, abs_path);

        if (uri1)
        {
            uri2 = axis2_uri_parse_relative(env, uri1, schema_location);
            if (uri2)
            {
                ref = axis2_uri_to_string(uri2, env, 1);
                /* axis2_uri_free(uri2, env);  */
                return xml_schema_input_source_create_with_system_id(env, ref);
            }
        }
        return NULL;
    }
    return xml_schema_input_source_create_with_system_id(env, schema_location);
}
/*
static axis2_bool_t
is_absolute_url(const axis2_char_t *url)
{
    if( url)
    {
        if(strncmp(url, "http://", 7) == 0)
            return AXIS2_TRUE;
    }
    return AXIS2_FALSE;
}
*/
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
get_file_url(const axis2_env_t *env,
        axis2_char_t *path)
{
    axis2_char_t *modified_path = NULL;
    axis2_char_t *final_path = NULL;
    if (!path)
        return NULL;
    modified_path = axis2_strdup(env, path);
    axis2_replace(env, modified_path, '\\', '/');
    if (strncmp(modified_path, "/", 1) == 0)
    {
        final_path = axis2_stracat(env, "file://", modified_path);
    }
    else
    {
        final_path = axis2_stracat(env, "file:///", modified_path);
    }
    if (modified_path)
    {
        AXIS2_FREE(env->allocator, modified_path);
    }
    return final_path;
}
