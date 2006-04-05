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

#include <axis2_om_namespace.h>
#include <axis2_string.h>

/**************************** Function Prototypes ******************************/

axis2_status_t AXIS2_CALL 
axis2_om_namespace_free (axis2_om_namespace_t *om_namespace,
                              axis2_env_t **env);

axis2_bool_t AXIS2_CALL 
axis2_om_namespace_equals (axis2_om_namespace_t *om_namespace,
                                axis2_env_t **env,
                                axis2_om_namespace_t *om_namespace1);

axis2_status_t AXIS2_CALL 
axis2_om_namespace_serialize (axis2_om_namespace_t *om_namespace,
                              axis2_env_t **env,
                              axis2_om_output_t *om_output);
                              
axis2_char_t* AXIS2_CALL
axis2_om_namespace_get_uri(axis2_om_namespace_t *om_namespace,
                            axis2_env_t **env);

axis2_char_t* AXIS2_CALL 
axis2_om_namespace_get_prefix(axis2_om_namespace_t *om_namespace,
                              axis2_env_t **env);

axis2_om_namespace_t* AXIS2_CALL
axis2_om_namespace_clone(axis2_om_namespace_t *om_namespace,
                         axis2_env_t **env);

/****************************** axis2_om_namesapce_impl_struct **************************/

typedef struct axis2_om_namespace_impl
{
    /*** This should be the first member for macros to work */
    axis2_om_namespace_t om_namespace;
    /** namespace URI */
    axis2_char_t *uri;
    /** namespace prefix  */
    axis2_char_t *prefix;

}axis2_om_namespace_impl_t;


/**************************************** Macro ****************************************/

#define AXIS2_INTF_TO_IMPL(ns) ((axis2_om_namespace_impl_t*)ns)


/************************************************************************************/
AXIS2_DECLARE(axis2_om_namespace_t *)
axis2_om_namespace_create (axis2_env_t **env,
                           const axis2_char_t * uri,
                           const axis2_char_t * prefix)
{
    axis2_om_namespace_impl_t *ns = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    /* There should be a uri */
    if(!uri)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }

    ns = (axis2_om_namespace_impl_t *) AXIS2_MALLOC ((*env)->allocator,
                                                    sizeof(axis2_om_namespace_impl_t));
    if (!ns)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY , AXIS2_FAILURE);
        return NULL;
    }
   
    
    ns->om_namespace.ops = NULL;
    ns->prefix = NULL;
    ns->uri = NULL;
     
    ns->uri = (axis2_char_t *) AXIS2_STRDUP(uri,env);
    if (!ns->uri)
    {
        AXIS2_FREE ((*env)->allocator, ns);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    if (prefix)
    {
        ns->prefix = (axis2_char_t *) AXIS2_STRDUP(prefix,env);
        if (!ns->prefix)
        {
            AXIS2_FREE ((*env)->allocator, ns);
            AXIS2_FREE ((*env)->allocator, ns->uri);
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
    }

    /* ops */
    ns->om_namespace.ops = (axis2_om_namespace_ops_t *) AXIS2_MALLOC (
                            (*env)->allocator, sizeof(axis2_om_namespace_ops_t));

    if (!ns->om_namespace.ops)
    {
        AXIS2_FREE ((*env)->allocator, ns);
        AXIS2_FREE ((*env)->allocator, ns->uri);
        AXIS2_FREE ((*env)->allocator, ns->prefix);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    ns->om_namespace.ops->free = axis2_om_namespace_free;
    ns->om_namespace.ops->equals = axis2_om_namespace_equals;
    ns->om_namespace.ops->serialize = axis2_om_namespace_serialize;
    ns->om_namespace.ops->get_uri = axis2_om_namespace_get_uri;
    ns->om_namespace.ops->get_prefix = axis2_om_namespace_get_prefix;
    ns->om_namespace.ops->clone = axis2_om_namespace_clone;
    return &(ns->om_namespace) ;
}



axis2_status_t AXIS2_CALL
axis2_om_namespace_free (axis2_om_namespace_t *om_namespace,
                         axis2_env_t **env)
{
    axis2_om_namespace_impl_t *ns_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    ns_impl = AXIS2_INTF_TO_IMPL(om_namespace);
    
    if (NULL != ns_impl->prefix)
    {
        AXIS2_FREE ((*env)->allocator, ns_impl->prefix);
        ns_impl->prefix = NULL;
    }

    if (NULL != ns_impl->uri)
    {
        AXIS2_FREE ((*env)->allocator, ns_impl->uri);
        ns_impl->uri = NULL;
    }

    if (NULL != om_namespace->ops)
    {
        AXIS2_FREE ((*env)->allocator, om_namespace->ops);
        om_namespace->ops = NULL;
    }

    AXIS2_FREE ((*env)->allocator, ns_impl);

    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_om_namespace_equals (axis2_om_namespace_t *om_namespace,
                                axis2_env_t **env,
                                axis2_om_namespace_t *om_namespace1)
{
    axis2_om_namespace_impl_t *ns1 = NULL;
    axis2_om_namespace_impl_t *ns2 = NULL;
    
    int uris_differ = 0;
    int prefixes_differ = 0;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, om_namespace, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, om_namespace1, AXIS2_FAILURE);
    
    ns1 = AXIS2_INTF_TO_IMPL(om_namespace);
    ns2 = AXIS2_INTF_TO_IMPL(om_namespace1);
  
    if (!ns1 || !ns2)
        return AXIS2_FALSE;

    if (ns1->uri && ns2->uri)
        uris_differ = axis2_strcmp ( ns1->uri, ns2->uri);
    else
        uris_differ = (ns1->uri || ns2->uri);

    if (ns1->prefix && ns2->prefix)
        prefixes_differ =
            axis2_strcmp ( ns1->prefix, ns2->prefix);
    else
        prefixes_differ = (ns1->prefix || ns2->prefix);

    return (!uris_differ && !prefixes_differ);
}

axis2_status_t AXIS2_CALL
axis2_om_namespace_serialize (axis2_om_namespace_t *om_namespace,
                                   axis2_env_t **env,
                                   axis2_om_output_t *om_output)
{ 
    int status = AXIS2_SUCCESS;
    axis2_om_namespace_impl_t *ns_impl = NULL;
    if(!om_namespace)
        return AXIS2_FAILURE;
        
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, om_output, AXIS2_FAILURE);

    ns_impl = AXIS2_INTF_TO_IMPL(om_namespace);
    
    if (ns_impl->uri && ns_impl->prefix)
    {
            status = axis2_om_output_write ( om_output, env, AXIS2_OM_NAMESPACE,
                                         2, ns_impl->prefix,
                                         ns_impl->uri);
    }
    else if(ns_impl->uri)
    {
        status = axis2_om_output_write(om_output, env, AXIS2_OM_NAMESPACE,
                                        2, NULL, ns_impl->uri);
    
    }
                                             
    return status;
}

axis2_char_t* AXIS2_CALL
axis2_om_namespace_get_uri(axis2_om_namespace_t *om_namespace,
                           axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_namespace)->uri;
}


axis2_char_t* AXIS2_CALL 
axis2_om_namespace_get_prefix(axis2_om_namespace_t *om_namespace,
                              axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_namespace)->prefix;
}

axis2_om_namespace_t* AXIS2_CALL
axis2_om_namespace_clone(axis2_om_namespace_t *om_namespace,
                         axis2_env_t **env)
{
    axis2_om_namespace_impl_t *ns_impl = NULL;
    axis2_om_namespace_t *cloned_ns    = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    ns_impl = AXIS2_INTF_TO_IMPL(om_namespace);
    
    cloned_ns = axis2_om_namespace_create(env,
                      ns_impl->uri, ns_impl->prefix);
    if(NULL != cloned_ns )
    {
        return cloned_ns;
    }
    return NULL;
}
