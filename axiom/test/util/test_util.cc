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

#include <axutil_uri.h>
#include <axutil_log_default.h>
#include <axutil_error_default.h>
#include <axiom_util.h>
#include <axiom.h>
#include <axis2_util.h>
#include <axiom_node.h>
#include <axiom_xml_reader.h>


/* FIXME 
 * These tests exercise code, but don't actually check that the output is
 * correct.  They didn't when they were in the old test format, either.
 */


class TestUtil: public ::testing::Test
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


FILE * f = NULL;

int read_input_callback(char *buffer, int size, void* ctx)
{
     return fread(buffer, sizeof(char), size, f);
}
int close_input_callback(void *ctx)
{
     return fclose(f);
}

TEST_F(TestUtil, test_build_and_serialize_om) {

    axiom_node_t *root_node = NULL;

    axiom_element_t *root_ele = NULL;
    axiom_document_t *document = NULL;
    axiom_stax_builder_t *om_builder = NULL;
    axiom_xml_reader_t *xml_reader = NULL;

    axiom_node_t *child = NULL;
    axiom_node_t *node = NULL;
    axiom_types_t node_type;
    axiom_node_t *first_node = NULL;
    axiom_element_t *my_ele = NULL;
    axiom_element_t *first_element = NULL;
    axis2_char_t *uri = "http://www.develop.com/student";
    const axutil_uri_t * uri1 = NULL;
    axiom_node_t *last_child = NULL;
    axiom_element_t *localname_last_child = NULL;
    axiom_element_t *localname_next_sibling = NULL;
    axiom_element_t *uri_localname_first_child = NULL;
    axiom_element_t *uri_localname_last_child = NULL;
    axiom_element_t *uri_localname_next_sibling = NULL;
    axiom_element_t *localname_first_child = NULL;
    axis2_char_t *localname = NULL;
    axiom_element_t *data_element = NULL;
    axiom_element_t *next_sibling = NULL;
    axiom_namespace_t *ns = NULL;
    axiom_element_t *last_element = NULL;
    axutil_qname_t *qname = NULL;
    axis2_char_t *attr_name = NULL;
    axiom_attribute_t *attr = NULL;
    axis2_char_t *attr_value = NULL;
    axiom_element_t *localname_attr_first_child = NULL;
    axiom_element_t *localname_attr_last_child = NULL;
    axiom_element_t *localname_attr_next_sibling = NULL;
    axiom_child_element_iterator_t * child_element = NULL;
    axis2_char_t * localpart = "type";
    axis2_char_t *child_node_text = NULL;
    axis2_char_t *node_namespace_uri = NULL;
    axiom_document_t * new_document = NULL;
    axutil_array_list_t * names;
    axiom_node_t *parent = NULL;
    axis2_char_t * target = NULL;
    axis2_char_t * value = NULL;
    axiom_node_t *temp_node = NULL;

    names = axutil_array_list_create(m_env, 1);
    axutil_array_list_add(names, m_env, "language");

	f = fopen("test.xml","r");
    ASSERT_NE(f, nullptr);

    xml_reader = axiom_xml_reader_create_for_io(m_env, read_input_callback, close_input_callback, NULL, NULL);
    ASSERT_NE(xml_reader, nullptr);

    om_builder = axiom_stax_builder_create(m_env, xml_reader);
    ASSERT_NE(om_builder, nullptr);

    document = axiom_stax_builder_get_document(om_builder, m_env);
    ASSERT_NE(document, nullptr);

    root_node = axiom_document_get_root_element(document, m_env);
    ASSERT_NE(root_node, nullptr);

    if(axiom_node_get_node_type(root_node, m_env) == AXIOM_ELEMENT)
    {
        root_ele = (axiom_element_t*)axiom_node_get_data_element(root_node, m_env);
        if(root_ele)
        {
            printf(" %s  is the root element  \n" ,axiom_element_get_localname(root_ele, m_env));
        }
    }

    axiom_document_build_all(document, m_env);
    first_node = axiom_node_get_first_child(root_node, m_env);
    ASSERT_NE(first_node, nullptr);
    printf ("%s\n", axiom_node_to_string (first_node, m_env)); 

    node = axiom_node_get_next_sibling(first_node, m_env);
    ASSERT_NE(node, nullptr);
    printf (" %s\n", axiom_node_to_string (node, m_env));

    temp_node = axiom_node_get_next_sibling  (node, m_env);
    ASSERT_NE(temp_node, nullptr);
    printf (" %s\n", axiom_node_to_string (temp_node, m_env));

    child = axiom_node_get_first_child(node, m_env);
    node_type = axiom_node_get_node_type(child,m_env);
    data_element =(axiom_element_t*)axiom_node_get_data_element(child,m_env); 
    last_child = axiom_node_get_last_child(temp_node,m_env);

    ns = axiom_element_get_namespace((axiom_element_t*)axiom_node_get_data_element(root_node,m_env), m_env, root_node);
    ASSERT_NE(ns, nullptr);
    printf("\nThe namespace = %s\n", axiom_namespace_to_string(ns,m_env));

    uri = axiom_namespace_get_uri(ns,m_env);
    ASSERT_NE(uri, nullptr);

    localname_next_sibling = axiom_util_get_next_siblng_element_with_localnames(
            localname_next_sibling,m_env,node,names,&child);
    ASSERT_NE(localname_next_sibling, nullptr);

    /* FIXME
    localname_last_child = axiom_util_get_last_child_element_with_localnames(
            localname_last_child,m_env,root_node,names,&last_child);
    ASSERT_NE(localname_last_child, nullptr);
    */

    my_ele = axiom_util_get_first_child_element_with_uri(root_node,m_env,uri,&child);
    ASSERT_NE(my_ele, nullptr);

    localname_first_child = axiom_util_get_first_child_element_with_localnames(
            localname_first_child,m_env,root_node,names,&child);
    ASSERT_NE(localname_first_child, nullptr);

    child = axiom_node_get_last_child(node, m_env);
    ASSERT_NE(child, nullptr);
    axiom_util_new_document(m_env,uri1);
    printf("\nmy_ele = ");
    printf("%s\n ",axiom_node_to_string(child, m_env));

    first_element =  axiom_util_get_first_child_element(my_ele,m_env,child,&child);
    printf("The first element = %s\n",axiom_node_to_string(node, m_env));
    last_element = axiom_util_get_last_child_element(my_ele,m_env,root_node,&child);
    /* FIXME
    localname = axiom_element_get_localname(my_ele,m_env); 
    */
    localname_last_child = axiom_util_get_last_child_element_with_localname(my_ele,m_env,root_node,localname,&child);       
    localname_next_sibling = axiom_util_get_next_siblng_element_with_localname(my_ele,m_env,root_node,localname,&child);
    uri_localname_first_child = axiom_util_get_first_child_element_with_uri_localname(my_ele,m_env,root_node,localname,uri,&child);
    uri_localname_last_child = axiom_util_get_last_child_element_with_uri_localname(my_ele,m_env,root_node,localname,uri,&child);
    uri_localname_next_sibling = axiom_util_get_next_sibling_element_with_uri_localname(my_ele,m_env,root_node,localname,uri,&child);
    qname =  axutil_qname_create(m_env,localpart, NULL, NULL);
    printf("The qname is ");	
    printf("%s",axutil_qname_to_string(qname,m_env));
    printf("\nThe localname is ");
    printf("%s\n", axiom_element_get_localname(my_ele, m_env));
    attr = axiom_element_get_attribute(my_ele,m_env,qname);
    attr_name = axiom_attribute_get_localname(attr,m_env);
    attr_value = axiom_element_get_attribute_value(my_ele,m_env,qname);
    localname_attr_first_child =  axiom_util_get_first_child_element_with_localname_attr(my_ele,m_env,root_node,localname,attr_name,attr_value,&child);
    localname_attr_last_child = axiom_util_get_last_child_element_with_localname_attr(my_ele,m_env,root_node,localname,attr_name,attr_value,&child);
    localname_attr_next_sibling = axiom_util_get_next_siblng_element_with_localname_attr(my_ele,m_env,root_node,localname,attr_name,attr_value,&child);
    axiom_util_get_child_text(node,m_env);
    node_namespace_uri = axiom_util_get_node_namespace_uri(node,m_env);
    child_element =  axiom_util_get_child_elements(my_ele,m_env,node);
    printf("%s\n",axiom_node_to_string(node, m_env));
    printf("%s\n","test is SUCCESS");

}

