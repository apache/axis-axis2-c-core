
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

#include <test_md5.h>
#include <stdio.h>
#include <axutil_string.h>
#include <axutil_md5.h>

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

void test_md5(const axutil_env_t *env)
{
    printf ("\nMD5 test suite:\n");
    printf ("test confirmation: Rivest, R., \"The MD5 Message-Digest Algorithm\", RFC 1321, April 1992.\n");
    MDString ("", env);
    MDString ("a", env);
    MDString ("abc", env);
    MDString ("message digest", env);
    MDString ("abcdefghijklmnopqrstuvwxyz", env);
    MDString ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", env);
    MDString ("12345678901234567890123456789012345678901234567890123456789012345678901234567890", env);
}
