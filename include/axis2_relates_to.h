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

#ifndef AXIS2_RELATES_TO_H
#define AXIS2_RELATES_TO_H


/**
 * @defgroup axis2_relates_to relates to
 * @ingroup axis2_addr
 * relates to encapsulates data that indicate how a message relates to 
 * another message. 
 * The related message is identified by a URI that corresponds to the 
 * related message's message ID. The type of the relationship is also captured
 * by relates to. Basically relates to handles the following WS-Addressing 
 * header <wsa:RelatesTo RelationshipType="..."?>xs:anyURI</wsa:RelatesTo>
 * @{
 */

/**
 * @file axis2_relates_to.h
 */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_const.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_relates_to */
    typedef struct axis2_relates_to axis2_relates_to_t;
    /** Type name for struct axis2_relates_to_ops */
    typedef struct axis2_relates_to_ops axis2_relates_to_ops_t;


    /**
     * relates to ops struct.
     */
    struct axis2_relates_to_ops
    {
        /**
         * Gets value. The value field represents the URI that corresponds to the 
         * related message's message ID
         * @param relates_to pointer to relates to struct
         * @param env pointer to environment struct
         * @return value string
         */
        const axis2_char_t *(AXIS2_CALL *
                get_value)(
                    const axis2_relates_to_t *relates_to,
                    const axis2_env_t *env);

        /**
         * Sets value. The value field represents the URI that corresponds to the 
         * related message's message ID
         * @param relates_to pointer to relates to struct
         * @param env pointer to environment struct
         * @param value value string 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_value)(
                    struct axis2_relates_to *relates_to,
                    const axis2_env_t *env,
                    const axis2_char_t *value);

        /**
         * Gets relationship type. 
         * @param relates_to pointer to relates to struct
         * @param env pointer to environment struct
         * @return relationship type string 
         */
        const axis2_char_t *(AXIS2_CALL *
                get_relationship_type)(
                    const axis2_relates_to_t *relates_to,
                    const axis2_env_t *env);

        /**
         * Sets relationship type. 
         * @param relates_to pointer to relates to struct
         * @param env pointer to environment struct
         * @param relationship_type relationship type string
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_relationship_type)(
                    struct axis2_relates_to *relates_to,
                    const axis2_env_t *env,
                    const axis2_char_t *relationship_type);

        /**
         * Frees relates to struct.                     
         * @param relates_to pointer to relates to struct
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    struct axis2_relates_to *relates_to,
                    const axis2_env_t *env);
    };

    /**
     * relates to struct.
     */
    struct axis2_relates_to
    {
        /** relates to ops */
        axis2_relates_to_ops_t *ops;
    };


    /**
     * creates relates to struct.
     * @param env pointer to environment struct
     * @param value value string 
     * @param relationship_type relationship type string 
     */
    AXIS2_EXTERN axis2_relates_to_t *AXIS2_CALL
    axis2_relates_to_create(
        const axis2_env_t *env,
        const axis2_char_t *value,
        const axis2_char_t *relationship_type);

/** Gets value.
    @sa axis2_relates_to_ops#get_value */
#define AXIS2_RELATES_TO_GET_VALUE(relates_to, env) \
      ((relates_to)->ops->get_value(relates_to, env))

/** Sets value.
    @sa axis2_relates_to_ops#set_value */
#define AXIS2_RELATES_TO_SET_VALUE(relates_to, env, vlaue) \
      ((relates_to)->ops->set_value(relates_to, env, vlaue))

/** Gets relationship type.
    @sa axis2_relates_to_ops#get_relationship */
#define AXIS2_RELATES_TO_GET_RELATIONSHIP_TYPE(relates_to, env) \
      ((relates_to)->ops->get_relationship_type(relates_to, env))

/** Sets relationship type.
    @sa axis2_relates_to_ops#set_relationship */
#define AXIS2_RELATES_TO_SET_RELATIONSHIP_TYPE(relates_to, env, relationship_type) \
      ((relates_to)->ops->set_relationship_type(relates_to, env, relationship_type))

/** Frees relates to struct.
    @sa axis2_relates_to_ops#free */
#define AXIS2_RELATES_TO_FREE(relates_to, env) \
      ((relates_to)->ops->free(relates_to, env))

/** @} */

#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_RELATES_TO_H */
