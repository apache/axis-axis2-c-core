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
#include <axiom.h>
#include <axiom_xml_reader.h>
#include <axis2_env.h>
#include <axis2_utils.h>
#include <platforms/axis2_platform_auto_sense.h>
#include <woden_resolver.h>
#include <woden_wsdl10_desc.h>
#include <woden_wsdl10_endpoint.h>
#include <woden_interface.h>
#include <woden_binding.h>
#include <woden_element_decl.h>
#include <woden_svc.h>

#include <axiom.h>

static axiom_document_t*
get_root_element_from_filename(
    const axis2_env_t *env,
    axis2_char_t *filename);

int main(int argc, char *argv[])
{
    axis2_env_t *env = NULL;
    axiom_document_t *om_doc = NULL;
    axiom_stax_builder_t *om_builder = NULL;
    axis2_char_t *doc_base_uri = NULL;
    axis2_char_t *axis2c_home = NULL;
    woden_resolver_t *resolver = NULL;
    void *desc = NULL;
    void *intface = NULL;
    axis2_array_list_t *intfaces = NULL;
    axis2_qname_t *intface_qname = NULL;
    axis2_char_t *filename = NULL;
    axis2_array_list_t *svc_list = NULL;
    axis2_array_list_t *binding_list = NULL;
    axis2_array_list_t *ed_list = NULL;
    void *ed = NULL;
    axis2_qname_t *ed_qname = NULL;

    if (argc > 1)
    {
        filename = argv[1];
        printf("filename:%s\n", filename);
    }
    else
    {
        printf("Give a WSDL 1.1 file name and continue\n");
        return 0;
    }

    env = axis2_env_create_all("test.log", 1);

    om_doc = get_root_element_from_filename(env, filename);
    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    doc_base_uri = axis2_stracat(env, axis2c_home, "/woden");
    resolver = woden_resolver_create(env);

    desc = WODEN_RESOLVER_READ(resolver, env, om_doc, doc_base_uri);
    AXIS2_FREE(env->allocator, doc_base_uri);
    WODEN_RESOLVER_FREE(resolver, env);

	 if (desc)
		  intfaces = WODEN_WSDL10_DESC_GET_INTERFACES(desc, env);
	 else
		  return -1;

	 if (intfaces)
		  intface = axis2_array_list_get(intfaces, env, 0);
	 else 
		  return -1;

    if (intface)
    {
        intface_qname = WODEN_INTERFACE_GET_QNAME(intface, env);
    }
    if (intface_qname)
        printf("Interface qname is %s\n", axis2_qname_to_string(intface_qname,
                env));
    svc_list = WODEN_WSDL10_DESC_ELEMENT_GET_SVC_ELEMENTS(desc, env);
    if (svc_list)
    {
        void *svc = NULL;
        axis2_array_list_t *endpoints = NULL;
        int i = 0, no_of_endpoints = -1;
        svc = axis2_array_list_get(svc_list, env, 0);
        if (svc)
        {
            axis2_qname_t *svc_qname = WODEN_SVC_GET_QNAME(svc, env);
            if (svc_qname)
            {
                printf("First service qname is %s\n", axis2_qname_to_string(
                            svc_qname, env));
            }
            endpoints = WODEN_SVC_GET_ENDPOINTS(svc, env);
            if (endpoints)
            {
                no_of_endpoints = axis2_array_list_size(endpoints, env);
            }
            else
            {
                printf("Endpoints is NULL\n");
                return 1;
            }
            for (i = 0; i < no_of_endpoints; i++)
            {
                woden_nc_name_t *ncname = NULL;
                axis2_char_t *ep_ncname = NULL;
                woden_wsdl10_endpoint_t *endpoint = NULL;

                endpoint = axis2_array_list_get(endpoints, env, i);
                ncname = WODEN_WSDL10_ENDPOINT_GET_NAME(endpoint, env);
                if (ncname)
                    ep_ncname = WODEN_NC_NAME_TO_STRING(ncname, env);
                if (ep_ncname)
                {
                    printf("ep_ncname:%s\n", ep_ncname);
                }
            }
        }
    }
    binding_list = WODEN_WSDL10_DESC_ELEMENT_GET_BINDING_ELEMENTS(desc, env);
    if (binding_list)
    {
        void *binding = NULL;
        binding = axis2_array_list_get(binding_list, env, 0);
        if (binding)
        {
            axis2_qname_t *binding_qname = WODEN_BINDING_GET_QNAME(binding, env);
            if (binding_qname)
            {
                printf("First binding qname is %s\n", axis2_qname_to_string(
                            binding_qname, env));
            }
        }
    }

    ed_list = WODEN_WSDL10_DESC_GET_ELEMENT_DECLS(desc, env);
    if (ed_list)
    {

        ed = axis2_array_list_get(ed_list, env, 0);
        if (ed)
            ed_qname = WODEN_ELEMENT_DECL_GET_QNAME(ed, env);
    }
    ed = WODEN_WSDL10_DESC_GET_ELEMENT_DECL(desc, env, ed_qname);
    if (ed)
    {
        axis2_char_t *content_model = NULL;
        axis2_generic_obj_t *obj = NULL;

        axis2_qname_t *ed_qname = WODEN_ELEMENT_DECL_GET_QNAME(ed, env);
        if (ed_qname)
        {
            printf("Element declaration qname is %s\n", axis2_qname_to_string(ed_qname, env));
        }
        content_model = WODEN_ELEMENT_DECL_GET_CONTENT_MODEL(ed, env);
        if (content_model)
        {
            printf("Content model is %s\n", content_model);
        }
        obj = WODEN_ELEMENT_DECL_GET_CONTENT(ed, env);
        if (obj)
        {
            void *value = NULL;

            value =  axis2_generic_obj_get_value(obj, env);
            printf("Content is:\n");
        }
    }
    om_builder = axiom_document_get_builder(om_doc, env);
    axiom_stax_builder_free(om_builder, env);
    WODEN_WSDL10_DESC_FREE(desc, env);
    axis2_env_free(env);
    return 0;
}

static axiom_document_t*
get_root_element_from_filename(
    const axis2_env_t *env,
    axis2_char_t *filename)
{
    axiom_xml_reader_t *reader = NULL;
    axiom_stax_builder_t *om_builder = NULL;
    axiom_document_t *doc   = NULL;

    reader = axiom_xml_reader_create_for_file(env, filename, NULL);
    om_builder = axiom_stax_builder_create(env, reader);
    doc = axiom_stax_builder_get_document(om_builder, env);
    axiom_document_build_all(doc, env);

    return doc;
}

