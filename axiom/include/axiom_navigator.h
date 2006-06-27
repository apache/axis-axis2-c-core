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
 
 #ifndef AXIOM_NAVIGATOR_H
 #define AXIOM_NAVIGATOR_H
 
/**
 * @file axiom_navigator.h
 * @brief defines axiom_navigator struct and its ops
 */

#include <axis2_utils.h>
#include <axis2_env.h>
#include <axiom_node.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axiom_navigator  navigator 
 * @ingroup axiom_om
 * @{
 */
 
 typedef struct axiom_navigator axiom_navigator_t;

 typedef struct axiom_navigator_ops axiom_navigator_ops_t;
 
/**
 * @brief node ops struct
 * Encapsulator struct for ops of axiom_node
 */
 
 struct axiom_navigator_ops 
 {
    /**
     * free function , free the axiom_navigator struct
     * @param om_navigator axiom_navigator_struct
     * @param env environment MUST not be NULL
     * @returns AXIS2_SUCCESS 
     */
     
     axis2_status_t (AXIS2_CALL *
     free)(axiom_navigator_t *om_navigator,
           const axis2_env_t *env); 
 
    /**
     *  returns the navigable status 
     * @param  om_navigator axiom_navigator_struct
     * @param env environment MUST not be NULL
     * @returns AXIS2_TRUE if the om is navigable 
     * otherwise returns AXIS2_FALSE
     */

     axis2_bool_t (AXIS2_CALL *
     is_navigable)(axiom_navigator_t *om_navigator,
                   const axis2_env_t *env);

    /**
     * returns the build status of this node 
     * if the node is completly build returns AXIS2_TRUE
     * otherwise AXIS2_FALSE
     * @param om_navigator axiom_navigator struct
     * @param env environment MUST not be NULL
     * @return AXIS2_TRUE if this node is completly built
     * otherwise return AXIS2_FALSE
     */
     
     axis2_bool_t (AXIS2_CALL *
     is_completed)(axiom_navigator_t *om_navigator,
                        const axis2_env_t *env);

    /**
     * gets the next node 
     * @param om_navigator om_navigaot struct 
     * @param env environment MUST not be NULL
     * @returns axiom_node_t pointer in the sequence of preorder travasal 
     * however the an element node is treated slightly differently
     * Once the om_element type om node is passed it returns the same om_node 
     * pointer in the next , returns NULL on error or if there is no more nodes
     */

     axiom_node_t* (AXIS2_CALL *
     next)(axiom_navigator_t *om_navigator, 
           const axis2_env_t *env);

    /**
     * method visited 
     * @param om_navigator om_navigaot struct
     * @param env environment MUST not be NULL
     * @returns AXIS2_TRUE if this node is alrady visited
     * otherwise AXIS2_FALSE
     */
     axis2_bool_t (AXIS2_CALL *
     visited)(axiom_navigator_t *om_navigator,
              const axis2_env_t *env);

 };
    
/** axiom_navigator_t struct */

struct axiom_navigator
{
    axiom_navigator_ops_t *ops;
};

/** create an axiom_navigator 
 * @param env environment MUST not be NULL
 * @param node a pointer to axiom_node_t struct 
 * which is to be navigated
 * @returns a pointer to axiom_navigator_t struct
 * or returns NULL on error
 */

AXIS2_EXTERN axiom_navigator_t * AXIS2_CALL
axiom_navigator_create(const axis2_env_t *env,
                          axiom_node_t *node);
                          
/** Macros ********************************************************************/

#define AXIOM_NAVIGATOR_FREE(navigator, env) \
        ((navigator)->ops->free(navigator, env))

#define AXIOM_NAVIGATOR_IS_NAVIGABLE(navigator, env) \
        ((navigator)->ops->is_navigable(navigator, env))

#define AXIOM_NAVIGATOR_IS_COMPLETED(navigator, env) \
        ((navigator)->ops->is_completed(navigator, env))

#define AXIOM_NAVIGATOR_VISITED(navigator, env) \
        ((navigator)->ops->visited(navigator, env))

#define AXIOM_NAVIGATOR_NEXT(navigator, env) \
        ((navigator)->ops->next(navigator, env))

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIOM_NAVIGATOR_H */
