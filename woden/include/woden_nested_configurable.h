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

#ifndef WODEN_NESTED_CONFIGURABLE_H
#define WODEN_NESTED_CONFIGURABLE_H

/**
 * @file woden_nested_configurable.h
 * @brief Axis2 Nested Configurable Interface
 * This abstract superclass implements support for accessing or
 * setting the 'parent' of a nested, configurable WSDL component.
 * All such classes will directly or indirectly extend this abstract 
 * class.
 * 
 */

#include <woden.h>
#include <woden_nested_component.h>
#include <woden_nested_element.h>
#include <woden_configurable.h>

/** @defgroup woden_nested_configurable Nested Configurable
  * @ingroup woden
  * @{
  */

typedef union woden_nested_configurable_base woden_nested_configurable_base_t;
typedef struct woden_nested_configurable woden_nested_configurable_t;
typedef struct woden_nested_configurable_ops woden_nested_configurable_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_nested_configurable_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *nested_configurable,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    to_nested_configurable_free) (
            void *nested_configurable,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *nested_configurable,
            const axis2_env_t *env);
    
    /**
     * @return the base implementation class
     */
    struct woden_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *nested_configurable,
            const axis2_env_t *env);

};

union woden_nested_configurable_base
{
    woden_configurable_t configurable;
    woden_nested_element_t nested_element;
    woden_nested_component_t nested_component;
};

struct woden_nested_configurable
{
    woden_nested_configurable_base_t base;
    woden_nested_configurable_ops_t *ops;
};

AXIS2_EXTERN woden_nested_configurable_t * AXIS2_CALL
woden_nested_configurable_create(
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_nested_configurable_t * AXIS2_CALL
woden_nested_configurable_to_nested_element(
        void *nested_configurable,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_nested_configurable_resolve_methods(
        woden_nested_configurable_t *nested_configurable,
        const axis2_env_t *env,
        woden_nested_configurable_t *nested_configurable_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/


#define WODEN_NESTED_CONFIGURABLE_FREE(nested_configurable, env) \
      (((woden_nested_configurable_t *) nested_configurable)->ops->\
         free(nested_configurable, env))

#define WODEN_NESTED_CONFIGURABLE_TO_NESTED_CONFIGURABLE_FREE(nested_configurable, env) \
      (((woden_nested_configurable_t *) nested_configurable)->ops->\
         to_nested_configurable_free(nested_configurable, env))

#define WODEN_NESTED_CONFIGURABLE_SUPER_OBJS(nested_configurable, env) \
      (((woden_nested_configurable_t *) nested_configurable)->ops->\
         super_objs(nested_configurable, env))

#define WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(nested_configurable, env) \
      (((woden_nested_configurable_t *) nested_configurable)->ops->\
         get_base_impl(nested_configurable, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_NESTED_CONFIGURABLE_H */
