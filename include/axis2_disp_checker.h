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

#ifndef AXIS2_DISP_CHECKER_H
#define AXIS2_DISP_CHECKER_H

/**
 * @file axis2_disp_checker.h
 * @brief Axis2 Dispatcher interface
 */

#include <axis2_defines.h>
#include <axis2_qname.h>
#include <axis2_handler.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_disp_checker;
    struct axis2_disp_checker_ops;
    
/**
 * @defgroup axis2_disp_checker dispatcher checker
 * @ingroup axis2_engine
 * @{
 */

/**
 *   \brief Dispatcher ops struct
 */
 AXIS2_DECLARE_DATA   typedef struct axis2_disp_checker_ops
    { 
        axis2_handler_t* (AXIS2_CALL *
      get_base) (struct axis2_disp_checker *disp_checker, 
                   const axis2_env_t *env);
      
      
        axis2_qname_t* (AXIS2_CALL *
      get_qname) (struct axis2_disp_checker *disp_checker, 
                    const axis2_env_t *env);
      
        axis2_status_t (AXIS2_CALL *
      set_qname) (struct axis2_disp_checker *disp_checker, 
                    const axis2_env_t *env, 
               axis2_qname_t *qname);
      
        axis2_status_t (AXIS2_CALL *
      free) (struct axis2_disp_checker *disp_checker, 
               const axis2_env_t *env);
        
    } axis2_disp_checker_ops_t;
   
   /** 
    * \brief Dispatcher struct
    */
    typedef struct axis2_disp_checker
    {
        /** Dispatcher related ops */
        axis2_disp_checker_ops_t *ops;
    } axis2_disp_checker_t;


/**
 * creates disp_checker struct
 * @param qname qname, can be NULL
 */
AXIS2_EXTERN axis2_disp_checker_t* AXIS2_CALL
axis2_disp_checker_create(const axis2_env_t *env, 
                    axis2_qname_t *qname);

#define AXIS2_DISP_CHECKER_GET_BASE(disp_checker, env) \
      ((disp_checker)->ops->get_base(disp_checker, env))
   
#define AXIS2_DISP_CHECKER_GET_QNAME(disp_checker, env) \
      ((disp_checker)->ops->get_qname(disp_checker, env))
   
#define AXIS2_DISP_CHECKER_SET_QNAME(disp_checker, env, name) \
      ((disp_checker)->ops->set_qname(disp_checker, env, name))
   
#define AXIS2_DISP_CHECKER_FREE(disp_checker, env) \
      ((disp_checker)->ops->free(disp_checker, env))
      
    
/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_DISP_CHECKER_H */
