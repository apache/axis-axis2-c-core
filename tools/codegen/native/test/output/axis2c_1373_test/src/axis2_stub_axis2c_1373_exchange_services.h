

        /**
        * output/axis2c_1373_test/src/axis2_stub_axis2c_1373_exchange_services.h
        *
        * This file was auto-generated from WSDL for "axis2c_1373_exchange_services|http://localhost/axis/axis2c_1373_exchange_services" service
        * by the Apache Axis2/C Native version: 1.0.0  Built on : Nov 21, 2025 (08:11:18 HST)
        */

        #ifndef AXIS2_STUB_AXIS2C_1373_EXCHANGE_SERVICES_H
        #define AXIS2_STUB_AXIS2C_1373_EXCHANGE_SERVICES_H

        #include <stdio.h>
        #include <axiom.h>
        #include <axutil_utils.h>
        #include <axiom_soap.h>
        #include <axis2_client.h>
        #include <axis2_stub.h>

       
         #include "adb_add.h"
        
         #include "adb_addResponse.h"
        
         #include "adb_div.h"
        
         #include "adb_divResponse.h"
        
         #include "adb_sub.h"
        
         #include "adb_subResponse.h"
        
         #include "adb_mul.h"
        
         #include "adb_mulResponse.h"
        

	#ifdef __cplusplus
	extern "C" {
	#endif

        /***************** function prototypes - for header file *************/
        /**
         * axis2_stub_create_axis2c_1373_exchange_services
         * Create and return the stub with services populated
         * @param env Environment ( mandatory)
         * @param client_home Axis2/C home ( mandatory )
         * @param endpoint_uri Service endpoint uri( optional ) - if NULL default picked from WSDL used
         * @return Newly created stub object
         */
        axis2_stub_t*
        axis2_stub_create_axis2c_1373_exchange_services(const axutil_env_t *env,
                                        const axis2_char_t *client_home,
                                        const axis2_char_t *endpoint_uri);

        /**
         * axis2_stub_populate_services_for_axis2c_1373_exchange_services
         * populate the svc in stub with the service and operation QNames
         * @param stub The stub
         * @param env environment ( mandatory)
         */
        void axis2_stub_populate_services_for_axis2c_1373_exchange_services( axis2_stub_t *stub, const axutil_env_t *env);

        /**
         * axis2_stub_get_endpoint_uri_of_axis2c_1373_exchange_services
         * Return the endpoint URI
         * @param env environment ( mandatory)
         * @return endpoint URI
         */
        axis2_char_t *
        axis2_stub_get_endpoint_uri_of_axis2c_1373_exchange_services(const axutil_env_t *env);

        /**
         * Auto generated function definition signature
         * for "add" operation.
         * @param stub The stub (axis2_stub_t)
         * @param env environment ( mandatory)
         * @param _add of the adb_add_t
         *
         * @return adb_addResponse_t*
         */
        adb_addResponse_t* 
        axis2_stub_add( axis2_stub_t *stub, const axutil_env_t *env,
                                  adb_add_t* _add);

        /**
         * Auto generated function definition signature
         * for "sub" operation.
         * @param stub The stub (axis2_stub_t)
         * @param env environment ( mandatory)
         * @param _sub of the adb_sub_t
         *
         * @return adb_subResponse_t*
         */
        adb_subResponse_t* 
        axis2_stub_sub( axis2_stub_t *stub, const axutil_env_t *env,
                                  adb_sub_t* _sub);

        /**
         * Auto generated function definition signature
         * for "mul" operation.
         * @param stub The stub (axis2_stub_t)
         * @param env environment ( mandatory)
         * @param _mul of the adb_mul_t
         *
         * @return adb_mulResponse_t*
         */
        adb_mulResponse_t* 
        axis2_stub_mul( axis2_stub_t *stub, const axutil_env_t *env,
                                  adb_mul_t* _mul);

        /**
         * Auto generated function definition signature
         * for "div" operation.
         * @param stub The stub (axis2_stub_t)
         * @param env environment ( mandatory)
         * @param _div of the adb_div_t
         *
         * @return adb_divResponse_t*
         */
        adb_divResponse_t* 
        axis2_stub_div( axis2_stub_t *stub, const axutil_env_t *env,
                                  adb_div_t* _div);

	#ifdef __cplusplus
	}
	#endif
	#endif /* AXIS2_STUB_AXIS2C_1373_EXCHANGE_SERVICES_H */
