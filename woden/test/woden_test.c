/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <CuTest.h>
#include <stdio.h>
#include <axiom.h>
#include <axis2_env.h>
#include "woden_test.h"


static axiom_document_t* 
get_root_element_from_filename(
        const axis2_env_t *env, 
        axis2_char_t *filename);

void test_mixed_content(
        CuTest *tc);

CuSuite* xml_schema_GetSuite()
{
    CuSuite* suite = CuSuiteNew();
    
    SUITE_ADD_TEST(suite, test_mixed_content);

    return suite;
}

void test_mixed_content(
        CuTest *tc)
{
    axis2_env_t *env = NULL;
    axiom_document_t *om_doc = NULL;
    axis2_xml_schema_collection_t *sch_collection = NULL;
    axis2_xml_schema_t *schema = NULL;
    axis2_qname_t *qn = NULL;
    void *ele = NULL;
    void *cmp_type = NULL;
    void *seq = NULL;
    axis2_xml_schema_obj_collection_t *items = NULL;
    void *sub_element = NULL;
    axis2_qname_t *qn1 = NULL;
    axis2_char_t *uri = NULL;
    
    
    axis2_char_t *filename = "./resources/Calculator.wsdl"; 
    
    env = axis2_env_create_all("test.log", 1);
    
    om_doc = get_root_element_from_filename(env, filename);
    
    sch_collection = axis2_xml_schema_collection_create(env);
    
    schema = AXIS2_XML_SCHEMA_COLLECTION_READ_DOCUMENT(
        sch_collection, env, om_doc);
    
    qn = axis2_qname_create(env, "unQualifiedLocals",
            "http://unqualified-elements.example.com", NULL);
    
    ele = AXIS2_XML_SCHEMA_GET_ELEMENT_BY_QNAME(schema, env, qn);

    CuAssertPtrNotNull(tc, ele);
    
    cmp_type = AXIS2_XML_SCHEMA_ELEMENT_GET_SCHEMA_TYPE(ele, env);
    
    CuAssertPtrNotNull(tc, cmp_type); 
    
    seq = AXIS2_XML_SCHEMA_COMPLEX_TYPE_GET_PARTICLE(cmp_type, env);
    
    CuAssertPtrNotNull(tc, seq);
    
    items = AXIS2_XML_SCHEMA_GROUP_BASE_GET_ITEMS(seq, env);
    
    CuAssertPtrNotNull(tc, items);
    
    sub_element = AXIS2_XML_SCHEMA_OBJ_COLLECTION_GET_ITEM(items, env, 0);
    
    CuAssertPtrNotNull(tc, sub_element);
    
    qn1 = AXIS2_XML_SCHEMA_ELEMENT_GET_QNAME(sub_element, env);
    
    CuAssertPtrNotNull(tc, qn1);
    
    uri = AXIS2_QNAME_GET_URI(qn1, env);
    
    CuAssertPtrNotNull(tc, uri);
    
    sub_element = NULL;
    qn1 = NULL;
    uri = NULL;
    
    sub_element = AXIS2_XML_SCHEMA_OBJ_COLLECTION_GET_ITEM(items, env, 0);
    
    CuAssertPtrNotNull(tc, sub_element);
    
    qn1 = AXIS2_XML_SCHEMA_ELEMENT_GET_QNAME(sub_element, env);
    
    CuAssertPtrNotNull(tc, qn1);
    
    uri = AXIS2_QNAME_GET_URI(qn1, env);
    
    CuAssertPtrNotNull(tc, uri);
    
}

static axiom_document_t* 
get_root_element_from_filename(
        const axis2_env_t *env, 
        axis2_char_t *filename)
{
    axis2_xml_reader_t *reader = NULL;
    axiom_stax_builder_t *om_builder = NULL;
    axiom_document_t *doc   = NULL;

    reader = axis2_xml_reader_create_for_file(env, filename, NULL);
    om_builder = axiom_stax_builder_create(env, reader);
    doc = axiom_document_create(env, NULL, om_builder); 
    AXIOM_DOCUMENT_BUILD_ALL(doc, env);

    return doc;    
}

