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

#include <guththila.h>


class TestToken: public ::testing::Test
{

    protected:
        void SetUp()
        {
            m_allocator = axutil_allocator_init(NULL);
            ASSERT_NE(m_allocator, nullptr);
            m_env = axutil_env_create(m_allocator);
            ASSERT_NE(m_env, nullptr);
        }

        void TearDown()
        {
            axutil_env_free(m_env);
        }

        axutil_allocator_t *m_allocator;
        axutil_env_t *m_env;
};


TEST_F(TestToken, test_token) {

    int status;
    guththila_tok_list_t *tok_list;

    tok_list = (guththila_tok_list_t*)AXIS2_MALLOC(m_allocator, sizeof(guththila_tok_list_t));
    ASSERT_NE(tok_list, nullptr);

    status = guththila_tok_list_init(tok_list, m_env);
    ASSERT_EQ(status, GUTHTHILA_SUCCESS);

    const char * abc = "abcdefghijklmnopqrstuvwxyz";
    const char * p;

    guththila_token_t * tokens = (guththila_token_t*)AXIS2_MALLOC(m_allocator,
                                                                    strlen(abc) * sizeof(guththila_token_t));
    ASSERT_NE(tokens, nullptr);

    for (p = abc; *p != 0; ++p)
    {
        int index = p - abc;
        guththila_set_token(&tokens[index], (char*)p, index % 7 + 1, 1, 0, 0, 1, m_env);
        status = guththila_tok_list_release_token(tok_list, &tokens[index], m_env);
        /* NOTE: status is index+16 because _init is pushing a bunch of empty
         * tokens onto the stack.  I'm not sure this is correct behavior,
         * but I don't want to risk changing it right now.
         * TODO: invesitgate.  See AXIS2C-1698
         */
        ASSERT_EQ(status, index + 16);
    }

    for (p-=1; p >= abc; --p)
    {
        ASSERT_EQ((guththila_token_t*)guththila_tok_list_get_token(tok_list, m_env), &tokens[p-abc]);
    }

    guththila_token_t tok0;
    guththila_set_token(&tok0, (char*)abc, 1, 1, 0, 0, 1, m_env);

    ASSERT_NE(0, guththila_tok_tok_cmp(&tokens[0], &tokens[1], m_env));
    ASSERT_EQ(0, guththila_tok_tok_cmp(&tokens[0], &tok0, m_env));

    ASSERT_NE(0, guththila_tok_str_cmp(&tokens[0], "b", strlen("b"), m_env));
    ASSERT_EQ(0, guththila_tok_str_cmp(&tokens[0], "a", strlen("a"), m_env));

    /*TODO test guththila_tok_list_grow*/
    /*TODO test guththila_tok_list_free_data*/
    guththila_tok_list_free(tok_list, m_env);
    AXIS2_FREE(m_allocator, tokens);

}
