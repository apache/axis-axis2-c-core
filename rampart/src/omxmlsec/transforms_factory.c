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

#include <stdio.h>
#include <axis2_util.h>
#include <oxs_constants.h>
#include <oxs_error.h>
#include <oxs_buffer.h>
#include <oxs_transform.h>
#include <oxs_transforms_factory.h>
#include <oxs_buffer.h>

/*Functions that implements transforms*/
axis2_status_t AXIS2_CALL
oxs_transforms_exc_c14n(const axis2_env_t *env,
            axiom_node_t *input,
            oxs_tr_dtype_t input_dtype,
            axis2_char_t **output,
            oxs_tr_dtype_t **output_dtype)
{
    *output= "SAMPLE-SAMPLE-SAMPLE";
    /*output_dtype = OXS_TRANSFORM_TYPE_CHAR;*/

    return AXIS2_SUCCESS;
}

/*Public functions*/
AXIS2_EXTERN oxs_transform_t *AXIS2_CALL
oxs_transforms_factory_produce_transform(const axis2_env_t *env,
    axis2_char_t *id)
{
    oxs_transform_t *tr =  NULL;

    /*Inspect the id and produce a transform*/
    if(0== axis2_strcmp(id, OXS_HREF_TRANSFORM_XML_EXC_C14N)){
        tr = oxs_transform_create(env);
        oxs_transform_set_id(tr, env, id);
        oxs_transform_set_input_data_type(tr, env, OXS_TRANSFORM_TYPE_NODE);
        oxs_transform_set_output_data_type(tr, env, OXS_TRANSFORM_TYPE_CHAR);
        oxs_transform_set_transform_func(tr, env, oxs_transforms_exc_c14n);
    }else{
        /*Error we do not support*/
        return NULL;
    }
}

