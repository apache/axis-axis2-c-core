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

#ifndef AXIS2_WODEN_DIRECTION_H
#define AXIS2_WODEN_DIRECTION_H

/**
 * @file axis2_woden_direction.h
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
 *      AXIS2_WODEN_INTERFACE_MSG_REF_SET_DIRECTION(intf_msg_ref, env, WODEN_IN);
 *      if(0 == AXIS2_STRCMP(WODEN_IN,AXIS2_WODEN_INTERFACE_GET_DIRECTION(
 *          intf_msg_ref, env)) ...
 * </pre>
 * 
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <woden/axis2_woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_direction axis2_woden_direction_t;
typedef struct axis2_woden_direction_ops axis2_woden_direction_ops_t;
struct axis2_woden_ext_element;

/** @defgroup axis2_woden_direction Direction
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_direction_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *direction,
            const axis2_env_t *env);
    
    void *(AXIS2_CALL *
    get_direction_in) (
            void *direction,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_direction_out) (
            void *direction,
            const axis2_env_t *env);

    axis2_char_t *(AXIS2_CALL *
    to_string) (
            void *direction,
            const axis2_env_t *env);


};

struct axis2_woden_direction
{
    axis2_woden_direction_ops_t *ops;
    
};

AXIS2_EXTERN axis2_woden_direction_t * AXIS2_CALL
axis2_woden_direction_create(
        const axis2_env_t *env,
        axis2_char_t *value);

#define AXIS2_WODEN_DIRECTION_FREE(direction, env) \
      (((axis2_woden_direction_t *) direction)->ops->\
         free (direction, env))

#define AXIS2_WODEN_DIRECTION_GET_DIRECTION_IN(direction, env) \
      (((axis2_woden_direction_t *) direction)->ops->\
         get_direction_in (direction, env))

#define AXIS2_WODEN_DIRECTION_GET_DIRECTION_OUT(direction, env) \
      (((axis2_woden_direction_t *) direction)->ops->\
         get_direction_out (direction, env))

#define AXIS2_WODEN_DIRECTION_TO_STRING(direction, env) \
      (((axis2_woden_direction_t *) direction)->ops->\
         to_string (direction, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_DIRECTION_H */
