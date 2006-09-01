

/**
* axis2_stub_calculator.h
*
* This file was auto-generated from WSDL for "Calculator|http://localhost/axis/Calculator" service
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
 * axis2_stub_calculator_create
 * create and return the stub with services populated
 * params - env : environment ( mandatory)
 *        - client_home : Axis2/C home ( mandatory )
 *        - endpoint_uri : service endpoint uri( optional ) - if NULL default picked from wsdl used
 */
axis2_stub_t*
axis2_stub_calculator_create (const axis2_env_t *env,
                                axis2_char_t *client_home,
                                axis2_char_t *endpoint_uri);
/**
 * axis2_stub_calculator_populate_services
 * populate the svc in stub with the service and operations
 */
void axis2_stub_calculator_populate_services( axis2_stub_t *stub, const axis2_env_t *env);
/**
 * axis2_stub_calculator_get_endpoint_uri_from_wsdl
 * return the endpoint URI picked from wsdl
 */
axis2_char_t *axis2_stub_calculator_get_endpoint_uri_from_wsdl ( const axis2_env_t *env );


/**
 * auto generated function declaration
 * for "add|http://localhost/axis/Calculator" operation.
 * @param content0
 */

axiom_node_t* axis2_stub_calculator_add( axis2_stub_t *stub, const axis2_env_t *env  ,
                                        axiom_node_t* content0);

/**
 * auto generated function declaration
 * for "sub|http://localhost/axis/Calculator" operation.
 * @param content2
 */

axiom_node_t* axis2_stub_calculator_sub( axis2_stub_t *stub, const axis2_env_t *env  ,
                                        axiom_node_t* content2);

/**
 * auto generated function declaration
 * for "mul|http://localhost/axis/Calculator" operation.
 * @param content4
 */

axiom_node_t* axis2_stub_calculator_mul( axis2_stub_t *stub, const axis2_env_t *env  ,
                                        axiom_node_t* content4);

/**
 * auto generated function declaration
 * for "div|http://localhost/axis/Calculator" operation.
 * @param content6
 */

axiom_node_t* axis2_stub_calculator_div( axis2_stub_t *stub, const axis2_env_t *env  ,
                                        axiom_node_t* content6);

/**
 * auto generated function declaration
 * for "add|http://localhost/axis/Calculator" operation.
 * @param content0
 * @param on_complete callback to handle on complete
 * @param on_error callback to handle on error 
 */


void axis2_stub_calculator_add_start( axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content0,
                                            axis2_status_t ( AXIS2_CALL *on_complete ) (struct axis2_callback *, const axis2_env_t* ) ,
                                            axis2_status_t ( AXIS2_CALL *on_error ) (struct axis2_callback *, const axis2_env_t*, int ) );


/**
 * auto generated function declaration
 * for "sub|http://localhost/axis/Calculator" operation.
 * @param content2
 * @param on_complete callback to handle on complete
 * @param on_error callback to handle on error 
 */


void axis2_stub_calculator_sub_start( axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content2,
                                            axis2_status_t ( AXIS2_CALL *on_complete ) (struct axis2_callback *, const axis2_env_t* ) ,
                                            axis2_status_t ( AXIS2_CALL *on_error ) (struct axis2_callback *, const axis2_env_t*, int ) );


/**
 * auto generated function declaration
 * for "mul|http://localhost/axis/Calculator" operation.
 * @param content4
 * @param on_complete callback to handle on complete
 * @param on_error callback to handle on error 
 */


void axis2_stub_calculator_mul_start( axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content4,
                                            axis2_status_t ( AXIS2_CALL *on_complete ) (struct axis2_callback *, const axis2_env_t* ) ,
                                            axis2_status_t ( AXIS2_CALL *on_error ) (struct axis2_callback *, const axis2_env_t*, int ) );


/**
 * auto generated function declaration
 * for "div|http://localhost/axis/Calculator" operation.
 * @param content6
 * @param on_complete callback to handle on complete
 * @param on_error callback to handle on error 
 */


void axis2_stub_calculator_div_start( axis2_stub_t *stub, const axis2_env_t *env, axiom_node_t* content6,
                                            axis2_status_t ( AXIS2_CALL *on_complete ) (struct axis2_callback *, const axis2_env_t* ) ,
                                            axis2_status_t ( AXIS2_CALL *on_error ) (struct axis2_callback *, const axis2_env_t*, int ) );


