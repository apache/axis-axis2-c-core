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

#ifndef AXIS2C_OM_NAMESPACE_H
#define AXIS2C_OM_NAMESPACE_H

typedef struct axis2c_om_namespace_t {
    char *uri;
    char *prefix;
} axis2c_om_namespace_t;


axis2c_om_namespace_t *axis2c_create_om_namespace(const char *uri,  const char *prefix);
void axis2c_free_om_namespace(axis2c_om_namespace_t * ns);
int axis2c_om_namespace_equals(axis2c_om_namespace_t * ns1, axis2c_om_namespace_t * ns2);

#endif	// AXIS2C_OM_NAMESPACE
