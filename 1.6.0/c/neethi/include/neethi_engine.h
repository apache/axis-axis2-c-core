
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

#ifndef NEETHI_ENGINE_H
#define NEETHI_ENGINE_H

/*neethis_engine.c contains all the useful functions
 * for dealing with a neethi_policy object
 */



/**
  * @file neethi_engine.h
  * @contains neethi_policy creation logic.
  */

#include <axis2_defines.h>
#include <axutil_env.h>
#include <neethi_includes.h>
#include <neethi_operator.h>
#include <neethi_policy.h>
#include <neethi_all.h>
#include <neethi_exactlyone.h>
#include <neethi_reference.h>
#include <neethi_registry.h>
#include <neethi_assertion.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * Given an axiom model this function will return 
     * a neethi_policy object.
     * @param env pointer to environment struct
     * @param node to an axiom_node
     * @param node to an axiom_element
     * @return pointer to a neethi_policy_t struct
     */    
    
    AXIS2_EXTERN neethi_policy_t *AXIS2_CALL
    neethi_engine_get_policy(
        const axutil_env_t * env,
        axiom_node_t * node,
        axiom_element_t * element);

     /**
     * Given a neethi_policy object this will return the
     * normalized policy object.     
     * @param env pointer to environment struct
     * @param deep to specify whether assertion level normalization
       needed.  
     * @param neethi_policy_t to the policy which is not
       normalized. 
     * @return pointer to a normalized neethi_policy_t struct
     */   

    /*This function will return a new neethi_policy struct.
      So it is callers responsibility to free the neethi_policy
      which is passed as an argument. */

    AXIS2_EXTERN neethi_policy_t *AXIS2_CALL
    neethi_engine_get_normalize(
        const axutil_env_t * env,
        axis2_bool_t deep,
        neethi_policy_t * neethi_policy);

    /**
     * Given a neethi_policy object this will return the
     * normalized policy object.     
     * @param env pointer to environment struct
     * @param deep to specify whether assertion level normalization
       needed.  
     * @param neethi_policy_t to the policy which is not
       normalized. 
     * @param registry neethi_registry_t struct which contains
       policy objects. 
     * @return pointer to a normalized neethi_policy_t struct
     */   

    /*This function will return a new neethi_policy struct.
      So it is callers responsibility to free the neethi_policy
      which is passed as an argument. */


    AXIS2_EXTERN neethi_policy_t *AXIS2_CALL
    neethi_engine_normalize(
        const axutil_env_t * env,
        neethi_policy_t * neethi_policy,
        neethi_registry_t * registry,
        axis2_bool_t deep);

    /*Givnen to normalized policy objects this function will
      retun the merged policy object.
    * @param env pointer to environment struct
    * @param neethi_policy1 pointer neethi_policy_t struct as an
    * input for merge.
    * @param neethi_policy2 pointer neethi_policy_t struct as an
    * input for merge.
    * @return pointer to a merged policy of both inputs.*/

    /*The input for this function should be two normalized policies
      otherwise the output may be wrong.*/

    AXIS2_EXTERN neethi_policy_t *AXIS2_CALL
    neethi_engine_merge(
        const axutil_env_t * env,
        neethi_policy_t * neethi_policy1,
        neethi_policy_t * neethi_policy2);

    /*Given a policy object this function will give the 
     * corresponding axiom model for that policy object.
     * @param policy pointer to the neethi_policy_t struct.
     * @param env pointer to environment struct
     */

    AXIS2_EXTERN axiom_node_t *AXIS2_CALL
    neethi_engine_serialize(
        neethi_policy_t * policy,
        const axutil_env_t * env);

    /** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* NEETHI_ENGINE_H */
