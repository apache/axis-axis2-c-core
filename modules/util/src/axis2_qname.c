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
#include <axis2_environment.h>
#include <axis2_defines.h>


axis2_status_t AXIS2_CALL  axis2_qname_impl_free (axis2_env_t * environment,
                                      axis2_qname_t * qname);
axis2_bool_t AXIS2_CALL  axis2_qname_impl_equals (axis2_env_t * environment,
                                      axis2_qname_t * qn1,
                                      axis2_qname_t * qn2);
/*AXIS2_DECLARE(axis2_qname_t*) axis2_qname_create (axis2_env_t * environment,
                                   const axis2_char_t * localpart,
                                   const axis2_char_t * namespace_uri,
                                   const axis2_char_t * prefix);
*/

axis2_status_t AXIS2_CALL
axis2_qname_impl_free (axis2_env_t * environment,
                       axis2_qname_t * qname)
{
    if (!qname)
    {
        environment->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }
    if (qname->localpart)
    {
        AXIS2_FREE (environment->allocator, qname->localpart);
    }
    if (qname->namespace_uri)
    {
        AXIS2_FREE (environment->allocator, qname->namespace_uri);
    }
    if (qname->prefix)
    {
        AXIS2_FREE (environment->allocator, qname->prefix);
    }
    if (qname->ops)
    {
        AXIS2_FREE (environment->allocator, qname->ops);
    }
    AXIS2_FREE (environment->allocator, qname);
    return AXIS2_SUCCESS;

}

axis2_bool_t AXIS2_CALL
axis2_qname_impl_equals (axis2_env_t * environment,
                         axis2_qname_t * qn1, axis2_qname_t * qn2)
{

    int uris_differ = 0;
    int localparts_differ = 0;

    if (!qn1 || !qn2)
    {
        environment->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FALSE;
    }


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


AXIS2_DECLARE(axis2_qname_t *)
axis2_qname_create (axis2_env_t * environment,
                    const axis2_char_t * localpart,
                    const axis2_char_t * namespace_uri,
                    const axis2_char_t * prefix)
{

    axis2_qname_t *qn = NULL;
    /* localpart or prefix can't be null */
    if (!localpart)
    {
        environment->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return NULL;
    }

    qn = (axis2_qname_t *) AXIS2_MALLOC (environment->allocator,
                                         sizeof (axis2_qname_t));
    if (!qn)
    {
        environment->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return NULL;
    }
    /* set properties */

    qn->localpart = axis2_strdup (localpart);
    if (!(qn->localpart))
    {
        AXIS2_FREE (environment->allocator, qn);
        environment->error->error_number = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }

    if (!prefix)
    {
        qn->prefix = axis2_strdup ("");
    }
    else
    {
        qn->prefix = axis2_strdup (prefix);
    }
    if (!(qn->prefix))
    {
        AXIS2_FREE (environment->allocator, qn->localpart);
        AXIS2_FREE (environment->allocator, qn);
        environment->error->error_number = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }
    if (!namespace_uri)
    {
        qn->namespace_uri = axis2_strdup ("");
    }
    else
    {
        qn->namespace_uri = axis2_strdup (namespace_uri);
    }
    if (!(qn->namespace_uri))
    {
        AXIS2_FREE (environment->allocator, qn->localpart);
        AXIS2_FREE (environment->allocator, qn->prefix);
        AXIS2_FREE (environment->allocator, qn);
        environment->error->error_number = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }


    qn->ops = NULL;
    qn->ops =
        AXIS2_MALLOC (environment->allocator, sizeof (axis2_qname_ops_t));
    /* operations */
    if (!qn->ops)
    {
        AXIS2_FREE (environment->allocator, qn->localpart);
        if (qn->namespace_uri)
            AXIS2_FREE (environment->allocator, qn->namespace_uri);
        AXIS2_FREE (environment->allocator, qn->prefix);
        AXIS2_FREE (environment->allocator, qn);
        environment->error->error_number = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }

    qn->ops->axis2_qname_ops_free = axis2_qname_impl_free;
    qn->ops->axis2_qname_ops_equals = axis2_qname_impl_equals;
    return qn;
}
