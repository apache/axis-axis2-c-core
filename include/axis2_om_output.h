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

#ifndef AXIS2_OM_OUTPUT_H
#define AXIS2_OM_OUTPUT_H

/**
 * @file axis2_om_output.h
 * @brief interface of om xml writer
 */

#include <axis2_defines.h>
#include <axis2.h>
#include <axis2_env.h>
#include <axis2_om_node.h>
#include <axis2_xml_writer.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_om_output OM Output
 * @ingroup axis2_om 
 * @{
 */

  /** 
    * \brief OM Output struct
    * The XML writer interface struct of om
    */
    typedef struct axis2_om_output axis2_om_output_t;

    
  /**
    * creates OM output struct 
    * @param env Environment. MUST NOT be NULL, .
    * @param xml_writer XML writer. 
    * @return a pointer to newly created output struct.
    */

    AXIS2_DECLARE(axis2_om_output_t*) 
    axis2_om_output_create (axis2_env_t **env,
                            axis2_xml_writer_t *xml_writer);

  /**
    *  Performs xml writing.
    *  Accepts variable number of args depending on the on OM type to be serialized
    *  @param om_output Output struct to be used 
    *  @param env Environment. MUST NOT be NULL, 
    *  @param type one of the OM types
    *  @param no_of_args number of arguments passed in the variable parameter list
    *  @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE
    */

    AXIS2_DECLARE(axis2_status_t)
    axis2_om_output_write (axis2_om_output_t * om_output, 
                           axis2_env_t **env,
                           axis2_om_types_t type,
                           int no_of_args, ...);
   /**
    * free om_output 
    *@param om_output om_output struct
    *@param env environment
    *@return status code AXIS2_SUCCESS on success, AXIS2_FAILURE otherwise 
    */
    AXIS2_DECLARE(axis2_status_t)
    axis2_om_output_free(axis2_om_output_t *om_output,
                         axis2_env_t **env);    

/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_OUTPUT_H */
