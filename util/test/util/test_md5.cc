
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

#include "test_md5.h"
#include <stdio.h>
#include <axutil_string.h>
#include <axutil_md5.h>
#include <axutil_log_default.h>
#include <axutil_log.h>
#include <axutil_error_default.h>

/* Digests a string and prints the result.
 */
static void MDString (char * string, const axutil_env_t * env)
{
  axutil_md5_ctx_t * context;
  unsigned char digest[16];
  unsigned int i;
  unsigned int len = axutil_strlen(string);

  context = axutil_md5_ctx_create(env);
  axutil_md5_update(context, env, string, len);
  axutil_md5_final(context, env, digest);
  axutil_md5_ctx_free(context, env);

  printf ("MD%d (\"%s\") = ", 5, string);
  for (i = 0; i < 16; i++)
     printf ("%02x", digest[i]);
  printf ("\n");
}

class TestMD5: public ::testing::Test
{

    protected:
        void SetUp()
        {

            m_allocator = axutil_allocator_init(NULL);
            m_axis_log = axutil_log_create(m_allocator, NULL, NULL);
            m_error = axutil_error_create(m_allocator);

            m_env = axutil_env_create_with_error_log(m_allocator, m_error, m_axis_log);

        }

        void TearDown()
        {
            axutil_env_free(m_env);
        }


        axutil_allocator_t *m_allocator = NULL;
        axutil_env_t *m_env = NULL;
        axutil_error_t *m_error = NULL;
        axutil_log_t *m_axis_log = NULL;

};

TEST_F(TestMD5, test_md5)
{

    printf ("\nMD5 test suite:\n");
    printf ("test confirmation: Rivest, R., \"The MD5 Message-Digest Algorithm\", RFC 1321, April 1992.\n");
    MDString ("", m_env);
    MDString ("a", m_env);
    MDString ("abc", m_env);
    MDString ("message digest", m_env);
    MDString ("abcdefghijklmnopqrstuvwxyz", m_env);
    MDString ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", m_env);
    MDString ("12345678901234567890123456789012345678901234567890123456789012345678901234567890", m_env);
}
