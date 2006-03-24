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
 
 /** <wsa:RelatesTo RelationshipType="..."?>xs:anyURI</wsa:RelatesTo> */

#include <axis2_svc_name.h>
#include <axis2_string.h>

typedef struct axis2_svc_name_impl
{
    axis2_svc_name_t svc_name;    
    /** service qname */
    axis2_qname_t *qname;    
    /** service endpoint name */
    axis2_char_t *endpoint_name;
} axis2_svc_name_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(svc_name) ((axis2_svc_name_impl_t *)svc_name)


axis2_qname_t* AXIS2_CALL 
axis2_svc_name_get_qname(struct axis2_svc_name *svc_name, 
                         axis2_env_t **env);
                         
axis2_status_t AXIS2_CALL 
axis2_svc_name_set_qname(struct axis2_svc_name *svc_name, 
                         axis2_env_t **env, 
                         axis2_qname_t *qname);
                         
axis2_char_t* AXIS2_CALL 
axis2_svc_name_get_endpoint_name(struct axis2_svc_name *svc_name, 
                                 axis2_env_t **env);
                                 
axis2_status_t AXIS2_CALL 
axis2_svc_name_set_endpoint_name(struct axis2_svc_name *svc_name, 
                                 axis2_env_t **env, 
                                 axis2_char_t *endpoint_name);
                                 
axis2_status_t AXIS2_CALL 
axis2_svc_name_free(struct axis2_svc_name *svc_name, 
                    axis2_env_t **env);
                    

axis2_svc_name_t* AXIS2_CALL 
axis2_svc_name_create(axis2_env_t **env, 
                      axis2_qname_t *qname, 
                      axis2_char_t *endpoint_name) 
{
    axis2_svc_name_impl_t *svc_name_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    svc_name_impl = 
            AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_svc_name_impl_t) );
    if (!svc_name_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    svc_name_impl->svc_name.ops = NULL;
    svc_name_impl->qname = NULL;
    svc_name_impl->endpoint_name = NULL;
    
    if (qname)
    {
        svc_name_impl->qname = AXIS2_QNAME_CLONE(qname, env);
        if (!(svc_name_impl->qname))
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            axis2_svc_name_free(&(svc_name_impl->svc_name), env);
            return NULL;        
        }
    }
    
    if (endpoint_name)
    {
        svc_name_impl->endpoint_name = AXIS2_STRDUP(endpoint_name, env);
        if (!(svc_name_impl->endpoint_name))
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            axis2_svc_name_free(&(svc_name_impl->svc_name), env);
            return NULL;        
        }
    }

    /* initialize ops */
    svc_name_impl->svc_name.ops  = 
            AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_svc_name_ops_t) );
    if (!svc_name_impl->svc_name.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_svc_name_free(&(svc_name_impl->svc_name), env);
        return NULL;        
    }

    svc_name_impl->svc_name.ops->get_qname = axis2_svc_name_get_qname;
    svc_name_impl->svc_name.ops->set_qname = axis2_svc_name_set_qname;
    svc_name_impl->svc_name.ops->get_endpoint_name = axis2_svc_name_get_endpoint_name;
    svc_name_impl->svc_name.ops->set_endpoint_name = axis2_svc_name_set_endpoint_name;
    svc_name_impl->svc_name.ops->free = axis2_svc_name_free;

    return &(svc_name_impl->svc_name);
}

/**
 * Method getName
 *
 * @return
 */
axis2_qname_t* AXIS2_CALL 
axis2_svc_name_get_qname(struct axis2_svc_name *svc_name, 
                         axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(svc_name)->qname;
}

/**
 * Method setName
 *
 * @param qname
 */
axis2_status_t AXIS2_CALL 
axis2_svc_name_set_qname(struct axis2_svc_name *svc_name, 
                         axis2_env_t **env, 
                         axis2_qname_t *qname) 
{
    axis2_svc_name_impl_t *svc_name_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    svc_name_impl = AXIS2_INTF_TO_IMPL(svc_name);
    
    if (svc_name_impl->qname)
    {
        AXIS2_QNAME_FREE(svc_name_impl->qname, env);
        svc_name_impl->qname = NULL;
    }
    
    if (qname)
    {
        svc_name_impl->qname = AXIS2_QNAME_CLONE(qname, env);
        if (!(svc_name_impl->qname))
            return AXIS2_FAILURE;
    }
    
    return AXIS2_SUCCESS;
}

/**
 * Method getEndpointName
 *
 * @return
 */
axis2_char_t* AXIS2_CALL 
axis2_svc_name_get_endpoint_name(struct axis2_svc_name *svc_name, 
                                 axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(svc_name)->endpoint_name;
}

/**
 * Method setEndpointName
 *
 * @param endpoint_name
 */
axis2_status_t AXIS2_CALL 
axis2_svc_name_set_endpoint_name(struct axis2_svc_name *svc_name, 
                                 axis2_env_t **env, 
                                 axis2_char_t *endpoint_name) 
{
    axis2_svc_name_impl_t *svc_name_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    svc_name_impl = AXIS2_INTF_TO_IMPL(svc_name);
    
    if (svc_name_impl->endpoint_name)
    {
        AXIS2_FREE((*env)->allocator, svc_name_impl->endpoint_name);
        svc_name_impl->endpoint_name = NULL;
    }
    
    if (endpoint_name)
    {
        svc_name_impl->endpoint_name = AXIS2_STRDUP(endpoint_name, env);
        if (!(svc_name_impl->endpoint_name))
            return AXIS2_FAILURE;
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_svc_name_free (struct axis2_svc_name *svc_name, 
                     axis2_env_t **env)
{
    axis2_svc_name_impl_t *svc_name_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    svc_name_impl = AXIS2_INTF_TO_IMPL(svc_name);
    
    if (svc_name_impl->svc_name.ops)
    {
        AXIS2_FREE((*env)->allocator, svc_name_impl->svc_name.ops);
        svc_name_impl->svc_name.ops = NULL;
    }
    
    if (svc_name_impl->qname)
    {
        AXIS2_QNAME_FREE(svc_name_impl->qname, env);
        svc_name_impl->qname = NULL;
    }    
    
    if (svc_name_impl->endpoint_name)
    {
        AXIS2_FREE((*env)->allocator, svc_name_impl->endpoint_name);
        svc_name_impl->endpoint_name = NULL;
    }    

    AXIS2_FREE((*env)->allocator, svc_name_impl);
    svc_name_impl = NULL;
    
    return AXIS2_SUCCESS;
}
