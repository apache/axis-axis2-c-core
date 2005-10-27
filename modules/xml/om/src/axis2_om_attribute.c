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

#include <axis2_om_attribute.h>
#include <string.h>
#include <axis2_defines.h>

axis2_status_t AXIS2_CALL axis2_om_attribute_impl_free (axis2_environment_t *
                                             environment,
                                             axis2_om_attribute_t *
                                             attribute);

axis2_qname_t * AXIS2_CALL axis2_om_attribute_impl_get_qname (axis2_environment_t *
                                                  environment,
                                                  axis2_om_attribute_t *
                                                  attribute);

axis2_status_t AXIS2_CALL axis2_om_attribute_impl_serialize (axis2_environment_t *
                                                  environment,
                                                  axis2_om_attribute_t *
                                                  attribute,
                                                  axis2_om_output_t *
                                                  om_output);


AXIS2_DECLARE(axis2_om_attribute_t*)
 axis2_om_attribute_create (axis2_environment_t * environment,
                           const axis2_char_t * localname,
                           const axis2_char_t * value,
                           axis2_om_namespace_t * ns)
{
    axis2_om_attribute_t *attribute = NULL;
    if (!localname)
    {   /* localname is mandatory */
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return NULL;
    }

    attribute =
        (axis2_om_attribute_t *) axis2_malloc (environment->allocator,
                                               sizeof (axis2_om_attribute_t));
    if (!attribute)
    {
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }
    /**  initialize fields */
    attribute->localname = axis2_strdup (environment->string, localname);
    if (!(attribute->localname))
    {
        axis2_free (environment->allocator, attribute);
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }
    attribute->value = axis2_strdup (environment->string, value);
    if (!(attribute->value))
    {
        axis2_free (environment->allocator, attribute);
        axis2_free (environment->allocator, attribute);
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        return NULL;
    }
    attribute->ns = ns;

    /** operations */

    attribute->ops =
        axis2_malloc (environment->allocator,
                      sizeof (axis2_om_attribute_ops_t));
    if (!(attribute->ops))
    {
        environment->error->errorno = AXIS2_ERROR_NO_MEMORY;
        axis2_free (environment->allocator, attribute->localname);
        axis2_free (environment->allocator, attribute->value);
        axis2_free (environment->allocator, attribute);
        return NULL;
    }
    attribute->ops->axis2_om_attribute_ops_free =
        axis2_om_attribute_impl_free;
    attribute->ops->axis2_om_attribute_ops_get_qname =
        axis2_om_attribute_impl_get_qname;
    attribute->ops->axis2_om_attribute_ops_serialize =
        axis2_om_attribute_impl_serialize;
    return attribute;
}

axis2_status_t AXIS2_CALL
axis2_om_attribute_impl_free (axis2_environment_t * environment,
                              axis2_om_attribute_t * attribute)
{
    if (attribute)
    {
        if (attribute->localname)
        {
            axis2_free (environment->allocator, attribute->localname);
        }
        if (attribute->value)
        {
            axis2_free (environment->allocator, attribute->value);
        }
        axis2_free (environment->allocator, attribute);
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}

axis2_qname_t* AXIS2_CALL
axis2_om_attribute_impl_get_qname (axis2_environment_t * environment,
                                   axis2_om_attribute_t * attribute)
{
    axis2_qname_t *qname = NULL;
    if (!attribute)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return NULL;
    }
    if (attribute->ns)
        qname =
            axis2_qname_create (environment, attribute->localname,
                                attribute->ns->uri, attribute->ns->prefix);
    else
        qname =
            axis2_qname_create (environment, attribute->localname, NULL,
                                NULL);

    return qname;
}


axis2_status_t AXIS2_CALL
axis2_om_attribute_impl_serialize (axis2_environment_t * environment,
                                   axis2_om_attribute_t * attribute,
                                   axis2_om_output_t * om_output)
{
    int status = AXIS2_SUCCESS;
    if (!attribute || !om_output)
    {
        environment->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
        return AXIS2_FAILURE;
    }

    if (attribute->ns && attribute->ns->uri && attribute->ns->prefix)
        status =
            axis2_om_output_write (environment, om_output, AXIS2_OM_ATTRIBUTE,
                                   4, attribute->localname, attribute->value,
                                   attribute->ns->uri, attribute->ns->prefix);
    else if (attribute->ns && attribute->ns->uri)
        status =
            axis2_om_output_write (environment, om_output, AXIS2_OM_ATTRIBUTE,
                                   3, attribute->localname, attribute->value,
                                   attribute->ns->uri);
    else
        status =
            axis2_om_output_write (environment, om_output, AXIS2_OM_ATTRIBUTE,
                                   2, attribute->localname, attribute->value);
    return status;
}
