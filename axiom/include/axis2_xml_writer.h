/*
 *   Copyright 2004,2005 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 */
 
#ifndef AXIS2_XML_WRITER_H
#define AXIS2_XML_WRITER_H

/**
 *@file axis2_xml_writer.h   
 *@brief this is the parser abstraction layer for axis2 
 */
 
 #include <axis2_env.h>
 
 #ifdef __cplusplus
extern "C"
{
#endif
    
typedef struct axis2_xml_writer_ops axis2_xml_writer_ops_t;    
typedef struct axis2_xml_writer axis2_xml_writer_t;

/**
 * @defgroup axis2_xml_writer 
 * @ingroup axis2_om_parser
 * @{
 */



  /**
    * \brief axis2_xml_writer ops
    * Encapsulator struct for ops of axis2_xml_writer
    */

    AXIS2_DECLARE_DATA struct axis2_xml_writer_ops 
    {
       /**
        * Free xml writer
        * @param writer pointer to xml_writer struct to be freed
        * @param env environment, MUST NOT be NULL.
        * @returns status of the op. 
        *       AXIS2_SUCCESS on success and AXIS2_FAILURE on error
        */
        
        axis2_status_t (AXIS2_CALL *
      free)(axis2_xml_writer_t *writer,
              const axis2_env_t *env);
       /**
        *  Write a start tag to output stream with localname.
        *  Internally the writer keeps track of the opened tags 
        *  @param writer pointer to xml writer struct
        *  @param env environment. MUST NOT be NULL.
        *  @param localname localname of the tag, May not be NULL.
        *  @return the status of the op, AXIS2_SUCCESS on success
                    AXIS2_FAILURE on error.
        */
        axis2_status_t (AXIS2_CALL *
      write_start_element)(axis2_xml_writer_t *writer,
                             const axis2_env_t *env, 
                      axis2_char_t *localname);
       /**
        *  write an end tag to the output relying on the internal
        *  state of writer to determine the prefix and localname of
        *  the element
        *  @param writer xml_writer struct
        *  @param env environment, MUST NOT be NULL.
        *  @return status of the op. AXIS2_SUCCESS on success.
        *           AXIS2_FAILURE on error.
        */
        axis2_status_t (AXIS2_CALL *
      end_start_element)(axis2_xml_writer_t *writer,
                           const axis2_env_t *env);
        /**
         * Write an element tag with localname and namespace uri 
         * @param writer pointer to xml writer struct
         * @param env environment struct
         * @param localname localname of the tag, May not be null.
         * @param namespace_uri the namespace URI of the the pefix
         *        to use.may not be null.
         * @returns status of the op, AXIS2_SUCCESS on success.
         *          AXIS2_FAILURE on error
         */
        axis2_status_t (AXIS2_CALL *
      write_start_element_with_namespace)(axis2_xml_writer_t *writer,
                                 const axis2_env_t *env,
                                 axis2_char_t *localname,
                                 axis2_char_t *namespace_uri);
       
        /**
         * write a start tag to output
         * @param writer pointer to xml_writer struct
         * @param environment, MUST NOT be NULL.
         * @param localname localname of the tag, May not be null.
         * @param namespace_uri namespace to bind the prefix to
         * @param prefix the prefix to the tag.May not be NULL.
         * @return status of the op 
                   AXIS2_SUCCESS on success. AXIS2_FAILURE on error.
         */
        axis2_status_t (AXIS2_CALL *
      write_start_element_with_namespace_prefix)(axis2_xml_writer_t *writer,
                                       const axis2_env_t *env,
                                       axis2_char_t *localname,
                                       axis2_char_t *namespace_uri,
                                       axis2_char_t *prefix);       
        /**
         * write an element tag with localname 
         * @param writer xml_writer
         * @param env environment
         * @param localname localname 
         * @return status of the op AXIS2_SUCCESS on success,
         *                AXIS2_FAILURE on error.
         */         
        axis2_status_t (AXIS2_CALL *
      write_empty_element)(axis2_xml_writer_t *writer,
                           const axis2_env_t *env,
                      axis2_char_t *localname);
        /**
         * write empty_element with localname and namespace uri.
         * @param writer xml writer
         * @param env environment
         * @param localname localname 
         * @param namespace uri
         * @return status of the op, AXIS2_SUCCESS on success,
         *               AXIS2_FAILURE on error.
         */
        
        axis2_status_t (AXIS2_CALL *
      write_empty_element_with_namespace)(axis2_xml_writer_t *writer,
                                 const axis2_env_t *env,
                                 axis2_char_t *localname,
                                 axis2_char_t *namespace_uri);
        /**
         * write empty element with namespace uri and prefix 
         * @param writer xml_writer
         * @param env environment
         * @param localname localname
         * @param namespace_uri namespace uri 
         * @param prefix prefix
         * @return status of the op, AXIS2_SUCCESS on success,
         *                    AXIS2_FAILURE on error.
         */
        axis2_status_t (AXIS2_CALL *
      write_empty_element_with_namespace_prefix)(axis2_xml_writer_t *writer,
                                       const axis2_env_t *env,
                                       axis2_char_t *localname,
                                       axis2_char_t *namespace_uri,
                                       axis2_char_t *prefix);
        /**
         * write end tag with correct localname prefix resolved internally 
         * @param writer xml writer
         * @param env environment
         * @return status of the op, AXIS2_SUCCESS on success,
         *            AXIS2_FAILURE on failure 
         */
        axis2_status_t (AXIS2_CALL *
      write_end_element)(axis2_xml_writer_t *writer,
                           const axis2_env_t *env);
        /**
         * write end document
         * @param writer xml writer
         * @param env environment
         * @return status of the op AXIS2_SUCCESS on success,
         *              AXIS2_FAILURE on error.
         */
        axis2_status_t (AXIS2_CALL *
      write_end_document)(axis2_xml_writer_t *writer,
                            const axis2_env_t *env);
        /**
         * write attribute with localname and value
         * @param writer writer
         * @param env environment
         * @param localname localname 
         * @param value text value of attribute
         * @return status of the op AXIS2_SUCCESS on success,
         *                 AXIS2_FAILURE on error.
         */
        axis2_status_t (AXIS2_CALL *
      write_attribute)(axis2_xml_writer_t *writer,
                   const axis2_env_t *env,
                   axis2_char_t *localname,
                   axis2_char_t *value);
        /**
         *  @param writer
         *  @param env environment
         *  @param localname
         *  @param value text value of attribute
         *  @param namespace uri namespace uri
         *  @return status code of the op AXIS2_SUCCESS on success,
         *                    AXIS2_FAILURE on error.
         */
        axis2_status_t (AXIS2_CALL *
      write_attribute_with_namespace)(axis2_xml_writer_t *writer,
                                const axis2_env_t *env,
                                axis2_char_t *localname,
                                axis2_char_t *value,
                                axis2_char_t *namespace_uri);
        /**
         * @param writer xml_writer
         * @param env environment
         * @param localname localname
         * @param value text value of attribute
         * @param namespace uri namespaceuri
         * @param prefix prefix 
         */         
        axis2_status_t (AXIS2_CALL *
      write_attribute_with_namespace_prefix)(axis2_xml_writer_t *writer,
                                                  const axis2_env_t *env,
                                                  axis2_char_t *localname,
                                                  axis2_char_t *value,
                                                  axis2_char_t *namespace_uri,
                                                  axis2_char_t *prefix);
        /**
         * @param writer xml_writer
         * @param env environment
         * @param prefix prefix 
         * @param namespace uri namespaceuri
         * @return status of op AXIS2_SUCCESS on success,
         *               AXIS2_FAILURE on error.
         */
        
        axis2_status_t (AXIS2_CALL *
      write_namespace)(axis2_xml_writer_t *writer,
                   const axis2_env_t *env,
                   axis2_char_t *prefix,
                   axis2_char_t *namespace_uri);   
        /**
         * @param writer xml_writer
         * @param env environment
         * @param namespace uri namespaceuri
         * @return status of op AXIS2_SUCCESS on success,
         *               AXIS2_FAILURE on error.
         */

        axis2_status_t (AXIS2_CALL *
      write_default_namespace)(axis2_xml_writer_t *writer,
                         const axis2_env_t *env,
                         axis2_char_t *namespace_uri);  
        /**
         * @param writer xml_writer
         * @param env environment
         * @param value value
         * @return status of op AXIS2_SUCCESS on success,
         *               AXIS2_FAILURE on error.
         */
                                                    

        axis2_status_t (AXIS2_CALL *
      write_comment)(axis2_xml_writer_t *writer,
                  const axis2_env_t *env,
                  axis2_char_t *value);
        /**
         * @param writer xml_writer
         * @param env environment
         * @param target pi target 
         * @return status of op AXIS2_SUCCESS on success,
         *               AXIS2_FAILURE on error.
         */


        axis2_status_t (AXIS2_CALL *
      write_processing_instruction)(axis2_xml_writer_t *writer,
                              const axis2_env_t *env,
                              axis2_char_t *target);
                                   
        /**
         * @param writer xml_writer
         * @param env environment
         * @param target pi target
         * @param data  pi data
         * @return status of op AXIS2_SUCCESS on success,
         *               AXIS2_FAILURE on error.
         */
                                   
        axis2_status_t (AXIS2_CALL *
      write_processing_instruction_data)(axis2_xml_writer_t *writer,
                                 const axis2_env_t *env,
                                 axis2_char_t *target,
                                 axis2_char_t *data);
         
        /**
         * @param writer xml_writer
         * @param env environment
         * @param data  cdata
         * @return status of op AXIS2_SUCCESS on success,
         *               AXIS2_FAILURE on error.
         */                                           
        axis2_status_t (AXIS2_CALL *
      write_cdata)(axis2_xml_writer_t *writer,
                const axis2_env_t *env,
                axis2_char_t *data);
        
         
        /**
         * @param writer xml_writer
         * @param env environment
         * @param dtd  dtd
         * @return status of op AXIS2_SUCCESS on success,
         *               AXIS2_FAILURE on error.
         */
        axis2_status_t (AXIS2_CALL *
      write_dtd)(axis2_xml_writer_t *writer,
               const axis2_env_t *env,
               axis2_char_t *dtd); 
        
         
        /**
         * @param writer xml_writer
         * @param env environment
         * @param name name
         * @return status of op AXIS2_SUCCESS on success,
         *               AXIS2_FAILURE on error.
         */
        axis2_status_t (AXIS2_CALL *
      write_entity_ref)(axis2_xml_writer_t *writer,
                          const axis2_env_t *env,
                          axis2_char_t *name); 
         
        /**
         * @param writer xml_writer
         * @param env environment
         * @return status of op AXIS2_SUCCESS on success,
         *               AXIS2_FAILURE on error.
         */
        axis2_status_t (AXIS2_CALL *
      write_start_document)(axis2_xml_writer_t *writer,
                              const axis2_env_t *env);  
        
         
        /**
         * @param writer xml_writer
         * @param env environment
         * @param version version 
         * @return status of op AXIS2_SUCCESS on success,
         *               AXIS2_FAILURE on error.
         */
        axis2_status_t (AXIS2_CALL *
      write_start_document_with_version)(axis2_xml_writer_t *writer,
                                           const axis2_env_t *env,
                                           axis2_char_t *version);   
         
        /**
         * @param writer xml_writer
         * @param env environment
         * @param version version
         * @param encoding encoding
         * @return status of op AXIS2_SUCCESS on success,
         *               AXIS2_FAILURE on error.
         */
        axis2_status_t (AXIS2_CALL *
      write_start_document_with_version_encoding)(axis2_xml_writer_t *writer,
                                                    const axis2_env_t *env,
                                                    axis2_char_t *version,
                                                    axis2_char_t *encoding);
         
        /**
         * @param writer xml_writer
         * @param env environment
         * @param text text 
         * @return status of op AXIS2_SUCCESS on success,
         *               AXIS2_FAILURE on error.
         */                                         
        axis2_status_t (AXIS2_CALL *
      write_characters)(axis2_xml_writer_t *writer,
                          const axis2_env_t *env,
                          axis2_char_t *text);   
         
        /**
         * @param writer xml_writer
         * @param env environment
         * @param uri uri
         * @return status of op AXIS2_SUCCESS on success,
         *               AXIS2_FAILURE on error.
         */
       axis2_char_t* (AXIS2_CALL *
      get_prefix)(axis2_xml_writer_t *writer,
                   const axis2_env_t *env,
                   axis2_char_t *uri);
         
        /**
         * @param writer xml_writer
         * @param env environment
         * @param prefix prefix
         * @param uri uri
         * @return status of op AXIS2_SUCCESS on success,
         *               AXIS2_FAILURE on error.
         */
       axis2_status_t (AXIS2_CALL *
      set_prefix)(axis2_xml_writer_t *writer,
                   const axis2_env_t *env,
                   axis2_char_t *prefix,
                   axis2_char_t *uri);
         
        /**
         * @param writer xml_writer
         * @param env environment
         * @param uri uri
         * @return status of op AXIS2_SUCCESS on success,
         *               AXIS2_FAILURE on error.
         */                                                 
       axis2_status_t (AXIS2_CALL *
      set_default_prefix)(axis2_xml_writer_t *writer,
                           const axis2_env_t *env,
                           axis2_char_t *uri);                                                 
         
        /**
         * @param writer xml_writer
         * @param env environment
         * @param text text
         * @param in_attr 
         * @return status of op AXIS2_SUCCESS on success,
         *               AXIS2_FAILURE on error.
         */
        axis2_status_t (AXIS2_CALL *
      write_encoded)(axis2_xml_writer_t *writer,
                       const axis2_env_t *env,
                       axis2_char_t *text,
                       int in_attr);
                                                 
        void* (AXIS2_CALL *
      get_xml)(axis2_xml_writer_t *writer,
                 const axis2_env_t *env);                                                 

        int (AXIS2_CALL *
      get_type)(axis2_xml_writer_t *writer,
                 const axis2_env_t *env);                                                 
    };

/** 
 * @brief axis2_pull_parser struct
  *   Axis2 OM pull_parser
 */

struct axis2_xml_writer
{
    axis2_xml_writer_ops_t *ops;    
};    

/**
 *  create function for axis2_xml_writer
 *
 */
AXIS2_EXTERN axis2_xml_writer_t * AXIS2_CALL
axis2_xml_writer_create(const axis2_env_t *env,
                        axis2_char_t *filename,
                        axis2_char_t *encoding,
                        int is_prefix_default,
                        int compression);

AXIS2_EXTERN axis2_xml_writer_t * AXIS2_CALL
axis2_xml_writer_create_for_memory(const axis2_env_t *env,
                                   axis2_char_t *encoding,
                                   int is_prefix_default,
                                   int compression,
                           int type);

/********************** start macros *******************************************/

#define AXIS2_XML_WRITER_FREE(writer, env) \
        ((writer)->ops->free(writer,env))

#define AXIS2_XML_WRITER_WRITE_START_ELEMENT(writer, env, localname) \
        ((writer)->ops->write_start_element(writer, env, localname))

#define AXIS2_XML_WRITER_END_START_ELEMENT(writer, env) \
        ((writer)->ops->end_start_element(writer,env))


#define AXIS2_XML_WRITER_WRITE_START_ELEMENT_WITH_NAMESPACE(\
        writer, env, localname, namespace_uri)\
        ((writer)->ops->write_start_element_with_namespace(\
        writer, env, localname, namespace_uri))

#define AXIS2_XML_WRITER_WRITE_START_ELEMENT_WITH_NAMESPACE_PREFIX(\
        writer, env, localname, namespace_uri, prefix) \
        ((writer)->ops->write_start_element_with_namespace_prefix(\
        writer,env,localname,namespace_uri,prefix))
        
        
#define AXIS2_XML_WRITER_WRITE_EMPTY_ELEMENT(writer,env,localname) \
        ((writer)->ops->write_empty_element(writer,env,localname))
        
#define AXIS2_XML_WRITER_WRITE_EMPTY_ELEMENT_WITH_NAMESPACE(\
        writer, env,localname,namespace_uri) \
        ((writer)->ops->write_empty_element_with_namespace(writer,env,localname,namespace_uri))       

#define AXIS2_XML_WRITER_WRITE_EMPTY_ELEMENT_WITH_NAMESPACE_PREFIX(\
        writer, env, localname, namespace_uri, prefix) \
        ((writer)->ops->write_empty_element_with_namespace_prefix(\
        writer,env,localname,namespace_uri, prefix))

#define AXIS2_XML_WRITER_WRITE_END_ELEMENT(writer, env) \
        ((writer)->ops->write_end_element(writer,env))


#define AXIS2_XML_WRITER_WRITE_END_DOCUMENT(writer, env) \
        ((writer)->ops->write_end_document(writer,env))

#define AXIS2_XML_WRITER_WRITE_ATTRIBUTE(writer, env, localname, value) \
        ((writer)->ops->write_attribute(writer,env, localname, value))

#define AXIS2_XML_WRITER_WRITE_ATTRIBUTE_WITH_NAMESPACE(\
        writer, env, localname, value, namespace_uri) \
        ((writer)->ops->write_attribute_with_namespace(\
        writer,env, localname, value,namespace_uri))

#define AXIS2_XML_WRITER_WRITE_ATTRIBUTE_WITH_NAMESPACE_PREFIX(\
        writer, env, localname, value, namespace_uri, prefix) \
        ((writer)->ops->write_attribute_with_namespace_prefix(\
        writer,env, localname, value,namespace_uri, prefix))

#define AXIS2_XML_WRITER_WRITE_NAMESPACE(writer, env, prefix , namespace_uri) \
        ((writer)->ops->write_namespace(writer, env, prefix, namespace_uri))

#define AXIS2_XML_WRITER_WRITE_DEFAULT_NAMESPACE(writer, env, namespace_uri) \
        ((writer)->ops->write_default_namespace(writer, env, namespace_uri))

#define AXIS2_XML_WRITER_WRITE_COMMENT(writer, env, value) \
        ((writer)->ops->write_comment(writer, env, value))

#define AXIS2_XML_WRITER_WRITE_PROCESSING_INSTRUCTION(writer, env, target) \
        ((writer)->ops->write_processing_instruction(writer, env, target))

#define AXIS2_XML_WRITER_WRITE_PROCESSING_INSTRUCTION_DATA(writer, env, target, data) \
        ((writer)->ops->write_processing_instruction_data(writer, env, target, data))
        
#define AXIS2_XML_WRITER_WRITE_CDATA(writer, env, data) \
        ((writer)->ops->write_cdata(writer, env, data))        

#define AXIS2_XML_WRITER_WRITE_DTD(writer, env, dtd) \
        ((writer)->ops->write_cdata(writer, env, dtd)) 

#define AXIS2_XML_WRITER_WRITE_ENTITY_REF(writer, env, entity_ref) \
        ((writer)->ops->write_entity_ref(writer, env, entity_ref)) 
        
#define AXIS2_XML_WRITER_WRITE_START_DOCUMENT(writer, env) \
        ((writer)->ops->write_start_document(writer,env))        

#define AXIS2_XML_WRITER_WRITE_START_DOCUMENT_WITH_VERSION(writer, env, version) \
        ((writer)->ops->write_start_document_with_version(writer, env, version)) 

#define AXIS2_XML_WRITER_WRITE_START_DOCUMENT_WITH_VERSION_ENCODING(\
        writer, env,  version, encoding) \
        ((writer)->ops->write_start_document_with_version_encoding(\
        writer, env, version, encoding))

#define AXIS2_XML_WRITER_WRITE_CHARACTERS(writer, env, text) \
        ((writer)->ops->write_characters(writer, env, text)) 

#define AXIS2_XML_WRITER_GET_PREFIX(writer, env, uri) \
        ((writer)->ops->get_prefix(writer, env, uri))

#define AXIS2_XML_WRITER_SET_PREFIX(writer, env, prefix, uri) \
        ((writer)->ops->set_prefix(writer, env, prefix, uri))

#define AXIS2_XML_WRITER_SET_DEFAULT_PREFIX(writer, env, uri) \
        ((writer)->ops->set_default_prefix(writer, env, uri))

#define AXIS2_XML_WRITER_WRITE_ENCODED(writer, env, text, in_attr) \
        ((writer)->ops->write_encoded(writer, env, text, in_attr))
        
#define AXIS2_XML_WRITER_GET_XML(writer, env) \
        ((writer)->ops->get_xml(writer, env))        

#define AXIS2_XML_WRITER_GET_TYPE(writer, env) \
        ((writer)->ops->get_type(writer, env)) 

/** @} */


#ifdef __cplusplus
}
#endif

#endif /* AXIS2_XML_WRITER_H */
