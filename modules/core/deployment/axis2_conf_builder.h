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

#ifndef AXIS2_CONF_BUILDER_H
#define AXIS2_CONF_BUILDER_H

/**
 * @file axis2_conf_builder.h
 * @brief Axis2 Conf Builder interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_desc_builder.h>
#include <axis2_conf.h>
#include <axis2_dep_engine.h>
#include <axis2_om_children_qname_iterator.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_desc_builder;
struct axis2_dep_engine;    
typedef struct axis2_conf_builder axis2_conf_builder_t;
typedef struct axis2_conf_builder_ops axis2_conf_builder_ops_t;

/** @defgroup axis2_conf_builder Conf Builder
 * @ingroup axis2_deployment
 * @{
 */

/** 
 * @brief Conf Builder ops struct
 * Encapsulator struct for ops of axis2_conf_builder
 */
AXIS2_DECLARE_DATA struct axis2_conf_builder_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *
    free)(axis2_conf_builder_t *conf_builder,
	        axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    populate_conf) (axis2_conf_builder_t *conf_builder,
                                        axis2_env_t **env);
    
    /**
     * To get the list og modules that is requird to be engage globally
     */
    axis2_status_t (AXIS2_CALL *
    process_module_refs) (axis2_conf_builder_t *conf_builder,
                            axis2_env_t **env,
                            axis2_om_children_qname_iterator_t *module_refs);
                               
 

};

/** 
 * @brief Conf Builder struct 
 */  
AXIS2_DECLARE_DATA struct axis2_conf_builder
{
	axis2_conf_builder_ops_t *ops;
    struct axis2_desc_builder *desc_builder;
};

/**
 * Creates conf builder struct
 * @return pointer to newly created conf builder
 */
AXIS2_DECLARE(axis2_conf_builder_t *) 
axis2_conf_builder_create (axis2_env_t **env);

/**
 * Creates conf builder struct
 * @param file This is the full path to the server xml file
 * @param dep_engine
 * @param conf
 * @return pointer to newly created conf builder
 */

AXIS2_DECLARE(axis2_conf_builder_t *)
axis2_conf_builder_create_with_file_and_dep_engine_and_conf (axis2_env_t **env,
                                            axis2_char_t *file,
                                            struct axis2_dep_engine *dep_engine,
                                            axis2_conf_t *conf);

/*************************** Function macros **********************************/

#define AXIS2_CONF_BUILDER_FREE(conf_builder, env) \
		((conf_builder)->ops->free (conf_builder, env))

#define AXIS2_CONF_BUILDER_POPULATE_CONF(conf_builder, env) \
		((conf_builder)->ops->populate_conf (conf_builder, env))

#define AXIS2_CONF_BUILDER_PROCESS_MODULE_REFS(conf_builder, env, module_refs) \
		((conf_builder)->ops->process_module_refs (conf_builder, env, module_refs))


/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_CONF_BUILDER_H */
