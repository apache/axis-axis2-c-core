/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License")" "
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


#ifndef OXS_CONSTANTS_H
#define OXS_CONSTANTS_H

#ifdef __cplusplus
extern "C"
{
#endif

/*Global prefixes**********************/

#define OXS_xenc "xenc"
#define OXS_ds "ds"
#define OXS_WSSE "wsse"

/* Global namespaces *******************/

#define OXS_Ns                    "http://www.aleksey.com/xmlsec/2002"
#define OXS_DSigNs                "http://www.w3.org/2000/09/xmldsig#"
#define OXS_EncNs                 "http://www.w3.org/2001/04/xmlenc#"
#define OXS_XkmsNs                "http://www.w3.org/2002/03/xkms#"
#define OXS_XPathNs               "http://www.w3.org/TR/1999/REC-xpath-19991116"
#define OXS_XPath2Ns              "http://www.w3.org/2002/06/xmldsig-filter2"
#define OXS_XPointerNs            "http://www.w3.org/2001/04/xmldsig-more/xptr"
#define OXS_Soap11Ns              "http://schemas.xmlsoap.org/soap/envelope/"
#define OXS_Soap12Ns              "http://www.w3.org/2002/06/soap-envelope"
#define OXS_WSSENS                "http://schemas.xmlsoap.org/ws/2002/04/secext"

/* DSig Nodes  **************************/

#define OXS_NodeSignature         "Signature" 
#define OXS_NodeSignedInfo        "SignedInfo" 
#define OXS_NodeCanonicalizationMethod "CanonicalizationMethod" 
#define OXS_NodeSignatureMethod    "SignatureMethod" 
#define OXS_NodeSignatureValue     "SignatureValue" 
#define OXS_NodeDigestMethod       "DigestMethod" 
#define OXS_NodeDigestValue        "DigestValue" 
#define OXS_NodeObject             "Object" 
#define OXS_NodeManifest           "Manifest" 
#define OXS_NodeSignatureProperties "SignatureProperties" 


/* Encryption Nodes ************************/

#define OXS_NodeEncryptedData             "EncryptedData" 
#define OXS_NodeEncryptionMethod          "EncryptionMethod" 
#define OXS_NodeEncryptionProperties      "EncryptionProperties"
#define OXS_NodeEncryptionProperty        "EncryptionProperty"
#define OXS_NodeCipherData                "CipherData"
#define OXS_NodeCipherValue               "CipherValue"
#define OXS_NodeCipherReference           "CipherReference"
#define OXS_NodeReferenceList             "ReferenceList"
#define OXS_NodeDataReference             "DataReference"
#define OXS_NodeKeyReference              "KeyReference"
#define OXS_NodeCarriedKeyName            "CarriedKeyName"
#define OXS_TypeEncContent                "http://www.w3.org/2001/04/xmlenc#Content"
#define OXS_TypeEncElement                "http://www.w3.org/2001/04/xmlenc#Element"

/* KeyInfo Nodes*****************************/

#define OXS_NodeKeyInfo               "KeyInfo" 
#define OXS_NodeReference             "Reference" 
#define OXS_NodeTransforms            "Transforms" 
#define OXS_NodeTransform             "Transform" 

/* KeyInfo Nodes*****************************/
#define OXS_NodeBinarySecurityToken     "BinarySecurityToken"


/* Attributes********************************/

#define OXS_AttrId            "Id"
#define OXS_AttrURI           "URI"
#define OXS_AttrType          "Type"
#define OXS_AttrMimeType      "MimeType"
#define OXS_AttrEncoding      "Encoding"
#define OXS_AttrAlgorithm     "Algorithm"
#define OXS_AttrFilter        "Filter"
#define OXS_AttrRecipient     "Recipient"
#define OXS_AttrTarget        "Target"
#define OXS_AttrEncodingType  "EncodingType"
#define OXS_AttrValueType     "ValueType"


/*************************************************************************
 *
 * AES strings
 *
 ************************************************************************/
#define OXS_NameAESKeyValue          "aes" 
#define OXS_NodeAESKeyValue          "AESKeyValue" 
#define OXS_HrefAESKeyValue          "http://www.aleksey.com/xmlsec/2002#AESKeyValue" 

#define OXS_NameAes128Cbc        "aes128-cbc" 
#define OXS_HrefAes128Cbc        "http://www.w3.org/2001/04/xmlenc#aes128-cbc" 

#define OXS_NameAes192Cbc        "aes192-cbc" 
#define OXS_HrefAes192Cbc        "http://www.w3.org/2001/04/xmlenc#aes192-cbc" 

#define OXS_NameAes256Cbc        "aes256-cbc" 
#define OXS_HrefAes256Cbc        "http://www.w3.org/2001/04/xmlenc#aes256-cbc" 

#define OXS_NameKWAes128         "kw-aes128" 
#define OXS_HrefKWAes128         "http://www.w3.org/2001/04/xmlenc#kw-aes128" 

#define OXS_NameKWAes192         "kw-aes192" 
#define OXS_HrefKWAes192         "http://www.w3.org/2001/04/xmlenc#kw-aes192" 

#define OXS_NameKWAes256         "kw-aes256" 
#define OXS_HrefKWAes256         "http://www.w3.org/2001/04/xmlenc#kw-aes256" 

/*************************************************************************
 *
 * BASE64 strings
 *
 ************************************************************************/
#define OXS_NameBase64           "base64" 
#define OXS_HrefBase64           "http://www.w3.org/2000/09/xmldsig#base64" 



/* C14N strings**************************/

#define OXS_NameC14Ni         "c14n" 
#define OXS_HrefC14N          "http://www.w3.org/TR/2001/REC-xml-c14n-20010315" 

#define OXS_NameC14NWithComments  "c14n-with-comments" 
#define OXS_HrefC14NWithComments  "http://www.w3.org/TR/2001/REC-xml-c14n-20010315#WithComments" 

#define OXS_NameExcC14N       "exc-c14n" 
#define OXS_HrefExcC14N       "http://www.w3.org/2001/10/xml-exc-c14n#" 

#define OXS_NameExcC14NWithComments   "exc-c14n-with-comments" 
#define OXS_HrefExcC14NWithComments   "http://www.w3.org/2001/10/xml-exc-c14n#WithComments" 

#define OXS_NsExcC14N             "http://www.w3.org/2001/10/xml-exc-c14n#" 
#define OXS_NsExcC14NWithComments "http://www.w3.org/2001/10/xml-exc-c14n#WithComments" 

#define OXS_NodeInclusiveNamespaces   "InclusiveNamespaces" 
#define OXS_AttrPrefixList            "PrefixList" 

/* DES strings*****************************/

#define OXS_NameDESKeyValue       "des" 
#define OXS_NodeDESKeyValue       "DESKeyValue" 
#define OXS_HrefDESKeyValue       "http://www.aleksey.com/xmlsec/2002#DESKeyValue" 

#define OXS_NameDes3Cbc           "tripledes-cbc" 
#define OXS_HrefDes3Cbc           "http://www.w3.org/2001/04/xmlenc#tripledes-cbc" 

#define OXS_NameKWDes3            "kw-tripledes" 
#define OXS_HrefKWDes3            "http://www.w3.org/2001/04/xmlenc#kw-tripledes" 


/*************************************************************************
 *
 * GOST2001 strings
 *
 ************************************************************************/
#define OXS_NameGOST2001KeyValue         "gost2001" 
#define OXS_NodeGOST2001KeyValue         "GOST3410-2001-KeyValue" 
#define OXS_HrefGOST2001KeyValue         "http://www.w3.org/2000/09/xmldsig#GOST2001KeyValue" 

#define OXS_NameGost2001GostR3411_94         "gostr34102001-gostr3411" 
#define OXS_HrefGost2001GostR3411_94         "http://www.w3.org/2001/04/xmldsig-more#gostr34102001-gostr3411" 

/*************************************************************************
 *
 * DSA strings
 *
 ************************************************************************/
#define OXS_NameDSAKeyValue          "dsa" 
#define OXS_NodeDSAKeyValue          "DSAKeyValue" 
#define OXS_HrefDSAKeyValue          "http://www.w3.org/2000/09/xmldsig#DSAKeyValue" 
#define OXS_NodeDSAP             "P" 
#define OXS_NodeDSAQ             "Q" 
#define OXS_NodeDSAG             "G" 
#define OXS_NodeDSAJ             "J" 
#define OXS_NodeDSAX             "X" 
#define OXS_NodeDSAY             "Y" 
#define OXS_NodeDSASeed          "Seed" 
#define OXS_NodeDSAPgenCounter       "PgenCounter" 

#define OXS_NameDsaSha1          "dsa-sha1" 
#define OXS_HrefDsaSha1          "http://www.w3.org/2000/09/xmldsig#dsa-sha1" 

/*************************************************************************
 *
 * EncryptedKey
 *
 ************************************************************************/
#define OXS_NameEncryptedKey         "enc-key" 
#define OXS_NodeEncryptedKey         "EncryptedKey" 
#define OXS_HrefEncryptedKey         "http://www.w3.org/2001/04/xmlenc#EncryptedKey" 

/*************************************************************************
 *
 * Enveloped transform strings
 *
 ************************************************************************/
#define OXS_NameEnveloped        "enveloped-signature" 
#define OXS_HrefEnveloped        "http://www.w3.org/2000/09/xmldsig#enveloped-signature" 


/*************************************************************************
 *
 * HMAC strings
 *
 ************************************************************************/
#define OXS_NameHMACKeyValue         "hmac" 
#define OXS_NodeHMACKeyValue         "HMACKeyValue" 
#define OXS_HrefHMACKeyValue         "http://www.aleksey.com/xmlsec/2002#HMACKeyValue" 

#define OXS_NodeHMACOutputLength     "HMACOutputLength" 

#define OXS_NameHmacMd5          "hmac-md5" 
#define OXS_HrefHmacMd5          "http://www.w3.org/2001/04/xmldsig-more#hmac-md5" 

#define OXS_NameHmacRipemd160        "hmac-ripemd160" 
#define OXS_HrefHmacRipemd160        "http://www.w3.org/2001/04/xmldsig-more#hmac-ripemd160" 

#define OXS_NameHmacSha1         "hmac-sha1" 
#define OXS_HrefHmacSha1         "http://www.w3.org/2000/09/xmldsig#hmac-sha1" 

#define OXS_NameHmacSha224           "hmac-sha224" 
#define OXS_HrefHmacSha224           "http://www.w3.org/2001/04/xmldsig-more#hmac-sha224" 

#define OXS_NameHmacSha256           "hmac-sha256" 
#define OXS_HrefHmacSha256           "http://www.w3.org/2001/04/xmldsig-more#hmac-sha256" 

#define OXS_NameHmacSha384           "hmac-sha384" 
#define OXS_HrefHmacSha384           "http://www.w3.org/2001/04/xmldsig-more#hmac-sha384" 

#define OXS_NameHmacSha512           "hmac-sha512" 
#define OXS_HrefHmacSha512           "http://www.w3.org/2001/04/xmldsig-more#hmac-sha512" 

/*************************************************************************
 *
 * KeyName strings
 *
 ************************************************************************/
#define OXS_NameKeyName          "key-name" 
#define OXS_NodeKeyName          "KeyName" 


/*************************************************************************
 *
 * KeyValue strings
 *
 ************************************************************************/
#define OXS_NameKeyValue         "key-value" 
#define OXS_NodeKeyValue         "KeyValue" 

/*************************************************************************
 *
 * Memory Buffer strings
 *
 ************************************************************************/
#define OXS_NameMemBuf           "membuf-transform" 

/*************************************************************************
 *
 * MD5 strings
 *
 ************************************************************************/
#define OXS_NameMd5              "md5" 
#define OXS_HrefMd5              "http://www.w3.org/2001/04/xmldsig-more#md5" 

/*************************************************************************
 *
 * RetrievalMethod
 *
 ************************************************************************/
#define OXS_NameRetrievalMethod      "retrieval-method" 
#define OXS_NodeRetrievalMethod      "RetrievalMethod" 

/*************************************************************************
 *
 * RIPEMD160 strings
 *
 ************************************************************************/
#define OXS_NameRipemd160        "ripemd160" 
#define OXS_HrefRipemd160        "http://www.w3.org/2001/04/xmlenc#ripemd160" 


/*************************************************************************
 *
 * RSA strings
 *
 ************************************************************************/
#define OXS_NameRSAKeyValue          "rsa" 
#define OXS_NodeRSAKeyValue          "RSAKeyValue" 
#define OXS_HrefRSAKeyValue          "http://www.w3.org/2000/09/xmldsig#RSAKeyValue" 
#define OXS_NodeRSAModulus           "Modulus" 
#define OXS_NodeRSAExponent          "Exponent" 
#define OXS_NodeRSAPrivateExponent       "PrivateExponent" 

#define OXS_NameRsaMd5           "rsa-md5" 
#define OXS_HrefRsaMd5           "http://www.w3.org/2001/04/xmldsig-more#rsa-md5" 

#define OXS_NameRsaRipemd160         "rsa-ripemd160" 
#define OXS_HrefRsaRipemd160         "http://www.w3.org/2001/04/xmldsig-more#rsa-ripemd160" 

#define OXS_NameRsaSha1          "rsa-sha1" 
#define OXS_HrefRsaSha1          "http://www.w3.org/2000/09/xmldsig#rsa-sha1" 

#define OXS_NameRsaSha224        "rsa-sha224" 
#define OXS_HrefRsaSha224        "http://www.w3.org/2001/04/xmldsig-more#rsa-sha224" 

#define OXS_NameRsaSha256        "rsa-sha256" 
#define OXS_HrefRsaSha256        "http://www.w3.org/2001/04/xmldsig-more#rsa-sha256" 

#define OXS_NameRsaSha384        "rsa-sha384" 
#define OXS_HrefRsaSha384        "http://www.w3.org/2001/04/xmldsig-more#rsa-sha384" 

#define OXS_NameRsaSha512        "rsa-sha512" 
#define OXS_HrefRsaSha512        "http://www.w3.org/2001/04/xmldsig-more#rsa-sha512" 

#define OXS_NameRsaPkcs1         "rsa-1_5" 
#define OXS_HrefRsaPkcs1         "http://www.w3.org/2001/04/xmlenc#rsa-1_5" 

#define OXS_NameRsaOaep          "rsa-oaep-mgf1p" 
#define OXS_HrefRsaOaep          "http://www.w3.org/2001/04/xmlenc#rsa-oaep-mgf1p" 
#define OXS_NodeRsaOAEPparams        "OAEPparams" 


/*************************************************************************
 *
 * GOSTR3411_94 strings
 *
 ************************************************************************/
#define OXS_NameGostR3411_94             "gostr3411" 
#define OXS_HrefGostR3411_94             "http://www.w3.org/2001/04/xmldsig-more#gostr3411" 

/*************************************************************************
 *
 * SHA1 strings
 *
 ************************************************************************/
#define OXS_NameSha1             "sha1" 
#define OXS_HrefSha1             "http://www.w3.org/2000/09/xmldsig#sha1" 

#define OXS_NameSha224           "sha224" 
#define OXS_HrefSha224           "http://www.w3.org/2001/04/xmldsig-more#sha224" 

#define OXS_NameSha256           "sha256" 
#define OXS_HrefSha256           "http://www.w3.org/2001/04/xmlenc#sha256" 

#define OXS_NameSha384           "sha384" 
#define OXS_HrefSha384           "http://www.w3.org/2001/04/xmldsig-more#sha384" 

#define OXS_NameSha512           "sha512" 
#define OXS_HrefSha512           "http://www.w3.org/2001/04/xmlenc#sha512" 

/*************************************************************************
 *
 * X509 strings
 *
 ************************************************************************/
#define OXS_NameX509Data         "x509" 
#define OXS_NodeX509Data         "X509Data" 
#define OXS_HrefX509Data         "http://www.w3.org/2000/09/xmldsig#X509Data" 

#define OXS_NodeX509Certificate      "X509Certificate" 
#define OXS_NodeX509CRL          "X509CRL" 
#define OXS_NodeX509SubjectName      "X509SubjectName" 
#define OXS_NodeX509IssuerSerial     "X509IssuerSerial" 
#define OXS_NodeX509IssuerName       "X509IssuerName" 
#define OXS_NodeX509SerialNumber     "X509SerialNumber" 
#define OXS_NodeX509SKI          "X509SKI" 

#define OXS_NameRawX509Cert          "raw-x509-cert" 
#define OXS_HrefRawX509Cert          "http://www.w3.org/2000/09/xmldsig#rawX509Certificate" 

#define OXS_NameX509Store        "x509-store" 

/*************************************************************************
 *
 * PGP strings
 *
 ************************************************************************/
#define OXS_NamePGPData          "pgp" 
#define OXS_NodePGPData          "PGPData" 
#define OXS_HrefPGPData          "http://www.w3.org/2000/09/xmldsig#PGPData" 

/*************************************************************************
 *
 * SPKI strings
 *
 ************************************************************************/
#define OXS_NameSPKIData         "spki" 
#define OXS_NodeSPKIData         "SPKIData" 
#define OXS_HrefSPKIData         "http://www.w3.org/2000/09/xmldsig#SPKIData" 


/*************************************************************************
 *
 * XPath/XPointer strings
 *
 ************************************************************************/
#define OXS_NameXPath            "xpath" 
#define OXS_NodeXPath            "XPath" 

#define OXS_NameXPath2           "xpath2" 
#define OXS_NodeXPath2           "XPath" 
#define OXS_XPath2FilterIntersect    "intersect" 
#define OXS_XPath2FilterSubtract     "subtract" 
#define OXS_XPath2FilterUnion        "union" 

#define OXS_NameXPointer         "xpointer" 
#define OXS_NodeXPointer         "XPointer" 

/*************************************************************************
 *
 * Xslt strings
 *
 ************************************************************************/
#define OXS_NameXslt             "xslt" 
#define OXS_HrefXslt             "http://www.w3.org/TR/1999/REC-xslt-19991116" 

/*************************************************************************
 *
 * SOAP 1.1/1.2 strings
 *
 ************************************************************************/
#define OXS_NodeEnvelope         "Envelope" 
#define OXS_NodeHeader           "Header" 
#define OXS_NodeBody                 "Body" 
#define OXS_NodeFault                "Fault" 
#define OXS_NodeFaultCode        "faultcode" 
#define OXS_NodeFaultString              "faultstring" 
#define OXS_NodeFaultActor               "faultactor" 
#define OXS_NodeFaultDetail              "detail" 
#define OXS_NodeCode             "Code" 
#define OXS_NodeReason           "Reason" 
#define OXS_NodeNode             "Node" 
#define OXS_NodeRole             "Role" 
#define OXS_NodeDetail           "Detail" 
#define OXS_NodeValue            "Value" 
#define OXS_NodeSubcode          "Subcode" 
#define OXS_NodeText             "Text" 


#define OXS_SoapFaultCodeVersionMismatch     "VersionMismatch" 
#define OXS_SoapFaultCodeMustUnderstand      "MustUnderstand" 
#define OXS_SoapFaultCodeClient          "Client" 
#define OXS_SoapFaultCodeServer          "Server" 
#define OXS_SoapFaultCodeReceiver        "Receiver" 
#define OXS_SoapFaultCodeSender          "Sender" 
#define OXS_SoapFaultDataEncodningUnknown    "DataEncodingUnknown" 








/*************************************************************************/



#ifdef __cplusplus
}
#endif

#endif /* OXS_CONSTANTS_H*/
