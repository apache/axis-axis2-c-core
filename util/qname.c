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

#include <axis2_qname.h>
#include <axis2_env.h>
#include <axis2_utils.h>
#include <axis2_utils_defines.h>
#include <string.h>

/********************************** Function prototypes *****************/

axis2_status_t AXIS2_CALL 
axis2_qname_free ( axis2_qname_t * qname,
                   axis2_env_t **env);


axis2_bool_t AXIS2_CALL 
axis2_qname_equals (axis2_qname_t * qname,
                    axis2_env_t **env,
                    axis2_qname_t * qname1);

axis2_qname_t* AXIS2_CALL
axis2_qname_clone(axis2_qname_t *qname,
                  axis2_env_t **env);
                                      
axis2_char_t* AXIS2_CALL
axis2_qname_get_uri(axis2_qname_t *qname,
                    axis2_env_t **env);
                                        
axis2_char_t* AXIS2_CALL 
axis2_qname_get_prefix(axis2_qname_t *qname,
                        axis2_env_t **env);

axis2_char_t* AXIS2_CALL 
axis2_qname_get_localpart(axis2_qname_t *qname,
                          axis2_env_t **env);
                          
axis2_char_t* AXIS2_CALL
axis2_qname_to_string(axis2_qname_t *qname,
                      axis2_env_t **env);                          
                                            
/*************************************** qname struct *********************/

typedef struct axis2_qname_impl_t
{
    /* this should be first member */
    axis2_qname_t qname;
    /** localpart of qname is mandatory */

    axis2_char_t *localpart;

    /** namespace uri is optional */
    axis2_char_t *namespace_uri;

    /**  prefix mandatory */
    axis2_char_t *prefix;
    
    axis2_char_t *qname_string;
    

}axis2_qname_impl_t;


/************************* Macro ****************************************/

#define AXIS2_INTF_TO_IMPL(qname) ((axis2_qname_impl_t*)qname)

/************************************************************************/

AXIS2_DECLARE(axis2_qname_t *)
axis2_qname_create (axis2_env_t **env,
                    const axis2_char_t * localpart,
                    const axis2_char_t * namespace_uri,
                    const axis2_char_t * prefix)
{

    axis2_qname_impl_t *qn = NULL;
    AXIS2_ENV_CHECK(env, NULL);
   
    /* localpart can't be null */
    if (!localpart)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;
    }

    qn = (axis2_qname_impl_t *) AXIS2_MALLOC ((*env)->allocator,
                                             sizeof (axis2_qname_impl_t));
    if (!qn)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    /* set properties */
    qn->localpart = NULL;
    qn->qname_string = NULL;
    qn->prefix = NULL;
    qn->namespace_uri = NULL;
    
    
    qn->localpart = (axis2_char_t *)AXIS2_STRDUP (localpart, env);
    if (!(qn->localpart))
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        AXIS2_FREE ((*env)->allocator, qn);
        return NULL;
    }
    /** if prefix is null it is set to "" */
    if (!prefix)
    {
        qn->prefix = (axis2_char_t*)AXIS2_STRDUP("", env);
    }
    else
    {
        qn->prefix =(axis2_char_t*)AXIS2_STRDUP (prefix, env);
    }
    if (!(qn->prefix))
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        AXIS2_FREE ((*env)->allocator, qn->localpart);
        AXIS2_FREE ((*env)->allocator, qn);
        return NULL;
    }
    /** if uri is null it is set to ""*/
    if (!namespace_uri)
    {
        qn->namespace_uri = (axis2_char_t*)AXIS2_STRDUP ("", env);
    }
    else
    {
        qn->namespace_uri = (axis2_char_t*)AXIS2_STRDUP (namespace_uri, env);
    }
    if (!(qn->namespace_uri))
    {
        AXIS2_FREE ((*env)->allocator, qn->localpart);
        AXIS2_FREE ((*env)->allocator, qn->prefix);
        AXIS2_FREE ((*env)->allocator, qn);
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;
    }

   
    qn->qname.ops = NULL;
    qn->qname.ops = (axis2_qname_ops_t*)AXIS2_MALLOC ((*env)->allocator,
                                                      sizeof (axis2_qname_ops_t));
 
    if (!qn->qname.ops)
    {
        AXIS2_FREE ((*env)->allocator, qn->localpart);
        if (qn->namespace_uri)
            AXIS2_FREE ((*env)->allocator, qn->namespace_uri);
        AXIS2_FREE ((*env)->allocator, qn->prefix);
        AXIS2_FREE ((*env)->allocator, qn);
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;
    }

    qn->qname.ops->free = axis2_qname_free;
    qn->qname.ops->equals = axis2_qname_equals;
    qn->qname.ops->clone = axis2_qname_clone;
    qn->qname.ops->get_localpart = axis2_qname_get_localpart;
    qn->qname.ops->get_prefix = axis2_qname_get_prefix;
    qn->qname.ops->get_uri = axis2_qname_get_uri;
    qn->qname.ops->to_string = axis2_qname_to_string;
   
    return &(qn->qname);
}


axis2_status_t AXIS2_CALL 
axis2_qname_free ( axis2_qname_t * qname,
                   axis2_env_t **env)
{   
    axis2_qname_impl_t *qname_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    qname_impl = AXIS2_INTF_TO_IMPL(qname);
    
    if (qname_impl->localpart)
    {
        AXIS2_FREE ((*env)->allocator, qname_impl->localpart);
        qname_impl->localpart = NULL;
    }
    if (qname_impl->namespace_uri)
    {
        AXIS2_FREE ((*env)->allocator, qname_impl->namespace_uri);
        qname_impl->namespace_uri = NULL;
    }
    if (qname_impl->prefix)
    {
        AXIS2_FREE ((*env)->allocator, qname_impl->prefix);
        qname_impl->prefix = NULL;
    }
    if(qname_impl->qname_string)
    {
        AXIS2_FREE((*env)->allocator, qname_impl->qname_string);
        qname_impl->qname_string = NULL;        
    }
    if (qname->ops)
    {
        AXIS2_FREE ((*env)->allocator, qname->ops);
    }
    AXIS2_FREE ((*env)->allocator, qname_impl);
    return AXIS2_SUCCESS;

}


axis2_bool_t AXIS2_CALL
axis2_qname_equals (axis2_qname_t *qname,
                    axis2_env_t **env, 
                    axis2_qname_t * qname1)
{
    axis2_qname_impl_t *qn1 = NULL;
    axis2_qname_impl_t *qn2 = NULL;
    int uris_differ = 0;
    int localparts_differ = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    
    if (!qname1)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return AXIS2_FALSE;
    }

    qn1 = AXIS2_INTF_TO_IMPL(qname);
    qn2 = AXIS2_INTF_TO_IMPL(qname1);

    if (qn1->localpart && qn2->localpart)
    {
        localparts_differ =
            axis2_strcmp (qn1->localpart,
                          qn2->localpart);
    }
    else
    {
        localparts_differ = ((qn1->localpart) || (qn2->localpart));
    }
    if (qn1->namespace_uri && qn2->namespace_uri)
    {
        uris_differ =
            axis2_strcmp (qn1->namespace_uri,
                          qn2->namespace_uri);
    }
    else
    {
        uris_differ = ((qn1->namespace_uri) || (qn2->namespace_uri));
    }
    return (!uris_differ && !localparts_differ) ? AXIS2_TRUE : AXIS2_FALSE;

}


axis2_qname_t* AXIS2_CALL
axis2_qname_clone(axis2_qname_t *qname,
                  axis2_env_t **env)
{
    axis2_qname_impl_t *qname_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    qname_impl = AXIS2_INTF_TO_IMPL(qname);
    return axis2_qname_create(env, qname_impl->localpart,
                              qname_impl->namespace_uri,
                              qname_impl->prefix);
}


                  
axis2_char_t* AXIS2_CALL
axis2_qname_get_uri(axis2_qname_t *qname,
                    axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env,NULL);
    return AXIS2_INTF_TO_IMPL(qname)->namespace_uri;
}                    
                                        
axis2_char_t* AXIS2_CALL 
axis2_qname_get_prefix(axis2_qname_t *qname,
                        axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env,NULL);
    return AXIS2_INTF_TO_IMPL(qname)->prefix;
}


axis2_char_t* AXIS2_CALL 
axis2_qname_get_localpart(axis2_qname_t *qname,
                          axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env,NULL);
    return AXIS2_INTF_TO_IMPL(qname)->localpart;
} 

axis2_char_t* AXIS2_CALL
axis2_qname_to_string(axis2_qname_t *qname,
                      axis2_env_t **env)
{
    axis2_qname_impl_t *qname_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    qname_impl = AXIS2_INTF_TO_IMPL(qname);
    if(qname_impl->qname_string)
    {
        return qname_impl->qname_string;
    }
    if(!(qname_impl->namespace_uri) || AXIS2_STRCMP(qname_impl->namespace_uri,"") == 0)
    {
        /** prefix url both null */
        qname_impl->qname_string = AXIS2_STRDUP(qname_impl->localpart, env);
    }
    else if(!(qname_impl->prefix) || AXIS2_STRCMP(qname_impl->prefix, "") == 0)
    {
        /** prefix null */
        
        axis2_char_t *temp_string1 = NULL;
        temp_string1 = AXIS2_STRACAT(qname_impl->localpart, "|", env);
        qname_impl->qname_string = AXIS2_STRACAT(temp_string1, qname_impl->namespace_uri, env);
        if(NULL != temp_string1)
        {
            AXIS2_FREE((*env)->allocator, temp_string1);
            temp_string1 = NULL;
        }            
    }
    else
    {
        /** both uri and prefix exist */
        axis2_char_t *temp_string1 = NULL;
        axis2_char_t *temp_string2 = NULL;
        axis2_char_t *temp_string3 = NULL;
        
        temp_string1 = AXIS2_STRACAT(qname_impl->localpart, "|", env);
        temp_string2 = AXIS2_STRACAT(temp_string1, qname_impl->namespace_uri, env);
        temp_string3 = AXIS2_STRACAT(temp_string2, "|", env);
        qname_impl->qname_string = AXIS2_STRACAT(temp_string3, qname_impl->prefix, env);
        
        if(NULL != temp_string1)
        {
            AXIS2_FREE((*env)->allocator, temp_string1);
            temp_string1 = NULL;
        }
        if(NULL != temp_string2)
        {
            AXIS2_FREE((*env)->allocator, temp_string2);
            temp_string2 = NULL;
        }
        if(NULL != temp_string3)
        {
            AXIS2_FREE((*env)->allocator, temp_string3);
            temp_string3 = NULL;
        }
    }     
    return qname_impl->qname_string;        
}

AXIS2_DECLARE(axis2_qname_t*) 
axis2_qname_create_from_string(axis2_env_t **env,
                               const axis2_char_t *qstring)
{
    axis2_char_t *localpart = NULL;
    axis2_char_t *namespace_uri = NULL;
    axis2_char_t *prefix = NULL;
    axis2_char_t *index = NULL;
    axis2_char_t *next = NULL;
    axis2_char_t *temp_string = NULL;
    axis2_qname_t *qn = NULL;
    if(!qstring || AXIS2_STRCMP(qstring, "") == 0)
        return NULL;
    
    temp_string = AXIS2_STRDUP(qstring, env);
    
    index = strchr(temp_string,'|');
    if(index != NULL)
    {
        
        next = index+1;
        temp_string[index - temp_string] = '\0';
        
        localpart = temp_string;
        
        index = strchr(next, '|');
        if(NULL != index)
        {
            prefix = index+1;
            next[index - next] = '\0';
            namespace_uri = next;
            
            qn = axis2_qname_create(env, localpart, namespace_uri, prefix);                   
        }
        else
        {
            /** only uri and localpart is available */
            qn = axis2_qname_create(env, localpart, next, NULL);
        }    
    }
    else
    {
        /** only localpart is there in this qname */
        qn = axis2_qname_create(env, temp_string, NULL, NULL);
    }
    if(NULL != temp_string)
    {
        AXIS2_FREE((*env)->allocator, temp_string);
        temp_string = NULL;
    }
    return qn;
}

