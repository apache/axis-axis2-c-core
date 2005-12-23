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

#ifndef AXIS2_ARCH_READER_H
#define AXIS2_ARCH_READER_H

/**
 * @file axis2_arch_reader.h
 * @brief Axis2 Arch Reader interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_conf.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_conf;
    
typedef struct axis2_arch_reader axis2_arch_reader_t;
typedef struct axis2_arch_reader_ops axis2_arch_reader_ops_t;

/** @defgroup axis2_arch_reader Arch Reader
 * @ingroup axis2_deployment
 * @{
 */

/** 
 * @brief Arch Reader ops struct
 * Encapsulator struct for ops of axis2_arch_reader
 */
AXIS2_DECLARE_DATA struct axis2_arch_reader_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *
    free)(axis2_arch_reader_t *arch_reader,
	        axis2_env_t **env);
    
 
                               
 

};

/** 
 * @brief Arch Reader struct 
 */  
AXIS2_DECLARE_DATA struct axis2_arch_reader
{
	axis2_arch_reader_ops_t *ops;
};

/**
 * Creates arch reader struct
 * @param repos_path
 * @return pointer to newly created arch reader
 */
AXIS2_DECLARE(axis2_arch_reader_t *) 
axis2_arch_reader_create (axis2_env_t **env);


/*************************** Function macros **********************************/

#define AXIS2_ARCH_READER_FREE(arch_reader, env) \
		((arch_reader->ops)->free (arch_reader, env))  



/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_ARCH_READER_H */
