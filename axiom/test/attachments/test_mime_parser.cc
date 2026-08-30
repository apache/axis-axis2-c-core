/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Attachment caching to attachmentDIR.
 *
 * These cover the file the parser writes when an attachment is too large to
 * hold in memory: where it puts it, what it calls it, and whether one sender
 * can reach another's file. The name used to be the Content-ID the sender
 * supplied, which let a repeated Content-ID land two requests in one file and
 * let a chosen Content-ID open something already sitting in the directory.
 *
 * Reaching the caching path at all takes some care, and the reasons are easy
 * to trip over:
 *
 *  - Caching only starts once the MIME boundary has not been found within the
 *    parser's buffers, which are AXIOM_MIME_PARSER_BUFFER_SIZE (512KB) each.
 *    An attachment below that is served from memory and never touches a file,
 *    so the payload here is deliberately larger.
 *  - parse_for_attachments needs the buffers that parse_for_soap allocates, so
 *    the body has to carry a SOAP part first and both calls have to run, in
 *    that order. That is the real transport's sequence.
 *  - The parser casts its callback context to axis2_callback_info_t and reads
 *    unread_len from it, so the reader below maintains one rather than passing
 *    a context of its own shape.
 */

#include <gtest/gtest.h>
#include <axiom_mime_parser.h>
#include <axiom_data_handler.h>
#include <axutil_env.h>
#include <axutil_hash.h>
#include <axutil_http_chunked_stream.h>
#include <axutil_utils.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <string>
#include <vector>

#define TEST_BOUNDARY "MIMEBOUNDARYurn_test"
#define TEST_CONTENT_ID "attachment-one@example.org"
/* Comfortably past the 512KB buffer so the boundary search spills and the
 * parser switches to caching. */
#define TEST_ATTACHMENT_LEN (1024 * 1024)

namespace
{

/* Serves a fixed buffer to the parser, the way a transport stream would. */
struct memory_source
{
    axis2_callback_info_t cb_info;
    const char *data;
    size_t len;
    size_t pos;
};

static int AXIS2_CALL
memory_read(char *buffer, int size, void *ctx)
{
    memory_source *src = (memory_source *)ctx;
    size_t remaining;
    size_t to_copy;

    if(!src || !buffer || size <= 0)
    {
        return 0;
    }

    /* The parser reserves the last byte for a terminator, matching
     * axis2_http_transport_utils_on_data_request. */
    size--;
    remaining = src->len - src->pos;
    to_copy = ((size_t)size < remaining) ? (size_t)size : remaining;

    if(to_copy > 0)
    {
        memcpy(buffer, src->data + src->pos, to_copy);
        src->pos += to_copy;
    }
    buffer[to_copy] = '\0';

    src->cb_info.unread_len = (int)(src->len - src->pos);
    return (int)to_copy;
}

/* SOAP part, then one attachment part, then the closing boundary. */
static std::string
build_mtom_body(const char *content_id, size_t attachment_len, char fill)
{
    std::string body;

    body += "--" TEST_BOUNDARY "\r\n";
    body += "Content-Type: application/xop+xml\r\n";
    body += "Content-ID: <soap@example.org>\r\n";
    body += "\r\n";
    body += "<soapenv:Envelope xmlns:soapenv=\"http://www.w3.org/2003/05/soap-envelope\">"
            "<soapenv:Body/></soapenv:Envelope>";
    body += "\r\n--" TEST_BOUNDARY "\r\n";
    body += "Content-Type: application/octet-stream\r\n";
    body += "Content-Transfer-Encoding: binary\r\n";
    body += "Content-ID: <";
    body += content_id;
    body += ">\r\n";
    body += "\r\n";
    body.append(attachment_len, fill);
    body += "\r\n--" TEST_BOUNDARY "--\r\n";

    return body;
}

static std::vector<std::string>
list_dir(const std::string &dir)
{
    std::vector<std::string> names;
    DIR *d = opendir(dir.c_str());
    struct dirent *ent;

    if(!d)
    {
        return names;
    }
    while((ent = readdir(d)) != NULL)
    {
        std::string n(ent->d_name);
        if(n != "." && n != "..")
        {
            names.push_back(n);
        }
    }
    closedir(d);
    return names;
}

static void
remove_dir(const std::string &dir)
{
    std::vector<std::string> names = list_dir(dir);
    for(size_t i = 0; i < names.size(); i++)
    {
        unlink((dir + names[i]).c_str());
    }
    rmdir(dir.c_str());
}

class TestMimeParserCaching : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        m_env = axutil_env_create_all("test_mime_parser.log", AXIS2_LOG_LEVEL_CRITICAL);
        ASSERT_NE(m_env, nullptr);

        char tmpl[] = "/tmp/axis2c_mime_XXXXXX";
        const char *made = mkdtemp(tmpl);
        ASSERT_NE(made, nullptr);
        /* The parser concatenates the directory and the name directly, so the
         * trailing separator belongs here. */
        m_dir = std::string(made) + "/";
    }

    void TearDown() override
    {
        if(!m_dir.empty())
        {
            remove_dir(m_dir);
        }
        if(m_env)
        {
            axutil_env_free(m_env);
        }
    }

    /* Runs a whole body through a fresh parser and hands back its parts map.
     * The parser is kept alive because the map belongs to it. */
    axutil_hash_t *parse(const std::string &body, axiom_mime_parser_t **parser_out)
    {
        memory_source src;
        axiom_mime_parser_t *parser = axiom_mime_parser_create(m_env);
        axutil_hash_t *map = NULL;

        if(!parser)
        {
            return NULL;
        }

        memset(&src, 0, sizeof(src));
        src.data = body.c_str();
        src.len = body.size();
        src.pos = 0;
        src.cb_info.env = m_env;
        src.cb_info.in_stream = NULL;
        src.cb_info.chunked_stream = NULL;
        src.cb_info.content_length = (int)body.size();
        src.cb_info.unread_len = (int)body.size();

        /* attachment_dir is borrowed by the parser, so the std::string buffer
         * is fine. mime_boundary is not: the parser frees it, so it gets a
         * heap copy. See the notes on those two setters. */
        axiom_mime_parser_set_attachment_dir(parser, m_env,
            (axis2_char_t *)m_dir.c_str());
        axiom_mime_parser_set_mime_boundary(parser, m_env,
            (axis2_char_t *)axutil_strdup(m_env, TEST_BOUNDARY));

        if(axiom_mime_parser_parse_for_soap(parser, m_env, memory_read, (void *)&src,
            (axis2_char_t *)TEST_BOUNDARY) == AXIS2_FAILURE)
        {
            axiom_mime_parser_free(parser, m_env);
            return NULL;
        }

        map = axiom_mime_parser_parse_for_attachments(parser, m_env, memory_read,
            (void *)&src, (axis2_char_t *)TEST_BOUNDARY, NULL);

        *parser_out = parser;
        return map;
    }

    axutil_env_t *m_env = NULL;
    std::string m_dir;
};

/* The attachment reaches a file, and the data handler points at it. */
TEST_F(TestMimeParserCaching, caches_a_large_attachment_to_a_file)
{
    axiom_mime_parser_t *parser = NULL;
    std::string body = build_mtom_body(TEST_CONTENT_ID, TEST_ATTACHMENT_LEN, 'A');

    axutil_hash_t *map = parse(body, &parser);
    ASSERT_NE(map, nullptr);

    axiom_data_handler_t *dh = (axiom_data_handler_t *)axutil_hash_get(map,
        (void *)TEST_CONTENT_ID, AXIS2_HASH_KEY_STRING);
    ASSERT_NE(dh, nullptr) << "no data handler stored for the Content-ID";
    ASSERT_TRUE(axiom_data_handler_get_cached(dh, m_env)) << "attachment was not cached to file";

    axis2_char_t *path = axiom_data_handler_get_file_name(dh, m_env);
    ASSERT_NE(path, nullptr);

    struct stat st;
    ASSERT_EQ(stat(path, &st), 0) << "the data handler names a file that is not there: " << path;
    ASSERT_GT(st.st_size, 0);

    axiom_mime_parser_free(parser, m_env);
}

/* The regression this file exists for: the name must not be the sender's. */
TEST_F(TestMimeParserCaching, file_name_is_not_derived_from_the_content_id)
{
    axiom_mime_parser_t *parser = NULL;
    std::string body = build_mtom_body(TEST_CONTENT_ID, TEST_ATTACHMENT_LEN, 'A');

    axutil_hash_t *map = parse(body, &parser);
    ASSERT_NE(map, nullptr);

    std::vector<std::string> names = list_dir(m_dir);
    ASSERT_EQ(names.size(), (size_t)1) << "expected exactly one cache file";

    /* The old name was the URL-encoded Content-ID; nothing about the sender's
     * id should be recoverable from the file name. */
    ASSERT_EQ(names[0].find("attachment-one"), std::string::npos)
        << "cache file name still carries the Content-ID: " << names[0];
    ASSERT_EQ(names[0].find("example.org"), std::string::npos)
        << "cache file name still carries the Content-ID: " << names[0];

    axiom_mime_parser_free(parser, m_env);
}

/* Two senders using one Content-ID must not meet in the same file. */
TEST_F(TestMimeParserCaching, same_content_id_twice_uses_two_files)
{
    axiom_mime_parser_t *parser_one = NULL;
    axiom_mime_parser_t *parser_two = NULL;

    std::string first = build_mtom_body(TEST_CONTENT_ID, TEST_ATTACHMENT_LEN, 'A');
    ASSERT_NE(parse(first, &parser_one), nullptr);

    std::string second = build_mtom_body(TEST_CONTENT_ID, TEST_ATTACHMENT_LEN, 'B');
    ASSERT_NE(parse(second, &parser_two), nullptr);

    std::vector<std::string> names = list_dir(m_dir);
    ASSERT_EQ(names.size(), (size_t)2)
        << "the second request reused the first request's file";

    /* And neither file holds a mixture of the two payloads. */
    for(size_t i = 0; i < names.size(); i++)
    {
        std::string path = m_dir + names[i];
        FILE *fp = fopen(path.c_str(), "rb");
        ASSERT_NE(fp, nullptr);
        char head[16];
        size_t got = fread(head, 1, sizeof(head), fp);
        fclose(fp);
        ASSERT_GT(got, (size_t)0);
        char first_byte = head[0];
        for(size_t j = 0; j < got; j++)
        {
            ASSERT_EQ(head[j], first_byte) << "file " << names[i] << " mixes two payloads";
        }
    }

    axiom_mime_parser_free(parser_one, m_env);
    axiom_mime_parser_free(parser_two, m_env);
}

/* A file already sitting in the directory under the sender's chosen name is
 * not what gets opened. */
TEST_F(TestMimeParserCaching, does_not_open_a_pre_existing_file)
{
    /* The old scheme URL-encoded the Content-ID; plant both that and the raw
     * form so either would be caught. */
    const char *planted[] = { "attachment-one%40example.org", TEST_CONTENT_ID };
    for(size_t i = 0; i < sizeof(planted) / sizeof(planted[0]); i++)
    {
        std::string p = m_dir + planted[i];
        FILE *fp = fopen(p.c_str(), "wb");
        ASSERT_NE(fp, nullptr);
        fputs("PLANTED", fp);
        fclose(fp);
    }

    axiom_mime_parser_t *parser = NULL;
    std::string body = build_mtom_body(TEST_CONTENT_ID, TEST_ATTACHMENT_LEN, 'A');
    ASSERT_NE(parse(body, &parser), nullptr);

    for(size_t i = 0; i < sizeof(planted) / sizeof(planted[0]); i++)
    {
        std::string p = m_dir + planted[i];
        struct stat st;
        ASSERT_EQ(stat(p.c_str(), &st), 0) << "planted file disappeared: " << planted[i];
        ASSERT_EQ(st.st_size, (off_t)7) << "the parser wrote into a file it did not create: "
                                        << planted[i];
    }

    axiom_mime_parser_free(parser, m_env);
}

/* Freeing the parser releases its path map but leaves the cached files, which
 * the data handlers still name. */
TEST_F(TestMimeParserCaching, freeing_the_parser_leaves_the_cache_file)
{
    axiom_mime_parser_t *parser = NULL;
    std::string body = build_mtom_body(TEST_CONTENT_ID, TEST_ATTACHMENT_LEN, 'A');

    axutil_hash_t *map = parse(body, &parser);
    ASSERT_NE(map, nullptr);

    axiom_data_handler_t *dh = (axiom_data_handler_t *)axutil_hash_get(map,
        (void *)TEST_CONTENT_ID, AXIS2_HASH_KEY_STRING);
    ASSERT_NE(dh, nullptr);
    std::string path(axiom_data_handler_get_file_name(dh, m_env));

    axiom_mime_parser_free(parser, m_env);

    struct stat st;
    ASSERT_EQ(stat(path.c_str(), &st), 0)
        << "the cache file went away with the parser, but a data handler still names it";
}

}
