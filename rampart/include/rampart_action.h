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

#ifndef RAMPART_ACTION_H
#define RAMPART_ACTION_H


/**
  * @file oxs_ctx.h
  * @brief 
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axiom_node.h>
#include <oxs_buffer.h>
#include <oxs_key.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup 
 * @ingroup 
 * @{
 */

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
                <xs:element name="encryptionPropFile" type="xs:string" minOccurs="0"/>
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
typedef struct _actions actionss, *actions_ptr;

struct _actions{
        /*axis2_char_t *items  ;
        axis2_char_t *user  ;
        axis2_char_t *passwordC_callback_lass  ;
        axis2_char_t *encryption_prop_file;
        axis2_char_t *signature_prop_file ;
        axis2_char_t *signature_key_identifier  ;
        axis2_char_t *encryption_key_identifier  ;*/
        axis2_char_t *encryption_user  ;
       /*axis2_char_t *signature_parts  ;
        axis2_char_t *encryption_parts  ;
        axis2_char_t *optimize_parts  ;*/
        axis2_char_t *encryption_sym_algorithm  ;
        /*axis2_char_t *embedded_key_callback_class  ;*/
        axis2_char_t *encryption_key_transport_algorithm  ;
        /*axis2_char_t *embedded_key_name  ;
        axis2_char_t *time_to_live  ;*/
}; 

AXIS2_EXTERN actions_ptr AXIS2_CALL
oxs_ctx_create_actions_ptr(const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_ctx_reset_actions_ptr(const axis2_env_t *env, actions_ptr actions);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_ctx_free_actions_ptr(const axis2_env_t *env, actions_ptr actions);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_ctx_populate_actions_ptr(const axis2_env_t *env, actions_ptr actions, axis2_param_t *param_action  );


/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* RAMPART_ACTION_H */
