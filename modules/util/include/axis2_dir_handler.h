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

#ifndef AXIS2_DIR_HANDLER_H
#define AXIS2_DIR_HANDLER_H

#include <axis2_utils_defines.h>
#include <axis2_error.h>
#include <axis2_env.h>
#include <axis2_string.h>
#include <axis2_array_list.h>
#include <axis2_utils.h>


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_dir_handler Dir Handler
 * @ingroup axis2_util 
 * @{
 */
    /**
     * List the dll files in the given service or module folder path
     * @param pathname path to your service or module directory
     * @return array list of dll file names
     */
    AXIS2_DECLARE(axis2_array_list_t *)
    axis2_dir_handler_list_services_or_modules_in_dir(axis2_env_t **env,
                                axis2_char_t *pathname); 
    
    /**
     * List services or modules directories in the services or modules folder
     * respectively
     * @param pathname path  your modules or services folder
     * @return array list of contents of services or modules folder
     */
    AXIS2_DECLARE(axis2_array_list_t *)
    axis2_dir_handler_list_service_or_module_dirs(axis2_env_t **env,
                                axis2_char_t *pathname);
                                
/***********************Function Macros****************************************/

#define AXIS2_DIR_HANDLER_LIST_SERVICES_OR_MODULES_IN_DIR(env, pathname) \
        (axis2_dir_handler_list_services_or_modules_in_dir(env, pathname))
        
#define AXIS2_DIR_HANDLER_LIST_SERVICE_OR_MODULE_DIRS(env, pathname) \
        (axis2_dir_handler_list_service_or_module_dirs(env, pathname))        

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_DIR_HANDLER_H */
