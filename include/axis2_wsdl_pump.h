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

#ifndef AXIS2_WSDL_PUMP_H
#define AXIS2_WSDL_PUMP_H

/**
 * @file axis2_wsdl_pump.h
 * @brief axis2 wsdl_pump interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>


#ifdef __cplusplus
extern "C"
{
#endif
	
typedef struct axis2_wsdl_pump_ops axis2_wsdl_pump_ops_t;
typedef struct axis2_wsdl_pump axis2_wsdl_pump_t;	
struct axis2_wsdl_desc;	

/** @defgroup axis2_wsdl_pump Wsdl Pump
  * @ingroup axis2_wsdl_pump
  * @{
  */

/** 
 * @brief Wsdl Pump ops struct
 * Encapsulator struct for ops of axis2_wsdl_pump
 */
struct axis2_wsdl_pump_ops
{
	/** Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (axis2_wsdl_pump_t *wsdl_pump,
                            const axis2_env_t *env);

	axis2_status_t (AXIS2_CALL *
	pump) (axis2_wsdl_pump_t *wsdl_pump,
						const axis2_env_t *env);

};

/** 
 * @brief Wsdl Pump struct
 *	Wsdl Pump  
 */ 
struct axis2_wsdl_pump
{
	axis2_wsdl_pump_ops_t *ops;
};

/**
 * Creates wsdl_pump struct
 * @return pointer to newly created wsdl_pump
 */
AXIS2_EXTERN axis2_wsdl_pump_t * AXIS2_CALL 
axis2_wsdl_pump_create (const axis2_env_t *env,
						struct axis2_wsdl_desc *wom_def,
						void *wsdl_parser);

/*************************** Function macros **********************************/

#define AXIS2_WSDL_PUMP_FREE(wsdl_pump, env) ((wsdl_pump)->ops->free (wsdl_pump, env))

#define AXIS2_WSDL_PUMP_PUMP(wsdl_pump, env) \
		((wsdl_pump)->ops->pump (wsdl_pump, env))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL_PUMP_H */
