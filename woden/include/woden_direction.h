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

#ifndef WODEN_DIRECTION_H
#define WODEN_DIRECTION_H

/**
 * @file woden_direction.h
 * @brief Axis2 Direction Interface
 * This class defines the values of the {direction} property of
 * Interface Message Reference and Interface Fault Reference. This property 
 * indicates whether a message is coming "in" to the service or going "out" 
 * from the service.
 * <p>
 * The property is represented in XML by the message or fault 
 * reference element's tag name:
 * <ul>
 * <li>&lt;input&gt; and &lt;infault&gt; have the direction "in" - 
 *     represented by the constant IN
 * <li>&lt;output&gt; and &lt;outfault&gt; have the direction "out" - 
 *     represented by the constant OUT
 * </ul>
 * This class uses the typesafe enum pattern. Applications should use the
 * public static final constants defined in this class to specify or to 
 * evaluate direction.
 * <p>
 * Examples:
 * <pre>
 *      WODEN_INTERFACE_MSG_REF_SET_DIRECTION(intf_msg_ref, env, WODEN_IN);
 *      if(0 == axis2_strcmp(WODEN_IN,WODEN_INTERFACE_GET_DIRECTION(
 *          intf_msg_ref, env)) ...
 * </pre>
 * 
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_direction woden_direction_t;
typedef struct woden_direction_ops woden_direction_ops_t;
struct woden_ext_element;

/** @defgroup woden_direction Direction
  * @ingroup woden
  * @{
  */

struct woden_direction_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *direction,
            const axutil_env_t *env);
    
    axis2_char_t *(AXIS2_CALL *
    to_string) (
            void *direction,
            const axutil_env_t *env);


};

struct woden_direction
{
    woden_direction_ops_t *ops;
    
};

AXIS2_EXTERN woden_direction_t *AXIS2_CALL
woden_direction_get_direction_in(
        const axutil_env_t *env);

AXIS2_EXTERN woden_direction_t *AXIS2_CALL
woden_direction_get_direction_out(
        const axutil_env_t *env);

#define WODEN_DIRECTION_FREE(direction, env) \
      (((woden_direction_t *) direction)->ops->\
         free (direction, env))

#define WODEN_DIRECTION_TO_STRING(direction, env) \
      (((woden_direction_t *) direction)->ops->\
         to_string (direction, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_DIRECTION_H */
