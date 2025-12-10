/*
 * Quick debug program to test JSON generation and parsing
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <axutil_utils_defines.h>
#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <json-c/json.h>

#include "large_json_payload_generator.h"

static axutil_env_t* create_test_environment(void)
{
    axutil_allocator_t* allocator = axutil_allocator_init(NULL);
    if (!allocator) return NULL;

    axutil_error_t* error = axutil_error_create(allocator);
    if (!error) return NULL;

    axutil_log_t* log = axutil_log_create_default(allocator);
    if (!log) return NULL;

    return axutil_env_create_with_error_log(allocator, error, log);
}

int main(void)
{
    axutil_env_t* env = create_test_environment();
    if (!env)
    {
        printf("Failed to create environment\n");
        return 1;
    }

    printf("Testing small JSON generation and parsing...\n");

    /* Test with a very small JSON payload first */
    axis2_char_t* json_str = axis2_h2_generate_simple_large_json(env, 1024); /* 1KB */
    if (!json_str)
    {
        printf("Failed to generate JSON\n");
        return 1;
    }

    printf("Generated JSON (%d bytes):\n", (int)strlen(json_str));
    printf("First 500 chars: %.500s\n", json_str);

    /* Test JSON parsing using pure json-c */
    json_object* json_obj = json_tokener_parse(json_str);
    if (!json_obj)
    {
        printf("Failed to parse JSON with json-c\n");
        AXIS2_FREE(env->allocator, json_str);
        return 1;
    }

    printf("✅ JSON parsing successful!\n");

    json_object_put(json_obj);
    AXIS2_FREE(env->allocator, json_str);
    axutil_env_free(env);

    return 0;
}