/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not svc_grp this file except in compliance with the License.
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

#ifndef AXIS2_SVC_GRP_BUILDER_H
#define AXIS2_SVC_GRP_BUILDER_H

/**
 * @file axis2_svc_grp_builder.h
 * @brief Axis2 Service Group Builder interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_desc_builder.h>
#include <axis2_svc_builder.h>

#ifdef __cplusplus
extern "C"
{
#endif
    
typedef struct axis2_svc_grp_builder axis2_svc_grp_builder_t;
typedef struct axis2_svc_grp_builder_ops axis2_svc_grp_builder_ops_t;

/** @defgroup axis2_svc_grp_builder Service Group Builder
 * @ingroup axis2_deployment
 * @{
 */

/** 
 * @brief Service Group Builder ops struct
 * Encapsulator struct for ops of axis2_svc_grp_builder
 */
AXIS2_DECLARE_DATA struct axis2_svc_grp_builder_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *
    free)(axis2_svc_grp_builder_t *svc_grp_builder,
	        axis2_env_t **env);
                               
    axis2_status_t (AXIS2_CALL *
    populate_svc_grp) (axis2_svc_grp_builder_t *grp_builder,
                                            axis2_env_t **env,
                                            axis2_svc_grp_t *svc_grp);
    
    /**
     * To get the list og modules that is requird to be engage globally
     * @param module_refs  <code>axis2_om_children_qname_iterator_t</code>
     */
    axis2_status_t (AXIS2_CALL *
    process_module_refs) (axis2_svc_grp_builder_t *grp_builder,
                                                axis2_env_t **env,
                                       axis2_om_children_qname_iterator_t *module_refs ,
                                                axis2_svc_grp_t *svc_grp);

};

/** 
 * @brief Service Group Builder struct 
 */  
AXIS2_DECLARE_DATA struct axis2_svc_grp_builder
{
	axis2_svc_grp_builder_ops_t *ops;
    axis2_desc_builder_t *desc_builder;
};

/**
 * Creates svc_grp builder struct
 * @return pointer to newly created service group builder
 */
AXIS2_DECLARE(axis2_svc_grp_builder_t *) 
axis2_svc_grp_builder_create (axis2_env_t **env);

/**
 * Creates svc_grp builder struct
 * @param svc
 * @param dep_engine
 * @return pointer to newly created service group builder
 */
AXIS2_DECLARE(axis2_svc_grp_builder_t *) 
axis2_svc_grp_builder_create_with_svc_and_dep_engine (axis2_env_t **env,
                                                axis2_om_node_t *svc,
                                                axis2_dep_engine_t *dep_engine);

/*************************** Function macros **********************************/

#define AXIS2_SVC_GRP_BUILDER_FREE(svc_grp_builder, env) \
		((svc_grp_builder->ops)->free (svc_grp_builder, env))

#define AXIS2_SVC_GRP_BUILDER_POPULATE_SVC_GRP(svc_grp_builder, env, svc_grp) \
		((svc_grp_builder->ops)->populate_svc_grp (svc_grp_builder, env, svc_grp))

#define AXIS2_SVC_GRP_BUILDER_PROCESS_MODULE_REFS(svc_grp_builder, env, \
        module_refs, svc_grp) \
		((svc_grp_builder->ops)->process_module_refs (svc_grp_builder, env, \
            module_refs, svc_grp))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_SVC_GRP_BUILDER_H */
