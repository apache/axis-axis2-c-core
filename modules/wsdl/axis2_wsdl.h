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
 
#ifndef AXIS2_WSDL_H
#define AXIS2_WSDL_H

/**
 * @file axis2_axis2_description.h
 * @brief axis2 DESCRIPTION
 */
 
#include <axis2.h>

#ifdef __cplusplus
extern "C" 
{
#endif
    
/*********************************** Constansts********************************/
/**
 * Field WSDL2_0_NAMESPACE
 */
#define AXIS2_WSDL2_0_NAMESPACE "http://www.w3.org/2004/03/wsdl"

/**
 * Field WSDL1_1_NAMESPACE
 */
#define AXIS2_WSDL1_1_NAMESPACE "http://schemas.xmlsoap.org/wsdl/"


/**
 * Field WSDL_MESSAGE_DIRECTION_IN
 */
#define AXIS2_WSDL_MESSAGE_DIRECTION_IN "in"


/**
 * Field WSDL_MESSAGE_DIRECTION_OUT
 */
#define AXIS2_WSDL_MESSAGE_DIRECTION_OUT "out"


/*********************Message Exchange Pattern Constants***********************/


/**
 * Field MEP_URI_IN_ONLY
 */
#define AXIS2_MEP_URI_IN_ONLY "http://www.w3.org/2004/08/wsdl/in-only"

#define AXIS2_MEP_CONSTANT_IN_ONLY 10

/**
 * Field MEP_URI_ROBUST_IN_ONLY
 */
#define AXIS2_MEP_URI_ROBUST_IN_ONLY "http://www.w3.org/2004/08/wsdl/robust-in-only"

#define AXIS2_MEP_CONSTANT_ROBUST_IN_ONLY 11

/**
 * Field MEP_URI_IN_OUT
 */
#define AXIS2_MEP_URI_IN_OUT "http://www.w3.org/2004/08/wsdl/in-out"

#define AXIS2_MEP_CONSTANT_IN_OUT 12

/**
 * Field MEP_URI_IN_OPTIONAL_OUT
 */
#define AXIS2_MEP_URI_IN_OPTIONAL_OUT "http://www.w3.org/2004/08/wsdl/in-opt-out"

#define AXIS2_MEP_CONSTANT_IN_OPTIONAL_OUT 13

/**
 * Field MEP_URI_OUT_ONLY
 */
#define AXIS2_MEP_URI_OUT_ONLY "http://www.w3.org/2004/08/wsdl/out-only"

#define AXIS2_MEP_CONSTANT_OUT_ONLY 14

/**
 * Field MEP_URI_ROBUST_OUT_ONLY
 */
#define AXIS2_MEP_URI_ROBUST_OUT_ONLY "http://www.w3.org/2004/08/wsdl/robust-out-only"

#define AXIS2_MEP_CONSTANT_ROBUST_OUT_ONLY 15

/**
 * Field MEP_URI_OUT_IN
 */
#define AXIS2_MEP_URI_OUT_IN "http://www.w3.org/2004/08/wsdl/out-in"

#define AXIS2_MEP_CONSTANT_OUT_IN 16

/**
 * Field MEP_URI_OUT_OPTIONL_IN
 */
#define AXIS2_MEP_URI_OUT_OPTIONAL_IN "http://www.w3.org/2004/08/wsdl/out-opt-in"

#define AXIS2_MEP_CONSTANT_OUT_OPTIONAL_IN 17


#define AXIS2_MEP_CONSTANT_INVALID -1

/******************************Message Labels**********************************/

/**
 * Constant to represent the message label "In" which is used by the
 * following WSDL 2.0 defined MEPs: In-Only, Robust In-Only, In-Out,
 * In-Optional-Out, Out-In, Out-Optional-In.
 */
#define AXIS2_MESSAGE_LABEL_IN 0

#define AXIS2_WSDL_MESSAGE_LABEL_IN_VALUE "In"

/**
 * Constant to represent the message label "Out" which is used by the
 * following WSDL 2.0 defined MEPs: In-Out, In-Optional-Out, Out-Only,
 * Robust Out-Only, Out-In, Out-Optional-In.
 */
#define AXIS2_MESSAGE_LABEL_OUT 1

#define AXIS2_WSDL_MESSAGE_LABEL_OUT_VALUE "Out"

/**
 *
 */
#define AXIS2_WSDL_USE_LITERAL "literal"
#define AXIS2_WSDL_USE_ENCODED "encoded"

/********************wsdl extensible constants*********************************/
/**
 * The Type name for the SOAP Address defined in the Port/Endpoint
 */
#define AXIS2_SOAP_11_ADDRESS "http://schemas.xmlsoap.org/wsdl/soap/"
#define AXIS2_SOAP_12_ADDRESS "http://schemas.xmlsoap.org/wsdl/soap12/"


#define AXIS2_SOAP_11_OP "http://schemas.xmlsoap.org/wsdl/soap/"
 #define AXIS2_SOAP_12_OP "http://schemas.xmlsoap.org/wsdl/soap12/"

#define AXIS2_SCHEMA "http://www.w3.org/2001/XMLSchema"

#define AXIS2_SOAP_11_BODY "http://schemas.xmlsoap.org/wsdl/soap/"
#define AXIS2_SOAP_12_BODY "http://schemas.xmlsoap.org/wsdl/soap12/"

#define AXIS2_SOAP_11_HEADER "http://schemas.xmlsoap.org/wsdl/soap/"
#define AXIS2_SOAP_12_HEADER "http://schemas.xmlsoap.org/wsdl/soap12/"

#define AXIS2_SOAP_11_BINDING "http://schemas.xmlsoap.org/wsdl/soap/"
#define AXIS2_SOAP_12_BINDING "http://schemas.xmlsoap.org/wsdl/soap12/"

/*********************************** Constants*********************************/	
	
	
#ifdef __cplusplus
}
#endif

#endif /* AXIS2_WSDL_H */
