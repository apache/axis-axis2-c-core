
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

#ifndef RP_ISSUED_TOKEN_BUILDER_H
#define RP_ISSUED_TOKEN_BUILDER_H

/** @defgroup trust10
 * @ingroup trust10
 * @{
 */

#include <rp_includes.h>
#include <rp_issued_token.h>
#include <neethi_operator.h>
#include <neethi_policy.h>
#include <neethi_exactlyone.h>
#include <neethi_all.h>
#include <neethi_engine.h>

#ifdef __cplusplus
extern "C"
{
#endif
	
	AXIS2_EXTERN neethi_assertion_t * AXIS2_CALL
	rp_issued_token_builder_build(const axutil_env_t *env,
			axiom_node_t *node, axiom_element_t *element);
	
	AXIS2_EXTERN axis2_status_t AXIS2_CALL rp_issued_token_builder_process_alternatives(
			const axutil_env_t *env, neethi_all_t *all,
			rp_issued_token_t *issued_token);
	
#ifdef __cplusplus
}
#endif
#endif
