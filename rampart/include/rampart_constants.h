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


#ifndef RAMPART_CONSTANTS_H
#define RAMPART_CONSTANTS_H

#ifdef __cplusplus
extern "C"
{
#endif

/*Default values*/

/* rampart element names*/
#define RAMPART_SECURITY "Security"
#define RAMPART_SECURITY_USERNAMETOKEN "UsernameToken"
#define RAMPART_SECURITY_USERNAMETOKEN_USERNAME "Username"
#define RAMPART_SECURITY_USERNAMETOKEN_PASSWORD "Password"
#define RAMPART_SECURITY_USERNAMETOKEN_CREATED "Created"
#define RAMPART_SECURITY_USERNAMETOKEN_NONCE "Nonce"
#define RAMPART_SECURITY_USERNAMETOKEN_PASSWORD_ATTR_TYPE "Type"
#define RAMPART_SECURITY_TIMESTAMP "Timestamp"
#define RAMPART_SECURITY_TIMESTAMP_CREATED "Created"
#define RAMPART_SECURITY_TIMESTAMP_EXPIRES "Expires"
    
/*Rampart URIs*/
#define RAMPART_WSSE "wsse"
#define RAMPART_WSSE_XMLNS "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd"
#define RAMPART_WSU "wsu"
#define RAMPART_WSU_XMLNS "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd"
#define RAMPART_PASSWORD_DIGEST_URI "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest"  

#define RAMPART_PASSWORD_TEXT_URI "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordText" 

/* Inflow outflow security parameter    */
#define RAMPART_INFLOW_SECURITY  "InflowSecurity"
#define RAMPART_OUTFLOW_SECURITY "OutflowSecurity"
#define RAMPART_ACTION "action"
#define RAMPART_ACTION_ITEMS "items"
#define RAMPART_USER "user"
#define RAMPART_PASSWORD_TYPE "passwordType"
#define RAMPART_PASSWORD_TEXT "passwordText"
#define RAMPART_PASSWORD_DIGEST "passwordDigest"
#define RAMPART_ACTION_ITEMS_USERNAMETOKEN "UsernameToken"
#define RAMPART_ACTION_ITEMS_TIMESTAMP "Timestamp"
#define RAMPART_ACTION_ITEMS_ENCRYPT "Encrypt"
#define RAMPART_ACTION_ITEMS_SIGNATURE "Signature"
#define RAMPART_ACTION_PW_CALLBACK_CLASS "passwordCallbackClass"

/*Dynamically set values*/
#define RAMPART_DYN_PASSWORD "password"

#ifdef __cplusplus
}
#endif

#endif /* RAMPART_CONSTANTS_H*/
