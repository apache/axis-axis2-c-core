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

#ifndef AXIS2_ANY_CONTENT_TYPE_H
#define AXIS2_ANY_CONTENT_TYPE_H

/**
 * @file axis2_any_content_type.h
 * @brief Axis2 core addressing interface
 */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2.h>
#include <axis2_hash.h>
#include <axis2_qname.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_any_content_type;
    struct axis2_any_content_type_ops;
    
/**
 * @defgroup axis2_any_content_type Addressing relates to header
 * @ingroup axis2_addr
 * @{
 */

/**
 *   \brief Dispatcher ops struct
 */
 AXIS2_DECLARE_DATA typedef struct axis2_any_content_type_ops
    { 
        /**
         * Method addReferenceValue
         *
         * @param name
         * @param value
         */
        axis2_status_t (AXIS2_CALL *add_value)(struct axis2_any_content_type *any_content_type, axis2_env_t **env, axis2_qname_t *qname, axis2_char_t *value); 
        /**
         * Method getReferenceValue
         *
         * @param qname
         * @return
         */
        axis2_char_t* (AXIS2_CALL *get_value)(struct axis2_any_content_type *any_content_type, axis2_env_t **env, axis2_qname_t *qname);
        axis2_hash_t* (AXIS2_CALL *get_value_map)(struct axis2_any_content_type *any_content_type, axis2_env_t **env);
        axis2_status_t (AXIS2_CALL *free)(struct axis2_any_content_type *any_content_type, 
                                                       axis2_env_t **env);
    } axis2_any_content_type_ops_t;
	
   /** 
    * \brief Dispatcher struct
    */
    typedef struct axis2_any_content_type
    {
        /** Dispatcher related ops */
        axis2_any_content_type_ops_t *ops;
    } axis2_any_content_type_t;


/**
 * creates any_content_type struct
 *
 * @param value
 * @param relationship_type
 */
AXIS2_DECLARE(axis2_any_content_type_t*) axis2_any_content_type_create(axis2_env_t **env);
    
#define AXIS2_ANY_CONTENT_TYPE_ADD_VALUE(any_content_type, env, qname, value) ((any_content_type)->ops->add_value(any_content_type, env, qname, value))
#define AXIS2_ANY_CONTENT_TYPE_GET_VALUE(any_content_type, env, qname) ((any_content_type)->ops->get_value(any_content_type, env, qname))
#define AXIS2_ANY_CONTENT_TYPE_GET_VALUE_MAP(any_content_type, env) ((any_content_type)->ops->get_value_map(any_content_type, env))
#define AXIS2_ANY_CONTENT_TYPE_FREE(any_content_type, env) ((any_content_type)->ops->free(any_content_type, env))

    
/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_ANY_CONTENT_TYPE_H */
