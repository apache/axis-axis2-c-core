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


axis2_om_attribute_t *axis2_om_attribute_create(const char *localname,
						  const char *value,axis2_om_namespace_t *ns)
{
    axis2_om_attribute_t *attr = (axis2_om_attribute_t *) malloc(
										sizeof(axis2_om_attribute_t));
    if (!attr)
    {
		return NULL;
    }

    attr->localname = strdup(localname);
    attr->value = strdup(value);
    attr->ns = ns;
    return attr;
}

void axis2_om_attribute_free(axis2_om_attribute_t * attr)
{
    if (attr)
    {
		if(attr->localname)
		{
			free(attr->localname);
		}
		if(attr->value)
		{
			free(attr->value);
		}
		if(attr->ns)
		{
			axis2_om_namespace_free(attr->ns);
		}
		free(attr);
	}

}

axis2_qname_t *axis2_om_attribute_get_qname(axis2_om_attribute_t * attr)
{
    axis2_qname_t *qname = NULL;
    if (!attr)
	{
		return NULL;
	}
		if (attr->ns)
		{
	    	qname = axis2_qname_create(attr->localname, attr->ns->uri,attr->ns->prefix);
		}
		else
		{
		    qname = axis2_qname_create(attr->localname, NULL, NULL);
		}
		return qname;
}


int axis2_om_attribute_serialize(axis2_om_attribute_t *attribute, axis2_om_output_t* om_output)
{
    int status = AXIS2_SUCCESS;
    // TODO : handle null pointer errors
    if (attribute->ns && attribute->ns->uri && attribute->ns->prefix)
        status = axis2_om_output_write (om_output, AXIS2_OM_ATTRIBUTE, 4,
                               attribute->localname, attribute->value, attribute->ns->uri,
                               attribute->ns->prefix);
    else if (attribute->ns && attribute->ns->uri)
        status = axis2_om_output_write (om_output, AXIS2_OM_ATTRIBUTE, 3,
                               attribute->localname, attribute->value, attribute->ns->uri);
    else 
        status = axis2_om_output_write (om_output, AXIS2_OM_ATTRIBUTE, 2,
                               attribute->localname, attribute->value);
    return status;
}

