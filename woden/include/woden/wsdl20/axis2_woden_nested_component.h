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

#ifndef AXIS2_WODEN_NESTED_COMPONENT_H
#define AXIS2_WODEN_NESTED_COMPONENT_H

/**
 * @file axis2_woden_nested_component.h
 * @brief Axis2 Nested Component Interface
 * Components which are nested within a 'top-level' component will extend
 * this interface. These are Property, Feature and the sub components of
 * Interface, Binding and Service. 
 * <p>
 * The components which are not nested are Description, Interface, 
 * Binding, Service, ElementDeclaration and TypeDefinition.
 *
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <woden/axis2_woden.h>
#include <woden/wsdl20/axis2_woden_wsdl_component.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_nested_component axis2_woden_nested_component_t;
typedef struct axis2_woden_nested_component_ops axis2_woden_nested_component_ops_t;
struct axis2_woden_component_exts;

/** @defgroup axis2_woden_nested_component Nested Component
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_nested_component_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *nested_component,
            const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    to_nested_component_free) (
            void *nested_component,
            const axis2_env_t *env);
    
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *nested_component,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_parent) (
            void *nested_component,
            const axis2_env_t *env);

};

struct axis2_woden_nested_component
{
    axis2_woden_wsdl_component_t wsdl_component;
    axis2_woden_nested_component_ops_t *ops;
};

/************************Woden C Internal Methods******************************/
axis2_status_t AXIS2_CALL
axis2_woden_nested_component_resolve_methods(
        axis2_woden_nested_component_t *nested_component,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_NESTED_COMPONENT_FREE(nested_component, env) \
		(((axis2_woden_nested_component_t *) nested_component)->ops->\
         free (nested_component, env))

#define AXIS2_WODEN_NESTED_COMPONENT_TO_NESTED_COMPONENT_FREE(nested_component, \
        env) \
		(((axis2_woden_nested_component_t *) nested_component)->ops->\
         to_nested_component_free (nested_component, env))

#define AXIS2_WODEN_NESTED_COMPONENT_TYPE(nested_component, env) \
		(((axis2_woden_nested_component_t *) nested_component)->ops->\
         type (nested_component, env))

#define AXIS2_WODEN_NESTED_COMPONENT_GET_PARENT(\
        nested_component, env) \
		(((axis2_woden_nested_component_t *) nested_component)->ops->\
         get_parent(nested_component, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_NESTED_COMPONENT_H */
