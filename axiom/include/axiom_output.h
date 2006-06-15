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

#ifndef AXIOM_OUTPUT_H
#define AXIOM_OUTPUT_H

/**
 * @file axiom_output.h
 * @brief interface of om xml writer
 */

#include <axis2_utils_defines.h>
#include <axis2_utils.h>
#include <axis2_env.h>
#include <axiom_node.h>
#include <axiom_xml_writer.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup axiom_output OM Output
 * @ingroup axiom 
 * @{
 */

  /** 
    * \brief OM Output struct
    * The XML writer interface struct of om
    */
    typedef struct axiom_output axiom_output_t;
    typedef struct axiom_output_ops axiom_output_ops_t;
    struct axiom_text;
    
AXIS2_DECLARE_DATA struct axiom_output_ops
{
   /**
    * free om_output 
    * @param om_output om_output struct
    * @param env environment
    * @return status code AXIS2_SUCCESS on success,
    *  AXIS2_FAILURE otherwise 
    */
    axis2_status_t (AXIS2_CALL *
   free_fn)(axiom_output_t *om_output,
             const axis2_env_t *env);
    
   /**
     * If the xml to be serialized is soap 11, this property is set to true
     * @param om_output pointer to om_output struct
     * @param env environment must not be NULL
     * @returns the output soap version 
     */   
    axis2_bool_t (AXIS2_CALL *
   is_soap11)(axiom_output_t *om_output,
            const axis2_env_t *env); 
   /**
     * @returns true if the ignore_xml_declaration property is true   
    */
   axis2_bool_t (AXIS2_CALL *
   is_ignore_xml_declaration)(axiom_output_t *om_output,
                        const axis2_env_t *env);                               

   /**
    * sets the ignore_xml_declaration property is true
    */
   axis2_status_t (AXIS2_CALL *
   set_ignore_xml_declaration)(axiom_output_t *om_output,
                         const axis2_env_t *env,
                         axis2_bool_t ignore_xml_dec); 
   /**
    * sets the soap11  property to true
    */                     
   axis2_status_t (AXIS2_CALL *
   set_soap11)(axiom_output_t *om_output,
            const axis2_env_t *env,
            axis2_bool_t soap11);
               
   /**
     * set xml_version property
     */    
   axis2_status_t (AXIS2_CALL *
   set_xml_version)(axiom_output_t *om_output,
                const axis2_env_t *env,
                axis2_char_t *xml_version);
   /**
     * @returns xml version property
     */    
   axis2_char_t* (AXIS2_CALL *
   get_xml_version)(axiom_output_t *om_output,
                const axis2_env_t *env);  
   /**   
     * set the char set encoding property
     */    
   axis2_status_t (AXIS2_CALL *
   set_char_set_encoding)(axiom_output_t *om_output,
                     const axis2_env_t *env,
                     axis2_char_t *char_set_encoding);
   /**
     * @returns the char set encoding property
     */    
   axis2_char_t* (AXIS2_CALL *
   get_char_set_encoding)(axiom_output_t *om_output,
                     const axis2_env_t *env); 
   /** 
     * set the do optimize property true
     */    
   axis2_status_t (AXIS2_CALL *
   set_do_optimize)(axiom_output_t *om_output,
                const axis2_env_t *env,
                axis2_bool_t optimize); 
   
   /**
     * returns the xml writer
     */    
   axiom_xml_writer_t* (AXIS2_CALL *
   get_xml_writer)(axiom_output_t *om_output,
               const axis2_env_t *env);                                                                                                                                                  /**
     *  returns the content type
     *  for soap11 'text/xml' etc..
     *  @param om_output
     *  @param env environemnt
     *  @returns content id
     */     
   const axis2_char_t* (AXIS2_CALL *
   get_content_type)(axiom_output_t *om_output,
                 const axis2_env_t *env);
                        
   /**
     * writes the xml versio encoding 
     */    
   axis2_status_t (AXIS2_CALL *
   write_xml_version_encoding)(axiom_output_t *om_output,
                         const axis2_env_t *env);

    /**
     * @returns whether the output is to be optimized 
     */
    axis2_bool_t (AXIS2_CALL *
    is_optimized)(axiom_output_t *om_output,
                  const axis2_env_t *env);                               

    /** returns the next content id 
     */
    axis2_char_t* (AXIS2_CALL *
    get_next_content_id)(axiom_output_t *om_output,
                         const axis2_env_t *env);
                         
    /**
     * root content id
     */                         
    axis2_char_t* (AXIS2_CALL *
    get_root_content_id)(axiom_output_t *om_output,
                         const axis2_env_t *env);
    
    
    axis2_char_t* (AXIS2_CALL *
    get_mime_boundry)(axiom_output_t *om_output,
                      const axis2_env_t *env);                       

    axis2_byte_t* (AXIS2_CALL *
    flush)(axiom_output_t *om_output,
                      const axis2_env_t *env,
                      axis2_byte_t **output_stream,
                      int *output_stream_size);

                                                  
};  
    

struct axiom_output
{
    axiom_output_ops_t *ops;
};
    
    
  /**
    * creates OM output struct 
    * @param env Environment. MUST NOT be NULL, .
    * @param xml_writer XML writer. 
    * @return a pointer to newly created output struct.
    */

AXIS2_EXTERN axiom_output_t* AXIS2_CALL 
axiom_output_create (const axis2_env_t *env,
                        axiom_xml_writer_t *xml_writer);


   /**
    *  Performs xml writing.
    *  Accepts variable number of args depending on the on OM type to be serialized
    *  @param om_output Output struct to be used 
    *  @param env Environment. MUST NOT be NULL, 
    *  @param type one of the OM types
    *  @param no_of_args number of arguments passed in the variable parameter list
    *  @return satus of the op. AXIS2_SUCCESS on success else AXIS2_FAILURE
    */

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_output_write(axiom_output_t * om_output, 
                      const axis2_env_t *env,
                      axiom_types_t type,
                      int no_of_args, ...);
                      
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_output_write_optimized(axiom_output_t *om_output, 
                      const axis2_env_t *env, 
                      struct axiom_text *om_text);
/************** Macros ********************************************************/

#define AXIOM_OUTPUT_FREE(output, env) \
        ((output)->ops->free_fn(output, env))
        
#define AXIOM_OUTPUT_IS_SOAP11(output, env) \
        ((output)->ops->is_soap11(output, env))       
        
#define AXIOM_OUTPUT_IS_IGNORE_XML_DECLARATION(output, env) \
        ((output)->ops->is_ignore_xml_version(output, env)) 
        
#define AXIOM_OUTPUT_SET_IGNORE_XML_DECLARATION(output, env, xml_dec) \
        ((output)->ops->set_ignore_xml_version(output, env, xml_dec))

#define AXIOM_OUTPUT_SET_SOAP11(output, env, soap11) \
        ((output)->ops->set_soap11(output, env, soap11))
        
#define AXIOM_OUTPUT_SET_XML_VERSION(output, env, xml_version) \
        ((output)->ops->set_xml_version(output, env, xml_version))
        
#define AXIOM_OUTPUT_GET_XML_VERSION(output, env) \
        ((output)->ops->get_xml_version(output, env))
        
#define AXIOM_OUTPUT_SET_CHAR_SET_ENCODING(output, env, char_set) \
        ((output)->ops->set_char_set_encoding(output, env, char_set))
        
#define AXIOM_OUTPUT_GET_CHAR_SET_ENCODING(output, env) \
        ((output)->ops->get_char_set_encoding(output, env))
        
#define AXIOM_OUTPUT_SET_DO_OPTIMIZE(output, env, optimize) \
        ((output)->ops->set_do_optimize(output, env, optimize))  
        
#define AXIOM_OUTPUT_GET_XML_WRITER(output, env) \
        ((output)->ops->get_xml_writer(output, env))
        
#define AXIOM_OUTPUT_GET_CONTENT_TYPE(output, env) \
        ((output)->ops->get_content_type(output, env))                      

#define AXIOM_OUTPUT_WRITE_XML_VERSION_ENCODING(output, env) \
        ((output)->ops->write_xml_version_encoding(output, env))

#define AXIOM_OUTPUT_IS_OPTIMIZED(output, env) \
        ((output)->ops->is_optimized(output, env))

#define AXIOM_OUTPUT_GET_NEXT_CONTENT_ID(om_output, env) \
        ((om_output)->ops->get_next_content_id(om_output, env))
        
#define AXIOM_OUTPUT_GET_ROOT_CONTENT_ID(om_output, env) \
        ((om_output)->ops->get_root_content_id(om_output, env))
        
#define AXIOM_OUTPUT_GET_MIME_BOUNDRY(om_output, env) \
        ((om_output)->ops->get_mime_boundry(om_output, env))                

#define AXIOM_OUTPUT_FLUSH(om_output, env, output_stream, output_stream_size) \
        ((om_output)->ops->flush(om_output, env, output_stream, output_stream_size))                
/** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* AXIOM_OUTPUT_H */
