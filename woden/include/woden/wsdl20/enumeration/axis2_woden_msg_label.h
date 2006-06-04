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

#ifndef AXIS2_WODEN_MSG_LABEL_H
#define AXIS2_WODEN_MSG_LABEL_H

/**
 * @file axis2_woden_msg_label.h
 * @brief Axis2 Message Label Interface
 * This class defines the values of the {msg_label} property of
 * Interface Message Reference and Interface Fault Reference. This property 
 * indicates whether a message is coming "in" to the service or going "out" 
 * from the service.
 * <p>
 * The property is represented in XML by the message or fault 
 * reference element's tag name:
 * <ul>
 * <li>&lt;input&gt; and &lt;infault&gt; have the msg_label "in" - 
 *     represented by the constant IN
 * <li>&lt;output&gt; and &lt;outfault&gt; have the msg_label "out" - 
 *     represented by the constant OUT
 * </ul>
 * This class uses the typesafe enum pattern. Applications should use the
 * constants defined in this class to specify or to 
 * evaluate msg_label.
 * <p>
 * Examples:
 * <pre>
 *     AXIS2_WODEN_INTERFACE_MSG_REF_SET_MSG_LABEL(msg_ref, env, msg_ref->AXIS2_WODEN_IN);
 *     if(msg_ref->AXIS2_WODEN_IN == AXIS2_WODEN_INTERFACE_MSG_REF_GET_MSG_LABEL(msg_ref, env))
 *          ...
 *     
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

typedef struct axis2_woden_msg_label axis2_woden_msg_label_t;
typedef struct axis2_woden_msg_label_ops axis2_woden_msg_label_ops_t;
struct axis2_woden_ext_element;

/** @defgroup axis2_woden_msg_label Message Label
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_msg_label_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *msg_label,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_msg_label_in) (
            void *msg_label,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_msg_label_out) (
            void *msg_label,
            const axis2_env_t *env);

    axis2_char_t *(AXIS2_CALL *
    to_string) (
            void *msg_label,
            const axis2_env_t *env);

    axis2_bool_t (AXIS2_CALL *
    is_valid) (
            void *msg_label,
            const axis2_env_t *env);

    axis2_bool_t (AXIS2_CALL *
    equals) (
            void *msg_label,
            const axis2_env_t *env,
            axis2_woden_msg_label_t *other);


};

struct axis2_woden_msg_label
{
    axis2_woden_msg_label_ops_t *ops;
    
};

AXIS2_EXTERN axis2_woden_msg_label_t * AXIS2_CALL
axis2_woden_msg_label_create(
        const axis2_env_t *env,
        axis2_char_t *value,
        axis2_bool_t valid);

#define AXIS2_WODEN_MSG_LABEL_FREE(msg_label, env) \
      (((axis2_woden_msg_label_t *) msg_label)->ops->\
         free (msg_label, env))

#define AXIS2_WODEN_MSG_LABEL_GET_MSG_LABEL_IN(msg_label, env) \
      (((axis2_woden_msg_label_t *) msg_label)->ops->\
         get_msg_label_in (msg_label, env))

#define AXIS2_WODEN_MSG_LABEL_GET_MSG_LABEL_OUT(msg_label, env) \
      (((axis2_woden_msg_label_t *) msg_label)->ops->\
         get_msg_label_out (msg_label, env))

#define AXIS2_WODEN_MSG_LABEL_TO_STRING(msg_label, env) \
      (((axis2_woden_msg_label_t *) msg_label)->ops->\
         to_string (msg_label, env))

#define AXIS2_WODEN_MSG_LABEL_IS_VALID(msg_label, env) \
      (((axis2_woden_msg_label_t *) msg_label)->ops->\
         is_valid (msg_label, env))

#define AXIS2_WODEN_MSG_LABEL_EQUALS(msg_label, env, other) \
      (((axis2_woden_msg_label_t *) msg_label)->ops->\
         equals (msg_label, env, other))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_MSG_LABEL_H */
