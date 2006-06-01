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

#ifndef AXIS2_WODEN_DOCUMENTATION_H
#define AXIS2_WODEN_DOCUMENTATION_H

/**
 * @file axis2_woden_documentation.h
 * @brief Axis2 Documentation Interface
 * This class implements support for parsing, creating and manipulating a
 * WSDL 2.0 &lt;wsdl:documentation&gt; XML element.
 * The &lt;wsdl:documentation&gt; element may contain mixed content, but this 
 * class does not attempt to understand that content. Instead it just wraps
 * the &lt;wsdl:documentation&gt; element's content model as a void *. 
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_documentation_element.h>
#include <woden/wsdl20/xml/axis2_woden_wsdl_element.h>

/** @defgroup axis2_woden_documentation Documentation
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_documentation_base axis2_woden_documentation_base_t;
typedef struct axis2_woden_documentation axis2_woden_documentation_t;
typedef struct axis2_woden_documentation_ops axis2_woden_documentation_ops_t;
struct axis2_woden_documentation_element;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_documentation_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *documentation,
            const axis2_env_t *env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *documentation,
            const axis2_env_t *env);
    
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *documentation,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    axis2_woden_wsdl_element_t *(AXIS2_CALL *
    get_base_impl) (
            void *documentation,
            const axis2_env_t *env);
};

union axis2_woden_documentation_base
{
    axis2_woden_wsdl_element_t wsdl_element;
    axis2_woden_documentation_element_t documentation_element;
};

struct axis2_woden_documentation
{
    axis2_woden_documentation_base_t base;
    axis2_woden_documentation_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_documentation_t *)
axis2_woden_documentation_create(
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_DECLARE(axis2_woden_documentation_t *)
axis2_woden_documentation_to_documentation_element(
        void *documentation,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_woden_documentation_t *)
axis2_woden_documentation_to_attr_extensible(
        void *documentation,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_woden_documentation_t *)
axis2_woden_documentation_to_element_extensible(
        void *documentation,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_status_t)
axis2_woden_documentation_resolve_methods(
        axis2_woden_documentation_t *documentation,
        const axis2_env_t *env,
        axis2_woden_documentation_t *documentation_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/


#define AXIS2_WODEN_DOCUMENTATION_FREE(documentation, env) \
		(((axis2_woden_documentation_t *) documentation)->ops->free(documentation, env))

#define AXIS2_WODEN_DOCUMENTATION_SUPER_OBJS(documentation, env) \
		(((axis2_woden_documentation_t *) documentation)->ops->\
         super_objs(documentation, env))

#define AXIS2_WODEN_DOCUMENTATION_TYPE(documentation, env) \
		(((axis2_woden_documentation_t *) documentation)->ops->type(documentation, env))

#define AXIS2_WODEN_DOCUMENTATION_GET_BASE_IMPL(documentation, env) \
		(((axis2_woden_documentation_t *) documentation)->ops->\
         get_base_impl(documentation, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_DOCUMENTATION_H */
