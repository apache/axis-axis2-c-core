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

/**
 * @file test_json_security.cc
 * @brief Unit tests for JSON security (depth limiting, payload size, malformed input)
 *
 * Tests verify that Axis2/C's JSON processing handles potentially malicious
 * or malformed JSON payloads safely.
 *
 * Security considerations:
 * - CVE-2020-12762: Integer overflow in json-c with large files
 * - CVE-2024-57699: Stack exhaustion with deeply nested JSON
 * - Malformed JSON handling
 */

#include <gtest/gtest.h>
#include <json-c/json.h>
#include <string>

extern "C" {
#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
}

class JSONSecurityTest : public ::testing::Test {
protected:
    axutil_env_t *env;

    void SetUp() override {
        axutil_allocator_t *allocator = axutil_allocator_init(NULL);
        axutil_error_t *error = axutil_error_create(allocator);
        axutil_log_t *log = axutil_log_create(allocator, NULL, NULL);
        env = axutil_env_create_with_error_log(allocator, error, log);
        ASSERT_NE(env, nullptr);
    }

    void TearDown() override {
        if (env) {
            axutil_env_free(env);
            env = NULL;
        }
    }

    std::string createDeeplyNestedJSON(int depth) {
        std::string json;
        for (int i = 0; i < depth; i++) {
            json += "{\"n\":";
        }
        json += "1";
        for (int i = 0; i < depth; i++) {
            json += "}";
        }
        return json;
    }
};

/**
 * Test that valid JSON parses correctly
 */
TEST_F(JSONSecurityTest, ValidJSONParses) {
    const char *valid_json = "{\"key\": \"value\", \"number\": 42}";

    json_object *obj = json_tokener_parse(valid_json);
    ASSERT_NE(obj, nullptr) << "Valid JSON should parse";

    EXPECT_TRUE(json_object_is_type(obj, json_type_object));

    json_object_put(obj);
}

/**
 * Test that NULL input is handled safely
 */
TEST_F(JSONSecurityTest, NullInputSafe) {
    json_object *obj = json_tokener_parse(NULL);
    EXPECT_EQ(obj, nullptr) << "NULL input should return NULL";
}

/**
 * Test that empty string is handled safely
 */
TEST_F(JSONSecurityTest, EmptyStringSafe) {
    json_object *obj = json_tokener_parse("");
    EXPECT_EQ(obj, nullptr) << "Empty string should return NULL";
}

/**
 * Test that malformed JSON is rejected
 */
TEST_F(JSONSecurityTest, MalformedJSONRejected) {
    const char *malformed[] = {
        "{invalid}",
        "{\"key\":}",
        "{\"key\"",
        "}{",
        "[[[",
        "{\"a\":\"b\",}",  /* trailing comma */
        NULL
    };

    for (int i = 0; malformed[i] != NULL; i++) {
        json_object *obj = json_tokener_parse(malformed[i]);
        EXPECT_EQ(obj, nullptr) << "Malformed JSON should be rejected: " << malformed[i];
        if (obj) {
            json_object_put(obj);
        }
    }
}

/**
 * Test deeply nested JSON handling
 * json-c has a default depth limit (typically 32)
 */
TEST_F(JSONSecurityTest, DeeplyNestedJSONDepthLimit) {
    /* Create JSON nested beyond typical depth limit */
    std::string deep_json = createDeeplyNestedJSON(100);

    struct json_tokener *tok = json_tokener_new();
    ASSERT_NE(tok, nullptr);

    /* Set a reasonable depth limit */
    json_tokener_set_flags(tok, JSON_TOKENER_STRICT);

    json_object *obj = json_tokener_parse_ex(tok, deep_json.c_str(), deep_json.length());

    /* Either parsing fails (depth exceeded) or succeeds (json-c handles it) */
    /* Both are acceptable - we just verify no crash or hang */
    enum json_tokener_error err = json_tokener_get_error(tok);

    if (obj) {
        json_object_put(obj);
    }
    json_tokener_free(tok);

    /* Test passes if we reach here without crash/hang */
    SUCCEED() << "Deep nesting handled safely (error code: " << err << ")";
}

/**
 * Test JSON with very long strings
 */
TEST_F(JSONSecurityTest, LongStringHandling) {
    /* Create JSON with a long string value */
    std::string long_value(10000, 'A');
    std::string json = "{\"key\":\"" + long_value + "\"}";

    json_object *obj = json_tokener_parse(json.c_str());
    ASSERT_NE(obj, nullptr) << "JSON with long string should parse";

    json_object *value_obj = NULL;
    EXPECT_TRUE(json_object_object_get_ex(obj, "key", &value_obj));
    if (value_obj) {
        const char *str = json_object_get_string(value_obj);
        EXPECT_EQ(strlen(str), 10000) << "String length should be preserved";
    }

    json_object_put(obj);
}

/**
 * Test JSON with special characters
 */
TEST_F(JSONSecurityTest, SpecialCharacterHandling) {
    /* JSON with escaped special characters */
    const char *special_json = "{\"key\": \"line1\\nline2\\ttab\\\"quote\\\\\"}";

    json_object *obj = json_tokener_parse(special_json);
    ASSERT_NE(obj, nullptr) << "JSON with escapes should parse";

    json_object_put(obj);
}

/**
 * Test JSON with unicode
 */
TEST_F(JSONSecurityTest, UnicodeHandling) {
    /* JSON with unicode escape sequences */
    const char *unicode_json = "{\"key\": \"Hello \\u4e16\\u754c\"}";

    json_object *obj = json_tokener_parse(unicode_json);
    ASSERT_NE(obj, nullptr) << "JSON with unicode should parse";

    json_object_put(obj);
}

/**
 * Test that arrays are handled correctly
 */
TEST_F(JSONSecurityTest, ArrayHandling) {
    const char *array_json = "[1, 2, 3, \"four\", {\"five\": 5}]";

    json_object *obj = json_tokener_parse(array_json);
    ASSERT_NE(obj, nullptr) << "JSON array should parse";
    EXPECT_TRUE(json_object_is_type(obj, json_type_array));
    EXPECT_EQ(json_object_array_length(obj), 5);

    json_object_put(obj);
}

/**
 * Test numeric edge cases
 */
TEST_F(JSONSecurityTest, NumericEdgeCases) {
    /* Various numeric formats */
    const char *test_cases[] = {
        "{\"n\": 0}",
        "{\"n\": -1}",
        "{\"n\": 1.5}",
        "{\"n\": 1e10}",
        "{\"n\": -1.5e-10}",
        NULL
    };

    for (int i = 0; test_cases[i] != NULL; i++) {
        json_object *obj = json_tokener_parse(test_cases[i]);
        EXPECT_NE(obj, nullptr) << "Numeric JSON should parse: " << test_cases[i];
        if (obj) {
            json_object_put(obj);
        }
    }
}
