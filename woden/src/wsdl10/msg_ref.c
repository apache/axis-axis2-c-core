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
#include <woden_wsdl10_msg_ref.h>
#include <woden_wsdl10_part.h>
#include <woden_wsdl10_part_element.h>
#include <woden_nested_configurable.h>
#include <woden_ext_element.h>
#include <woden_component_exts.h>
#include <woden_xml_attr.h>
#include <woden_documentation_element.h>
#include <xml_schema_element.h>

typedef struct woden_wsdl10_msg_ref_impl woden_wsdl10_msg_ref_impl_t;

/**
 * @brief Interface Operation Struct Impl
 *   Axis2 Interface Operation
 */
struct woden_wsdl10_msg_ref_impl
{
      woden_wsdl10_msg_ref_t msg_ref;
      woden_nested_configurable_t *nested_configurable;
      woden_obj_types_t obj_type;
      axis2_hash_t *super;
      axis2_hash_t *methods;

      axis2_qname_t *f_qname;
      void *f_part;
};

#define INTF_TO_IMPL(msg_ref) ((woden_wsdl10_msg_ref_impl_t *) msg_ref)

axis2_status_t AXIS2_CALL
woden_wsdl10_msg_ref_free(
      void *msg_ref,
      const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
woden_wsdl10_msg_ref_super_objs(
      void *msg_ref,
      const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_wsdl10_msg_ref_type(
      void *msg_ref,
      const axis2_env_t *env);

woden_nested_configurable_t *AXIS2_CALL
woden_wsdl10_msg_ref_get_base_impl(
      void *msg_ref,
      const axis2_env_t *env);

/*************************************************************
 *  Interface Operation  methods (the WSDL Component model)
 *************************************************************/

axis2_qname_t *AXIS2_CALL
woden_wsdl10_msg_ref_get_qname(
      void *msg_ref,
      const axis2_env_t *env);

void *AXIS2_CALL
woden_wsdl10_msg_ref_to_element(
      void *msg_ref,
      const axis2_env_t *env);

/* ************************************************************
 *  Interface Operation Element methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
woden_wsdl10_msg_ref_set_qname(
      void *msg_ref,
      const axis2_env_t *env,
      axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
woden_wsdl10_msg_ref_set_part(
      void *msg_ref,
      const axis2_env_t *env,
      void *part);

void *AXIS2_CALL
woden_wsdl10_msg_ref_get_part(
      void *msg_ref,
      const axis2_env_t *env);

static woden_wsdl10_msg_ref_t *
create(const axis2_env_t *env);

static axis2_status_t
woden_wsdl10_msg_ref_free_ops(
      void *msg_ref,
      const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_msg_ref_element(
      void *msg_ref,
      const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!msg_ref)
      {
	    msg_ref = (void *) create (env);
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;
      }
      else
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;

      if (msg_ref)
	    woden_wsdl10_msg_ref_free_ops(msg_ref, env);

      msg_ref_impl->msg_ref.base.msg_ref_element.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_wsdl10_msg_ref_element_ops_t));
      woden_wsdl10_msg_ref_element_resolve_methods(&(msg_ref_impl->msg_ref.base.
						     msg_ref_element), env, 
						   msg_ref_impl->methods);
      return msg_ref;
}

AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_nested_configurable(
      void *msg_ref,
      const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!msg_ref)
      {
	    msg_ref = (void *) create (env);
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;
      }
      else
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;

      if (msg_ref)
	    woden_wsdl10_msg_ref_free_ops(msg_ref, env);

      msg_ref_impl->msg_ref.base.nested_configurable.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_nested_configurable_ops_t));
      woden_nested_configurable_resolve_methods(&(msg_ref_impl->msg_ref.base.
						  nested_configurable), env, 
						msg_ref_impl->nested_configurable,
						msg_ref_impl->methods);
      return msg_ref;
}

AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_nested_component(
      void *msg_ref,
      const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!msg_ref)
      {
	    msg_ref = (void *) create (env);
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;
      }
      else
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;

      if (msg_ref)
	    woden_wsdl10_msg_ref_free_ops(msg_ref, env);

      msg_ref_impl->msg_ref.base.nested_configurable.base.nested_component.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_nested_component_ops_t));
      woden_nested_component_resolve_methods(&(msg_ref_impl->msg_ref.base.
					       nested_configurable.base.
					       nested_component), env,
					     msg_ref_impl->methods);
      return msg_ref;
}

AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_configurable(
      void *msg_ref,
      const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;
      void *configurable = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!msg_ref)
      {
	    msg_ref = (void *) create (env);
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;
      }
      else
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;

      if (msg_ref)
	    woden_wsdl10_msg_ref_free_ops(msg_ref, env);

      msg_ref_impl->msg_ref.base.nested_configurable.base.configurable.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_configurable_ops_t));
      configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
	    msg_ref_impl->nested_configurable, env);
      woden_configurable_resolve_methods(&(msg_ref_impl->msg_ref.base.
					   nested_configurable.base.configurable),
					 env, configurable, msg_ref_impl->methods);
      return msg_ref;
}

AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_nested_element(
      void *msg_ref,
      const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!msg_ref)
      {
	    msg_ref = (void *) create (env);
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;
      }
      else
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;

      if (msg_ref)
	    woden_wsdl10_msg_ref_free_ops(msg_ref, env);

      msg_ref_impl->msg_ref.base.msg_ref_element.base.nested_element.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_nested_element_ops_t));
      woden_nested_element_resolve_methods(&(msg_ref_impl->msg_ref.base.
					     msg_ref_element.base.nested_element),
					   env, msg_ref_impl->methods);
      return msg_ref;
}


AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_configurable_element(
      void *msg_ref,
      const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!msg_ref)
      {
	    msg_ref = (void *) create (env);
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;
      }
      else
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;

      if (msg_ref)
	    woden_wsdl10_msg_ref_free_ops(msg_ref, env);

      msg_ref_impl->msg_ref.base.msg_ref_element.base.configurable_element.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_configurable_element_ops_t));
      woden_configurable_element_resolve_methods(&(msg_ref_impl->msg_ref.base.
						   msg_ref_element.base.
						   configurable_element), env,
						 msg_ref_impl->methods);
      return msg_ref;
}

AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_documentable_element(
      void *msg_ref,
      const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!msg_ref)
      {
	    msg_ref = (void *) create (env);
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;
      }
      else
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;

      if (msg_ref)
	    woden_wsdl10_msg_ref_free_ops(msg_ref, env);

      msg_ref_impl->msg_ref.base.msg_ref_element.base.documentable_element.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_documentable_element_ops_t));
      woden_documentable_element_resolve_methods(&(msg_ref_impl->msg_ref.base.
						   msg_ref_element.base.
						   documentable_element), env,
						 msg_ref_impl->methods);
      return msg_ref;
}

AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_documentable(
      void *msg_ref,
      const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!msg_ref)
      {
	    msg_ref = (void *) create (env);
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;
      }
      else
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;

      if (msg_ref)
	    woden_wsdl10_msg_ref_free_ops(msg_ref, env);

      msg_ref_impl->msg_ref.base.nested_configurable.base.
	    configurable.base.documentable.ops = AXIS2_MALLOC
	    (env->allocator,
	     sizeof(woden_documentable_ops_t));

      woden_documentable_resolve_methods(
	    &(msg_ref_impl->msg_ref.base.
	      nested_configurable.base.configurable.base.documentable), env, NULL,
	    msg_ref_impl->methods);

      return msg_ref;
}

AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_attr_extensible(
      void *msg_ref,
      const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!msg_ref)
      {
	    msg_ref = (void *) create (env);
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;
      }
      else
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;

      if (msg_ref)
	    woden_wsdl10_msg_ref_free_ops(msg_ref, env);

      msg_ref_impl->msg_ref.base.msg_ref_element.base.documentable_element.
	    wsdl_element.base.attr_extensible.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_attr_extensible_ops_t));
      woden_attr_extensible_resolve_methods(
	    &(msg_ref_impl->msg_ref.base.
	      msg_ref_element.base.documentable_element.wsdl_element.base.
	      attr_extensible), env, NULL, msg_ref_impl->methods);

      return msg_ref;
}


AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_element_extensible(
      void *msg_ref,
      const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      if (!msg_ref)
      {
	    msg_ref = (void *) create (env);
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;
      }
      else
	    msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) msg_ref;

      if (msg_ref)
	    woden_wsdl10_msg_ref_free_ops(msg_ref, env);

      msg_ref_impl->msg_ref.base.msg_ref_element.base.documentable_element.
	    wsdl_element.base.element_extensible.ops =
	    AXIS2_MALLOC(env->allocator,
			 sizeof(woden_element_extensible_ops_t));
      woden_element_extensible_resolve_methods(
	    &(msg_ref_impl->msg_ref.base.
	      msg_ref_element.base.documentable_element.wsdl_element.base.
	      element_extensible), env, NULL, msg_ref_impl->methods);

      return msg_ref;
}


/************************End of Woden C Internal Methods***********************/
static woden_wsdl10_msg_ref_t *
create(const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      msg_ref_impl = AXIS2_MALLOC(env->allocator,
				  sizeof(woden_wsdl10_msg_ref_impl_t));

      msg_ref_impl->obj_type = WODEN_WSDL10_MSG_REF;
      msg_ref_impl->super = NULL;
      msg_ref_impl->methods = NULL;
      msg_ref_impl->f_qname = NULL;
      msg_ref_impl->f_part = NULL;

      msg_ref_impl->msg_ref.base.msg_ref_element.ops = NULL;
      msg_ref_impl->msg_ref.base.nested_configurable.ops = NULL;
      msg_ref_impl->msg_ref.base.nested_configurable.base.
	    nested_component.ops = NULL;
      msg_ref_impl->msg_ref.base.nested_configurable.base.configurable.ops =
	    NULL;
      msg_ref_impl->msg_ref.base.msg_ref_element.base.
	    nested_element.ops = NULL;
      msg_ref_impl->msg_ref.base.msg_ref_element.base.
	    configurable_element.ops = NULL;
      msg_ref_impl->msg_ref.base.msg_ref_element.base.
	    documentable_element.ops = NULL;
      msg_ref_impl->msg_ref.base.nested_configurable.base.configurable.base.
	    documentable.ops = NULL;
      msg_ref_impl->msg_ref.base.msg_ref_element.base.
	    documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
      msg_ref_impl->msg_ref.base.msg_ref_element.base.
	    documentable_element.wsdl_element.base.element_extensible.ops = NULL;

      msg_ref_impl->msg_ref.ops = AXIS2_MALLOC(env->allocator,
					       sizeof(woden_wsdl10_msg_ref_ops_t));

      msg_ref_impl->msg_ref.ops->free = woden_wsdl10_msg_ref_free;
      msg_ref_impl->msg_ref.ops->super_objs = woden_wsdl10_msg_ref_super_objs;
      msg_ref_impl->msg_ref.ops->type = woden_wsdl10_msg_ref_type;
      msg_ref_impl->msg_ref.ops->get_base_impl = woden_wsdl10_msg_ref_get_base_impl;

      msg_ref_impl->msg_ref.ops->get_qname =
	    woden_wsdl10_msg_ref_get_qname;
      msg_ref_impl->msg_ref.ops->to_element =
	    woden_wsdl10_msg_ref_to_element;

      msg_ref_impl->methods = axis2_hash_make(env);
      if (!msg_ref_impl->methods)
      {
	    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
	    return NULL;
      }
      axis2_hash_set(msg_ref_impl->methods, "free", AXIS2_HASH_KEY_STRING,
		     woden_wsdl10_msg_ref_free);
      axis2_hash_set(msg_ref_impl->methods, "super_objs",
		     AXIS2_HASH_KEY_STRING, woden_wsdl10_msg_ref_super_objs);
      axis2_hash_set(msg_ref_impl->methods, "type",
		     AXIS2_HASH_KEY_STRING, woden_wsdl10_msg_ref_type);

      axis2_hash_set(msg_ref_impl->methods, "get_qname",
		     AXIS2_HASH_KEY_STRING,
		     woden_wsdl10_msg_ref_get_qname);
      axis2_hash_set(msg_ref_impl->methods, "to_element",
		     AXIS2_HASH_KEY_STRING,
		     woden_wsdl10_msg_ref_to_element);
      axis2_hash_set(msg_ref_impl->methods, "set_qname",
		     AXIS2_HASH_KEY_STRING,
		     woden_wsdl10_msg_ref_set_qname);
      axis2_hash_set(msg_ref_impl->methods, "set_part",
		     AXIS2_HASH_KEY_STRING,
		     woden_wsdl10_msg_ref_set_part);
      axis2_hash_set(msg_ref_impl->methods, "get_part",
		     AXIS2_HASH_KEY_STRING,
		     woden_wsdl10_msg_ref_get_part);

      return &(msg_ref_impl->msg_ref);
}

AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_create(const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;
      void *configurable = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      msg_ref_impl = (woden_wsdl10_msg_ref_impl_t *) create(env);

      msg_ref_impl->nested_configurable = woden_nested_configurable_create(env);

      msg_ref_impl->super = axis2_hash_make(env);
      if (!msg_ref_impl->super)
      {
	    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
	    return NULL;
      }
      axis2_hash_set(msg_ref_impl->super, "WODEN_WSDL10_MSG_REF",
		     AXIS2_HASH_KEY_STRING,
		     &(msg_ref_impl->msg_ref));
      axis2_hash_set(msg_ref_impl->super, "WODEN_NESTED_CONFIGURABLE",
		     AXIS2_HASH_KEY_STRING,
		     msg_ref_impl->nested_configurable);
      configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
	    msg_ref_impl->nested_configurable, env);
      axis2_hash_set(msg_ref_impl->super, "WODEN_CONFIGURABLE",
		     AXIS2_HASH_KEY_STRING, configurable);

      return &(msg_ref_impl->msg_ref);
}

static axis2_status_t
woden_wsdl10_msg_ref_free_ops(
      void *msg_ref,
      const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      msg_ref_impl = INTF_TO_IMPL(msg_ref);

      if (msg_ref_impl->msg_ref.base.msg_ref_element.ops)
      {
	    AXIS2_FREE(env->allocator, msg_ref_impl->msg_ref.base.
		       msg_ref_element.ops);
	    msg_ref_impl->msg_ref.base.msg_ref_element.ops = NULL;
      }

      if (msg_ref_impl->msg_ref.base.nested_configurable.ops)
      {
	    AXIS2_FREE(env->allocator, msg_ref_impl->msg_ref.base.
		       nested_configurable.ops);
	    msg_ref_impl->msg_ref.base.nested_configurable.ops =
		  NULL;
      }

      if (msg_ref_impl->msg_ref.base.nested_configurable.base.
	  nested_component.ops)
      {
	    AXIS2_FREE(env->allocator, msg_ref_impl->msg_ref.base.
		       nested_configurable.base.nested_component.ops);
	    msg_ref_impl->msg_ref.base.nested_configurable.base.
		  nested_component.ops = NULL;
      }

      if (msg_ref_impl->msg_ref.base.nested_configurable.base.
	  configurable.ops)
      {
	    AXIS2_FREE(env->allocator, msg_ref_impl->msg_ref.base.
		       nested_configurable.base.configurable.ops);
	    msg_ref_impl->msg_ref.base.nested_configurable.base.
		  configurable.ops = NULL;
      }

      if (msg_ref_impl->msg_ref.base.msg_ref_element.base.
	  nested_element.ops)
      {
	    AXIS2_FREE(env->allocator, msg_ref_impl->msg_ref.base.
		       msg_ref_element.base.nested_element.ops);
	    msg_ref_impl->msg_ref.base.msg_ref_element.base.
		  nested_element.ops = NULL;
      }

      if (msg_ref_impl->msg_ref.base.msg_ref_element.base.
	  configurable_element.ops)
      {
	    AXIS2_FREE(env->allocator, msg_ref_impl->msg_ref.base.
		       msg_ref_element.base.configurable_element.ops);
	    msg_ref_impl->msg_ref.base.msg_ref_element.base.
		  configurable_element.ops = NULL;
      }

      if (msg_ref_impl->msg_ref.base.msg_ref_element.base.
	  documentable_element.ops)
      {
	    AXIS2_FREE(env->allocator, msg_ref_impl->msg_ref.base.
		       msg_ref_element.base.documentable_element.ops);
	    msg_ref_impl->msg_ref.base.msg_ref_element.base.
		  documentable_element.ops = NULL;
      }

      if (msg_ref_impl->msg_ref.base.nested_configurable.base.
	  configurable.base.documentable.ops)
      {
	    AXIS2_FREE(env->allocator, msg_ref_impl->msg_ref.base.
		       nested_configurable.base.configurable.base.documentable.ops);
	    msg_ref_impl->msg_ref.base.nested_configurable.base.
		  configurable.base.documentable.ops = NULL;
      }

      if (msg_ref_impl->msg_ref.base.msg_ref_element.base.
	  documentable_element.wsdl_element.base.attr_extensible.ops)
      {
	    AXIS2_FREE(env->allocator, msg_ref_impl->msg_ref.base.
		       msg_ref_element.base.documentable_element.wsdl_element.base.
		       attr_extensible.ops);
	    msg_ref_impl->msg_ref.base.msg_ref_element.base.
		  documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
      }

      if (msg_ref_impl->msg_ref.base.msg_ref_element.base.
	  documentable_element.wsdl_element.base.element_extensible.ops)
      {
	    AXIS2_FREE(env->allocator, msg_ref_impl->msg_ref.base.
		       msg_ref_element.base.documentable_element.wsdl_element.base.
		       element_extensible.ops);
	    msg_ref_impl->msg_ref.base.msg_ref_element.base.
		  documentable_element.wsdl_element.base.element_extensible.ops = NULL;
      }


      return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_wsdl10_msg_ref_free(
      void *msg_ref,
      const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      msg_ref_impl = INTF_TO_IMPL(msg_ref);

      if (msg_ref_impl->f_qname)
      {
	    AXIS2_QNAME_FREE(msg_ref_impl->f_qname, env);
	    msg_ref_impl->f_qname = NULL;
      }

      if (msg_ref_impl->f_part)
      {
	    WODEN_WSDL10_PART_FREE(msg_ref_impl->f_part, env);
	    msg_ref_impl->f_part = NULL;
      }

      if (msg_ref_impl->super)
      {
	    axis2_hash_free(msg_ref_impl->super, env);
	    msg_ref_impl->super = NULL;
      }

      if (msg_ref_impl->methods)
      {
	    axis2_hash_free(msg_ref_impl->methods, env);
	    msg_ref_impl->methods = NULL;
      }

      if (msg_ref_impl->nested_configurable)
      {
	    WODEN_NESTED_CONFIGURABLE_FREE(msg_ref_impl->
					   nested_configurable, env);
	    msg_ref_impl->nested_configurable = NULL;
      }

      if (msg_ref)
	    woden_wsdl10_msg_ref_free_ops(msg_ref, env);

      if ((&(msg_ref_impl->msg_ref))->ops)
      {
	    AXIS2_FREE(env->allocator, (&(msg_ref_impl->msg_ref))->ops);
	    (&(msg_ref_impl->msg_ref))->ops = NULL;
      }

      if (msg_ref_impl)
      {
	    AXIS2_FREE(env->allocator, msg_ref_impl);
	    msg_ref_impl = NULL;
      }
      return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_wsdl10_msg_ref_super_objs(
      void *msg_ref,
      const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      msg_ref_impl = INTF_TO_IMPL(msg_ref);

      return msg_ref_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_wsdl10_msg_ref_type(
      void *msg_ref,
      const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      msg_ref_impl = INTF_TO_IMPL(msg_ref);

      return msg_ref_impl->obj_type;
}

woden_nested_configurable_t *AXIS2_CALL
woden_wsdl10_msg_ref_get_base_impl(
      void *msg_ref,
      const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      msg_ref_impl = INTF_TO_IMPL(msg_ref);

      return msg_ref_impl->nested_configurable;
}

axis2_status_t AXIS2_CALL
woden_wsdl10_msg_ref_resolve_methods(
      woden_wsdl10_msg_ref_t *msg_ref,
      const axis2_env_t *env,
      woden_wsdl10_msg_ref_t *msg_ref_impl,
      axis2_hash_t *methods)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl_l = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
      msg_ref_impl_l = INTF_TO_IMPL(msg_ref_impl);

      msg_ref->ops->free = axis2_hash_get(methods, "free",
					  AXIS2_HASH_KEY_STRING);
      msg_ref->ops->super_objs = axis2_hash_get(methods, "super_objs",
						AXIS2_HASH_KEY_STRING);
      msg_ref->ops->type = axis2_hash_get(methods, "type",
					  AXIS2_HASH_KEY_STRING);

      msg_ref->ops->get_qname = axis2_hash_get(methods,
					       "get_qname", AXIS2_HASH_KEY_STRING);
      if (!msg_ref->ops->get_qname && msg_ref_impl_l)
	    msg_ref->ops->get_qname =
		  msg_ref_impl_l->msg_ref.ops->get_qname;

      msg_ref->ops->to_element = axis2_hash_get(methods,
						"to_element", AXIS2_HASH_KEY_STRING);
      if (!msg_ref->ops->to_element && msg_ref_impl_l)
	    msg_ref->ops->to_element =
		  msg_ref_impl_l->msg_ref.ops->to_element;

      return AXIS2_SUCCESS;
}

/* ************************************************************
 *  Interface Operation  methods (the WSDL Component model)
 * ************************************************************/

axis2_qname_t *AXIS2_CALL
woden_wsdl10_msg_ref_get_qname(
      void *msg_ref,
      const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;
      axis2_hash_t *super = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      super = WODEN_WSDL10_MSG_REF_SUPER_OBJS(msg_ref, env);
      msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super,
						 "WODEN_WSDL10_MSG_REF", 
						 AXIS2_HASH_KEY_STRING));

      return msg_ref_impl->f_qname;
}

void *AXIS2_CALL
woden_wsdl10_msg_ref_to_element(
      void *msg_ref,
      const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;
      axis2_hash_t *super = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      super = WODEN_WSDL10_MSG_REF_SUPER_OBJS(msg_ref, env);
      msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super,
						 "WODEN_WSDL10_MSG_REF", 
						 AXIS2_HASH_KEY_STRING));

      return &(msg_ref_impl->msg_ref);
}

/* ************************************************************
 *  Interface Operation Element methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
woden_wsdl10_msg_ref_set_qname(
      void *msg_ref,
      const axis2_env_t *env,
      axis2_qname_t *qname)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;
      axis2_hash_t *super = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
      super = WODEN_WSDL10_MSG_REF_SUPER_OBJS(msg_ref, env);
      msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super,
						 "WODEN_WSDL10_MSG_REF", 
						 AXIS2_HASH_KEY_STRING));

      if (msg_ref_impl->f_qname)
      {
	    AXIS2_QNAME_FREE(msg_ref_impl->f_qname, env);
	    msg_ref_impl->f_qname = NULL;
      }
      msg_ref_impl->f_qname = AXIS2_QNAME_CLONE(qname, env);

      return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_wsdl10_msg_ref_set_part(
      void *msg_ref,
      const axis2_env_t *env,
      void *part)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;
      axis2_hash_t *super = NULL;

      AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
      AXIS2_PARAM_CHECK(env->error, part, AXIS2_FAILURE);
      super = WODEN_WSDL10_MSG_REF_SUPER_OBJS(msg_ref, env);
      msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super,
						 "WODEN_WSDL10_MSG_REF", 
						 AXIS2_HASH_KEY_STRING));

      if (!msg_ref_impl->f_part)
      {
	    /* TODO */
      }
      msg_ref_impl->f_part = part;


      return AXIS2_SUCCESS;
}

void *AXIS2_CALL
woden_wsdl10_msg_ref_get_part(
      void *msg_ref,
      const axis2_env_t *env)
{
      woden_wsdl10_msg_ref_impl_t *msg_ref_impl = NULL;
      axis2_hash_t *super = NULL;

      AXIS2_ENV_CHECK(env, NULL);
      super = WODEN_WSDL10_MSG_REF_SUPER_OBJS(msg_ref, env);
      msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super,
						 "WODEN_WSDL10_MSG_REF",
						 AXIS2_HASH_KEY_STRING));

      return msg_ref_impl->f_part;
}

