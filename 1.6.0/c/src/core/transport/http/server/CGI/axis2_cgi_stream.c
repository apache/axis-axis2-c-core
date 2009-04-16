
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

#include <string.h>
#include <stdlib.h>
#include "axis2_cgi_stream.h"

/**
 * @brief Stream struct impl
 * Streaming mechanisms for cgi
 */

typedef struct cgi_stream_impl
{
    axutil_stream_t stream;
    axutil_stream_type_t stream_type;
    unsigned int cur_pos;
    unsigned int content_length;
}
cgi_stream_impl_t;

#define AXIS2_INTF_TO_IMPL(stream) ((cgi_stream_impl_t *)(stream))

axutil_stream_type_t AXIS2_CALL cgi_stream_get_type(
    axutil_stream_t * stream,
    const axutil_env_t * env);

int AXIS2_CALL cgi_stream_write(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    const void *buffer,
    size_t count);

int AXIS2_CALL cgi_stream_read(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    void *buffer,
    size_t count);

int AXIS2_CALL cgi_stream_skip(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    int count);

int AXIS2_CALL cgi_stream_get_char(
    axutil_stream_t * stream,
    const axutil_env_t * env);

axutil_stream_t *AXIS2_CALL
axutil_stream_create_cgi(
    const axutil_env_t * env,
	unsigned int content_length)
{
    cgi_stream_impl_t *stream_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
	AXIS2_PARAM_CHECK(env->error, content_length, NULL);

    stream_impl =
        (cgi_stream_impl_t *) AXIS2_MALLOC(env->allocator,
                                           sizeof(cgi_stream_impl_t));

    if (!stream_impl)
    {
        return NULL;
    }
    stream_impl->cur_pos = 0;
	stream_impl->content_length = content_length;
    stream_impl->stream_type = AXIS2_STREAM_MANAGED;

    axutil_stream_set_read(&(stream_impl->stream), env, cgi_stream_read);
    axutil_stream_set_write(&(stream_impl->stream), env, cgi_stream_write);
    axutil_stream_set_skip(&(stream_impl->stream), env, cgi_stream_skip);

    return &(stream_impl->stream);
}
int AXIS2_CALL
cgi_stream_read(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    void *buffer,
    size_t count)
{
    /*void *temp_buff = NULL;*/
    /*unsigned int data_to_read = 0;*/
    unsigned int read_bytes = 0;
	/*axis2_bool_t ret_ok = AXIS2_TRUE;*/

    cgi_stream_impl_t *stream_impl = NULL;
    /*char *temp = NULL;*/

    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
    stream_impl = (cgi_stream_impl_t *) stream;
	
	if (count < stream_impl->content_length)
	{
		read_bytes = fread(buffer, sizeof(char), count, stdin);
	}
	else
	{
		read_bytes = fread(buffer, sizeof(char), stream_impl->content_length, stdin);
	}

    return read_bytes;
}

int AXIS2_CALL
cgi_stream_write(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    const void *buf,
    size_t count)
{	
	/* Cannot write on cgi stdin, explicitly an input stream */

	return -1;
}

int AXIS2_CALL
cgi_stream_skip(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    int count)
{
	cgi_stream_impl_t *stream_impl = NULL;
    int skipped = -1;
    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
    stream_impl = AXIS2_INTF_TO_IMPL(stream);
	
	if (fseek(stdin, count, SEEK_CUR)==0) skipped = count;
    return skipped;
}

int AXIS2_CALL
cgi_stream_get_char(
    axutil_stream_t * stream,
    const axutil_env_t * env)
{
    int ret = -1;
    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);

    return ret;
}

axutil_stream_type_t AXIS2_CALL
cgi_stream_get_type(
    axutil_stream_t * stream,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
    return AXIS2_INTF_TO_IMPL(stream)->stream_type;
}
