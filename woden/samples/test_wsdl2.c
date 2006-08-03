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

#include <stdio.h>
#include <axiom.h>
#include <axiom_xml_reader.h>
#include <axis2_env.h>
#include <axis2_utils.h>
#include <platforms/axis2_platform_auto_sense.h>
#include <woden_resolver.h>
#include <woden_desc.h>
#include <woden_interface.h>
#include <woden_endpoint.h>
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
    
    if(argc > 1)
    {
        filename = argv[1];
        printf("filename:%s\n", filename);
    }
    else
    {
        printf("Give a wsdl2 file name and continue\n");
        return 0;        
    }
    
    env = axis2_env_create_all("test.log", 1);
    
    om_doc = get_root_element_from_filename(env, filename);
    axis2c_home = AXIS2_GETENV("AXIS2C_HOME");
    doc_base_uri = AXIS2_STRACAT (axis2c_home, "/woden", env);
    resolver = woden_resolver_create(env);
    
    desc = WODEN_RESOLVER_READ(resolver, env, om_doc, doc_base_uri);
    intfaces = WODEN_DESC_GET_INTERFACES(desc, env);
    intface = AXIS2_ARRAY_LIST_GET(intfaces, env, 0);
    intface_qname = WODEN_INTERFACE_GET_QNAME(intface, env);
    printf("Interface qname is %s\n", AXIS2_QNAME_TO_STRING(intface_qname, env));
    svc_list = WODEN_DESC_ELEMENT_GET_SVC_ELEMENTS(desc, env);
    if (svc_list)
    {
        void *svc = NULL;
        axis2_array_list_t *endpoints = NULL;
        int i = 0, no_of_endpoints = -1;

        svc = AXIS2_ARRAY_LIST_GET(svc_list, env, 0);
        if (svc)
        {
            axis2_qname_t *svc_qname = WODEN_SVC_GET_QNAME(svc, env);
            if (svc_qname)
            {
                printf("First service qname is %s\n", AXIS2_QNAME_TO_STRING(svc_qname, env));
            }
            endpoints = WODEN_SVC_GET_ENDPOINTS(svc, env);
            if(endpoints)
            {
                no_of_endpoints = AXIS2_ARRAY_LIST_SIZE(endpoints, env);
            }
            else
            {
                printf("Endpoints is NULL\n");
                return 1;
            }
            for(i = 0; i < no_of_endpoints; i++)
            {
                woden_nc_name_t *ncname = NULL;
                axis2_char_t *ep_ncname = NULL;
                woden_endpoint_t *endpoint = NULL;

                endpoint = AXIS2_ARRAY_LIST_GET(endpoints, env, i);
                ncname = WODEN_ENDPOINT_GET_NAME(endpoint, env);
                if(ncname)
                    ep_ncname = WODEN_NC_NAME_TO_STRING(ncname, env);
                if(ep_ncname)
                {
                    printf("ep_ncname:%s\n", ep_ncname); 
                }
            }
        }
    }
    om_builder = AXIOM_DOCUMENT_GET_BUILDER(om_doc, env);
    AXIOM_STAX_BUILDER_FREE(om_builder, env);
    WODEN_DESC_FREE(desc, env);
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
    doc = AXIOM_STAX_BUILDER_GET_DOCUMENT(om_builder, env);
    AXIOM_DOCUMENT_BUILD_ALL(doc, env);

    return doc;    
}

