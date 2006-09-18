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
#include "ssl_utils.h"
BIO *bio_err = 0;

AXIS2_EXTERN SSL_CTX* AXIS2_CALL
axis2_ssl_utils_initialize_ctx(const axis2_env_t *env)
{
    SSL_METHOD *meth = NULL;
    axis2_char_t *ca_file = NULL;
    SSL_CTX *ctx = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    /*TODO getenv */
    ca_file = AXIS2_GETENV("AXIS2_SSL_CA_FILE");
    if (NULL == ca_file)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SSL_NO_CA_FILE,
                AXIS2_FAILURE);
        return NULL;
    }
    if (!bio_err)
    {
        /* Global system initialization*/
        SSL_library_init();
        SSL_load_error_strings();

        /* An error write context */
        bio_err = BIO_new_fp(stderr, BIO_NOCLOSE);
    }

    /* Create our context*/
    meth = SSLv23_method();
    ctx = SSL_CTX_new(meth);

    /* Load our keys and certificates
     * If we need client certificates it has to be done here
     */
    /*if(!(SSL_CTX_use_certificate_chain_file(ctx, keyfile)))
    {
        SSL_CTX_free(ctx);
        return NULL;
    }*/
    /* Load the CAs we trust*/
    if (!(SSL_CTX_load_verify_locations(ctx, ca_file, 0)))
    {
        SSL_CTX_free(ctx);
        return NULL;
    }

#if (OPENSSL_VERSION_NUMBER < 0x00905100L)
    SSL_CTX_set_verify_depth(ctx, 1);
#endif

    return ctx;
}

AXIS2_EXTERN SSL* AXIS2_CALL
axis2_ssl_utils_initialize_ssl(const axis2_env_t *env, SSL_CTX *ctx,
        axis2_socket_t socket)
{
    SSL *ssl = NULL;
    BIO *sbio = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ctx, NULL);

    ssl = SSL_new(ctx);
    if (NULL == ssl)
    {
        return NULL;
    }
    sbio = BIO_new_socket(socket, BIO_NOCLOSE);
    if (NULL == sbio)
    {
        return NULL;
    }
    SSL_set_bio(ssl, sbio, sbio);
    if (SSL_connect(ssl) <= 0)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SSL_ENGINE,
                AXIS2_FAILURE);
        return NULL;
    }
    return ssl;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_ssl_utils_cleanup_ssl(const axis2_env_t *env, SSL_CTX *ctx, SSL *ssl)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (NULL != ssl)
    {
        SSL_shutdown(ssl);
    }
    if (NULL != ctx)
    {
        SSL_CTX_free(ctx);
    }
    return AXIS2_SUCCESS;
}
