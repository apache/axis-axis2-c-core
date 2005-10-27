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
#include <axis2_environment.h>
#include <axis2_om_node.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_om_output OM Output
 * @ingroup axis2_om 
 * @{
 */

 /*  static  const char *DEFAULT_CHAR_SET_ENCODING = "utf-8"; */


  /** 
    * \brief OM Output struct
    * The XML writer interface struct of om
    */
    typedef struct axis2_om_output
    {
        /** xml stream writer. Should implement the Guththila pull parser interface */
        void *xml_writer;

        /* following fields are not used currently but will be used in the future */
        axis2_bool_t do_optimize;
        axis2_char_t *mime_boundary;
        axis2_char_t *root_content_id;
        int next_id;
        axis2_bool_t is_soap_11;
        axis2_char_t *char_set_encoding;
        /* xml version */
        axis2_char_t *xml_version;
        axis2_bool_t ignore_xml_declaration;
    } axis2_om_output_t;
    
  /**
    * creates OM output struct 
    * @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
    * @param xml_writer XML writer. Optional, can be NULL. If NULL default xml writer
    *           (guththila writer ) will be created with stdout as the output stream
	* @param writer_env writer envioronment. Optional, can be NULL.
    * @return a pointer to newly created output struct.
    */

    AXIS2_DECLARE(axis2_om_output_t*) axis2_om_output_create (axis2_environment_t *
                                               environment, void *xml_writer, void* writer_env);

  /**
    *  Performs xml writing.
    *  Accepts variable number of args depending on the on OM type to be serialized
    *  @param environment Environment. MUST NOT be NULL, if NULL behaviour is undefined.
    *  @param om_output Output struct to be used 
    *  @param type one of the OM types
    *  @param no_of_args number of arguments passed in the variable parameter list
    *  @return satus of the operation. AXIS2_SUCCESS on success else AXIS2_FAILURE
    */

    AXIS2_DECLARE(axis2_status_t) axis2_om_output_write (axis2_environment_t * environment,
                                          axis2_om_output_t * om_output,
                                          axis2_om_types_t type,
                                          int no_of_args, ...);

/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_OUTPUT_H */
