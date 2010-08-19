
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

#ifndef RP_SECPOLICY_BUILDER_H
#define RP_SECPOLICY_BUILDER_H

/** @defgroup rp_secpolicy_builder
 * @ingroup rp_secpolicy_builder
 * @{
 */

#include <rp_includes.h>
#include <rp_property.h>
#include <rp_secpolicy.h>
#include <neethi_policy.h>

#ifdef __cplusplus
extern "C"
{
#endif

    AXIS2_EXTERN rp_secpolicy_t *AXIS2_CALL
    rp_secpolicy_builder_build(
        const axutil_env_t * env,
        neethi_policy_t * policy);

#ifdef __cplusplus
}
#endif
#endif
