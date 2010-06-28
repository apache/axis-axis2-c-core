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
#include "apache2_stream.h"
#include <http_protocol.h>

typedef struct apache2_stream_impl
{
    axutil_stream_t stream;
    axutil_stream_type_t stream_type;
    request_rec *request;
} apache2_stream_impl_t;

#define AXIS2_INTF_TO_IMPL(stream) ((apache2_stream_impl_t *)(stream))

axutil_stream_type_t AXIS2_CALL
apache2_stream_get_type(
    axutil_stream_t * stream,
    const axutil_env_t * env);

int AXIS2_CALL apache2_stream_write(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    const void *buffer,
    size_t count);

int AXIS2_CALL apache2_stream_read(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    void *buffer,
    size_t count);

static int AXIS2_CALL
apache2_stream_skip(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    int count);

int AXIS2_CALL apache2_stream_get_char(
    axutil_stream_t * stream,
    const axutil_env_t * env);

static apr_size_t
apache2_ap_get_client_block(
    request_rec *r,
    char* buffer, 
    apr_size_t bufsiz);

AXIS2_EXTERN axutil_stream_t *AXIS2_CALL
axutil_stream_create_apache2(
    const axutil_env_t * env,
    request_rec * request)
{
    apache2_stream_impl_t *stream_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, request, NULL);

    stream_impl = (apache2_stream_impl_t *)AXIS2_MALLOC(env->allocator,
        sizeof(apache2_stream_impl_t));

    if(!stream_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    memset(&(stream_impl->stream), 0, sizeof(axutil_stream_t));

    stream_impl->request = request;
    stream_impl->stream_type = AXIS2_STREAM_MANAGED;

    axutil_stream_set_read(&(stream_impl->stream), env, apache2_stream_read);
    axutil_stream_set_write(&(stream_impl->stream), env, apache2_stream_write);
    axutil_stream_set_skip(&(stream_impl->stream), env, apache2_stream_skip);

    return &(stream_impl->stream);
}

int AXIS2_CALL
apache2_stream_read(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    void *buffer,
    size_t count)
{
    apache2_stream_impl_t *stream_impl = NULL;
    size_t read = 0;
    size_t len = 0;

    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);

    stream_impl = AXIS2_INTF_TO_IMPL(stream);

    while(count - len > 0)
    {
        read = apache2_ap_get_client_block(stream_impl->request, (char *) buffer + len,
                                   count - len);
        if(read > 0 && read != 0xFFFFFFFF)
        {
            len += read;
        }
        else
        {
            break;
        }
    }

    return (int)len;
    /* We are sure that the difference lies within the int range */
}

int AXIS2_CALL
apache2_stream_write(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    const void *buf,
    size_t count)
{
    apache2_stream_impl_t *stream_impl = NULL;
    axis2_char_t *buffer = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
    AXIS2_PARAM_CHECK(env->error, buf, AXIS2_FAILURE);
    stream_impl = AXIS2_INTF_TO_IMPL(stream);
    buffer = (axis2_char_t *)buf;
    if(count <= 0)
    {
        return (int)count;
        /* We are sure that the difference lies within the int range */
    }
    /* assume that buffer is not null terminated */
    return ap_rwrite(buffer, (int)count, stream_impl->request);
    /* We are sure that the difference lies within the int range */
}

static int AXIS2_CALL
apache2_stream_skip(
    axutil_stream_t * stream,
    const axutil_env_t * env,
    int count)
{
    apache2_stream_impl_t *stream_impl = NULL;
    axis2_char_t *tmp_buffer = NULL;
    apr_size_t len = -1;
    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
    stream_impl = AXIS2_INTF_TO_IMPL(stream);

    tmp_buffer = AXIS2_MALLOC(env->allocator, count * sizeof(axis2_char_t));
    if(tmp_buffer == NULL)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return -1;
    }
    len = apache2_ap_get_client_block(stream_impl->request, tmp_buffer, count);
    AXIS2_FREE(env->allocator, tmp_buffer);
    return (int)len;

}

int AXIS2_CALL
apache2_stream_get_char(
    axutil_stream_t * stream,
    const axutil_env_t * env)
{
    int ret = -1;
    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);

    return ret;
}

axutil_stream_type_t AXIS2_CALL
apache2_stream_get_type(
    axutil_stream_t * stream,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
    return AXIS2_INTF_TO_IMPL(stream)->stream_type;
}

/*
 * This is a re-write of get_client_block found in http_filters.c in httpd
 * which does not work when dealing with compressed payloads (or any other input
 * filters that could potentially return 0 bytes of filtered data and not be at
 * the end of the stream).
 * get_client_block is called in a loop to get the request message body.
 * This is quite simple if the client includes a content-length
 * (the normal case), but gets messy if the body is chunked. Note that
 * r->remaining is used to maintain state across calls and that
 * r->read_length is the total number of bytes given to the caller
 * across all invocations.  It is messy because we have to be careful not
 * to read past the data provided by the client, since these reads block.
 * Returns 0 on End-of-body, -1 on error or premature chunk end.
 *
 */
static apr_size_t
apache2_ap_get_client_block(
    request_rec *r,
    char *buffer,
    apr_size_t bufsiz)
{
    apr_status_t rv;
    apr_bucket_brigade *bb;
    int loop = 1;
    apr_size_t origBufSize = bufsiz;

    if (r->remaining < 0 || (!r->read_chunked && r->remaining == 0)) {
        return 0;
    }

    bb = apr_brigade_create(r->pool, r->connection->bucket_alloc);
    if (bb == NULL) {
        r->connection->keepalive = AP_CONN_CLOSE;
        return -1;
    }

    /* we need to loop until the input filters (if any) give us data */
    while (loop) {
        rv = ap_get_brigade(r->input_filters, bb, AP_MODE_READBYTES,
                APR_BLOCK_READ, bufsiz);

        /* We lose the failure code here.  This is why ap_get_client_block should
         * not be used.
         */
        if (rv != APR_SUCCESS) {
            /* if we actually fail here, we want to just return and
             * stop trying to read data from the client.
             */
            r->connection->keepalive = AP_CONN_CLOSE;
            apr_brigade_destroy(bb);
            return -1;
        }

        /* If this fails, it means that a filter is written incorrectly and that
         * it needs to learn how to properly handle APR_BLOCK_READ requests by
         * returning data when requested.
         */
        AP_DEBUG_ASSERT(!APR_BRIGADE_EMPTY(bb));

        /* Check to see if EOS in the brigade.
         *
         * If so, we have to leave a nugget for the *next* ap_get_client_block
         * call to return 0.
         */
        if (APR_BUCKET_IS_EOS(APR_BRIGADE_LAST(bb))) {
            if (r->read_chunked) {
                r->remaining = -1;
            } else {
                r->remaining = 0;
            }
        }

        rv = apr_brigade_flatten(bb, buffer, &bufsiz);
        if (rv != APR_SUCCESS) {
            apr_brigade_destroy(bb);
            return -1;
        }

        /* XXX yank me? */
        r->read_length += bufsiz;

        /* it is possible that the entire bucket brigade is exhausted, but no data
         * has been produced by the input filter (mod_deflate, for example)....
         * in this scenario, we really need to keep looping
         */
        if (bufsiz != 0 || r->remaining <= 0) {
            loop = 0;
            apr_brigade_destroy(bb);
        } else {
            if (bufsiz == 0) {
                bufsiz = origBufSize;
            }
        }

    }

    return (long)bufsiz;
}
