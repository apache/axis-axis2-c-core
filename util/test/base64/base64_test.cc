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

#include <gtest/gtest.h>
#include <string.h>
#include <axutil_base64.h>

class TestBase64 : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

/* Test basic encode/decode roundtrip */
TEST_F(TestBase64, test_basic_encode_decode)
{
    const char *plaintext = "Hello, World!";
    int plain_len = strlen(plaintext);

    /* Encode */
    int encoded_len = axutil_base64_encode_len(plain_len);
    char *encoded = new char[encoded_len];
    axutil_base64_encode(encoded, plaintext, plain_len);

    ASSERT_STREQ(encoded, "SGVsbG8sIFdvcmxkIQ==");

    /* Decode */
    int decoded_len = axutil_base64_decode_len(encoded);
    char *decoded = new char[decoded_len + 1];
    int actual_len = axutil_base64_decode(decoded, encoded);

    ASSERT_EQ(actual_len, plain_len);
    ASSERT_STREQ(decoded, plaintext);

    delete[] encoded;
    delete[] decoded;
}

/* Test decode with embedded newlines (AXIS2C-1666) */
TEST_F(TestBase64, test_decode_with_newlines)
{
    /* "Hello, World!" encoded as SGVsbG8sIFdvcmxkIQ== but with newlines */
    const char *encoded_with_newlines = "SGVs\nbG8s\nIFdv\ncmxk\nIQ==";
    const char *expected = "Hello, World!";

    int decoded_len = axutil_base64_decode_len(encoded_with_newlines);
    ASSERT_GT(decoded_len, 0);

    char *decoded = new char[decoded_len + 1];
    int actual_len = axutil_base64_decode(decoded, encoded_with_newlines);

    ASSERT_EQ(actual_len, (int)strlen(expected));
    ASSERT_STREQ(decoded, expected);

    delete[] decoded;
}

/* Test decode with spaces */
TEST_F(TestBase64, test_decode_with_spaces)
{
    /* "Hello, World!" with spaces in encoding */
    const char *encoded_with_spaces = "SGVs bG8s IFdv cmxk IQ==";
    const char *expected = "Hello, World!";

    int decoded_len = axutil_base64_decode_len(encoded_with_spaces);
    ASSERT_GT(decoded_len, 0);

    char *decoded = new char[decoded_len + 1];
    int actual_len = axutil_base64_decode(decoded, encoded_with_spaces);

    ASSERT_EQ(actual_len, (int)strlen(expected));
    ASSERT_STREQ(decoded, expected);

    delete[] decoded;
}

/* Test decode with tabs */
TEST_F(TestBase64, test_decode_with_tabs)
{
    /* "Hello, World!" with tabs in encoding */
    const char *encoded_with_tabs = "SGVs\tbG8s\tIFdv\tcmxk\tIQ==";
    const char *expected = "Hello, World!";

    int decoded_len = axutil_base64_decode_len(encoded_with_tabs);
    ASSERT_GT(decoded_len, 0);

    char *decoded = new char[decoded_len + 1];
    int actual_len = axutil_base64_decode(decoded, encoded_with_tabs);

    ASSERT_EQ(actual_len, (int)strlen(expected));
    ASSERT_STREQ(decoded, expected);

    delete[] decoded;
}

/* Test decode with carriage returns (Windows line endings) */
TEST_F(TestBase64, test_decode_with_crlf)
{
    /* "Hello, World!" with CRLF line endings */
    const char *encoded_with_crlf = "SGVs\r\nbG8s\r\nIFdv\r\ncmxk\r\nIQ==";
    const char *expected = "Hello, World!";

    int decoded_len = axutil_base64_decode_len(encoded_with_crlf);
    ASSERT_GT(decoded_len, 0);

    char *decoded = new char[decoded_len + 1];
    int actual_len = axutil_base64_decode(decoded, encoded_with_crlf);

    ASSERT_EQ(actual_len, (int)strlen(expected));
    ASSERT_STREQ(decoded, expected);

    delete[] decoded;
}

/* Test decode with mixed whitespace */
TEST_F(TestBase64, test_decode_with_mixed_whitespace)
{
    /* "Hello, World!" with various whitespace characters */
    const char *encoded_mixed = "  SGVs\n\tbG8s \r\n IFdv\t\ncmxk  IQ==  ";
    const char *expected = "Hello, World!";

    int decoded_len = axutil_base64_decode_len(encoded_mixed);
    ASSERT_GT(decoded_len, 0);

    char *decoded = new char[decoded_len + 1];
    int actual_len = axutil_base64_decode(decoded, encoded_mixed);

    ASSERT_EQ(actual_len, (int)strlen(expected));
    ASSERT_STREQ(decoded, expected);

    delete[] decoded;
}

/* Test decode of PEM-style certificate (64-char lines with newlines) */
TEST_F(TestBase64, test_decode_pem_style)
{
    /* Simulated PEM-style content: base64 with 64-char line wrapping */
    const char *pem_content =
        "VGhpcyBpcyBhIHRlc3Qgb2YgUEVNLXN0eWxlIGJhc2U2NCBlbmNvZGluZyB3\n"
        "aXRoIGxpbmUgYnJlYWtzIGV2ZXJ5IDY0IGNoYXJhY3RlcnMu";
    const char *expected = "This is a test of PEM-style base64 encoding with line breaks every 64 characters.";

    int decoded_len = axutil_base64_decode_len(pem_content);
    ASSERT_GT(decoded_len, 0);

    char *decoded = new char[decoded_len + 1];
    int actual_len = axutil_base64_decode(decoded, pem_content);

    ASSERT_EQ(actual_len, (int)strlen(expected));
    ASSERT_STREQ(decoded, expected);

    delete[] decoded;
}

/* Test decode_len returns correct length with whitespace */
TEST_F(TestBase64, test_decode_len_with_whitespace)
{
    /* Both should return the same decoded length */
    const char *clean = "SGVsbG8sIFdvcmxkIQ==";
    const char *with_ws = "SGVs\nbG8s\nIFdv\ncmxk\nIQ==";

    int len_clean = axutil_base64_decode_len(clean);
    int len_with_ws = axutil_base64_decode_len(with_ws);

    ASSERT_EQ(len_clean, len_with_ws);
    ASSERT_EQ(len_clean, 13); /* "Hello, World!" is 13 bytes */
}

/* Test null input handling */
TEST_F(TestBase64, test_null_input)
{
    ASSERT_EQ(axutil_base64_decode_len(NULL), -1);

    char buf[64];
    ASSERT_EQ(axutil_base64_decode(buf, NULL), -1);
    ASSERT_EQ(axutil_base64_decode_binary((unsigned char *)buf, NULL), -1);
}

/* Test empty string */
TEST_F(TestBase64, test_empty_string)
{
    const char *empty = "";

    int decoded_len = axutil_base64_decode_len(empty);
    ASSERT_EQ(decoded_len, 0);
}

/* Test binary data roundtrip */
TEST_F(TestBase64, test_binary_roundtrip)
{
    /* Binary data with null bytes and high values */
    unsigned char binary_data[] = {0x00, 0x01, 0x02, 0xFF, 0xFE, 0xFD, 0x80, 0x7F};
    int binary_len = sizeof(binary_data);

    /* Encode */
    int encoded_len = axutil_base64_encode_len(binary_len);
    char *encoded = new char[encoded_len];
    axutil_base64_encode_binary(encoded, binary_data, binary_len);

    /* Decode */
    int decoded_len = axutil_base64_decode_len(encoded);
    unsigned char *decoded = new unsigned char[decoded_len];
    int actual_len = axutil_base64_decode_binary(decoded, encoded);

    ASSERT_EQ(actual_len, binary_len);
    ASSERT_EQ(memcmp(decoded, binary_data, binary_len), 0);

    delete[] encoded;
    delete[] decoded;
}
