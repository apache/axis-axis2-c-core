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
 
 #ifndef AXIS2_SOAP_BUILDER_H
 #define AXIS2_SOAP_BUILDER_H
 
 #include <axis2_om_stax_builder.h>
 #include <axis2_soap_envelope.h>
 
/**
 * @file axis2_soap_builder.h
 * @brief axis2_soap_builder struct 
 */

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct axis2_soap_builder axis2_soap_builder_t;
typedef struct axis2_soap_builder_ops  axis2_soap_builder_ops_t;
    
/**
 * @defgroup axis2_soap_builder
 * @ingroup axis2_soap_model builder
 * @{
 */

/**
 *   \brief soap_model builder operations struct
 *   ops Encapsulator struct of axis2_soap_operations 
 */

AXIS2_DECLARE_DATA   struct axis2_soap_builder_ops
{
        axis2_status_t (AXIS2_CALL *free)(axis2_soap_builder_t *builder,
                                          const axis2_env_t *env);

        axis2_soap_envelope_t* (AXIS2_CALL *get_soap_envelope)
                                         (axis2_soap_builder_t *builder,
                                          const axis2_env_t *env);
                                          
        axis2_om_document_t* (AXIS2_CALL *get_document)
                                         (axis2_soap_builder_t *builder,
                                          const axis2_env_t *env);
                                          
        axis2_status_t (AXIS2_CALL *next)(axis2_soap_builder_t *builder,
                                const axis2_env_t *env);
                                
                                                     
        axis2_om_node_t* (AXIS2_CALL *get_document_element)
                                            (axis2_soap_builder_t *builder,
                                             const axis2_env_t *env); 
                                             
        axis2_status_t (AXIS2_CALL *set_bool_processing_mandatory_fault_elements)
                                            (axis2_soap_builder_t *builder,
                                             const axis2_env_t *env,
                                             axis2_bool_t value);
        
        axis2_status_t (AXIS2_CALL *set_processing_detail_elements)
                                    (axis2_soap_builder_t *builder,
                                     const axis2_env_t *env,
                                     axis2_bool_t value);
                                     
        axis2_bool_t (AXIS2_CALL *is_processing_detail_elements)
                                    (axis2_soap_builder_t *builder, 
                                     const axis2_env_t *env);
                                     
                                     
        int (AXIS2_CALL *get_soap_version)(axis2_soap_builder_t *builder,
                                           const axis2_env_t *env);
                                           
        axis2_status_t (AXIS2_CALL *process_namespace_data)
                                          (axis2_soap_builder_t *builder,
                                           const axis2_env_t *env,
                                           axis2_om_node_t *om_node,
                                           axis2_bool_t is_soap_element);
        axis2_status_t (AXIS2_CALL *
        set_mime_body_parts)
                                    (axis2_soap_builder_t *builder, 
                                     const axis2_env_t *env,
                                     axis2_hash_t *map); 
};
                                                      

/**
 * \brief soap_body struct
 * represent a soap_body
 */
 
struct axis2_soap_builder
{
    /** operation of axis2_soap_body struct */
    axis2_soap_builder_ops_t *ops;
};

/**
 * creates a axis2_soap_builder struct
 * @param env Environment. MUST NOT be NULL
 */

AXIS2_EXTERN axis2_soap_builder_t * AXIS2_CALL
axis2_soap_builder_create(const axis2_env_t *env,
                                axis2_om_stax_builder_t *builder,
                                axis2_char_t *soap_version);

/******************** Macros **************************************************/
    
#define AXIS2_SOAP_BUILDER_FREE(builder, env) \
        ((builder)->ops->free(builder, env))

#define AXIS2_SOAP_BUILDER_GET_SOAP_ENVELOPE(builder, env) \
        ((builder)->ops->get_soap_envelope(builder, env))
        
#define AXIS2_SOAP_BUILDER_GET_DOCUMENT_ELEMENT(builder, env) \
        ((builder)->ops->get_document_element(builder, env))
        
#define AXIS2_SOAP_BUILDER_NEXT(builder, env) \
        ((builder)->ops->next(builder, env))
        
#define AXIS2_SOAP_BUILDER_GET_DOCUMENT(builder, env) \
        ((builder)->ops->get_document(builder, env))
        
#define AXIS2_SOAP_BUILDER_NEXT(builder, env) \
        ((builder)->ops->next(builder, env)) 
        
#define AXIS2_SOAP_BUILDER_SET_BOOL_PROCESSING_MANDATORY_FAULT_ELEMENTS(builder, env, value) \
        ((builder)->ops->set_bool_processing_mandatory_fault_elements(builder, env, value))
 
#define AXIS2_SOAP_BUILDER_SET_PROCESSING_DETAIL_ELEMENTS(builder, env, value) \
        ((builder)->ops->set_processing_detail_elements(builder, env, value))
        
#define AXIS2_SOAP_BUILDER_IS_PROCESSING_DETAIL_ELEMENTS(builder, env) \
        ((builder)->ops->is_processing_detail_elements(builder, env))                         


#define AXIS2_SOAP_BUILDER_GET_SOAP_VERSION(builder, env) \
        ((builder)->ops->get_soap_version(builder, env))
        
#define AXIS2_SOAP_BUILDER_PROCESS_NAMESPACE_DATA(builder, env, om_node, is_soap_element) \
        ((builder)->ops->process_namespace_data(builder, env, om_node, is_soap_element))
        
#define AXIS2_SOAP_BUILDER_SET_MIME_BODY_PARTS(builder, env, map) \
        ((builder)->ops->set_mime_body_parts(builder, env, map))
        
/** @} */
#ifdef __cplusplus
}
#endif 
#endif /* AXIS2_SOAP_BUILDER_H */
