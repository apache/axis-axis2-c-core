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

#include <axis2_wsdl_pump.h>
#include <axis2_wsdl4c_binding.h>
#include <axis2_wsdl4c_constraint.h>
#include <axis2_wsdl4c_element.h>
#include <axis2_wsdl4c_message.h>
#include <axis2_wsdl4c_operation.h>
#include <axis2_wsdl4c_parser.h>
#include <axis2_wsdl4c_part.h>
#include <axis2_wsdl4c_port_type.h>
#include <axis2_wsdl4c_qname.h>
#include <axis2_wsdl4c_service.h>
#include <axis2_wsdl4c_soap.h>
#include <axis2_wsdl4c_schema_parser.h>

#include <axis2_wsdl11_mep_finder.h>
#include <axis2_wsdl_ext_soap_address.h>
#include <axis2_wsdl_ext_soap_binding.h>
#include <axis2_wsdl_ext_soap_body.h>
#include <axis2_wsdl_ext_soap_header.h>
#include <axis2_wsdl_ext_soap_op.h>

#include <axis2_wsdl_binding_fault.h>
#include <axis2_wsdl_binding.h>
#include <axis2_wsdl_binding_msg_ref.h>
#include <axis2_wsdl_binding_op.h>
#include <axis2_wsdl_component.h>
#include <axis2_wsdl_desc.h>
#include <axis2_wsdl_endpoint.h>
#include <axis2_wsdl_extensible_attribute.h>
#include <axis2_wsdl_extensible_component.h>
#include <axis2_wsdl_extensible_element.h>
#include <axis2_wsdl_fault_ref.h>
#include <axis2_wsdl_interface.h>
#include <axis2_wsdl_msg_ref.h>
#include <axis2_wsdl_op.h>
#include <axis2_wsdl_soap_op.h>
#include <axis2_wsdl_svc.h>
#include <axis2_wsdl_types.h>
#include <axis2_string.h>

#define XMLSCHEMA_NAMESPACE_URI Constants.URI_2001_SCHEMA_XSD
#define XMLSCHEMA_NAMESPACE_PREFIX "xs"
#define XML_SCHEMA_LOCAL_NAME "schema"
#define XML_SCHEMA_SEQUENCE_LOCAL_NAME "sequence"
#define XML_SCHEMA_COMPLEX_TYPE_LOCAL_NAME "complexType"
#define XML_SCHEMA_ELEMENT_LOCAL_NAME "element"
#define XML_SCHEMA_IMPORT_LOCAL_NAME "import"

#define XSD_NAME "name"
#define XSD_TARGETNAMESPACE "targetNamespace"
#define XMLNS_AXIS2WRAPPED "xmlns:axis2wrapped"
#define AXIS2WRAPPED "axis2wrapped"
#define XSD_TYPE "type"
#define XSD_REF "ref"
#define BOUND_INTERFACE_NAME "BoundInterface"
#define XSD_ELEMENT_FORM_DEFAULT "elementFormDefault"
#define XSD_UNQUALIFIED "unqualified"

/** 
 * @brief Wsdl pump struct impl
 *   Wsdl Pump  
 */ 
typedef struct axis2_wsdl_pump_impl
{
   axis2_wsdl_pump_t wsdl_pump;
    int ns_count;
    axis2_array_list_t *wsdl4c_svcs;
    axis2_wsdl_desc_t *wom_def;
    axis2_hash_t * declared_nameSpaces;
    axis2_hash_t *resolved_rpc_wrapped_element_map;
   void *parser;

} axis2_wsdl_pump_impl_t;

#define AXIS2_INTF_TO_IMPL(wsdl_pump) \
      ((axis2_wsdl_pump_impl_t *)wsdl_pump)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
   axis2_wsdl_pump_free (axis2_wsdl_pump_t *wsdl_pump,
                           const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_pump_pump(axis2_wsdl_pump_t *wsdl_pump,
                  const axis2_env_t *env);

static axis2_status_t
axis2_wsdl_pump_populate_def(axis2_wsdl_pump_t *wsdl_pump,
                                const axis2_env_t *env);

/*//////////////////////////////////////////////////////////////////////////////
//////////////////////// Top level Components Copying ////////////////////////*/

/**
 * Simply Copy information.
 *
 * @param axis2_wsdl_interface
 * @param wsdl4c_port_type
 */
/* FIXME Evaluate a way of injecting pumps and properties with a general
 *  formatted input */

static axis2_status_t
axis2_wsdl_pump_populate_interfaces(axis2_wsdl_pump_t *wsdl_pump,
                                    const axis2_env_t *env,
                                    axis2_wsdl_interface_t *interface,
                                    void *port_type);

/**
 *
 * Finds whether a given message is wrappable
 * @return
 */
static axis2_bool_t
axis2_wsdl_pump_find_wrappable(axis2_wsdl_pump_t *wsdl_pump,
                        const axis2_env_t *env,
                        void *message);

/**
 * Pre Condition: The Interface Components must be copied by now.
 */
static axis2_status_t
axis2_wsdl_pump_populate_bindings(axis2_wsdl_pump_t *wsdl_pump,
                                    const axis2_env_t *env,
                                    axis2_wsdl_binding_t *wsdl_binding,
                                    void *binding);

/**
 *
 * @param wsdl_binding_op
 * @param wsdl4c_binding_op
 * @param namepace_of_the_binding_op
 */
static axis2_status_t
axis2_wsdl_pump_populate_binding_operation(axis2_wsdl_pump_t *wsdl_pump,
                                 const axis2_env_t *env,
                                            void *binding,
                                            axis2_wsdl_binding_op_t *
                                                    wsdl_binding_op,
                                            int op_index,
                                            char *op_name);

axis2_status_t AXIS2_CALL
axis2_wsdl_pump_populate_services(axis2_wsdl_pump_t *wsdl_pump,
                           const axis2_env_t *env,
                           axis2_wsdl_svc_t *wsdl_svc,
                           void *wsdl4c_svc);
               
/*
/////////////////////////////////////////////////////////////////////////////
//////////////////////////// Internal Component Copying ///////////////////
*/

axis2_status_t AXIS2_CALL
axis2_wsdl_pump_populate_operations(axis2_wsdl_pump_t *wsdl_pump,
                           const axis2_env_t *env,
                           axis2_wsdl_op_t *wsdl_op,
                           void *wsdl4c_op,
                           axis2_char_t *namespce_of_op,
                           axis2_wsdl_types_t *wsdl_types);

/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

/**
 * Generates a reference QName
 * @param wsdl4c_message
 * @return
 */
static axis2_qname_t *
axis2_wsdl_pump_generate_reference_qname(axis2_wsdl_pump_t *wsdl_pump,
                                 const axis2_env_t *env,
                                 axis2_qname_t *outer_qname,
                                 void *wsdl4c_msg,
                                 axis2_bool_t is_wrappable);

axis2_status_t AXIS2_CALL
axis2_wsdl_pump_populate_ports(axis2_wsdl_pump_t *wsdl_pump,
                        const axis2_env_t *env,
                        axis2_wsdl_endpoint_t *wsdl_endpoint,
                        void *wsdl4c_binding,
                        char *port_name,
                        char *target_namespc);

/**
 * This method will fill up the gap of WSDL 1.1 and WSDL 2.0 w.r.t. the
 * bound interface for the Service Component Defined in the WSDL 2.0. Logic
 * being if there exist only one PortType in the WSDL 1.1 file then that
 * will be set as the bound interface of the Service. If more than one
 * Porttype exist in the WSDl 1.1 file this will create a dummy Interface
 * with the available PortTypes and will return that interface so that it
 * will inherit all those interfaces.
 * <p/>
 * Eventually this will have to be fixed using user input since
 *
 * @param service
 * @return wsdl interface
 */
static axis2_wsdl_interface_t *
axis2_wsdl_pump_get_bound_interface(axis2_wsdl_pump_t *wsdl_pump,
                           const axis2_env_t *env,
                           axis2_wsdl_svc_t *wsdl_svc);
                                
/************************** End of function prototypes ************************/

axis2_wsdl_pump_t * AXIS2_CALL 
axis2_wsdl_pump_create (const axis2_env_t *env,
                  axis2_wsdl_desc_t *wom_def,
                  void *wsdl_parser)
{
    axis2_wsdl_pump_impl_t *wsdl_pump_impl = NULL;
    
   AXIS2_ENV_CHECK(env, NULL);
   
   wsdl_pump_impl = (axis2_wsdl_pump_impl_t *) AXIS2_MALLOC(env->allocator,
         sizeof(axis2_wsdl_pump_impl_t));

   if(NULL == wsdl_pump_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    wsdl_pump_impl->wom_def = NULL;
   wsdl_pump_impl->parser = NULL;
   wsdl_pump_impl->wsdl4c_svcs = NULL;
    wsdl_pump_impl->wsdl_pump.ops = NULL;

   if(wom_def)
   {
      wsdl_pump_impl->wom_def = wom_def;
   }

   if(wsdl_parser)
   {
      wsdl_pump_impl->parser = wsdl_parser;
   }
   
   wsdl_pump_impl->wsdl_pump.ops = 
      AXIS2_MALLOC (env->allocator, sizeof(axis2_wsdl_pump_ops_t));
   if(NULL == wsdl_pump_impl->wsdl_pump.ops)
    {
        axis2_wsdl_pump_free(&(wsdl_pump_impl->wsdl_pump), env);
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
   wsdl_pump_impl->wsdl_pump.ops->free =  axis2_wsdl_pump_free;
   wsdl_pump_impl->wsdl_pump.ops->pump = axis2_wsdl_pump_pump;
   
   return &(wsdl_pump_impl->wsdl_pump);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_pump_free (axis2_wsdl_pump_t *wsdl_pump, 
                            const axis2_env_t *env)
{
    axis2_wsdl_pump_impl_t *pump_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    pump_impl = AXIS2_INTF_TO_IMPL(wsdl_pump);
    
   if(NULL != wsdl_pump->ops)
        AXIS2_FREE(env->allocator, wsdl_pump->ops);
    
    AXIS2_FREE(env->allocator, pump_impl);
    pump_impl = NULL;
    
   return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_pump_pump(axis2_wsdl_pump_t *wsdl_pump,
                  const axis2_env_t *env)
{
   AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   return axis2_wsdl_pump_populate_def(wsdl_pump, env);   
}

static axis2_status_t
axis2_wsdl_pump_populate_def(axis2_wsdl_pump_t *wsdl_pump,
                                const axis2_env_t *env)
{
    axis2_wsdl_pump_impl_t *pump_impl = NULL;
    int element_type = -1;
    axis2_wsdl_interface_t *wsdl_interface = NULL;
    void *port_type = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_wsdl_binding_t *wsdl_binding = NULL;
    void *binding = NULL;
    axis2_wsdl_svc_t *wsdl_svc = NULL;
    void *service = NULL;


    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    pump_impl = AXIS2_INTF_TO_IMPL(wsdl_pump);

    /* Go through the axis2_wsdl4c interfaces and pump it to the WOM */
    
    /*
    /////////////////////////////////////////////////////////////////// //
    // Order of the following items shouldn't be changed unless you     //
    // really know what you are doing. Reason being the components that //
    // are copied(pumped) towards the end depend on the components that //
    // has already being pumped. Following Lists some of the            //
    // dependencies.                                                    //
    //  1) The Binding refers to the Interface                          //
    //  2) The Endpoint refers to the Bindings                          //
    // ....                                                             //
    //                                                      //
    //////////////////////////////////////////////////////////////////////
    */
   /*
    ///////////////////(2)Copy the Interfaces////////////////////////////
    //copy the Interfaces: Get the port_types from axis2_wsdl4c parse OM and
    // copy it to the  WOM's axis2_wsdl_interface Components
    */
    while (axis2_wsdl4c_parser_get_event_type(pump_impl->parser) != 
                    AXIS2_WSDL4C_PARSER_END)
    {
       element_type = axis2_wsdl4c_parser_get_next_element(pump_impl->parser);
       switch (element_type)
        {
            case AXIS2_WSDL4C_PARSER_PORT_TYPE:
                wsdl_interface = axis2_wsdl_interface_create(env);
                if(!wsdl_interface)
                {
                    return AXIS2_FAILURE;
                }
                port_type = axis2_wsdl4c_parser_get_port_type(pump_impl->parser);
                status = axis2_wsdl_pump_populate_interfaces(wsdl_pump, env, 
                    wsdl_interface, port_type);
                if(AXIS2_SUCCESS != status)
                {
                    return status;
                }
                status = AXIS2_WSDL_DESC_ADD_INTERFACE(pump_impl->wom_def, env, 
                 wsdl_interface);
                if(AXIS2_SUCCESS != status)
                {
                    return status;
                }
                break;

    /*
   ///////////////////////////////////////(3)Copy the Bindings///////////////////////
    //pump the Bindings: Get the Bindings map from wsdl4c and create a new
    // map of wsdl_binding elements. At this point we need to do some extra work since there
    //can be header parameters 
   */
            case AXIS2_WSDL4C_PARSER_BINDING:
                wsdl_binding = axis2_wsdl_binding_create(env);
                if(!wsdl_binding)
                {
                    return AXIS2_FAILURE;
                }
                binding = axis2_wsdl4c_parser_get_binding(pump_impl->parser);
                status = axis2_wsdl_pump_populate_bindings(wsdl_pump, env, wsdl_binding, 
                    binding);
                if(AXIS2_SUCCESS != status)
                {
                    return status;
                }
                status = AXIS2_WSDL_DESC_ADD_BINDING(pump_impl->wom_def, env, 
                            wsdl_binding);
                if(AXIS2_SUCCESS != status)
                {
                    return status;
                }
                break;

   /* ///////////////////(4)Copy the Services///////////////////////////////*/

            case AXIS2_WSDL4C_PARSER_SERVICE:
                wsdl_svc = axis2_wsdl_svc_create(env);
                if(!wsdl_svc)
                {
                    return AXIS2_FAILURE;
                }
                 service = axis2_wsdl4c_parser_get_service(pump_impl->parser);
                status = axis2_wsdl_pump_populate_services(wsdl_pump, env, wsdl_svc, 
                    service);
                if(AXIS2_SUCCESS != status)
                {
                    return status;
                }
                status = AXIS2_WSDL_DESC_ADD_SVC(pump_impl->wom_def, env, wsdl_svc);
                if(AXIS2_SUCCESS != status)
                {
                    return status;
                }
                break;
        }
    }
   return AXIS2_SUCCESS;
}

/*//////////////////////////////////////////////////////////////////////////////
//////////////////////// Top level Components Copying ////////////////////////*/

/**
 * Simply Copy information.
 *
 * @param axis2_wsdl_interface
 * @param wsdl4c_port_type
 */
/* FIXME Evaluate a way of injecting pumps and properties with a general
 *  formatted input */

static axis2_status_t
axis2_wsdl_pump_populate_interfaces(axis2_wsdl_pump_t *wsdl_pump,
                                    const axis2_env_t *env,
                                    axis2_wsdl_interface_t *interface,
                                    void *port_type)
{
    axis2_wsdl_pump_impl_t *pump_impl = NULL;
    axis2_qname_t *port_type_qname = NULL;
    axis2_char_t *port_type_name = NULL;
    axis2_array_list_t *operations = NULL;
    axis2_wsdl_op_t *wsdl_op = NULL;
    void *operation = NULL;
   axis2_status_t status = AXIS2_FAILURE;
   int i = 0, size = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, interface, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, port_type, AXIS2_FAILURE);
    pump_impl = AXIS2_INTF_TO_IMPL(wsdl_pump);

    /* 
     * Copy the Attribute information items
     * Copied with the Same QName so it will require no Query in Binding
     * Coping.
     */
    port_type_name = axis2_wsdl4c_port_type_get_name(port_type);
    port_type_qname = axis2_qname_create(env, port_type_name, NULL, NULL); 
    if(!port_type_qname)
    {
        return AXIS2_FAILURE;
    }
    status = AXIS2_WSDL_INTERFACE_SET_NAME(interface, env, port_type_qname);
    if(AXIS2_SUCCESS != status)
    {
        return status;
    }
    operations = axis2_wsdl4c_port_type_get_operations(port_type);
    size = AXIS2_ARRAY_LIST_SIZE(operations, env);
    for(i = 0; i < size; i++)
    {
        axis2_wsdl_types_t *wsdl_types = NULL;
        char *namespc_uri = NULL;

        wsdl_op = axis2_wsdl_op_create(env);
        if(!wsdl_op)
        {
            return AXIS2_FAILURE;
        }
        operation = (void *) AXIS2_ARRAY_LIST_GET(operations, env, i);
        namespc_uri = AXIS2_QNAME_GET_URI(port_type_qname, env); 
        wsdl_types = AXIS2_WSDL_DESC_GET_TYPES(pump_impl->wom_def, env);
        axis2_wsdl_pump_populate_operations(wsdl_pump, env, wsdl_op, operation,
            namespc_uri, wsdl_types);
        status = AXIS2_WSDL_INTERFACE_SET_OP(interface, env, wsdl_op);
        if(AXIS2_SUCCESS != status)
        {
            return status;
        }
    }
    if(operations)
        AXIS2_ARRAY_LIST_FREE(operations, env);
   return AXIS2_SUCCESS;
}

/**
 *
 * Finds whether a given message is wrappable
 * @return
 */
static axis2_bool_t
axis2_wsdl_pump_find_wrappable(axis2_wsdl_pump_t *wsdl_pump,
                        const axis2_env_t *env,
                        void *message)
{
   int no_of_parts = 0;
   axis2_bool_t wrappable = AXIS2_FALSE;
   
   AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK(env->error, message, AXIS2_FAILURE);

/*
// ********************************************************************************************
// Note
// We will not use the binding to set the wrappable/unwrappable state here. instead we'll look at the
// Messages for the following features
// 1. Messages with multiple parts -> We have no choice but to wrap
// 2. Messages with one part having a type attribute -> Again we have no choice but to wrap

// ********************************************************************************************
*/
   no_of_parts = axis2_wsdl4c_msg_get_num_parts(message);
   if(no_of_parts > 1)
      wrappable = AXIS2_TRUE;
   if(AXIS2_FALSE == wrappable)
   {
      void *part = axis2_wsdl4c_msg_get_message_part_a_index(message, 0);
      int part_id = axis2_wsdl4c_part_type(part);
      if(0 != part_id)
         wrappable = AXIS2_TRUE;
   }
   return wrappable;
}

/**
 * Pre Condition: The Interface Components must be copied by now.
 */
static axis2_status_t
axis2_wsdl_pump_populate_bindings(axis2_wsdl_pump_t *wsdl_pump,
                                    const axis2_env_t *env,
                                    axis2_wsdl_binding_t *wsdl_binding,
                                    void *binding)
{
    axis2_wsdl_pump_impl_t *pump_impl = NULL;
    axis2_qname_t *interface_qname = NULL;
    axis2_char_t *interface_name = NULL;
    axis2_qname_t *binding_qname = NULL;
    axis2_char_t *binding_name = NULL;
    axis2_wsdl_interface_t *wsdl_interface = NULL;
    axis2_array_list_t *ops = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    void *port_type;
    int i = 0, size = 0;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, wsdl_binding, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, binding, AXIS2_FAILURE);
    pump_impl = AXIS2_INTF_TO_IMPL(wsdl_pump);

    binding_name = axis2_wsdl4c_binding_get_name(binding);
    binding_qname = axis2_qname_create(env, binding_name, NULL, NULL);
    status = AXIS2_WSDL_BINDING_SET_NAME(wsdl_binding, env, binding_qname);
    if(binding_qname)
    {
        AXIS2_QNAME_FREE(binding_qname, env);
        binding_qname = NULL;
    }
    if(AXIS2_SUCCESS != status)
    {
        return status;
    }
    port_type = axis2_wsdl4c_binding_get_port_type(binding);
    interface_name = axis2_wsdl4c_port_type_get_name(port_type);
    interface_qname = axis2_qname_create(env, interface_name, NULL, NULL);
    wsdl_interface = AXIS2_WSDL_DESC_GET_INTERFACE(pump_impl->wom_def, env, 
         interface_qname);
    if(interface_qname)
    {
        AXIS2_QNAME_FREE(interface_qname, env);
        interface_qname = NULL;
    }
    /* FIXME Do We need this eventually??? */
    if(!wsdl_interface)
    {
        AXIS2_ERROR_SET(env->error, 
            AXIS2_ERROR_INTERFACE_OR_PORT_TYPE_NOT_FOUND_FOR_THE_BINDING,
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    status = AXIS2_WSDL_BINDING_SET_BOUND_INTERFACE(wsdl_binding, env, 
        wsdl_interface);
    if(AXIS2_SUCCESS != status)
    {
        return status;
    }
    ops = axis2_wsdl4c_port_type_get_operations(port_type);
    if(!ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_WSDL_PARSER_INVALID_STATE,
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    } 
    size = AXIS2_ARRAY_LIST_SIZE(ops, env);
    for(i = 0; i < size; i++)
    {
        void *operation = NULL;
        axis2_wsdl_op_t *wsdl_op = NULL;
        axis2_char_t *op_name = NULL;
        int op_index = 0;
        axis2_wsdl_binding_op_t *wsdl_binding_op = NULL;

        operation = AXIS2_ARRAY_LIST_GET(ops, env, i);
        op_name = axis2_wsdl4c_operation_get_name(operation);
        op_index  = axis2_wsdl4c_port_type_get_operation_index(port_type, 
            op_name);
        wsdl_binding_op = axis2_wsdl_binding_op_create(env);
        if(!wsdl_binding_op)
        {
            return AXIS2_FAILURE;
        }
        status = axis2_wsdl_pump_populate_binding_operation(wsdl_pump, env, 
            binding, wsdl_binding_op, op_index, op_name);
        if(AXIS2_SUCCESS != status)
        {
            return status;
        }
        wsdl_op = AXIS2_WSDL_INTERFACE_GET_OP(wsdl_interface, env, op_name);
       
        AXIS2_WSDL_BINDING_OP_SET_OP(wsdl_binding_op, env, wsdl_op);
        status = AXIS2_WSDL_BINDING_ADD_BINDING_OP(wsdl_binding, env, 
            wsdl_binding_op);
        if(AXIS2_SUCCESS != status)
        {
            return status;
        }
    }
    if(ops)
        AXIS2_ARRAY_LIST_FREE(ops, env);
   return AXIS2_SUCCESS;
}

/**
 *
 * @param wsdl_binding_op
 * @param wsdl4c_binding_op
 * @param namepace_of_the_binding_op
 */
static axis2_status_t
axis2_wsdl_pump_populate_binding_operation(axis2_wsdl_pump_t *wsdl_pump,
                                 const axis2_env_t *env,
                                            void *binding,
                                            axis2_wsdl_binding_op_t *
                                                    wsdl_binding_op,
                                            int op_index,
                                            char *op_name)
{
    axis2_wsdl_pump_impl_t *pump_impl = NULL;
    int *bindings = 0;
    int soap_op_binding_id = 0;
    void *soap = NULL;
    axis2_char_t *action = NULL;
    axis2_wsdl4c_style_t style = 0;
   axis2_char_t *str_style = NULL;
    axis2_qname_t *binding_op_qname = NULL;
    int nbindings = 0;
    int i = 0;
    axis2_wsdl_binding_msg_ref_t *wsdl_input_binding = NULL;
    axis2_wsdl_binding_msg_ref_t *wsdl_output_binding = NULL;
    axis2_wsdl_binding_msg_ref_t *wsdl_fault_binding = NULL;
   axis2_status_t status = AXIS2_FAILURE;
   axis2_wsdl_ext_soap_op_t *ext_soap_op = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, wsdl_binding_op, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, binding, AXIS2_FAILURE);
    pump_impl = AXIS2_INTF_TO_IMPL(wsdl_pump);
   
    axis2_wsdl4c_binding_get_op_binding(binding, op_index, &bindings); 
    soap_op_binding_id = bindings[0];
    soap = axis2_wsdl4c_parser_get_extensibility_handler_a_ns(pump_impl->parser, 
         AXIS2_WSDL4C_SOAP_BINDING_URI);
    axis2_wsdl4c_soap_get_operation_info(soap, soap_op_binding_id, &action, 
         &style);
   ext_soap_op = axis2_wsdl_ext_soap_op_create(env, NULL);
   AXIS2_WSDL_EXT_SOAP_OP_SET_SOAP_ACTION(ext_soap_op, env, action);
   if(AXIS2_WSDL4C_RPC == style)
   {
      str_style = AXIS2_STRDUP("rpc", env);
   }
   if(AXIS2_WSDL4C_DOC == style)
   {
      str_style = AXIS2_STRDUP("document", env);
   }
   AXIS2_WSDL_EXT_SOAP_OP_SET_STYLE(ext_soap_op, env, str_style);
    ext_soap_op->base.namespc = AXIS2_STRDUP(AXIS2_WSDL4C_SOAP_BINDING_URI, env);
        if(!ext_soap_op->base.namespc) return AXIS2_FAILURE;
   AXIS2_WSDL_COMPONENT_ADD_EXTENSIBILITY_ELEMENT(wsdl_binding_op->
               extensible_component->wsdl_component, env, ext_soap_op);
    binding_op_qname = axis2_qname_create(env, op_name, NULL, NULL);
    AXIS2_WSDL_BINDING_OP_SET_QNAME(wsdl_binding_op, env, binding_op_qname); 
    
    /* Fill input message reference */ 
    wsdl_input_binding = axis2_wsdl_binding_msg_ref_create(env);
    if(!wsdl_input_binding)
    {
        return AXIS2_FAILURE;
    } 
    AXIS2_WSDL_BINDING_MSG_REF_SET_DIRECTION(wsdl_input_binding, env,
        AXIS2_WSDL_MESSAGE_DIRECTION_IN); 
    nbindings = axis2_wsdl4c_binding_get_input_binding(binding, op_index, &bindings);
    for(i = 0; i < nbindings; i++)
    {
        if(AXIS2_TRUE == axis2_wsdl4c_soap_is_soap_body(soap, bindings[i]))
        {
            axis2_wsdl_ext_soap_body_t *soap_body = NULL;
            axis2_wsdl4c_encoding_t use = 0;
            axis2_char_t *nsp = NULL;
         axis2_char_t *str_use = NULL;
            axis2_char_t *encoding_style = NULL;

            axis2_wsdl4c_soap_get_body_info(soap, bindings[i], &nsp, &use, &encoding_style);
            soap_body = axis2_wsdl_ext_soap_body_create(env, NULL);
            if(!soap_body) return AXIS2_FAILURE;
         if(AXIS2_WSDL4C_RPC == use)
            str_use = AXIS2_STRDUP("rpc", env);
         if(AXIS2_WSDL4C_DOC == use)
            str_use = AXIS2_STRDUP("document", env);
          AXIS2_WSDL_EXT_SOAP_BODY_SET_USE(soap_body, env, str_use);
         if(str_use)
         {
            AXIS2_FREE(env->allocator, str_use);
            str_use = NULL;
         }
          AXIS2_WSDL_EXT_SOAP_BODY_SET_NAMESPC_URI(soap_body, env, nsp);
            AXIS2_WSDL_COMPONENT_ADD_EXTENSIBILITY_ELEMENT(wsdl_input_binding->
                extensible_component->wsdl_component, env, soap_body);
        }
        else if(AXIS2_TRUE == axis2_wsdl4c_soap_is_soap_header(soap, bindings[i]))
        {
            axis2_wsdl_ext_soap_header_t *soap_header = NULL;
            int part_id = 0;
         axis2_char_t *part_name = NULL;
         axis2_char_t *msg_name = NULL;
         axis2_qname_t *msg_qname = NULL;
            void *message = NULL;
    
            axis2_wsdl4c_soap_get_header_info(soap, bindings[i], &part_id, &message);
         part_name = axis2_wsdl4c_msg_get_part_name(message, part_id);
         msg_name = axis2_wsdl4c_msg_get_name(message);
         msg_qname = axis2_qname_create(env, msg_name, NULL, NULL);
            AXIS2_WSDL_EXT_SOAP_HEADER_SET_PART(soap_header, env, part_name);
            AXIS2_WSDL_EXT_SOAP_HEADER_SET_MSG_QNAME(soap_header, env, msg_qname);
         if(msg_qname)
         {
            AXIS2_QNAME_FREE(msg_qname, env);
            msg_qname = NULL;
         }
            AXIS2_WSDL_COMPONENT_ADD_EXTENSIBILITY_ELEMENT(wsdl_input_binding->
               extensible_component->wsdl_component, env, soap_header);
        }
        status = AXIS2_WSDL_BINDING_OP_SET_INPUT(wsdl_binding_op, env, 
            wsdl_input_binding);
      if(AXIS2_SUCCESS != status)
      {
          return status;
      }
    }

    /* Fill output message reference */ 
    wsdl_output_binding = axis2_wsdl_binding_msg_ref_create(env);
    if(!wsdl_output_binding)
    {
        return AXIS2_FAILURE;
    } 
    AXIS2_WSDL_BINDING_MSG_REF_SET_DIRECTION(wsdl_output_binding, env,
        AXIS2_WSDL_MESSAGE_DIRECTION_OUT); 
    nbindings = axis2_wsdl4c_binding_get_output_binding(binding, op_index, &bindings);
    for(i = 0; i < nbindings; i++)
    {
        if(AXIS2_TRUE == axis2_wsdl4c_soap_is_soap_body(soap, bindings[i]))
        {
            axis2_wsdl_ext_soap_body_t *soap_body = NULL;
            axis2_wsdl4c_encoding_t use = 0;
         axis2_char_t *str_use = NULL;
            axis2_char_t *nsp = NULL;
            axis2_char_t *encoding_style = NULL;

            axis2_wsdl4c_soap_get_body_info(soap, bindings[i], &nsp, &use, &encoding_style);
            soap_body = axis2_wsdl_ext_soap_body_create(env, NULL);
            if(!soap_body) return AXIS2_FAILURE;
         if(AXIS2_WSDL4C_RPC == use)
            str_use = AXIS2_STRDUP("rpc", env);
         if(AXIS2_WSDL4C_DOC == use)
            str_use = AXIS2_STRDUP("document", env);
          AXIS2_WSDL_EXT_SOAP_BODY_SET_USE(soap_body, env, str_use);
          AXIS2_WSDL_EXT_SOAP_BODY_SET_NAMESPC_URI(soap_body, env, nsp);
            AXIS2_WSDL_COMPONENT_ADD_EXTENSIBILITY_ELEMENT(wsdl_output_binding->
                extensible_component->wsdl_component, env, soap_body);
        }
        else if(AXIS2_TRUE == axis2_wsdl4c_soap_is_soap_header(soap, bindings[i]))
        {
            axis2_wsdl_ext_soap_header_t *soap_header = NULL;
            int part_id = 0;
         axis2_char_t *part_name = NULL;
         axis2_char_t *msg_name = NULL;
         axis2_qname_t *msg_qname = NULL;
            void *message = NULL;
         
    
            axis2_wsdl4c_soap_get_header_info(soap, bindings[i], &part_id, &message);
         part_name = axis2_wsdl4c_msg_get_part_name(message, part_id);
         msg_name = axis2_wsdl4c_msg_get_name(message);
         msg_qname = axis2_qname_create(env, msg_name, NULL, NULL);
            AXIS2_WSDL_EXT_SOAP_HEADER_SET_PART(soap_header, env, part_name);
            AXIS2_WSDL_EXT_SOAP_HEADER_SET_MSG_QNAME(soap_header, env, msg_qname);
            AXIS2_WSDL_COMPONENT_ADD_EXTENSIBILITY_ELEMENT(wsdl_output_binding->
                extensible_component->wsdl_component, env, soap_header);
        }
        status = AXIS2_WSDL_BINDING_OP_SET_OUTPUT(wsdl_binding_op, env, wsdl_output_binding);
        if(AXIS2_SUCCESS != status)
       {
           return status;
       }
    }
    
    /* Fill fault message reference */ 
    wsdl_fault_binding = axis2_wsdl_binding_msg_ref_create(env);
    if(!wsdl_fault_binding)
    {
        return AXIS2_FAILURE;
    } 
    AXIS2_WSDL_BINDING_MSG_REF_SET_DIRECTION(wsdl_fault_binding, env,
        AXIS2_WSDL_MESSAGE_DIRECTION_OUT); 
    nbindings = axis2_wsdl4c_binding_get_fault_binding(binding, op_index, &bindings);
    for(i = 0; i < nbindings; i++)
    {
        if(AXIS2_TRUE == axis2_wsdl4c_soap_is_soap_body(soap, bindings[i]))
        {
            axis2_wsdl_ext_soap_body_t *soap_body = NULL;
            axis2_wsdl4c_encoding_t use = 0;
         axis2_char_t *str_use = NULL;
            axis2_char_t *nsp = NULL;
            axis2_char_t *encoding_style;

            axis2_wsdl4c_soap_get_body_info(soap, bindings[i], &nsp, &use, &encoding_style);
         if(AXIS2_WSDL4C_RPC == use)
            str_use = AXIS2_STRDUP("rpc", env);
         if(AXIS2_WSDL4C_DOC == use)
            str_use = AXIS2_STRDUP("document", env);
          AXIS2_WSDL_EXT_SOAP_BODY_SET_USE(soap_body, env, str_use);
          AXIS2_WSDL_EXT_SOAP_BODY_SET_NAMESPC_URI(soap_body, env, nsp);
            AXIS2_WSDL_COMPONENT_ADD_EXTENSIBILITY_ELEMENT(wsdl_fault_binding->
                extensible_component->wsdl_component, env, soap_body);
        }
        else if(AXIS2_TRUE == axis2_wsdl4c_soap_is_soap_header(soap, bindings[i]))
        {
            axis2_wsdl_ext_soap_header_t *soap_header = NULL;
            int part_id = 0;
         axis2_char_t *part_name = NULL;
         axis2_char_t *msg_name = NULL;
         axis2_qname_t *msg_qname = NULL;
            void *message = NULL;
    
            axis2_wsdl4c_soap_get_header_info(soap, bindings[i], &part_id, &message);
         part_name = axis2_wsdl4c_msg_get_part_name(message, part_id);
         msg_name = axis2_wsdl4c_msg_get_name(message);
         msg_qname = axis2_qname_create(env, msg_name, NULL, NULL);
            AXIS2_WSDL_EXT_SOAP_HEADER_SET_PART(soap_header, env, part_name);
            AXIS2_WSDL_EXT_SOAP_HEADER_SET_MSG_QNAME(soap_header, env, msg_qname);
            AXIS2_WSDL_COMPONENT_ADD_EXTENSIBILITY_ELEMENT(wsdl_fault_binding->
                extensible_component->wsdl_component, env, soap_header);
        }
        status = AXIS2_WSDL_BINDING_OP_SET_OUTPUT(wsdl_binding_op, env, wsdl_fault_binding);
      if(AXIS2_SUCCESS != status)
      {
          return status;
      }
    }
   return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_pump_populate_services(axis2_wsdl_pump_t *wsdl_pump,
                           const axis2_env_t *env,
                           axis2_wsdl_svc_t *wsdl_svc,
                           void *wsdl4c_svc)
{
   axis2_wsdl_pump_impl_t *pump_impl = NULL;
    axis2_qname_t *svc_qname = NULL;
   axis2_char_t *svc_name = NULL;
   axis2_array_list_t *ports = NULL;
   int i = 0, size = 0;
   axis2_wsdl_interface_t *bound_interface = NULL;
   axis2_status_t status = AXIS2_FAILURE;

   AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK(env->error, wsdl_svc, AXIS2_FAILURE);   
   AXIS2_PARAM_CHECK(env->error, wsdl4c_svc, AXIS2_FAILURE);
      pump_impl = AXIS2_INTF_TO_IMPL(wsdl_pump);
      
   svc_name = axis2_wsdl4c_service_get_name(wsdl4c_svc); 
   svc_qname = axis2_qname_create(env, svc_name, NULL, NULL);
   if(!svc_qname) return AXIS2_FAILURE;
   status = AXIS2_WSDL_SVC_SET_QNAME(wsdl_svc, env, svc_qname);
   if(AXIS2_SUCCESS != status) return status;

   bound_interface = axis2_wsdl_pump_get_bound_interface(wsdl_pump, env, 
      wsdl_svc);
   status = AXIS2_WSDL_SVC_SET_SVC_INTERFACE(wsdl_svc, env, bound_interface);
   if(AXIS2_SUCCESS != status) return status;
   ports = axis2_wsdl4c_service_get_ports(wsdl4c_svc);
   size = AXIS2_ARRAY_LIST_SIZE(ports ,env);
   for(i = 0; i < size; i++)
   {
       char *port_name = NULL;
      axis2_wsdl_endpoint_t *wsdl_endpoint = NULL;
      char *svc_location = NULL;
      void *soap = NULL;
      void *binding = NULL;
      int svc_ext_id = 0;
      axis2_wsdl_ext_soap_address_t *ext_soap_address = NULL;

      port_name = AXIS2_ARRAY_LIST_GET(ports, env, i);
       binding = axis2_wsdl4c_service_get_port_binding(wsdl4c_svc, port_name);
      wsdl_endpoint = axis2_wsdl_endpoint_create(env);
      if(!wsdl_endpoint) return AXIS2_FAILURE;
      status = axis2_wsdl_pump_populate_ports(wsdl_pump, env, wsdl_endpoint, 
            binding, port_name, NULL);
      if(AXIS2_SUCCESS != status) return status;
      soap = axis2_wsdl4c_parser_get_extensibility_handler_a_ns(pump_impl->parser, 
            AXIS2_WSDL4C_SOAP_BINDING_URI);
      if(!soap) return AXIS2_FAILURE;
      svc_ext_id = axis2_wsdl4c_binding_get_service_ext_id(binding);
      axis2_wsdl4c_soap_get_service_location(soap, svc_ext_id, &svc_location);
      ext_soap_address = axis2_wsdl_ext_soap_address_create(env, NULL);
      if(!ext_soap_address) return AXIS2_FAILURE;
      AXIS2_WSDL_EXT_SOAP_ADDRESS_SET_LOCATION_URI(ext_soap_address, env, svc_location);
      ext_soap_address->base.namespc = AXIS2_STRDUP(AXIS2_WSDL4C_SOAP_BINDING_URI, env);
        if(!ext_soap_address->base.namespc) return AXIS2_FAILURE;
      AXIS2_WSDL_COMPONENT_ADD_EXTENSIBILITY_ELEMENT(wsdl_endpoint->wsdl_component, 
            env, ext_soap_address);
      status = AXIS2_WSDL_SVC_SET_ENDPOINT(wsdl_svc, env, wsdl_endpoint);
       if(AXIS2_SUCCESS != status) return status;
   }
    if(ports)
        AXIS2_ARRAY_LIST_FREE(ports, env);
   return AXIS2_SUCCESS;
}
               
/*
/////////////////////////////////////////////////////////////////////////////
//////////////////////////// Internal Component Copying ///////////////////
*/

axis2_status_t AXIS2_CALL
axis2_wsdl_pump_populate_operations(axis2_wsdl_pump_t *wsdl_pump,
                           const axis2_env_t *env,
                           axis2_wsdl_op_t *wsdl_op,
                           void *wsdl4c_op,
                           axis2_char_t *namespc_of_op,
                           axis2_wsdl_types_t *wsdl_types)
{
   axis2_wsdl_pump_impl_t *pump_impl = NULL;
   axis2_char_t *op_name = NULL;
   axis2_qname_t *op_qname = NULL;
   void *wsdl4c_input_msg = NULL;
   void *wsdl4c_output_msg = NULL;
   void *wsdl4c_fault_msg = NULL;
   axis2_wsdl_msg_ref_t *wsdl_input_msg = NULL;
   axis2_array_list_t *faults = NULL;
   axis2_qname_t *wrapped_input_qname = NULL;
   axis2_qname_t *wrapped_output_qname = NULL;
   axis2_char_t *qname_localpart = NULL;
   axis2_char_t *qname_uri = NULL;
   axis2_char_t *qname_prefix = NULL;
   axis2_char_t *temp = NULL;
   int i = 0, size = 0;
   axis2_char_t *mep = NULL;

   AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK(env->error, namespc_of_op, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK(env->error, wsdl4c_op, AXIS2_FAILURE);
   pump_impl = AXIS2_INTF_TO_IMPL(wsdl_pump);

   wsdl_input_msg = axis2_wsdl_msg_ref_create(env);
   if(!wsdl_input_msg) return AXIS2_FAILURE;
   AXIS2_WSDL_MSG_REF_SET_DIRECTION(wsdl_input_msg, env, AXIS2_WSDL_MESSAGE_DIRECTION_IN);
   AXIS2_WSDL_MSG_REF_SET_MSG_LABEL(wsdl_input_msg, env, AXIS2_WSDL_MESSAGE_LABEL_IN_VALUE);


   
    /* Copy Name Attribute */
   op_name = axis2_wsdl4c_operation_get_name(wsdl4c_op);
   op_qname = axis2_qname_create(env, op_name, namespc_of_op, NULL);
   if(!op_qname) return AXIS2_FAILURE;
   AXIS2_WSDL_OP_SET_QNAME(wsdl_op, env, op_qname);

    /*
    * This code make no attempt to make use of the special xs:Token
     * defined in the WSDL 2.0. eg like #any, #none
     * Create the Input Message and add
    */
    wsdl4c_input_msg = axis2_wsdl4c_operation_get_message(wsdl4c_op, 
      AXIS2_WSDL4C_INPUT);
   wrapped_input_qname = AXIS2_WSDL_OP_GET_QNAME(wsdl_op, env);
   if(wsdl4c_input_msg)
   {
      axis2_wsdl_msg_ref_t *wsdl_input_msg = NULL;
      axis2_qname_t *ref_qname = NULL;
      axis2_bool_t is_wrappable = AXIS2_FALSE;

      wsdl_input_msg = axis2_wsdl_msg_ref_create(env);
      if(!wsdl_input_msg) 
      {
         return AXIS2_FAILURE;
      }
      AXIS2_WSDL_MSG_REF_SET_DIRECTION(wsdl_input_msg, env, 
         AXIS2_WSDL_MESSAGE_DIRECTION_IN);
      AXIS2_WSDL_MSG_REF_SET_MSG_LABEL(wsdl_input_msg, env,
         AXIS2_WSDL_MESSAGE_LABEL_IN_VALUE);
      is_wrappable = axis2_wsdl_pump_find_wrappable(wsdl_pump, env,
         wsdl4c_input_msg);
      ref_qname = axis2_wsdl_pump_generate_reference_qname(wsdl_pump, env,
         wrapped_input_qname, wsdl4c_input_msg, is_wrappable);
      AXIS2_WSDL_MSG_REF_SET_ELEMENT(wsdl_input_msg, env, ref_qname);
      if(ref_qname)
      {
         AXIS2_QNAME_FREE(ref_qname, env);
         ref_qname = NULL;
      }
      AXIS2_WSDL_OP_SET_INPUT_MSG(wsdl_op, env, wsdl_input_msg);         
   }

    /* Create an output message and add */
   temp = AXIS2_QNAME_GET_LOCALPART(wrapped_input_qname, env);
   qname_localpart = AXIS2_STRACAT(temp, "Response", env);
   qname_uri = AXIS2_QNAME_GET_URI(wrapped_input_qname, env);
    qname_prefix = AXIS2_QNAME_GET_PREFIX(wrapped_input_qname, env);
   wrapped_output_qname = axis2_qname_create(env, qname_localpart, qname_uri,
      qname_prefix);
   wsdl4c_output_msg = axis2_wsdl4c_operation_get_message(wsdl4c_op, 
      AXIS2_WSDL4C_OUTPUT);
   if(wsdl4c_output_msg)
   {
      axis2_wsdl_msg_ref_t *wsdl_output_msg = NULL;
      axis2_qname_t *ref_qname = NULL;
      axis2_bool_t is_wrappable = AXIS2_FALSE;

      wsdl_output_msg = axis2_wsdl_msg_ref_create(env);
      if(!wsdl_output_msg) 
      {
         return AXIS2_FAILURE;
      }
      AXIS2_WSDL_MSG_REF_SET_DIRECTION(wsdl_output_msg, env, 
         AXIS2_WSDL_MESSAGE_DIRECTION_OUT);
      AXIS2_WSDL_MSG_REF_SET_MSG_LABEL(wsdl_output_msg, env,
         AXIS2_WSDL_MESSAGE_LABEL_OUT_VALUE);
      is_wrappable = axis2_wsdl_pump_find_wrappable(wsdl_pump, env,
         wsdl4c_output_msg);
      ref_qname = axis2_wsdl_pump_generate_reference_qname(wsdl_pump, env,
         wrapped_output_qname, wsdl4c_output_msg, is_wrappable);
      AXIS2_WSDL_MSG_REF_SET_ELEMENT(wsdl_output_msg, env, ref_qname);
      if(ref_qname)
      {
         AXIS2_QNAME_FREE(ref_qname, env);
         ref_qname = NULL;
      }
      AXIS2_WSDL_OP_SET_OUTPUT_MSG(wsdl_op, env, wsdl_output_msg);         
   }

   faults = axis2_wsdl4c_operation_get_faults(wsdl4c_op);
    if(faults)
    {
        size = AXIS2_ARRAY_LIST_SIZE(faults, env);
    }
   for(i = 0; i < size; i++)
   {   
      wsdl4c_fault_msg = AXIS2_ARRAY_LIST_GET(faults, env, i);
      if(wsdl4c_fault_msg)
      {
         axis2_wsdl_fault_ref_t *wsdl_fault_msg = NULL;
         axis2_char_t *fault_name = NULL;
         axis2_qname_t *fault_qname = NULL;
         axis2_qname_t *ref_qname = NULL;
         axis2_bool_t is_wrappable = AXIS2_FALSE;

         wsdl_fault_msg = axis2_wsdl_fault_ref_create(env);
         if(!wsdl_fault_msg) 
         {
            return AXIS2_FAILURE;
         }
         AXIS2_WSDL_MSG_REF_SET_DIRECTION(wsdl_fault_msg, env, 
            AXIS2_WSDL_MESSAGE_DIRECTION_OUT);
         is_wrappable = axis2_wsdl_pump_find_wrappable(wsdl_pump, env,
            wsdl4c_fault_msg);
         fault_name = axis2_wsdl4c_msg_get_name(wsdl4c_fault_msg);
         fault_qname = axis2_qname_create(env, fault_name, NULL, NULL);
         ref_qname = axis2_wsdl_pump_generate_reference_qname(wsdl_pump, env, 
            fault_qname, wsdl4c_fault_msg, is_wrappable);
         AXIS2_WSDL_FAULT_REF_SET_REF(wsdl_fault_msg, env, ref_qname);
         if(ref_qname)
         {
            AXIS2_QNAME_FREE(ref_qname, env);
            ref_qname = NULL;
         }
         AXIS2_WSDL_OP_ADD_OUT_FAULT(wsdl_op, env, wsdl_fault_msg);
      }
   }
    if(faults)
        AXIS2_ARRAY_LIST_FREE(faults, env);
    /* Set the MEP */
   mep = axis2_wsdl11_mep_finder_get_mep(wsdl4c_op, env);
   return AXIS2_WSDL_OP_SET_MSG_EXCHANGE_PATTERN(wsdl_op, env, mep);
}

/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

/**
 * Generates a reference QName
 * @param wsdl4c_message
 * @return
 */
static axis2_qname_t *
axis2_wsdl_pump_generate_reference_qname(axis2_wsdl_pump_t *wsdl_pump,
                                 const axis2_env_t *env,
                                 axis2_qname_t *outer_qname,
                                 void *wsdl4c_msg,
                                 axis2_bool_t is_wrappable)
{
    axis2_wsdl_pump_impl_t *pump_impl = NULL;
   axis2_qname_t *reference_qname = NULL;
   
   AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK(env->error, outer_qname, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK(env->error, wsdl4c_msg, AXIS2_FAILURE);
    pump_impl = AXIS2_INTF_TO_IMPL(wsdl_pump);

   /*if(AXIS2_TRUE == is_wrappable)
   {
      
    }
   else 
   {*/
       int i = 0;
      int no_of_parts = 0;
      
      no_of_parts = axis2_wsdl4c_msg_get_num_parts(wsdl4c_msg);
      for(i = 0; i < no_of_parts; i++)
      {
         void *out_part = axis2_wsdl4c_msg_get_message_part_a_index(wsdl4c_msg, i);
            if(!out_part)
            {
                /* TODO set error code */
                return NULL;
            }
         void *element = axis2_wsdl4c_part_element(out_part);
            int schema_id = axis2_wsdl4c_part_schema_id(out_part);
            void *sp = axis2_wsdl4c_parser_get_schema_parser_a_schema_id(
               pump_impl->parser, schema_id);
            if(!sp)
            {
                /* TODO set error code */
                return NULL;
            }
            axis2_char_t *namespc = axis2_wsdl4c_schema_parser_get_namespace(sp);
         axis2_char_t *name = axis2_wsdl4c_element_get_name(element);
         reference_qname = axis2_qname_create(env, name, namespc, NULL);
         if(!reference_qname) return AXIS2_FAILURE;
      }
            
    /*}*/

    return reference_qname;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_pump_populate_ports(axis2_wsdl_pump_t *wsdl_pump,
                        const axis2_env_t *env,
                        axis2_wsdl_endpoint_t *wsdl_endpoint,
                        void *wsdl4c_binding,
                        char *port_name,
                        char *target_namespc)
{
   axis2_wsdl_pump_impl_t *pump_impl = NULL;
   axis2_qname_t *port_qname = NULL;
   axis2_char_t *binding_name = NULL;
   axis2_qname_t *binding_qname = NULL;
   axis2_wsdl_binding_t *wsdl_binding = NULL;
   
   AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK(env->error, wsdl_endpoint, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK(env->error, wsdl4c_binding, AXIS2_FAILURE);
   pump_impl = AXIS2_INTF_TO_IMPL(wsdl_pump);
   
   port_qname = axis2_qname_create(env, port_name, target_namespc, NULL);
   if(!port_qname) return AXIS2_FAILURE;
    AXIS2_WSDL_ENDPOINT_SET_NAME(wsdl_endpoint, env, port_qname);
   if(port_qname)
   {
      AXIS2_QNAME_FREE(port_qname, env);
      port_qname = NULL;
   }
   binding_name = axis2_wsdl4c_binding_get_name(wsdl4c_binding);
   binding_qname = axis2_qname_create(env, binding_name, target_namespc, NULL);
   wsdl_binding = AXIS2_WSDL_DESC_GET_BINDING(pump_impl->wom_def, env, binding_qname);
    if(binding_qname)
   {
      AXIS2_QNAME_FREE(binding_qname, env);
      binding_qname = NULL;
   }   
   return AXIS2_WSDL_ENDPOINT_SET_BINDING(wsdl_endpoint, env, wsdl_binding);
}

/**
 * This method will fill up the gap of WSDL 1.1 and WSDL 2.0 w.r.t. the
 * bound interface for the Service Component Defined in the WSDL 2.0. Logic
 * being if there exist only one PortType in the WSDL 1.1 file then that
 * will be set as the bound interface of the Service. If more than one
 * Porttype exist in the WSDl 1.1 file this will create a dummy Interface
 * with the available PortTypes and will return that interface so that it
 * will inherit all those interfaces.
 * <p/>
 * Eventually this will have to be fixed using user input since
 *
 tt
e @param service
 * @return wsdl interface
 */
static axis2_wsdl_interface_t *
axis2_wsdl_pump_get_bound_interface(axis2_wsdl_pump_t *wsdl_pump,
                           const axis2_env_t *env,
                           axis2_wsdl_svc_t *wsdl_svc)
{
   axis2_wsdl_pump_impl_t *pump_impl = NULL;
    int no_of_interfaces = 0;
   axis2_hash_t *interfaces = NULL;
   
   AXIS2_ENV_CHECK(env, NULL);
   AXIS2_PARAM_CHECK(env->error, wsdl_svc, NULL);
   pump_impl = AXIS2_INTF_TO_IMPL(wsdl_pump);
   
   interfaces = AXIS2_WSDL_DESC_GET_INTERFACES(pump_impl->wom_def, env);
   no_of_interfaces = axis2_hash_count(interfaces);
    /* Return error code  if there are no interfaces defined as at yet */
   if(0 == no_of_interfaces)
   {
      AXIS2_ERROR_SET(env->error, 
         AXIS2_ERROR_INTERFACES_OR_PORTS_NOT_FOUND_FOR_PARTIALLY_BUILT_WOM,
         AXIS2_FAILURE);
      return NULL;
   }
   /*
    * If there is only one Interface available then return that because
    * normally that interface must be the one to the service should get bound.
    */
   if(1 == no_of_interfaces)
   {
      axis2_hash_index_t *index = NULL;
      void *value = NULL;
      axis2_wsdl_interface_t *interface = NULL;
      
       index = axis2_hash_first (interfaces, env);
      axis2_hash_this(index, NULL, NULL, &value);
      interface = (axis2_wsdl_interface_t *) value;
      return interface;
   }
   if( 1 < no_of_interfaces)
   {
         /* TODO Handle this situation */
   }
   return NULL;
}

