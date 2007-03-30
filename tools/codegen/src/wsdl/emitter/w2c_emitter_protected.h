/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <w2c_emitter.h>
#include <axutil_array_list.h>
#include <axis2_string.h>
#include <w2c_string.h>
#include <w2c_engine_configuration.h>
#include <w2c_config_property_loader.h>
#include <axiom_document.h>
#include <w2c_writer.h>
#include <axutil_dll_desc.h>
#include <w2c_class_loader.h>
#include <w2c_messages.h>
#include <woden_reader.h>
#include <woden_resolver.h>
#include <w2c_qname2name_maker.h>

#define W2C_EMITTER_CALL_BACK_HANDLER_SUFFIX  "CallbackHandler"
#define W2C_EMITTER_STUB_SUFFIX  "Stub"
#define W2C_EMITTER_TEST_SUFFIX  "Test"
#define W2C_EMITTER_SKELETON_CLASS_SUFFIX  "Skeleton"
#define W2C_EMITTER_SKELETON_INTERFACE_SUFFIX  "SkeletonInterface"
#define W2C_EMITTER_MESSAGE_RECEIVER_SUFFIX  "MessageReceiver"
#define W2C_EMITTER_FAULT_SUFFIX  "Exception"
#define W2C_EMITTER_DATABINDING_SUPPORTER_NAME_SUFFIX  "DatabindingSupporter"

#define W2C_EMITTER_SRC_DIR_NAME  "src"
#define W2C_EMITTER_TEST_SRC_DIR_NAME  "test"
#define W2C_EMITTER_RESOURCE_SRC_DIR_NAME  "resources"

#define W2C_EMITTER_WRITER_DIR W2C_CLASS_LOADER_WRITER_PATH

typedef axis2_char_t* (*W2C_EMITTER_NAME_MAKER)
              ( axis2_char_t *name, const axutil_env_t *env);

/** 
 * @brief
 */
typedef struct w2c_emitter_impl
{
    w2c_emitter_t emitter;
    
    w2c_engine_configuration_t* config;
    w2c_config_property_loader_t* loader;
        
    void* desc;
    woden_resolver_t* resolver;
    axiom_node_t* common_node;
    W2C_EMITTER_NAME_MAKER name_maker_func;
    w2c_qname2name_maker_t *qname2name_maker;
} w2c_emitter_impl_t;


#define W2C_INTF_TO_IMPL(emitter) \
    ((w2c_emitter_impl_t *) emitter)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
w2c_emitter_free(w2c_emitter_t *emitter,
       const axutil_env_t *env);

axis2_status_t AXIS2_CALL
w2c_emitter_set_config(w2c_emitter_t *emitter,
       const axutil_env_t* env,
       w2c_engine_configuration_t* config,
       w2c_config_property_loader_t* loader );

axis2_status_t AXIS2_CALL
w2c_emitter_emit_stub(w2c_emitter_t *emitter,
       const axutil_env_t* env);

axis2_status_t AXIS2_CALL
w2c_emitter_emit_skel(w2c_emitter_t *emitter,
       const axutil_env_t* env);

/* implementations for protected functions */

axis2_status_t
w2c_emitter_write_class(w2c_emitter_impl_t* emitter_impl,
                      const axutil_env_t* env,
                      axiom_node_t* root,
                      w2c_writer_t* writer);

axis2_status_t
w2c_emitter_init( w2c_emitter_impl_t* emitter_impl,
                  const axutil_env_t *env);

w2c_emitter_impl_t*
w2c_emitter_create_impl( const axutil_env_t *env);

axiom_document_t*
get_root_element_from_filename(
        const axutil_env_t *env,
        axis2_char_t *filename);

axis2_status_t
w2c_emitter_parse_wsdl( w2c_emitter_impl_t* emitter_impl,
                        const axutil_env_t* env);

axis2_qname_t*
w2c_emitter_pick_service_name ( w2c_emitter_impl_t* emitter_impl,
                               const axutil_env_t* env);

axis2_status_t
w2c_emitter_load_services( w2c_emitter_impl_t* emitter_impl,
                            const axutil_env_t* env,
                            axiom_node_t* doc);

axis2_status_t
w2c_emitter_add_param( w2c_emitter_impl_t *emitter_impl,
                            const axutil_env_t *env,
                            axiom_node_t *param_direction,
                            axis2_qname_t *msg_qname);

axis2_char_t* w2c_emitter_default_namemaker( axis2_char_t *name,
                            const axutil_env_t *env);

axis2_status_t
w2c_emitter_init( w2c_emitter_impl_t* emitter_impl,
                  const axutil_env_t *env);

/************************** End of function prototypes ************************/

