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
  
#ifndef W2C_SCHEMA_WRITER_META_INFO_H
#define W2C_SCHEMA_WRITER_META_INFO_H

/**
 * @file w2c_schema_writer_meta_info.h
 * @brief represent command line option
 */

#include <axis2_utils.h>
#include <w2c_engine_configuration.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_schema_writer_meta_info represent command line option
 * @ingroup w2c_wsdl_utils
 * @{
 */

typedef struct w2c_schema_writer_meta_info w2c_schema_writer_meta_info_t;
typedef struct w2c_schema_writer_meta_info_ops w2c_schema_writer_meta_info_ops_t;

AXIS2_DECLARE_DATA  struct w2c_schema_writer_meta_info_ops
{
   /**
    * free w2c_schema_writer_meta_info.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @return status of the op. AXIS2_SUCCESS on success
    *     else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    free)(w2c_schema_writer_meta_info_t *schema_writer_meta_info,
         const axutil_env_t *env);


   /**
    * setter for ordered.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  ordered value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_ordered)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            axis2_bool_t ordered);
   /**
    * getter for ordered.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @return value of the ordered
    */ 
    axis2_bool_t (AXIS2_CALL *
    get_ordered)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

    /**
     * setter for anonymous.
     * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
     * @param  env Environment. MUST NOT be NULL
     * @param  anonymous value to set
     * @return status of the op. AXIS2_SUCCESS on success
     *      else AXIS2_FAILURE
     */
    axis2_status_t (AXIS2_CALL *
    set_anonymous)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            axis2_bool_t anonymous);

   /**
    * getter for anonymous.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @return value for the anonymous
    */ 
    axis2_bool_t (AXIS2_CALL *
    get_anonymous)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for choice.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  choice value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_choice)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            axis2_bool_t choice);

   /**
    * getter for choice.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @return value for the choice
    */ 
    axis2_bool_t (AXIS2_CALL *
    get_choice)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);
   
   /**
    * setter for extension.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  extension value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_extension)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            axis2_bool_t extension);

   /**
    * getter for extension.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @return value for the extension
    */
    axis2_bool_t (AXIS2_CALL *
    get_extension)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for restriction.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  restriction value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_restriction)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            axis2_bool_t restriction);

   /**
    * getter for restriction.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @return value for the restriction
    */
    axis2_bool_t (AXIS2_CALL *
    get_restriction)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for extension_classname.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  extension_classname value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_extension_classname)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            axis2_char_t *extension_classname);
   /**
    * getter for extension_classname.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the extension_classname
    */
    axis2_char_t *(AXIS2_CALL *
    get_extension_classname)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for restriction_classname.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  restriction_classname value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_restriction_classname)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            axis2_char_t *restriction_classname);
   /**
    * getter for restriction_classname.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the restriction_classname
    */
    axis2_char_t *(AXIS2_CALL *
    get_restriction_classname)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for own_classname.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  own_classname value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_own_classname)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            axis2_char_t *own_classname);
   /**
    * getter for own_classname.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the own_classname
    */
    axis2_char_t *(AXIS2_CALL *
    get_own_classname)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for enum_facet.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  enum_face value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_enum_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            axis2_char_t *enum_face);
   /**
    * getter for enum_facet.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the enum_facet
    */
    axis2_char_t *(AXIS2_CALL *
    get_enum_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for pattern_facet.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  pattern_face value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_pattern_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            axis2_char_t *pattern_face);
   /**
    * getter for pattern_facet.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the pattern_face
    */
    axis2_char_t *(AXIS2_CALL *
    get_pattern_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for extension_basetype.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  extension_basetype value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_extension_basetype)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            axutil_qname_t *extension_basetype);
   /**
    * getter for extension_basetype.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the extension_basetype
    */
    axutil_qname_t *(AXIS2_CALL *
    get_extension_basetype)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for restriction_basetype.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  restriction_basetype value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_restriction_basetype)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            axutil_qname_t *restriction_basetype);
   /**
    * getter for restriction_basetype.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the restriction_basetype
    */
    axutil_qname_t *(AXIS2_CALL *
    get_restriction_basetype)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for ownqname.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  ownqname value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_ownqname)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            axutil_qname_t *ownqname);
   /**
    * getter for ownqname.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the ownqname
    */
    axutil_qname_t *(AXIS2_CALL *
    get_ownqname)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for element2schema_qnamemap.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  element2schema_qnamemap value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_element2schema_qnamemap)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            axutil_hash_t *element2schema_qnamemap);
   /**
    * getter for element2schema_qnamemap.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the element2schema_qnamemap
    */
    axutil_hash_t *(AXIS2_CALL *
    get_element2schema_qnamemap)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for element2classmap.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  element2classmap value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_element2classmap)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            axutil_hash_t *element2classmap);
   /**
    * getter for element2classmap.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the element2classmap
    */
    axutil_hash_t *(AXIS2_CALL *
    get_element2classmap)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for specialtype_flagmap.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  specialtype_flagmap value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_specialtype_flagmap)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            axutil_hash_t *specialtype_flagmap);
   /**
    * getter for specialtype_flagmap.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the specialtype_flagmap
    */
    axutil_hash_t *(AXIS2_CALL *
    get_specialtype_flagmap)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for qname_maxoccurs_countmap.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  qname_maxoccurs_countmap value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_qname_maxoccurs_countmap)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            axutil_hash_t *qname_maxoccurs_countmap);
   /**
    * getter for qname_maxoccurs_countmap.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the qname_maxoccurs_countmap
    */
    axutil_hash_t *(AXIS2_CALL *
    get_qname_maxoccurs_countmap)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for qname_minoccurs_countmap.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  qname_minoccurs_countmap value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_qname_minoccurs_countmap)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            axutil_hash_t *qname_minoccurs_countmap);
   /**
    * getter for qname_minoccurs_countmap.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the qname_minoccurs_countmap
    */
    axutil_hash_t *(AXIS2_CALL *
    get_qname_minoccurs_countmap)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

    /**
    * setter for length_facet.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  length_facet value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_length_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            long length_facet);
   /**
    * getter for length_facet.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the length_facet
    */
    long (AXIS2_CALL *
    get_length_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for maxexclusive_facet.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  maxexclusive_facet value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_maxexclusive_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            long maxexclusive_facet);
   /**
    * getter for maxexclusive_facet.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the maxexclusive_facet
    */
    long (AXIS2_CALL *
    get_maxexclusive_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for minexclusive_facet.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  minexclusive_facet value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_minexclusive_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            long minexclusive_facet);
   /**
    * getter for minexclusive_facet.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the minexclusive_facet
    */
    long (AXIS2_CALL *
    get_minexclusive_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for maxinclusive_facet.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  maxinclusive_facet value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_maxinclusive_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            long maxinclusive_facet);
   /**
    * getter for maxinclusive_facet.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the maxinclusive_facet
    */
    long (AXIS2_CALL *
    get_maxinclusive_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for mininclusive_facet.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  mininclusive_facet value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_mininclusive_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            long mininclusive_facet);
   /**
    * getter for mininclusive_facet.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the mininclusive_facet
    */
    long (AXIS2_CALL *
    get_mininclusive_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for maxlength_facet.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  maxlength_facet value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_maxlength_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            long maxlength_facet);
   /**
    * getter for maxlength_facet.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the maxlength_facet
    */
    long (AXIS2_CALL *
    get_maxlength_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * setter for minlength_facet.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  minlength_facet value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_minlength_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            long minlength_facet);
   /**
    * getter for minlength_facet.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  value for the minlength_facet
    */
    long (AXIS2_CALL *
    get_minlength_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);
   /**
    * setter for parent.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  parent value to set
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    set_parent)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env,
                            w2c_schema_writer_meta_info_t *parent);
   /**
    * getter for parent.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @return value of the parent
    */
    w2c_schema_writer_meta_info_t *(AXIS2_CALL *
    get_parent)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axutil_env_t *env);

   /**
    * checks if it is a restriction base type.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  restriction_base_type vlaue to check
    * return AXIS2_TRUE if it is a base type, AXIS2_FALSE otherwise
    */
    axis2_bool_t (AXIS2_CALL *
    is_restriction_base_type)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                         const axutil_env_t *env,
                         axutil_qname_t *restriction_base_type);

   /**
    * register nillable qname.
    * @param  schema_writer_meta_info pointer to schema_writer_meta_info struct
    * @param  env Environment. MUST NOT be NULL
    * @param  qname qnamt to register
    * @return status of the op. AXIS2_SUCCESS on success
    *      else AXIS2_FAILURE
    */
    axis2_status_t (AXIS2_CALL *
    register_nillable_qname)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                         const axutil_env_t *env,
                         axutil_qname_t *qname);
    
    axis2_bool_t (AXIS2_CALL *
    is_nillable)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                         const axutil_env_t *env,
                         axutil_qname_t *qname);


    axis2_status_t (AXIS2_CALL *
    register_mapping)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axutil_env_t *env,
                             axutil_qname_t *qname,
                             axutil_qname_t *schema_name,
                             axis2_char_t *class_name,
                             int type);

    axutil_qname_t* (AXIS2_CALL *
    get_schemaqname4qname)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axutil_env_t *env,
                             axutil_qname_t *qname);


    axis2_char_t* (AXIS2_CALL *
    get_classname4qname)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axutil_env_t *env,
                             axutil_qname_t *qname);


    axis2_bool_t (AXIS2_CALL *
    get_attri_status4qname)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axutil_env_t *env,
                             axutil_qname_t *qname);

    axis2_bool_t (AXIS2_CALL *
    get_any_status4qname)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axutil_env_t *env,
                             axutil_qname_t *qname);

    axis2_bool_t (AXIS2_CALL *
    get_array_status4qname)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axutil_env_t *env,
                             axutil_qname_t *qname);

    axis2_bool_t (AXIS2_CALL *
    get_binary_status4qname)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axutil_env_t *env,
                             axutil_qname_t *qname);

    axis2_bool_t (AXIS2_CALL *
    get_any_attri_status4qname)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axutil_env_t *env,
                             axutil_qname_t *qname);
         
    axis2_status_t (AXIS2_CALL *
    add_min_occurs)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axutil_env_t *env,
                             axutil_qname_t *qname,
                             long min_occurs);

    axis2_status_t (AXIS2_CALL *
    add_max_occurs)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axutil_env_t *env,
                             axutil_qname_t *qname,
                             long max_occurs);

    axis2_status_t (AXIS2_CALL *
    register_qname_index)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axutil_env_t *env,
                             axutil_qname_t *qname,
                             int index);

    long (AXIS2_CALL *
    get_min_occurs)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axutil_env_t *env,
                             axutil_qname_t *qname);

    long (AXIS2_CALL *
    get_max_occurs)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axutil_env_t *env,
                             axutil_qname_t *qname);

    axutil_array_list_t* (AXIS2_CALL *
    get_qname_array)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                              const axutil_env_t *env);

    axutil_array_list_t* (AXIS2_CALL *
    get_ordered_qname_array)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                              const axutil_env_t *env);

    int (AXIS2_CALL *
    get_order_start_point)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                              const axutil_env_t *env);


    axis2_status_t (AXIS2_CALL *
    add_status)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axutil_env_t *env,
                             axutil_qname_t *type,
                             int mask);
    
    axis2_status_t (AXIS2_CALL *
    add_enum_facet)( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axutil_env_t *env,
                             axis2_char_t *facet);
};

AXIS2_DECLARE_DATA  struct w2c_schema_writer_meta_info
{
     struct w2c_schema_writer_meta_info_ops *ops;
};


/**
 * create a schema_writer_meta_info obj.
 * @param env Environment. MUST NOT be NULL
 * @param parser user options parser
 * @return newly created schema_writer_meta_info obj
 */
AXIS2_EXTERN w2c_schema_writer_meta_info_t* AXIS2_CALL
w2c_schema_writer_meta_info_create( const axutil_env_t *env);

/*************************** Function macros **********************************/

#define W2C_SCHEMA_WRITER_META_INFO_FREE(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->free(schema_writer_meta_info, env)) 

#define W2C_SCHEMA_WRITER_META_INFO_SET_ORDERED(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_ordered(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_ORDERED(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_ordered(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_ANONYMOUS(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_anonymous(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_ANONYMOUS(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_anonymous(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_CHOICE(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_choice(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_CHOICE(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_choice(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_EXTENSION(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_extension(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_EXTENSION(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_extension(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_RESTRICTION(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_restriction(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_RESTRICTION(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_restriction(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_EXTENSION_CLASSNAME(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_extension_classname(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_EXTENSION_CLASSNAME(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_extension_classname(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_RESTRICTION_CLASSNAME(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_restriction_classname(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_RESTRICTION_CLASSNAME(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_restriction_classname(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_OWN_CLASSNAME(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_own_classname(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_OWN_CLASSNAME(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_own_classname(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_ENUM_FACET(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_enum_facet(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_ENUM_FACET(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_enum_facet(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_PATTERN_FACET(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_pattern_facet(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_PATTERN_FACET(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_pattern_facet(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_EXTENSION_BASETYPE(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_extension_basetype(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_EXTENSION_BASETYPE(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_extension_basetype(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_RESTRICTION_BASETYPE(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_restriction_basetype(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_RESTRICTION_BASETYPE(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_restriction_basetype(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_OWNQNAME(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_ownqname(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_OWNQNAME(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_ownqname(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_ELEMENT2SCHEMA_QNAMEMAP(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_element2schema_qnamemap(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_ELEMENT2SCHEMA_QNAMEMAP(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_element2schema_qnamemap(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_ELEMENT2CLASSMAP(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_element2classmap(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_ELEMENT2CLASSMAP(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_element2classmap(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_SPECIALTYPE_FLAGMAP(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_specialtype_flagmap(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_SPECIALTYPE_FLAGMAP(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_specialtype_flagmap(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_QNAME_MAXOCCURS_COUNTMAP(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_qname_maxoccurs_countmap(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_QNAME_MAXOCCURS_COUNTMAP(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_qname_maxoccurs_countmap(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_QNAME_MINOCCURS_COUNTMAP(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_qname_minoccurs_countmap(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_QNAME_MINOCCURS_COUNTMAP(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_qname_minoccurs_countmap(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_LENGTH_FACET(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_length_facet(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_LENGTH_FACET(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_length_facet(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_MAXEXCLUSIVE_FACET(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_maxexclusive_facet(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_MAXEXCLUSIVE_FACET(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_maxexclusive_facet(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_MINEXCLUSIVE_FACET(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_minexclusive_facet(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_MINEXCLUSIVE_FACET(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_minexclusive_facet(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_MAXINCLUSIVE_FACET(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_maxinclusive_facet(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_MAXINCLUSIVE_FACET(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_maxinclusive_facet(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_MININCLUSIVE_FACET(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_mininclusive_facet(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_MININCLUSIVE_FACET(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_mininclusive_facet(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_MAXLENGTH_FACET(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_maxlength_facet(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_MAXLENGTH_FACET(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_maxlength_facet(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_MINLENGTH_FACET(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_minlength_facet(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_MINLENGTH_FACET(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_minlength_facet(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_NILLABLE_QNAMELIST(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_nillable_qnamelist(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_NILLABLE_QNAMELIST(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_nillable_qnamelist(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_SET_PARENT(schema_writer_meta_info, env, value) \
      ((schema_writer_meta_info)->ops->set_parent(schema_writer_meta_info, env, value))

#define W2C_SCHEMA_WRITER_META_INFO_GET_PARENT(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_parent(schema_writer_meta_info, env))


#define W2C_SCHEMA_WRITER_META_INFO_IS_RESTRICTION_BASE_TYPE(schema_writer_meta_info, env, type) \
      ((schema_writer_meta_info)->ops->is_restriction_base_type(schema_writer_meta_info, env, type))

#define W2C_SCHEMA_WRITER_META_INFO_REGISTER_NILLABLE_QNAME(schema_writer_meta_info, env, qname) \
      ((schema_writer_meta_info)->ops->register_nillable_qname(schema_writer_meta_info, env, qname))

#define W2C_SCHEMA_WRITER_META_INFO_IS_NILLABLE(schema_writer_meta_info, env, qname) \
      ((schema_writer_meta_info)->ops->is_nillable(schema_writer_meta_info, env, qname))

#define W2C_SCHEMA_WRITER_META_INFO_REGISTER_MAPPING(schema_writer_meta_info, \
                                       env, qname, schema_name, class_name, type) \
      ((schema_writer_meta_info)->ops->register_mapping(schema_writer_meta_info, \
                                       env, qname, schema_name, class_name, type)) 

#define W2C_SCHEMA_WRITER_META_INFO_GET_SCHEMAQNAME4QNAME(schema_writer_meta_info, env, qname) \
      ((schema_writer_meta_info)->ops->get_schemaqname4qname(schema_writer_meta_info, env, qname))

#define W2C_SCHEMA_WRITER_META_INFO_GET_CLASSNAME4QNAME(schema_writer_meta_info, env, qname) \
      ((schema_writer_meta_info)->ops->get_classname4qname(schema_writer_meta_info, env, qname))

#define W2C_SCHEMA_WRITER_META_INFO_GET_ATTRI_STATUS4QNAME(schema_writer_meta_info, env, qname) \
      ((schema_writer_meta_info)->ops->get_attri_status4qname(schema_writer_meta_info, env, qname))

#define W2C_SCHEMA_WRITER_META_INFO_GET_ANY_STATUS4QNAME(schema_writer_meta_info, env, qname) \
      ((schema_writer_meta_info)->ops->get_any_status4qname(schema_writer_meta_info, env, qname))

#define W2C_SCHEMA_WRITER_META_INFO_GET_ARRAY_STATUS4QNAME(schema_writer_meta_info, env, qname) \
      ((schema_writer_meta_info)->ops->get_array_status4qname(schema_writer_meta_info, env, qname))

#define W2C_SCHEMA_WRITER_META_INFO_GET_BINARY_STATUS4QNAME(schema_writer_meta_info, env, qname) \
      ((schema_writer_meta_info)->ops->get_binary_status4qname(schema_writer_meta_info, env, qname))

#define W2C_SCHEMA_WRITER_META_INFO_GET_ANY_ATTRI_STATUS4QNAME(schema_writer_meta_info, env, qname) \
      ((schema_writer_meta_info)->ops->get_any_attri_status4qname(schema_writer_meta_info, env, qname))

#define W2C_SCHEMA_WRITER_META_INFO_ADD_MIN_OCCURS(schema_writer_meta_info, env, qname, min_occurs) \
      ((schema_writer_meta_info)->ops->add_min_occurs(schema_writer_meta_info, env, qname, min_occurs))

#define W2C_SCHEMA_WRITER_META_INFO_ADD_MAX_OCCURS(schema_writer_meta_info, env, qname, max_occurs) \
      ((schema_writer_meta_info)->ops->add_max_occurs(schema_writer_meta_info, env, qname, max_occurs))

#define W2C_SCHEMA_WRITER_META_INFO_REGISTER_QNAME_INDEX(schema_writer_meta_info, env, qname, index) \
      ((schema_writer_meta_info)->ops->register_qname_index(schema_writer_meta_info, env, qname, index))

#define W2C_SCHEMA_WRITER_META_INFO_GET_MIN_OCCURS(schema_writer_meta_info, env, qname) \
      ((schema_writer_meta_info)->ops->get_min_occurs(schema_writer_meta_info, env, qname))

#define W2C_SCHEMA_WRITER_META_INFO_GET_MAX_OCCURS(schema_writer_meta_info, env, qname) \
      ((schema_writer_meta_info)->ops->get_max_occurs(schema_writer_meta_info, env, qname))

#define W2C_SCHEMA_WRITER_META_INFO_GET_QNAME_ARRAY(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_qname_array(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_GET_ORDERED_QNAME_ARRAY(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_ordered_qname_array(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_GET_ORDER_START_POINT(schema_writer_meta_info, env) \
      ((schema_writer_meta_info)->ops->get_order_start_point(schema_writer_meta_info, env))

#define W2C_SCHEMA_WRITER_META_INFO_ADD_STATUS(schema_writer_meta_info, env, type, mask) \
      ((schema_writer_meta_info)->ops->add_status(schema_writer_meta_info, env, type, mask))

#define W2C_SCHEMA_WRITER_META_INFO_ADD_ENUM_FACET(schema_writer_meta_info, env, enum_facet) \
      ((schema_writer_meta_info)->ops->add_enum_facet(schema_writer_meta_info, env, enum_facet))

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_SCHEMA_WRITER_META_INFO_H */
