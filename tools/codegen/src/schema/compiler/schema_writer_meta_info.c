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

#include <w2c_schema_writer_meta_info.h>
#include <w2c_schema_consts.h>
#include <w2c_string.h>

/**
 * @brief
 */
typedef struct w2c_schema_writer_meta_info_impl
{
    w2c_schema_writer_meta_info_t writer_meta_info;

    axis2_bool_t ordered; 
    axis2_bool_t anonymous; 
    axis2_bool_t choice; 
    axis2_bool_t extension; 
    axis2_bool_t restriction; 

    axis2_char_t *extension_classname; 
    axis2_char_t *restriction_classname; 
    axis2_char_t *own_classname;
    axis2_char_t *enum_facet;
    axis2_char_t *pattern_facet;
    
    axis2_qname_t *extension_basetype;
    axis2_qname_t *restriction_basetype;
    axis2_qname_t *ownqname;
    axis2_hash_t *element2schema_qnamemap;
    axis2_hash_t *element2classmap;
    axis2_hash_t *specialtype_flagmap;
    axis2_hash_t *qname_maxoccurs_countmap;
    axis2_hash_t *qname_minoccurs_countmap;
    axis2_hash_t *qname_orderlist;

    long length_facet; 
    long maxexclusive_facet; 
    long minexclusive_facet; 
    long maxinclusive_facet; 
    long mininclusive_facet; 
    long maxlength_facet; 
    long minlength_facet; 

    axis2_hash_t *nillable_qnamelist;

    w2c_schema_writer_meta_info_t *parent;

} w2c_schema_writer_meta_info_impl_t;

#define W2C_INTF_TO_IMPL(schema_writer_meta_info) \
        ((w2c_schema_writer_meta_info_impl_t *) schema_writer_meta_info)

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_free (w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                                    const axis2_env_t *env);


axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_ordered( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_bool_t ordered);
axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_ordered( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_anonymous( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_bool_t anonymous);
axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_anonymous( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_choice( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_bool_t choice);
axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_choice( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_extension( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_bool_t extension);
axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_extension( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_restriction( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_bool_t restriction);
axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_restriction( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_extension_classname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_char_t *extension_classname);
axis2_char_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_extension_classname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_restriction_classname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_char_t *restriction_classname);
axis2_char_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_restriction_classname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_own_classname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_char_t *own_classname);
axis2_char_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_own_classname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_enum_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_char_t *enum_facet);
axis2_char_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_enum_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_pattern_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_char_t *pattern_face);
axis2_char_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_pattern_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_extension_basetype( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_qname_t *extension_basetype);
axis2_qname_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_extension_basetype( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_restriction_basetype( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_qname_t *restriction_basetype);
axis2_qname_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_restriction_basetype( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_ownqname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_qname_t *ownqname);
axis2_qname_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_ownqname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_element2schema_qnamemap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_hash_t *element2schema_qnamemap);
axis2_hash_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_element2schema_qnamemap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_element2classmap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_hash_t *element2classmap);
axis2_hash_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_element2classmap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_specialtype_flagmap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_hash_t *specialtype_flagmap);
axis2_hash_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_specialtype_flagmap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_qname_maxoccurs_countmap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_hash_t *qname_maxoccurs_countmap);
axis2_hash_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_qname_maxoccurs_countmap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_qname_minoccurs_countmap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_hash_t *qname_minoccurs_countmap);
axis2_hash_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_qname_minoccurs_countmap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_length_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            long length_facet);
long AXIS2_CALL
w2c_schema_writer_meta_info_get_length_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_maxexclusive_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            long maxexclusive_facet);
long AXIS2_CALL
w2c_schema_writer_meta_info_get_maxexclusive_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_minexclusive_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            long minexclusive_facet);
long AXIS2_CALL
w2c_schema_writer_meta_info_get_minexclusive_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_maxinclusive_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            long maxinclusive_facet);
long AXIS2_CALL
w2c_schema_writer_meta_info_get_maxinclusive_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_mininclusive_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            long mininclusive_facet);
long AXIS2_CALL
w2c_schema_writer_meta_info_get_mininclusive_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_maxlength_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            long maxlength_facet);
long AXIS2_CALL
w2c_schema_writer_meta_info_get_maxlength_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_minlength_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            long minlength_facet);
long AXIS2_CALL
w2c_schema_writer_meta_info_get_minlength_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_parent( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            w2c_schema_writer_meta_info_t *parent);
w2c_schema_writer_meta_info_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_parent( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env);

/** methods other than getters/setters */
axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_is_restriction_base_type( 
                             w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *restriction_base_type);


axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_register_nillable_qname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname);

axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_is_nillable( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_register_mapping( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname,
                             axis2_qname_t *schema_name,
                             axis2_char_t *class_name,
                             int type);

axis2_qname_t* AXIS2_CALL
w2c_schema_writer_meta_info_get_schemaqname4qname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname);


axis2_char_t* AXIS2_CALL
w2c_schema_writer_meta_info_get_classname4qname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname);


axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_attri_status4qname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname);

axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_any_status4qname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname);

axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_array_status4qname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname);

axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_binary_status4qname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname);

axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_any_attri_status4qname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname);
         
axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_add_min_occurs( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname,
                             long min_occurs);
          
axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_add_max_occurs( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname,
                             long max_occurs);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_register_qname_index( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname,
                             int index);

long AXIS2_CALL
w2c_schema_writer_meta_info_get_min_occurs( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname);

long AXIS2_CALL
w2c_schema_writer_meta_info_get_max_occurs( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname);

axutil_array_list_t* AXIS2_CALL
w2c_schema_writer_meta_info_get_qname_array( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                                     const axis2_env_t *env);

axutil_array_list_t* AXIS2_CALL
w2c_schema_writer_meta_info_get_ordered_qname_array( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                                     const axis2_env_t *env);

int AXIS2_CALL
w2c_schema_writer_meta_info_get_order_start_point( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                                     const axis2_env_t *env);


axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_add_status( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *type,
                             int mask);

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_add_enum_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_char_t *enum_facet);
/** private methods */
axis2_bool_t 
w2c_schema_writer_meta_info_get_status( w2c_schema_writer_meta_info_impl_t *schema_writer_meta_info_impl,
                             const axis2_env_t *env,
                             int stored_status,
                             int mask);

AXIS2_EXTERN w2c_schema_writer_meta_info_t * AXIS2_CALL
w2c_schema_writer_meta_info_create (const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    writer_meta_info_impl = (w2c_schema_writer_meta_info_impl_t *) AXIS2_MALLOC(env->
               allocator, sizeof(w2c_schema_writer_meta_info_impl_t));

    if(! writer_meta_info_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    writer_meta_info_impl->ordered = AXIS2_FALSE; 
    writer_meta_info_impl->anonymous = AXIS2_FALSE; 
    writer_meta_info_impl->choice = AXIS2_FALSE; 
    writer_meta_info_impl->extension = AXIS2_FALSE; 
    writer_meta_info_impl->restriction = AXIS2_FALSE; 

    writer_meta_info_impl-> extension_classname = NULL;
    writer_meta_info_impl-> restriction_classname = NULL;
    writer_meta_info_impl-> own_classname = NULL;
    writer_meta_info_impl-> enum_facet = NULL;
    writer_meta_info_impl-> pattern_facet = NULL;
    writer_meta_info_impl-> nillable_qnamelist = axis2_hash_make( env);
    writer_meta_info_impl-> parent = NULL;
    
    writer_meta_info_impl-> extension_basetype = NULL;
    writer_meta_info_impl-> restriction_basetype = NULL;
    writer_meta_info_impl-> ownqname = NULL;
    writer_meta_info_impl-> element2schema_qnamemap = axis2_hash_make(env);
    writer_meta_info_impl-> element2classmap= axis2_hash_make(env);
    writer_meta_info_impl-> specialtype_flagmap = axis2_hash_make(env);
    writer_meta_info_impl-> qname_maxoccurs_countmap = axis2_hash_make(env);
    writer_meta_info_impl-> qname_minoccurs_countmap = axis2_hash_make(env);
    writer_meta_info_impl-> qname_orderlist = axis2_hash_make(env);

    writer_meta_info_impl->length_facet = -1; 
    writer_meta_info_impl->maxexclusive_facet = -1; 
    writer_meta_info_impl->minexclusive_facet = -1; 
    writer_meta_info_impl->maxinclusive_facet = -1; 
    writer_meta_info_impl->mininclusive_facet = -1; 
    writer_meta_info_impl->maxlength_facet = -1; 
    writer_meta_info_impl->minlength_facet = -1; 



    writer_meta_info_impl->writer_meta_info.ops =
    AXIS2_MALLOC (env->allocator, sizeof(w2c_schema_writer_meta_info_ops_t));
    if(! writer_meta_info_impl->writer_meta_info.ops)
    {
        w2c_schema_writer_meta_info_free(&(writer_meta_info_impl->writer_meta_info), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    writer_meta_info_impl->writer_meta_info.ops->free = w2c_schema_writer_meta_info_free;

    writer_meta_info_impl->writer_meta_info.ops->get_ordered =
                                 w2c_schema_writer_meta_info_get_ordered;
    writer_meta_info_impl->writer_meta_info.ops->set_ordered =
                                 w2c_schema_writer_meta_info_set_ordered;
    writer_meta_info_impl->writer_meta_info.ops->get_anonymous =
                                 w2c_schema_writer_meta_info_get_anonymous;
    writer_meta_info_impl->writer_meta_info.ops->set_anonymous =
                                 w2c_schema_writer_meta_info_set_anonymous;
    writer_meta_info_impl->writer_meta_info.ops->get_choice =
                                 w2c_schema_writer_meta_info_get_choice;
    writer_meta_info_impl->writer_meta_info.ops->set_choice =
                                 w2c_schema_writer_meta_info_set_choice;
    writer_meta_info_impl->writer_meta_info.ops->get_extension =
                                 w2c_schema_writer_meta_info_get_extension;
    writer_meta_info_impl->writer_meta_info.ops->set_extension =
                                 w2c_schema_writer_meta_info_set_extension;
    writer_meta_info_impl->writer_meta_info.ops->get_restriction =
                                w2c_schema_writer_meta_info_get_restriction; 
    writer_meta_info_impl->writer_meta_info.ops->set_restriction =
                                w2c_schema_writer_meta_info_set_restriction; 
     
    writer_meta_info_impl->writer_meta_info.ops->get_extension_classname =
                                 w2c_schema_writer_meta_info_get_extension_classname;
    writer_meta_info_impl->writer_meta_info.ops->set_extension_classname =
                                 w2c_schema_writer_meta_info_set_extension_classname;
    writer_meta_info_impl->writer_meta_info.ops->get_restriction_classname =
                                 w2c_schema_writer_meta_info_get_restriction_classname;
    writer_meta_info_impl->writer_meta_info.ops->set_restriction_classname =
                                 w2c_schema_writer_meta_info_set_restriction_classname;
    writer_meta_info_impl->writer_meta_info.ops->get_own_classname =
                                 w2c_schema_writer_meta_info_get_own_classname;
    writer_meta_info_impl->writer_meta_info.ops->set_own_classname =
                                 w2c_schema_writer_meta_info_set_own_classname;
    writer_meta_info_impl->writer_meta_info.ops->get_enum_facet =
                                 w2c_schema_writer_meta_info_get_enum_facet;
    writer_meta_info_impl->writer_meta_info.ops->set_enum_facet =
                                 w2c_schema_writer_meta_info_set_enum_facet;
    writer_meta_info_impl->writer_meta_info.ops->get_pattern_facet =
                                 w2c_schema_writer_meta_info_get_pattern_facet;
    writer_meta_info_impl->writer_meta_info.ops->set_pattern_facet =
                                 w2c_schema_writer_meta_info_set_pattern_facet;
    writer_meta_info_impl->writer_meta_info.ops->get_extension_basetype =
                                 w2c_schema_writer_meta_info_get_extension_basetype;
    writer_meta_info_impl->writer_meta_info.ops->set_extension_basetype =
                                 w2c_schema_writer_meta_info_set_extension_basetype;
    writer_meta_info_impl->writer_meta_info.ops->get_restriction_basetype =
                                 w2c_schema_writer_meta_info_get_restriction_basetype;
    writer_meta_info_impl->writer_meta_info.ops->set_restriction_basetype =
                                 w2c_schema_writer_meta_info_set_restriction_basetype;
    writer_meta_info_impl->writer_meta_info.ops->get_ownqname =
                                 w2c_schema_writer_meta_info_get_ownqname;
    writer_meta_info_impl->writer_meta_info.ops->set_ownqname =
                                 w2c_schema_writer_meta_info_set_ownqname;
    writer_meta_info_impl->writer_meta_info.ops->get_element2schema_qnamemap =
                                 w2c_schema_writer_meta_info_get_element2schema_qnamemap;
    writer_meta_info_impl->writer_meta_info.ops->set_element2schema_qnamemap =
                                 w2c_schema_writer_meta_info_set_element2schema_qnamemap;
    writer_meta_info_impl->writer_meta_info.ops->get_element2classmap =
                                 w2c_schema_writer_meta_info_get_element2classmap;
    writer_meta_info_impl->writer_meta_info.ops->set_element2classmap =
                                 w2c_schema_writer_meta_info_set_element2classmap;
    writer_meta_info_impl->writer_meta_info.ops->get_specialtype_flagmap =
                                 w2c_schema_writer_meta_info_get_specialtype_flagmap;
    writer_meta_info_impl->writer_meta_info.ops->set_specialtype_flagmap =
                                 w2c_schema_writer_meta_info_set_specialtype_flagmap;
    writer_meta_info_impl->writer_meta_info.ops->get_qname_maxoccurs_countmap =
                                 w2c_schema_writer_meta_info_get_qname_maxoccurs_countmap;
    writer_meta_info_impl->writer_meta_info.ops->set_qname_maxoccurs_countmap =
                                 w2c_schema_writer_meta_info_set_qname_maxoccurs_countmap;
    writer_meta_info_impl->writer_meta_info.ops->get_qname_minoccurs_countmap =
                                 w2c_schema_writer_meta_info_get_qname_minoccurs_countmap;
    writer_meta_info_impl->writer_meta_info.ops->set_qname_minoccurs_countmap =
                                 w2c_schema_writer_meta_info_set_qname_minoccurs_countmap;

    writer_meta_info_impl->writer_meta_info.ops->get_length_facet =
                                 w2c_schema_writer_meta_info_get_length_facet;
    writer_meta_info_impl->writer_meta_info.ops->set_length_facet =
                                 w2c_schema_writer_meta_info_set_length_facet;
    writer_meta_info_impl->writer_meta_info.ops->get_maxexclusive_facet =
                                 w2c_schema_writer_meta_info_get_maxexclusive_facet;
    writer_meta_info_impl->writer_meta_info.ops->set_maxexclusive_facet =
                                 w2c_schema_writer_meta_info_set_maxexclusive_facet;
    writer_meta_info_impl->writer_meta_info.ops->get_minexclusive_facet =
                                 w2c_schema_writer_meta_info_get_minexclusive_facet;
    writer_meta_info_impl->writer_meta_info.ops->set_minexclusive_facet =
                                 w2c_schema_writer_meta_info_set_minexclusive_facet;
    writer_meta_info_impl->writer_meta_info.ops->get_maxinclusive_facet =
                                 w2c_schema_writer_meta_info_get_maxinclusive_facet;
    writer_meta_info_impl->writer_meta_info.ops->set_maxinclusive_facet =
                                 w2c_schema_writer_meta_info_set_maxinclusive_facet;
    writer_meta_info_impl->writer_meta_info.ops->get_mininclusive_facet =
                                 w2c_schema_writer_meta_info_get_mininclusive_facet;
    writer_meta_info_impl->writer_meta_info.ops->set_mininclusive_facet =
                                 w2c_schema_writer_meta_info_set_mininclusive_facet;
    writer_meta_info_impl->writer_meta_info.ops->get_maxlength_facet =
                                 w2c_schema_writer_meta_info_get_maxlength_facet;
    writer_meta_info_impl->writer_meta_info.ops->set_maxlength_facet =
                                 w2c_schema_writer_meta_info_set_maxlength_facet;
    writer_meta_info_impl->writer_meta_info.ops->get_minlength_facet =
                                 w2c_schema_writer_meta_info_get_minlength_facet;
    writer_meta_info_impl->writer_meta_info.ops->set_minlength_facet =
                                 w2c_schema_writer_meta_info_set_minlength_facet;
    writer_meta_info_impl->writer_meta_info.ops->get_parent =
                                 w2c_schema_writer_meta_info_get_parent;
    writer_meta_info_impl->writer_meta_info.ops->set_parent =
                                 w2c_schema_writer_meta_info_set_parent;

    writer_meta_info_impl->writer_meta_info.ops->is_restriction_base_type=
                                 w2c_schema_writer_meta_info_is_restriction_base_type;
    writer_meta_info_impl->writer_meta_info.ops->register_nillable_qname =
                                 w2c_schema_writer_meta_info_register_nillable_qname;
    writer_meta_info_impl->writer_meta_info.ops->is_nillable=
                                 w2c_schema_writer_meta_info_is_nillable;
    writer_meta_info_impl->writer_meta_info.ops->register_mapping =
                                 w2c_schema_writer_meta_info_register_mapping;
    writer_meta_info_impl->writer_meta_info.ops->get_schemaqname4qname =
                                 w2c_schema_writer_meta_info_get_schemaqname4qname;
    writer_meta_info_impl->writer_meta_info.ops->get_classname4qname =
                                 w2c_schema_writer_meta_info_get_classname4qname;
    writer_meta_info_impl->writer_meta_info.ops->get_attri_status4qname=
                                 w2c_schema_writer_meta_info_get_attri_status4qname;
    writer_meta_info_impl->writer_meta_info.ops->get_any_status4qname=
                                 w2c_schema_writer_meta_info_get_any_status4qname;
    writer_meta_info_impl->writer_meta_info.ops->get_array_status4qname=
                                 w2c_schema_writer_meta_info_get_array_status4qname;
    writer_meta_info_impl->writer_meta_info.ops->get_binary_status4qname=
                                 w2c_schema_writer_meta_info_get_binary_status4qname;
    writer_meta_info_impl->writer_meta_info.ops->get_any_attri_status4qname=
                                 w2c_schema_writer_meta_info_get_any_attri_status4qname;
    writer_meta_info_impl->writer_meta_info.ops->add_min_occurs =
                                 w2c_schema_writer_meta_info_add_min_occurs;
    writer_meta_info_impl->writer_meta_info.ops->add_max_occurs =
                                 w2c_schema_writer_meta_info_add_max_occurs;
    writer_meta_info_impl->writer_meta_info.ops->register_qname_index =
                                 w2c_schema_writer_meta_info_register_qname_index;
    writer_meta_info_impl->writer_meta_info.ops->get_min_occurs =
                                 w2c_schema_writer_meta_info_get_min_occurs;
    writer_meta_info_impl->writer_meta_info.ops->get_max_occurs =
                                 w2c_schema_writer_meta_info_get_max_occurs;
    writer_meta_info_impl->writer_meta_info.ops->get_qname_array =
                                 w2c_schema_writer_meta_info_get_qname_array;
    writer_meta_info_impl->writer_meta_info.ops->get_ordered_qname_array =
                                 w2c_schema_writer_meta_info_get_ordered_qname_array;
    writer_meta_info_impl->writer_meta_info.ops->get_order_start_point =
                                 w2c_schema_writer_meta_info_get_order_start_point;
    writer_meta_info_impl->writer_meta_info.ops->add_status =
                                 w2c_schema_writer_meta_info_add_status;
    writer_meta_info_impl->writer_meta_info.ops->add_enum_facet=
                                 w2c_schema_writer_meta_info_add_enum_facet;


    return &(writer_meta_info_impl->writer_meta_info);
}

axis2_status_t AXIS2_CALL 
w2c_schema_writer_meta_info_free (w2c_schema_writer_meta_info_t *schema_writer_meta_info, 
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *schema_writer_meta_info_impl = NULL;
    axis2_hash_index_t *hi = NULL;
    axis2_char_t *key = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    schema_writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    
    /*if (schema_writer_meta_info_impl-> extension_classname)
    {
        AXIS2_FREE(env-> allocator, schema_writer_meta_info_impl-> extension_classname);
    }
    if (schema_writer_meta_info_impl-> restriction_classname)
    {
        AXIS2_FREE(env-> allocator, schema_writer_meta_info_impl-> restriction_classname);
    }
    if (schema_writer_meta_info_impl-> own_classname)
    {
        AXIS2_FREE(env-> allocator, schema_writer_meta_info_impl-> own_classname);
    }*/
    if (schema_writer_meta_info_impl-> enum_facet)
    {
        AXIS2_FREE(env-> allocator, schema_writer_meta_info_impl-> enum_facet);
    }
    if (schema_writer_meta_info_impl->element2classmap)
    {
        axis2_hash_free(schema_writer_meta_info_impl->element2classmap, env);
    }
    if (schema_writer_meta_info_impl->element2schema_qnamemap)
    {
        axis2_hash_free(schema_writer_meta_info_impl->element2schema_qnamemap, env);
    }
    if (schema_writer_meta_info_impl->specialtype_flagmap)
    {
        axis2_hash_free(schema_writer_meta_info_impl->specialtype_flagmap, env);
    }
    if (schema_writer_meta_info_impl->qname_maxoccurs_countmap)
    {
        axis2_hash_free(schema_writer_meta_info_impl->qname_maxoccurs_countmap, env);
    }
    if (schema_writer_meta_info_impl->qname_minoccurs_countmap)
    {
        axis2_hash_free(schema_writer_meta_info_impl->qname_minoccurs_countmap, env);
    }
    if (schema_writer_meta_info_impl->qname_orderlist)
    {
        for (hi = axis2_hash_first( schema_writer_meta_info_impl-> qname_orderlist, env);
                                        hi; hi = axis2_hash_next(env, hi) )
        {
            axis2_hash_this(hi, (void*)&key, NULL, NULL);
            if ( key!= NULL ) /* apperently true */
            {
                AXIS2_FREE( env-> allocator, key);
            }
        }
        axis2_hash_free(schema_writer_meta_info_impl->qname_orderlist, env);
    }
    /*if (schema_writer_meta_info_impl-> pattern_facet)
    {
        AXIS2_FREE(env-> allocator, schema_writer_meta_info_impl-> pattern_facet);
    }
    
    if (schema_writer_meta_info_impl->extension_basetype)
    {
        axis2_qname_free(schema_writer_meta_info_impl->extension_basetype, env);
    }
    if (schema_writer_meta_info_impl->restriction_basetype)
    {
        axis2_qname_free(schema_writer_meta_info_impl->restriction_basetype, env);
    }
    if (schema_writer_meta_info_impl->ownqname)
    {
        axis2_qname_free(schema_writer_meta_info_impl->ownqname, env);
    }
    if (schema_writer_meta_info_impl->nillable_qnamelist)
    {
        axis2_hash_free(schema_writer_meta_info_impl->nillable_qnamelist, env);
    }
    if (schema_writer_meta_info_impl->parent)
    {
        W2C_SCHEMA_WRITER_META_INFO_FREE(schema_writer_meta_info_impl->parent, env);
    }*/

    if(schema_writer_meta_info->ops)
    {
        AXIS2_FREE(env->allocator, schema_writer_meta_info->ops);
        schema_writer_meta_info->ops = NULL;
    }
    if(schema_writer_meta_info_impl)
    {
        AXIS2_FREE(env->allocator, schema_writer_meta_info_impl);
        schema_writer_meta_info_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}



axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_ordered( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_bool_t ordered)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> ordered = ordered;
    return AXIS2_SUCCESS;
}
axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_ordered( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> ordered;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_anonymous( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_bool_t anonymous)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> anonymous = anonymous;
    return AXIS2_SUCCESS;
}
axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_anonymous( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> anonymous;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_choice( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_bool_t choice)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> choice = choice;
    return AXIS2_SUCCESS;
}
axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_choice( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> choice;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_extension( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_bool_t extension)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> extension = extension;
    return AXIS2_SUCCESS;
}
axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_extension( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> extension;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_restriction( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_bool_t restriction)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> restriction = restriction;
    return AXIS2_SUCCESS;
}
axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_restriction( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> restriction;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_extension_classname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_char_t *extension_classname)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> extension_classname = extension_classname;
    return AXIS2_SUCCESS;
}
axis2_char_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_extension_classname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> extension_classname;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_restriction_classname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_char_t *restriction_classname)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> restriction_classname = restriction_classname;
    return AXIS2_SUCCESS;
}
axis2_char_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_restriction_classname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> restriction_classname;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_own_classname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_char_t *own_classname)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> own_classname = own_classname;
    return AXIS2_SUCCESS;
}
axis2_char_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_own_classname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> own_classname;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_enum_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_char_t *enum_facet)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> enum_facet = axis2_strdup(env, enum_facet);
    return AXIS2_SUCCESS;
}
axis2_char_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_enum_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> enum_facet;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_pattern_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_char_t *pattern_facet)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> pattern_facet = pattern_facet;
    return AXIS2_SUCCESS;
}
axis2_char_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_pattern_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> pattern_facet;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_extension_basetype( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_qname_t *extension_basetype)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> extension_basetype = extension_basetype;
    return AXIS2_SUCCESS;
}
axis2_qname_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_extension_basetype( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> extension_basetype;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_restriction_basetype( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_qname_t *restriction_basetype)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> restriction_basetype = restriction_basetype;
    return AXIS2_SUCCESS;
}
axis2_qname_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_restriction_basetype( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> restriction_basetype;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_ownqname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_qname_t *ownqname)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> ownqname = ownqname;
    return AXIS2_SUCCESS;
}
axis2_qname_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_ownqname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> ownqname;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_element2schema_qnamemap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_hash_t *element2schema_qnamemap)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> element2schema_qnamemap = element2schema_qnamemap;
    return AXIS2_SUCCESS;
}
axis2_hash_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_element2schema_qnamemap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> element2schema_qnamemap;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_element2classmap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_hash_t *element2classmap)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> element2classmap = element2classmap;
    return AXIS2_SUCCESS;
}
axis2_hash_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_element2classmap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> element2classmap;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_specialtype_flagmap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_hash_t *specialtype_flagmap)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> specialtype_flagmap = specialtype_flagmap;
    return AXIS2_SUCCESS;
}
axis2_hash_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_specialtype_flagmap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> specialtype_flagmap;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_qname_maxoccurs_countmap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_hash_t *qname_maxoccurs_countmap)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> qname_maxoccurs_countmap = qname_maxoccurs_countmap;
    return AXIS2_SUCCESS;
}
axis2_hash_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_qname_maxoccurs_countmap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> qname_maxoccurs_countmap;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_qname_minoccurs_countmap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            axis2_hash_t *qname_minoccurs_countmap)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> qname_minoccurs_countmap = qname_minoccurs_countmap;
    return AXIS2_SUCCESS;
}
axis2_hash_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_qname_minoccurs_countmap( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> qname_minoccurs_countmap;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_length_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            long length_facet)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> length_facet = length_facet;
    return AXIS2_SUCCESS;
}
long AXIS2_CALL
w2c_schema_writer_meta_info_get_length_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, 0);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> length_facet;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_maxexclusive_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            long maxexclusive_facet)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> maxexclusive_facet = maxexclusive_facet;
    return AXIS2_SUCCESS;
}
long AXIS2_CALL
w2c_schema_writer_meta_info_get_maxexclusive_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, 0);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> maxexclusive_facet;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_minexclusive_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            long minexclusive_facet)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> minexclusive_facet = minexclusive_facet;
    return AXIS2_SUCCESS;
}
long AXIS2_CALL
w2c_schema_writer_meta_info_get_minexclusive_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, 0);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> minexclusive_facet;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_maxinclusive_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            long maxinclusive_facet)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> maxinclusive_facet = maxinclusive_facet;
    return AXIS2_SUCCESS;
}
long AXIS2_CALL
w2c_schema_writer_meta_info_get_maxinclusive_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, 0);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> maxinclusive_facet;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_mininclusive_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            long mininclusive_facet)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> mininclusive_facet = mininclusive_facet;
    return AXIS2_SUCCESS;
}
long AXIS2_CALL
w2c_schema_writer_meta_info_get_mininclusive_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, 0);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> mininclusive_facet;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_maxlength_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            long maxlength_facet)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> maxlength_facet = maxlength_facet;
    return AXIS2_SUCCESS;
}
long AXIS2_CALL
w2c_schema_writer_meta_info_get_maxlength_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, 0);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> maxlength_facet;
}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_minlength_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            long minlength_facet)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> minlength_facet = minlength_facet;
    return AXIS2_SUCCESS;
}
long AXIS2_CALL
w2c_schema_writer_meta_info_get_minlength_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, 0);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> minlength_facet;
}



axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_set_parent( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env,
                            w2c_schema_writer_meta_info_t *parent)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> parent = parent;
    return AXIS2_SUCCESS;
}
w2c_schema_writer_meta_info_t *AXIS2_CALL
w2c_schema_writer_meta_info_get_parent( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                            const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    return writer_meta_info_impl-> parent;
}

/** functions other than getters and setters */
axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_is_restriction_base_type( 
                             w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *restriction_base_type)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    return axis2_qname_equals( W2C_INTF_TO_IMPL(schema_writer_meta_info)-> restriction_basetype,
                    env,
                    restriction_base_type );
}


axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_register_nillable_qname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    axis2_char_t *qname_str = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);

    qname_str = w2c_string_make_key_from_qname( qname, env);
    axis2_hash_set( writer_meta_info_impl-> nillable_qnamelist, qname_str,
                 AXIS2_HASH_KEY_STRING, (void*)1);

    return AXIS2_SUCCESS;
}


axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_is_nillable( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    axis2_char_t *qname_str = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);

    qname_str = w2c_string_make_key_from_qname( qname, env);
    return (axis2_bool_t)axis2_hash_get( writer_meta_info_impl-> nillable_qnamelist, qname_str,
                 AXIS2_HASH_KEY_STRING);


}

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_register_mapping( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname,
                             axis2_qname_t *schema_name,
                             axis2_char_t *class_name,
                             int type)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    axis2_char_t *qname_str = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);

    qname_str = w2c_string_make_key_from_qname( qname, env);
    axis2_hash_set( writer_meta_info_impl-> element2classmap, qname_str, 
            AXIS2_HASH_KEY_STRING, class_name);
    axis2_hash_set( writer_meta_info_impl-> element2schema_qnamemap, qname_str, 
            AXIS2_HASH_KEY_STRING, schema_name);
    w2c_schema_writer_meta_info_add_status( schema_writer_meta_info, env, qname, type);
    return AXIS2_SUCCESS;
}


axis2_qname_t* AXIS2_CALL
w2c_schema_writer_meta_info_get_schemaqname4qname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    axis2_char_t *qname_str = NULL;
    axis2_qname_t *ret_qname = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);

    qname_str = w2c_string_make_key_from_qname( qname, env);
    ret_qname = (axis2_qname_t*)axis2_hash_get( writer_meta_info_impl-> element2schema_qnamemap, qname_str,
             AXIS2_HASH_KEY_STRING);
    return ret_qname;
}

axis2_char_t* AXIS2_CALL
w2c_schema_writer_meta_info_get_classname4qname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    axis2_char_t *qname_str = NULL;
    axis2_char_t *ret_name = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);

    qname_str = w2c_string_make_key_from_qname( qname, env);
    ret_name = (axis2_char_t*)axis2_hash_get( writer_meta_info_impl-> element2classmap, qname_str,
             AXIS2_HASH_KEY_STRING);
    return ret_name;
}

axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_attri_status4qname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    axis2_char_t *qname_str = NULL;
    int state = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    qname_str = w2c_string_make_key_from_qname( qname, env);
    state = (int)axis2_hash_get( writer_meta_info_impl-> specialtype_flagmap, qname_str,
             AXIS2_HASH_KEY_STRING);
    if ( state != 0 )
    {
        return w2c_schema_writer_meta_info_get_status( writer_meta_info_impl, env,
                         state, W2C_SCHEMA_CONSTS_ATTRIBUTE_TYPE);
    }
    return AXIS2_FALSE;
}

axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_any_status4qname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    axis2_char_t *qname_str = NULL;
    int state = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    qname_str = w2c_string_make_key_from_qname( qname, env);
    state = (int)axis2_hash_get( writer_meta_info_impl-> specialtype_flagmap, qname_str,
             AXIS2_HASH_KEY_STRING);
    if ( state != 0 )
    {
        return w2c_schema_writer_meta_info_get_status( writer_meta_info_impl, env,
                         state, W2C_SCHEMA_CONSTS_ANY_TYPE);
    }
    return AXIS2_FALSE;
}

axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_array_status4qname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    axis2_char_t *qname_str = NULL;
    int state = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    qname_str = w2c_string_make_key_from_qname( qname, env);
    state = (int)axis2_hash_get( writer_meta_info_impl-> specialtype_flagmap, qname_str,
             AXIS2_HASH_KEY_STRING);
    if ( state != 0 )
    {
        return w2c_schema_writer_meta_info_get_status( writer_meta_info_impl, env,
                         state, W2C_SCHEMA_CONSTS_ARRAY_TYPE);
    }
    return AXIS2_FALSE;
}

axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_binary_status4qname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    axis2_char_t *qname_str = NULL;
    int state = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    qname_str = w2c_string_make_key_from_qname( qname, env);
    state = (int)axis2_hash_get( writer_meta_info_impl-> specialtype_flagmap, qname_str,
             AXIS2_HASH_KEY_STRING);
    if ( state != 0 )
    {
        return w2c_schema_writer_meta_info_get_status( writer_meta_info_impl, env,
                         state, W2C_SCHEMA_CONSTS_BINARY_TYPE);
    }
    return AXIS2_FALSE;
}

axis2_bool_t AXIS2_CALL
w2c_schema_writer_meta_info_get_any_attri_status4qname( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);

    return w2c_schema_writer_meta_info_get_any_status4qname(
                      schema_writer_meta_info, env, qname ) &&
           w2c_schema_writer_meta_info_get_attri_status4qname(
                      schema_writer_meta_info, env, qname );
}
         
axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_add_min_occurs( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname,
                             long min_occurs)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    axis2_char_t *qname_str = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    qname_str = w2c_string_make_key_from_qname( qname, env);
    axis2_hash_set( writer_meta_info_impl-> qname_minoccurs_countmap, qname_str,
             AXIS2_HASH_KEY_STRING, (void*)min_occurs);
    return AXIS2_SUCCESS;
} 
          
axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_add_max_occurs( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname,
                             long max_occurs)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    axis2_char_t *qname_str = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    qname_str = w2c_string_make_key_from_qname( qname, env);
    axis2_hash_set( writer_meta_info_impl-> qname_maxoccurs_countmap, qname_str,
             AXIS2_HASH_KEY_STRING, (void*)max_occurs);
    return AXIS2_SUCCESS;
} 

axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_register_qname_index( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname,
                             int index)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    axis2_char_t *index_str = NULL; 

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    index_str = (axis2_char_t*)AXIS2_MALLOC( env-> allocator, 32*sizeof(int));
    sprintf( index_str, "%d", index);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    
    axis2_hash_set( writer_meta_info_impl-> qname_orderlist, (void*)index_str, AXIS2_HASH_KEY_STRING, (void*)qname);
                                             
    return AXIS2_SUCCESS;
}

long AXIS2_CALL
w2c_schema_writer_meta_info_get_min_occurs( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    axis2_char_t *qname_str = NULL;
    long min_occurs = 0;

    AXIS2_ENV_CHECK(env, 0);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    qname_str = w2c_string_make_key_from_qname( qname, env);
    min_occurs = (int)axis2_hash_get( writer_meta_info_impl-> qname_minoccurs_countmap, qname_str,
             AXIS2_HASH_KEY_STRING);
    return min_occurs?min_occurs:1; /* default to 1 */
}

long AXIS2_CALL
w2c_schema_writer_meta_info_get_max_occurs( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *qname)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    axis2_char_t *qname_str = NULL;
    long max_occurs = 0;

    AXIS2_ENV_CHECK(env, 0);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    qname_str = w2c_string_make_key_from_qname( qname, env);
    max_occurs = (int)axis2_hash_get( writer_meta_info_impl-> qname_maxoccurs_countmap, qname_str,
             AXIS2_HASH_KEY_STRING);
    return max_occurs?max_occurs:1; /* default to 1 */
}  

axutil_array_list_t* AXIS2_CALL
w2c_schema_writer_meta_info_get_qname_array( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                                     const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    axis2_char_t *qname_str = NULL;
    axis2_hash_index_t *hi = NULL;
    axutil_array_list_t *arr_list = NULL;
    axis2_qname_t *qname = NULL;
    int i = 0;


    AXIS2_ENV_CHECK(env, NULL);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    arr_list = axutil_array_list_create( env, 10 );
    for (hi = axis2_hash_first( writer_meta_info_impl-> element2classmap, env), i = 0;
                        hi; hi = axis2_hash_next(env, hi), i++)
    {
        axis2_hash_this(hi, (void*)&qname_str, NULL, NULL);
        if (qname_str)
        {
            qname = axis2_qname_create_from_string( env, qname_str);
            axutil_array_list_add_at( arr_list, env, i, qname);
        }
    }
    return arr_list;
}

axutil_array_list_t* AXIS2_CALL
w2c_schema_writer_meta_info_get_ordered_qname_array( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                                     const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    axutil_array_list_t *arr_list = NULL;
    int i = 0;
    int real_index = 0;
    int size = 0;
    axis2_qname_t *qname = NULL;
    axutil_array_list_t *temp_arr_list = NULL;
    axis2_char_t index_str[32];
    


    AXIS2_ENV_CHECK(env, NULL);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);

    size = w2c_schema_writer_meta_info_get_order_start_point( schema_writer_meta_info , env);
    arr_list = axutil_array_list_create( env, size);

    /** copy all the qnames in the orderemap */
    for ( i = 0, real_index = 0; i < size; i ++ )
    {
        sprintf(index_str, "%d", i);
        qname = (axis2_qname_t*) axis2_hash_get(  
                writer_meta_info_impl-> qname_orderlist, index_str, AXIS2_HASH_KEY_STRING );
        if ( qname != NULL )
        {
            axutil_array_list_add_at( arr_list, env, real_index, qname );
            real_index ++;
        }
    }
  
    temp_arr_list = w2c_schema_writer_meta_info_get_qname_array( schema_writer_meta_info, env);

    size = axutil_array_list_size( temp_arr_list, env);
    for ( i = 0; i < size; i ++)
    {
        qname = (axis2_qname_t*) axutil_array_list_get(
                       temp_arr_list, env, i );
        if ( w2c_schema_writer_meta_info_get_any_attri_status4qname( 
                    schema_writer_meta_info, env, qname ) )
        {
            axutil_array_list_add_at( arr_list, env, real_index, qname );
            real_index ++;
        } 
    }
    axutil_array_list_free( temp_arr_list, env);
     
    return arr_list;
}

int AXIS2_CALL
w2c_schema_writer_meta_info_get_order_start_point( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                                     const axis2_env_t *env)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    int count = 0;
    axis2_char_t *x = NULL;
    axis2_hash_index_t *hi = NULL;

    AXIS2_ENV_CHECK(env, 0);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);

    for (count = 0, hi = axis2_hash_first( writer_meta_info_impl-> qname_orderlist, env);
                    hi; hi = axis2_hash_next(env, hi) )
    {
        axis2_hash_this(hi, (void*)&x, NULL, NULL);
        if ( x!= NULL ) /* apperently true */
        {
            count ++;
        }
    }
    return count;
}


axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_add_status( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_qname_t *type,
                             int mask)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;
    axis2_char_t *qname_str = NULL;
    int status = 0;

    AXIS2_ENV_CHECK(env, 0);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    qname_str = w2c_string_make_key_from_qname( type, env);
    status = (int)axis2_hash_get( writer_meta_info_impl-> specialtype_flagmap, qname_str,
             AXIS2_HASH_KEY_STRING);

    if ( status != 0 )
    {
        axis2_hash_set( writer_meta_info_impl-> specialtype_flagmap, qname_str,
                             AXIS2_HASH_KEY_STRING, (void*)(status | mask));
    }
    axis2_hash_set( writer_meta_info_impl-> specialtype_flagmap, qname_str,
                             AXIS2_HASH_KEY_STRING, (void*)mask);
    return AXIS2_SUCCESS;
}

/** private method */
axis2_bool_t 
w2c_schema_writer_meta_info_get_status( w2c_schema_writer_meta_info_impl_t *schema_writer_meta_info_impl,
                             const axis2_env_t *env,
                             int stored_status,
                             int mask)
{
    return mask == (mask & stored_status);
}


axis2_status_t AXIS2_CALL
w2c_schema_writer_meta_info_add_enum_facet( w2c_schema_writer_meta_info_t *schema_writer_meta_info,
                             const axis2_env_t *env,
                             axis2_char_t *facet)
{
    w2c_schema_writer_meta_info_impl_t *writer_meta_info_impl = NULL;

    AXIS2_ENV_CHECK(env, 0);
    writer_meta_info_impl = W2C_INTF_TO_IMPL(schema_writer_meta_info);
    writer_meta_info_impl-> enum_facet = w2c_string_add_string( writer_meta_info_impl-> enum_facet, "|", env);
    writer_meta_info_impl-> enum_facet = w2c_string_add_string( writer_meta_info_impl-> enum_facet, facet, env);
    return AXIS2_SUCCESS;
}
