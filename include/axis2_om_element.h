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
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param node node containing an instance of an OM element. Mandatory, cannot be NULL.
        * @param uri namespace uri. Mandatory, cannot be NULL.
        * @param prefix namespace prefix. Optional, can be NULL.
        * @return pointer to the namespace, if found, else NULL. On error, returns 
        *           NULL and sets the errorno in environment.
        */
        axis2_om_namespace_t *
            (AXIS2_CALL *axis2_om_element_ops_find_namespace) (axis2_env_t *
                                                     environment,
                                                     axis2_om_node_t * node,
                                                     const axis2_char_t * uri,
                                                     const axis2_char_t *
                                                     prefix);
      /**
        * Declare a namespace in current element (in the scope of this element ).
        * It checks to see if it is already declared.
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param node node containing an instance of an OM element. Mandatory, cannot be NULL.
        * @param ns pointer to the namespace struct to be declared
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE.
        */
        axis2_status_t (AXIS2_CALL *axis2_om_element_ops_declare_namespace)
            (axis2_env_t * environment, axis2_om_node_t *node,
             axis2_om_namespace_t * ns);

      /**
        * find namespaces in the scope of current element
        * @param environment Environment .MUST NOT be NULL, if NULL behaviour is undefined.
        * @param element pointer to an om element
        * @param prefix namespace prefix
        * @param uri    namespace uri
        */
        /*   AXIS2_DECLARE_DATA axis2_om_namespace_t
            *(AXIS2_CALL *axis2_om_element_ops_find_declared_namespace)
            (axis2_env_t * environment,
             struct axis2_om_element * element, const axis2_char_t * uri,
             const axis2_char_t * prefix);
             */

      /**
        * Finds a namespace using qname
        * Start to find from the given node and go up the hierarchy.
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param node node containing an instance of an OM element. Mandatory, cannot be NULL.
        * @param qname qname of the namespace to be found. Mandatory, cannot be NULL.
        * @return pointer to the namespace, if found, else NULL. On error, returns 
        *           NULL and sets the errorno in environment.
        */
         axis2_om_namespace_t *
            (AXIS2_CALL *axis2_om_element_ops_find_namespace_with_qname)
            (axis2_env_t *environment, axis2_om_node_t *node,
             axis2_qname_t *qname);

      /**
        * Adds an attribute to current element
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param element element to which the attribute is to be added. Mandatory, cannot be NULL.
        * @param attribute attribute to be added.
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE.
        */
         axis2_status_t (AXIS2_CALL *axis2_om_element_ops_add_attribute)
            (axis2_env_t * environment,
             struct axis2_om_element * element,
             axis2_om_attribute_t * attribute);

      /**
        * Gets (finds) the attribute with the given qname
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param element element whose attribute is to be found. Mandatory, cannot be NULL.
        * @qname qname qname of the attribute to be found. Mandatory, cannot be NULL.
        * @return a pointer to the attribute with given qname if found, else NULL.
        *           On error, returns NULL and sets the error.
        */
         axis2_om_attribute_t *
            (AXIS2_CALL *axis2_om_element_ops_get_attribute) (axis2_env_t *
                                                    environment,
                                                    struct axis2_om_element *
                                                    element,
                                                    axis2_qname_t * qname);

      /**
        * Frees given element 
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param element OM element to be freed.
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE.
        */
         axis2_status_t (AXIS2_CALL *axis2_om_element_ops_free) (axis2_env_t *
                                                       environment,
                                                       struct axis2_om_element
                                                       * element);

      /**
        * Serializes the start part of the given element
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param element element to be serialized.
        * @param om_output OM output handler to be used in serializing
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *axis2_om_element_ops_serialize_start_part)
            (axis2_env_t * environment,
             struct axis2_om_element * element,
             axis2_om_output_t * om_output);

      /**
        * Serializes the end part of the given element. serialize_start_part must 
        *     have been called before calling this method.
        * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
        * @param element element to be serialized.
        * @param om_output OM output handler to be used in serializing
        * @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *axis2_om_element_ops_serialize_end_part)
            (axis2_env_t * environment,
             struct axis2_om_element * element,
             axis2_om_output_t * om_output);

    } axis2_om_element_ops_t;

    
  /**
    * \brief OM element struct
    * Handles the XML element in OM
    */
    typedef struct axis2_om_element
    {
        /** operations of attribute struct */
        axis2_om_element_ops_t *ops;
        /** Element's namespace */
        axis2_om_namespace_t *ns;
        /** Element's local name */
        axis2_char_t *localname;
        /** List of attributes */
        axis2_hash_t *attributes;
        /** List of namespaces */
        axis2_hash_t *namespaces;
    } axis2_om_element_t;

  /**
    * Creates an OM element with given local name
    * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
    * @param parent parent of the element node to be created. Optional, can be NULL.
    * @param localname local name of the elment. Mandatory, cannot be NULL.
    * @param ns namespace of the element. Optional, can be NULL.
    * @param node This is an out parameter. Mandatory, cannot be NULL.
    *                       Returns the node corresponding to the comment created.
    *                       Node type will be set to AXIS2_OM_ELEMENT
    * @return a pointer to the newly created element struct
    */
    AXIS2_DECLARE(axis2_om_element_t *) axis2_om_element_create (axis2_env_t *
                                                 environment,
                                                 axis2_om_node_t * parent,
                                                 const axis2_char_t *
                                                 localname,
                                                 axis2_om_namespace_t * ns,
                                                 axis2_om_node_t ** node);

  /**
    * Creates an OM element with given qname
    * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
    * @param parent parent of the element node to be created. Optional, can be NULL.
    * @param qname qname of the elment. Mandatory, cannot be NULL.
    * @param node This is an out parameter. Mandatory, cannot be NULL.
    *                       Returns the node corresponding to the comment created.
    *                       Node type will be set to AXIS2_OM_ELEMENT
    * @return a pointer to the newly created element struct
    */
    AXIS2_DECLARE(axis2_om_element_t *) 
        axis2_om_element_create_with_qname (axis2_env_t *
                                             environment,
                                             axis2_om_node_t * parent,
                                             axis2_qname_t * qname,
                                             axis2_om_node_t ** node);

/** finds given namespace with respect to given node */
#define axis2_om_element_find_namespace(environment, node, uri, prefix) (((axis2_om_element_t*)(node->data_element))->ops->axis2_om_element_ops_find_namespace(environment, node, uri, prefix))
/** declares given namespace with respect to the given node */
#define axis2_om_element_declare_namespace(environment, node, ns) (((axis2_om_element_t*)(node->data_element))->ops->axis2_om_element_ops_declare_namespace(environment, node, ns) )
/** finds namespace with given qname */
#define axis2_om_element_find_namespace_with_qname (environment, node, qname) ((element)->ops->axis2_om_element_ops_find_namespace_with_qname (environment, node, qname))
/** adds given attribute to given element */
#define axis2_om_element_add_attribute(environment, element, attribute) ((element)->ops->axis2_om_element_ops_add_attribute(environment, element, attribute))
/** gets (finds) given attribute in given element */
#define axis2_om_element_get_attribute(environment, element, qname) ((element)->ops->axis2_om_element_ops_get_attribute(environment, element, qname))
/*#define axis2_om_element_find_declared_namespace(environment,element,uri,prefix) ((element)->ops->axis2_om_element_find_declared_namespace(environment,element,uri,prefix))*/
/** frees given element */
#define axis2_om_element_free(environment, element) ((element)->ops->axis2_om_element_ops_free(environment, element))
/** serialize the start part of given element */
#define axis2_om_element_serialize_start_part(environment, element, om_output) ((element)->ops->axis2_om_element_ops_serialize_start_part(environment, element, om_output))
/** serialize the end part of given element */
#define axis2_om_element_serialize_end_part(environment, element, om_output) ((element)->ops->axis2_om_element_ops_serialize_end_part(environment, element, om_output))

/** @} */


#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_ELEMENT_H */
