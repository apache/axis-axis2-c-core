

/**
* axis2_stub_interop_test_port_type_doc_service.h
*
* This file was auto-generated from WSDL for "InteropTestPortTypeDocService|http://soapinterop.org" service
* by the Apache Axis2/C version: #axisVersion# #today#
*/

#include <stdio.h>
#include <axiom.h>
#include <axis2_util.h>
#include <axiom_soap.h>
#include <axis2_client.h>
#include <axis2_stub.h>


/* function prototypes - for header file*/
/**
 * axis2_stub_interop_test_port_type_doc_service_create
 * create and return the stub with services populated
 * params - env : environment ( mandatory)
 *        - client_home : Axis2/C home ( mandatory )
 *        - endpoint_uri : service endpoint uri( optional ) - if NULL default picked from wsdl used
 */
axis2_stub_t*
axis2_stub_interop_test_port_type_doc_service_create (const axis2_env_t *env,
                                axis2_char_t *client_home,
                                axis2_char_t *endpoint_uri);
/**
 * axis2_stub_interop_test_port_type_doc_service_populate_services
 * populate the svc in stub with the service and operations
 */
void axis2_stub_interop_test_port_type_doc_service_populate_services( axis2_stub_t *stub, const axis2_env_t *env);
/**
 * axis2_stub_interop_test_port_type_doc_service_get_endpoint_uri_from_wsdl
 * return the endpoint URI picked from wsdl
 */
axis2_char_t *axis2_stub_interop_test_port_type_doc_service_get_endpoint_uri_from_wsdl ( const axis2_env_t *env );


/**
 * auto generated function declaration
 * for "echoString|http://soapinterop.org" operation.
 * @param content0
 */

axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_string( axis2_stub_t *stub, const axis2_env_t *env  ,
                                        axiom_node_t* content0);

/**
 * auto generated function declaration
 * for "echoStringArray|http://soapinterop.org" operation.
 * @param content2
 */

axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_string_array( axis2_stub_t *stub, const axis2_env_t *env  ,
                                        axiom_node_t* content2);

/**
 * auto generated function declaration
 * for "echoInteger|http://soapinterop.org" operation.
 * @param content4
 */

axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_integer( axis2_stub_t *stub, const axis2_env_t *env  ,
                                        axiom_node_t* content4);

/**
 * auto generated function declaration
 * for "echoIntegerArray|http://soapinterop.org" operation.
 * @param content6
 */

axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_integer_array( axis2_stub_t *stub, const axis2_env_t *env  ,
                                        axiom_node_t* content6);

/**
 * auto generated function declaration
 * for "echoFloat|http://soapinterop.org" operation.
 * @param content8
 */

axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_float( axis2_stub_t *stub, const axis2_env_t *env  ,
                                        axiom_node_t* content8);

/**
 * auto generated function declaration
 * for "echoFloatArray|http://soapinterop.org" operation.
 * @param content10
 */

axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_float_array( axis2_stub_t *stub, const axis2_env_t *env  ,
                                        axiom_node_t* content10);

/**
 * auto generated function declaration
 * for "echoStruct|http://soapinterop.org" operation.
 * @param content12
 */

axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_struct( axis2_stub_t *stub, const axis2_env_t *env  ,
                                        axiom_node_t* content12);

/**
 * auto generated function declaration
 * for "echoStructArray|http://soapinterop.org" operation.
 * @param content14
 */

axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_struct_array( axis2_stub_t *stub, const axis2_env_t *env  ,
                                        axiom_node_t* content14);

/**
 * auto generated function declaration
 * for "echoVoid|http://soapinterop.org" operation.
 * @param content16
 */

axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_void( axis2_stub_t *stub, const axis2_env_t *env  ,
                                        axiom_node_t* content16);

/**
 * auto generated function declaration
 * for "echoBase64|http://soapinterop.org" operation.
 * @param content18
 */

axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_base64( axis2_stub_t *stub, const axis2_env_t *env  ,
                                        axiom_node_t* content18);

/**
 * auto generated function declaration
 * for "echoDate|http://soapinterop.org" operation.
 * @param content20
 */

axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_date( axis2_stub_t *stub, const axis2_env_t *env  ,
                                        axiom_node_t* content20);

/**
 * auto generated function declaration
 * for "echoHexBinary|http://soapinterop.org" operation.
 * @param content22
 */

axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_hex_binary( axis2_stub_t *stub, const axis2_env_t *env  ,
                                        axiom_node_t* content22);

/**
 * auto generated function declaration
 * for "echoDecimal|http://soapinterop.org" operation.
 * @param content24
 */

axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_decimal( axis2_stub_t *stub, const axis2_env_t *env  ,
                                        axiom_node_t* content24);

/**
 * auto generated function declaration
 * for "echoBoolean|http://soapinterop.org" operation.
 * @param content26
 */

axiom_node_t* axis2_stub_interop_test_port_type_doc_service_echo_boolean( axis2_stub_t *stub, const axis2_env_t *env  ,
                                        axiom_node_t* content26);

/**
 * auto generated function declaration
 * for "echoString|http://soapinterop.org" operation.
 * @param content0
 * @param on_complete callback to handle on complete
 * @param on_error callback to handle on error 
 */


void axis2_stub_interop_test_port_type_doc_service_echo_string_start( axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content0,
                                            axis2_status_t ( AXIS2_CALL *on_complete ) (struct axis2_callback *, const axis2_env_t* ) ,
                                            axis2_status_t ( AXIS2_CALL *on_error ) (struct axis2_callback *, const axis2_env_t*, int ) );


/**
 * auto generated function declaration
 * for "echoStringArray|http://soapinterop.org" operation.
 * @param content2
 * @param on_complete callback to handle on complete
 * @param on_error callback to handle on error 
 */


void axis2_stub_interop_test_port_type_doc_service_echo_string_array_start( axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content2,
                                            axis2_status_t ( AXIS2_CALL *on_complete ) (struct axis2_callback *, const axis2_env_t* ) ,
                                            axis2_status_t ( AXIS2_CALL *on_error ) (struct axis2_callback *, const axis2_env_t*, int ) );


/**
 * auto generated function declaration
 * for "echoInteger|http://soapinterop.org" operation.
 * @param content4
 * @param on_complete callback to handle on complete
 * @param on_error callback to handle on error 
 */


void axis2_stub_interop_test_port_type_doc_service_echo_integer_start( axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content4,
                                            axis2_status_t ( AXIS2_CALL *on_complete ) (struct axis2_callback *, const axis2_env_t* ) ,
                                            axis2_status_t ( AXIS2_CALL *on_error ) (struct axis2_callback *, const axis2_env_t*, int ) );


/**
 * auto generated function declaration
 * for "echoIntegerArray|http://soapinterop.org" operation.
 * @param content6
 * @param on_complete callback to handle on complete
 * @param on_error callback to handle on error 
 */


void axis2_stub_interop_test_port_type_doc_service_echo_integer_array_start( axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content6,
                                            axis2_status_t ( AXIS2_CALL *on_complete ) (struct axis2_callback *, const axis2_env_t* ) ,
                                            axis2_status_t ( AXIS2_CALL *on_error ) (struct axis2_callback *, const axis2_env_t*, int ) );


/**
 * auto generated function declaration
 * for "echoFloat|http://soapinterop.org" operation.
 * @param content8
 * @param on_complete callback to handle on complete
 * @param on_error callback to handle on error 
 */


void axis2_stub_interop_test_port_type_doc_service_echo_float_start( axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content8,
                                            axis2_status_t ( AXIS2_CALL *on_complete ) (struct axis2_callback *, const axis2_env_t* ) ,
                                            axis2_status_t ( AXIS2_CALL *on_error ) (struct axis2_callback *, const axis2_env_t*, int ) );


/**
 * auto generated function declaration
 * for "echoFloatArray|http://soapinterop.org" operation.
 * @param content10
 * @param on_complete callback to handle on complete
 * @param on_error callback to handle on error 
 */


void axis2_stub_interop_test_port_type_doc_service_echo_float_array_start( axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content10,
                                            axis2_status_t ( AXIS2_CALL *on_complete ) (struct axis2_callback *, const axis2_env_t* ) ,
                                            axis2_status_t ( AXIS2_CALL *on_error ) (struct axis2_callback *, const axis2_env_t*, int ) );


/**
 * auto generated function declaration
 * for "echoStruct|http://soapinterop.org" operation.
 * @param content12
 * @param on_complete callback to handle on complete
 * @param on_error callback to handle on error 
 */


void axis2_stub_interop_test_port_type_doc_service_echo_struct_start( axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content12,
                                            axis2_status_t ( AXIS2_CALL *on_complete ) (struct axis2_callback *, const axis2_env_t* ) ,
                                            axis2_status_t ( AXIS2_CALL *on_error ) (struct axis2_callback *, const axis2_env_t*, int ) );


/**
 * auto generated function declaration
 * for "echoStructArray|http://soapinterop.org" operation.
 * @param content14
 * @param on_complete callback to handle on complete
 * @param on_error callback to handle on error 
 */


void axis2_stub_interop_test_port_type_doc_service_echo_struct_array_start( axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content14,
                                            axis2_status_t ( AXIS2_CALL *on_complete ) (struct axis2_callback *, const axis2_env_t* ) ,
                                            axis2_status_t ( AXIS2_CALL *on_error ) (struct axis2_callback *, const axis2_env_t*, int ) );


/**
 * auto generated function declaration
 * for "echoVoid|http://soapinterop.org" operation.
 * @param content16
 * @param on_complete callback to handle on complete
 * @param on_error callback to handle on error 
 */


void axis2_stub_interop_test_port_type_doc_service_echo_void_start( axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content16,
                                            axis2_status_t ( AXIS2_CALL *on_complete ) (struct axis2_callback *, const axis2_env_t* ) ,
                                            axis2_status_t ( AXIS2_CALL *on_error ) (struct axis2_callback *, const axis2_env_t*, int ) );


/**
 * auto generated function declaration
 * for "echoBase64|http://soapinterop.org" operation.
 * @param content18
 * @param on_complete callback to handle on complete
 * @param on_error callback to handle on error 
 */


void axis2_stub_interop_test_port_type_doc_service_echo_base64_start( axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content18,
                                            axis2_status_t ( AXIS2_CALL *on_complete ) (struct axis2_callback *, const axis2_env_t* ) ,
                                            axis2_status_t ( AXIS2_CALL *on_error ) (struct axis2_callback *, const axis2_env_t*, int ) );


/**
 * auto generated function declaration
 * for "echoDate|http://soapinterop.org" operation.
 * @param content20
 * @param on_complete callback to handle on complete
 * @param on_error callback to handle on error 
 */


void axis2_stub_interop_test_port_type_doc_service_echo_date_start( axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content20,
                                            axis2_status_t ( AXIS2_CALL *on_complete ) (struct axis2_callback *, const axis2_env_t* ) ,
                                            axis2_status_t ( AXIS2_CALL *on_error ) (struct axis2_callback *, const axis2_env_t*, int ) );


/**
 * auto generated function declaration
 * for "echoHexBinary|http://soapinterop.org" operation.
 * @param content22
 * @param on_complete callback to handle on complete
 * @param on_error callback to handle on error 
 */


void axis2_stub_interop_test_port_type_doc_service_echo_hex_binary_start( axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content22,
                                            axis2_status_t ( AXIS2_CALL *on_complete ) (struct axis2_callback *, const axis2_env_t* ) ,
                                            axis2_status_t ( AXIS2_CALL *on_error ) (struct axis2_callback *, const axis2_env_t*, int ) );


/**
 * auto generated function declaration
 * for "echoDecimal|http://soapinterop.org" operation.
 * @param content24
 * @param on_complete callback to handle on complete
 * @param on_error callback to handle on error 
 */


void axis2_stub_interop_test_port_type_doc_service_echo_decimal_start( axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content24,
                                            axis2_status_t ( AXIS2_CALL *on_complete ) (struct axis2_callback *, const axis2_env_t* ) ,
                                            axis2_status_t ( AXIS2_CALL *on_error ) (struct axis2_callback *, const axis2_env_t*, int ) );


/**
 * auto generated function declaration
 * for "echoBoolean|http://soapinterop.org" operation.
 * @param content26
 * @param on_complete callback to handle on complete
 * @param on_error callback to handle on error 
 */


void axis2_stub_interop_test_port_type_doc_service_echo_boolean_start( axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content26,
                                            axis2_status_t ( AXIS2_CALL *on_complete ) (struct axis2_callback *, const axis2_env_t* ) ,
                                            axis2_status_t ( AXIS2_CALL *on_error ) (struct axis2_callback *, const axis2_env_t*, int ) );


