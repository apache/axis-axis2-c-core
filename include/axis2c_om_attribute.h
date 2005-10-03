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

#ifndef AXIS2C_OM_ATTRIBUTE_H
#define AXIS2C_OM_ATTRIBUTE_H

/**
 * @file axis2c_om_attribute.h
 * @brief om attribute strcut represents an xml attribute
 */

#include <axis2c_qname.h>
#include <axis2c_node.h>
#include <axis2c_om_namespace.h>

/**
 * localname   
 * value
 * namespace 
 */

typedef struct axis2c_om_attribute_t{
	char *localname;
	char *value;
	axis2c_om_namespace_t *ns;
}axis2c_om_attribute_t;

/**
 * creates an om_attribute structure 
 * @param localname
 * @param value 
 * @param axis2c_om_namespace namespace 
 * @return The attribute struct just created
  */

axis2c_om_attribute_t *axis2c_om_attribute_create(const char *localname,const char *value, axis2c_om_namespace_t *ns);

/**
 * creates and returns a unique qname struct for this attribute 
 * @param attribute pointer to an attribute struct
 * @return axis2c_qname_t struct
 */

axis2c_qname_t *axis2c_om_attribute_get_qname(axis2c_om_attribute_t *attribute);

/**
 * free a om attribute struct
 * @param attr pointer to the struct to be freed
 * @return 
 */

void axis2c_om_attribute_free(axis2c_om_attribute_t *attr);

#endif   // _AXISC_ATTRIBUTE_H_
