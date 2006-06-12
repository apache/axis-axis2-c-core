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

#ifndef AXIS2_GENERIC_OBJ_H
#define AXIS2_GENERIC_OBJ_H

#include <axis2_utils_defines.h>
#include <axis2_error.h>
#include <axis2_env.h>
#include <axis2_utils.h>


#ifdef __cplusplus
extern "C"
{
#endif
    
typedef struct axis2_generic_obj axis2_generic_obj_t;
typedef struct axis2_generic_obj_ops axis2_generic_obj_ops_t;
    
/**
 * @defgroup axis2_generic_obj Generic Object
 * @ingroup axis2_util 
 * @{
 */

/** 
 * @brief Description Builder ops struct
 * Encapsulator struct for ops of axis2_desc_builder
 */
AXIS2_DECLARE_DATA struct axis2_generic_obj_ops
{

    axis2_status_t (AXIS2_CALL *
    free) (
            axis2_generic_obj_t *generic_obj, 
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_free_func) (axis2_generic_obj_t *generic_obj,
                    const axis2_env_t *env,
                    AXIS2_FREE_VOID_ARG free_func);

    axis2_status_t (AXIS2_CALL *
    set_value) (axis2_generic_obj_t *generic_obj,
                const axis2_env_t *env,
                void *value);

    void *(AXIS2_CALL *
    get_value) (axis2_generic_obj_t *generic_obj,
                            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_type) (
            axis2_generic_obj_t *generic_obj,
            const axis2_env_t *env,
            int type);

    int (AXIS2_CALL *
    get_type) (
            axis2_generic_obj_t *generic_obj,
            const axis2_env_t *env);


};
    
     
/** 
     * @brief
     */ 
AXIS2_DECLARE_DATA struct axis2_generic_obj
{
   axis2_generic_obj_ops_t *ops;
};

/**
 * create new generic_obj
 * @return generic_obj newly created generic_obj
 */
AXIS2_EXTERN axis2_generic_obj_t * AXIS2_CALL
axis2_generic_obj_create(const axis2_env_t *env);

/*************************** Function macros **********************************/

#define AXIS2_GENERIC_OBJ_FREE(generic_obj, env) \
      (((axis2_generic_obj_t *) generic_obj)->ops->free (generic_obj, env))

#define AXIS2_GENERIC_OBJ_SET_FREE_FUNC(generic_obj, env, free_func) \
      ((generic_obj)->ops->set_free_func (generic_obj, env, free_func))  

#define AXIS2_GENERIC_OBJ_SET_VALUE(generic_obj, env, value) \
      ((generic_obj)->ops->set_value (generic_obj, env, value))

#define AXIS2_GENERIC_OBJ_GET_VALUE(generic_obj, env) \
        ((generic_obj)->ops->get_value(generic_obj, env))
 
#define AXIS2_GENERIC_OBJ_SET_TYPE(generic_obj, env, type) \
      ((generic_obj)->ops->set_type (generic_obj, env, type))

#define AXIS2_GENERIC_OBJ_GET_TYPE(generic_obj, env) \
        ((generic_obj)->ops->get_type(generic_obj, env))
                                                
/*************************** End of function macros ***************************/



/** @} */
    
#ifdef __cplusplus
}
#endif

#endif  /* AXIS2_GENERIC_OBJ_H */
