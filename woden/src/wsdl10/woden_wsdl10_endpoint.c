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

#include <woden_wsdl_obj.h>
#include <woden_wsdl10_endpoint.h>
#include <woden_binding.h>
#include <woden_nested_configurable.h>
#include <woden_ext_element.h>
#include <woden_component_exts.h>
#include <woden_xml_attr.h>
#include <woden_documentation_element.h>

typedef struct woden_wsdl10_endpoint_impl woden_wsdl10_endpoint_impl_t;

/**
 * @brief Endpoint Struct Impl
 *   Axis2 Endpoint
 */
struct woden_wsdl10_endpoint_impl
{
      woden_wsdl10_endpoint_t endpoint;
      woden_obj_types_t obj_type;
      woden_nested_configurable_t *nested_configurable;
      axis2_hash_t *super;
      axis2_hash_t *methods;
      woden_nc_name_t *f_name;
      axis2_qname_t *f_binding_qname;
      void *f_binding;
      axis2_uri_t *f_address;
};

#define INTF_TO_IMPL(endpoint) ((woden_wsdl10_endpoint_impl_t *) endpoint)

axis2_status_t AXIS2_CALL
woden_wsdl10_endpoint_free(
      void *endpoint,
      const axutil_env_t *env);

axis2_hash_t *AXIS2_CALL
woden_wsdl10_endpoint_super_objs(
      void *endpoint,
      const axutil_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_wsdl10_endpoint_type(
      void *endpoint,
      const axutil_env_t *env);

woden_nested_configurable_t *AXIS2_CALL
woden_wsdl10_endpoint_get_base_impl(
      void *endpoint,
      const axutil_env_t *env);

/* ************************************************************
 *  Endpoint interface methods (the WSDL Component model)
 * ************************************************************/

woden_nc_name_t *AXIS2_CALL
woden_wsdl10_endpoint_get_name(
      void *endpoint,
      const axutil_env_t *env);

void *AXIS2_CALL
woden_wsdl10_endpoint_get_binding(
      void *endpoint,
      const axutil_env_t *env);

axis2_uri_t *AXIS2_CALL
woden_wsdl10_endpoint_get_address(
      void *endpoint,
      const axutil_env_t *env);
/* ************************************************************
 *  Endpoint Element interface methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
woden_wsdl10_endpoint_set_name(
      void *endpoint,
      const axutil_env_t *env,
      woden_nc_name_t *name);

axis2_status_t AXIS2_CALL
woden_wsdl10_endpoint_set_binding_qname(
      void *endpoint,
      const axutil_env_t *env,
      axis2_qname_t *binding_qname);

axis2_qname_t *AXIS2_CALL
woden_wsdl10_endpoint_get_binding_qname(
      void *endpoint,
      const axutil_env_t *env);

void *AXIS2_CALL
woden_wsdl10_endpoint_get_binding_element(
      void *endpoint,
      const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_wsdl10_endpoint_set_address(
      void *endpoint,
      const axutil_env_t *env,
      axis2_uri_t *uri);

/******************************************************************************
 *  Non-API implementation methods
 *****************************************************************************/

axis2_status_t AXIS2_CALL
woden_wsdl10_endpoint_set_binding_element(
      void *endpoint,
      const axutil_env_t *env,
      void *binding);

static woden_wsdl10_endpoint_t *
create(const axutil_env_t *env);

static axis2_status_t
woden_wsdl10_endpoint_free_ops(
      void *endpoint,
      const axutil_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_wsdl10_endpoint_t * AXIS2_CALL
woden_wsdl10_endpoint_to_endpoint_element(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!endpoint)
      {
	    endpoint = (void *) create (env);
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *) endpoint;
      }
      else
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *) endpoint;

      if (endpoint)
	    woden_wsdl10_endpoint_free_ops(endpoint, env);

      endpoint_impl->endpoint.base.endpoint_element.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_wsdl10_endpoint_element_ops_t));
      woden_wsdl10_endpoint_element_resolve_methods(&(endpoint_impl->endpoint.base.
						      endpoint_element), env, endpoint_impl->methods);
      return endpoint;
}

AXIS2_EXTERN woden_wsdl10_endpoint_t * AXIS2_CALL
woden_wsdl10_endpoint_to_nested_element(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!endpoint)
      {
	    endpoint = (void *) create (env);
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *) endpoint;
      }
      else
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *) endpoint;

      if (endpoint)
	    woden_wsdl10_endpoint_free_ops(endpoint, env);

      endpoint_impl->endpoint.base.endpoint_element.base.nested_element.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_nested_element_ops_t));
      woden_nested_element_resolve_methods(&(endpoint_impl->endpoint.base.
					     endpoint_element.base.nested_element), env, endpoint_impl->methods);
      return endpoint;
}

AXIS2_EXTERN woden_wsdl10_endpoint_t * AXIS2_CALL
woden_wsdl10_endpoint_to_documentable_element(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!endpoint)
      {
	    endpoint = (void *) create (env);
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *)endpoint;
      }
      else
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *) endpoint;

      if (endpoint)
	    woden_wsdl10_endpoint_free_ops(endpoint, env);

      endpoint_impl->endpoint.base.endpoint_element.base.documentable_element.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_documentable_element_ops_t));
      woden_documentable_element_resolve_methods(&(endpoint_impl->endpoint.base.
						   endpoint_element.base.documentable_element), env,
						 endpoint_impl->methods);
      return endpoint;
}

AXIS2_EXTERN woden_wsdl10_endpoint_t * AXIS2_CALL
woden_wsdl10_endpoint_to_nested_configurable(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!endpoint)
      {
	    endpoint = (void *) create (env);
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *)endpoint;
      }
      else
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *) endpoint;

      if (endpoint)
	    woden_wsdl10_endpoint_free_ops(endpoint, env);

      endpoint_impl->endpoint.base.nested_configurable.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_nested_configurable_ops_t));
      woden_nested_configurable_resolve_methods(&(endpoint_impl->endpoint.base.
						  nested_configurable), env, endpoint_impl->nested_configurable,
						endpoint_impl->methods);
      return endpoint;
}

AXIS2_EXTERN woden_wsdl10_endpoint_t * AXIS2_CALL
woden_wsdl10_endpoint_to_configurable(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;
      void *configurable = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!endpoint)
      {
	    endpoint = (void *) create (env);
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *)endpoint;
      }
      else
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *) endpoint;

      if (endpoint)
	    woden_wsdl10_endpoint_free_ops(endpoint, env);

      endpoint_impl->endpoint.base.nested_configurable.base.configurable.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_configurable_ops_t));
      configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
	    endpoint_impl->nested_configurable, env);
      woden_configurable_resolve_methods(&(endpoint_impl->endpoint.base.
					   nested_configurable.base.configurable), env, configurable,
					 endpoint_impl->methods);
      return endpoint;
}

AXIS2_EXTERN woden_wsdl10_endpoint_t * AXIS2_CALL
woden_wsdl10_endpoint_to_nested_component(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!endpoint)
      {
	    endpoint = (void *) create (env);
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *)endpoint;
      }
      else
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *) endpoint;

      if (endpoint)
	    woden_wsdl10_endpoint_free_ops(endpoint, env);

      endpoint_impl->endpoint.base.nested_configurable.base.nested_component.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_nested_component_ops_t));
      woden_nested_component_resolve_methods(&(endpoint_impl->endpoint.base.
					       nested_configurable.base.nested_component), env, endpoint_impl->methods);
      return endpoint;
}

AXIS2_EXTERN woden_wsdl10_endpoint_t * AXIS2_CALL
woden_wsdl10_endpoint_to_configurable_component(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!endpoint)
      {
	    endpoint = (void *) create (env);
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *)endpoint;
      }
      else
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *) endpoint;

      if (endpoint)
	    woden_wsdl10_endpoint_free_ops(endpoint, env);

      endpoint_impl->endpoint.base.configurable_component.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_configurable_component_ops_t));
      woden_configurable_component_resolve_methods(&(endpoint_impl->endpoint.base.
						     configurable_component), env, endpoint_impl->methods);
      return endpoint;
}

AXIS2_EXTERN woden_wsdl10_endpoint_t * AXIS2_CALL
woden_wsdl10_endpoint_to_wsdl_component(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);

      if (!endpoint)
      {
	    endpoint = (void *) create (env);
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *)endpoint;
      }
      else
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *) endpoint;

      if (endpoint)
	    woden_wsdl10_endpoint_free_ops(endpoint, env);

      endpoint_impl->endpoint.base.configurable_component.wsdl_component.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_wsdl_component_ops_t));
      woden_wsdl_component_resolve_methods(&(endpoint_impl->endpoint.base.
					     configurable_component.wsdl_component), env, endpoint_impl->methods);
      return endpoint;
}

AXIS2_EXTERN woden_wsdl10_endpoint_t * AXIS2_CALL
woden_wsdl10_endpoint_to_configurable_element(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!endpoint)
      {
	    endpoint = (void *) create (env);
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *)endpoint;
      }
      else
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *) endpoint;

      if (endpoint)
	    woden_wsdl10_endpoint_free_ops(endpoint, env);

      endpoint_impl->endpoint.base.endpoint_element.base.configurable_element.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_configurable_element_ops_t));
      woden_configurable_element_resolve_methods(&(endpoint_impl->endpoint.base.
						   endpoint_element.base.configurable_element), env, endpoint_impl->methods);
      return endpoint;
}

AXIS2_EXTERN woden_wsdl10_endpoint_t * AXIS2_CALL
woden_wsdl10_endpoint_to_documentable(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!endpoint)
      {
	    endpoint = (void *) create (env);
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *)endpoint;
      }
      else
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *) endpoint;

      if (endpoint)
	    woden_wsdl10_endpoint_free_ops(endpoint, env);

      endpoint_impl->endpoint.base.nested_configurable.base.
	    configurable.base.documentable.ops = AXIS2_MALLOC(env->allocator,
							      sizeof(woden_documentable_ops_t));
      woden_documentable_resolve_methods(&(endpoint_impl->endpoint.base.
					   nested_configurable.base.configurable.base.documentable), env, NULL,
					 endpoint_impl->methods);
      return endpoint;
}

AXIS2_EXTERN woden_wsdl10_endpoint_t * AXIS2_CALL
woden_wsdl10_endpoint_to_attr_extensible(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;
      void *configurable = NULL;
      void *documentable = NULL;
      void *wsdl_obj = NULL;
      void *wsdl_el = NULL;
      void *attr_extensible = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!endpoint)
      {
	    endpoint = (void *) create (env);
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *)endpoint;
      }
      else
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *) endpoint;

      if (endpoint)
	    woden_wsdl10_endpoint_free_ops(endpoint, env);

      endpoint_impl->endpoint.base.endpoint_element.base.documentable_element.
	    wsdl_element.base.attr_extensible.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_attr_extensible_ops_t));

      configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(endpoint_impl->
							     nested_configurable, env);
      documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(configurable, env);
      wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
      wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
      attr_extensible = WODEN_WSDL_ELEMENT_GET_ATTR_EXTENSIBLE(wsdl_el, env);
      woden_attr_extensible_resolve_methods(&(endpoint_impl->endpoint.base.
					      endpoint_element.base.documentable_element.wsdl_element.base.
					      attr_extensible), env, attr_extensible, endpoint_impl->methods);
      return endpoint;
}


AXIS2_EXTERN woden_wsdl10_endpoint_t * AXIS2_CALL
woden_wsdl10_endpoint_to_element_extensible(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;
      void *configurable = NULL;
      void *documentable = NULL;
      void *wsdl_obj = NULL;
      void *wsdl_el = NULL;
      void *element_extensible = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!endpoint)
      {
	    endpoint = (void *) create (env);
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *)endpoint;
      }
      else
	    endpoint_impl = (woden_wsdl10_endpoint_impl_t *) endpoint;

      if (endpoint)
	    woden_wsdl10_endpoint_free_ops(endpoint, env);

      endpoint_impl->endpoint.base.endpoint_element.base.documentable_element.
	    wsdl_element.base.element_extensible.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_element_extensible_ops_t));

      configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(endpoint_impl->
							     nested_configurable, env);
      documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(configurable, env);
      wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
      wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
      element_extensible = WODEN_WSDL_ELEMENT_GET_ELEMENT_EXTENSIBLE(wsdl_el, env);
      woden_element_extensible_resolve_methods(&(endpoint_impl->endpoint.base.
						 endpoint_element.base.documentable_element.wsdl_element.base.
						 element_extensible), env, element_extensible, endpoint_impl->methods);
      return endpoint;
}

/************************End of Woden C Internal Methods***********************/
static woden_wsdl10_endpoint_t *
create(const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      endpoint_impl = AXIS2_MALLOC(env->allocator,
				   sizeof(woden_wsdl10_endpoint_impl_t));

      endpoint_impl->obj_type = WODEN_WSDL10_ENDPOINT;
      endpoint_impl->super = NULL;
      endpoint_impl->methods = NULL;
      endpoint_impl->f_name = NULL;
      endpoint_impl->f_binding_qname = NULL;
      endpoint_impl->f_binding = NULL;
      endpoint_impl->f_address = NULL;

      endpoint_impl->endpoint.base.endpoint_element.ops = NULL;
      endpoint_impl->endpoint.base.endpoint_element.base.nested_element.ops = NULL;
      endpoint_impl->endpoint.base.endpoint_element.base.documentable_element.ops =
	    NULL;
      endpoint_impl->endpoint.base.nested_configurable.ops = NULL;
      endpoint_impl->endpoint.base.nested_configurable.base.configurable.ops = NULL;
      endpoint_impl->endpoint.base.nested_configurable.base.nested_component.ops = NULL;
      endpoint_impl->endpoint.base.configurable_component.ops = NULL;
      endpoint_impl->endpoint.base.configurable_component.wsdl_component.ops = NULL;
      endpoint_impl->endpoint.base.endpoint_element.base.
	    configurable_element.ops = NULL;
      endpoint_impl->endpoint.base.nested_configurable.base.configurable.base.
	    documentable.ops = NULL;
      endpoint_impl->endpoint.base.endpoint_element.base.
	    documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
      endpoint_impl->endpoint.base.endpoint_element.base.
	    documentable_element.wsdl_element.base.element_extensible.ops = NULL;

      endpoint_impl->endpoint.ops = AXIS2_MALLOC(env->allocator,
						 sizeof(woden_wsdl10_endpoint_ops_t));

      endpoint_impl->endpoint.ops->free = woden_wsdl10_endpoint_free;
      endpoint_impl->endpoint.ops->super_objs = woden_wsdl10_endpoint_super_objs;
      endpoint_impl->endpoint.ops->type = woden_wsdl10_endpoint_type;
      endpoint_impl->endpoint.ops->get_base_impl = woden_wsdl10_endpoint_get_base_impl;

      endpoint_impl->endpoint.ops->get_name = woden_wsdl10_endpoint_get_name;
      endpoint_impl->endpoint.ops->get_binding = woden_wsdl10_endpoint_get_binding;
      endpoint_impl->endpoint.ops->get_address =
	    woden_wsdl10_endpoint_get_address;
      endpoint_impl->endpoint.ops->set_binding_element =
	    woden_wsdl10_endpoint_set_binding_element;

      endpoint_impl->methods = axis2_hash_make(env);
      if (!endpoint_impl->methods)
      {
	    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
	    return NULL;
      }
      axis2_hash_set(endpoint_impl->methods, "free", AXIS2_HASH_KEY_STRING,
		     woden_wsdl10_endpoint_free);
      axis2_hash_set(endpoint_impl->methods, "super_objs",
		     AXIS2_HASH_KEY_STRING, woden_wsdl10_endpoint_super_objs);
      axis2_hash_set(endpoint_impl->methods, "type",
		     AXIS2_HASH_KEY_STRING, woden_wsdl10_endpoint_type);

      axis2_hash_set(endpoint_impl->methods, "get_name",
		     AXIS2_HASH_KEY_STRING,
		     woden_wsdl10_endpoint_get_name);
      axis2_hash_set(endpoint_impl->methods, "get_binding",
		     AXIS2_HASH_KEY_STRING,
		     woden_wsdl10_endpoint_get_binding);
      axis2_hash_set(endpoint_impl->methods, "get_address",
		     AXIS2_HASH_KEY_STRING,
		     woden_wsdl10_endpoint_get_address);
      axis2_hash_set(endpoint_impl->methods, "set_name",
		     AXIS2_HASH_KEY_STRING,
		     woden_wsdl10_endpoint_set_name);
      axis2_hash_set(endpoint_impl->methods, "set_binding_qname",
		     AXIS2_HASH_KEY_STRING,
		     woden_wsdl10_endpoint_set_binding_qname);
      axis2_hash_set(endpoint_impl->methods, "get_binding_qname",
		     AXIS2_HASH_KEY_STRING,
		     woden_wsdl10_endpoint_get_binding_qname);
      axis2_hash_set(endpoint_impl->methods, "get_binding_element",
		     AXIS2_HASH_KEY_STRING,
		     woden_wsdl10_endpoint_get_binding_element);
      axis2_hash_set(endpoint_impl->methods, "set_address",
		     AXIS2_HASH_KEY_STRING,
		     woden_wsdl10_endpoint_set_address);
      axis2_hash_set(endpoint_impl->methods, "set_binding_element",
		     AXIS2_HASH_KEY_STRING,
		     woden_wsdl10_endpoint_set_binding_element);

      return &(endpoint_impl->endpoint);
}

AXIS2_EXTERN woden_wsdl10_endpoint_t * AXIS2_CALL
woden_wsdl10_endpoint_create(const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;
      void *configurable = NULL;
      void *documentable = NULL;
      void *wsdl_obj = NULL;
      void *wsdl_el = NULL;
      void *element_extensible = NULL;
      void *attr_extensible = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      endpoint_impl = (woden_wsdl10_endpoint_impl_t *) create(env);

      endpoint_impl->nested_configurable = woden_nested_configurable_create(env);

      endpoint_impl->super = axis2_hash_make(env);
      if (!endpoint_impl->super)
      {
	    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
	    return NULL;
      }
      axis2_hash_set(endpoint_impl->super, "WODEN_WSDL10_ENDPOINT",
		     AXIS2_HASH_KEY_STRING, &(endpoint_impl->endpoint));
      axis2_hash_set(endpoint_impl->super, "WODEN_NESTED_CONFIGURABLE",
		     AXIS2_HASH_KEY_STRING, endpoint_impl->nested_configurable);
      configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
	    endpoint_impl->nested_configurable, env);
      axis2_hash_set(endpoint_impl->super, "WODEN_CONFIGURABLE",
		     AXIS2_HASH_KEY_STRING, configurable);

      documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(configurable, env);
      wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env);
      wsdl_el = WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env);
      element_extensible = WODEN_WSDL_ELEMENT_GET_ELEMENT_EXTENSIBLE(wsdl_el, env);
      attr_extensible = WODEN_WSDL_ELEMENT_GET_ATTR_EXTENSIBLE(wsdl_el, env);
      axis2_hash_set(endpoint_impl->super, "WODEN_DOCUMENTABLE",
		     AXIS2_HASH_KEY_STRING, documentable);
      axis2_hash_set(endpoint_impl->super, "WODEN_WSDL_OBJ",
		     AXIS2_HASH_KEY_STRING, wsdl_obj);
      axis2_hash_set(endpoint_impl->super, "WODEN_WSDL_ELEMENT",
		     AXIS2_HASH_KEY_STRING, wsdl_el);
      axis2_hash_set(endpoint_impl->super, "WODEN_ELEMENT_EXTENSIBLE",
		     AXIS2_HASH_KEY_STRING, element_extensible);
      axis2_hash_set(endpoint_impl->super, "WODEN_ATTR_EXTENSIBLE",
		     AXIS2_HASH_KEY_STRING, attr_extensible);


      return &(endpoint_impl->endpoint);
}

static axis2_status_t
woden_wsdl10_endpoint_free_ops(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      endpoint_impl = INTF_TO_IMPL(endpoint);

      if (endpoint_impl->endpoint.base.endpoint_element.ops)
      {
	    AXIS2_FREE(env->allocator, endpoint_impl->endpoint.base.
		       endpoint_element.ops);
	    endpoint_impl->endpoint.base.endpoint_element.ops = NULL;
      }

      if (endpoint_impl->endpoint.base.endpoint_element.base.nested_element.ops)
      {
	    AXIS2_FREE(env->allocator, endpoint_impl->endpoint.base.
		       endpoint_element.base.nested_element.ops);
	    endpoint_impl->endpoint.base.endpoint_element.base.nested_element.ops =
		  NULL;
      }

      if (endpoint_impl->endpoint.base.endpoint_element.base.documentable_element.ops)
      {
	    AXIS2_FREE(env->allocator, endpoint_impl->endpoint.base.
		       endpoint_element.base.documentable_element.ops);
	    endpoint_impl->endpoint.base.endpoint_element.base.documentable_element.ops =
		  NULL;
      }

      if (endpoint_impl->endpoint.base.nested_configurable.ops)
      {
	    AXIS2_FREE(env->allocator, endpoint_impl->endpoint.base.
		       nested_configurable.ops);
	    endpoint_impl->endpoint.base.nested_configurable.ops =
		  NULL;
      }

      if (endpoint_impl->endpoint.base.nested_configurable.base.configurable.ops)
      {
	    AXIS2_FREE(env->allocator, endpoint_impl->endpoint.base.
		       nested_configurable.base.configurable.ops);
	    endpoint_impl->endpoint.base.nested_configurable.base.configurable.ops =
		  NULL;
      }

      if (endpoint_impl->endpoint.base.nested_configurable.base.nested_component.ops)
      {
	    AXIS2_FREE(env->allocator, endpoint_impl->endpoint.base.
		       nested_configurable.base.nested_component.ops);
	    endpoint_impl->endpoint.base.nested_configurable.base.nested_component.ops = NULL;
      }

      if (endpoint_impl->endpoint.base.configurable_component.ops)
      {
	    AXIS2_FREE(env->allocator, endpoint_impl->endpoint.base.
		       configurable_component.ops);
	    endpoint_impl->endpoint.base.configurable_component.ops = NULL;
      }

      if (endpoint_impl->endpoint.base.configurable_component.wsdl_component.ops)
      {
	    AXIS2_FREE(env->allocator, endpoint_impl->endpoint.base.
		       configurable_component.wsdl_component.ops);
	    endpoint_impl->endpoint.base.configurable_component.wsdl_component.ops = NULL;
      }

      if (endpoint_impl->endpoint.base.endpoint_element.base.
	  configurable_element.ops)
      {
	    AXIS2_FREE(env->allocator, endpoint_impl->endpoint.base.
		       endpoint_element.base.configurable_element.ops);
	    endpoint_impl->endpoint.base.endpoint_element.base.
		  configurable_element.ops = NULL;
      }

      if (endpoint_impl->endpoint.base.nested_configurable.base.
	  configurable.base.documentable.ops)
      {
	    AXIS2_FREE(env->allocator, endpoint_impl->endpoint.base.
		       nested_configurable.base.configurable.base.documentable.ops);
	    endpoint_impl->endpoint.base.nested_configurable.base.
		  configurable.base.documentable.ops = NULL;
      }

      if (endpoint_impl->endpoint.base.endpoint_element.base.
	  documentable_element.wsdl_element.base.attr_extensible.ops)
      {
	    AXIS2_FREE(env->allocator, endpoint_impl->endpoint.base.
		       endpoint_element.base.documentable_element.wsdl_element.base.
		       attr_extensible.ops);
	    endpoint_impl->endpoint.base.endpoint_element.base.
		  documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
      }

      if (endpoint_impl->endpoint.base.endpoint_element.base.
	  documentable_element.wsdl_element.base.element_extensible.ops)
      {
	    AXIS2_FREE(env->allocator, endpoint_impl->endpoint.base.
		       endpoint_element.base.documentable_element.wsdl_element.base.
		       element_extensible.ops);
	    endpoint_impl->endpoint.base.endpoint_element.base.
		  documentable_element.wsdl_element.base.element_extensible.ops = NULL;
      }


      return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_wsdl10_endpoint_free(void *endpoint,
			   const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      endpoint_impl = INTF_TO_IMPL(endpoint);

      if (endpoint_impl->f_name)
      {
	    WODEN_NC_NAME_FREE(endpoint_impl->f_name, env);
	    endpoint_impl->f_name = NULL;
      }

      if (endpoint_impl->f_binding_qname)
      {
	    axis2_qname_free(endpoint_impl->f_binding_qname, env);
	    endpoint_impl->f_binding_qname = NULL;
      }

      if (endpoint_impl->f_binding)
      {
	    WODEN_BINDING_FREE(endpoint_impl->f_binding, env);
	    endpoint_impl->f_binding = NULL;
      }

      if (endpoint_impl->f_address)
      {
	    axis2_uri_free(endpoint_impl->f_address, env);
	    endpoint_impl->f_address = NULL;
      }

      /* TODO free f_parent */

      if (endpoint_impl->super)
      {
	    axis2_hash_free(endpoint_impl->super, env);
	    endpoint_impl->super = NULL;
      }

      if (endpoint_impl->methods)
      {
	    axis2_hash_free(endpoint_impl->methods, env);
	    endpoint_impl->methods = NULL;
      }

      if (endpoint_impl->nested_configurable)
      {
	    WODEN_NESTED_CONFIGURABLE_FREE(endpoint_impl->nested_configurable, env);
	    endpoint_impl->nested_configurable = NULL;
      }

      woden_wsdl10_endpoint_free_ops(endpoint, env);

      if ((&(endpoint_impl->endpoint))->ops)
      {
	    AXIS2_FREE(env->allocator, (&(endpoint_impl->endpoint))->ops);
	    (&(endpoint_impl->endpoint))->ops = NULL;
      }

      if (endpoint_impl)
      {
	    AXIS2_FREE(env->allocator, endpoint_impl);
	    endpoint_impl = NULL;
      }
      return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_wsdl10_endpoint_super_objs(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      endpoint_impl = INTF_TO_IMPL(endpoint);

      return endpoint_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_wsdl10_endpoint_type(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      endpoint_impl = INTF_TO_IMPL(endpoint);

      return endpoint_impl->obj_type;
}

woden_nested_configurable_t *AXIS2_CALL
woden_wsdl10_endpoint_get_base_impl(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      endpoint_impl = INTF_TO_IMPL(endpoint);

      return endpoint_impl->nested_configurable;
}

axis2_status_t AXIS2_CALL
woden_wsdl10_endpoint_resolve_methods(
      woden_wsdl10_endpoint_t *endpoint,
      const axutil_env_t *env,
      woden_wsdl10_endpoint_t *endpoint_impl,
      axis2_hash_t *methods)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl_l = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
      endpoint_impl_l = INTF_TO_IMPL(endpoint_impl);

      endpoint->ops->free = axis2_hash_get(methods, "free",
					   AXIS2_HASH_KEY_STRING);
      endpoint->ops->super_objs = axis2_hash_get(methods, "super_objs",
						 AXIS2_HASH_KEY_STRING);
      endpoint->ops->type = axis2_hash_get(methods, "type",
					   AXIS2_HASH_KEY_STRING);

      endpoint->ops->get_name = axis2_hash_get(methods,
					       "get_name", AXIS2_HASH_KEY_STRING);
      if (!endpoint->ops->get_name && endpoint_impl_l)
	    endpoint->ops->get_name =
		  endpoint_impl_l->endpoint.ops->get_name;

      endpoint->ops->get_binding = axis2_hash_get(methods,
						  "get_binding", AXIS2_HASH_KEY_STRING);
      if (!endpoint->ops->get_binding && endpoint_impl_l)
	    endpoint->ops->get_binding =
		  endpoint_impl_l->endpoint.ops->get_binding;

      endpoint->ops->get_address = axis2_hash_get(methods,
						  "get_address", AXIS2_HASH_KEY_STRING);
      if (!endpoint->ops->get_address && endpoint_impl_l)
	    endpoint->ops->get_address =
		  endpoint_impl_l->endpoint.ops->get_address;

      endpoint->ops->set_binding_element = axis2_hash_get(methods,
							  "set_binding_element", AXIS2_HASH_KEY_STRING);
      if (!endpoint->ops->set_binding_element && endpoint_impl_l)
	    endpoint->ops->set_binding_element =
		  endpoint_impl_l->endpoint.ops->set_binding_element;

      return AXIS2_SUCCESS;
}
/* ************************************************************
 *  Endpoint interface methods (the WSDL Component model)
 * ************************************************************/

woden_nc_name_t *AXIS2_CALL
woden_wsdl10_endpoint_get_name(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;
      axis2_hash_t *super = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      super = WODEN_WSDL10_ENDPOINT_SUPER_OBJS(endpoint, env);
      endpoint_impl = INTF_TO_IMPL(axis2_hash_get(super,
						  "WODEN_WSDL10_ENDPOINT", AXIS2_HASH_KEY_STRING));

      return endpoint_impl->f_name;
}

void *AXIS2_CALL
woden_wsdl10_endpoint_get_binding(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;
      axis2_hash_t *super = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      super = WODEN_WSDL10_ENDPOINT_SUPER_OBJS(endpoint, env);
      endpoint_impl = INTF_TO_IMPL(axis2_hash_get(super,
						  "WODEN_WSDL10_ENDPOINT", AXIS2_HASH_KEY_STRING));

      return endpoint_impl->f_binding;
}

axis2_uri_t *AXIS2_CALL
woden_wsdl10_endpoint_get_address(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;
      axis2_hash_t *super = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      super = WODEN_WSDL10_ENDPOINT_SUPER_OBJS(endpoint, env);
      endpoint_impl = INTF_TO_IMPL(axis2_hash_get(super,
						  "WODEN_WSDL10_ENDPOINT", AXIS2_HASH_KEY_STRING));

      return endpoint_impl->f_address;
}
/* ************************************************************
 *  Endpoint Element interface methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
woden_wsdl10_endpoint_set_name(
      void *endpoint,
      const axutil_env_t *env,
      woden_nc_name_t *name)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;
      axis2_hash_t *super = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      super = WODEN_WSDL10_ENDPOINT_SUPER_OBJS(endpoint, env);
      endpoint_impl = INTF_TO_IMPL(axis2_hash_get(super,
						  "WODEN_WSDL10_ENDPOINT", AXIS2_HASH_KEY_STRING));

      if (endpoint_impl->f_name)
      {
	    WODEN_NC_NAME_FREE(endpoint_impl->f_name, env);
      }
      endpoint_impl->f_name = name;
      return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_wsdl10_endpoint_set_binding_qname(
      void *endpoint,
      const axutil_env_t *env,
      axis2_qname_t *binding_qname)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;
      axis2_hash_t *super = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      super = WODEN_WSDL10_ENDPOINT_SUPER_OBJS(endpoint, env);
      endpoint_impl = INTF_TO_IMPL(axis2_hash_get(super,
						  "WODEN_WSDL10_ENDPOINT", AXIS2_HASH_KEY_STRING));

      if (endpoint_impl->f_binding_qname)
      {
	    axis2_qname_free(endpoint_impl->f_binding_qname, env);
      }

      endpoint_impl->f_binding_qname = axis2_qname_clone(binding_qname, env);
      return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
woden_wsdl10_endpoint_get_binding_qname(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;
      axis2_hash_t *super = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      super = WODEN_WSDL10_ENDPOINT_SUPER_OBJS(endpoint, env);
      endpoint_impl = INTF_TO_IMPL(axis2_hash_get(super,
						  "WODEN_WSDL10_ENDPOINT", AXIS2_HASH_KEY_STRING));

      return endpoint_impl->f_binding_qname;
}

void *AXIS2_CALL
woden_wsdl10_endpoint_get_binding_element(
      void *endpoint,
      const axutil_env_t *env)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;
      axis2_hash_t *super = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      super = WODEN_WSDL10_ENDPOINT_SUPER_OBJS(endpoint, env);
      endpoint_impl = INTF_TO_IMPL(axis2_hash_get(super,
						  "WODEN_WSDL10_ENDPOINT", AXIS2_HASH_KEY_STRING));

      return endpoint_impl->f_binding;
}

axis2_status_t AXIS2_CALL
woden_wsdl10_endpoint_set_address(
      void *endpoint,
      const axutil_env_t *env,
      axis2_uri_t *uri)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;
      axis2_hash_t *super = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      super = WODEN_WSDL10_ENDPOINT_SUPER_OBJS(endpoint, env);
      endpoint_impl = INTF_TO_IMPL(axis2_hash_get(super,
						  "WODEN_WSDL10_ENDPOINT", AXIS2_HASH_KEY_STRING));

      if (endpoint_impl->f_address)
      {
	    axis2_uri_free(endpoint_impl->f_address, env);
	    endpoint_impl->f_address = NULL;
      }

      endpoint_impl->f_address = axis2_uri_clone(uri, env);
      return AXIS2_SUCCESS;
}

/******************************************************************************
 *  Non-API implementation methods
 *****************************************************************************/

axis2_status_t AXIS2_CALL
woden_wsdl10_endpoint_set_binding_element(
      void *endpoint,
      const axutil_env_t *env,
      void *binding)
{
      woden_wsdl10_endpoint_impl_t *endpoint_impl = NULL;
      axis2_hash_t *super = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      super = WODEN_WSDL10_ENDPOINT_SUPER_OBJS(endpoint, env);
      endpoint_impl = INTF_TO_IMPL(axis2_hash_get(super,
						  "WODEN_WSDL10_ENDPOINT", AXIS2_HASH_KEY_STRING));

      if (endpoint_impl->f_binding)
      {
	    /* TODO */
      }

      endpoint_impl->f_binding = binding;
      return AXIS2_SUCCESS;
}


