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

#ifndef AXIS2_WODEN_INTERFACE_MSG_REF_H
#define AXIS2_WODEN_INTERFACE_MSG_REF_H

/**
 * @file axis2_woden_interface_msg_ref.h
 * @brief Axis2 Interface Message Reference Interface Message Reference
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_interface_msg_ref_element.h>
#include <woden/wsdl20/axis2_woden_nested_configurable.h>
#include <axis2_qname.h>

/** @defgroup axis2_woden_interface_msg_ref Interface Message Reference
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_interface_msg_ref_base axis2_woden_interface_msg_ref_base_t;
typedef struct axis2_woden_interface_msg_ref axis2_woden_interface_msg_ref_t;
typedef struct axis2_woden_interface_msg_ref_ops axis2_woden_interface_msg_ref_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_interface_msg_ref_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *interface_msg_ref,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    to_interface_msg_ref_free) (void *interface_msg_ref,
            axis2_env_t **env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *interface_msg_ref,
            axis2_env_t **env);

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *interface_msg_ref,
            axis2_env_t **env);
    /**
     * @return the base implementation class
     */
    struct axis2_woden_nested_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *interface_msg_ref,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    get_msg_label) (
            void *interface_msg_ref,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    get_direction) (
            void *interface_msg_ref,
            axis2_env_t **env);

    /**
     * Indicates the type of message content.#any means any single element, 
     * #none means no message content, #other means non-XML extension type system
     * or #element means XML Schema global element definition.
     *   
     * @return string representing the type of message content
     */
    axis2_char_t *(AXIS2_CALL *
    get_msg_content_model) (
            void *interface_msg_ref,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    get_element_declaration) (
            void *interface_msg_ref,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    to_element)  (
            void *interface_msg_ref,
            axis2_env_t **env);

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/
    axis2_status_t (AXIS2_CALL *
    set_element_declaration) (
            void *interface_msg_ref,
            axis2_env_t **env,
            void *element);

    axis2_status_t (AXIS2_CALL *
    set_types) (
            void *interface_msg_ref,
            axis2_env_t **env,
            void *types);
};

union axis2_woden_interface_msg_ref_base
{
    axis2_woden_nested_configurable_t nested_configurable;
    axis2_woden_interface_msg_ref_element_t interface_msg_ref_element;
};

struct axis2_woden_interface_msg_ref
{
    axis2_woden_interface_msg_ref_base_t base;
    axis2_woden_interface_msg_ref_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_interface_msg_ref_t *)
axis2_woden_interface_msg_ref_create(
        axis2_env_t **env);


/***************************Woden C Internal Methods***************************/
AXIS2_DECLARE(axis2_woden_interface_msg_ref_t *)
axis2_woden_interface_msg_ref_to_interface_msg_ref_element(
        void *interface_msg_ref,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_interface_msg_ref_t *)
axis2_woden_interface_msg_ref_to_nested_configurable(
        void *interface_msg_ref,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_interface_msg_ref_t *)
axis2_woden_interface_msg_ref_to_configurable(
        void *interface_msg_ref,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_status_t)
axis2_woden_interface_msg_ref_resolve_methods(
        axis2_woden_interface_msg_ref_t *interface_msg_ref,
        axis2_env_t **env,
        axis2_woden_interface_msg_ref_t *interface_msg_ref_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_INTERFACE_MSG_REF_FREE(interface_msg_ref, env) \
		(((axis2_woden_interface_msg_ref_t *) interface_msg_ref)->ops->free(interface_msg_ref, env))

#define AXIS2_WODEN_INTERFACE_MSG_REF_TO_INTERFACE_MSG_REF_FREE(interface_msg_ref, env) \
		(((axis2_woden_interface_msg_ref_t *) interface_msg_ref)->ops->to_interface_msg_ref_free(interface_msg_ref, env))

#define AXIS2_WODEN_INTERFACE_MSG_REF_SUPER_OBJS(interface_msg_ref, env) \
		(((axis2_woden_interface_msg_ref_t *) interface_msg_ref)->ops->super_objs(interface_msg_ref, env))

#define AXIS2_WODEN_INTERFACE_MSG_REF_TYPE(interface_msg_ref, env) \
		(((axis2_woden_interface_msg_ref_t *) interface_msg_ref)->ops->type(interface_msg_ref, env))

#define AXIS2_WODEN_INTERFACE_MSG_REF_GET_BASE_IMPL(interface_msg_ref, env) \
		(((axis2_woden_interface_msg_ref_t *) interface_msg_ref)->ops->get_base_impl(interface_msg_ref, env))

#define AXIS2_WODEN_INTERFACE_MSG_REF_GET_MESSAGE_LABEL(interface_msg_ref, env) \
		(((axis2_woden_interface_msg_ref_t *) interface_msg_ref)->\
         get_msg_label(interface_msg_ref, env))

#define AXIS2_WODEN_INTERFACE_MSG_REF_GET_DIRECTION(interface_msg_ref, env) \
		(((axis2_woden_interface_msg_ref_t *) interface_msg_ref)->\
         get_direction(interface_msg_ref, env))

#define AXIS2_WODEN_INTERFACE_MSG_REF_GET_MSG_CONTENT_MODEL(interface_msg_ref, env) \
		(((axis2_woden_interface_msg_ref_t *) interface_msg_ref)->\
         get_msg_content_model(interface_msg_ref, env))

#define AXIS2_WODEN_INTERFACE_MSG_REF_GET_ELEMENT_DECLARATION(interface_msg_ref, env) \
		(((axis2_woden_interface_msg_ref_t *) interface_msg_ref)->\
         get_element_declaration(interface_msg_ref, env))

#define AXIS2_WODEN_INTERFACE_MSG_REF_TO_ELEMENT(interface_msg_ref, env) \
		(((axis2_woden_interface_msg_ref_t *) interface_msg_ref)->ops->\
         to_element(interface_msg_ref, env))

#define AXIS2_WODEN_INTERFACE_MSG_REF_SET_ELEMENT_DECLARATION(interface_msg_ref, env, element) \
		(((axis2_woden_interface_msg_ref_t *) interface_msg_ref)->\
         set_element_declaration(interface_msg_ref, env, element))

#define AXIS2_WODEN_INTERFACE_MSG_REF_SET_TYPES(interface_msg_ref, env, types) \
		(((axis2_woden_interface_msg_ref_t *) interface_msg_ref)->\
         set_types(interface_msg_ref, env, types))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_INTERFACE_MSG_REF_H */
