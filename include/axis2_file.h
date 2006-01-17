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

#ifndef AXIS2_FILE_H
#define AXIS2_FILE_H

#include <axis2_defines.h>
#include <axis2_error.h>
#include <axis2_env.h>
#include <axis2.h>


#ifdef __cplusplus
extern "C"
{
#endif
    
typedef struct axis2_file axis2_file_t;
    
/**
 * @defgroup axis2_file File
 * @ingroup axis2_util 
 * @{
 */
    
    /**
     * create new file
     * @return file newly created file
     */
    AXIS2_DECLARE(axis2_file_t *)
    axis2_file_create(axis2_env_t **env);

    AXIS2_DECLARE(axis2_status_t)
    axis2_file_free (axis2_file_t *file, 
                        axis2_env_t **env); 

    /**
     * create a newly allocated clone of the argument file
     */
    axis2_file_t *AXIS2_CALL
    axis2_file_clone(axis2_file_t *file,
                      axis2_env_t **env);
    
    /** 
     * @brief
     * To store filename and timestamp of each module or service archive file
     */ 
    struct axis2_file
    {
        axis2_char_t *name;
        AXIS2_TIME_T time_stamp;
        
        
    };


/** @} */
    
#ifdef __cplusplus
}
#endif

#endif  /* AXIS2_FILE_H */
