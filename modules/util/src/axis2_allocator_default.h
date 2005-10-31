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

#ifndef AXIS2_ALLOCATOR_DEFAULT_H
#define AXIS2_ALLOCATOR_DEFAULT_H

/**
 * @file axis2_allocator_default.h
 * @brief Axis2 default memory allocator interface
 */

#include <axis2_defines.h>

#include <axis2_allocator.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_allocator Default Memory Allocator
 * @ingroup axis2_util 
 * @{
 */

  /**
    * Initializes (creates) an allocator.
    * @param allocator user defined allcator. Optional, can be NULL. If NULL, a default allocator will be returned.
    * @return initialized allocator. NULL on error.
    */
    AXIS2_DECLARE(axis2_allocator_t *) axis2_allocator_init (axis2_allocator_t * allocator);

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_ALLOCATOR_DEFAULT_H */
