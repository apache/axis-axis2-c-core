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
#ifdef AXIS2_SSL_ENABLED

#include "ssl_utils.h"
#include <openssl/err.h>


/* compatibility functions for openssl < 1.1.0 */
#if OPENSSL_VERSION_NUMBER < 0x10100000L

STACK_OF(X509_OBJECT) * X509_STORE_get0_objects(X509_STORE *st)
{
    if (st)
    {
        return st->objs;
    }
    return NULL;
};

X509 * X509_OBJECT_get0_X509(const X509_OBJECT *obj)
{
    if (obj)
    {
        return (obj->data).x509;
    }
    return NULL;
};

void X509_get0_signature(const ASN1_BIT_STRING **psig,
                         const X509_ALGOR **palg,
                         const X509 *x)
{
    if (x) {
        *psig = x->signature;
        *palg = x->sig_alg;
    }
}

int ASN1_STRING_cmp(const ASN1_BIT_STRING *a, const ASN1_BIT_STRING *b)
{
    return M_ASN1_BIT_STRING_cmp(a, b);
}

#endif



BIO *bio_err = 0;

static int
password_cb(
    char *buf,
    int size,
    int rwflag,
    void *passwd)
{
    strncpy(buf, (char *) passwd, size);
    buf[size - 1] = '\0';
    return (int)(strlen(buf));
    /* We are sure that the difference lies within the int range */
}

AXIS2_EXTERN SSL_CTX *AXIS2_CALL
axis2_ssl_utils_initialize_ctx(
    const axutil_env_t * env,
    axis2_char_t * server_cert,
    axis2_char_t * key_file,
    axis2_char_t * ssl_pp)
{
    SSL_CTX *ctx = NULL;
    axis2_char_t *ca_file = server_cert;
	SSL_METHOD *meth;
    if (!ca_file)
    {
        AXIS2_LOG_INFO(env->log, "[ssl client] CA certificate not specified");
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_SSL_NO_CA_FILE, AXIS2_FAILURE);
        return NULL;
    }

    if (!bio_err)
    {
        /* Global system initialization */
        SSL_library_init();
        SSL_load_error_strings();
	OpenSSL_add_all_algorithms();

        /* An error write context */
        bio_err = BIO_new_fp(stderr, BIO_NOCLOSE);
    }

    /* Create our context
     *
     * Security hardening (CVE prevention):
     * - Use TLS_client_method() for OpenSSL 1.1.0+ which auto-negotiates
     *   the highest available TLS version
     * - For older OpenSSL, use SSLv23_method() but disable SSLv2/SSLv3
     *   to prevent POODLE and DROWN attacks
     */
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
    /* OpenSSL 1.1.0+: TLS_client_method() is the modern approach */
    meth = TLS_client_method();
#else
    /* OpenSSL < 1.1.0: SSLv23_method() with protocol restrictions */
    meth = SSLv23_method();
#endif
    ctx = SSL_CTX_new(meth);

    if (!ctx)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[ssl client] Failed to create SSL context");
        return NULL;
    }

    /* Disable insecure SSL protocols (SSLv2, SSLv3) to prevent:
     * - POODLE attack (CVE-2014-3566)
     * - DROWN attack (CVE-2016-0800)
     * - Other SSLv2/v3 vulnerabilities
     * Also enable other security options:
     * - SSL_OP_NO_COMPRESSION: Prevent CRIME attack (CVE-2012-4929)
     */
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_COMPRESSION);

    /* Load our keys and certificates
     * If we need client certificates it has to be done here
     */
    if (key_file) /*can we check if the server needs client auth? */
    {
        if (!ssl_pp)
        {
            AXIS2_LOG_INFO(env->log,
                "[ssl client] No passphrase specified for \
key file %s and server cert %s", key_file, server_cert);
        }

        SSL_CTX_set_default_passwd_cb_userdata(ctx, (void *) ssl_pp);
        SSL_CTX_set_default_passwd_cb(ctx, password_cb);

        if (!(SSL_CTX_use_certificate_chain_file(ctx, key_file)))
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "[ssl client] Loading client certificate failed \
, key file %s", key_file);
            SSL_CTX_free(ctx);
            return NULL;
        }

        if (!(SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM)))
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "[ssl client] Loading client key failed, key file \
%s", key_file);
            SSL_CTX_free(ctx);
            return NULL;
        }
    }
    else
    {
        AXIS2_LOG_INFO(env->log,
            "[ssl client] Client certificate chain file"
            "not specified");
    }

    /* Load the CAs we trust */
    if (!(SSL_CTX_load_verify_locations(ctx, ca_file, 0) ||  (!SSL_CTX_set_default_verify_paths(ctx))))
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[ssl client] Loading CA certificate failed, \
ca_file is %s", ca_file);
        SSL_CTX_free(ctx);
        return NULL;
    }

    return ctx;
}

AXIS2_EXTERN SSL *AXIS2_CALL
axis2_ssl_utils_initialize_ssl(
    const axutil_env_t * env,
    SSL_CTX * ctx,
    axis2_socket_t socket,
    axis2_char_t * host)
{
    SSL *ssl = NULL;
    BIO *sbio = NULL;

    AXIS2_PARAM_CHECK(env->error, ctx, NULL);

    ssl = SSL_new(ctx);
    if (!ssl)
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI,
            "[ssl]unable to create new ssl context");
        return NULL;
    }

    sbio = BIO_new_socket((int)socket, BIO_NOCLOSE);
    if (!sbio)
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI,
            "[ssl]unable to create BIO new socket for socket %d",
            (int)socket);
        SSL_free(ssl);
        return NULL;
    }

    SSL_set_bio(ssl, sbio, sbio);
    if (SSL_connect(ssl) <= 0)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_SSL_ENGINE, AXIS2_FAILURE);
        SSL_free(ssl);
        return NULL;
    }

    if (SSL_get_verify_result(ssl) != X509_V_OK)
    {
        char sslerror[128]; /** error buffer must be at least 120 bytes long */
        X509 *peer_cert = NULL;
        X509_STORE *cert_store = NULL;
        X509_NAME *peer_name = NULL;
        X509_OBJECT *client_object = NULL;
        X509 *client_cert = NULL;

        peer_cert = SSL_get_peer_certificate(ssl);
        if (peer_cert)
        {
            peer_name = X509_get_subject_name(peer_cert);
        }

        cert_store = SSL_CTX_get_cert_store(ctx);
        if (peer_name && cert_store)
        {
            client_object = X509_OBJECT_retrieve_by_subject(
                    X509_STORE_get0_objects(cert_store),
                    X509_LU_X509,
                    peer_name);
        }

        if (client_object)
        {
            client_cert = X509_OBJECT_get0_X509(client_object);
            if (client_cert)
            {
                const ASN1_BIT_STRING *peer_sig = NULL;
                const X509_ALGOR *peer_alg = NULL;

                const ASN1_BIT_STRING *client_sig = NULL;
                const X509_ALGOR *client_alg = NULL;

                X509_get0_signature(&peer_sig, &peer_alg, peer_cert);
                X509_get0_signature(&client_sig, &client_alg, client_cert);

                if (ASN1_STRING_cmp(peer_sig, client_sig) == 0)
                {
                    if (peer_cert) {
                        /* if the caller passed a hostname, verify it against the cert */
                        if (host) {
                            if (X509_check_host(peer_cert, host, strlen(host), 0, NULL) == 1) {
                                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                        "[ssl client] peer name matches certificate CN/SAN");
                            } else {
                                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                                        "[ssl client] peer name does not match certificate CN/SAN");
                                X509_free(peer_cert);
                                SSL_free(ssl);
                                return NULL;
                            }
                        }
                        X509_free(peer_cert);
                    }
                    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                            "[ssl client] SSL certificate verified against peer");
                    return ssl;
                }
            }
        }
        if (peer_cert)
        {
            X509_free(peer_cert);
        }
        ERR_error_string(SSL_get_verify_result(ssl), sslerror);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[ssl client] SSL certificate verification failed (%s)",
            sslerror);
        SSL_free(ssl);
        return NULL;
    }
    else {
        /* X509_V_OK means verification succeeded or no peer cert was presented.
         * We need to check which is the case, so let's see if there's a
         * peer cert.
         */
        X509 *peer_cert = NULL;
        peer_cert = SSL_get_peer_certificate(ssl);
        if (peer_cert) {
            /* if the caller passed a hostname, verify it against the cert */
            if (host) {
                if (X509_check_host(peer_cert, host, strlen(host), 0, NULL) == 1) {
                    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                            "[ssl client] peer name matches certificate CN/SAN");
                } else {
                    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                            "[ssl client] peer name does not match certificate CN/SAN");
                    X509_free(peer_cert);
                    SSL_free(ssl);
                    return NULL;
                }
            }

            X509_free(peer_cert);
        }

    }

    return ssl;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_ssl_utils_cleanup_ssl(
    const axutil_env_t * env,
    SSL_CTX * ctx,
    SSL * ssl)
{

    if (ssl)
    {
        int shutdown_ret;

        /* AXIS2C-1605 FIX: Proper bidirectional SSL shutdown
         *
         * SSL_shutdown() return values:
         *   0  = Shutdown in progress (sent close_notify, awaiting peer's)
         *   1  = Shutdown complete (bidirectional)
         *   <0 = Error
         *
         * To prevent sockets from being left in CLOSE_WAIT state, we must
         * call SSL_shutdown() twice when the first call returns 0:
         * - First call: sends our close_notify to the peer
         * - Second call: waits to receive peer's close_notify
         *
         * Without the second call, the peer's close_notify arrives after
         * we've freed the SSL object, leaving the socket in CLOSE_WAIT.
         */
        shutdown_ret = SSL_shutdown(ssl);

        if (shutdown_ret == 0)
        {
            /* Shutdown not yet complete - call again to receive peer's close_notify */
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                "[ssl] Performing bidirectional shutdown (waiting for peer close_notify)");
            SSL_shutdown(ssl);
        }
        else if (shutdown_ret < 0)
        {
            /* Log error but continue with cleanup */
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                "[ssl] SSL_shutdown returned error: %d", shutdown_ret);
        }

        SSL_free(ssl);
    }
    if (ctx)
    {
        SSL_CTX_free(ctx);
    }
    return AXIS2_SUCCESS;
}

#endif

