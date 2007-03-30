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
 
#include <w2c_writer.h>
#include <axis2_string.h>
#include <w2c_string.h>
#include <w2c_engine_configuration.h>
#include <w2c_properties.h>

/* TODO:this would be moved to some header */
#define W2C_WRITER_EXTENSION_SUFFIX "extension"
#define W2C_WRITER_TEMPLATE_SUFFIX "template"
#define W2C_WRITER_CLASS "w2c_writer"
/** 
 * @brief
 */
typedef struct w2c_writer_impl
{
    w2c_writer_t writer;

    axis2_char_t* xslt_filename;
    axis2_char_t* out_filename;
    axis2_char_t* language;
    axis2_char_t* self_name;
    w2c_engine_configuration_t* config;
    w2c_config_property_loader_t* loader;
       
} w2c_writer_impl_t;

#define W2C_INTF_TO_IMPL(writer) \
    ((w2c_writer_impl_t *) writer)

/************************* Function prototypes ********************************/

w2c_writer_impl_t * AXIS2_CALL 
w2c_writer_create_impl (const axutil_env_t *env);

axis2_status_t AXIS2_CALL
w2c_writer_free(w2c_writer_t *writer,
       const axutil_env_t *env);

axis2_status_t AXIS2_CALL
w2c_writer_initialize(w2c_writer_t *writer,
       const axutil_env_t *env,
       w2c_config_property_loader_t* loader,
       w2c_engine_configuration_t* config);

axis2_char_t* AXIS2_CALL
w2c_writer_create_out_file(w2c_writer_t *writer,
       const axutil_env_t *env,
       axis2_char_t* package_name,
       axis2_char_t* file_name);

axis2_status_t AXIS2_CALL
w2c_writer_parse(w2c_writer_t *writer,
       const axutil_env_t *env,
       axiom_node_t* root);

/** protected methods */

axis2_char_t* 
w2c_writer_find_template ( w2c_writer_impl_t* writer_impl,
                           const axutil_env_t* env,
                           axutil_hash_t* lang_speci_map);

axis2_char_t*
w2c_writer_get_extension( w2c_writer_impl_t* writer_impl,
             const axutil_env_t* env);

axis2_status_t
w2c_writer_set_self_name( w2c_writer_impl_t* writer_impl,
             const axutil_env_t* env,
             axis2_char_t* self_name);

axis2_status_t 
w2c_writer_set_language(w2c_writer_impl_t *writer_impl,
       const axutil_env_t *env);

axis2_status_t
w2c_writer_load_template(w2c_writer_impl_t *writer_impl,
       const axutil_env_t *env);


/************************** End of function prototypes ************************/

