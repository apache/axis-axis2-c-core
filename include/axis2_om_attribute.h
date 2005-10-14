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

#ifndef AXIS2_OM_ATTRIBUTE_H
#define AXIS2_OM_ATTRIBUTE_H

/**
* @file axis2_om_attribute.h
* @brief om attribute struct represents an xml attribute
*/

#include <axis2_qname.h>
#include <axis2_om_namespace.h>
#include <axis2_om_output.h>



struct axis2_om_attribute;
struct axis2_om_attribute_ops;

/**  \struct axis2_om_attribute
 *   \brief OM attribute operations struct
 *
 *   Encapsulator struct for axis2_om_attribute_t
 */
typedef struct axis2_om_attribute_ops
{
   /**
    *  Free an axis2_om_attribute struct
    *  @return Status code
    */

    axis2_status_t (*axis2_om_attribute_ops_free)(axis2_environment_t *environment, struct axis2_om_attribute *om_attribute);

   /** 
    *  Creates and returns a qname struct for this attribute
    *  @param om_attribute
    *  @return returns null on error 
    */

    axis2_qname_t *(*axis2_om_attribute_ops_get_qname)(axis2_environment_t *environment,struct axis2_om_attribute *om_attribute);

   /**
    * Serialize operation
    * @param om_output OM output handler to be used in serializing
    * @return Status code
    */

    int (*axis2_om_attribute_ops_serialize)(axis2_environment_t *environment,struct axis2_om_attribute *om_attribute,axis2_om_output_t *om_output);

}axis2_om_attribute_ops_t;



typedef struct axis2_om_attribute 
{
    /** operations of attributes */
    axis2_om_attribute_ops_t *ops;

    /** localname of this attribute  */
    axis2_char_t *localname;
    /** value of this attribute */
    axis2_char_t *value;
    /** attribute namespace */
    axis2_om_namespace_t *ns;
}axis2_om_attribute_t;

/**
 * creates an om_attribute structure 
 * @environment axis2_environment
 * @param localname The local part of the attribute name
 * @param value normalized attribute value
 * @param ns The namespace name, if any, of the attribute 
 * @return The a pointer to newly created attribute struct 
 */

axis2_om_attribute_t *axis2_om_attribute_create(axis2_environment_t *environment ,const axis2_char_t *localname,const axis2_char_t *value, axis2_om_namespace_t *ns);

/* macros */
#define axis2_om_attribute_free(environment, om_attribute) ((om_attribute)->ops->axis2_om_attribute_ops_free(environment, om_attribute))
#define axis2_om_attribute_get_qname(environment, om_attribute) ((om_attribute)->ops->axis2_om_attribute_ops_get_qname(environment, om_attribute))
#define axis2_om_attribute_serialize(environment, om_attribute,om_ouput) ((om_attribute)->ops->axis2_om_attribute_ops_serialize(environment, om_attribute,om_output))


#endif /* AXIS2_OM_ATTRIBUTE_H */
