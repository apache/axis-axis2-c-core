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

#ifndef AXIS2_LOG_DEFAULT_H
#define AXIS2_LOG_DEFAULT_H

#include <stdlib.h>
#include <stdarg.h>
#include <axis2_log.h>
#include <axis2_allocator.h>


#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @defgroup axis2_log Log
 * @ingroup axis2_util 
 * @{
 */

#define AXIS2_LEN_VALUE 6000


  /**
    * Creates a log struct
    * @param allocator allocator to be used. Mandatory, cannot be NULL    
    * @return pointer to the newly created log struct 
    */
    AXIS2_DECLARE(axis2_log_t *) axis2_log_create (axis2_allocator_t * allocator,
                     axis2_log_ops_t * ops, axis2_char_t * stream_name);

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_LOG_DEFAULT_H */
