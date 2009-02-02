
/*
* Copyright 2004,2005 The Apache Software Foundation.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain count copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef AXIS2_HTTP_TRANSPORT_H
#define AXIS2_HTTP_TRANSPORT_H

#include <axis2_const.h>
#include <axutil_version.h>
#include <axiom_mime_const.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** @defgroup axis2_core_trans_http http transport
     * @ingroup axis2_transport
     * Description.
     * @{
     */

    /**
     * @defgroup axis2_core_transport_http core http transport
     * @ingroup axis2_core_trans_http
     * @{
     */

    /**
     * @brief HTTP protocol and message context constants.
     *
     */
#define AXIS2_HTTP_OUT_TRANSPORT_INFO "HTTPOutTransportInfo"

    /**
     * CARRIAGE RETURN AND LINE FEED
     */
#define AXIS2_HTTP_CRLF AXIS2_CRLF

    /**
     * PROTOCOL_VERSION
     */
#define AXIS2_HTTP_PROTOCOL_VERSION "PROTOCOL"

    /**
     * REQUEST_URI
     */
#define AXIS2_HTTP_REQUEST_URI "REQUEST_URI"

    /**
     * RESPONSE_CODE
     */
#define AXIS2_HTTP_RESPONSE_CODE "RESPONSE_CODE"

    /**
     * RESPONSE_WORD
     */
#define AXIS2_HTTP_RESPONSE_WORD "RESPONSE_WORD"

    /*
     * RESPONSE_CONTINUE_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_CONTINUE_CODE_VAL 100

    /*
     * RESPONSE_OK_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_OK_CODE_VAL 200

    /*
     * RESPONSE_CREATED_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_CREATED_CODE_VAL 201

    /**
     * RESPONSE_ACK_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_ACK_CODE_VAL 202

    /**
     * RESPONSE_NON_AUTHORITATIVE_INFO_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_NON_AUTHORITATIVE_INFO_CODE_VAL 203

    /**
     * RESPONSE_NO_CONTENT_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_NO_CONTENT_CODE_VAL 204

    /**
     * RESPONSE_RESET_CONTENT_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_RESET_CONTENT_CODE_VAL 205

    /**
     * RESPONSE_MULTIPLE_CHOICES_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_MULTIPLE_CHOICES_CODE_VAL 300

    /**
     * RESPONSE_MOVED_PERMANENTLY_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_MOVED_PERMANENTLY_CODE_VAL 301

    /**
     * RESPONSE_SEE_OTHER_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_SEE_OTHER_CODE_VAL 303

    /**
     * RESPONSE_NOT_MODIFIED_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_NOT_MODIFIED_CODE_VAL 304

    /**
     * RESPONSE_TEMPORARY_REDIRECT_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_TEMPORARY_REDIRECT_CODE_VAL 307

    /**
     * RESPONSE_BAD_REQUEST_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_VAL 400

    /**
     * RESPONSE_HTTP_UNAUTHORIZED_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_HTTP_UNAUTHORIZED_CODE_VAL 401

	/**
	* RESPONSE_HTTP_FORBIDDEN_CODE_VAL
	*/
#define AXIS2_HTTP_RESPONSE_FORBIDDEN_CODE_VAL 403


    /**
     * RESPONSE_NOT_FOUND_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_NOT_FOUND_CODE_VAL 404

    /**
     * RESPONSE_METHOD_NOT_ALLOWED_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_METHOD_NOT_ALLOWED_CODE_VAL 405

    /**
     * RESPONSE_NOT_ACCEPTABLE_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_NOT_ACCEPTABLE_CODE_VAL 406

    /**
     * RESPONSE_PROXY_AUTHENTICATION_REQUIRED_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_PROXY_AUTHENTICATION_REQUIRED_CODE_VAL 407

    /**
     * RESPONSE_REQUEST_TIMEOUT_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_REQUEST_TIMEOUT_CODE_VAL 408

    /**
     * RESPONSE_CONFLICT_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_CONFLICT_CODE_VAL 409

    /**
     * RESPONSE_GONE_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_GONE_CODE_VAL 410

    /**
     * RESPONSE_LENGTH_REQUIRED_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_LENGTH_REQUIRED_CODE_VAL 411

    /**
     * RESPONSE_PRECONDITION_FAILED_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_PRECONDITION_FAILED_CODE_VAL 412

    /**
     * RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_VAL 413

    /**
     * RESPONSE_INTERNAL_SERVER_ERROR_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_INTERNAL_SERVER_ERROR_CODE_VAL 500

    /**
     * RESPONSE_NOT_IMPLEMENTED_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_NOT_IMPLEMENTED_CODE_VAL 501

    /**
     * RESPONSE_SERVICE_UNAVAILABLE_CODE_VAL
     */
#define AXIS2_HTTP_RESPONSE_SERVICE_UNAVAILABLE_CODE_VAL 503

    /**
     * RESPONSE_CONTINUE_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_CONTINUE_CODE_NAME "Continue"

    /**
     * RESPONSE_OK_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_OK_CODE_NAME "OK"

    /*
     * RESPONSE_CREATED_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_CREATED_CODE_NAME "Created"

    /**
     * RESPONSE_ACK_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_ACK_CODE_NAME "Accepted"

    /**
     * RESPONSE_NO_CONTENT_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_NO_CONTENT_CODE_NAME "No Content"

    /**
     * RESPONSE_NON_AUTHORITATIVE_INFO_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_NON_AUTHORITATIVE_INFO_CODE_NAME "Non-Authoritative Information"

    /**
     * RESPONSE_RESET_CONTENT_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_RESET_CONTENT_CODE_NAME "Reset Content"

    /**
     * RESPONSE_MULTIPLE_CHOICES_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_MULTIPLE_CHOICES_CODE_NAME "Multiple Choices"

    /**
     * RESPONSE_MOVED_PERMANENTLY_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_MOVED_PERMANENTLY_CODE_NAME "Moved Permanently"

    /**
     * RESPONSE_SEE_OTHER_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_SEE_OTHER_CODE_NAME "See Other"

    /**
     * RESPONSE_NOT_MODIFIED_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_NOT_MODIFIED_CODE_NAME "Not Modified"

    /**
     * RESPONSE_TEMPORARY_REDIRECT_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_TEMPORARY_REDIRECT_CODE_NAME "Temporary Redirect"

    /**
     * RESPONSE_BAD_REQUEST_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_NAME "Bad Request"

    /**
     * RESPONSE_HTTP_UNAUTHORIZED_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_HTTP_UNAUTHORIZED_CODE_NAME "Unauthorized"


	/**
	 *  RESPONSE_HTTP_FORBIDDEN_CODE_NAME                                                                     
	 */
#define AXIS2_HTTP_RESPONSE_HTTP_FORBIDDEN_CODE_NAME "Forbidden"

    /**
     * RESPONSE_NOT_FOUND_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_NOT_FOUND_CODE_NAME "Not Found"

    /**
     * RESPONSE_METHOD_NOT_ALLOWED_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_METHOD_NOT_ALLOWED_CODE_NAME "Method Not Allowed"

    /**
     * RESPONSE_NOT_ACCEPTABLE_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_NOT_ACCEPTABLE_CODE_NAME "Not Acceptable"

    /**
     * RESPONSE_PROXY_AUTHENTICATION_REQUIRED_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_PROXY_AUTHENTICATION_REQUIRED_CODE_NAME "Proxy Authentication Required"

    /**
     * RESPONSE_REQUEST_TIMEOUT_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_REQUEST_TIMEOUT_CODE_NAME "Request Timeout"

    /**
     * RESPONSE_CONFLICT_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_CONFLICT_CODE_NAME "Conflict"

    /**
     * RESPONSE_GONE_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_GONE_CODE_NAME "Gone"

    /**
     * RESPONSE_LENGTH_REQUIRED_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_LENGTH_REQUIRED_CODE_NAME "Length Required"

    /**
     * RESPONSE_PRECONDITION_FAILED_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_PRECONDITION_FAILED_CODE_NAME "Precondition Failed"

    /**
     * RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_NAME "Request Entity Too Large"

    /**
     * RESPONSE_INTERNAL_SERVER_ERROR_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_INTERNAL_SERVER_ERROR_CODE_NAME "Internal Server Error"

    /**
     * RESPONSE_NOT_IMPLEMENTED_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_NOT_IMPLEMENTED_CODE_NAME "Not Implemented"

    /**
     * RESPONSE_SERVICE_UNAVAILABLE_CODE_NAME
     */
#define AXIS2_HTTP_RESPONSE_SERVICE_UNAVAILABLE_CODE_NAME "Service Unavailable"

    /**
     * SOCKET
     */
#define AXIS2_SOCKET "SOCKET"

    /**
     * HEADER_PROTOCOL_10
     */
#define AXIS2_HTTP_HEADER_PROTOCOL_10 "HTTP/1.0"

    /**
     * HEADER_PROTOCOL_11
     */
#define AXIS2_HTTP_HEADER_PROTOCOL_11 "HTTP/1.1"

    /**
     * CHAR_SET_ENCODING
     */
#define AXIS2_HTTP_CHAR_SET_ENCODING "charset"

    /**
     * HEADER_POST
     */
#define AXIS2_HTTP_POST  "POST"

    /**
     * HEADER_GET
     */
#define AXIS2_HTTP_GET "GET"

    /**
     * HEADER_HEAD
     */
#define AXIS2_HTTP_HEAD "HEAD"

    /**
     * HEADER_PUT
     */
#define AXIS2_HTTP_PUT "PUT"

    /**
     * HEADER_DELETE
     */
#define AXIS2_HTTP_DELETE "DELETE"

    /**
     * HEADER_HOST
     */
#define AXIS2_HTTP_HEADER_HOST "Host"

    /**
     * HEADER_CONTENT_DESCRIPTION
     */
#define AXIS2_HTP_HEADER_CONTENT_DESCRIPTION "Content-Description"

    /**
     * HEADER_CONTENT_TYPE
     */
#define AXIS2_HTTP_HEADER_CONTENT_TYPE "Content-Type"
#define AXIS2_HTTP_HEADER_CONTENT_TYPE_ "Content-Type: "

    /**
     *USER DEFINED HEADER CONTENT TYPE
     */

#define AXIS2_USER_DEFINED_HTTP_HEADER_CONTENT_TYPE "User_Content_Type"

    /**
     * HEADER_CONTENT_TYPE
     */
#define AXIS2_HTTP_HEADER_CONTENT_TYPE_MIME_BOUNDARY "boundary"

    /**
     * HEADER_CONTENT_TRANSFER_ENCODING
     */
#define AXIS2_HTTP_HEADER_CONTENT_TRANSFER_ENCODING \
                                    "Content-Transfer-Encoding"

    /**
     * HEADER_CONTENT_LENGTH
     */
#define AXIS2_HTTP_HEADER_CONTENT_LENGTH "Content-Length"

    /**
     * HEADER_CONTENT_LANGUAGE
     */
#define AXIS2_HTTP_HEADER_CONTENT_LANGUAGE "Content-Language"

#define AXIS2_HTTP_HEADER_CONTENT_LENGTH_ "Content-Length: "

    /**
     * HEADER_CONTENT_LOCATION
     */
#define AXIS2_HTTP_HEADER_CONTENT_LOCATION "Content-Location"

    /**
     * HEADER_CONTENT_ID
     */
#define AXIS2_HTTP_HEADER_CONTENT_ID "Content-Id"

    /**
     * HEADER_SOAP_ACTION
     */
#define AXIS2_HTTP_HEADER_SOAP_ACTION "SOAPAction"
#define AXIS2_HTTP_HEADER_SOAP_ACTION_ "SOAPAction: "

    /**
     * HEADER_AUTHORIZATION
     */
#define AXIS2_HTTP_HEADER_AUTHORIZATION "Authorization"

    /**
     * HEADER_WWW_AUTHENTICATE
     */
#define AXIS2_HTTP_HEADER_WWW_AUTHENTICATE "WWW-Authenticate"

    /**
      * HEADER_PROXY_AUTHENTICATE
      */
#define AXIS2_HTTP_HEADER_PROXY_AUTHENTICATE "Proxy-Authenticate"

    /**
     * HEADER_PROXY_AUTHORIZATION
     */
#define AXIS2_HTTP_HEADER_PROXY_AUTHORIZATION "Proxy-Authorization"

    /**
     * AUTHORIZATION_REQUEST_PARAM_REALM
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_REALM "realm"

    /**
     * AUTHORIZATION_REQUEST_PARAM_DOMAIN
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_DOMAIN "domain"

    /**
     * AUTHORIZATION_REQUEST_PARAM_NONCE
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_NONCE "nonce"

    /**
     * AUTHORIZATION_REQUEST_PARAM_OPAQUE
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_OPAQUE "opaque"

    /**
     * AUTHORIZATION_REQUEST_PARAM_STALE
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_STALE "stale"

    /**
     * AUTHORIZATION_REQUEST_PARAM_ALGORITHM
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_ALGORITHM "algorithm"

    /**
     * AUTHORIZATION_REQUEST_PARAM_QOP
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_QOP "qop"

    /**
     * AUTHORIZATION_REQUEST_PARAM_USERNAME
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_USERNAME "username"

    /**
     * AUTHORIZATION_REQUEST_PARAM_URI
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_URI "uri"

    /**
     * AUTHORIZATION_REQUEST_PARAM_RESPONSE
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_RESPONSE "response"

    /**
     * AUTHORIZATION_REQUEST_PARAM_NONCE_COUNT
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_NONCE_COUNT "nc"

    /**
     * AUTHORIZATION_REQUEST_PARAM_CLIENT_NONCE
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_PARAM_CLIENT_NONCE "cnonce"

    /**
     * AUTHORIZATION_REQUEST_DEFAULT_CLIENT_NONCE
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_DEFAULT_CLIENT_NONCE "00000001"

    /**
     * AUTHORIZATION_REQUEST_QOP_OPTION_AUTH
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_QOP_OPTION_AUTH "auth"

    /**
     * AUTHORIZATION_REQUEST_QOP_OPTION_AUTH_INT
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_QOP_OPTION_AUTH_INT "auth-int"

    /**
     * AUTHORIZATION_REQUEST_STALE_STATE_TRUE
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_STALE_STATE_TRUE "true"

    /**
     * AUTHORIZATION_REQUEST_STALE_STATE_FALSE
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_STALE_STATE_FALSE "false"

    /**
     * AUTHORIZATION_REQUEST_ALGORITHM_MD5
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_ALGORITHM_MD5 "MD5"

    /**
     * AUTHORIZATION_REQUEST_ALGORITHM_MD5_SESS
     */
#define AXIS2_HTTP_AUTHORIZATION_REQUEST_ALGORITHM_MD5_SESS "MD5-sess"

    /**
     * HEADER_EXPECT
     */
#define AXIS2_HTTP_HEADER_EXPECT "Expect"

    /**
     * HEADER_EXPECT_100_Continue
     */
#define AXIS2_HTTP_HEADER_EXPECT_100_CONTINUE "100-continue"

    /**
     * HEADER_USER_AGENT
     */
#define AXIS2_HTTP_HEADER_USER_AGENT "User-Agent"

    /**
     * HEADER_USER_AGENT_AXIS2C
     */
#define AXIS2_HTTP_HEADER_USER_AGENT_AXIS2C "User-Agent: Axis2C/" AXIS2_VERSION_STRING

    /**
     * HEADER_SERVER
     */
#define AXIS2_HTTP_HEADER_SERVER "Server"

    /**
     * HEADER_DATE
     */
#define AXIS2_HTTP_HEADER_DATE "Date"

    /**
     * HEADER_SERVER_AXIS2C
     */
#define AXIS2_HTTP_HEADER_SERVER_AXIS2C "Axis2C/" AXIS2_VERSION_STRING

#define AXIS2_HTTP_HEADER_ACCEPT_ "Accept: "

#define AXIS2_HTTP_HEADER_EXPECT_ "Expect: "

    /**
     * HEADER_CACHE_CONTROL
     */
#define AXIS2_HTTP_HEADER_CACHE_CONTROL "Cache-Control"

    /**
     * HEADER_CACHE_CONTROL_NOCACHE
     */
#define AXIS2_HTTP_HEADER_CACHE_CONTROL_NOCACHE "no-cache"

    /**
     * HEADER_PRAGMA
     */
#define AXIS2_HTTP_HEADER_PRAGMA "Pragma"

    /**
     * HEADER_LOCATION
     */
#define AXIS2_HTTP_HEADER_LOCATION "Location"

    /**
     * REQUEST_HEADERS
     */
#define AXIS2_HTTP_REQUEST_HEADERS "HTTP-Request-Headers"

    /**
     * RESPONSE_HEADERS
     */
#define AXIS2_HTTP_RESPONSE_HEADERS "HTTP-Response-Headers"

    /* http 1.1 */

    /**
     * HEADER_TRANSFER_ENCODING
     */
#define AXIS2_HTTP_HEADER_TRANSFER_ENCODING "Transfer-Encoding"

    /**
     * HEADER_TRANSFER_ENCODING_CHUNKED
     */
#define AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED "chunked"

    /**
     * HEADER_CONNECTION
     */
#define AXIS2_HTTP_HEADER_CONNECTION "Connection"

    /**
     * HEADER_CONNECTION_CLOSE
     */
#define AXIS2_HTTP_HEADER_CONNECTION_CLOSE "close"

    /**
     * HEADER_CONNECTION_KEEPALIVE
     */
#define AXIS2_HTTP_HEADER_CONNECTION_KEEPALIVE "Keep-Alive"

    /**
     * HEADER_ACCEPT
     */
#define AXIS2_HTTP_HEADER_ACCEPT "Accept"

    /**
     * HEADER_ACCEPT_CHARSET
     */
#define AXIS2_HTTP_HEADER_ACCEPT_CHARSET "Accept-Charset"

    /**
      * AXIS2_HTTP_HEADER_ACCEPT_LANGUAGE
      */
#define AXIS2_HTTP_HEADER_ACCEPT_LANGUAGE "Accept-Language"

    /**
     * HEADER_ALLOW
     */
#define AXIS2_HTTP_HEADER_ALLOW "Allow"

    /**
     * HEADER_ACCEPT_ALL
     */
#define AXIS2_HTTP_HEADER_ACCEPT_ALL "*/*"

    /**
     * HEADER_ACCEPT_TEXT_ALL
     */
#define AXIS2_HTTP_HEADER_ACCEPT_TEXT_ALL "text/*"

    /**
     * HEADER_ACCEPT_TEXT_PLAIN
     */
#define AXIS2_HTTP_HEADER_ACCEPT_TEXT_PLAIN "text/plain"

    /**
     * HEADER_ACCEPT_TEXT_HTML
     */
#define AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML "text/html"

    /**
     * HEADER APPLICATION_XML
     */
#define AXIS2_HTTP_HEADER_ACCEPT_APPLICATION_XML "application/xml"

    /**
     * HEADER_ACCEPT_TEXT_XML
     */
#define AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML "text/xml"

    /**
     * HEADER_ACCEPT_APPL_SOAP
     */
#define AXIS2_HTTP_HEADER_ACCEPT_APPL_SOAP "application/soap+xml"

    /**
     * HEADER_ACCEPT_X_WWW_FORM_URLENCODED
     */
#define AXIS2_HTTP_HEADER_ACCEPT_X_WWW_FORM_URLENCODED "application/x-www-form-urlencoded"

    /**
     * HEADER XOP XML
     */
#define AXIS2_HTTP_HEADER_ACCEPT_XOP_XML AXIOM_MIME_TYPE_XOP_XML

    /**
     * HEADER_ACCEPT_MULTIPART_RELATED
     */
#define AXIS2_HTTP_HEADER_ACCEPT_MULTIPART_RELATED AXIOM_MIME_TYPE_MULTIPART_RELATED

    /**
     * HEADER_ACCEPT_APPLICATION_DIME
     */
#define AXIS2_HTTP_HEADER_ACCEPT_APPLICATION_DIME "application/dime"

    /**
     * Cookie headers
     */
#define AXIS2_HTTP_HEADER_COOKIE "Cookie"

    /**
     * HEADER_COOKIE2
     */
#define AXIS2_HTTP_HEADER_COOKIE2 "Cookie2"

    /**
     * HEADER_SET_COOKIE
     */
#define AXIS2_HTTP_HEADER_SET_COOKIE "Set-Cookie"

    /**
     * HEADER_SET_COOKIE2
     */
#define AXIS2_HTTP_HEADER_SET_COOKIE2 "Set-Cookie2"

    /**
     * HTTP header field values
     */
#define AXIS2_HTTP_HEADER_DEFAULT_CHAR_ENCODING "iso-8859-1"

    /**
     * REPONSE_HTTP_OK
     */
#define AXIS2_HTTP_RESPONSE_OK "200 OK"

    /**
     * RESPONSE_HTTP_NOCONTENT
     */
#define AXIS2_HTTP_RESPONSE_NOCONTENT "202 OK";

    /**
     * RESPONSE_HTTP_UNAUTHORIZED
     */
#define AXIS2_HTTP_RESPONSE_HTTP_UNAUTHORIZED "401 Unauthorized"


   /**
    *	RESPONSE_HTTP_FORBIDDEN
	*/
#define AXIS2_HTTP_RESPONSE_HTTP_FORBIDDEN "403 Forbidden"

    /**
      * RESPONSE_PROXY_AUTHENTICATION_REQUIRED
      */
#define AXIS2_HTTP_RESPONSE_PROXY_AUTHENTICATION_REQUIRED "407 Proxy Authentication Required"

    /**
     * RESPONSE_BAD_REQUEST
     */
#define AXIS2_HTTP_RESPONSE_BAD_REQUEST "400 Bad Request"

    /**
     * RESPONSE_HTTP_INTERNAL_SERVER_ERROR
     */
#define AXIS2_HTTP_RESPONSE_INTERNAL_SERVER_ERROR "500 Internal Server Error"

    /**
     * HTTP_REQ_TYPE
     */
#define AXIS2_HTTP_REQ_TYPE "HTTP_REQ_TYPE"

    /**
     * SO_TIMEOUT
     */
#define AXIS2_HTTP_SO_TIMEOUT "SO_TIMEOUT"

    /**
     * CONNECTION_TIMEOUT
     */
#define AXIS2_HTTP_CONNECTION_TIMEOUT "CONNECTION_TIMEOUT"

    /**
     * DEFAULT_SO_TIMEOUT
     */
#define AXIS2_HTTP_DEFAULT_SO_TIMEOUT 60000

    /**
     * DEFAULT_CONNECTION_TIMEOUT
     */
#define AXIS2_HTTP_DEFAULT_CONNECTION_TIMEOUT 60000

#define AXIS2_HTTP_PROXY "PROXY"

    /**
     * ISO-8859-1 encoding
     */
#define AXIS2_HTTP_ISO_8859_1 "ISO-8859-1"

    /**
     * Default charset in content
     */
#define AXIS2_HTTP_DEFAULT_CONTENT_CHARSET "ISO-8859-1"

    /**
     * Field TRANSPORT_HTTP
     */
#define AXIS2_TRANSPORT_HTTP "http"

    /**
     * Msg context response written key
     */
#define AXIS2_RESPONSE_WRITTEN "CONTENT_WRITTEN"

    /**
     * Content type for MTOM
     */
#define MTOM_RECIVED_CONTENT_TYPE "MTOM_RECEIVED"

    /**
      * Constant for HTTP authentication
      */
#define AXIS2_HTTP_AUTHENTICATION "HTTP-Authentication"

    /**
      * Constant for HTTP authentication username
      */
#define AXIS2_HTTP_AUTHENTICATION_USERNAME "username"

    /**
      * Constant for HTTP authentication password
      */
#define AXIS2_HTTP_AUTHENTICATION_PASSWORD "password"

    /**
     * Constant for HTTP proxy
     */
#define AXIS2_HTTP_PROXY "PROXY"

    /**
     * Constant for HTTP proxy host
     */
#define AXIS2_HTTP_PROXY_HOST "proxy_host"

    /**
     * Constant for HTTP proxy port
     */
#define AXIS2_HTTP_PROXY_PORT "proxy_port"

    /**
      * Constant for HTTP proxy username
      */
#define AXIS2_HTTP_PROXY_USERNAME "proxy_username"

    /**
      * Constant for HTTP proxy password
      */
#define AXIS2_HTTP_PROXY_PASSWORD "proxy_password"


#define AXIS2_HTTP_PROXY_API "PROXY_API"

    /**
     * Constant for HTTP method
     */
#define AXIS2_HTTP_METHOD "HTTP_METHOD"

    /**
     * Constant for SSL Server Certificate
     */
#define AXIS2_SSL_SERVER_CERT "SERVER_CERT"

    /**
     * Constant for SSL Key File
     */
#define AXIS2_SSL_KEY_FILE "KEY_FILE"

    /**
     * Constant for SSL Passphrase
     */
#define AXIS2_SSL_PASSPHRASE "SSL_PASSPHRASE"

    /**
     * HTTP authentication username property name
     */
#define AXIS2_HTTP_AUTH_UNAME "HTTP_AUTH_USERNAME"

    /**
     * HTTP authentication password property name
     */
#define AXIS2_HTTP_AUTH_PASSWD "HTTP_AUTH_PASSWD"

    /**
      * Proxy authentication username property name
      */
#define AXIS2_PROXY_AUTH_UNAME "PROXY_AUTH_USERNAME"

    /**
      * Proxy authentication password property name
      */
#define AXIS2_PROXY_AUTH_PASSWD "PROXY_AUTH_PASSWD"


    /*#define AXIS2_HTTP_AUTH_TYPE "HTTP_AUTH_TYPE"*/

    /**
     * HTTP "Basic" authentication
     */
#define AXIS2_HTTP_AUTH_TYPE_BASIC "Basic"

    /**
     * HTTP "Digest" authentication
     */
#define AXIS2_HTTP_AUTH_TYPE_DIGEST "Digest"

    /**
      * Proxy "Basic" authentication
      */
#define AXIS2_PROXY_AUTH_TYPE_BASIC "Basic"

    /**
      * Proxy "Digest" authentication
      */
#define AXIS2_PROXY_AUTH_TYPE_DIGEST "Digest"


    /**
     *HTTP Transport Level Error
     */
#define AXIS2_HTTP_TRANSPORT_ERROR "http_transport_error"

    /**
     *415 Unsupported media Type
     */
#define AXIS2_HTTP_UNSUPPORTED_MEDIA_TYPE "415 Unsupported Media Type\r\n"

    /**
     *Constant for HTTP headers that user specify, Those headers will
     *provided as property to the message context.
     */
#define AXIS2_TRANSPORT_HEADER_PROPERTY "HTTP_HEADER_PROPERTY"


#define AXIS2_TRANSPORT_URL_HTTPS "HTTPS"

#define AXIS2_Q_MARK_STR "?"

#define AXIS2_Q_MARK '?'

#define AXIS2_H_MARK '#'

#define AXIS2_ALL "ALL"

#define AXIS2_USER_AGENT "Axis2C/" AXIS2_VERSION_STRING

#define AXIS2_AND_SIGN "&"

#define AXIS2_ESC_DOUBLE_QUOTE '\"'

#define AXIS2_ESC_DOUBLE_QUOTE_STR "\""

#define AXIS2_ESC_SINGLE_QUOTE '\''

#define AXIS2_DOUBLE_QUOTE '"'

#define AXIS2_ESC_NULL '\0'

#define AXIS2_SEMI_COLON_STR ";"

#define AXIS2_SEMI_COLON ';'

#define AXIS2_COLON ':'

#define AXIS2_COLON_STR ":"

#define AXIS2_CONTENT_TYPE_ACTION ";action=\""

#define AXIS2_CONTENT_TYPE_CHARSET ";charset="

#define AXIS2_CHARSET "charset"

#define AXIS2_PORT_STRING "port"

#define AXIS2_DEFAULT_HOST_ADDRESS "127.0.0.1"

#define AXIS2_DEFAULT_SVC_PATH "/axis2/services/"

#define AXIS2_HTTP_PROTOCOL "http"

#define AXIS2_HTTP "HTTP"

#define AXIS2_SPACE_COMMA " ,"

#define AXIS2_COMMA ','

#define AXIS2_Q 'q'

#define AXIS2_EQ_N_SEMICOLON " =;"

#define AXIS2_LEVEL "level"

#define AXIS2_SPACE_SEMICOLON " ;"

#define AXIS2_SPACE ' '

#define AXIS2_RETURN '\r'

#define AXIS2_NEW_LINE '\n'

#define AXIS2_F_SLASH '/'

#define AXIS2_B_SLASH '\\'

#define AXIS2_EQ '='

#define AXIS2_AND '&'

#define AXIS2_PERCENT '%'

#define AXIS2_HTTP_SERVER " (Simple Axis2 HTTP Server)"

#define AXIS2_COMMA_SPACE_STR ", "

#define AXIS2_SPACE_TAB_EQ " \t="

#define AXIS2_ACTION "action"

    /* Error Messages */

#define AXIS2_HTTP_NOT_FOUND "<html><head><title>404 Not Found</title></head>\
 <body><h2>Not Found</h2><p>The requested URL was not found on this server.\
</p></body></html>"  


#define AXIS2_HTTP_NOT_IMPLEMENTED "<html><head><title>501 Not Implemented\
</title></head><body><h2>Not Implemented</h2><p>The requested Method is not\
implemented on this server.</p></body></html>"


#define AXIS2_HTTP_INTERNAL_SERVER_ERROR "<html><head><title>500 Internal Server\
 Error</title></head><body><h2>Internal Server Error</h2><p>The server \
encountered an unexpected condition which prevented it from fulfilling the \
request.</p></body></html>"


#define AXIS2_HTTP_METHOD_NOT_ALLOWED "<html><head><title>405 Method Not Allowed\
</title></head><body><h2>Method Not Allowed</h2><p>The requested method is not\
allowed for this URL.</p></body></html>"

#define AXIS2_HTTP_NOT_ACCEPTABLE "<html><head><title>406 Not Acceptable\
</title></head><body><h2>Not Acceptable</h2><p>An appropriate representation of \
the requested resource could not be found on this server.</p></body></html>"

#define AXIS2_HTTP_BAD_REQUEST "<html><head><title>400 Bad Request</title>\
</head><body><h2>Bad Request</h2><p>Your client sent a request that this server\
 could not understand.</p></body></html>"

#define AXIS2_HTTP_REQUEST_TIMEOUT "<html><head><title>408 Request Timeout\
</title></head><body><h2>Request Timeout</h2><p>Cannot wait any longer for \
the HTTP request from the client.</p></body></html>" 

#define AXIS2_HTTP_CONFLICT "<html><head><title>409 Conflict</title></head>\
<body><h2>Conflict</h2><p>The client attempted to put the server\'s resources\
 into an invalid state.</p></body></html>"

#define AXIS2_HTTP_GONE "<html><head><title>410 Gone</title></head><body>\
<h2>Gone</h2><p>The requested resource is no longer available on this server.\
</p></body></html>"

#define AXIS2_HTTP_PRECONDITION_FAILED "<html><head><title>412 Precondition \
Failed</title></head><body><h2>Precondition Failed</h2><p>A precondition for\
 the requested URL failed.</p></body></html>"

#define AXIS2_HTTP_TOO_LARGE "<html><head><title>413 Request Entity Too Large\
</title></head><body><h2>Request Entity Too Large</h2><p>The data provided in\
 the request is too large or the requested resource does not allow request \
data.</p></body></html>"

#define AXIS2_HTTP_SERVICE_UNAVILABLE "<html><head><title>503 Service \
Unavailable</title></head><body><h2>Service Unavailable</h2><p>The service\
 is temporarily unable to serve your request.</p></body></html>"

    /** @} */

#ifdef __cplusplus
}
#endif
#endif                          /* AXIS2_HTTP_TRANSPORT_H */
