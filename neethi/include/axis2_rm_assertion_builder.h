
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

#ifndef AXIS2_RM_ASSERTION_BUILDER_H
#define AXIS2_RM_ASSERTION_BUILDER_H

/** @defgroup axis2_rm_assertion_builder
 * @ingroup axis2_rm_assertion_builder
 * @{
 */

#include <neethi_constants.h>
#include <axis2_rm_assertion.h>
#include <neethi_assertion.h>
#include <neethi_includes.h>

#ifdef __cplusplus
extern "C"
{
#endif

   AXIS2_EXTERN neethi_assertion_t *AXIS2_CALL
    axis2_rm_assertion_builder_build(
        const axutil_env_t *env,
        axiom_node_t *rm_assertion_node,
        axiom_element_t *rm_assertion_ele);
 

#ifdef __cplusplus
}
#endif
#endif
