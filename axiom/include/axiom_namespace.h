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

#ifndef AXIOM_NAMESPACE_H
#define AXIOM_NAMESPACE_H

/**
 * @file axiom_namespace.h
 * @brief defines axiom_namespace data structure,
 *        used for representing XML namespaces in OM
 */

#include <axis2_utils_defines.h>
#include <axis2_env.h>
#include <axiom_output.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axiom_namespace;
    struct axiom_namespace_ops;
    
/**
 * @defgroup axiom_namespace namespace
 * @ingroup axiom_om
 * @{
 */

  /** 
    * \brief namespace ops struct
    * Encapsulator struct for ops of axiom_namespace
    */
    typedef struct axiom_namespace_ops
    {
      /**
        * Frees given AXIOM namespcae
        * @param om_namespace namespace to be freed.
        * @param env Environment. MUST NOT be NULL.
        * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE.
        */
        axis2_status_t (AXIS2_CALL *
        free) (struct axiom_namespace *om_namespace,
               const axis2_env_t *env);

      /**
        * Compares two namepsaces
        * @param om_namespace first namespase to be compared
        * @param env Environment. MUST NOT be NULL.
        * @param om_namespace1 second namespace to be compared
        * @return AXIS2_TRUE if the two namespaces are equal,AXIS2_FALSE otherwise
        */
        axis2_bool_t (AXIS2_CALL *
        equals)(struct axiom_namespace *om_namespace,
                const axis2_env_t *env,
                struct axiom_namespace *om_namespace1);

      /**
        * Serializes given namespace 
        * @param om_namespace namespace to be serialized.
        * @param env Environment. MUST NOT be NULL.
        * @param om_output AXIOM output handler to be used in serializing
        * @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE.
        */
        axis2_status_t (AXIS2_CALL *
        serialize)(struct axiom_namespace *om_namespace,
                   const axis2_env_t *env,
                   axiom_output_t * om_output);
       /**   
        *@param om_namespace pointer to om_namespace struct
        *@param env environment , MUST NOT be NULL.
        *@returns namespace uri , NULL on error
        */
        axis2_char_t* (AXIS2_CALL *
        get_uri)(struct axiom_namespace *om_namespace,
                 const axis2_env_t *env);
       /**   
        *@param om_namespace pointer to om namespace struct
        *@param env  environment, MUST NOT be NULL
        *@return prefix , NULL on error
        */
        axis2_char_t* (AXIS2_CALL *
        get_prefix)(struct axiom_namespace *om_namespace,
                    const axis2_env_t *env);


        /**
         * clones an om_namespace struct
         * @param om_namespace pointer to namespace struct
         * @param env environment
         * @returns axiom_namespace on success , NULL on error
         */
        struct axiom_namespace* (AXIS2_CALL *
        clone)(struct axiom_namespace *om_namespace,
               const axis2_env_t *env);
               
        /**
         * to string , returns the string by combining namespace_uri,
         * and prefix seperated by a '|' character
         * @param om_namespace 
         * @param env environment
         * @returns pointer to string , This is a property of namespace,
         * should not be freed by user
         */                                                                                     
        axis2_char_t * (AXIS2_CALL *
        to_string)(struct axiom_namespace *om_namespace,
                   const axis2_env_t *env);

    } axiom_namespace_ops_t;

  /** 
    * \brief namespace struct
    * Handles the XML namespace in AXIOM
    */
    typedef struct axiom_namespace
    {
        /** AXIOM namespace related ops */
        axiom_namespace_ops_t *ops;
        int ref;        
    } axiom_namespace_t;

  /**
    * Creates a namespace struct
    * @param uri namespace URI
    * @param prefix namespace prefix
    * @return a pointer to newly created namespace struct
    */
    AXIS2_EXTERN axiom_namespace_t * AXIS2_CALL
    axiom_namespace_create (const axis2_env_t *env,
                               const axis2_char_t * uri,
                               const axis2_char_t *prefix);

/** frees given namespace */
#define AXIOM_NAMESPACE_FREE(om_namespace,env) \
        ((om_namespace)->ops->free(om_namespace, env))
        
/** compares the given two namespaces for equality */
#define AXIOM_NAMESPACE_EQUALS(om_namespace, env, om_namespace1) \
        ((om_namespace)->ops->equals(om_namespace, env, om_namespace1))
        
/** serializes given namespace */
#define AXIOM_NAMESPACE_SERIALIZE(om_namespace,env, om_output) \
        ((om_namespace)->ops->serialize(om_namespace, env,om_output))
/** get prefix */        
#define AXIOM_NAMESPACE_GET_PREFIX(om_namespace, env) \
        ((om_namespace)->ops->get_prefix(om_namespace, env))
/** get namespace uri */
#define AXIOM_NAMESPACE_GET_URI(om_namespace, env) \
        ((om_namespace)->ops->get_uri(om_namespace, env))
/** clones a namespace */
#define AXIOM_NAMESPACE_CLONE(om_namespace, env) \
        ((om_namespace)->ops->clone(om_namespace, env))
/** returns a string from namespace */
#define AXIOM_NAMESPACE_TO_STRING(om_namespace, env) \
        ((om_namespace)->ops->to_string(om_namespace, env))

/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIOM_NAMESPACE */
