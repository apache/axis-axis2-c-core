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

#ifndef AXIS2_RELATES_TO_H
#define AXIS2_RELATES_TO_H

/**
 * @file axis2_relates_to.h
 * @brief Axis2 core addressing interface
 */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_const.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_relates_to;
    struct axis2_relates_to_ops;
    
/**
 * @defgroup axis2_relates_to relates to
 * @ingroup axis2_addr
 * @{
 */

/**
 *   \brief Dispatcher ops struct
 */
 AXIS2_DECLARE_DATA typedef struct axis2_relates_to_ops
    { 
        /**
         * Method get_value
         *
         * @return
         */
        axis2_char_t* (AXIS2_CALL *
      get_value)(struct axis2_relates_to *relates_to, 
               const axis2_env_t *env);

        /**
         * Method set_value
         *
         * @param value
         */
        axis2_status_t (AXIS2_CALL *
      set_value)(struct axis2_relates_to *relates_to, 
                const axis2_env_t *env, 
                   axis2_char_t * value);

        /**
         * Method get_relationship_type
         *
         * @return
         */
        axis2_char_t* (AXIS2_CALL *
      get_relationship_type)(struct axis2_relates_to *relates_to, 
                            const axis2_env_t *env);

        /**
         * Method set_relationship_type
         *
         * @param relationship_type
         */
        axis2_status_t (AXIS2_CALL *
      set_relationship_type)(struct axis2_relates_to *relates_to, 
                            const axis2_env_t *env, 
                               axis2_char_t *relationship_type);
        
       /**
        * frees relates_to struct
        */
        
        axis2_status_t (AXIS2_CALL *
      free) (struct axis2_relates_to *relates_to, 
               const axis2_env_t *env);
        
    } axis2_relates_to_ops_t;
   
   /** 
    * \brief Dispatcher struct
    */
    typedef struct axis2_relates_to
    {
        /** Dispatcher related ops */
        axis2_relates_to_ops_t *ops;
    } axis2_relates_to_t;


/**
 * creates relates_to struct
 *
 * @param value
 * @param relationship_type
 */
AXIS2_EXTERN axis2_relates_to_t* AXIS2_CALL
axis2_relates_to_create(const axis2_env_t *env, 
                  const axis2_char_t *value, 
                  const axis2_char_t *relationship_type);
    
#define AXIS2_RELATES_TO_GET_VALUE(relates_to, env) \
      ((relates_to)->ops->get_value(relates_to, env))
   
#define AXIS2_RELATES_TO_SET_VALUE(relates_to, env, vlaue) \
      ((relates_to)->ops->set_value(relates_to, env, vlaue))
   
#define AXIS2_RELATES_TO_GET_RELATIONSHIP_TYPE(relates_to, env) \
      ((relates_to)->ops->get_relationship_type(relates_to, env))
      
#define AXIS2_RELATES_TO_SET_RELATIONSHIP_TYPE(relates_to, env, relationship_type) \
      ((relates_to)->ops->set_relationship_type(relates_to, env, relationship_type))
      
#define AXIS2_RELATES_TO_FREE(relates_to, env) \
      ((relates_to)->ops->free(relates_to, env))
    
/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_RELATES_TO_H */
