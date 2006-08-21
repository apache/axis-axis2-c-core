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

    /** Type name for struct rampart_actions_ops */
    typedef struct rampart_actions_ops rampart_actions_ops_t;
    /** Type name for struct rampart_actions */
    typedef struct rampart_actions rampart_actions_t;


    struct rampart_actions_ops
    {
        
        /**
        * Gets encryption_user of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @return encryption_user
        */
        axis2_char_t *(AXIS2_CALL *
        get_encryption_user )(
                    rampart_actions_t *actions,
                    const axis2_env_t *env
                    );
        
        /**
        * Gets encryption_sym_algorithm of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @return encryption_sym_algorithm
        */
        axis2_char_t *(AXIS2_CALL *
        get_encryption_sym_algorithm )(
                    rampart_actions_t *actions,
                    const axis2_env_t *env
                    );

        /**
        * Gets encryption_key_transport_algorithm of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @return encryption_key_transport_algorithm
        */
        axis2_char_t *(AXIS2_CALL *
        get_encryption_key_transport_algorithm )(
                    rampart_actions_t *actions,
                    const axis2_env_t *env
                    );


        /**
        * Gets items of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @return items
        */
        axis2_char_t *(AXIS2_CALL *
        get_items )(
                    rampart_actions_t *actions,
                    const axis2_env_t *env
                    );

        /**
        * Gets user of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @return user
        */
        axis2_char_t *(AXIS2_CALL *
        get_user )(
                    rampart_actions_t *actions,
                    const axis2_env_t *env
                    );

        /**
        * Gets password_callback_class of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @return password_callback_class
        */
        axis2_char_t *(AXIS2_CALL *
        get_password_callback_class )(
                    rampart_actions_t *actions,
                    const axis2_env_t *env
                    );

        /**
        * Gets encryption_prop_file of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @return encryption_prop_file
        */
        axis2_char_t *(AXIS2_CALL *
        get_encryption_prop_file )(
                    rampart_actions_t *actions,
                    const axis2_env_t *env
                    );

        /**
        * Gets signature_prop_file  of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @return signature_prop_file
        */
        axis2_char_t *(AXIS2_CALL *
        get_signature_prop_file)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env
                    );
          
        /**
        * Gets signature_key_identifier of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @return signature_key_identifier
        */
        axis2_char_t *(AXIS2_CALL *
        get_signature_key_identifier)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env
                    );
          
        /**
        * Gets encryption_key of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @return encryption_key
        */
        axis2_char_t *(AXIS2_CALL *
        get_encryption_key_identifier)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env
                    );
          
        /**
        * Gets signature_parts  of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @return signature_parts
        */
        axis2_char_t *(AXIS2_CALL *
        get_signature_parts)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env
                    );
          
        /**
        * Gets encryption parts of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @return encryption_parts 
        */
        axis2_char_t *(AXIS2_CALL *
        get_encryption_parts)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env
                    );
        /**
        * Sets encryption_user of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @param encryption_user
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
        set_encryption_user)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env,
                    axis2_char_t *encryption_user
                    );

        /**
        * Sets encryption_sym_algorithm of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @param encryption_sym_algorithm
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
        set_encryption_sym_algorithm)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env,
                    axis2_char_t *encryption_sym_algorithm
                    );

        /**
        * Sets encryption_key_transport_algorithm of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @param encryption_key_transport_algorithm
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
        set_encryption_key_transport_algorithm)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env,
                    axis2_char_t *encryption_key_transport_algorithm
                    );
        
        /**
        * Sets items  of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @param items
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
        set_items)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env,
                    axis2_char_t *items
                    );
        
        /**
        * Sets user of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @param user
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
        set_user)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env,
                    axis2_char_t *user
                    );
        
        /**
        * Sets password_callback_class of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @param password_callback_class
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
        set_password_callback_class)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env,
                    axis2_char_t *password_callback_class
                    );
        
        /**
        * Sets encryption_prop_file of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @param encryption_prop_file
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
        set_encryption_prop_file)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env,
                    axis2_char_t *encryption_prop_file
                    );
        
        /**
        * Sets signature_prop_file of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @param signature_prop_file
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
        set_signature_prop_file)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env,
                    axis2_char_t *signature_prop_file
                    );
        
        /**
        * Sets signature_key_identifier of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @param signature_key_identifier
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
        set_signature_key_identifier)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env,
                    axis2_char_t *signature_key_identifier
                    );
        
        /**
        * Sets encryption_key_identifier of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @param encryption_key_identifier
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
        set_encryption_key_identifier)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env,
                    axis2_char_t *encryption_key_identifier
                    );
        
        /**
        * Sets signature_parts of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @param signature_parts
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
        set_signature_parts)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env,
                    axis2_char_t *signature_parts
                    );
        
        /**
        * Sets encryption_parts of the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @param encryption_parts
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
        set_encryption_parts)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env,
                    axis2_char_t *encryption_parts
                    );
        


        /**
        * Reset rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
        reset)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env
                    );

        /**
        * Free the rampart action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
        free)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env
                    );

        /**
        * Populate rampart action reading the param_action.
        * @param actions rampart_action ptr to action
        * @param env pointer to environment struct
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
        populate)(
                    rampart_actions_t *actions,
                    const axis2_env_t *env, 
                    axis2_param_t *param_action
                    );

    };

    struct rampart_actions
    {
        /** operations of rampart_actions */
        rampart_actions_ops_t *ops;
    };

    /*Create function*/    
    AXIS2_EXTERN rampart_actions_t *AXIS2_CALL
    rampart_actions_create(const axis2_env_t *env);


/**********************Macros******************************************/
#define RAMPART_ACTIONS_GET_ENC_USER(actions, env) \
        ((actions)->ops->get_encryption_user(actions, env) )

#define RAMPART_ACTIONS_SET_ENC_USER(actions, env, encryption_user) \
        ((actions)->ops->set_encryption_user(actions, env, encryption_user) )

#define RAMPART_ACTIONS_GET_ENC_SYM_ALGO(actions, env) \
        ((actions)->ops->get_encryption_sym_algorithm(actions, env) )

#define RAMPART_ACTIONS_SET_ENC_SYM_ALGO(actions, env, encryption_sym_algorithm) \
        ((actions)->ops->set_encryption_sym_algorithm(actions, env, encryption_sym_algorithm) )

#define RAMPART_ACTIONS_GET_ENC_KT_ALGO(actions, env) \
        ((actions)->ops->get_encryption_key_transport_algorithm(actions, env) )

#define RAMPART_ACTIONS_SET_ENC_KT_ALGO(actions, env, encryption_key_transport_algorithm)\
        ((actions)->ops->set_encryption_key_transport_algorithm(actions, env, encryption_key_transport_algorithm))

#define RAMPART_ACTIONS_GET_ITEMS(actions, env) \
        ((actions)->ops->get_items(actions, env) )

#define RAMPART_ACTIONS_SET_ITEMS(actions, env, items)\
        ((actions)->ops->set_items(actions, env, items))

#define RAMPART_ACTIONS_GET_USER(actions, env) \
        ((actions)->ops->get_user(actions, env) )

#define RAMPART_ACTIONS_SET_USER(actions, env, user)\
        ((actions)->ops->set_user(actions, env, user))

#define RAMPART_ACTIONS_GET_PW_CB_CLASS(actions, env) \
        ((actions)->ops->get_password_callback_class(actions, env) )

#define RAMPART_ACTIONS_SET_PW_CB_CLASS(actions, env, password_callback_class)\
        ((actions)->ops->set_password_callback_class(actions, env,password_callback_class ))

#define RAMPART_ACTIONS_GET_ENC_PROP_FILE(actions, env) \
        ((actions)->ops->get_encryption_prop_file(actions, env) )

#define RAMPART_ACTIONS_SET_ENC_PROP_FILE(actions, env, encryption_prop_file)\
        ((actions)->ops->set_encryption_prop_file(actions, env, encryption_prop_file))

#define RAMPART_ACTIONS_GET_SIG_PROP_FILE(actions, env) \
        ((actions)->ops->get_signature_prop_file(actions, env) )

#define RAMPART_ACTIONS_SET_SIG_PROP_FILE(actions, env, signature_prop_file)\
        ((actions)->ops->set_signature_prop_file(actions, env, signature_prop_file))

#define RAMPART_ACTIONS_GET_SIG_KEY_IDENTIFIER(actions, env) \
        ((actions)->ops->get_signature_key_identifier(actions, env) )

#define RAMPART_ACTIONS_SET_SIG_KEY_IDENTIFIER(actions, env, signature_key_identifier)\
        ((actions)->ops->set_signature_key_identifier(actions, env, signature_key_identifier))

#define RAMPART_ACTIONS_GET_ENC_KEY_IDENTIFIER(actions, env) \
        ((actions)->ops->get_encryption_key_identifier(actions, env) )

#define RAMPART_ACTIONS_SET_ENC_KEY_IDENTIFIER(actions, env, encryption_key_identifier)\
        ((actions)->ops->set_encryption_key_identifier(actions, env, encryption_key_identifier))

#define RAMPART_ACTIONS_GET_SIGNATURE_PARTS(actions, env) \
        ((actions)->ops->get_signature_parts(actions, env) )

#define RAMPART_ACTIONS_SET_SIGNATURE_PARTS(actions, env, signature_parts)\
        ((actions)->ops->set_signature_parts(actions, env, signature_parts))

#define RAMPART_ACTIONS_GET_ENCRYPTION_PARTS(actions, env) \
        ((actions)->ops->get_encryption_parts(actions, env) )

#define RAMPART_ACTIONS_SET_ENCRYPTION_PARTS(actions, env, encryption_parts)\
        ((actions)->ops->set_encryption_parts(actions, env, encryption_parts))

#define RAMPART_ACTIONS_RESET(actions, env) \
        ((actions)->ops->reset(actions, env))

#define RAMPART_ACTIONS_FREE(actions, env) \
        ((actions)->ops->free(actions, env))

#define RAMPART_ACTIONS_POPULATE(actions, env, param_action)\
        ((actions)->ops->populate(actions, env, param_action))



/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* RAMPART_ACTION_H */
