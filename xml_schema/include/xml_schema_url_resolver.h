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

#ifndef XML_SCHEMA_URL_RESOLVER_H
#define XML_SCHEMA_URL_RESOLVER_H

/**
 * @file xml_schema_url_resolver.h
 */
#include <axis2_url.h>
#include <xml_schema_input_source.h>

#ifdef __cplusplus
extern "C"
{
#endif

AXIS2_EXTERN xml_schema_input_source_t* AXIS2_CALL
xml_schema_url_resolver_resolve_entity(
        const axutil_env_t *env,
        axis2_char_t *ns,
        axis2_char_t *schema_location,
        axis2_char_t *base_uri);
        

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
get_file_url(const axutil_env_t *env,
        axis2_char_t *path);        
        
/** @} */
#ifdef __cplusplus
}
#endif
#endif /* XML_SCHEMA_IMPORT_H */
