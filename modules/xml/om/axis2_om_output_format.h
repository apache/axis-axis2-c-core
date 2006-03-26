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

#ifndef AXIS2_OM_OUTPUT_FORMAT_H
#define AXIS2_OM_OUTPUT_FORMAT_H

/**
 * @file axis2_om_output_format.h
 * @brief axis2 mime_output interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_array_list.h>
#include <axis2_handler_desc.h>
#include <axis2_mime_output.h>


#ifdef __cplusplus
extern "C"
{
#endif

/* This should be moved to header file later axis2_defines.h*/    
#define axis2_byte_t char

    /**
     * Field DEFAULT_CHAR_SET_ENCODING. Specifies the default
     * character encoding scheme to be used.
     */
#define AXIS2_DEFAULT_CHAR_SET_ENCODING  "utf-8"
	
typedef struct axis2_om_output_format_ops axis2_om_output_format_ops_t;
typedef struct axis2_om_output_format axis2_om_output_format_t;	
	

/** @defgroup axis2_om_output_format Flow
  * @ingroup axis2_om_output_format
  * @{
  */

/** 
 * @brief Flow ops struct
 * Encapsulator struct for ops of axis2_om_output_format
 */
struct axis2_om_output_format_ops
{
    axis2_status_t (AXIS2_CALL *is_optimized) (axis2_om_output_format_t *om_output_format, axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *set_do_optimized) (axis2_om_output_format_t *om_output_format, axis2_env_t **env, const axis2_bool_t is_optimized);

    axis2_char_t* (AXIS2_CALL *get_content_type) (axis2_om_output_format_t *om_output_format, axis2_env_t **env);
    
    axis2_char_t* (AXIS2_CALL *get_mime_boundary) (axis2_om_output_format_t *om_output_format, axis2_env_t **env);
    
    axis2_char_t* (AXIS2_CALL *get_root_content_id) (axis2_om_output_format_t *om_output_format, axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *set_char_set_encoding) (axis2_om_output_format_t *om_output_format, axis2_env_t **env, axis2_char_t *char_set_encoding);
    
    axis2_char_t* (AXIS2_CALL *get_char_set_encoding) (axis2_om_output_format_t *om_output_format, axis2_env_t **env);
    
    axis2_char_t*  (AXIS2_CALL *get_next_content_id) (axis2_om_output_format_t *om_output_format, axis2_env_t **env);
    
    axis2_char_t* (AXIS2_CALL *get_xml_version)( axis2_om_output_format_t *om_output_format, axis2_env_t **env);
    
    axis2_status_t  (AXIS2_CALL *set_xml_version) (axis2_om_output_format_t *om_output_format, axis2_env_t **env, axis2_char_t *xml_version);
    
    axis2_bool_t  (AXIS2_CALL *is_soap11) (axis2_om_output_format_t *om_output_format, axis2_env_t **env); 
    
    axis2_status_t  (AXIS2_CALL *set_is_soap11) (axis2_om_output_format_t *om_output_format, axis2_env_t **env, const axis2_bool_t is_soap11);
    
    axis2_bool_t  (AXIS2_CALL *is_ignore_xml_declaration) (axis2_om_output_format_t *om_output_format, axis2_env_t **env); 
    
    axis2_status_t  (AXIS2_CALL *set_ignore_xml_declaration) (axis2_om_output_format_t *om_output_format, axis2_env_t **env, const axis2_bool_t ignore_xml_declaration);
    
	/** Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *free) (axis2_om_output_format_t *mime_output,
                            axis2_env_t **env);

};

/** 
 * @brief Flow struct
 *	Flow  
 */ 
struct axis2_om_output_format
{
	axis2_om_output_format_ops_t *ops;
};

/**
 * Creates mime_output struct
 * @return pointer to newly created mime_output
 */
AXIS2_DECLARE(axis2_om_output_format_t *) 
axis2_om_output_format_create (axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_OM_OUTPUT_FORMAT_FREE(om_output_format, env) ((om_output_format)->ops->free (om_output_format, env))

#define AXIS2_OM_OUTPUT_FORMAT_IS_OPTIMIZED(om_output_format, env) ((om_output_format)->ops->is_optimized(om_output_format, env))

#define AXIS2_OM_OUTPUT_FORMAT_SET_DO_OPTIMIZED(om_output_format, env, is_optimized) ((om_output_format)->ops->set_do_optimized(om_output_format, env, is_optimized))



 #define AXIS2_OM_OUTPUT_FORMAT_GET_CONTENT_TYPE(om_output_format, env) ((om_output_format)->ops->get_content_type(om_output_format, env))

 #define AXIS2_OM_OUTPUT_FORMAT_GET_MIME_BOUNDARY(om_output_format, env) ((om_output_format)->ops->get_mime_boundary(om_output_format, env))

 #define AXIS2_OM_OUTPUT_FORMAT_GET_ROOT_CONTENT_ID(om_output_format, env) ((om_output_format)->ops->get_root_content_id(om_output_format, env))

 #define AXIS2_OM_OUTPUT_FORMAT_SET_CHAR_SET_ENCODING(om_output_format, env, char_set_encoding) ((om_output_format)->ops->set_char_set_encoding(om_output_format, env, char_set_encoding))

 #define AXIS2_OM_OUTPUT_FORMAT_GET_CHAR_SET_ENCODING(om_output_format, env) ((om_output_format)->ops->get_char_set_encoding(om_output_format, env))

 #define AXIS2_OM_OUTPUT_FORMAT_GET_NEXT_CONTENT_ID(om_output_format, env) ((om_output_format)->ops->get_next_content_id(om_output_format, env))

 #define AXIS2_OM_OUTPUT_FORMAT_GET_XML_VERSION(om_output_format, env) ((om_output_format)->ops->get_xml_version(om_output_format, env))

 #define AXIS2_OM_OUTPUT_FORMAT_SET_XML_VERSION(om_output_format, env, xml_version) ((om_output_format)->ops->get_xml_version(om_output_format, env, xml_version))

 #define AXIS2_OM_OUTPUT_FORMAT_IS_SOAP11(om_output_format, env) ((om_output_format)->ops->is_soap11(om_output_format, env)) 

 #define AXIS2_OM_OUTPUT_FORMAT_SET_IS_SOAP11(om_output_format, env, is_soap11) ((om_output_format)->ops->set_is_soap11(om_output_format, env, is_soap11))

 #define AXIS2_OM_OUTPUT_FORMAT_IS_IGNORE_XML_DECLARATION(om_output_format, env) ((om_output_format)->ops->is_ignore_xml_declaration(om_output_format, env)) 

 #define AXIS2_OM_OUTPUT_FORMAT_SET_IGNORE_XML_DECLARATION(om_output_format, env, ignore_xml_declaration) ((om_output_format)->ops->set_ignore_xml_declaration(om_output_format, env, ignore_xml_declaration))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_OM_OUTPUT_FORMAT_H */
