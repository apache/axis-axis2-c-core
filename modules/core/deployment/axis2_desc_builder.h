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

#ifndef AXIS2_DESC_BUILDER_H
#define AXIS2_DESC_BUILDER_H

/**
 * @file axis2_desc_builder.h
 * @brief Axis2 Description Builder interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>

#include "axis2_deployment.h"
#include "axis2_dep_engine.h"
#include <axis2_flow.h>
#include <axiom_stax_builder.h>
#include <axiom_document.h>
#include <axiom_element.h>
#include <axiom_node.h>
#include <axis2_param_container.h>
#include <axis2_handler_desc.h>
#include <axiom_attribute.h>
#include <axiom_xml_reader.h>
#include <axis2_handler_desc.h>
#include <axis2_op.h>
#include <axiom_children_with_specific_attribute_iterator.h>
#include <axis2_phase.h> 
#include <axis2_dll_desc.h>


#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_flow;
struct axis2_phase;
struct axis2_dep_engine;
   
typedef struct axis2_desc_builder axis2_desc_builder_t;
typedef struct axis2_desc_builder_ops axis2_desc_builder_ops_t;

/** @defgroup axis2_desc_builder Description Builder
 * @ingroup axis2_deployment
 * @{
 */

/** 
 * @brief Description Builder ops struct
 * Encapsulator struct for ops of axis2_desc_builder
 */
AXIS2_DECLARE_DATA struct axis2_desc_builder_ops
{
   /** De-allocate memory
      * @return status code
      */
   axis2_status_t (AXIS2_CALL *
    free)(axis2_desc_builder_t *desc_builder,
           const axis2_env_t *env);
                               
     /**
     * This will creat OMElemnt for a given descrition document (axis2.xml , 
     * services.xml and module.xml)
     *
     * @return OMElement <code>OMElement</code>
     */
    axiom_node_t *(AXIS2_CALL *
    build_OM) (axis2_desc_builder_t *desc_builder,
                    const axis2_env_t *env);
    
    
    /**
     * To process Flow elements in services.xml
     * @param flow_element       <code>axiom_element_t</code>
     * @return flow
     */
    axis2_flow_t *(AXIS2_CALL *
    process_flow) (axis2_desc_builder_t *desc_builder,
                                    const axis2_env_t *env,
                                    axiom_element_t *flow_element,
                                    axis2_param_container_t *parent,
                                    axiom_node_t *node);
    
    /**
     *  To process Handler element
     * @param handler_element    <code>OMElement</code>
     * @return
     */
    axis2_handler_desc_t *(AXIS2_CALL *
    process_handler) (axis2_desc_builder_t *desc_builder,
                                        const axis2_env_t *env,
                                        axiom_node_t *handler_element,
                                        axis2_param_container_t *parent);
    
    /**
     * To get the Param out from the OM
     * @param params <code>axis2_param_t</code>
     * @param param_container <code>axis2_param_container_t</code>
     * @param parent <code>axis2_param_container_t</code>
     */
    axis2_status_t (AXIS2_CALL *
    process_params) (axis2_desc_builder_t *desc_builder,
                                    const axis2_env_t *env,
                                    axiom_children_qname_iterator_t *params,
                                    axis2_param_container_t *param_container,
                                    axis2_param_container_t *parent );
    
    
    axis2_status_t (AXIS2_CALL *
    process_op_module_refs) (axis2_desc_builder_t *desc_builder,
                              const axis2_env_t *env,
                              axiom_children_qname_iterator_t *module_refs, 
                              axis2_op_t *op);
                                  
    axis2_msg_recv_t *(AXIS2_CALL *
    load_msg_recv) (axis2_desc_builder_t *desc_builder,
                                        const axis2_env_t *env,
                                        axiom_element_t *recv_element);
    
    /**
     * This method is used to retrive service name form the arechive file name
     * if the archive file name is service1.aar , then axis service name would be service1
     *
     * @param file_name
     * @return axis2_char_t *
     */
    axis2_char_t *(AXIS2_CALL *
    get_short_file_name) (
        axis2_desc_builder_t *desc_builder,
        const axis2_env_t *env,
        axis2_char_t *file_name);
    
    axis2_char_t *(AXIS2_CALL *
    get_file_name_without_prefix) (
        axis2_desc_builder_t *desc_builder,
        const axis2_env_t *env,
        axis2_char_t *short_file_name);    
 
 /**
     * this method is to get the value of attribue
     * eg xsd:anyVal --> anyVal
     *
     * @return String
     */
    axis2_char_t *(AXIS2_CALL *
    get_value) (
        axis2_desc_builder_t *desc_builder,
        const axis2_env_t *env,
        axis2_char_t *in);
                                
};

struct axis2_msg_recv *AXIS2_CALL
axis2_desc_builder_load_default_msg_recv(
    const axis2_env_t *env);

/** 
 * @brief Description Builder struct 
 */  
AXIS2_DECLARE_DATA struct axis2_desc_builder
{
   axis2_desc_builder_ops_t *ops;
    struct axis2_dep_engine *engine;
};

/**
 * Creates description builder struct
 * @return pointer to newly created description builder
 */
AXIS2_EXTERN axis2_desc_builder_t * AXIS2_CALL
axis2_desc_builder_create (const axis2_env_t *env);

/**
 * Creates description builder struct
 * @param engine
 * @return pointer to newly created description builder
 */
AXIS2_EXTERN axis2_desc_builder_t * AXIS2_CALL
axis2_desc_builder_create_with_dep_engine (
                                        const axis2_env_t *env,
                                        struct axis2_dep_engine *engine);
/**
 * Creates description builder struct
 * @param file_name
 * @param engine                                        
 * @return pointer to newly created description builder
 */
AXIS2_EXTERN axis2_desc_builder_t * AXIS2_CALL 
axis2_desc_builder_create_with_file_and_dep_engine (
                                        const axis2_env_t *env, 
                                        axis2_char_t *file_name, 
                                        struct axis2_dep_engine *engine);

/*************************** Function macros **********************************/

#define AXIS2_DESC_BUILDER_FREE(desc_builder, env) \
      ((desc_builder)->ops->free (desc_builder, env))

#define AXIS2_DESC_BUILDER_BUILD_OM(desc_builder, env) \
      ((desc_builder)->ops->build_OM (desc_builder, env))                                        

#define AXIS2_DESC_BUILDER_PROCESS_FLOW(desc_builder, env, flow_element, \
        parent, node) ((desc_builder)->ops->process_flow (desc_builder, env, \
            flow_element, parent, node))                                          

#define AXIS2_DESC_BUILDER_PROCESS_HANDLER(desc_builder, env, handler_element, \
        parent) ((desc_builder)->ops->process_handler (desc_builder, \
            env, handler_element, parent))                                          

#define AXIS2_DESC_BUILDER_PROCESS_PARAMS(desc_builder, env, params, \
        param_container, parent) ((desc_builder)->ops->process_params \
            (desc_builder, env, params, param_container, parent))                                          

#define AXIS2_DESC_BUILDER_PROCESS_OP_MODULE_REFS(desc_builder, env, \
        module_refs, op) ((desc_builder)->ops->process_op_module_refs \
            (desc_builder, env, module_refs, op))                                          

#define AXIS2_DESC_BUILDER_LOAD_MSG_RECV(desc_builder, env, recv_element) \
      ((desc_builder)->ops->load_msg_recv (desc_builder, env, recv_element))  

#define AXIS2_DESC_BUILDER_GET_SHORT_FILE_NAME(desc_builder, env, file_name) \
      ((desc_builder)->ops->get_short_file_name (desc_builder, env, file_name))  

#define AXIS2_DESC_BUILDER_GET_FILE_NAME_WITHOUT_PREFIX(desc_builder, env, file_name) \
      ((desc_builder)->ops->get_file_name_without_prefix (desc_builder, env, file_name))  

#define AXIS2_DESC_BUILDER_GET_VALUE(desc_builder, env, in) \
      ((desc_builder)->ops->get_value (desc_builder, env, in))  
                                        
/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_DESC_BUILDER_H */
