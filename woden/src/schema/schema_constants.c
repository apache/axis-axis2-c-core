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
 
#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_uri.h>
#include <axis2_qname.h>
#include <woden/axis2_woden.h>
#include <woden/axis2_woden_schema_constants.h>

axis2_bool_t AXIS2_CALL
woden_schema_constants_compare_schema(
        axis2_qname_t *schema, 
        const axis2_env_t *env)
{
    axis2_qname_t *q1999 = NULL;
    axis2_qname_t *q2000 = NULL;
    axis2_qname_t *q2001 = NULL;
    axis2_bool_t ret = AXIS2_FALSE;

    q1999 = axis2_qname_create_from_string(env, WODEN_Q_ELEM_XSD_1999);
    q2000 = axis2_qname_create_from_string(env, WODEN_Q_ELEM_XSD_2000);
    q2001 = axis2_qname_create_from_string(env, WODEN_Q_ELEM_XSD_2001);

    if(AXIS2_TRUE == AXIS2_QNAME_EQUALS(schema, env, q1999) ||
            AXIS2_TRUE == AXIS2_QNAME_EQUALS(schema, env, q2000) ||
            AXIS2_TRUE == AXIS2_QNAME_EQUALS(schema, env, q2001))
    {
        ret = AXIS2_TRUE;
    }
    if(q1999)
        AXIS2_QNAME_FREE(q1999, env);
    if(q2000)
        AXIS2_QNAME_FREE(q2000, env);
    if(q2001)
        AXIS2_QNAME_FREE(q2001, env);
    
    return ret;
}

axis2_bool_t AXIS2_CALL
woden_schema_constants_compare_include(
        axis2_qname_t *include, 
        const axis2_env_t *env)
{
    axis2_qname_t *q1999 = NULL;
    axis2_qname_t *q2000 = NULL;
    axis2_qname_t *q2001 = NULL;
    axis2_bool_t ret = AXIS2_FALSE;

    q1999 = axis2_qname_create_from_string(env, WODEN_Q_ELEM_INCLUDE_XSD_1999);
    q2000 = axis2_qname_create_from_string(env, WODEN_Q_ELEM_INCLUDE_XSD_2000);
    q2001 = axis2_qname_create_from_string(env, WODEN_Q_ELEM_INCLUDE_XSD_2001);

    if(AXIS2_TRUE == AXIS2_QNAME_EQUALS(include, env, q1999) ||
            AXIS2_TRUE == AXIS2_QNAME_EQUALS(include, env, q2000) ||
            AXIS2_TRUE == AXIS2_QNAME_EQUALS(include, env, q2001))
    {
        ret = AXIS2_TRUE;
    }
    if(q1999)
        AXIS2_QNAME_FREE(q1999, env);
    if(q2000)
        AXIS2_QNAME_FREE(q2000, env);
    if(q2001)
        AXIS2_QNAME_FREE(q2001, env);
    
    return ret;
}

axis2_bool_t AXIS2_CALL
woden_schema_constants_compare_import(
        axis2_qname_t *import, 
        const axis2_env_t *env)
{
    axis2_qname_t *q1999 = NULL;
    axis2_qname_t *q2000 = NULL;
    axis2_qname_t *q2001 = NULL;
    axis2_bool_t ret = AXIS2_FALSE;

    q1999 = axis2_qname_create_from_string(env, WODEN_Q_ELEM_IMPORT_XSD_1999);
    q2000 = axis2_qname_create_from_string(env, WODEN_Q_ELEM_IMPORT_XSD_2000);
    q2001 = axis2_qname_create_from_string(env, WODEN_Q_ELEM_IMPORT_XSD_2001);

    if(AXIS2_TRUE == AXIS2_QNAME_EQUALS(import, env, q1999) ||
            AXIS2_TRUE == AXIS2_QNAME_EQUALS(import, env, q2000) ||
            AXIS2_TRUE == AXIS2_QNAME_EQUALS(import, env, q2001))
    {
        ret = AXIS2_TRUE;
    }
    if(q1999)
        AXIS2_QNAME_FREE(q1999, env);
    if(q2000)
        AXIS2_QNAME_FREE(q2000, env);
    if(q2001)
        AXIS2_QNAME_FREE(q2001, env);
    
    return ret;
}

axis2_bool_t AXIS2_CALL
woden_schema_constants_compare_redefine(
        axis2_qname_t *redefine, 
        const axis2_env_t *env)
{
    axis2_qname_t *q1999 = NULL;
    axis2_qname_t *q2000 = NULL;
    axis2_qname_t *q2001 = NULL;
    axis2_bool_t ret = AXIS2_FALSE;

    q1999 = axis2_qname_create_from_string(env, WODEN_Q_ELEM_REDEFINE_XSD_1999);
    q2000 = axis2_qname_create_from_string(env, WODEN_Q_ELEM_REDEFINE_XSD_2000);
    q2001 = axis2_qname_create_from_string(env, WODEN_Q_ELEM_REDEFINE_XSD_2001);

    if(AXIS2_TRUE == AXIS2_QNAME_EQUALS(redefine, env, q1999) ||
            AXIS2_TRUE == AXIS2_QNAME_EQUALS(redefine, env, q2000) ||
            AXIS2_TRUE == AXIS2_QNAME_EQUALS(redefine, env, q2001))
    {
        ret = AXIS2_TRUE;
    }
    if(q1999)
        AXIS2_QNAME_FREE(q1999, env);
    if(q2000)
        AXIS2_QNAME_FREE(q2000, env);
    if(q2001)
        AXIS2_QNAME_FREE(q2001, env);
    
    return ret;
}

