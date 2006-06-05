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

#ifndef AXIS2_XML_SCHEMA_URL_RESOLVER_H
#define AXIS2_XML_SCHEMA_URL_RESOLVER_H

/**
 * @file axis2_xml_schema_url_resolver.h
 */
#include <axis2_url.h>

#ifdef __cplusplus
extern "C"
{
#endif

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_xml_schema_url_resolver_resolve_entity(
        const axis2_env_t *env,
        axis2_char_t *ns,
        axis2_char_t *schema_location,
        axis2_char_t *base_uri);
        
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_XML_SCHEMA_IMPORT_H */
