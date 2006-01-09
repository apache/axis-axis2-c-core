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

#ifndef AXIS2_REPOS_LISTENER_H
#define AXIS2_REPOS_LISTENER_H

/**
 * @file axis2_repos_listener.h
 * @brief Axis2 Repos Listener interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_array_list.h>
#include <axis2_ws_info_list.h>
#include <axis2_dep_engine.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_dep_engine;    
typedef struct axis2_repos_listener axis2_repos_listener_t;
typedef struct axis2_repos_listener_ops axis2_repos_listener_ops_t;

/** @defgroup axis2_repos_listener Repos Listener
 * @ingroup axis2_deployment
 * @{
 */

/** 
 * @brief Repos Listener ops struct
 * Encapsulator struct for ops of axis2_repos_listener
 */
AXIS2_DECLARE_DATA struct axis2_repos_listener_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *
    free)(axis2_repos_listener_t *repos_listener,
	        axis2_env_t **env);
    
    /**
     * this method ask serachWS to serch for the folder to caheck
     * for updates
     */
    axis2_status_t (AXIS2_CALL *
    check_modules) (axis2_repos_listener_t *listener,
                                        axis2_env_t **env);
    
    /**
     * this method ask serachWS to serch for the folder to caheck
     * for updates
     */
    axis2_status_t (AXIS2_CALL *
    check_svcs) (axis2_repos_listener_t *listener,
                                        axis2_env_t **env);
    
    /**
     * call to update method of WSInfoList object
     */
    axis2_status_t (AXIS2_CALL *
    update) (axis2_repos_listener_t *listener,
                                        axis2_env_t **env);
    
    /**
     * First it call to initalize method of WSInfoList to initilizat that.
     * then it call to checkModules to load all the module.jar s
     * and then it call to update() method inorder to update the Deployment engine and
     * engine regsitry
     */
    axis2_status_t (AXIS2_CALL *
    init) (axis2_repos_listener_t *listener,
                                        axis2_env_t **env);
    
    /**
     * this is the actual method that is call from scheduler
     */
    axis2_status_t (AXIS2_CALL *
    start_listen) (axis2_repos_listener_t *listener,
                                        axis2_env_t **env);
    

};

/** 
 * @brief Repos Listener struct 
 */  
AXIS2_DECLARE_DATA struct axis2_repos_listener
{
	axis2_repos_listener_ops_t *ops;
};

/**
 * @return pointer to newly created deployment engine
 */
AXIS2_DECLARE(axis2_repos_listener_t *) 
axis2_repos_listener_create (axis2_env_t **env);

/**
 * @param folder_name
 * @param dep_engine
 * @return pointer to newly created deployment engine
 */
AXIS2_DECLARE(axis2_repos_listener_t *) 
axis2_repos_listener_create_with_folder_name_and_dep_engine(axis2_env_t **env,
                                                axis2_char_t *folder_name,
                                                struct axis2_dep_engine *dep_engine);

/*************************** Function macros **********************************/

#define AXIS2_REPOS_LISTENER_FREE(repos_listener, env) \
		((repos_listener->ops)->free (repos_listener, env))

#define AXIS2_REPOS_LISTENER_CHECK_MODULES(repos_listener, env) \
		((repos_listener->ops)->check_modules (repos_listener, env))
        
#define AXIS2_REPOS_LISTENER_CHECK_SVCS(repos_listener, env) \
		((repos_listener->ops)->check_svcs (repos_listener, env))

#define AXIS2_REPOS_LISTENER_UPDATE(repos_listener, env) \
		((repos_listener->ops)->update (repos_listener, env))
        
#define AXIS2_REPOS_LISTENER_INIT(repos_listener, env) \
		((repos_listener->ops)->init (repos_listener, env))

#define AXIS2_REPOS_LISTENER_START_LISTEN(repos_listener, env) \
		((repos_listener->ops)->start_listen (repos_listener, env)) 

        

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_REPOS_LISTENER_H */
