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


#ifndef RAMPART_CONSTANTS_H
#define RAMPART_CONSTANTS_H

/**
  * @file rampart_constants.h
  * @brief Holds constants for rampart
  */

#ifdef __cplusplus
extern "C"
{
#endif

/*Default values*/
#define RAMPART_DEFAULT_KT_ALGO OXS_DEFAULT_KT_ALGO_HREF
#define RAMPART_STR_DEFAULT OXS_STR_DEFAULT

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
/*
<?xml version="1.0" encoding="UTF-8"?>
<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema" elementFormDefault="qualified" attributeFormDefault="unqualified">
    <xs:element name="action">
        <xs:annotation>
            <xs:documentation>Outflow security 'action' configuration</xs:documentation>
        </xs:annotation>
        <xs:complexType>
            <xs:sequence>
                <xs:element name="items" type="xs:string"/>
                <xs:element name="user" type="xs:string"/>
                <xs:element name="passwordCallbackClass" type="xs:string" minOccurs="0"/>
                <xs:element name="signaturePropFile" type="xs:string" minOccurs="0"/>
                <xs:element name="encryptionPropFile" type="xs:string" minOccurs="0"/>
                <xs:element name="decryptionPropFile" type="xs:string" minOccurs="0"/>
                <xs:element name="signatureKeyIdentifier" type="xs:string" minOccurs="0"/>
                <xs:element name="encryptionKeyIdentifier" type="xs:string" minOccurs="0"/>
                <xs:element name="encryptionUser" type="xs:string" minOccurs="0"/>
                <xs:element name="signatureParts" type="xs:string" minOccurs="0"/>
                <xs:element name="encryptionParts" type="xs:string" minOccurs="0"/>
                <xs:element name="optimizeParts" type="xs:string" minOccurs="0"/>
                <xs:element name="encryptionSymAlgorithm" type="xs:string" minOccurs="0"/>
                <xs:element name="EmbeddedKeyCallbackClass" type="xs:string" minOccurs="0"/>
                <xs:element name="encryptionKeyTransportAlgorithm" type="xs:string" minOccurs="0"/>
                <xs:element name="EmbeddedKeyName" type="xs:string" minOccurs="0"/>
                <xs:element name="timeToLive" type="xs:string" minOccurs="0"/>
            </xs:sequence>
        </xs:complexType>
    </xs:element>
</xs:schema>
*/
#define RAMPART_INFLOW_SECURITY                     "InflowSecurity"
#define RAMPART_OUTFLOW_SECURITY                    "OutflowSecurity"
#define RAMPART_PASSWORD_TEXT                       "passwordText"
#define RAMPART_PASSWORD_DIGEST                     "passwordDigest"
#define RAMPART_ACTION                              "action"
/*These are the parameters of actions in the axis2.xml file*/
#define RAMPART_ACTION_ITEMS                        "items"
#define RAMPART_ACTION_ITEMS_USERNAMETOKEN          "UsernameToken"
#define RAMPART_ACTION_ITEMS_TIMESTAMP              "Timestamp"
#define RAMPART_ACTION_ITEMS_ENCRYPT                "Encrypt"
#define RAMPART_ACTION_ITEMS_SIGNATURE              "Signature"
#define RAMPART_ACTION_PASSWORD_TYPE                "passwordType"
#define RAMPART_ACTION_USER                         "user"
#define RAMPART_ACTION_PW_CALLBACK_CLASS            "passwordCallbackClass"
#define RAMPART_ACTION_AUTHN_MODULE_NAME            "authnModuleName"
#define RAMPART_ACTION_SIGNATURE_PROP_FILE          "signaturePropFile"
#define RAMPART_ACTION_ENCRYPTION_PROP_FILE         "encryptionPropFile"
#define RAMPART_ACTION_DECRYPTION_PROP_FILE         "decryptionPropFile"
#define RAMPART_ACTION_SIGNATURE_KEY_IDENTIFIER     "signatureKeyIdentifier"
#define RAMPART_ACTION_ENCRYPTION_KEY_IDENTIFIER    "encryptionKeyIdentifier"
#define RAMPART_ACTION_ENCRYPTION_USER              "encryptionUser"
#define RAMPART_ACTION_SIGNATURE_PARTS              "signatureParts"
#define RAMPART_ACTION_ENCRYPTION_PARTS             "encryptionParts"
#define RAMPART_ACTION_OPTIMIZE_PARTS               "optimizeParts"
#define RAMPART_ACTION_ENCRYPTION_SYM_ALGORITHM     "encryptionSymAlgorithm"
#define RAMPART_ACTION_EMBEDDED_KEY_CALLBACK_CLASS  "EmbeddedKeyCallbackClass"
#define RAMPART_ACTION_ENCRYPTION_KEY_TRANSFORM_ALGORITHM "encryptionKeyTransportAlgorithm"
#define RAMPART_ACTION_EMBEDDED_KEY_NAME            "EmbeddedKeyName"
#define RAMPART_ACTION_TIME_TO_LIVE                 "timeToLive"
#define RAMPART_ACTION_KEY_BUF                      "keyBuffer"


/************fault codes***************/
#define RAMPART_FAULT_UNSUPPORTED_SECURITY_TOKEN    "wsse:UnsupportedSecurityToken"
#define RAMPART_FAULT_UNSUPPORTED_ALGORITHM         "wsse:UnsupportedAlgorithm"
#define RAMPART_FAULT_INVALID_SECURITY              "wsse:InvalidSecurity"
#define RAMPART_FAULT_INVALID_SECURITY_TOKEN        "wsse:InvalidSecurityToken"
#define RAMPART_FAULT_FAILED_AUTHENTICATION         "wsse:FailedAuthentication"
#define RAMPART_FAULT_FAILED_CHECK                  "wsse:FailedCheck"
#define RAMPART_FAULT_SECURITY_TOKEN_UNAVAILABLE    "wsse:SecurityTokenUnavailable"

/***********fault related strings*********/
#define RAMPART_FAULT_IN_TIMESTAMP             "wsse:Timestamp"
#define RAMPART_FAULT_IN_USERNAMETOKEN         "wsse:UsernameToken"
#define RAMPART_FAULT_IN_ENCRYPTED_KEY         "xenc:EncryptedKey"
#define RAMPART_FAULT_IN_ENCRYPTED_DATA        "xenc:EncryptedData"

/*Dynamically set values*/
#define RAMPART_ACTION_PASSWORD "password"
#define RAMPART_ACTION_ENC_USER_PASSWORD "encUserPassword"
#define RAMPART_CALLBACK_SPECIFIC_PROPERTY "callbackSpecificProperty"

/*Security processed results*/
#define RAMPART_SECURITY_PROCESSED_RESULTS "SecurityProcessedResults"
#define RAMPART_SPR_UT_USERNAME "SPR_UT_username"
#define RAMPART_SPR_UT_CREATED "SPR_UT_created"
#define RAMPART_SPR_UT_NONCE "SPR_UT_nonce"
#define RAMPART_SPR_UT_PASSWORD_TYPE "SPR_UT_passwordType"
#define RAMPART_SPR_TS_CREATED "SPR_TS_created"
#define RAMPART_SPR_TS_EXPIRES "SPR_TS_expires"
#define RAMPART_SPR_UT_CHECKED "SPR_UT_Checked"
#define RAMPART_SPR_TS_CHECKED "SPR_TS_Checked"
#define RAMPART_SPR_ENC_CHECKED "SPR_ENC_Checked"

#define RAMPART_YES "YES"
#define RAMPART_NO "NO"

#define RAMPART_STR_DIRECT_REFERENCE    OXS_STR_DIRECT_REFERENCE 
#define RAMPART_STR_KEY_IDENTIFIER      OXS_STR_KEY_IDENTIFIER
#define RAMPART_STR_EMBEDDED            OXS_STR_EMBEDDED
#define RAMPART_STR_ISSUER_SERIAL       OXS_STR_ISSUER_SERIAL

#ifdef __cplusplus
}
#endif

#endif /* RAMPART_CONSTANTS_H*/
