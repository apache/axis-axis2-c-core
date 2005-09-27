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





#include <axis2c_om_attribute.h>
#include <string.h>


axis2c_om_attribute_t *axis2c_create_om_attribute(const char *localname,
					   const char *value,
					   axis2c_om_namespace_t * ns)
{
    axis2c_om_attribute_t *attr =
	(axis2c_om_attribute_t *) malloc(sizeof(axis2c_om_attribute_t));
    if (!attr)
    {

	return NULL;
    }

    attr->localname = strdup(localname);
    attr->value = strdup(value);
    attr->ns = ns;
    return attr;
}

void axis2c_om_attribute_free(axis2c_om_attribute_t * attr)
{
    if (attr)
    {
	free(attr->localname);
	free(attr->value);
	/* should namespace pointer be  ...
	 */

    }

}

axis2c_qname_t *axis2c_om_attribute_get_qname(axis2c_om_attribute_t * attr)
{
    axis2c_qname_t *qn = NULL;
    if (attr)
    {
	if (attr->ns)
	{
	    qn = axis2c_create_qname(attr->localname, attr->ns->uri,
				     attr->ns->prefix);
	}
	else
	{
	    qn = axis2c_create_qname(attr->localname, NULL, NULL);
	}
	return qn;
    }
    return NULL;

}

