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
#include <axis2_om.h>
#include <xml_schema/axis2_xml_schema_includes.h>
#include <axis2_env.h>
#include "xml_schema_test.h"


static axis2_om_document_t* 
get_root_element_from_filename(axis2_env_t **env, 
                               axis2_char_t *filename);

void test_simple_type_schema_generation(CuTest *tc);
void test_any_attribute(CuTest *tc);
void test_block_content(CuTest *tc);
void test_circular_schema(CuTest *tc);
void test_schema_import1(CuTest *tc);
void test_schema_import2(CuTest *tc);
void test_mixed_content(CuTest *tc);
void test_local_elements(CuTest *tc);
void test_element_refs(CuTest *tc);
void test_forward_refs(CuTest *tc);
void test_local_unnamed_simple_type(CuTest *tc);
void test_simple_restriction(CuTest *tc);
void test_unqualified_schemas(CuTest *tc);
void test_two_schmes(CuTest *tc);


CuSuite* xml_schema_GetSuite()
{
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_simple_type_schema_generation);
    SUITE_ADD_TEST(suite, test_any_attribute);
    SUITE_ADD_TEST(suite, test_block_content);
    SUITE_ADD_TEST(suite, test_circular_schema);
    SUITE_ADD_TEST(suite, test_schema_import1);
    SUITE_ADD_TEST(suite, test_schema_import2);
    SUITE_ADD_TEST(suite, test_mixed_content);
    SUITE_ADD_TEST(suite, test_local_elements);
    SUITE_ADD_TEST(suite, test_element_refs);
    SUITE_ADD_TEST(suite, test_forward_refs);
    SUITE_ADD_TEST(suite, test_local_unnamed_simple_type);
    SUITE_ADD_TEST(suite, test_simple_restriction);
    SUITE_ADD_TEST(suite, test_unqualified_schemas);
    SUITE_ADD_TEST(suite, test_two_schmes);
    return suite;
}

void test_simple_type_schema_generation(CuTest *tc)
{
    axis2_env_t *env = NULL;
    axis2_om_document_t *om_doc = NULL;
    axis2_xml_schema_collection_t *sch_collection = NULL;
    axis2_xml_schema_t *schema = NULL;
    axis2_xml_schema_obj_table_t *obj_table = NULL;
    axis2_array_list_t *list = NULL;
    int i = 0;
    
    void *ele = NULL;
    void *sch_type = NULL;
    axis2_char_t *filename = "./..//test-resources/allSimpleTypes.xsd"; 
    
    
    
    env = axis2_env_create_all("test.log", 1);
    om_doc = get_root_element_from_filename(&env, filename);
    sch_collection = axis2_xml_schema_collection_create(&env);
    schema = AXIS2_XML_SCHEMA_COLLECTION_READ_DOCUMENT(
        sch_collection, &env, om_doc);
    
    CuAssertPtrNotNull(tc, schema); 
    
    obj_table = AXIS2_XML_SCHEMA_GET_ELEMENTS(schema, &env);
    
    CuAssertPtrNotNull(tc, obj_table); 
    
    list = AXIS2_XML_SCHEMA_OBJ_TABLE_GET_VALUES(obj_table, &env);
    
    CuAssertPtrNotNull(tc, list);
    
    for(i = 0; i < AXIS2_ARRAY_LIST_SIZE(list, &env); i++)
    {
        ele = AXIS2_ARRAY_LIST_GET(list, &env, i);
            
        CuAssertPtrNotNull(tc, ele);
        
        sch_type = AXIS2_XML_SCHEMA_ELEMENT_GET_SCHEMA_TYPE(ele, &env);
        
        CuAssertPtrNotNull(tc, sch_type);
        
        ele = NULL;
        sch_type = NULL;
    }
}    

void test_any_attribute(CuTest *tc){}
void test_block_content(CuTest *tc){}
void test_circular_schema(CuTest *tc){}
void test_schema_import1(CuTest *tc){}
void test_schema_import2(CuTest *tc){}
void test_mixed_content(CuTest *tc){}
void test_local_elements(CuTest *tc){}
void test_element_refs(CuTest *tc){}
void test_forward_refs(CuTest *tc){}
void test_local_unnamed_simple_type(CuTest *tc){}
void test_simple_restriction(CuTest *tc){}
void test_unqualified_schemas(CuTest *tc){}
void test_two_schmes(CuTest *tc){}

static axis2_om_document_t* 
get_root_element_from_filename(axis2_env_t **env, 
                               axis2_char_t *filename)
{
    axis2_xml_reader_t *reader = NULL;
    axis2_om_stax_builder_t *om_builder = NULL;
    axis2_om_document_t *doc   = NULL;
    reader = axis2_xml_reader_create_for_file(env, filename, NULL);
    om_builder = axis2_om_stax_builder_create(env, reader);
    doc = axis2_om_document_create(env, NULL, om_builder); 
    AXIS2_OM_DOCUMENT_BUILD_ALL(doc, env);
    return doc;    
}                               