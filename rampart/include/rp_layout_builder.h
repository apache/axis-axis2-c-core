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

#ifndef RP_LAYOUT_BUILDER_H
#define RP_LAYOUT_BUILDER_H

/** @defgroup rp_layout_builder
 * @ingroup rp_layout_builder
 * @{
 */

#include <rp_includes.h>
#include <rp_property.h>
#include <rp_layout.h>
#include <rp_qname_matcher.h>

#ifdef __cplusplus
extern "C"
{
#endif

    AXIS2_EXTERN rp_layout_t *AXIS2_CALL
    rp_layout_builder_build(
                const axis2_env_t *env,
                axiom_node_t *layout);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_layout_builder_set_value(
            axiom_node_t *node,
            axiom_element_t *element,
            axis2_char_t *value,
            rp_layout_t *layout,
            const axis2_env_t *env);


#ifdef __cplusplus
}
#endif
#endif  
