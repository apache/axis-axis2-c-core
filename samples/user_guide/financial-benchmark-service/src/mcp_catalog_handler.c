/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file mcp_catalog_handler.c
 * @brief MCP catalog HTTP handler for Axis2/C — dynamic service introspection
 *
 * Mirrors Java's OpenApiSpecGenerator.generateMcpCatalogJson() exactly.
 *
 * Design notes:
 *
 * Parameter lookup order — matches the Java getMcpStringParam() / getMcpBoolParam():
 *   1. axis2_op_get_param(op, env, name)   — operation level
 *   2. axis2_svc_get_param(svc, env, name) — service level
 *   3. default value
 *
 * System service filter — excludes "Version", "AdminService", names starting
 * with "__", and names containing "AdminService" (same list as Java).
 *
 * Auth flag — services whose lowercased name contains "login" or equals
 * "adminconsole" set x-requiresAuth: false; all others: true.
 *
 * mcpInputSchema — if the parameter value is valid JSON it is embedded as a
 * parsed json_object tree.  If json_tokener_parse() fails, a WARN is logged
 * and the fallback empty schema is used — matching Java's behaviour.
 *
 * JSON safety — all string values passed to json-c go through
 * json_object_new_string(), which handles escaping internally.  No manual
 * string concatenation is used for JSON values.
 */

#include "mcp_catalog_handler.h"
#include "axis2_json_secure_fault.h"

#include <axis2_svc.h>
#include <axis2_op.h>
#include <axis2_conf.h>
#include <axutil_param.h>
#include <axutil_hash.h>
#include <axutil_qname.h>
#include <axutil_string.h>
#include <axutil_log.h>

#include <json-c/json.h>
#include <stdio.h>   /* snprintf */
#include <string.h>
#include <ctype.h>

/* ============================================================================
 * Internal helpers
 * ============================================================================
 */

/**
 * Retrieve a string-valued parameter, checking the operation first then the
 * service.  Returns default_val (which may be NULL) when neither has the param.
 *
 * Never returns a pointer to freed memory: the return value points into the
 * axutil_param_t's own storage (owned by the Axis2 config, not this function).
 */
static const axis2_char_t *
get_mcp_string_param(
    axis2_op_t         *op,
    axis2_svc_t        *svc,
    const axutil_env_t *env,
    const axis2_char_t *param_name,
    const axis2_char_t *default_val)
{
    axutil_param_t *p = NULL;

    /* Operation level first */
    p = axis2_op_get_param(op, env, param_name);
    if (!p)
        p = axis2_svc_get_param(svc, env, param_name);

    if (p) {
        void *val = axutil_param_get_value(p, env);
        if (val && ((axis2_char_t *)val)[0] != '\0')
            return (const axis2_char_t *)val;
    }
    return default_val;
}

/**
 * Retrieve a boolean-valued MCP parameter.
 * Accepts "true" / "false" (case-insensitive); anything else returns default_val.
 */
static axis2_bool_t
get_mcp_bool_param(
    axis2_op_t         *op,
    axis2_svc_t        *svc,
    const axutil_env_t *env,
    const axis2_char_t *param_name,
    axis2_bool_t        default_val)
{
    axutil_param_t *p = NULL;

    p = axis2_op_get_param(op, env, param_name);
    if (!p)
        p = axis2_svc_get_param(svc, env, param_name);

    if (p) {
        void *val = axutil_param_get_value(p, env);
        if (val) {
            const axis2_char_t *s = (const axis2_char_t *)val;
            if (axutil_strcasecmp(s, "true")  == 0) return AXIS2_TRUE;
            if (axutil_strcasecmp(s, "false") == 0) return AXIS2_FALSE;
        }
    }
    return default_val;
}

/**
 * Return AXIS2_TRUE if the service is an Axis2 internal service that should
 * be excluded from the MCP catalog.  Mirrors Java's isSystemService().
 */
static axis2_bool_t
is_system_service(const axis2_char_t *svc_name)
{
    if (!svc_name) return AXIS2_TRUE;
    if (axutil_strcmp(svc_name, "Version")      == 0) return AXIS2_TRUE;
    if (axutil_strcmp(svc_name, "AdminService") == 0) return AXIS2_TRUE;
    if (strncmp(svc_name, "__", 2)             == 0) return AXIS2_TRUE;
    if (strstr(svc_name, "AdminService")        != NULL) return AXIS2_TRUE;
    return AXIS2_FALSE;
}

/**
 * Return AXIS2_TRUE if the service requires a Bearer auth token.
 * loginService and adminconsole do not; everything else does.
 * Case-insensitive comparison — mirrors Java's requiresAuth logic.
 */
static axis2_bool_t
service_requires_auth(const axis2_char_t *svc_name)
{
    if (!svc_name) return AXIS2_TRUE;

    /* Build a lowercase copy for comparison */
    char lower[256];
    size_t i;
    size_t len = strlen(svc_name);
    if (len >= sizeof(lower))
        len = sizeof(lower) - 1;
    for (i = 0; i < len; i++)
        lower[i] = (char)tolower((unsigned char)svc_name[i]);
    lower[len] = '\0';

    if (strstr(lower, "login")      != NULL) return AXIS2_FALSE;
    if (strcmp(lower, "adminconsole") == 0)  return AXIS2_FALSE;
    return AXIS2_TRUE;
}

/**
 * Build an empty baseline inputSchema json_object:
 *   { "type": "object", "properties": {}, "required": [] }
 *
 * The caller owns the returned object; it must be added to the parent with
 * json_object_object_add() (which takes ownership) or freed with json_object_put().
 */
static json_object *
make_empty_input_schema(void)
{
    json_object *schema = json_object_new_object();
    if (!schema) return NULL;
    json_object_object_add(schema, "type",       json_object_new_string("object"));
    json_object_object_add(schema, "properties", json_object_new_object());
    json_object_object_add(schema, "required",   json_object_new_array());
    return schema;
}

/**
 * Build the inputSchema json_object for an operation.
 *
 * If the mcpInputSchema parameter is present and parses as valid JSON it is
 * returned directly (json_tokener_parse creates a new object tree).
 * On parse failure a WARN is logged and the empty baseline schema is returned.
 * When the parameter is absent the empty baseline is returned.
 */
static json_object *
build_input_schema(
    axis2_op_t         *op,
    axis2_svc_t        *svc,
    const axutil_env_t *env,
    const axis2_char_t *op_name,
    const axis2_char_t *svc_name)
{
    const axis2_char_t *schema_str = get_mcp_string_param(
            op, svc, env, "mcpInputSchema", NULL);

    if (schema_str) {
        json_object *parsed = json_tokener_parse(schema_str);
        if (parsed) {
            return parsed;   /* json_tokener_parse allocates a new tree */
        }
        /* Invalid JSON — log and fall through to empty schema */
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI,
            "[MCP] Invalid mcpInputSchema JSON for operation '%s' in service '%s'"
            " — falling back to empty schema",
            op_name  ? op_name  : "<unknown>",
            svc_name ? svc_name : "<unknown>");
    }

    return make_empty_input_schema();
}

/* ============================================================================
 * Public API
 * ============================================================================
 */

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
mcp_catalog_generate_json(
    axis2_conf_t       *conf,
    const axutil_env_t *env)
{
    if (!env) return NULL;
    if (!conf) {
        char corr_id[AXIS2_JSON_CORR_ID_LEN];
        axis2_json_corr_id_generate(corr_id, sizeof(corr_id));
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[MCP][%s] mcp_catalog_generate_json: conf is NULL — "
            "cannot walk services", corr_id);
        return axis2_json_secure_fault(env, corr_id,
            "catalog generation failed — see server log");
    }

    /* ── Root object ─────────────────────────────────────────────────────── */
    json_object *root = json_object_new_object();
    if (!root) {
        char corr_id[AXIS2_JSON_CORR_ID_LEN];
        axis2_json_corr_id_generate(corr_id, sizeof(corr_id));
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[MCP][%s] json_object_new_object() returned NULL — "
            "out of memory", corr_id);
        return axis2_json_secure_fault(env, corr_id,
            "catalog generation failed — see server log");
    }

    /* ── _meta block ─────────────────────────────────────────────────────── */
    json_object *meta = json_object_new_object();
    json_object_object_add(meta, "axis2JsonRpcFormat",
        json_object_new_string(
            "{\"<operationName>\":[{\"arg0\":{<params>}}]}"));
    json_object_object_add(meta, "contentType",
        json_object_new_string("application/json"));
    json_object_object_add(meta, "authHeader",
        json_object_new_string("Authorization: Bearer <token>"));
    json_object_object_add(meta, "tokenEndpoint",
        json_object_new_string("POST /services/loginService/doLogin"));

    /* Optional global ticker resolve endpoint (axis2.xml parameter) */
    axutil_param_t *ticker_param =
            axis2_conf_get_param(conf, env, "mcpTickerResolveService");
    if (ticker_param) {
        void *ticker_val = axutil_param_get_value(ticker_param, env);
        if (ticker_val && ((axis2_char_t *)ticker_val)[0] != '\0') {
            /* Build "POST /services/<value>" */
            const axis2_char_t *svc_op = (const axis2_char_t *)ticker_val;
            char endpoint_buf[512];
            snprintf(endpoint_buf, sizeof(endpoint_buf),
                     "POST " MCP_SERVICES_PREFIX "%s", svc_op);
            json_object_object_add(meta, "tickerResolveEndpoint",
                json_object_new_string(endpoint_buf));
        }
    }

    json_object_object_add(root, "_meta", meta);

    /* ── tools array ─────────────────────────────────────────────────────── */
    json_object *tools = json_object_new_array();

    axutil_hash_t *svc_map = axis2_conf_get_all_svcs(conf, env);
    if (svc_map) {
        axutil_hash_index_t *svc_hi = axutil_hash_first(svc_map, env);
        while (svc_hi) {
            void *svc_void = NULL;
            axutil_hash_this(svc_hi, NULL, NULL, &svc_void);
            svc_hi = axutil_hash_next(env, svc_hi);

            if (!svc_void) continue;
            axis2_svc_t *svc = (axis2_svc_t *)svc_void;

            /* Service name */
            const axutil_qname_t *svc_qname = axis2_svc_get_qname(svc, env);
            if (!svc_qname) continue;
            const axis2_char_t *svc_name =
                    axutil_qname_get_localpart(svc_qname, env);
            if (!svc_name) continue;

            /* Skip system services */
            if (is_system_service(svc_name)) continue;

            axis2_bool_t requires_auth = service_requires_auth(svc_name);

            /* Walk operations */
            axutil_hash_t *ops_hash = axis2_svc_get_all_ops(svc, env);
            if (!ops_hash) continue;

            axutil_hash_index_t *op_hi = axutil_hash_first(ops_hash, env);
            while (op_hi) {
                void *op_void = NULL;
                axutil_hash_this(op_hi, NULL, NULL, &op_void);
                op_hi = axutil_hash_next(env, op_hi);

                if (!op_void) continue;
                axis2_op_t *op = (axis2_op_t *)op_void;

                const axutil_qname_t *op_qname = axis2_op_get_qname(op, env);
                if (!op_qname) continue;
                const axis2_char_t *op_name =
                        axutil_qname_get_localpart(op_qname, env);
                if (!op_name) continue;

                /* Build endpoint path:  POST /services/SvcName/opName */
                char endpoint_buf[512];
                snprintf(endpoint_buf, sizeof(endpoint_buf),
                         "POST " MCP_SERVICES_PREFIX "%s/%s",
                         svc_name, op_name);

                /* Build payload template:
                 *   {"opName":[{"arg0":{}}]}
                 * json-c handles escaping of op_name and svc_name strings. */
                char template_buf[512];
                snprintf(template_buf, sizeof(template_buf),
                         "{\"%s\":[{\"arg0\":{}}]}", op_name);

                /* Description: operation > service > auto-generated fallback */
                char auto_desc[512];
                snprintf(auto_desc, sizeof(auto_desc), "%s: %s",
                         svc_name, op_name);
                const axis2_char_t *description = get_mcp_string_param(
                        op, svc, env, "mcpDescription", auto_desc);

                /* MCP 2025-03-26 annotation hints */
                axis2_bool_t read_only   = get_mcp_bool_param(op, svc, env,
                                               "mcpReadOnly",    AXIS2_FALSE);
                axis2_bool_t destructive = get_mcp_bool_param(op, svc, env,
                                               "mcpDestructive", AXIS2_FALSE);
                axis2_bool_t idempotent  = get_mcp_bool_param(op, svc, env,
                                               "mcpIdempotent",  AXIS2_FALSE);
                axis2_bool_t open_world  = get_mcp_bool_param(op, svc, env,
                                               "mcpOpenWorld",   AXIS2_FALSE);

                /* inputSchema — static param or empty baseline */
                json_object *input_schema =
                        build_input_schema(op, svc, env, op_name, svc_name);

                /* Build tool node */
                json_object *tool = json_object_new_object();
                json_object_object_add(tool, "name",
                    json_object_new_string(op_name));
                json_object_object_add(tool, "description",
                    json_object_new_string(description));
                json_object_object_add(tool, "inputSchema", input_schema);
                json_object_object_add(tool, "endpoint",
                    json_object_new_string(endpoint_buf));
                json_object_object_add(tool, "x-axis2-payloadTemplate",
                    json_object_new_string(template_buf));
                json_object_object_add(tool, "x-requiresAuth",
                    json_object_new_boolean(requires_auth));

                /* annotations sub-object */
                json_object *annotations = json_object_new_object();
                json_object_object_add(annotations, "readOnlyHint",
                    json_object_new_boolean(read_only));
                json_object_object_add(annotations, "destructiveHint",
                    json_object_new_boolean(destructive));
                json_object_object_add(annotations, "idempotentHint",
                    json_object_new_boolean(idempotent));
                json_object_object_add(annotations, "openWorldHint",
                    json_object_new_boolean(open_world));
                json_object_object_add(tool, "annotations", annotations);

                json_object_array_add(tools, tool);
            } /* end op loop */
        } /* end svc loop */
    }

    json_object_object_add(root, "tools", tools);

    /* ── Serialise ───────────────────────────────────────────────────────── */
    const char *json_cstr =
            json_object_to_json_string_ext(root, JSON_C_TO_STRING_PLAIN);

    axis2_char_t *result = NULL;
    if (json_cstr) {
        result = axutil_strdup(env, json_cstr);
        if (!result) {
            /* axutil_strdup OOM — log with correlation ID and return fault */
            char corr_id[AXIS2_JSON_CORR_ID_LEN];
            axis2_json_corr_id_generate(corr_id, sizeof(corr_id));
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "[MCP][%s] axutil_strdup failed serialising catalog "
                "(%zu bytes) — out of memory",
                corr_id, strlen(json_cstr));
            json_object_put(root);
            return axis2_json_secure_fault(env, corr_id,
                "catalog generation failed — see server log");
        }
    } else {
        char corr_id[AXIS2_JSON_CORR_ID_LEN];
        axis2_json_corr_id_generate(corr_id, sizeof(corr_id));
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[MCP][%s] json_object_to_json_string_ext returned NULL",
            corr_id);
        json_object_put(root);
        return axis2_json_secure_fault(env, corr_id,
            "catalog generation failed — see server log");
    }

    /* json_object_put frees the entire tree including all children */
    json_object_put(root);

    return result;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
mcp_catalog_check_and_respond(
    const axutil_env_t  *env,
    axis2_conf_t        *conf,
    const axis2_char_t  *request_path,
    axis2_char_t       **out_json)
{
    if (!env || !request_path || !out_json) return AXIS2_FALSE;

    /* Suffix match: the path must END with MCP_CATALOG_PATH so that a
     * path like /evil/_mcp/openapi-mcp.json/../../admin (which contains
     * the catalog path as a substring but not as a suffix) does not match.
     * strstr-anywhere matching would allow path-confusion info disclosure. */
    size_t path_len    = strlen(request_path);
    size_t catalog_len = strlen(MCP_CATALOG_PATH);
    if (path_len < catalog_len) return AXIS2_FALSE;
    if (strcmp(request_path + path_len - catalog_len, MCP_CATALOG_PATH) != 0)
        return AXIS2_FALSE;

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "[MCP] Catalog request matched path '%s'", request_path);

    *out_json = mcp_catalog_generate_json(conf, env);

    /* mcp_catalog_generate_json returns NULL on conf==NULL, env==NULL, or OOM.
     * Return AXIS2_FALSE rather than AXIS2_TRUE with a NULL response pointer
     * so the caller falls through to normal dispatch instead of sending NULL. */
    if (!*out_json) {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "[MCP] mcp_catalog_generate_json returned NULL for path '%s' "
            "(conf=%p) — falling through to normal dispatch",
            request_path, (void *)conf);
        return AXIS2_FALSE;
    }

    return AXIS2_TRUE;
}
