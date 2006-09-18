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

#include <w2c_xslt_template_processor.h>
#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
#include <libxml/xinclude.h>
#include <libxml/catalog.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>


AXIS2_EXTERN void AXIS2_CALL
w2c_xslt_template_processor_parse(
    const axis2_env_t *env,
    axis2_char_t *xml_stream,
    axis2_char_t *xslt_filename,
    axis2_char_t *out_filename)
{
    xsltStylesheetPtr ssheet = NULL;
    xmlDocPtr doc = NULL;
    xmlDocPtr res = NULL;
    FILE *out = NULL;

    xmlSubstituteEntitiesDefault(1);
    xmlLoadExtDtdDefaultValue = 1;

    ssheet = xsltParseStylesheetFile((const xmlChar *)xslt_filename);
    doc = (xmlDocPtr)xmlParseMemory(xml_stream,
            (unsigned int)strlen(xml_stream));
    res = xsltApplyStylesheet(ssheet, doc, NULL);

    out = fopen(out_filename, "w+");
    xsltSaveResultToFile(out, res, ssheet);

    xsltFreeStylesheet(ssheet);
    xmlFreeDoc(res);
    xmlFreeDoc(doc);

    xsltCleanupGlobals();
    xmlCleanupParser();
    /* adding new line at the end */
    fprintf(out, "\n");
    fclose(out);
}

