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

#ifndef AXIS2_OM_TEXT_H
#define AXIS2_OM_TEXT_H

/**
 * @file axis2_om_text.h
 * @brief Axis2 OM XML text
 */

#include <axis2_environment.h>
#include <axis2_om_node.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup axis2_om AXIOM (Axis Object Model)
  * @ingroup axis2
  * @{
  */

/** @} */ 

/**
 * @defgroup axis2_om_text OM Text
 * @ingroup axis2_om 
 * @{
 */

/** @cond */
struct axis2_om_text;
struct axis2_om_text_ops;
/** @endcond */
    
/** @struct axis2_om_text_ops
    @brief OM text operations struct

    Encapsulator struct for operations of axis2_om_text_t
*/
typedef struct axis2_om_text_ops
{
  /**
	* Free an axis2_om_text_t structure
    * @return Status code
    */
    int (*free)(axis2_environment_t *environment, struct axis2_om_text *om_text);
    
  /**
    * Serialize operation
    * @param om_output OM output handler to be used in serializing
    * @return Status code
    */
    int (*serialize)(axis2_environment_t *environment, const struct axis2_om_text *om_text, axis2_om_output_t* om_output);
} axis2_om_text_ops_t;
    
/** \struct axis2_om_text
    \brief OM Text struct

    Handles the XML text in OM
*/
typedef struct axis2_om_text
{
  /**
    * OM text related operations
    */
    axis2_om_text_ops_t* ops;
    
   /**
	* Text value
	*/	
	axis2_char_t *value; 
	
   /**
    * The following fields are for MTOM
    */
	/*axis2_om_namespace_t *ns;*/
	axis2_char_t *mime_type;
	int optimize;
	axis2_char_t *localname;
	int is_binary;
	axis2_char_t *content_id;
	/*axis2_om_attribute_t *attribute;*/
} axis2_om_text_t;


/**
 * Create a text struct and stores it in a node struct and returns a pointer
 * to the newly created text struct
 * @param parent Parent of the new node. If null newly created node becomes a root node
 *          The parent element must be of type AXIS2_OM_ELEMENT
 * @param value Text value 
 * @param node Out parameter to store the newly created node
 * @return pointer to newly created text struct 
 */

axis2_om_text_t *axis2_om_text_create(axis2_environment_t *environment, axis2_om_node_t *parent,const axis2_char_t *value
						,axis2_om_node_t **node);

#define axis2_om_text_serialize(environment, om_text, om_output) ((om_text)->ops->serialize(environment, om_text, om_output))
#define axis2_om_text_free(environment, om_text) ((om_text)->ops->free(environment, om_text))

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_OM_TEXT_H */
