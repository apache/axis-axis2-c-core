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

#ifndef AXIS2_WODEN_OM_UTIL_H
#define AXIS2_WODEN_OM_UTIL_H

/**
 * @file axis2_woden_om_util.h
 * @brief Axis2 Woden OM Util Interface
 *          This is an abstract superclass for all classes representing different
 *          types of XML attribute information items.
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <woden/axis2_woden.h>
#include <axiom_node.h>
#include <woden/wsdl20/axis2_woden_desc.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_woden_om_util Woden OM Util
  * @ingroup axis2_wsdl
  * @{
  */

AXIS2_EXTERN axis2_qname_t * AXIS2_CALL
woden_om_util_get_qname(
        const axis2_env_t *env,
        axiom_node_t *context_el_node,
        axis2_char_t *prefixed_value,
        void *desc);
 
AXIS2_EXTERN axis2_status_t  AXIS2_CALL
woden_om_util_register_unique_prefix(
        const axis2_env_t *env,
        axis2_char_t *prefix,
        axis2_char_t *namespc_uri_str,
        void *desc);
 

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_OM_UTIL_H */

