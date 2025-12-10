/*
 * Simple JSON test to isolate JSON processing issues
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
        printf("FAILED: Could not create environment\n");
        return 1;
    }

    printf("Testing simple JSON processing...\n");

    /* Test 1: Generate simple JSON */
    axis2_char_t* json_payload = axis2_h2_generate_simple_large_json(env, 1024);
    if (!json_payload)
    {
        printf("FAILED: Could not generate JSON payload\n");
        axutil_env_free(env);
        return 1;
    }

    printf("SUCCESS: Generated JSON payload (%d bytes)\n", (int)strlen(json_payload));

    /* Test 2: Parse JSON using pure json-c */
    json_object* json_obj = json_tokener_parse(json_payload);
    if (!json_obj)
    {
        printf("FAILED: Could not parse JSON with json-c\n");
        AXIS2_FREE(env->allocator, json_payload);
        axutil_env_free(env);
        return 1;
    }

    printf("SUCCESS: JSON parsing successful using json-c\n");

    /* Test 3: Validate JSON object */
    if (json_object_get_type(json_obj) != json_type_object)
    {
        printf("FAILED: Root is not a JSON object\n");
        json_object_put(json_obj);
        AXIS2_FREE(env->allocator, json_payload);
        axutil_env_free(env);
        return 1;
    }

    printf("SUCCESS: Validated JSON object\n");

    json_object_put(json_obj);
    AXIS2_FREE(env->allocator, json_payload);
    axutil_env_free(env);

    printf("ALL TESTS PASSED\n");
    return 0;
}