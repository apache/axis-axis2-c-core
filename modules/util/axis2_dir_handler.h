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

#include <axis2_defines.h>
#include <axis2_error.h>
#include <axis2_env.h>
#include <axis2_string.h>
#include <axis2_array_list.h>
#include <axis2.h>


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
     * List the .zip or tar.gz files in the given path
     * @param pathname path to the directory where your modules or services are
     * @return array list of .zip or tar.gz file names
     */
    AXIS2_DECLARE(axis2_array_list_t *)
    axis2_dir_handler_list_dir(axis2_env_t **env,
                                axis2_char_t *pathname); 
    
    /** 
     * @brief
     * To store filename and timestamp of each module or service archive file
     */ 
    typedef struct axis2_arch_file
    {
        axis2_char_t *name;
        time_t time_stamp;
        
    } axis2_arch_file_t;

#define AXIS2_LIST_DIR(env, pathname) axis2_list_dir(env, pathname)

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_DIR_HANDLER_H */
