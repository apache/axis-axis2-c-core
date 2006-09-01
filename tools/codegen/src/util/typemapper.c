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
 
#include <w2c_typemapper.h>
#include <axis2_array_list.h>
#include <axis2_string.h>
#include <w2c_string.h>
#include <axiom_document.h>
#include <axiom_xml_reader.h>
#include <axiom_stax_builder.h>
#include <axiom_element.h>
#include <axiom_node.h>
#include <axiom_children_iterator.h>
#include <stdlib.h>



#define W2C_TYPEMAPPER_UPPER_PARAM_LIMIT 1000
#define W2C_TYPEMAPPER_PARAM_SUFFIX "param"

/** 
 * @brief
 */
typedef struct w2c_typemapper_impl
{
    w2c_typemapper_t typemapper;

    axis2_char_t *mapper_name;
    axis2_hash_t *qname2name;
    axis2_char_t *default_type;
    axis2_qname_t *default_qname;

       
} w2c_typemapper_impl_t;

typedef struct w2c_typemapper_type_info
{
    axis2_qname_t *qname;
    axis2_char_t *type;
} w2c_typemapper_type_info_t;
#define W2C_INTF_TO_IMPL(typemapper) \
    ((w2c_typemapper_impl_t *) typemapper)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
w2c_typemapper_free (w2c_typemapper_t *typemapper, 
           const axis2_env_t *env);

axis2_char_t* AXIS2_CALL
w2c_typemapper_get_parameter_name(w2c_typemapper_t *typemapper,
            const axis2_env_t *env,
            axis2_qname_t *qname);

axis2_char_t* AXIS2_CALL
w2c_typemapper_get_type_name(w2c_typemapper_t *typemapper,
            const axis2_env_t *env,
            axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
w2c_typemapper_add_typemapping_name(w2c_typemapper_t *typemapper,
            const axis2_env_t *env,
            axis2_qname_t *qname,
            axis2_char_t *name);

axis2_char_t* AXIS2_CALL
w2c_typemapper_get_default_mapping_name(w2c_typemapper_t *typemapper,
            const axis2_env_t *env);

axis2_qname_t* AXIS2_CALL
w2c_typemapper_get_default_qname(w2c_typemapper_t *typemapper,
            const axis2_env_t *env);

 /************************** End of function prototypes ************************/

AXIS2_EXTERN w2c_typemapper_t * AXIS2_CALL 
w2c_typemapper_create (const axis2_env_t *env)
{
    w2c_typemapper_impl_t *typemapper_impl = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);

    typemapper_impl = (w2c_typemapper_impl_t *) AXIS2_MALLOC(env->
               allocator, sizeof(w2c_typemapper_impl_t));

    if(NULL == typemapper_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    typemapper_impl->qname2name = NULL;
    typemapper_impl->default_type = NULL;
    
    typemapper_impl->typemapper.ops = 
    AXIS2_MALLOC (env->allocator, sizeof(w2c_typemapper_ops_t));
    if(NULL == typemapper_impl->typemapper.ops)
    {
        w2c_typemapper_free(&(typemapper_impl->typemapper), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    typemapper_impl->typemapper.ops->free = w2c_typemapper_free;
    typemapper_impl->typemapper.ops->get_parameter_name = 
                                        w2c_typemapper_get_parameter_name;
    typemapper_impl->typemapper.ops->get_type_name= 
                                        w2c_typemapper_get_type_name;
    typemapper_impl->typemapper.ops->add_typemapping_name = 
                                        w2c_typemapper_add_typemapping_name;
    typemapper_impl->typemapper.ops->get_default_mapping_name = 
                                        w2c_typemapper_get_default_mapping_name;
    typemapper_impl->typemapper.ops->get_default_qname= 
                                        w2c_typemapper_get_default_qname;

    return &(typemapper_impl->typemapper);
}

AXIS2_EXTERN w2c_typemapper_t * AXIS2_CALL 
w2c_typemapper_create_from_file(const axis2_env_t *env,
                          axis2_char_t *filename)
{
    w2c_typemapper_impl_t *typemapper_impl = NULL;
    axiom_xml_reader_t *xml_reader      = NULL;
    axiom_stax_builder_t *om_builder = NULL;
    axiom_document_t *om_doc         = NULL;
    axis2_char_t *file_path = NULL;
    axiom_node_t *root = NULL;
    axiom_element_t *root_ele = NULL;
    axiom_children_iterator_t *it = NULL;
    axiom_node_t *child = NULL;
    axiom_element_t *child_ele = NULL;
    axis2_bool_t is_default = AXIS2_FALSE;
    axis2_char_t *uri = NULL;
    axis2_char_t *prefix = NULL;
    axis2_char_t *local_name = NULL;
    axis2_qname_t *qname = NULL;
    axis2_char_t *type_val = NULL;
    w2c_typemapper_type_info_t *type_info = NULL;
    axis2_char_t *qname2str = NULL;
    axis2_char_t *attri_val = NULL;

  
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, filename, NULL);
    
    typemapper_impl = (w2c_typemapper_impl_t *) w2c_typemapper_create(env);
    if(!typemapper_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;  
    }
    file_path = getenv("AXIS2C_HOME");
    file_path = AXIS2_STRDUP( file_path, env);
    file_path = w2c_string_add_string(file_path, W2C_TYPEMAPPER_MAPPING_PATH, env );
    file_path = w2c_string_add_string(file_path, filename, env );

    xml_reader =
        axiom_xml_reader_create_for_file(env, file_path, NULL);
    if(!xml_reader)
        return AXIS2_FAILURE;
    om_builder = axiom_stax_builder_create(env, xml_reader);
    om_doc = axiom_document_create(env, NULL, om_builder);
    AXIOM_DOCUMENT_BUILD_ALL(om_doc, env);

    typemapper_impl-> qname2name = axis2_hash_make ( env);
    root = AXIOM_DOCUMENT_GET_ROOT_ELEMENT(om_doc, env);
    root_ele = AXIOM_NODE_GET_DATA_ELEMENT(root, env);
    typemapper_impl-> mapper_name =
        AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(root_ele, env, "dbf");
    typemapper_impl-> mapper_name = 
        AXIS2_STRDUP( typemapper_impl-> mapper_name, env);
    it = AXIOM_ELEMENT_GET_CHILDREN(root_ele, env, root);
    while ( (child = AXIOM_CHILDREN_ITERATOR_NEXT(it, env )) != NULL )
    {
        child_ele = AXIOM_NODE_GET_DATA_ELEMENT( child, env);
        is_default = AXIS2_FALSE;
        if ( (attri_val = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME
                    (child_ele, env, "default") ) != NULL )
        {
            is_default = AXIS2_TRUE;
        }
        child = AXIOM_NODE_GET_FIRST_CHILD( child, env);
        child_ele = AXIOM_NODE_GET_DATA_ELEMENT( child, env);
        uri = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME
                    (child_ele, env, "namespace");
        uri = AXIS2_STRDUP( uri, env);
        prefix = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME
                                (child_ele, env, "prefix");
        prefix = AXIS2_STRDUP( prefix, env);
        local_name = AXIOM_ELEMENT_GET_TEXT(child_ele, env, child);
        local_name = AXIS2_STRDUP( local_name, env);
        qname = axis2_qname_create( env, local_name, uri, prefix);
        qname2str = AXIS2_QNAME_TO_STRING( qname, env);
        qname2str = AXIS2_STRDUP( qname2str, env);

        child = AXIOM_NODE_GET_NEXT_SIBLING( child, env );
        child_ele = AXIOM_NODE_GET_DATA_ELEMENT( child, env);
        type_val = AXIOM_ELEMENT_GET_TEXT( child_ele, env, child);
        type_val = AXIS2_STRDUP( type_val, env);
        if ( is_default )
        {
            typemapper_impl-> default_type = AXIS2_STRDUP( type_val, env);
            typemapper_impl-> default_qname = qname;
        }
        type_info = (w2c_typemapper_type_info_t*) AXIS2_MALLOC
              ( env-> allocator, sizeof(w2c_typemapper_type_info_t) );
        type_info-> qname =  qname;
        type_info-> type = type_val;
        axis2_hash_set( typemapper_impl-> qname2name, qname2str,
                        AXIS2_HASH_KEY_STRING, type_info );
    }

    AXIOM_STAX_BUILDER_FREE(om_builder, env);
    axiom_xml_reader_cleanup();

   return &(typemapper_impl->typemapper);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
w2c_typemapper_free (w2c_typemapper_t *typemapper, 
                            const axis2_env_t *env)
{
    w2c_typemapper_impl_t *typemapper_impl = NULL;
 
    axis2_char_t *key = NULL;
    w2c_typemapper_type_info_t* value =NULL;
    axis2_hash_index_t *hi = NULL;
    axis2_hash_t *h = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    typemapper_impl = W2C_INTF_TO_IMPL(typemapper);
    h = typemapper_impl-> qname2name;
    
    for (hi = axis2_hash_first(h, env) ;
                hi; hi = axis2_hash_next(env, hi))
    {
        axis2_hash_this(hi, (void*)&key, NULL, (void*)&value);
        if (value)
        {
            if (value-> type)
                AXIS2_FREE ( env-> allocator, value-> type );
            if (value-> qname)
                AXIS2_QNAME_FREE( value-> qname, env);
        }
        if (key) AXIS2_FREE ( env-> allocator, key); 
    }
    axis2_hash_free( h , env);
    
    if (typemapper_impl-> mapper_name)
    {
        AXIS2_FREE (env-> allocator, typemapper_impl-> mapper_name);
    }
    if (typemapper_impl-> default_type)
    {
        AXIS2_FREE (env-> allocator, typemapper_impl-> default_type);
    }
    if(typemapper->ops)
    {
        AXIS2_FREE(env->allocator, typemapper->ops);
        typemapper->ops = NULL;
    }
    if(typemapper_impl)
    {
        AXIS2_FREE(env->allocator, typemapper_impl);
        typemapper_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}


axis2_char_t* AXIS2_CALL
w2c_typemapper_get_parameter_name(w2c_typemapper_t *typemapper,
            const axis2_env_t *env,
            axis2_qname_t *qname)
{
    w2c_typemapper_impl_t *typemapper_impl = NULL;
 
    axis2_char_t *key = NULL;
    w2c_typemapper_type_info_t *value =NULL;
    axis2_hash_t *h = NULL;
    axis2_char_t *prefix= NULL;
    static int counter = 0;
    axis2_char_t counter_str[32];

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    typemapper_impl = W2C_INTF_TO_IMPL(typemapper);
    h = typemapper_impl-> qname2name;

    key = AXIS2_QNAME_TO_STRING(qname, env);
    value = axis2_hash_get( h, key, AXIS2_HASH_KEY_STRING );
    if ( NULL == value )
    {
        return NULL;
    }

    qname = value-> qname;

    prefix = AXIS2_QNAME_GET_LOCALPART( qname, env);
    prefix = AXIS2_STRDUP( prefix, env);

    if (counter == W2C_TYPEMAPPER_UPPER_PARAM_LIMIT)
    {
        counter = 0;
    }
    sprintf(counter_str,"%d",counter++);
    prefix = w2c_string_add_string(prefix, counter_str, env );
    
    return prefix;    
}

axis2_char_t* AXIS2_CALL
w2c_typemapper_get_type_name(w2c_typemapper_t *typemapper,
            const axis2_env_t *env,
            axis2_qname_t *qname)
{
    w2c_typemapper_impl_t *typemapper_impl = NULL;
 
    axis2_char_t *key = NULL;
    w2c_typemapper_type_info_t *value =NULL;
    axis2_hash_t *h = NULL;
    axis2_char_t *type= NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    typemapper_impl = W2C_INTF_TO_IMPL(typemapper);
    h = typemapper_impl-> qname2name;

    key = AXIS2_QNAME_TO_STRING(qname, env);
    value = axis2_hash_get( h, key, AXIS2_HASH_KEY_STRING );
    if ( NULL == value )
        return NULL;
    
    type = value-> type;
    return type;    
}


axis2_status_t AXIS2_CALL
w2c_typemapper_add_typemapping_name(w2c_typemapper_t *typemapper,
            const axis2_env_t *env,
            axis2_qname_t *qname,
            axis2_char_t *name)
{
    w2c_typemapper_impl_t *typemapper_impl = NULL;
    axis2_hash_t *h = NULL;
    axis2_char_t *key= NULL;
    w2c_typemapper_type_info_t *type_info = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    typemapper_impl = W2C_INTF_TO_IMPL(typemapper);
    h = typemapper_impl-> qname2name;

    key = AXIS2_QNAME_TO_STRING( qname, env);
    type_info = (w2c_typemapper_type_info_t*) AXIS2_MALLOC
              ( env-> allocator, sizeof(w2c_typemapper_type_info_t) );
    type_info-> qname =  qname;
    type_info-> type = name;
    axis2_hash_set( typemapper_impl-> qname2name, key,
                        AXIS2_HASH_KEY_STRING, type_info );
    return AXIS2_SUCCESS;
}
 
axis2_char_t* AXIS2_CALL
w2c_typemapper_get_default_mapping_name(w2c_typemapper_t *typemapper,
            const axis2_env_t *env)
{
    w2c_typemapper_impl_t *typemapper_impl = NULL;
 
    AXIS2_ENV_CHECK(env, NULL);
    
    typemapper_impl = W2C_INTF_TO_IMPL(typemapper);

    return typemapper_impl-> default_type;
}


axis2_qname_t* AXIS2_CALL
w2c_typemapper_get_default_qname(w2c_typemapper_t *typemapper,
            const axis2_env_t *env)
{
    w2c_typemapper_impl_t *typemapper_impl = NULL;
 
    AXIS2_ENV_CHECK(env, NULL);
    
    typemapper_impl = W2C_INTF_TO_IMPL(typemapper);

    return typemapper_impl-> default_qname;
}


