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

#ifndef OXS_SIGN_PART_H
#define OXS_SIGN_PART_H


/**
  * @file oxs_sign_part.h
  * @brief Keeps information relavent for a single node of signing. 
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axiom_node.h>

#ifdef __cplusplus
extern "C"
{
#endif


    typedef struct oxs_sign_part_t oxs_sign_part_t;

    /*Create function*/
    AXIS2_EXTERN oxs_sign_part_t *AXIS2_CALL
    oxs_sign_part_create(const axis2_env_t *env);

    /*Free*/
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    oxs_sign_part_free(oxs_sign_part_t *ctx,
        const axis2_env_t *env);


/**********************Getter functions******************************************/
axis2_char_t *AXIS2_CALL
oxs_sign_part_get_id(
    const oxs_sign_part_t *sign_part,
    const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
oxs_sign_part_get_digest_mtd(
    const oxs_sign_part_t *sign_part,
    const axis2_env_t *env);

axiom_node_t *AXIS2_CALL
oxs_sign_part_get_node(
    const oxs_sign_part_t *sign_part,
    const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
oxs_sign_part_get_transforms(
    const oxs_sign_part_t *sign_part,
    const axis2_env_t *env);

/**********************Setter functions******************************************/
axis2_status_t AXIS2_CALL
oxs_sign_part_set_id(
    oxs_sign_part_t *sign_part,
    const axis2_env_t *env,
    axis2_char_t *id);

axis2_status_t AXIS2_CALL
oxs_sign_part_set_digest_mtd(
    oxs_sign_part_t *sign_part,
    const axis2_env_t *env,
    axis2_char_t *digest_mtd);

axis2_status_t AXIS2_CALL
oxs_sign_part_set_node(
    oxs_sign_part_t *sign_part,
    const axis2_env_t *env,
    axiom_node_t *node);

axis2_status_t AXIS2_CALL
oxs_sign_part_set_transforms(
    oxs_sign_part_t *sign_part,
    const axis2_env_t *env,
    axis2_array_list_t *transforms);



/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_SIGN_PART_H */
