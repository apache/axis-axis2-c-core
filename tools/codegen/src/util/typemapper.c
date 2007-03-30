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
 
#include <w2c_typemapper.h>
#include <axutil_array_list.h>
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
    axutil_hash_t *qname2name;
    axis2_char_t *default_type;
    axis2_qname_t *default_qname;
    axutil_hash_t *primitive_map;

    axutil_hash_t *qname2prefix;

       
} w2c_typemapper_impl_t;

#define W2C_INTF_TO_IMPL(typemapper) \
    ((w2c_typemapper_impl_t *) typemapper)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
w2c_typemapper_free (w2c_typemapper_t *typemapper, 
           const axutil_env_t *env);

axis2_char_t* AXIS2_CALL
w2c_typemapper_get_parameter_name(w2c_typemapper_t *typemapper,
            const axutil_env_t *env,
            axis2_qname_t *qname);

axis2_char_t* AXIS2_CALL
w2c_typemapper_get_type_name(w2c_typemapper_t *typemapper,
            const axutil_env_t *env,
            axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
w2c_typemapper_add_typemapping_name(w2c_typemapper_t *typemapper,
            const axutil_env_t *env,
            axis2_qname_t *qname,
            axis2_char_t *name);

axis2_char_t* AXIS2_CALL
w2c_typemapper_get_default_mapping_name(w2c_typemapper_t *typemapper,
            const axutil_env_t *env);

axis2_qname_t* AXIS2_CALL
w2c_typemapper_get_default_qname(w2c_typemapper_t *typemapper,
            const axutil_env_t *env);

axutil_hash_t* AXIS2_CALL
w2c_typemapper_get_all(w2c_typemapper_t *typemapper,
            const axutil_env_t *env);

axis2_bool_t AXIS2_CALL
w2c_typemapper_is_primitive(w2c_typemapper_t *typemapper,
                    const axutil_env_t *env,
                    axis2_qname_t *qname);

 /************************** End of function prototypes ************************/

AXIS2_EXTERN w2c_typemapper_t * AXIS2_CALL 
w2c_typemapper_create (const axutil_env_t *env)
{
    w2c_typemapper_impl_t *typemapper_impl = NULL;
   
    AXIS2_ENV_CHECK(env, NULL);

    typemapper_impl = (w2c_typemapper_impl_t *) AXIS2_MALLOC(env->
               allocator, sizeof(w2c_typemapper_impl_t));

    if(! typemapper_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    typemapper_impl->qname2name = NULL;
    typemapper_impl->qname2prefix = NULL;
    typemapper_impl->default_type = NULL;
    
    typemapper_impl->typemapper.ops = 
    AXIS2_MALLOC (env->allocator, sizeof(w2c_typemapper_ops_t));
    if(! typemapper_impl->typemapper.ops)
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
    typemapper_impl->typemapper.ops->get_all= 
                                        w2c_typemapper_get_all;
    typemapper_impl->typemapper.ops->is_primitive= 
                                        w2c_typemapper_is_primitive;

    return &(typemapper_impl->typemapper);
}

AXIS2_EXTERN w2c_typemapper_t * AXIS2_CALL 
w2c_typemapper_create_from_file(const axutil_env_t *env,
                          axis2_char_t *filename)
{
    w2c_typemapper_impl_t *typemapper_impl = NULL;
    axiom_xml_reader_t *xml_reader      = NULL;
    axiom_stax_builder_t *om_builder = NULL;
    axiom_document_t *om_doc         = NULL;
    axis2_char_t *file_path = NULL;
    axiom_node_t *root = NULL;
    axiom_element_t *root_ele = NULL;
    axiom_children_iterator_t *it = NULL, *it2 = NULL;
    axiom_node_t *child = NULL;
    axiom_element_t *child_ele = NULL;
    axis2_bool_t is_default = AXIS2_FALSE;
    axis2_char_t *uri = NULL;
    axis2_char_t *prefix = NULL;
    axis2_char_t *local_name = NULL;
    axis2_char_t *attri_val = NULL;
    axis2_char_t *type_val = NULL;
    axis2_char_t *qname2str = NULL;

  
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, filename, NULL);
    
    typemapper_impl = (w2c_typemapper_impl_t *) w2c_typemapper_create(env);
    if(!typemapper_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;  
    }
    file_path = getenv("AXIS2C_HOME");
    file_path = axis2_strdup(env, file_path);
    file_path = w2c_string_add_string(file_path, W2C_TYPEMAPPER_MAPPING_PATH, env );
    file_path = w2c_string_add_string(file_path, filename, env );

    xml_reader =
        axiom_xml_reader_create_for_file(env, file_path, NULL);
    if(!xml_reader)
		return NULL;
    om_builder = axiom_stax_builder_create(env, xml_reader);
    om_doc = axiom_document_create(env, NULL, om_builder);
    axiom_document_build_all(om_doc, env);

    typemapper_impl-> qname2name = axutil_hash_make ( env);
    typemapper_impl-> qname2prefix = axutil_hash_make ( env);
    typemapper_impl-> primitive_map = axutil_hash_make ( env);
    root = axiom_document_get_root_element(om_doc, env);
    root_ele = AXIOM_NODE_GET_DATA_ELEMENT(root, env);
    typemapper_impl-> mapper_name =
        axiom_element_get_attribute_value_by_name(root_ele, env, "dbf");
    typemapper_impl-> mapper_name = 
        axis2_strdup(env, typemapper_impl-> mapper_name);
    it = axiom_element_get_children(root_ele, env, root);
    while ( (child = axiom_children_iterator_next(it, env )) != NULL )
    {
        if(AXIOM_NODE_GET_NODE_TYPE(child, env) != AXIOM_ELEMENT)
        {
            continue; /* skip the starting empty */
        }

        child_ele = AXIOM_NODE_GET_DATA_ELEMENT( child, env);
        is_default = AXIS2_FALSE;
        if ( (attri_val = axiom_element_get_attribute_value_by_name
                    (child_ele, env, "default") ) != NULL )
        {
            is_default = AXIS2_TRUE;
        }
        /** next get the 2nd child iterator to the child */
        it2 = axiom_element_get_children( child_ele, env, child);

        /** here on ward child referes to the childs of child */
        while ( (child = axiom_children_iterator_next(it2, env )) != NULL )
        {
            if( AXIOM_NODE_GET_NODE_TYPE(child, env) == AXIOM_ELEMENT )
            {
                break; /* stop at the first valid child */
            }
        }

        child_ele = AXIOM_NODE_GET_DATA_ELEMENT( child, env);
        uri = axiom_element_get_attribute_value_by_name
                    (child_ele, env, "namespace");
        prefix = axiom_element_get_attribute_value_by_name
                                (child_ele, env, "prefix");
        prefix = axis2_strdup(env, prefix);
        local_name = axiom_element_get_text(child_ele, env, child);
        qname2str = axis2_strdup(env, local_name);
        qname2str = w2c_string_add_string( qname2str, "|", env);
        qname2str = w2c_string_add_string( qname2str, uri, env);

        while ( (child = axiom_children_iterator_next(it2, env )) != NULL )
        {
            if( AXIOM_NODE_GET_NODE_TYPE(child, env) == AXIOM_ELEMENT )
            {
                break; /* stop at the second valid child */
            }
        }
        /* child = AXIOM_NODE_GET_NEXT_SIBLING( child, env ); */
        child_ele = AXIOM_NODE_GET_DATA_ELEMENT( child, env);
        type_val = axiom_element_get_text( child_ele, env, child);
        type_val = axis2_strdup(env, type_val);
        if ( is_default )
        {
            uri = axis2_strdup(env,  uri);
            local_name = axis2_strdup(env, local_name);
            prefix = axis2_strdup(env, prefix);
            typemapper_impl-> default_qname = 
                axis2_qname_create( env, local_name, uri, prefix);
            typemapper_impl-> default_type = axis2_strdup(env, type_val);
        }
        axutil_hash_set( typemapper_impl-> qname2name, qname2str,
                        AXIS2_HASH_KEY_STRING, type_val );
        qname2str = axis2_strdup(env, qname2str);
        axutil_hash_set( typemapper_impl-> qname2prefix, qname2str,
                        AXIS2_HASH_KEY_STRING, prefix );
        qname2str = axis2_strdup(env, qname2str);
        axutil_hash_set( typemapper_impl-> primitive_map, qname2str,
                        AXIS2_HASH_KEY_STRING, (void*)1);
    }

    axiom_stax_builder_free(om_builder, env);
    axiom_xml_reader_cleanup();

   return &(typemapper_impl->typemapper);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
w2c_typemapper_free (w2c_typemapper_t *typemapper, 
                            const axutil_env_t *env)
{
    w2c_typemapper_impl_t *typemapper_impl = NULL;
 
    axis2_char_t *key = NULL;
    axutil_hash_index_t *hi = NULL;
    axutil_hash_t *h = NULL;
    axis2_char_t *value = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    typemapper_impl = W2C_INTF_TO_IMPL(typemapper);
    h = typemapper_impl-> qname2name;
    
    for (hi = axutil_hash_first(h, env) ;
                hi; hi = axutil_hash_next(env, hi))
    {
        axutil_hash_this(hi, (void*)&key, NULL, (void*)&value);
        if (value)
        {
            AXIS2_FREE ( env-> allocator, value);
        }
        if (key) AXIS2_FREE ( env-> allocator, key); 
    }
    axutil_hash_free( h , env);
    
    h = typemapper_impl-> qname2prefix;
    for (hi = axutil_hash_first(h, env) ;
                hi; hi = axutil_hash_next(env, hi))
    {
        axutil_hash_this(hi, (void*)&key, NULL, (void*)&value);
        if (value)
        {
            AXIS2_FREE ( env-> allocator, value);
        }
        if (key) AXIS2_FREE ( env-> allocator, key); 
    }
    axutil_hash_free( h , env);
    
    h = typemapper_impl-> primitive_map;
    for (hi = axutil_hash_first(h, env) ;
                hi; hi = axutil_hash_next(env, hi))
    {
        axutil_hash_this(hi, (void*)&key, NULL, (void*)&value);
        if (key) AXIS2_FREE ( env-> allocator, key); 
    }
    axutil_hash_free( h , env);

    if (typemapper_impl-> mapper_name)
    {
        AXIS2_FREE (env-> allocator, typemapper_impl-> mapper_name);
    }
    if (typemapper_impl-> default_type)
    {
        AXIS2_FREE (env-> allocator, typemapper_impl-> default_type);
    }
    if (typemapper_impl-> default_qname)
    {
        axis2_qname_free( typemapper_impl-> default_qname, env);
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
            const axutil_env_t *env,
            axis2_qname_t *qname)
{
    axis2_char_t *local= NULL;
    static int counter = 0;
    axis2_char_t counter_str[32];


	AXIS2_ENV_CHECK(env, NULL);
    

    local = axis2_strdup(env, "param");

    if (counter == W2C_TYPEMAPPER_UPPER_PARAM_LIMIT)
    {
        counter = 0;
    }
    sprintf(counter_str,"%d",counter++);
    local = w2c_string_add_string(local, counter_str, env );
    
    return local;    
}

axis2_char_t* AXIS2_CALL
w2c_typemapper_get_type_name(w2c_typemapper_t *typemapper,
            const axutil_env_t *env,
            axis2_qname_t *qname)
{
    w2c_typemapper_impl_t *typemapper_impl = NULL;
 
    axis2_char_t *str = NULL;
    axis2_char_t *key = NULL;
    axis2_char_t *value =NULL;
    axutil_hash_t *h = NULL;

	AXIS2_ENV_CHECK(env, NULL);
    
    typemapper_impl = W2C_INTF_TO_IMPL(typemapper);
    h = typemapper_impl-> qname2name;

    str = w2c_string_make_key_from_qname(qname, env);

    for ( key = str ; *key != ':' && *key != '\0' && *key != '|'; key ++ );
    if(  *key == '\0' || *key == '|')
    {
        key = str;
    }
    else
    {
        key++;
    }
    value = axutil_hash_get( h, key, AXIS2_HASH_KEY_STRING );
    AXIS2_FREE( env-> allocator, str); 
    return value;    
}


axis2_status_t AXIS2_CALL
w2c_typemapper_add_typemapping_name(w2c_typemapper_t *typemapper,
            const axutil_env_t *env,
            axis2_qname_t *qname,
            axis2_char_t *name)
{
    w2c_typemapper_impl_t *typemapper_impl = NULL;
    axis2_char_t *key= NULL;
    axis2_char_t *prefix = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    typemapper_impl = W2C_INTF_TO_IMPL(typemapper);

    prefix = axis2_qname_get_prefix( qname, env);
    key = w2c_string_make_key_from_qname( qname, env);
    key = axis2_strdup(env, key);
    name = axis2_strdup(env, name);

    axutil_hash_set( typemapper_impl-> qname2name, key,
                        AXIS2_HASH_KEY_STRING, name);
    if ( prefix && axis2_strlen( prefix) > 0 )
    {
        key = axis2_strdup(env, key);
        axutil_hash_set( typemapper_impl-> qname2prefix , key,
                        AXIS2_HASH_KEY_STRING, prefix);
    }
    return AXIS2_SUCCESS;
}
 
axis2_char_t* AXIS2_CALL
w2c_typemapper_get_default_mapping_name(w2c_typemapper_t *typemapper,
            const axutil_env_t *env)
{
    w2c_typemapper_impl_t *typemapper_impl = NULL;
 
    AXIS2_ENV_CHECK(env, NULL);
    
    typemapper_impl = W2C_INTF_TO_IMPL(typemapper);

    return typemapper_impl-> default_type;
}


axis2_qname_t* AXIS2_CALL
w2c_typemapper_get_default_qname(w2c_typemapper_t *typemapper,
            const axutil_env_t *env)
{
    w2c_typemapper_impl_t *typemapper_impl = NULL;
 
    AXIS2_ENV_CHECK(env, NULL);
    
    typemapper_impl = W2C_INTF_TO_IMPL(typemapper);

    return typemapper_impl-> default_qname;
}

axutil_hash_t* AXIS2_CALL
w2c_typemapper_get_all(w2c_typemapper_t *typemapper,
            const axutil_env_t *env)
{
    w2c_typemapper_impl_t *typemapper_impl = NULL;
 
    AXIS2_ENV_CHECK(env, NULL);
    
    typemapper_impl = W2C_INTF_TO_IMPL(typemapper);

    return typemapper_impl-> qname2name;
}

axis2_bool_t AXIS2_CALL
w2c_typemapper_is_primitive(w2c_typemapper_t *typemapper,
                    const axutil_env_t *env,
                    axis2_qname_t *qname)
{
    w2c_typemapper_impl_t *typemapper_impl = NULL;
    axis2_char_t *key = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);

    typemapper_impl = W2C_INTF_TO_IMPL(typemapper);

    key = w2c_string_make_key_from_qname(qname, env);
    return (axis2_bool_t)axutil_hash_get( typemapper_impl-> primitive_map, 
                key, AXIS2_HASH_KEY_STRING );
}
