/*
 *   Copyright 2003-2004 The Apache Software Foundation.
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
 */

#include <stdio.h>
#include <axis2_util.h>
#include <oxs_axis2_utils.h>


AXIS2_EXTERN oxs_buffer_ptr AXIS2_CALL  oxs_base64_encode(axis2_env_t *env,
    oxs_buffer_ptr in_buf    )
{
    axis2_char_t* encoded_str=NULL;
    oxs_buffer_ptr out_buf = NULL;

    int ret;

    encoded_str = AXIS2_MALLOC(env->allocator, axis2_base64_encode_len(in_buf->size));

    printf("\noxs_base64_encode\nIn Data Size= %d", in_buf->size);
    ret =  axis2_base64_encode(encoded_str, in_buf->data, in_buf->size);
    if(ret < 0) return (-1);
    
    printf("\nEncoded_str= %s", encoded_str);
    /*Set to the output buffer */
    
    out_buf = oxs_string_to_buffer(env, encoded_str);    
    return out_buf;
}

