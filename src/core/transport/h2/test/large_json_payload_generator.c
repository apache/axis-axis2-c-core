/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "large_json_payload_generator.h"
#include <axutil_string.h>
#include <axutil_date_time.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

/* HTTP/2 Pure JSON Architecture - Big data record templates */
static const axis2_char_t* RECORD_TYPES[] = {
    "TYPE_A", "TYPE_B", "TYPE_C", "TYPE_D", "TYPE_E", "TYPE_F"
};
static const int NUM_RECORD_TYPES = 6;

static const axis2_char_t* CATEGORIES[] = {
    "TECHNOLOGY", "HEALTHCARE", "SERVICES", "ENERGY", "CONSUMER",
    "INDUSTRIAL", "UTILITIES", "MATERIALS", "COMMUNICATIONS", "LOGISTICS"
};
static const int NUM_CATEGORIES = 10;

static const axis2_char_t* CURRENCIES[] = {
    "USD", "EUR", "GBP", "JPY", "CAD", "AUD", "CHF", "CNY"
};
static const int NUM_CURRENCIES = 8;

/* Internal helper functions */
static axis2_char_t* generate_record(const axutil_env_t* env, int index);
static axis2_char_t* generate_identifier(const axutil_env_t* env, int index);
static void append_padding(axis2_char_t* json_str, int current_size, int target_size);

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_h2_generate_big_data_json(
    const axutil_env_t* env,
    int target_size_bytes)
{
    axis2_char_t* json_str = NULL;
    axis2_char_t* temp_str = NULL;
    axis2_char_t* record_str = NULL;
    int estimated_record_size = 800;
    int num_records = 0;
    int i = 0;
    double total_value = 0.0;
    time_t current_time;

    if (!env)
    {
        return NULL;
    }

    /* Seed random number generator for reproducible tests */
    srand(12345);
    time(&current_time);

    /* Calculate number of records needed */
    num_records = (target_size_bytes - 5000) / estimated_record_size;
    if (num_records < 100) num_records = 100;

    /* Allocate initial buffer */
    json_str = (axis2_char_t*)AXIS2_MALLOC(env->allocator, target_size_bytes + 1000);
    if (!json_str)
    {
        return NULL;
    }

    /* Start JSON structure */
    sprintf(json_str,
        "{\n"
        "  \"datasetId\": \"h2_bigdata_dataset_%ld\",\n"
        "  \"timestamp\": \"%ld\",\n"
        "  \"totalValue\": 0.0,\n"
        "  \"currency\": \"USD\",\n"
        "  \"profile\": \"H2_OPTIMIZED\",\n"
        "  \"records\": [\n",
        current_time, current_time);

    /* Generate records */
    for (i = 0; i < num_records; i++)
    {
        if (i > 0)
        {
            strcat(json_str, ",\n");
        }

        record_str = generate_record(env, i);
        if (record_str)
        {
            strcat(json_str, record_str);
            AXIS2_FREE(env->allocator, record_str);
        }

        total_value += 15000 + ((double)rand() / RAND_MAX) * 50000;

        /* Check if we're approaching target size */
        if (strlen(json_str) > target_size_bytes - 2000)
        {
            break;
        }
    }

    /* Add metrics section */
    temp_str = (axis2_char_t*)AXIS2_MALLOC(env->allocator, 2000);
    if (temp_str)
    {
        sprintf(temp_str,
            "\n  ],\n"
            "  \"metrics\": {\n"
            "    \"totalRecords\": %d,\n"
            "    \"totalValue\": %.2f,\n"
            "    \"distributionScore\": %.2f,\n"
            "    \"qualityScore\": %.2f,\n"
            "    \"expectedGrowth\": %.4f,\n"
            "    \"variability\": %.4f,\n"
            "    \"efficiency\": %.4f\n"
            "  },\n",
            i + 1, total_value,
            ((double)rand() / RAND_MAX) * 100,
            ((double)rand() / RAND_MAX) * 10,
            ((double)rand() / RAND_MAX) * 0.15,
            ((double)rand() / RAND_MAX) * 0.30,
            ((double)rand() / RAND_MAX) * 2.0);

        strcat(json_str, temp_str);
        AXIS2_FREE(env->allocator, temp_str);
    }

    /* Add HTTP/2 processing metadata */
    strcat(json_str,
        "  \"http2Processing\": {\n"
        "    \"lastCalculationTime\": \"2025-11-29T16:00:00\",\n"
        "    \"processingEngine\": \"h2-json-engine-v1.0\",\n"
        "    \"streamMultiplexing\": true,\n"
        "    \"flowControlEnabled\": true,\n"
        "    \"compressionRatio\": 0.75\n"
        "  }\n"
        "}");

    /* Pad to target size if needed */
    append_padding(json_str, strlen(json_str), target_size_bytes);

    return json_str;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_h2_generate_streaming_json(
    const axutil_env_t* env,
    int target_size_bytes)
{
    axis2_char_t* json_str = NULL;
    axis2_char_t* chunk_str = NULL;
    int chunk_size = 1024; /* 1KB chunks for HTTP/2 streaming */
    int num_chunks = 0;
    int i = 0, j = 0;
    time_t current_time;

    if (!env)
    {
        return NULL;
    }

    time(&current_time);
    num_chunks = (target_size_bytes - 500) / chunk_size;

    json_str = (axis2_char_t*)AXIS2_MALLOC(env->allocator, target_size_bytes + 1000);
    if (!json_str)
    {
        return NULL;
    }

    sprintf(json_str,
        "{\n"
        "  \"http2StreamingData\": true,\n"
        "  \"timestamp\": \"%ld\",\n"
        "  \"dataChunks\": [\n",
        current_time);

    /* Generate streaming chunks optimized for HTTP/2 flow control */
    for (i = 0; i < num_chunks; i++)
    {
        if (i > 0) strcat(json_str, ",\n");

        chunk_str = (axis2_char_t*)AXIS2_MALLOC(env->allocator, chunk_size + 200);
        if (chunk_str)
        {
            sprintf(chunk_str,
                "    {\n"
                "      \"chunkId\": %d,\n"
                "      \"sequenceNumber\": %d,\n"
                "      \"timestamp\": \"%ld\",\n"
                "      \"http2StreamId\": %d,\n"
                "      \"data\": \"",
                i, i, current_time, (i % 100) + 1);

            /* Fill with streaming data */
            int data_size = chunk_size - 250;
            for (j = 0; j < data_size && strlen(chunk_str) < chunk_size - 50; j++)
            {
                char c = 'A' + (j % 26);
                strncat(chunk_str, &c, 1);
            }

            strcat(chunk_str,
                "\",\n"
                "      \"checksum\": \"");

            /* Simple checksum */
            char checksum[16];
            sprintf(checksum, "%08x", i * 12345);
            strcat(chunk_str, checksum);
            strcat(chunk_str, "\"\n    }");

            strcat(json_str, chunk_str);
            AXIS2_FREE(env->allocator, chunk_str);
        }

        if (strlen(json_str) > target_size_bytes - 500)
        {
            break;
        }
    }

    strcat(json_str,
        "\n  ],\n"
        "  \"totalChunks\": ");

    char num_str[32];
    sprintf(num_str, "%d", i + 1);
    strcat(json_str, num_str);

    strcat(json_str,
        ",\n"
        "  \"totalSize\": ");
    sprintf(num_str, "%d", target_size_bytes);
    strcat(json_str, num_str);

    strcat(json_str,
        ",\n"
        "  \"http2Optimized\": true\n"
        "}");

    return json_str;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_h2_generate_simple_large_json(
    const axutil_env_t* env,
    int target_size_bytes)
{
    axis2_char_t* json_str = NULL;
    int object_size = 120; /* Approximate size per object */
    int num_objects = 0;
    int i = 0;
    time_t current_time;

    if (!env)
    {
        return NULL;
    }

    time(&current_time);
    num_objects = (target_size_bytes - 100) / object_size;

    json_str = (axis2_char_t*)AXIS2_MALLOC(env->allocator, target_size_bytes + 1000);
    if (!json_str)
    {
        return NULL;
    }

    strcpy(json_str, "{\"http2SimpleData\": [");

    for (i = 0; i < num_objects; i++)
    {
        if (i > 0) strcat(json_str, ",");

        char obj_str[200];
        sprintf(obj_str,
            "{\"id\": %d, \"value\": \"h2_data_%d\", \"timestamp\": %ld, \"flag\": %s, \"streamId\": %d}",
            i, i, current_time + i, (i % 2 == 0) ? "true" : "false", (i % 100) + 1);

        strcat(json_str, obj_str);

        if (strlen(json_str) > target_size_bytes - 200)
        {
            break;
        }
    }

    strcat(json_str, "]}");

    /* Pad to exact size if needed */
    append_padding(json_str, strlen(json_str), target_size_bytes);

    return json_str;
}

AXIS2_EXTERN int AXIS2_CALL
axis2_h2_get_json_byte_size(
    const axis2_char_t* json_string)
{
    if (!json_string)
    {
        return 0;
    }
    return strlen(json_string);
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_h2_validate_json_size(
    const axis2_char_t* json_string,
    int target_size,
    double tolerance)
{
    if (!json_string || target_size <= 0 || tolerance < 0.0)
    {
        return AXIS2_FALSE;
    }

    int actual_size = axis2_h2_get_json_byte_size(json_string);
    double difference = abs(actual_size - target_size) / (double)target_size;

    return (difference <= tolerance) ? AXIS2_TRUE : AXIS2_FALSE;
}

/* Internal helper functions */

static axis2_char_t* generate_record(const axutil_env_t* env, int index)
{
    axis2_char_t* record_str = NULL;
    axis2_char_t* identifier = NULL;
    const axis2_char_t* record_type = NULL;
    const axis2_char_t* category = NULL;
    const axis2_char_t* currency = NULL;
    double quantity, price, market_value;
    time_t current_time;

    if (!env)
    {
        return NULL;
    }

    time(&current_time);
    record_str = (axis2_char_t*)AXIS2_MALLOC(env->allocator, 1000);
    if (!record_str)
    {
        return NULL;
    }

    identifier = generate_identifier(env, index);
    record_type = RECORD_TYPES[index % NUM_RECORD_TYPES];
    category = CATEGORIES[index % NUM_CATEGORIES];
    currency = CURRENCIES[index % NUM_CURRENCIES];

    quantity = 100 + ((double)rand() / RAND_MAX) * 10000;
    price = 10 + ((double)rand() / RAND_MAX) * 1000;
    market_value = quantity * price;

    sprintf(record_str,
        "    {\n"
        "      \"recordId\": \"H2_RECORD_%06d\",\n"
        "      \"identifier\": \"%s\",\n"
        "      \"recordType\": \"%s\",\n"
        "      \"category\": \"%s\",\n"
        "      \"currency\": \"%s\",\n"
        "      \"quantity\": %.4f,\n"
        "      \"price\": %.2f,\n"
        "      \"value\": %.2f,\n"
        "      \"weight\": %.4f,\n"
        "      \"lastUpdated\": \"%ld\",\n"
        "      \"http2Processed\": true,\n"
        "      \"attributes\": {\n"
        "        \"factor\": %.4f,\n"
        "        \"variability\": %.4f,\n"
        "        \"dailyVar\": %.2f\n"
        "      },\n"
        "      \"metrics\": {\n"
        "        \"ytdGrowth\": %.4f,\n"
        "        \"monthlyGrowth\": %.4f,\n"
        "        \"efficiency\": %.4f\n"
        "      }\n"
        "    }",
        index,
        identifier ? identifier : "UNKNOWN",
        record_type, category, currency,
        quantity, price, market_value,
        ((double)rand() / RAND_MAX) * 0.1,
        current_time,
        ((double)rand() / RAND_MAX) * 2.0,
        ((double)rand() / RAND_MAX) * 0.5,
        ((double)rand() / RAND_MAX) * 1000,
        (((double)rand() / RAND_MAX) - 0.5) * 0.3,
        (((double)rand() / RAND_MAX) - 0.5) * 0.1,
        ((double)rand() / RAND_MAX) * 3.0);

    if (identifier)
    {
        AXIS2_FREE(env->allocator, identifier);
    }

    return record_str;
}

static axis2_char_t* generate_identifier(const axutil_env_t* env, int index)
{
    static const axis2_char_t* prefixes[] = {"H2TECH", "H2HLTH", "H2SERV", "H2ENRG", "H2CONS", "H2INDU", "H2UTIL", "H2MATL"};
    static const int num_prefixes = 8;

    axis2_char_t* identifier = NULL;

    if (!env)
    {
        return NULL;
    }

    identifier = (axis2_char_t*)AXIS2_MALLOC(env->allocator, 16);
    if (identifier)
    {
        sprintf(identifier, "%s%03d", prefixes[index % num_prefixes], index % 1000);
    }

    return identifier;
}

static void append_padding(axis2_char_t* json_str, int current_size, int target_size)
{
    if (!json_str || current_size >= target_size)
    {
        return;
    }

    int padding_needed = target_size - current_size - 30; /* Reserve space for padding structure */
    if (padding_needed > 0)
    {
        /* Remove closing brace */
        int len = strlen(json_str);
        if (len > 0 && json_str[len - 1] == '}')
        {
            json_str[len - 1] = '\0';
            strcat(json_str, ",\n  \"http2Padding\": \"");

            /* Add padding characters */
            int i;
            for (i = 0; i < padding_needed && i < 1000; i++)
            {
                strcat(json_str, "X");
            }

            strcat(json_str, "\"\n}");
        }
    }
}