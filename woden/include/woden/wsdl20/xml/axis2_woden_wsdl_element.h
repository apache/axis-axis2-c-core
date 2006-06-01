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

#ifndef AXIS2_WODEN_WSDL_ELEMENT_H
#define AXIS2_WODEN_WSDL_ELEMENT_H

/**
 * @file axis2_woden_wsdl_element.h
 * @brief Axis2 Wsdl Element Interface
 * This abstract class defines the behaviour common to all WSDL elements.
 * That is, it implements support for extension attributes and elements.
 * This interface can be used as a common reference for all WSDL elements
 * represented by the Element API.
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_url.h>
#include <axis2_array_list.h>
#include <woden/axis2_woden.h>
#include <woden/wsdl20/extensions/axis2_woden_attr_extensible.h>
#include <woden/wsdl20/extensions/axis2_woden_element_extensible.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_wsdl_element axis2_woden_wsdl_element_t;
typedef struct axis2_woden_wsdl_element_ops axis2_woden_wsdl_element_ops_t;
typedef union axis2_woden_wsdl_element_base axis2_woden_wsdl_element_base_t;
struct axis2_woden_xml_attr;
struct axis2_woden_ext_element;

/** @defgroup axis2_woden_wsdl_element Wsdl Element
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_wsdl_element_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *wsdl_element,
            axis2_env_t **env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *wsdl_element,
            axis2_env_t **env);
 
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *wsdl_element,
            axis2_env_t **env);
    
};

union axis2_woden_wsdl_element_base
{
    axis2_woden_attr_extensible_t attr_extensible;
    axis2_woden_element_extensible_t element_extensible;
};

struct axis2_woden_wsdl_element
{
    axis2_woden_wsdl_element_base_t base;
    axis2_woden_wsdl_element_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_wsdl_element_t *)
axis2_woden_wsdl_element_create(
        axis2_env_t **env);

/************************Woden C Internal Methods******************************/
AXIS2_DECLARE(axis2_woden_wsdl_element_t *)
axis2_woden_wsdl_element_to_attr_extensible(
        void *wsdl_element,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_wsdl_element_t *)
axis2_woden_wsdl_element_to_element_extensible(
        void *wsdl_element,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_status_t)
axis2_woden_wsdl_element_resolve_methods(
        axis2_woden_wsdl_element_t *wsdl_element,
        axis2_env_t **env,
        axis2_woden_wsdl_element_t *wsdl_element_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_WSDL_ELEMENT_FREE(wsdl_element, env) \
		(((axis2_woden_wsdl_element_t *) wsdl_element)->ops->\
         free (wsdl_element, env))

#define AXIS2_WODEN_WSDL_ELEMENT_SUPER_OBJS(wsdl_element, env) \
		(((axis2_woden_wsdl_element_t *) wsdl_element)->ops->\
         super_objs (wsdl_element, env))

#define AXIS2_WODEN_WSDL_ELEMENT_TYPE(wsdl_element, env) \
		(((axis2_woden_wsdl_element_t *) wsdl_element)->ops->\
         type (wsdl_element, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_WSDL_ELEMENT_H */
