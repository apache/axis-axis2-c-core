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

#ifndef AXIS2_WSDL11_MEP_FINDER_H
#define AXIS2_WSDL11_MEP_FINDER_H

/**
 * @file axis2_WSDL11_MEP_FINDER.h
 * @brief Axis2 Wsdl11 Mep Finder interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_WSDL11_MEP_FINDER Wsdl11 Mep Finder
 * @ingroup axis2_wsdl
 * @{
 */
const axis2_char_t *AXIS2_CALL
axis2_wsdl11_mep_finder_get_mep(void *op,
                          const axis2_env_t *env);

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL11_MEP_FINDER_H */
