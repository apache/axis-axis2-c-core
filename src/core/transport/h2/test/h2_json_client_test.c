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

/*
 * Tests for the HTTP/2 JSON client (axis2_h2_json_client.h).
 *
 * The offline checks always run. The live checks need a real Axis2/C server
 * speaking JSON over HTTP/2 with TLS, and run only when it is named:
 *
 *   AXIS2_H2_TEST_HOST        address to connect to
 *   AXIS2_H2_TEST_PORT        port
 *   AXIS2_H2_TEST_CA          PEM CA the server certificate chains to
 *   AXIS2_H2_TEST_CERT/_KEY   client certificate and key, if the server wants mTLS
 *   AXIS2_H2_TEST_NAME        name the server certificate carries, if not HOST
 *   AXIS2_H2_TEST_PATH        operation path (default: the financial benchmark
 *                             sample's portfolioVariance)
 */

#include <axis2_h2_json_client.h>
#include <axutil_env.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

#define CHECK(cond, what) do { \
        if (cond) printf("  ok    %s\n", what); \
        else { printf("  FAIL  %s\n", what); failures++; } } while (0)

static const char *PV_BODY =
    "{\"n_assets\":2,\"weights\":[0.5,0.5],"
    "\"covariance_matrix\":[[0.04,0.01],[0.01,0.09]],\"n_periods_per_year\":1}";

static void
offline_checks(const axutil_env_t *env)
{
    axis2_h2_json_client_options_t o;
    axis2_h2_json_client_t *c;
    axis2_char_t *resp = (axis2_char_t *)"sentinel";
    int status = -1;

    printf("offline\n");

    memset(&o, 0, sizeof(o));
    o.port = 443;
    o.ca_file = "/nonexistent-ca.pem";
    CHECK(axis2_h2_json_client_create(env, &o) == NULL, "refuses a missing host");

    o.host = "127.0.0.1";
    o.port = 0;
    CHECK(axis2_h2_json_client_create(env, &o) == NULL, "refuses port 0");

    o.port = 443;
    o.ca_file = NULL;
    CHECK(axis2_h2_json_client_create(env, &o) == NULL, "refuses a missing ca_file");

    o.ca_file = "/nonexistent-ca.pem";
    CHECK(axis2_h2_json_client_create(env, &o) == NULL, "refuses a ca_file it cannot load");

    o.cert_file = "/nonexistent-cert.pem";
    CHECK(axis2_h2_json_client_create(env, &o) == NULL, "refuses cert_file without key_file");
    o.cert_file = NULL;

    CHECK(axis2_h2_json_client_create(env, NULL) == NULL, "refuses NULL options");
    axis2_h2_json_client_free(NULL, env);
    CHECK(1, "free(NULL) is harmless");

    /* A connect failure must be a clean FAILURE with a reason, not a hang. */
    {
        const char *ca = getenv("AXIS2_H2_TEST_CA");
        if (ca)
        {
            o.ca_file = ca;
            o.host = "127.0.0.1";
            o.port = 1;                  /* nothing listens on tcpmux */
            o.connect_timeout_ms = 2000;
            c = axis2_h2_json_client_create(env, &o);
            CHECK(c != NULL, "creates with a loadable CA");
            if (c)
            {
                axis2_status_t s = axis2_h2_json_client_post(c, env, "/services/X/op",
                    "{}", 2, &resp, NULL, &status);
                CHECK(s == AXIS2_FAILURE, "a refused connection fails");
                CHECK(resp == NULL && status == 0, "and clears the outputs");
                CHECK(strstr(axis2_h2_json_client_get_error(c), "cannot connect") != NULL,
                      "and says why");
                s = axis2_h2_json_client_post(c, env, "no-slash", "{}", 2, &resp, NULL, NULL);
                CHECK(s == AXIS2_FAILURE, "refuses a path without a leading '/'");
                axis2_h2_json_client_free(c, env);
            }
        }
        else
            printf("  skip  connect-failure checks (set AXIS2_H2_TEST_CA)\n");
    }
}

static void
fill_live_options(axis2_h2_json_client_options_t *o)
{
    memset(o, 0, sizeof(*o));
    o->host = getenv("AXIS2_H2_TEST_HOST");
    o->port = atoi(getenv("AXIS2_H2_TEST_PORT"));
    o->ca_file = getenv("AXIS2_H2_TEST_CA");
    o->cert_file = getenv("AXIS2_H2_TEST_CERT");
    o->key_file = getenv("AXIS2_H2_TEST_KEY");
    o->verify_name = getenv("AXIS2_H2_TEST_NAME");
}

static void
live_checks(const axutil_env_t *env)
{
    axis2_h2_json_client_options_t o;
    axis2_h2_json_client_t *c;
    const char *path = getenv("AXIS2_H2_TEST_PATH");
    axis2_char_t *resp = NULL;
    size_t len = 0;
    int status = 0, i;

    if (!getenv("AXIS2_H2_TEST_HOST") || !getenv("AXIS2_H2_TEST_PORT") ||
        !getenv("AXIS2_H2_TEST_CA"))
    {
        printf("live\n  skip  no server named (set AXIS2_H2_TEST_HOST, _PORT, _CA)\n");
        return;
    }
    if (!path)
        path = "/services/FinancialBenchmarkService/portfolioVariance";
    printf("live: %s:%s%s\n", getenv("AXIS2_H2_TEST_HOST"), getenv("AXIS2_H2_TEST_PORT"), path);

    fill_live_options(&o);
    c = axis2_h2_json_client_create(env, &o);
    CHECK(c != NULL, "creates");
    if (!c)
        return;

    /* Three posts on one client: the connection is opened once and reused. */
    for (i = 0; i < 3; i++)
    {
        axis2_status_t s = axis2_h2_json_client_post(c, env, path, PV_BODY, strlen(PV_BODY),
                                                     &resp, &len, &status);
        char label[64];
        snprintf(label, sizeof(label), "post %d answers 200 with a body", i + 1);
        CHECK(s == AXIS2_SUCCESS && status == 200 && resp && len > 0, label);
        if (s != AXIS2_SUCCESS)
            printf("        error: %s\n", axis2_h2_json_client_get_error(c));
        if (resp)
        {
            if (i == 0)
                printf("        %.*s\n", (int)(len < 200 ? len : 200), resp);
            CHECK(strlen(resp) == len, "body is NUL-terminated at its length");
            AXIS2_FREE(env->allocator, resp);
            resp = NULL;
        }
    }

    /* A response larger than the cap is abandoned, not truncated. */
    axis2_h2_json_client_free(c, env);
    fill_live_options(&o);
    o.max_response_bytes = 8;
    c = axis2_h2_json_client_create(env, &o);
    if (c)
    {
        axis2_status_t s = axis2_h2_json_client_post(c, env, path, PV_BODY, strlen(PV_BODY),
                                                     &resp, &len, &status);
        CHECK(s == AXIS2_FAILURE && resp == NULL, "a response over max_response_bytes fails");
        CHECK(strstr(axis2_h2_json_client_get_error(c), "exceeded") != NULL, "and says why");
        /* ...and the client recovers on the next call with a sane limit. */
        axis2_h2_json_client_free(c, env);
    }

    /* The certificate name is checked: a name the certificate does not carry
     * is refused even though the chain is good. */
    fill_live_options(&o);
    o.verify_name = "not-the-server.invalid";
    c = axis2_h2_json_client_create(env, &o);
    if (c)
    {
        axis2_status_t s = axis2_h2_json_client_post(c, env, path, PV_BODY, strlen(PV_BODY),
                                                     &resp, &len, &status);
        CHECK(s == AXIS2_FAILURE, "a certificate that does not carry verify_name is refused");
        printf("        error: %s\n", axis2_h2_json_client_get_error(c));
        axis2_h2_json_client_free(c, env);
    }
}

int
main(void)
{
    axutil_allocator_t *allocator = axutil_allocator_init(NULL);
    axutil_error_t *error = axutil_error_create(allocator);
    axutil_log_t *log = axutil_log_create(allocator, NULL, "h2_json_client_test.log");
    axutil_env_t *env = axutil_env_create_with_error_log(allocator, error, log);

    offline_checks(env);
    live_checks(env);

    printf("%s: %d failure(s)\n", failures ? "FAILED" : "PASSED", failures);
    axutil_env_free(env);
    return failures ? 1 : 0;
}
