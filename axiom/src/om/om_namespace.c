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

#include <axiom_namespace.h>
#include <axis2_string.h>
#include "axiom_namespace_internal.h"
/**************************** Function Prototypes ******************************/

/****************************** axiom_namesapce_impl_struct **************************/

typedef struct axiom_namespace_impl
{
    /*** This should be the first member for macros to work */
    axiom_namespace_t om_namespace;
    /** namespace URI */
    axis2_char_t *uri;
    /** namespace prefix  */
    axis2_char_t *prefix;

    axis2_char_t *key;

}
axiom_namespace_impl_t;

static const axiom_namespace_ops_t axiom_namespace_ops_var = {
    0
};


/**************************************** Macro ****************************************/

#define AXIS2_INTF_TO_IMPL(ns) ((axiom_namespace_impl_t*)ns)


/************************************************************************************/
AXIS2_EXTERN axiom_namespace_t * AXIS2_CALL
axiom_namespace_create(const axis2_env_t *env,
        const axis2_char_t * uri,
        const axis2_char_t * prefix)
{
    axiom_namespace_impl_t *ns = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    /* There should be a uri */
    /*     if (!uri) */
    /*     { */
    /*         AXIS2_ERROR_SET(env->error, */
    /*                 AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE); */
    /*         return NULL; */
    /*     } */
    /*     if (AXIS2_STRCMP(uri, "") == 0) */
    /*     { */
    /*         AXIS2_ERROR_SET(env->error, */
    /*                 AXIS2_ERROR_INVALID_EMPTY_NAMESPACE_URI, AXIS2_FAILURE); */
    /*         return NULL; */
    /*     } */

    if (!uri)
      uri = "";

    ns = (axiom_namespace_impl_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axiom_namespace_impl_t));
    if (!ns)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY , AXIS2_FAILURE);
        return NULL;
    }


    ns->om_namespace.ops = &axiom_namespace_ops_var;
    ns->om_namespace.ref = 0;
    ns->prefix = NULL;
    ns->uri = NULL;
    ns->key = NULL;

    ns->uri = (axis2_char_t *) AXIS2_STRDUP(uri, env);
    if (!ns->uri)
    {
        AXIS2_FREE(env->allocator, ns);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    if (prefix)
    {
        ns->prefix = (axis2_char_t *) AXIS2_STRDUP(prefix, env);
        if (!ns->prefix)
        {
            AXIS2_FREE(env->allocator, ns);
            AXIS2_FREE(env->allocator, ns->uri);
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
    }

    return &(ns->om_namespace) ;
}



AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_namespace_free(axiom_namespace_t *om_namespace,
        const axis2_env_t *env)
{
    axiom_namespace_impl_t *ns_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (--om_namespace->ref > 0)
    {
        return AXIS2_SUCCESS;
    }

    ns_impl = AXIS2_INTF_TO_IMPL(om_namespace);

    if (ns_impl->prefix)
    {
        AXIS2_FREE(env->allocator, ns_impl->prefix);
        ns_impl->prefix = NULL;
    }

    if (ns_impl->uri)
    {
        AXIS2_FREE(env->allocator, ns_impl->uri);
        ns_impl->uri = NULL;
    }

    if (ns_impl->key)
    {
        AXIS2_FREE(env->allocator, ns_impl->key);
        ns_impl->key = NULL;
    }

    AXIS2_FREE(env->allocator, ns_impl);

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axiom_namespace_equals(axiom_namespace_t *om_namespace,
        const axis2_env_t *env,
        axiom_namespace_t *om_namespace1)
{
    axiom_namespace_impl_t *ns1 = NULL;
    axiom_namespace_impl_t *ns2 = NULL;

    int uris_differ = 0;
    int prefixes_differ = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_namespace, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_namespace1, AXIS2_FAILURE);

    ns1 = AXIS2_INTF_TO_IMPL(om_namespace);
    ns2 = AXIS2_INTF_TO_IMPL(om_namespace1);

    if (!ns1 || !ns2)
        return AXIS2_FALSE;

    if (ns1->uri && ns2->uri)
        uris_differ = axis2_strcmp(ns1->uri, ns2->uri);
    else
        uris_differ = (ns1->uri || ns2->uri);

    if (ns1->prefix && ns2->prefix)
        prefixes_differ =
            axis2_strcmp(ns1->prefix, ns2->prefix);
    else
        prefixes_differ = (ns1->prefix || ns2->prefix);

    return (!uris_differ && !prefixes_differ);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_namespace_serialize(axiom_namespace_t *om_namespace,
        const axis2_env_t *env,
        axiom_output_t *om_output)
{
    int status = AXIS2_SUCCESS;
    axiom_namespace_impl_t *ns_impl = NULL;
    if (!om_namespace)
        return AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_output, AXIS2_FAILURE);

    ns_impl = AXIS2_INTF_TO_IMPL(om_namespace);

    if (ns_impl->uri && NULL != ns_impl->prefix &&
            AXIS2_STRCMP(ns_impl->prefix, "") != 0)
    {
        status = axiom_output_write(om_output, env, AXIOM_NAMESPACE,
                2, ns_impl->prefix,
                ns_impl->uri);
    }
    else if (ns_impl->uri)
    {
        status = axiom_output_write(om_output, env, AXIOM_NAMESPACE,
                2, NULL, ns_impl->uri);
    }
    return status;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axiom_namespace_get_uri(axiom_namespace_t *om_namespace,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_namespace)->uri;
}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axiom_namespace_get_prefix(axiom_namespace_t *om_namespace,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(om_namespace)->prefix;
}

AXIS2_EXTERN axiom_namespace_t* AXIS2_CALL
axiom_namespace_clone(axiom_namespace_t *om_namespace,
        const axis2_env_t *env)
{
    axiom_namespace_impl_t *ns_impl = NULL;
    axiom_namespace_t *cloned_ns    = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    ns_impl = AXIS2_INTF_TO_IMPL(om_namespace);

    cloned_ns = axiom_namespace_create(env,
            ns_impl->uri, ns_impl->prefix);
    if (cloned_ns)
    {
        return cloned_ns;
    }
    return NULL;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axiom_namespace_to_string(axiom_namespace_t *om_namespace,
        const axis2_env_t *env)
{
    axiom_namespace_impl_t *ns_impl = NULL;
    axis2_char_t *temp_str = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ns_impl = AXIS2_INTF_TO_IMPL(om_namespace);
    if (ns_impl->key)
    {
        AXIS2_FREE(env->allocator, ns_impl->key);
        ns_impl->key = NULL;
    }
    if ((ns_impl->uri) && (NULL != ns_impl->prefix))
    {
        temp_str = AXIS2_STRACAT(ns_impl->uri, "|", env);
        ns_impl->key = AXIS2_STRACAT(temp_str, ns_impl->prefix, env);
        if (temp_str)
        {
            AXIS2_FREE(env->allocator, temp_str);
            temp_str = NULL;
        }
    }
    else if ((ns_impl->uri) && !(ns_impl->prefix))
    {
        ns_impl->key = AXIS2_STRDUP(ns_impl->uri, env);
        if (NULL == ns_impl->key)
        {
            return NULL;
        }
    }
    return ns_impl->key;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_namespace_set_uri(axiom_namespace_t *ns,
        const axis2_env_t *env,
        const axis2_char_t *uri)
{
    axiom_namespace_impl_t *ns_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, uri, AXIS2_FAILURE);
    ns_impl = AXIS2_INTF_TO_IMPL(ns);

    if (ns_impl->uri)
    {
        AXIS2_FREE(env->allocator, ns_impl->uri);
        ns_impl->uri = NULL;
    }

    ns_impl->uri = AXIS2_STRDUP(uri, env);
    if (!(ns_impl->uri))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;

    }
    return AXIS2_SUCCESS;
}


