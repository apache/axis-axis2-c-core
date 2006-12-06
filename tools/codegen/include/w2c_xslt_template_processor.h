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
  
#ifndef W2C_XSLT_TEMPLATE_PROCESSOR_H
#define W2C_XSLT_TEMPLATE_PROCESSOR_H

/**
 * @file w2c_xslt_template_processor.h
 * @brief handles Axis2/C w2c xslt template processing
 */

#include <stdio.h>
#include <axis2_utils.h>
#include <axiom.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_xslt_template_processor xslt template procssing
 * @ingroup w2c_utils
 * @{
 */    
 

/**
 * Parses an XML stream with an XSL stream.
 * @param env           double pointer to environment struct. MUST NOT be NULL
 * @param xml_stream    source XML stream
 * @param xslt_filename source XSL file name
 * @param out_filename  filename to write the output
 */
AXIS2_EXTERN void AXIS2_CALL
w2c_xslt_template_processor_parse(
    const axis2_env_t *env,
    axis2_char_t *xml_stream,
    axis2_char_t *xslt_filename,
    axis2_char_t *out_filename);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_XSLT_TEMPLATE_PROCESSOR_H */
