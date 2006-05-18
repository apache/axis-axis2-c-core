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
 
 #ifndef AXIS2_OM_NAVIGATOR_H
 #define AXIS2_OM_NAVIGATOR_H
 
/** @defgroup axis2_om AXIOM (Axis Object Model)
 * @ingroup axis2
 * @{
 */

/** @} */
      
/**
 * @file axis2_om_navigator.h
 * @brief defines axis2_om_navigator struct and its ops
 */

#include <axis2_utils.h>
#include <axis2_env.h>
#include <axis2_om_node.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_om_navigator  om_navigator 
 * @ingroup axis2_om
 * @{
 */
 
 typedef struct axis2_om_navigator axis2_om_navigator_t;

 typedef struct axis2_om_navigator_ops axis2_om_navigator_ops_t;
 
/**
 * @brief OM Node ops struct
 * Encapsulator struct for ops of axis2_om_node
 */
 
 struct axis2_om_navigator_ops 
 {
    /**
     * free function , free the axis2_om_navigator struct
     * @param om_navigator axis2_om_navigator_struct
     * @param env environment MUST not be NULL
     * @returns AXIS2_SUCCESS 
     */
     
     axis2_status_t (AXIS2_CALL *
     free)(axis2_om_navigator_t *om_navigator,
           axis2_env_t **env); 
 
    /**
     *  returns the navigable status 
     * @param  om_navigator axis2_om_navigator_struct
     * @param env environment MUST not be NULL
     * @returns AXIS2_TRUE if the om is navigable 
     * otherwise returns AXIS2_FALSE
     */

     axis2_bool_t (AXIS2_CALL *
     is_navigable)(axis2_om_navigator_t *om_navigator,
                   axis2_env_t **env);

    /**
     * returns the build status of this node 
     * if the node is completly build returns AXIS2_TRUE
     * otherwise AXIS2_FALSE
     * @param om_navigator axis2_om_navigator struct
     * @param env environment MUST not be NULL
     * @return AXIS2_TRUE if this node is completly built
     * otherwise return AXIS2_FALSE
     */
     
     axis2_bool_t (AXIS2_CALL *
     is_completed)(axis2_om_navigator_t *om_navigator,
                        axis2_env_t **env);

    /**
     * gets the next node 
     * @param om_navigator om_navigaot struct 
     * @param env environment MUST not be NULL
     * @returns axis2_om_node_t pointer in the sequence of preorder travasal 
     * however the an element node is treated slightly differently
     * Once the om_element type om node is passed it returns the same om_node 
     * pointer in the next , returns NULL on error or if there is no more nodes
     */

     axis2_om_node_t* (AXIS2_CALL *
     next)(axis2_om_navigator_t *om_navigator, 
           axis2_env_t **env);

    /**
     * method visited 
     * @param om_navigator om_navigaot struct
     * @param env environment MUST not be NULL
     * @returns AXIS2_TRUE if this node is alrady visited
     * otherwise AXIS2_FALSE
     */
     axis2_bool_t (AXIS2_CALL *
     visited)(axis2_om_navigator_t *om_navigator,
              axis2_env_t **env);

 };
    
/** axis2_om_navigator_t struct */

struct axis2_om_navigator
{
    axis2_om_navigator_ops_t *ops;
};

/** create an axis2_om_navigator 
 * @param env environment MUST not be NULL
 * @param node a pointer to axis2_om_node_t struct 
 * which is to be navigated
 * @returns a pointer to axis2_om_navigator_t struct
 * or returns NULL on error
 */

AXIS2_DECLARE(axis2_om_navigator_t *)
axis2_om_navigator_create(axis2_env_t **env,
                          axis2_om_node_t *node);
                          
/** Macros ********************************************************************/

#define AXIS2_OM_NAVIGATOR_FREE(navigator, env) \
        ((navigator)->ops->free(navigator, env))

#define AXIS2_OM_NAVIGATOR_IS_NAVIGABLE(navigator, env) \
        ((navigator)->ops->is_navigable(navigator, env))

#define AXIS2_OM_NAVIGATOR_IS_COMPLETED(navigator, env) \
        ((navigator)->ops->is_completed(navigator, env))

#define AXIS2_OM_NAVIGATOR_VISITED(navigator, env) \
        ((navigator)->ops->visited(navigator, env))

#define AXIS2_OM_NAVIGATOR_NEXT(navigator, env) \
        ((navigator)->ops->next(navigator, env))

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_OM_NAVIGATOR_H */
