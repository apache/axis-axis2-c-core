
    /**
     * axis2_Calculator.c 
     *
     * This file was auto-generated from WSDL 
     * by the Apache Axis2/Java version: #axisVersion# #today#
     * axis2_Calculator Axis2/C skeleton for the axisService
     */

     #include "axis2_Calculator.h"

     

       
        /**
         * Auto generated function definition
         
          * @param param0
         
         */
        axis2_addResponse20_t*axis2_Calculator_add (const axis2_env_t* env  ,axis2_add_t* param0 )

        {
          /* Todo fill this with the necessary business logic */
          axis2_addResponse_t* add_out = NULL;
         axis2_add_t* add_in = param0;
          axis2_addRequest_t* add_req = NULL;
          axis2_addResponse20_t* add_res = NULL;

          int ret_val = 0;

          int val1 = 0;
          int val2 = 0;


     add_req = AXIS2_ADD_GET_ADD( add_in, env );
         
     val1 = AXIS2_ADDREQUEST_GET_IN0 ( add_req, env );
          val2 = AXIS2_ADDREQUEST_GET_IN1 ( add_req, env );

          ret_val = val1 + val2;

          add_out = axis2_addResponse_create ( env );
     AXIS2_ADDRESPONSE_SET_ADDRETURN ( add_out, env, ret_val );
          
     add_res = axis2_addResponse20_create ( env );
     AXIS2_ADDRESPONSE20_SET_ADDRESPONSE ( add_res, env, add_out );
     
          return add_res;
        }
     

       
        /**
         * Auto generated function definition
         
          * @param param2
         
         */
        axis2_divResponse16_t*axis2_Calculator_div (const axis2_env_t* env  ,axis2_div_t* param2 )

        {
          /* Todo fill this with the necessary business logic */
          axis2_addResponse_t* add_out = NULL;
         axis2_add_t* add_in = param2;
          axis2_addRequest_t* add_req = NULL;
          axis2_addResponse20_t* add_res = NULL;

          int ret_val = 0;

          int val1 = 0;
          int val2 = 0;


     add_req = AXIS2_ADD_GET_ADD( add_in, env );
         
     val1 = AXIS2_ADDREQUEST_GET_IN0 ( add_req, env );
          val2 = AXIS2_ADDREQUEST_GET_IN1 ( add_req, env );


          ret_val = val1 / val2;

          add_out = axis2_addResponse_create ( env );
     AXIS2_ADDRESPONSE_SET_ADDRETURN ( add_out, env, ret_val );
          
     add_res = axis2_addResponse20_create ( env );
     AXIS2_ADDRESPONSE20_SET_ADDRESPONSE ( add_res, env, add_out );
     
          return add_res;
        }
     

       
        /**
         * Auto generated function definition
         
          * @param param4
         
         */
        axis2_subResponse12_t*axis2_Calculator_sub (const axis2_env_t* env  ,axis2_sub_t* param4 )

        {
          /* Todo fill this with the necessary business logic */
          axis2_addResponse_t* add_out = NULL;
         axis2_add_t* add_in = param4;
          axis2_addRequest_t* add_req = NULL;
          axis2_addResponse20_t* add_res = NULL;

          int ret_val = 0;

          int val1 = 0;
          int val2 = 0;


     add_req = AXIS2_ADD_GET_ADD( add_in, env );
         
     val1 = AXIS2_ADDREQUEST_GET_IN0 ( add_req, env );
          val2 = AXIS2_ADDREQUEST_GET_IN1 ( add_req, env );


          ret_val = val1 - val2;

          add_out = axis2_addResponse_create ( env );
     AXIS2_ADDRESPONSE_SET_ADDRETURN ( add_out, env, ret_val );
          
     add_res = axis2_addResponse20_create ( env );
     AXIS2_ADDRESPONSE20_SET_ADDRESPONSE ( add_res, env, add_out );
     
          return add_res;
        }
     

       
        /**
         * Auto generated function definition
         
          * @param param6
         
         */
        axis2_mulResponse14_t*axis2_Calculator_mul (const axis2_env_t* env  ,axis2_mul_t* param6 )

        {
          /* Todo fill this with the necessary business logic */
          axis2_addResponse_t* add_out = NULL;
         axis2_add_t* add_in = param6;
          axis2_addRequest_t* add_req = NULL;
          axis2_addResponse20_t* add_res = NULL;

          int ret_val = 0;

          int val1 = 0;
          int val2 = 0;


     add_req = AXIS2_ADD_GET_ADD( add_in, env );
         
     val1 = AXIS2_ADDREQUEST_GET_IN0 ( add_req, env );
          val2 = AXIS2_ADDREQUEST_GET_IN1 ( add_req, env );


          ret_val = val1 * val2;

          add_out = axis2_addResponse_create ( env );
     AXIS2_ADDRESPONSE_SET_ADDRETURN ( add_out, env, ret_val );
          
     add_res = axis2_addResponse20_create ( env );
     AXIS2_ADDRESPONSE20_SET_ADDRESPONSE ( add_res, env, add_out );
     
          return add_res;
        }
     
