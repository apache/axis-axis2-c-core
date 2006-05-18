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

#ifndef AXIS2_PROPERTY_H
#define AXIS2_PROPERTY_H

#include <axis2_defines.h>
#include <axis2_error.h>
#include <axis2_env.h>
#include <axis2_const.h>


#ifdef __cplusplus
extern "C"
{
#endif
    
typedef struct axis2_property axis2_property_t;
typedef struct axis2_property_ops axis2_property_ops_t;
    
/**
 * @defgroup axis2_property Property
 * @ingroup axis2_util 
 * @{
 */

/** 
 * @brief Description Builder ops struct
 * Encapsulator struct for ops of axis2_desc_builder
 */
AXIS2_DECLARE_DATA struct axis2_property_ops
{

    axis2_status_t (AXIS2_CALL *
    free) (axis2_property_t *property, 
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_scope) (axis2_property_t *property,
                axis2_env_t **env,
                axis2_scope_t scope);

    axis2_status_t (AXIS2_CALL *
    set_free_func) (axis2_property_t *property,
                    axis2_env_t **env,
                    AXIS2_FREE_VOID_ARG free_func);

    axis2_status_t (AXIS2_CALL *
    set_value) (axis2_property_t *property,
                axis2_env_t **env,
                void *value);
    void *(AXIS2_CALL *
    get_value) (axis2_property_t *property,
                            axis2_env_t **env);

};
    
     
/** 
     * @brief
     */ 
AXIS2_DECLARE_DATA struct axis2_property
{
	axis2_property_ops_t *ops;
};

/**
 * create new property
 * @return property newly created property
 */
AXIS2_DECLARE(axis2_property_t *)
axis2_property_create(axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_PROPERTY_FREE(property, env) \
		((property)->ops->free (property, env))

#define AXIS2_PROPERTY_SET_FREE_FUNC(property, env, free_func) \
		((property)->ops->set_free_func (property, env, free_func))  

#define AXIS2_PROPERTY_SET_SCOPE(property, env, scope) \
		((property)->ops->set_scope (property, env, scope)) 

#define AXIS2_PROPERTY_SET_VALUE(property, env, value) \
		((property)->ops->set_value (property, env, value))

#define AXIS2_PROPERTY_GET_VALUE(property, env) \
        ((property)->ops->get_value(property, env))
                                        
/*************************** End of function macros ***************************/



/** @} */
    
#ifdef __cplusplus
}
#endif

#endif  /* AXIS2_PROPERTY_H */
