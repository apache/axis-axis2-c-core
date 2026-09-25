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

#ifndef AXIS2_H2_JSON_CLIENT_H
#define AXIS2_H2_JSON_CLIENT_H

/**
 * @file axis2_h2_json_client.h
 * @brief JSON over HTTP/2, the client half of the HTTP/2 JSON transport.
 *
 * Posts a JSON body to one operation of an Axis2/C service (for example
 * /services/FinancialBenchmarkService/portfolioVariance) and returns the JSON
 * response. It is the counterpart of mod_axis2 serving JSON over mod_http2,
 * and depends only on axutil, nghttp2 and OpenSSL -- no engine, no axiom --
 * so an embedded or mobile caller links what it uses.
 *
 * HTTP/2 only. ALPN must select "h2"; a server that offers only HTTP/1.1 is
 * refused rather than spoken to, because Axis2/C serves JSON over HTTP/2 and
 * a silent downgrade would fail later and somewhere less obvious.
 *
 * TLS is mandatory and the server certificate is always verified, including
 * its name. A client certificate is presented when one is configured (mTLS).
 *
 * One client holds one connection, opened on the first post and reused by
 * later ones. A client is not thread-safe: use one per thread.
 */

#include <axutil_utils.h>
#include <axutil_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Opaque client. */
    typedef struct axis2_h2_json_client axis2_h2_json_client_t;

    /**
     * Connection and TLS settings. Strings are copied at create time.
     */
    typedef struct axis2_h2_json_client_options
    {
        /** Address to connect to: an IP literal or a DNS name. Required. */
        const axis2_char_t *host;

        /** TCP port. Required. */
        int port;

        /**
         * Name the server certificate must carry, and the TLS SNI and
         * :authority sent. NULL means host. Set it when connecting by IP to a
         * server whose certificate names it by DNS name -- the equivalent of
         * curl --resolve. Verification is never skipped.
         */
        const axis2_char_t *verify_name;

        /** PEM file of the CA(s) the server certificate must chain to. Required. */
        const axis2_char_t *ca_file;

        /** PEM client certificate (chain) for mTLS, or NULL for none. */
        const axis2_char_t *cert_file;

        /** PEM private key for cert_file. Required when cert_file is set. */
        const axis2_char_t *key_file;

        /** TCP connect and TLS handshake limit. 0 means 5000 ms. */
        int connect_timeout_ms;

        /** Longest wait for the server to make progress. 0 means 30000 ms. */
        int io_timeout_ms;

        /**
         * Longest a post may take from sending the request to receiving the
         * whole response, however steadily the server makes progress.
         * Connecting is bounded separately. 0 means 120000 ms.
         */
        int request_timeout_ms;

        /**
         * Largest response body accepted. 0 means 16 MB, the size of the
         * server's JSON transport buffer; a larger response is abandoned.
         */
        size_t max_response_bytes;
    } axis2_h2_json_client_options_t;

    /**
     * Create a client. No connection is made until the first post.
     * @return the client, or NULL if a required option is missing or invalid
     *         (the reason is logged).
     */
    AXIS2_EXTERN axis2_h2_json_client_t *AXIS2_CALL
    axis2_h2_json_client_create(
        const axutil_env_t *env,
        const axis2_h2_json_client_options_t *options);

    /**
     * POST a JSON body and wait for the whole response.
     *
     * @param path      request path, starting with '/'
     * @param json      request body; need not be NUL-terminated
     * @param json_len  its length in bytes
     * @param response_out  set to the response body, NUL-terminated, allocated
     *                      with env->allocator; the caller frees it. Set to
     *                      NULL on failure.
     * @param response_len_out  set to the body length (may be NULL)
     * @param http_status_out   set to the HTTP status (may be NULL)
     * @return AXIS2_SUCCESS when a complete response arrived, whatever its
     *         status -- a 500 from the service is still an answer. FAILURE
     *         for anything that prevented one: connect, TLS, ALPN, timeout,
     *         reset stream, or a response over the size limit. The reason is
     *         in axis2_h2_json_client_get_error. The connection is dropped
     *         after a failure and reopened by the next post; nothing is
     *         retried automatically, since a POST may not be safe to repeat.
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_h2_json_client_post(
        axis2_h2_json_client_t *client,
        const axutil_env_t *env,
        const axis2_char_t *path,
        const axis2_char_t *json,
        size_t json_len,
        axis2_char_t **response_out,
        size_t *response_len_out,
        int *http_status_out);

    /**
     * Why the last call failed, or "" after a success. Owned by the client;
     * valid until the next call.
     */
    AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
    axis2_h2_json_client_get_error(
        const axis2_h2_json_client_t *client);

    /** Close the connection and free the client. NULL is ignored. */
    AXIS2_EXTERN void AXIS2_CALL
    axis2_h2_json_client_free(
        axis2_h2_json_client_t *client,
        const axutil_env_t *env);

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_H2_JSON_CLIENT_H */
