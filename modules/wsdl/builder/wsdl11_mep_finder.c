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

axis2_char_t *AXIS2_CALL
axis2_wom_builder_wsdl11_mep_finder_get_mep(wsdlOperation operation) 
{
    wsdlOperationType operation_type = operation.type;
    if(operation_type == AXIS2_REQUEST_RESPONSE_OPERATION)
    {
        return AXIS2_MEP_URI_IN_OUT;
    }
    if (operation_type ==  AXIS2_ONE_WAY_OPERATION)
        return AXIS2_MEP_URI_IN_ONLY;

    if (operationType == AXIS2_NOTIFICATION_OPERATION)
        return AXIS2_MEP_URI_OUT_ONLY;

    if (operationType == AXIS2_SOLICIT_RESPONSE_OPERATION)
        return AXIS2_MEP_URI_OUT_IN;

    AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_MEP_CANNOT_DETERMINE_MEP,
        AXIS2_FAILURE);
    return NULL;
}
