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

#ifndef AXIS2_OM_NAMESPACE_H
#define AXIS2_OM_NAMESPACE_H

/**
 * @file axis2_om_namespace.h
 * @brief defines axis2_om_namespace data structure, used for constructing the om tree and its 
 * manipulation functions
 * represents an xml namespace
 */

#include <axis2.h>
#include <axis2_om_output.h>


typedef struct axis2_om_namespace_t {
    char *uri;
    char *prefix;
} axis2_om_namespace_t;

/**
 *	creates an axis2_om_namespace
 *  @param axis2_om_namesapce_t pointer to axis2_om_namespace_t struct
 */

axis2_om_namespace_t *axis2_om_namespace_create(const char *uri,  const char *prefix);

/**
 * free an om_namespcae
 *
 */
void axis2_om_namespace_free(axis2_om_namespace_t * om_namespace);
/**
 *	compares two namepsaces
 * @param ns1 ,ns2 namespace to be compared
 * @return true if ns1 equals ns2 , false otherwise	
 */

int axis2_om_namespace_equals(axis2_om_namespace_t *om_namespace1, axis2_om_namespace_t *om_namespace2);

int axis2_om_namespace_serialize(axis2_om_namespace_t *om_namespace, axis2_om_output_t* om_output);

#endif	// AXIS2_OM_NAMESPACE
