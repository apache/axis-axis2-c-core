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
#include <platforms/axis2_platform_auto_sense.h>


#ifdef __cplusplus
extern "C"
{
#endif
    
typedef struct axis2_file axis2_file_t;
typedef struct axis2_file_ops axis2_file_ops_t;
    
/**
 * @defgroup axis2_file File
 * @ingroup axis2_util 
 * @{
 */

/** 
 * @brief Description Builder ops struct
 * Encapsulator struct for ops of axis2_desc_builder
 */
AXIS2_DECLARE_DATA struct axis2_file_ops
{

    axis2_status_t (AXIS2_CALL *
    free) (axis2_file_t *file, 
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_name)(axis2_file_t *file,
                axis2_env_t **env,
                axis2_char_t *name);
    
    axis2_char_t *(AXIS2_CALL *
    get_name)(axis2_file_t *file,
                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_path)(axis2_file_t *file,
                axis2_env_t **env,
                axis2_char_t *path);
    
    axis2_char_t *(AXIS2_CALL *
    get_path)(axis2_file_t *file,
                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_timestamp)(axis2_file_t *file,
                    axis2_env_t **env,
                    AXIS2_TIME_T timestamp);
    
    AXIS2_TIME_T (AXIS2_CALL *
    get_timestamp)(axis2_file_t *file,
                    axis2_env_t **env);    

    /**
     * create a newly allocated clone of the argument file
     */
    axis2_file_t *(AXIS2_CALL *
    clone)(axis2_file_t *file,
                      axis2_env_t **env);
};
    
     
/** 
     * @brief
     * To store filename and timestamp of each module or service archive file
     */ 
AXIS2_DECLARE_DATA struct axis2_file
{
	axis2_file_ops_t *ops;
};

/**
 * create new file
 * @return file newly created file
 */
AXIS2_DECLARE(axis2_file_t *)
axis2_file_create(axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_FILE_FREE(file, env) \
		((file)->ops->free (file, env))

#define AXIS2_FILE_GET_NAME(file, env) \
		((file)->ops->get_name (file, env))  

#define AXIS2_FILE_SET_NAME(file, env, name) \
		((file)->ops->set_name (file, env, name)) 

#define AXIS2_FILE_GET_PATH(file, env) \
		((file)->ops->get_path (file, env))  

#define AXIS2_FILE_SET_PATH(file, env, path) \
		((file)->ops->set_path (file, env, path)) 

#define AXIS2_FILE_GET_TIMESTAMP(file, env) \
		((file)->ops->get_timestamp (file, env))  

#define AXIS2_FILE_SET_TIMESTAMP(file, env, timestamp) \
		((file)->ops->set_timestamp (file, env, timestamp)) 

#define AXIS2_FILE_CLONE(file, env) \
		((file)->ops->clone (file, env))


                                        
/*************************** End of function macros ***************************/



/** @} */
    
#ifdef __cplusplus
}
#endif

#endif  /* AXIS2_FILE_H */
