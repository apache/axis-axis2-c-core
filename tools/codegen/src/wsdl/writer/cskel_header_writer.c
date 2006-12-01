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
 
#include "writer_protected.h"
#include <w2c_file_writer.h>

#define W2C_CSKEL_HEADER_WRITER_CLASS "w2c_cskel_header_writer"
/************************* overrided functions ********************************/
axis2_char_t* AXIS2_CALL
w2c_cskel_header_writer_create_out_file(w2c_writer_t *writer,
       const axis2_env_t *env,
       axis2_char_t* package_name,
       axis2_char_t* file_name)
{
    w2c_writer_impl_t* writer_impl = NULL;
    axis2_char_t* output_locat = NULL;

	AXIS2_ENV_CHECK(env, NULL);
    writer_impl = W2C_INTF_TO_IMPL(writer);

    /* here the package_name would be build */
    package_name = "";
    output_locat = W2C_ENGINE_CONFIGURATION_GET_OUTPUT_LOCATION (
                             writer_impl-> config, env);
    writer_impl ->out_filename =
              w2c_file_writer_create_classfile_name (env, output_locat,
                               package_name,
                               file_name,
                               ".h");

    return writer_impl ->out_filename;
}

/****************** standard create and delete for DLL ************************/
AXIS2_EXPORT int
axis2_get_instance(w2c_writer_t **inst,
                   const axis2_env_t *env)
{
    w2c_writer_impl_t* writer_impl = NULL;
 
    AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
    writer_impl = w2c_writer_create_impl (env);
    if(!writer_impl)
    {
        return AXIS2_FAILURE;
    }
    w2c_writer_set_self_name ( writer_impl, env, W2C_CSKEL_HEADER_WRITER_CLASS );
    /* bit of overriding */
    writer_impl->writer.ops->create_out_file = 
                      w2c_cskel_header_writer_create_out_file;

    *inst = &(writer_impl->writer);

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int
axis2_remove_instance(w2c_writer_t *inst,
                      const axis2_env_t *env)
{
   axis2_status_t status = AXIS2_FAILURE;

   AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
   if (inst)
   {
        status = W2C_WRITER_FREE(inst, env);
    }
    return status;
}

