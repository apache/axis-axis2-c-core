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
#ifndef AXIS2_MODULE_H
#define AXIS2_MODULE_H

/**
 * @file axis2_module.h
 * @brief axis2 module interface
 * Every module provides an implementation of this class. Modules are in one of
 * three states: "available" and "initialized". All modules that the runtime
 * detects (from the system modules/ directory or from other means) are said to
 * be in the "available" state. If some service indicates a dependency on this
 * module then the module is initialized (once for the life of the system) and
 * the state changes to "initialized".
 * <p/>
 * <p/>Any module which is in the "initialized" state can be engaged as needed
 * by the engine to respond to a message. Currently module engagement is done
 * via deployment (using module.xml). In the future we may engage modules
 * programmatically by introducing an engage() method to this interface, thereby
 * allowing more dynamic scenarios. 
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_conf.h>
#include <axis2_hash.h>


#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_module Module 
  * @ingroup axis2_module
  * @{
  */

struct axis2_conf;
typedef struct axis2_module_ops axis2_module_ops_t;   
typedef struct axis2_module axis2_module_t;
    
AXIS2_DECLARE_DATA struct axis2_module_ops
{
    /* initialize the module */
    axis2_status_t (AXIS2_CALL *
    init) (struct axis2_module *module, 
            const axis2_env_t *env);

    /* TODO figure out how to get the engage() concept done */
    /* public void engage(ExecutionChain exeChain) throws AxisFault; */
    
    /* shutdown the module */
    axis2_status_t (AXIS2_CALL * 
    shutdown)(struct axis2_module *module,
                const axis2_env_t *env);
    
    /** 
     * Return a hash map of handler create functions for the module
     * @return function ptr map 
     */
    axis2_status_t (AXIS2_CALL * 
    fill_handler_create_func_map)(struct axis2_module *module,
                                    const axis2_env_t *env);


} ;

AXIS2_DECLARE_DATA struct axis2_module 
{
    axis2_module_ops_t *ops;
    axis2_hash_t *handler_create_func_map;
};

/**
 * Creates axis2_module struct
 * @return pointer to newly created axis2_module
 */
AXIS2_EXTERN axis2_module_t * AXIS2_CALL 
axis2_module_create (const axis2_env_t *env);

/*************************** Function macros **********************************/

#define AXIS2_MODULE_INIT(module, env) \
      ((module)->ops->init (module, env)) 

#define AXIS2_MODULE_SHUTDOWN(module, env) \
      ((module)->ops->shutdown (module, env)) 

#define AXIS2_MODULE_FILL_HANDLER_CREATE_FUNC_MAP(module, env) \
      ((module)->ops->fill_handler_create_func_map (module, env))

/** @} */

#ifdef __cplusplus
}
#endif
#endif /* AXIS2_MODULE_H */
