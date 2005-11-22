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

#ifndef AXIS2_OM_ELEMENT_H
#define AXIS2_OM_ELEMENT_H

#include <axis2_om_namespace.h>
#include <axis2_om_attribute.h>
#include <axis2_om_output.h>
#include <axis2_om_node.h>
#include <axis2_hash.h>
#include <axis2.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_om_element;
    struct axis2_om_element_ops;

/**
 * @defgroup axis2_om_element OM Element
 * @ingroup axis2_om 
 * @{
 */


    /** 
    * @brief OM element operations struct
    * Encapsulator struct for operations of axis2_om_element
    */
AXIS2_DECLARE_DATA    typedef struct axis2_om_element_ops
    {
       /*
        * Find a namespace in the scope of the document.
        * Start to find from the given node and go up the hierarchy.
        * @param om_element pointer to om_element_struct contained in
        *        node , 
        * @param env Environment. MUST NOT be NULL.
        * @param node node containing an instance of an OM element,cannot be NULL.
        * @param uri namespace uri..
        * @param prefix namespace prefix. can be NULL.
        * @return pointer to the namespace, if found, else NULL. On error, returns 
        *           NULL and sets error code in environment,s error
        */
        axis2_om_namespace_t* (AXIS2_CALL *find_namespace)
                                        (struct axis2_om_element *om_element,
                                        axis2_env_t **env,
                                        axis2_om_node_t *node,
                                        const axis2_char_t *uri,
                                        const axis2_char_t *prefix);
      /**
        * Declare a namespace in current element (in the scope of this element ).
        * It checks to see if it is already declared.
        * @param om_element contained in the om node struct
        * @param env Environment. MUST NOT be NULL.
        * @param node node containing an instance of an OM element.
        * @param ns pointer to the namespace struct to be declared
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE.
        */
        axis2_status_t (AXIS2_CALL *declare_namespace)
                                        (struct axis2_om_element *om_element,
                                         axis2_env_t **env,
                                         axis2_om_node_t *node,
                                         axis2_om_namespace_t * ns);
       /**
        * Finds a namespace using qname
        * Start to find from the given node and go up the hierarchy.
        * @param om_element om_element contained in node
        * @param env Environment. MUST NOT be NULL.
        * @param node node containing an instance of an OM element, cannot be NULL.                                        
        * @param qname qname of the namespace to be found. cannot be NULL.
        * @return pointer to the namespace, if found, else NULL. On error, returns 
        *           NULL and sets the error code in environment's error struct.
        */
         axis2_om_namespace_t *(AXIS2_CALL *find_namespace_with_qname)
                                            (struct axis2_om_element *om_element,
                                             axis2_env_t **env,
                                             axis2_om_node_t *node,
                                             axis2_qname_t *qname);

      /**
        * Adds an attribute to current element
        * @param om_element element to which the attribute is to be added.cannot be NULL.
        * @param env Environment. MUST NOT be NULL.
        * @param attribute attribute to be added.
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE.
        */
         axis2_status_t (AXIS2_CALL *add_attribute)(struct axis2_om_element *om_element,
                                                    axis2_env_t **env,
                                                    axis2_om_attribute_t *attribute);

      /**
        * Gets (finds) the attribute with the given qname
        * @param element element whose attribute is to be found. 
        * @param env Environment. MUST NOT be NULL.
        * @qname qname qname of the attribute to be found. should not be NULL.
        * @return a pointer to the attribute with given qname if found, else NULL.
        *           On error, returns NULL and sets the error code in environment's error struct.
        */
         axis2_om_attribute_t *(AXIS2_CALL *get_attribute) (struct axis2_om_element *om_element,
                                                            axis2_env_t **env,
                                                            axis2_qname_t * qname);

      /**
        * Frees given element 
        * @param element OM element to be freed.
        * @param env Environment. MUST NOT be NULL.
         * @return satus of the operation. AXIS2_SUCCESS on success ,AXIS2_FAILURE on error.
        */
         axis2_status_t (AXIS2_CALL *free) (struct axis2_om_element *element,
                                            axis2_env_t **env);

      /**
        * Serializes the start part of the given element
        * @param element element to be serialized.
        * @param env Environment. MUST NOT be NULL.
        * @param om_output OM output handler to be used in serializing
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *serialize_start_part)
                                                (struct axis2_om_element *om_element,
                                                 axis2_env_t **env,
                                                 axis2_om_output_t * om_output);

      /**
        * Serializes the end part of the given element. serialize_start_part must 
        *     have been called before calling this method.
        * @param om_element element to be serialized.
        * @param env Environment must not be null.
        * @param om_output OM output handler to be used in serializing
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *serialize_end_part)
                                                    (struct axis2_om_element *om_element,
                                                     axis2_env_t **env,
                                                     axis2_om_output_t * om_output);
                                                        
       /**
        *   returns the localname of this element
        * @param om_element om_element struct
        * @param env environment struct
        * @returns localname of element, returns NULL on error.
        */        
        axis2_char_t* (AXIS2_CALL *get_localname)(struct axis2_om_element *om_element,
                                                  axis2_env_t **env);
       /**
        * set the localname of this element
        * @param om_element om_element_struct to with localname is set
        * @param env environment struct
        * @localname text value to be set as localname 
        * @returns status code of operation, AXIS2_SUCCESS on success,
        *                   AXIS2_FAILURE on error.
        */
        axis2_status_t (AXIS2_CALL *set_localname)(struct axis2_om_element *om_element,
                                                  axis2_env_t **env,
                                                  const axis2_char_t *localname);
        /**
         * get the namespace  of om_element 
         * @param om_element om_element struct
         * @param env environemt, MUST NOT be NULL.
         * @returns pointer to axis2_om_namespace_t struct 
         *          NULL if there is no namespace associated with the element,
         *          NULL on error with error code set to environment's error
         */
        axis2_om_namespace_t *(AXIS2_CALL *get_namespace)
                                                (struct axis2_om_element *om_element,
                                                 axis2_env_t **env);
       /**
        * set the namespace of the element
        * @param om_element Om_element struct
        * @param env environment must not be null
        * @param ns pointer to namespace
        * @returns status code of the operation , NULL on error with error code 
        *                  set to environment's error
        */                                         
        axis2_status_t (AXIS2_CALL *set_namespace)(struct axis2_om_element *om_element,
                                                   axis2_env_t **env,
                                                   axis2_om_namespace_t *ns,
                                                   axis2_om_node_t *node);                                                                                                                                                              
    } axis2_om_element_ops_t;

    
  /**
    * \brief OM element struct
    * Handles the XML element in OM
    */
    typedef struct axis2_om_element
    {
        /** operations of attribute struct */
        axis2_om_element_ops_t *ops;

    } axis2_om_element_t;

  /**
    * Creates an OM element with given local name
    * @param env Environment. MUST NOT be NULL.
    * @param parent parent of the element node to be created. can be NULL.
    * @param localname local name of the elment. cannot be NULL.
    * @param ns namespace of the element.  can be NULL.
    * @param node This is an out parameter. cannot be NULL.
    *                       Returns the node corresponding to the comment created.
    *                       Node type will be set to AXIS2_OM_ELEMENT
    * @return a pointer to the newly created element struct
    */
    AXIS2_DECLARE(axis2_om_element_t *)
    axis2_om_element_create (axis2_env_t **env,
                             axis2_om_node_t *parent,
                             const axis2_char_t *localname,
                             axis2_om_namespace_t *ns,
                             axis2_om_node_t **node);

  /**
    * Creates an OM element with given qname
    * @param env Environment. MUST NOT be NULL.
    * @param parent parent of the element node to be created. can be NULL.
    * @param qname qname of the elment.cannot be NULL.
    * @param node This is an out parameter. cannot be NULL.
    *                       Returns the node corresponding to the comment created.
    *                       Node type will be set to AXIS2_OM_ELEMENT
    * @return a pointer to the newly created element struct
    */
    AXIS2_DECLARE(axis2_om_element_t *) 
    axis2_om_element_create_with_qname (axis2_env_t **env,
                                        axis2_om_node_t *parent,
                                        axis2_qname_t *qname,
                                        axis2_om_node_t ** node);

/** finds given namespace with respect to given node */
#define AXIS2_OM_ELEMENT_FIND_NAMESPACE(om_element,env,om_node, uri, prefix) \
        ((om_element)->ops->find_namespace (om_element, env,om_node, uri, prefix))
        
/** declares given namespace with respect to the given node */
#define AXIS2_OM_ELEMENT_DECLARE_NAMESPACE(om_element, env , om_node, ns) \
        ((om_element)->ops->declare_namespace (om_element, env, om_node, ns))

/** finds namespace with given qname */
#define AXIS2_OM_ELEMENT_FIND_NAMESPACE_WITH_QNAME (om_element, env, om_node, qname) \
        ((om_element)->ops->find_namespace_with_qname (om_element, environment, node, qname))
        
/** adds given attribute to given element */
#define AXIS2_OM_ELEMENT_ADD_ATTRIBUTE(om_element, env, attribute) \
        ((om_element)->ops->add_attribute(om_element , env, attribute))
        
/** gets (finds) given attribute in given element */
#define AXIS2_OM_ELEMENT_GET_ATTRIBUTE(om_element, env, qname) \
        ((om_element)->ops->get_attribute(om_element, env, qname))
        
/** free an om element */
#define AXIS2_OM_ELEMENT_FREE(om_element, env) \
        ((om_element)->ops->free(om_element, env))
        
/** serialize the start part of given element */
#define AXIS2_OM_ELEMENT_SERIALIZE_START_PART(om_element, env, om_output) \
        ((om_element)->ops->serialize_start_part(om_element, env, om_output))
        
/** serialize the end part of given element */
#define AXIS2_OM_ELEMENT_SERIALIZE_END_PART(om_element, env, om_output) \
        ((om_element)->ops->serialize_end_part(om_element, env, om_output))
/** get localname */
#define AXIS2_OM_ELEMENT_GET_LOCALNAME(om_element, env)\
        ((om_element)->ops->get_localname(om_element,env))
/** get namespace of this element */        
#define AXIS2_OM_ELEMENT_GET_NAMESPACE(om_element, env) \
        ((om_element)->ops->get_namespace(om_element, env))
/** set localname */        
#define AXIS2_OM_ELEMENT_SET_LOCALNAME(om_element, env, localname) \
        ((om_element)->ops->set_localname(om_element, env))
/** set namespace */        
#define AXIS2_OM_ELEMENT_SET_NAMESPACE(om_element, env, ns , node) \
        ((om_element)->ops->set_namespace(om_element, env, ns, node))
        
/** @} */


#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_ELEMENT_H */
