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

#include <stdio.h>
#include <guththila_buffer.h>
#include <guththila_reader.h>
#include <guththila_writer.h>
#include <guththila.h>
#include <axis2_util.h>
int main()
{
    char *t;
    axis2_allocator_t *allocator;
    axis2_env_t *env ;
    guththila_t *parser ;
    /*guththila_writer_t *writer;*/
    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);
    /*writer = guththila_writer_create_for_memory (env);*/
    parser = guththila_create(env, NULL);
	guththila_create_xml_stream_writer_for_memory(env, parser);
// guththila_write_start_element_with_prefix (env, parser, "my", "one");
	 guththila_write_start_element(env, parser, "two");
    guththila_write_default_namespace(env, parser, "http://another.host.com");
    guththila_write_start_element(env, parser, "two.one");
    guththila_write_end_element(env, parser);
    guththila_write_end_document(env, parser);
	/*guththila_write_to_buffer (env, parser, xml);
	  guththila_flush (env, parser);*/
    t = guththila_writer_get_buffer(env, parser->xsw->writer);
    printf("%s \n", t);
	AXIS2_FREE (env->allocator, t);
    guththila_xml_writer_free(env, parser);
    guththila_free(env, parser);
    axis2_env_free(env);
    return 0;
}
