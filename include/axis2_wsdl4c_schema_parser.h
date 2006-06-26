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
 
 #ifndef AXIS2_WSDL4C_SCHEMA_PARSER_H
 #define AXIS2_WSDL4C_SCHEMA_PARSER_H
 
 /**
 * @file axis2_wsdl4c_schema_parser.h
 * @brief defines WSDL4C Schema Parser
 */
#include <axis2_array_list.h>
#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axis2_wsdl4c_schema_parser wsdl4c_schema_parser
 * @ingroup axis2_wsdl4c
 * @{
 */
   /**
   * The default constructor for Schema Parser
   * @param input stream for the schema file
   * @param output stream for any error outputs
   * @param schema_path path to directory containing XML schema files
   *
   */
    void *
    axis2_wsdl4c_schema_parser_create(char* schema_file, char *schema_path);

    void
    axis2_wsdl4c_schema_parser_destroy(void *schema_parser);

   /**
    * Return the namespace of the schema document
    */
    axis2_char_t *
    axis2_wsdl4c_schema_parser_get_namespace(void *schema_parser);
                                

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_WSDL4C_SCHEMA_PARSER_H */
