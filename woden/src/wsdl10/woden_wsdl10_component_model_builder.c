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


#include "woden_wsdl10_component_model_builder.h"
#include <woden_wsdl10_desc_element.h>
#include <woden_include_element.h>
#include <woden_import_element.h>
#include <woden_wsdl10_desc.h>
#include <woden_types.h>
#include <woden_element_decl.h>
#include <woden_type_def.h>
#include <woden_configurable.h>
#include <woden_configurable_element.h>
#include <woden_interface.h>
#include <woden_interface_element.h>
#include <woden_interface_op.h>
#include <woden_interface_op_element.h>
#include <woden_wsdl10_interface_fault_ref.h>
#include <woden_wsdl10_interface_fault_ref_element.h>
#include <woden_wsdl10_interface_msg_ref.h>
#include <woden_wsdl10_msg_ref.h>
#include <woden_wsdl10_msg_ref_element.h>
#include <woden_wsdl10_part.h>
#include <woden_wsdl10_part_element.h>
#include <woden_wsdl10_interface_msg_ref_element.h>
#include <woden_binding.h>
#include <woden_binding_element.h>
#include <woden_wsdl10_binding_op.h>
#include <woden_wsdl10_binding_op_element.h>
#include <woden_binding_fault_ref.h>
#include <woden_binding_fault_ref_element.h>
#include <woden_wsdl10_binding_msg_ref.h>
#include <woden_wsdl10_binding_msg_ref_element.h>
#include <woden_wsdl10_ext_registry.h>
#include <woden_component_exts.h>
#include <woden_property.h>
#include <woden_property_element.h>

#include <xml_schema.h>
#include <xml_schema_obj_table.h>

#include "../wsdl20/woden_constants.h"
#include <axis2_array_list.h>
#include <axis2_uri.h>

typedef struct woden_wsdl10_component_model_builder_impl
{
      woden_wsdl10_component_model_builder_t builder;
      void *f_desc;
      axis2_array_list_t *f_schemas_done;
      axis2_array_list_t *f_msgs_done;
      axis2_array_list_t *f_interfaces_done;
      axis2_array_list_t *f_bindings_done;
      axis2_array_list_t *f_svcs_done;

}
woden_wsdl10_component_model_builder_impl_t;

#define AXIS2_INTF_TO_IMPL(builder) \
    ((woden_wsdl10_component_model_builder_impl_t *) builder)

axis2_status_t AXIS2_CALL
woden_wsdl10_component_model_builder_free(
      void *builder,
      const axis2_env_t *env);

static axis2_status_t
init_components(
      void *builder,
      const axis2_env_t *env,
      void *desc);

/*
 * Extract from the collections of in-scope schemas
 * the element declarations and type definitions.
 */
static axis2_status_t
build_elements_and_types(
      void *builder,
      const axis2_env_t *env,
      void *desc);

/*
 * Extract the element declarations from the given schema.
 */
static axis2_status_t
build_element_decls(
      void *builder,
      const axis2_env_t *env,
      void *schema_def,
      axis2_uri_t *type_system_uri);

/*
 * Extract the type definitions from the given schema.
 */
static axis2_status_t
build_type_defs(
      void *builder,
      const axis2_env_t *env,
      void *schema_def,
      axis2_uri_t *type_system_uri);

/* *******************************************************************************
 *  MESSAGE
 * ******************************************************************************/

/*
 * Initialize the Message component and its child components from the
 * Msg Element and its child elements.
 */
static axis2_status_t
build_msg_refs(
      void *builder,
      const axis2_env_t *env,
      axis2_array_list_t *msg_els);

static axis2_status_t
build_parts(
      void *builder,
      const axis2_env_t *env,
      void *msg);
/* *******************************************************************************
 *  INTERFACE
 * ******************************************************************************/

/*
 * Initialize the Interface component and its child components from the
 * Interface Element and its child elements.
 */
static axis2_status_t
build_interfaces(
      void *builder,
      const axis2_env_t *env,
      void *desc);

static axis2_status_t
build_interface_ops(
      void *builder,
      const axis2_env_t *env,
      void *interface);

static axis2_status_t
build_interface_fault_refs(
      void *builder,
      const axis2_env_t *env,
      void *op);

static axis2_status_t
build_interface_msg_refs(
      void *builder,
      const axis2_env_t *env,
      void *op);

/* *******************************************************************************
 *  BINDING
 * ******************************************************************************/

/*
 * Initialize the Binding component and its child components from the
 * Binding Element and its child elements.
 */
static axis2_status_t
build_bindings(
      void *builder,
      const axis2_env_t *env,
      void *desc);

static axis2_status_t
build_binding_ops(
      void *builder,
      const axis2_env_t *env,
      void *binding);

static axis2_status_t
build_binding_fault_refs(
      void *builder,
      const axis2_env_t *env,
      void *op);

static axis2_status_t
build_binding_msg_refs(
      void *builder,
      const axis2_env_t *env,
      void *op);

static axis2_status_t
build_binding_exts(
      void *builder,
      const axis2_env_t *env,
      void *binding);

static axis2_status_t
build_binding_op_exts(
      void *builder,
      const axis2_env_t *env,
      void *bind_op);

static axis2_status_t
build_binding_msg_ref_exts(
      void *builder,
      const axis2_env_t *env,
      void *bind_msg_ref);

static axis2_status_t
build_binding_fault_ref_exts(
      void *builder,
      const axis2_env_t *env,
      void *bind_fault_ref);

/*
 * This helper method factors out common code for creating Component Extensions registered
 * in the Extension Registry.
 */
static void *
create_component_exts(
      void *builder,
      const axis2_env_t *env,
      axis2_char_t *parent_class,
      void *parent_elem,
      axis2_uri_t *ext_ns);

/* *******************************************************************************
 *  SERVICE
 * ******************************************************************************/
static axis2_status_t
build_svcs(
      void *builder,
      const axis2_env_t *env,
      void *desc);

/* *******************************************************************************
 *  PROPERTY
 * ******************************************************************************/

static axis2_status_t
build_properties(
      void *builder,
      const axis2_env_t *env,
      axis2_array_list_t *prop_els,
      void *parent);

/************************** End of function prototypes ************************/

AXIS2_EXTERN woden_wsdl10_component_model_builder_t *AXIS2_CALL
woden_wsdl10_component_model_builder_create(
      const axis2_env_t *env,
      void *desc)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);

      builder_impl = (woden_wsdl10_component_model_builder_impl_t *)
            AXIS2_MALLOC(env->allocator,
			 sizeof(woden_wsdl10_component_model_builder_impl_t));

      if (NULL == builder_impl)
      {
	    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
	    return NULL;
      }
      builder_impl->f_desc = NULL;
      builder_impl->f_schemas_done = NULL;
      builder_impl->f_msgs_done = NULL;
      builder_impl->f_interfaces_done = NULL;
      builder_impl->f_bindings_done = NULL;
      builder_impl->f_svcs_done = NULL;

      builder_impl->builder.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_wsdl10_component_model_builder_ops_t));
      if (NULL == builder_impl->builder.ops)
      {
	    woden_wsdl10_component_model_builder_free(&(builder_impl->builder), env);
	    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
	    return NULL;
      }


      builder_impl->builder.ops->free =
	    woden_wsdl10_component_model_builder_free;

      builder_impl->f_desc = desc;
      init_components(&(builder_impl->builder), env, builder_impl->f_desc);
      return &(builder_impl->builder);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
woden_wsdl10_component_model_builder_free(
      void *builder,
      const axis2_env_t *env)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      if (builder_impl->builder.ops)
      {
	    AXIS2_FREE(env->allocator, builder_impl->
		       builder.ops);
	    builder_impl->builder.ops = NULL;
      }

      if (builder_impl)
      {
	    AXIS2_FREE(env->allocator, builder_impl);
	    builder_impl = NULL;
      }
      return AXIS2_SUCCESS;
}

static axis2_status_t
init_components(
      void *builder,
      const axis2_env_t *env,
      void *desc)
{
      axis2_array_list_t *includes = NULL;
      axis2_array_list_t *imports = NULL;
      int i = 0, size = 0;

      build_elements_and_types(builder, env, desc);
      build_interfaces(builder, env, desc);
      build_bindings(builder, env, desc);
      build_svcs(builder, env, desc);

      desc = woden_wsdl10_desc_to_desc_element(desc, env);
      includes = WODEN_WSDL10_DESC_ELEMENT_GET_INCLUDE_ELEMENTS(desc, env);
      if (includes)
      {
	    size = axis2_array_list_size(includes, env);
	    for (i = 0; i < size; i++)
	    {
		  void *incl_desc = NULL;
		  void *incl_el = NULL;

		  incl_el = axis2_array_list_get(includes, env, i);
		  incl_desc = WODEN_INCLUDE_ELEMENT_GET_DESC_ELEMENT(incl_el, env);
		  if (incl_desc)
		  {
			init_components(builder, env, incl_desc);
		  }
	    }
      }

      imports = WODEN_WSDL10_DESC_ELEMENT_GET_IMPORT_ELEMENTS(desc, env);
      if (imports)
      {
	    size = axis2_array_list_size(imports, env);
	    for (i = 0; i < size; i++)
	    {
		  void *imp_desc = NULL;
		  void *imp_el = NULL;

		  imp_el = axis2_array_list_get(imports, env, i);
		  imp_desc = WODEN_IMPORT_ELEMENT_GET_DESC_ELEMENT(imp_el, env);
		  if (imp_desc)
		  {
			init_components(builder, env, imp_desc);
		  }
	    }
      }

      return AXIS2_SUCCESS;
}

/* *******************************************************************************
 *  TYPES
 * ******************************************************************************/

/*
 * Extract from the collections of in-scope schemas
 * the element declarations and type definitions.
 */
static axis2_status_t
build_elements_and_types(
      void *builder,
      const axis2_env_t *env,
      void *desc)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      void *types = NULL;
      axis2_uri_t *type_system_uri = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, desc, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      desc = woden_wsdl10_desc_to_desc_element(desc, env);
      types = WODEN_WSDL10_DESC_ELEMENT_GET_TYPES_ELEMENT(desc, env);

      type_system_uri = axis2_uri_parse_string(env, WODEN_TYPE_XSD_2001);

      if (types)
      {
	    axis2_array_list_t *referenceable_schema_defs = NULL;
	    int i = 0, size = 0;

	    referenceable_schema_defs =
		  WODEN_TYPES_GET_REFERENCEABLE_SCHEMA_DEFS(types, env);
	    if (referenceable_schema_defs)
		  size = axis2_array_list_size(referenceable_schema_defs, env);
	    for (i = 0; i < size; i++)
	    {
		  void *schema_def = NULL;
		  int i = 0, size = 0;
		  axis2_bool_t new_schema = AXIS2_TRUE;

		  schema_def = axis2_array_list_get(referenceable_schema_defs, env, i);
		  if (builder_impl->f_schemas_done)
			size = axis2_array_list_size(builder_impl->f_schemas_done, env);
		  for (i = 0; i < size; i++)
		  {
			void *schema_done = NULL;

			schema_done = axis2_array_list_get(builder_impl->f_schemas_done,
							   env, i);
			/* TODO Check the logic here */
			if (schema_def == schema_done)
			{
			      new_schema = AXIS2_FALSE;
			      break;
			}
		  }
		  /* if builder_impl->f_schemas_done does not contain schema_def */
		  if (new_schema)
		  {
			build_element_decls(builder, env, schema_def, type_system_uri);
			build_type_defs(builder, env, schema_def, type_system_uri);
			if (!builder_impl->f_schemas_done)
			{
			      builder_impl->f_schemas_done = axis2_array_list_create(env, 0);
			      if (!builder_impl->f_schemas_done)
				    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
						    AXIS2_FAILURE);
			}
			axis2_array_list_add(builder_impl->f_schemas_done, env, schema_def);
		  }
	    }
      }
      return AXIS2_SUCCESS;
}

/*
 * Extract the element declarations from the given schema.
 */
static axis2_status_t
build_element_decls(
      void *builder,
      const axis2_env_t *env,
      void *schema_def,
      axis2_uri_t *type_system_uri)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      axis2_char_t *schema_tns = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, schema_def, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, type_system_uri, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      schema_tns = XML_SCHEMA_GET_TARGET_NAMESPACE(schema_def, env);
      if (schema_tns)
      {
	    xml_schema_obj_table_t *element_table = NULL;
	    axis2_array_list_t *qnames = NULL;
	    int i = 0, size = 0;

	    element_table = XML_SCHEMA_GET_ELEMENTS(schema_def, env);
	    qnames = XML_SCHEMA_OBJ_TABLE_GET_NAMES(element_table, env);
	    if (qnames)
		  size = axis2_array_list_size(qnames, env);
	    for (i = 0; i < size; i++)
	    {
		  axis2_qname_t *qname = NULL;
		  axis2_char_t *namespc = NULL;

		  qname = (axis2_qname_t *) axis2_array_list_get(qnames, env, i);
		  if (qname)
			namespc = AXIS2_QNAME_GET_URI(qname, env);
		  if (namespc && 0 == AXIS2_STRCMP(namespc, schema_tns))
		  {
			void *ed = NULL;

			ed = woden_element_decl_create(env);
			WODEN_ELEMENT_DECL_SET_QNAME(ed, env, qname);
			WODEN_ELEMENT_DECL_SET_SYSTEM(ed, env, type_system_uri);
			WODEN_ELEMENT_DECL_SET_CONTENT_MODEL(ed, env,
							     WODEN_API_APACHE_WS_XS);
			WODEN_ELEMENT_DECL_SET_CONTENT(ed, env,
						       XML_SCHEMA_OBJ_TABLE_GET_ITEM(element_table, env,
										     qname));
			WODEN_WSDL10_DESC_ADD_TO_ALL_ELEMENT_DECLS(builder_impl->f_desc,
								   env, ed);
		  }
	    }
      }
      return AXIS2_SUCCESS;
}

/*
 * Extract the type definitions from the given schema.
 */
static axis2_status_t
build_type_defs(
      void *builder,
      const axis2_env_t *env,
      void *schema_def,
      axis2_uri_t *type_system_uri)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      axis2_char_t *schema_tns = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, schema_def, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, type_system_uri, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      schema_tns = XML_SCHEMA_GET_TARGET_NAMESPACE(schema_def, env);
      if (schema_tns)
      {
	    xml_schema_obj_table_t *type_table = NULL;
	    axis2_array_list_t *qnames = NULL;
	    int i = 0, size = 0;

	    type_table = XML_SCHEMA_GET_SCHEMA_TYPES(schema_def, env);
	    qnames = XML_SCHEMA_OBJ_TABLE_GET_NAMES(type_table, env);
	    if (qnames)
		  size = axis2_array_list_size(qnames, env);
	    for (i = 0; i < size; i++)
	    {
		  axis2_qname_t *qname = NULL;
		  axis2_char_t *namespc = NULL;

		  qname = (axis2_qname_t *) axis2_array_list_get(qnames, env, i);
		  if (qname)
			namespc = AXIS2_QNAME_GET_URI(qname, env);
		  if (0 == AXIS2_STRCMP(namespc, schema_tns))
		  {
			void *td = NULL;

			td = woden_type_def_create(env);
			WODEN_TYPE_DEF_SET_QNAME(td, env, qname);
			WODEN_TYPE_DEF_SET_SYSTEM(td, env, type_system_uri);
			WODEN_TYPE_DEF_SET_CONTENT_MODEL(td, env,
							 WODEN_API_APACHE_WS_XS);
			WODEN_TYPE_DEF_SET_CONTENT(td, env,
						   XML_SCHEMA_OBJ_TABLE_GET_ITEM(type_table, env,
										 qname));
			WODEN_WSDL10_DESC_ADD_TO_ALL_TYPE_DEFS(builder_impl->f_desc,
							       env, td);
		  }
	    }
      }
      return AXIS2_SUCCESS;
}

/* *******************************************************************************
 *  MESSAGE
 * ******************************************************************************/

/*
 * Initialize the Message component and its child components from the
 * Msg Element and its child elements.
 */
static axis2_status_t
build_msg_refs(
      void *builder,
      const axis2_env_t *env,
      axis2_array_list_t *msg_els)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      int i = 0, size = 0;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, msg_els, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      size = axis2_array_list_size(msg_els, env);
      for (i = 0; i < size; i++)
      {
	    void *msg_impl = NULL;

	    msg_impl = axis2_array_list_get(msg_els, env, i);
	    if (!builder_impl->f_msgs_done || (AXIS2_TRUE !=
					       axis2_array_list_contains(builder_impl->f_msgs_done, env,
									 msg_impl)))
	    {
		  WODEN_WSDL10_DESC_ADD_TO_ALL_MSGS(builder_impl->f_desc,
						    env, msg_impl);
		  build_parts(builder, env, msg_impl);
		  if (!builder_impl->f_msgs_done)
		  {
			builder_impl->f_msgs_done = axis2_array_list_create(env, 0);
			if (!builder_impl->f_msgs_done)
			      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
					      AXIS2_FAILURE);
		  }
		  axis2_array_list_add(builder_impl->f_msgs_done, env, msg_impl);
	    }
      }
      return AXIS2_SUCCESS;
}

static axis2_status_t
build_parts(
      void *builder,
      const axis2_env_t *env,
      void *msg)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      void *part = NULL;
      axis2_qname_t *qname = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, msg, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      msg = woden_wsdl10_msg_ref_to_msg_ref_element(msg, env);
      part = WODEN_WSDL10_MSG_REF_ELEMENT_GET_PART(msg, env);

      part = woden_wsdl10_part_to_part_element(part, env);
      qname = WODEN_WSDL10_PART_ELEMENT_GET_ELEMENT_QNAME(part, env);
      if (qname)
      {
	    void *element_decl = WODEN_WSDL10_DESC_GET_ELEMENT_DECL(
		  builder_impl->f_desc, env, qname);
	    WODEN_WSDL10_PART_SET_ELEMENT_DECLARATION(part, env,
						      element_decl);
      }
      return AXIS2_SUCCESS;
}


/* *******************************************************************************
 *  INTERFACE
 * ******************************************************************************/

/*
 * Initialize the Interface component and its child components from the
 * Interface Element and its child elements.
 */
static axis2_status_t
build_interfaces(
      void *builder,
      const axis2_env_t *env,
      void *desc)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      axis2_array_list_t *interface_els = NULL;
      int i = 0, size = 0;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, desc, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      desc = woden_wsdl10_desc_to_desc_element(desc, env);
      interface_els = WODEN_WSDL10_DESC_ELEMENT_GET_INTERFACE_ELEMENTS(desc, env);
      if (interface_els)
      {
	    size = axis2_array_list_size(interface_els, env);
      }
      for (i = 0; i < size; i++)
      {
	    void *int_impl = NULL;

	    int_impl = axis2_array_list_get(interface_els, env, i);
	    if (!builder_impl->f_interfaces_done || AXIS2_TRUE !=
                axis2_array_list_contains(builder_impl->f_interfaces_done, env,
					  int_impl))
	    {
		  axis2_array_list_t *property_els = NULL;
		  void *configurable = NULL;

		  WODEN_WSDL10_DESC_ADD_TO_ALL_INTERFACES(builder_impl->f_desc,
							  env, int_impl);
		  configurable = WODEN_INTERFACE_GET_BASE_IMPL(int_impl, env);
		  configurable = woden_configurable_to_configurable_element(configurable,
									    env);
		  property_els = WODEN_CONFIGURABLE_ELEMENT_GET_PROPERTY_ELEMENTS(
                        configurable, env);
		  build_properties(builder, env, property_els, int_impl);
		  build_interface_ops(builder, env, int_impl);
		  if (!builder_impl->f_interfaces_done)
		  {
			builder_impl->f_interfaces_done = axis2_array_list_create(env, 0);
			if (!builder_impl->f_interfaces_done)
			      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
					      AXIS2_FAILURE);
		  }
		  axis2_array_list_add(builder_impl->f_interfaces_done, env, int_impl);
	    }
      }
      return AXIS2_SUCCESS;
}

static axis2_status_t
build_interface_ops(
      void *builder,
      const axis2_env_t *env,
      void *interface)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      axis2_array_list_t *ops = NULL;
      int i = 0, size = 0;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, interface, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      interface = woden_interface_to_interface_element(interface, env);
      ops = WODEN_INTERFACE_ELEMENT_GET_INTERFACE_OP_ELEMENTS(
	    interface, env);
      if (ops)
	    size = axis2_array_list_size(ops, env);
      for (i = 0; i < size; i++)
      {
	    void *op = NULL;
	    axis2_array_list_t *property_elements = NULL;
	    void *nested_confble = NULL;
	    void *confble = NULL;

	    op = axis2_array_list_get(ops, env, i);
	    nested_confble = WODEN_INTERFACE_OP_GET_BASE_IMPL(op, env);
	    confble = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(nested_confble, env);
	    confble = woden_configurable_to_configurable_element(confble, env);
	    property_elements = WODEN_CONFIGURABLE_ELEMENT_GET_PROPERTY_ELEMENTS(
		  confble, env);
	    build_properties(builder, env, property_elements, op);
	    build_interface_fault_refs(builder, env, op);
	    build_interface_msg_refs(builder, env, op);
      }
      return AXIS2_SUCCESS;
}

static axis2_status_t
build_interface_fault_refs(
      void *builder,
      const axis2_env_t *env,
      void *op)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      axis2_array_list_t *fault_refs = NULL;
      int i = 0, size = 0;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, op, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      op = woden_interface_op_to_interface_op_element(op, env);
      fault_refs = WODEN_INTERFACE_OP_ELEMENT_GET_INTERFACE_FAULT_REF_ELEMENTS(
	    op, env);
      if (fault_refs)
	    size = axis2_array_list_size(fault_refs, env);
      for (i = 0; i < size; i++)
      {
	    void *fault_ref = NULL;
	    axis2_array_list_t *property_elements = NULL;
	    axis2_qname_t *qname = NULL;

	    fault_ref = axis2_array_list_get(fault_refs, env, i);
	    fault_ref = woden_wsdl10_interface_fault_ref_to_configurable_element(fault_ref, env);
	    property_elements = WODEN_CONFIGURABLE_ELEMENT_GET_PROPERTY_ELEMENTS(
		  fault_ref, env);
	    build_properties(builder, env, property_elements, fault_ref);
	    fault_ref = woden_wsdl10_interface_fault_ref_to_interface_fault_ref_element(
		  fault_ref, env);
	    qname = WODEN_WSDL10_INTERFACE_FAULT_REF_ELEMENT_GET_MSG_QNAME(fault_ref, env);
	    if (qname)
	    {
		  axis2_array_list_t *msgs = NULL;

		  msgs = WODEN_WSDL10_DESC_GET_MSGS(builder_impl->f_desc, env);
		  build_msg_refs(builder, env, msgs);
	    }
      }
      return AXIS2_SUCCESS;
}

static axis2_status_t
build_interface_msg_refs(
      void *builder,
      const axis2_env_t *env,
      void *op)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      axis2_array_list_t *int_msg_refs = NULL;
      int i = 0, size = 0;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, op, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      op = woden_interface_op_to_interface_op_element(op, env);
      int_msg_refs = WODEN_INTERFACE_OP_ELEMENT_GET_INTERFACE_MSG_REF_ELEMENTS(
	    op, env);
      if (int_msg_refs)
	    size = axis2_array_list_size(int_msg_refs, env);
      for (i = 0; i < size; i++)
      {
	    void *int_msg_ref = NULL;
	    axis2_qname_t *qname = NULL;
	    int_msg_ref = axis2_array_list_get(int_msg_refs, env, i);
	    int_msg_ref = woden_wsdl10_interface_msg_ref_to_configurable_element(int_msg_ref, env);


	    int_msg_ref = woden_wsdl10_interface_msg_ref_to_interface_msg_ref_element(int_msg_ref, env);
	    qname = WODEN_WSDL10_INTERFACE_MSG_REF_ELEMENT_GET_MSG_QNAME(int_msg_ref, env);
	    if (qname)
	    {
		  axis2_array_list_t *msgs = NULL;

		  msgs = WODEN_WSDL10_DESC_GET_MSGS(builder_impl->f_desc, env);
		  build_msg_refs(builder, env, msgs);
	    }
      }
      return AXIS2_SUCCESS;
}

/* *******************************************************************************
 *  BINDING
 * ******************************************************************************/

/*
 * Initialize the Binding component and its child components from the
 * Binding Element and its child elements.
 */
static axis2_status_t
build_bindings(
      void *builder,
      const axis2_env_t *env,
      void *desc)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      axis2_array_list_t *binding_els = NULL;
      int i = 0, size = 0;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, desc, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      desc = woden_wsdl10_desc_to_desc_element(desc, env);
      binding_els = WODEN_WSDL10_DESC_ELEMENT_GET_BINDING_ELEMENTS(desc, env);
      if (binding_els)
	    size = axis2_array_list_size(binding_els, env);
      for (i = 0; i < size; i++)
      {
	    void *bind_impl = NULL;

	    bind_impl = axis2_array_list_get(binding_els, env, i);
	    if (builder_impl->f_bindings_done && AXIS2_TRUE !=
                axis2_array_list_contains(builder_impl->f_bindings_done,
					  env, bind_impl))
	    {
		  axis2_array_list_t *property_elements = NULL;

		  WODEN_WSDL10_DESC_ADD_TO_ALL_BINDINGS(builder_impl->f_desc,
							env, bind_impl);
		  bind_impl = woden_binding_to_configurable_element(bind_impl, env);
		  property_elements = WODEN_CONFIGURABLE_ELEMENT_GET_PROPERTY_ELEMENTS(
                        bind_impl, env);
		  build_properties(builder, env, property_elements, bind_impl);
		  build_binding_ops(builder, env, bind_impl);
		  build_binding_exts(builder, env, bind_impl);
		  if (!builder_impl->f_bindings_done)
		  {
			builder_impl->f_bindings_done = axis2_array_list_create(env, 0);
			if (!builder_impl->f_bindings_done)
			      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
					      AXIS2_FAILURE);
		  }
		  axis2_array_list_add(builder_impl->f_bindings_done, env, bind_impl);

	    }
      }
      return AXIS2_SUCCESS;
}

static axis2_status_t
build_binding_ops(
      void *builder,
      const axis2_env_t *env,
      void *binding)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      axis2_array_list_t *ops = NULL;
      int i = 0, size = 0;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, binding, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      binding = woden_binding_to_binding_element(binding, env);
      ops = WODEN_BINDING_ELEMENT_GET_BINDING_OP_ELEMENTS(
	    binding, env);
      if (ops)
	    size = axis2_array_list_size(ops, env);
      for (i = 0; i < size; i++)
      {
	    void *op = NULL;
	    axis2_array_list_t *property_elements = NULL;

	    op = axis2_array_list_get(ops, env, i);
	    op = woden_wsdl10_binding_op_to_configurable_element(op, env);
	    property_elements = WODEN_CONFIGURABLE_ELEMENT_GET_PROPERTY_ELEMENTS(
		  op, env);
	    build_properties(builder, env, property_elements, op);
	    build_binding_fault_refs(builder, env, op);
	    build_binding_msg_refs(builder, env, op);
	    build_binding_op_exts(builder, env, op);
      }
      return AXIS2_SUCCESS;
}

static axis2_status_t
build_binding_fault_refs(
      void *builder,
      const axis2_env_t *env,
      void *op)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      axis2_array_list_t *fault_refs = NULL;
      int i = 0, size = 0;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, op, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      op = woden_wsdl10_binding_op_to_binding_op_element(op, env);
      op = woden_wsdl10_binding_op_to_binding_op_element(op, env);
      fault_refs = WODEN_WSDL10_BINDING_OP_ELEMENT_GET_BINDING_FAULT_REF_ELEMENTS(
	    op, env);
      if (fault_refs)
	    size = axis2_array_list_size(fault_refs, env);
      for (i = 0; i < size; i++)
      {
	    void *fault_ref = NULL;

	    fault_ref = axis2_array_list_get(fault_refs, env, i);
	    build_binding_fault_ref_exts(builder, env, fault_ref);
      }
      return AXIS2_SUCCESS;
}

static axis2_status_t
build_binding_msg_refs(
      void *builder,
      const axis2_env_t *env,
      void *op)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      axis2_array_list_t *msgs = NULL;
      int i = 0, size = 0;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, op, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      op = woden_wsdl10_binding_op_to_binding_op_element(op, env);
      msgs = WODEN_WSDL10_BINDING_OP_ELEMENT_GET_BINDING_MSG_REF_ELEMENTS(
	    op, env);
      if (msgs)
	    size = axis2_array_list_size(msgs, env);
      for (i = 0; i < size; i++)
      {
	    void *msg = NULL;

	    msg = axis2_array_list_get(msgs, env, i);
	    build_binding_msg_ref_exts(builder, env, msg);
      }
      return AXIS2_SUCCESS;
}

static axis2_status_t
build_binding_exts(
      void *builder,
      const axis2_env_t *env,
      void *binding)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      woden_wsdl10_ext_registry_t *er = NULL;
      axis2_array_list_t *ext_namespcs = NULL;
      int i = 0, size = 0;
      axis2_uri_t *binding_type = NULL;
      axis2_char_t *binding_type_str = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, binding, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      builder_impl->f_desc = woden_wsdl10_desc_to_desc_element(builder_impl->f_desc, env);
      er = WODEN_WSDL10_DESC_ELEMENT_GET_EXT_REGISTRY(builder_impl->f_desc, env);
      ext_namespcs = WODEN_WSDL10_EXT_REGISTRY_QUERY_COMPONENT_EXT_NAMESPACES(er,
									      env, "binding");

      /*
       * First, create a Component Extensions object for each registered extension namespace 
       * used with this binding. 
       */
      size = axis2_array_list_size(ext_namespcs, env);
      for (i = 0; i < size; i++)
      {
	    axis2_uri_t *ext_ns = NULL;
	    axis2_bool_t temp1 = AXIS2_FALSE;
	    axis2_bool_t temp2 = AXIS2_FALSE;
	    void *wsdl_obj = NULL;
	    void *wsdl_el = NULL;
	    void *docble = NULL;
	    void *confble = NULL;

	    ext_ns = axis2_array_list_get(ext_namespcs, env, i);

	    confble = WODEN_BINDING_GET_BASE_IMPL(binding, env);
	    docble = WODEN_CONFIGURABLE_GET_BASE_IMPL(confble, env);
	    wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(docble, env);
	    wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
	    wsdl_el = woden_wsdl_element_to_attr_extensible(wsdl_el, env);
	    temp1 = WODEN_ATTR_EXTENSIBLE_HAS_EXT_ATTRS_FOR_NAMESPACE(
		  wsdl_el, env, ext_ns);
	    wsdl_el = woden_wsdl_element_to_element_extensible(wsdl_el, env);
	    temp2 = WODEN_ELEMENT_EXTENSIBLE_HAS_EXT_ELEMENTS_FOR_NAMESPACE(
		  wsdl_el, env, ext_ns);
	    if (AXIS2_TRUE == temp1 || AXIS2_TRUE == temp2)
	    {
		  void *comp_ext = NULL;

		  comp_ext = create_component_exts(builder, env, "binding",
						   binding, ext_ns);
		  binding = woden_binding_to_wsdl_obj(binding, env);
		  WODEN_WSDL_OBJ_SET_COMPONENT_EXTS(binding, env, ext_ns, comp_ext);
	    }
      }

      /*
       * Second, apply the rules from WSDL 2.0 Part 2 Adjuncts spec for default values for SOAP and HTTP 
       * extension properties to create ComponentExtension objects even if the related extension attributes
       * or elements do not exist in the WSDL. 
       * 
       * TODO chg this hardcoded behaviour so that any default rules for SOAP, HTTP or user-defined
       * extensions can be registered in some way and interpreted here at run time.
       */
      binding_type = WODEN_BINDING_GET_TYPE(binding, env);
      binding_type_str = AXIS2_URI_TO_STRING(binding_type, env, AXIS2_URI_UNP_OMITUSERINFO);
      if (0 == AXIS2_STRCMP(WODEN_URI_NS_SOAP, binding_type_str))
      {
	    void *sbe = NULL;
	    axis2_uri_t *uri_ns_soap = NULL;

	    /* If the binding type is SOAP, the {soap version} property defaults to "1.2" so if a
	     * a SOAP Binding Extensions object has not already been created, create one now to handle
	     * this default value.
	     */
	    binding = woden_binding_to_wsdl_component(binding, env);
	    uri_ns_soap = axis2_uri_parse_string(env, WODEN_URI_NS_SOAP);
	    if (NULL == WODEN_WSDL_COMPONENT_GET_COMPONENT_EXTS_FOR_NAMESPACE(binding,
									      env, uri_ns_soap))
	    {
		  void *comp_ext = NULL;

		  comp_ext = create_component_exts(builder, env, "binding",
						   binding, uri_ns_soap);
		  binding = woden_binding_to_wsdl_obj(binding, env);
		  WODEN_WSDL_OBJ_SET_COMPONENT_EXTS(binding, env,
						    uri_ns_soap, comp_ext);
	    }
	    sbe = WODEN_WSDL_COMPONENT_GET_COMPONENT_EXTS_FOR_NAMESPACE(binding,
									env, uri_ns_soap);
	    /* TODO Complete this */
      }
      return AXIS2_SUCCESS;
}

static axis2_status_t
build_binding_op_exts(
      void *builder,
      const axis2_env_t *env,
      void *bind_op)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      woden_wsdl10_ext_registry_t *er = NULL;
      axis2_array_list_t *ext_namespcs = NULL;
      int i = 0, size = 0;
      void *parent_element = NULL;
      axis2_uri_t *binding_type = NULL;
      axis2_char_t *binding_type_str = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, bind_op, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      builder_impl->f_desc = woden_wsdl10_desc_to_desc_element(builder_impl->f_desc, env);
      er = WODEN_WSDL10_DESC_ELEMENT_GET_EXT_REGISTRY(builder_impl->f_desc, env);
      ext_namespcs = WODEN_WSDL10_EXT_REGISTRY_QUERY_COMPONENT_EXT_NAMESPACES(er, env, "binding_op");
      if (ext_namespcs)
	    size = axis2_array_list_size(ext_namespcs, env);
      /*
       * First, create a Component Extensions object for each registered extension 
       * namespace used with this binding operation. 
       */
      for (i = 0; i < size; i++)
      {
	    axis2_uri_t *ext_ns = NULL;
	    axis2_bool_t temp1 = AXIS2_FALSE;
	    axis2_bool_t temp2 = AXIS2_FALSE;

	    ext_ns = axis2_array_list_get(ext_namespcs, env, i);
	    bind_op = woden_wsdl10_binding_op_to_attr_extensible(
		  bind_op, env);
	    temp1 = WODEN_ATTR_EXTENSIBLE_HAS_EXT_ATTRS_FOR_NAMESPACE(
		  bind_op, env, ext_ns);
	    bind_op = woden_wsdl10_binding_op_to_element_extensible(
		  bind_op, env);
	    temp2 = WODEN_ELEMENT_EXTENSIBLE_HAS_EXT_ELEMENTS_FOR_NAMESPACE(
		  bind_op, env, ext_ns);
	    if (AXIS2_TRUE == temp1 || AXIS2_TRUE == temp2)
	    {
		  void *comp_ext = NULL;

		  comp_ext = create_component_exts(builder, env, "binding_op",
						   bind_op, ext_ns);
		  bind_op = woden_wsdl10_binding_op_to_wsdl_obj(bind_op, env);
		  WODEN_WSDL_OBJ_SET_COMPONENT_EXTS(bind_op, env,
						    ext_ns, comp_ext);
	    }
      }
      /*
       * Second, apply the rules from WSDL 2.0 Part 2 Adjuncts spec for default values for SOAP and HTTP 
       * extension properties to create Component Extension objects even if the related extension attributes
       * or elements do not exist in the WSDL. 
       * 
       * TODO chg this hardcoded behaviour so that any default rules for SOAP, HTTP or user-defined
       * extensions can be registered in some way and interpreted here at run time.
       */
      bind_op = woden_wsdl10_binding_op_to_nested_element(bind_op, env);
      parent_element = WODEN_NESTED_ELEMENT_GET_PARENT_ELEMENT(bind_op, env);
      parent_element = woden_binding_to_binding_element(parent_element, env);
      binding_type = WODEN_BINDING_ELEMENT_GET_TYPE(parent_element, env);
      binding_type_str = AXIS2_URI_TO_STRING(binding_type, env, AXIS2_URI_UNP_OMITUSERINFO);
      if (0 == AXIS2_STRCMP(WODEN_URI_NS_SOAP, binding_type_str))
      {
	    axis2_uri_t *uri_ns_soap = axis2_uri_parse_string(env, WODEN_URI_NS_SOAP);
	    /*
	     * If the binding type is HTTP then the {http input serialization} and {http output serialization} 
	     * properties have default values defined by the HTTP Binding rules in the WSDL 2.0 Part 2 Adjuncts
	     * spec, the {http fault serialization} property has the default "application/xml" and the 
	     * {http query parameter separator default} property has the default "&". So if an 
	     * HTTP Binding Operation Extensions object has not already been created, create one now to handle 
	     * these default values.
	     */
	    bind_op = woden_wsdl10_binding_op_to_wsdl_component(bind_op, env);
	    if (NULL == WODEN_WSDL_COMPONENT_GET_COMPONENT_EXTS_FOR_NAMESPACE(bind_op,
									      env, uri_ns_soap))
	    {
		  /*void *comp_ext = NULL;

		  comp_ext = create_component_exts(builder, env, "binding_op", 
		  bind_op, WODEN_URI_NS_SOAP);
		  bind_op = woden_wsdl10_binding_op_to_wsdl_obj(bind_op, env);
		  WODEN_WSDL_OBJ_SET_COMPONENT_EXTS(bind_op, env, 
		  WODEN_URI_NS_SOAP, comp_ext);*/
	    }
      }
      return AXIS2_SUCCESS;
}


static axis2_status_t
build_binding_msg_ref_exts(
      void *builder,
      const axis2_env_t *env,
      void *bind_msg_ref)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      woden_wsdl10_ext_registry_t *er = NULL;
      axis2_array_list_t *ext_namespcs = NULL;
      int i = 0, size = 0;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, bind_msg_ref, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      builder_impl->f_desc = woden_wsdl10_desc_to_desc_element(builder_impl->f_desc, env);
      er = WODEN_WSDL10_DESC_ELEMENT_GET_EXT_REGISTRY(builder_impl->f_desc, env);
      ext_namespcs = WODEN_WSDL10_EXT_REGISTRY_QUERY_COMPONENT_EXT_NAMESPACES(er, env, "binding_msg_ref");
      if (ext_namespcs)
	    size = axis2_array_list_size(ext_namespcs, env);
      for (i = 0; i < size; i++)
      {
	    axis2_uri_t *ext_ns = NULL;
	    axis2_bool_t temp1 = AXIS2_FALSE;
	    axis2_bool_t temp2 = AXIS2_FALSE;

	    ext_ns = axis2_array_list_get(ext_namespcs, env, i);
	    bind_msg_ref = woden_wsdl10_binding_msg_ref_to_attr_extensible(
		  bind_msg_ref, env);
	    temp1 = WODEN_ATTR_EXTENSIBLE_HAS_EXT_ATTRS_FOR_NAMESPACE(
		  bind_msg_ref, env, ext_ns);
	    bind_msg_ref = woden_wsdl10_binding_msg_ref_to_element_extensible(
		  bind_msg_ref, env);
	    temp2 = WODEN_ELEMENT_EXTENSIBLE_HAS_EXT_ELEMENTS_FOR_NAMESPACE(
		  bind_msg_ref, env, ext_ns);
	    if (AXIS2_TRUE == temp1 || AXIS2_TRUE == temp2)
	    {
		  void *comp_ext = NULL;

		  comp_ext = create_component_exts(builder, env, "binding_msg_ref",
						   bind_msg_ref, ext_ns);
		  bind_msg_ref = woden_wsdl10_binding_msg_ref_to_wsdl_obj(bind_msg_ref, env);
		  WODEN_WSDL_OBJ_SET_COMPONENT_EXTS(bind_msg_ref, env,
						    ext_ns, comp_ext);
	    }
      }
      return AXIS2_SUCCESS;
}

static axis2_status_t
build_binding_fault_ref_exts(
      void *builder,
      const axis2_env_t *env,
      void *bind_fault_ref)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      woden_wsdl10_ext_registry_t *er = NULL;
      axis2_array_list_t *ext_namespcs = NULL;
      int i = 0, size = 0;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, bind_fault_ref, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      builder_impl->f_desc = woden_wsdl10_desc_to_desc_element(builder_impl->f_desc, env);
      er = WODEN_WSDL10_DESC_ELEMENT_GET_EXT_REGISTRY(builder_impl->f_desc, env);
      ext_namespcs = WODEN_WSDL10_EXT_REGISTRY_QUERY_COMPONENT_EXT_NAMESPACES(er, env, "binding_fault_ref");
      if (ext_namespcs)
	    size = axis2_array_list_size(ext_namespcs, env);
      for (i = 0; i < size; i++)
      {
	    axis2_uri_t *ext_ns = NULL;
	    axis2_bool_t temp1 = AXIS2_FALSE;
	    axis2_bool_t temp2 = AXIS2_FALSE;

	    ext_ns = axis2_array_list_get(ext_namespcs, env, i);
	    bind_fault_ref = woden_binding_fault_ref_to_attr_extensible(
		  bind_fault_ref, env);
	    temp1 = WODEN_ATTR_EXTENSIBLE_HAS_EXT_ATTRS_FOR_NAMESPACE(
		  bind_fault_ref, env, ext_ns);
	    bind_fault_ref = woden_binding_fault_ref_to_element_extensible(
		  bind_fault_ref, env);
	    temp2 = WODEN_ELEMENT_EXTENSIBLE_HAS_EXT_ELEMENTS_FOR_NAMESPACE(
		  bind_fault_ref, env, ext_ns);
	    if (AXIS2_TRUE == temp1 || AXIS2_TRUE == temp2)
	    {
		  void *comp_ext = NULL;

		  comp_ext = create_component_exts(builder, env, "binding_fault_ref",
						   bind_fault_ref, ext_ns);
		  bind_fault_ref = woden_binding_fault_ref_to_wsdl_obj(bind_fault_ref, env);
		  WODEN_WSDL_OBJ_SET_COMPONENT_EXTS(bind_fault_ref, env,
						    ext_ns, comp_ext);
	    }
      }
      return AXIS2_SUCCESS;
}

/*
 * This helper method factors out common code for creating Component Extensions registered
 * in the Extension Registry.
 */
static void *
create_component_exts(
      void *builder,
      const axis2_env_t *env,
      axis2_char_t *parent_class,
      void *parent_elem,
      axis2_uri_t *ext_ns)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      woden_wsdl10_ext_registry_t *er = NULL;
      void *comp_ext = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      AXIS2_PARAM_CHECK(env->error, parent_class, NULL);
      AXIS2_PARAM_CHECK(env->error, parent_elem, NULL);
      AXIS2_PARAM_CHECK(env->error, ext_ns, NULL);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      builder_impl->f_desc = woden_wsdl10_desc_to_desc_element(builder_impl->f_desc, env);
      er = WODEN_WSDL10_DESC_ELEMENT_GET_EXT_REGISTRY(builder_impl->f_desc, env);
      comp_ext = WODEN_WSDL10_EXT_REGISTRY_QUERY_COMPONENT_EXT(er, env,
							       parent_class, ext_ns);
      WODEN_COMPONENT_EXTS_INIT(comp_ext, env, parent_elem, ext_ns);

      return comp_ext;
}


/* *******************************************************************************
 *  SERVICE
 * ******************************************************************************/
static axis2_status_t
build_svcs(
      void *builder,
      const axis2_env_t *env,
      void *desc)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      axis2_array_list_t *svcs = NULL;
      int i = 0, size = 0;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, desc, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      desc = woden_wsdl10_desc_to_desc_element(desc, env);
      svcs = WODEN_WSDL10_DESC_ELEMENT_GET_SVC_ELEMENTS(desc, env);
      if (svcs)
	    size = axis2_array_list_size(svcs, env);
      for (i = 0; i < size; i++)
      {
	    void *svc = NULL;

	    svc = axis2_array_list_get(svcs, env, i);
	    if (builder_impl->f_svcs_done &&
                (AXIS2_TRUE != axis2_array_list_contains(builder_impl->f_svcs_done,
							 env, svc)))
	    {
		  WODEN_WSDL10_DESC_ADD_TO_ALL_SVCS(builder_impl->f_desc, env, svc);
		  if (!builder_impl->f_svcs_done)
		  {
			builder_impl->f_svcs_done = axis2_array_list_create(env, 0);
			if (!builder_impl->f_svcs_done)
			{
			      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY,
					      AXIS2_FAILURE);
			      return AXIS2_FAILURE;
			}
		  }
		  axis2_array_list_add(builder_impl->f_svcs_done, env, svc);
	    }
      }
      return AXIS2_SUCCESS;
}


/* *******************************************************************************
 *  PROPERTY
 * ******************************************************************************/

static axis2_status_t
build_properties(
      void *builder,
      const axis2_env_t *env,
      axis2_array_list_t *prop_els,
      void *parent)
{
      woden_wsdl10_component_model_builder_impl_t *builder_impl = NULL;
      int i = 0, size = 0;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, prop_els, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, parent, AXIS2_FAILURE);
      builder_impl = AXIS2_INTF_TO_IMPL(builder);

      size = axis2_array_list_size(prop_els, env);
      for (i = 0; i < size; i++)
      {
	    void *prop = NULL;
	    axis2_qname_t *qname = NULL;

	    prop = axis2_array_list_get(prop_els, env, i);
	    prop = woden_property_to_property_element(prop, env);
	    qname = WODEN_PROPERTY_ELEMENT_GET_CONSTRAINT_QNAME(prop, env);
	    if (qname)
	    {
		  void *value = NULL;

		  value = WODEN_WSDL10_DESC_GET_TYPE_DEF(builder_impl->f_desc, env, qname);
		  WODEN_PROPERTY_SET_VALUE_CONSTRAINT(prop, env, value);
	    }
      }
      return AXIS2_SUCCESS;
}

