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
    typedef struct axis2_om_output_ops axis2_om_output_ops_t;
    
AXIS2_DECLARE_DATA struct axis2_om_output_ops
{
   /**
    * free om_output 
    *@param om_output om_output struct
    *@param env environment
    *@return status code AXIS2_SUCCESS on success,
    * AXIS2_FAILURE otherwise 
    */
        axis2_status_t (AXIS2_CALL *free_fn)
                                  (axis2_om_output_t *om_output,
                                   axis2_env_t **env);
                                             
        axis2_bool_t (AXIS2_CALL *is_soap11)
                                  (axis2_om_output_t *om_output,
                                   axis2_env_t **env); 
                                                                                           
        axis2_bool_t (AXIS2_CALL *is_ignore_xml_declaration)
                                  (axis2_om_output_t *om_output,
                                   axis2_env_t **env);                               

        axis2_status_t (AXIS2_CALL *set_ignore_xml_declaration)
                                  (axis2_om_output_t *om_output,
                                   axis2_env_t **env,
                                   axis2_bool_t ignore_xml_dec); 
                                   
        axis2_status_t (AXIS2_CALL *set_soap11)
                                  (axis2_om_output_t *om_output,
                                   axis2_env_t **env,
                                   axis2_bool_t soap11);
                                   
        axis2_status_t (AXIS2_CALL *set_xml_version)
                                  (axis2_om_output_t *om_output,
                                   axis2_env_t **env,
                                   axis2_char_t *xml_version);
                                     
        axis2_char_t* (AXIS2_CALL *get_xml_version)
                                  (axis2_om_output_t *om_output,
                                   axis2_env_t **env);  
                                    
        axis2_status_t (AXIS2_CALL *set_char_set_encoding)
                                  (axis2_om_output_t *om_output,
                                   axis2_env_t **env,
                                   axis2_char_t *char_set_encoding);
                                     
        axis2_char_t* (AXIS2_CALL *get_char_set_encoding)
                                  (axis2_om_output_t *om_output,
                                   axis2_env_t **env); 
                                    
        axis2_status_t (AXIS2_CALL *set_do_optimize)
                                  (axis2_om_output_t *om_output,
                                   axis2_env_t **env,
                                   axis2_bool_t optimize); 
                                   
        axis2_xml_writer_t* (AXIS2_CALL *get_xml_writer)
                                  (axis2_om_output_t *om_output,
                                   axis2_env_t **env);                                                                                                                                                                             
                                                                     
};
    

struct axis2_om_output
{
    axis2_om_output_ops_t *ops;
};
    
    
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
axis2_om_output_write(axis2_om_output_t * om_output, 
                      axis2_env_t **env,
                      axis2_om_types_t type,
                      int no_of_args, ...);
/************** Macros ********************************************************/

#define AXIS2_OM_OUTPUT_FREE(output, env) \
        ((output)->ops->free_fn(output, env))
        
#define AXIS2_OM_OUTPUT_IS_SOAP11(output, env) \
        ((output)->ops->is_soap11(output, env))       
        
#define AXIS2_OM_OUTPUT_IS_IGNORE_XML_DECLARATION(output, env) \
        ((output)->ops->is_ignore_xml_version(output, env)) 
        
#define AXIS2_OM_OUTPUT_SET_IGNORE_XML_DECLARATION(output, env, xml_dec) \
        ((output)->ops->set_ignore_xml_version(output, env, xml_dec))

#define AXIS2_OM_OUTPUT_SET_SOAP11(output, env, soap11) \
        ((output)->ops->set_soap11(output, env, soap11))
        
#define AXIS2_OM_OUTPUT_SET_XML_VERSION(output, env, xml_version) \
        ((output)->ops->set_xml_version(output, env, xml_version))
        
#define AXIS2_OM_OUTPUT_GET_XML_VERSION(output, env) \
        ((output)->ops->get_xml_version(output, env))
        
#define AXIS2_OM_OUTPUT_SET_CHAR_SET_ENCODING(output, env, char_set) \
        ((output)->ops->set_char_set_encoding(output, env, char_set))
        
#define AXIS2_OM_OUTPUT_GET_CHAR_SET_ENCODING(output, env) \
        ((output)->ops->get_char_set_encoding(output, env))
        
#define AXIS2_OM_OUTPUT_SET_DO_OPTIMIZE(output, env, optimize) \
        ((output)->ops->set_do_optimize(output, env, optimize))  
        
#define AXIS2_OM_OUTPUT_GET_XML_WRITER(output, env) \
        ((output)->ops->get_xml_writer(output, env))              
   
/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OM_OUTPUT_H */
