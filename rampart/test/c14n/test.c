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
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axiom.h>
#include <axiom_xml_reader.h>
#include <axiom_stax_builder.h>
#include "oxs_c14n.h"

int test(int argc, char **argv);

void p_fail()
{
    printf ("File cannot be processed. \n");
}

int main(int argc, char **argv)
{
    return test(argc, argv);
/*    return test2();*/
}

/*int test2()
{
    axis2_allocator_t *allocator = NULL;
    axis2_env_t *env = NULL;
    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);

    axiom_namespace_t *ns1 = axiom_namespace_create(env, "urn:ns1", "ns1");
    axiom_namespace_t *ns2 = axiom_namespace_create(env, "urn:ns2", "ns2");
    axiom_namespace_t *ns3 = axiom_namespace_create(env, "urn:ns3", "ns3");
    return 0;
}*/

int test(int argc, char **argv)
{
    axis2_allocator_t *allocator = NULL;
    axis2_env_t *env = NULL;
    
    axis2_char_t *file = NULL;

    axiom_xml_reader_t *xml_reader = NULL;
    axiom_stax_builder_t *om_builder = NULL;
    axiom_document_t *doc = NULL;
    axis2_stream_t *stream = NULL;
    axis2_char_t *c14n_doc = NULL;
    int len;
    axis2_status_t res = AXIS2_SUCCESS;

    if (argc < 2)
    {
        printf("Usage: test_c14n filename [exclusive]\n");
        return 1;
    }

    allocator = axis2_allocator_init(NULL);
    env = axis2_env_create(allocator);
    
    file = argv[1];

    xml_reader = axiom_xml_reader_create_for_file(env, file, "UTF-8");  /*"ISO-8859-1")*/
    if (!xml_reader) { p_fail(); return 1; }

    om_builder = axiom_stax_builder_create(env, xml_reader);
    if (!om_builder) { p_fail(); return 1; }

    doc = axiom_stax_builder_get_document(om_builder, env);
    AXIOM_DOCUMENT_BUILD_ALL (doc, env); 
    if (!doc) { p_fail(); return 1; }
    axis2_char_t *txt = NULL;
    /*res = oxs_c14n_apply(env, doc, AXIS2_TRUE, &txt, AXIS2_FALSE, NULL);
    printf("%s", txt);*/
    
    /*printf("\n--------------stream:\n");*/ /*removed for xml_pp*/
    
    stream = axis2_stream_create_basic(env);
    
    axiom_node_t *root_node = AXIOM_DOCUMENT_GET_ROOT_ELEMENT(doc, env);
    axiom_node_t *c14n_node = AXIOM_NODE_GET_FIRST_ELEMENT(root_node, env);

    if (argc>2 && !(argv[2][0]-'e'))
        res = oxs_c14n_apply_stream(env, doc, AXIS2_TRUE, stream, AXIS2_TRUE , NULL, c14n_node);
    else
        res = oxs_c14n_apply_stream(env, doc, AXIS2_TRUE, stream, AXIS2_FALSE , NULL, c14n_node);

    if (!res) return -1; /*error occured!*/

    len = AXIS2_STREAM_BASIC_GET_LEN(stream, env) + 1;
    c14n_doc = (axis2_char_t*)AXIS2_MALLOC(env->allocator, len);
    
    
    AXIS2_STREAM_READ(stream, env, c14n_doc, len);
    
    printf("%s",c14n_doc);

    if (txt)
    {
        AXIS2_FREE(env->allocator, txt);
        txt = NULL;
    }
    if (stream)
    {
        AXIS2_STREAM_FREE(stream, env);
        stream = NULL;
    }
    if (c14n_doc)
    {
        AXIS2_FREE(env->allocator, c14n_doc);
        c14n_doc = NULL;
    }

    if (om_builder)
    {
        axiom_stax_builder_free(om_builder, env);
        om_builder = NULL;
    }

    if (env)
    {
        axis2_env_free(env);
        env = NULL;
    }

    /*printf("\n");*/
    return 0;
}
