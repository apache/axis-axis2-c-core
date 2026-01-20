
/*
 *   Copyright 2003-2004 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <axutil_utils_defines.h>

/*
 * @file axutil_base64.h
 * @brief AXIS2-UTIL Base64 Encoding
 */
#ifndef AXUTIL_BASE64_H
#define AXUTIL_BASE64_H

#ifdef __cplusplus
extern "C"
{
#endif

    /*
     * @defgroup AXIS2_Util_Base64 base64 encoding
     * @ingroup AXIS2_Util
     */

    /* Simple BASE64 encode/decode functions.
     *
     * As we might encode binary strings, hence we require the length of
     * the incoming plain source. And return the length of what we decoded.
     *
     * The decoding function takes any non valid char (i.e. whitespace, \0
     * or anything non A-Z,0-9 etc as terminal.
     *
     * plain strings/binary sequences are not assumed '\0' terminated. Encoded
     * strings are neither. But probably should.
     *
     */

    /*
     * Given the length of an un-encoded string, get the buffer size needed
     * to hold the base64-encoded result.
     * @param len the length of the plain text string to be encoded.
     * @return the buffer size needed, including space for null terminator.
     *         This is one more than strlen() would return on the encoded string.
     *         Use this value for memory allocation: malloc(axutil_base64_encode_len(len))
     */
    AXIS2_EXTERN int AXIS2_CALL
    axutil_base64_encode_len(
        int len);

    /*
     * Encode a text string using base64 encoding.
     * @param coded_dst The destination buffer for the encoded string.
     *                  Must be at least axutil_base64_encode_len(len_plain_src) bytes.
     * @param plain_src The original string in plain text
     * @param len_plain_src The length of the plain text string
     * @return the number of bytes written including null terminator.
     *         This is one more than strlen(coded_dst) would return.
     */
    AXIS2_EXTERN int AXIS2_CALL
    axutil_base64_encode(
        char *coded_dst,
        const char *plain_src,
        int len_plain_src);

    /*
     * Encode binary data using base64 encoding.
     * @param coded_dst The destination buffer for the encoded string.
     *                  Must be at least axutil_base64_encode_len(len_plain_src) bytes.
     * @param plain_src The binary data to encode
     * @param len_plain_src The length of the binary data
     * @return the number of bytes written including null terminator.
     *         This is one more than strlen(coded_dst) would return.
     */
    AXIS2_EXTERN int AXIS2_CALL
    axutil_base64_encode_binary(
        char *coded_dst,
        const unsigned char *plain_src,
        int len_plain_src);

    /*
     * Determine the length of a plain text string given the encoded version
     * @param coded_src The encoded string
     * @return the length of the plain text string
     */
    AXIS2_EXTERN int AXIS2_CALL
    axutil_base64_decode_len(
        const char *coded_src);

    /*
     * Decode a string to plain text
     * @param plain_dst The destination string for the plain text. size of this should be axutil_base64_decode_len + 1
     * @param coded_src The encoded string
     * @return the length of the plain text string
     */
    AXIS2_EXTERN int AXIS2_CALL
    axutil_base64_decode(
        char *plain_dst,
        const char *coded_src);

    /*
     * Decode an EBCDIC string to plain text
     * @param plain_dst The destination string for the plain text. size of this should be axutil_base64_decode_len
     * @param coded_src The encoded string
     * @return the length of the plain text string
     */
    AXIS2_EXTERN int AXIS2_CALL
    axutil_base64_decode_binary(
        unsigned char *plain_dst,
        const char *coded_src);

    /* @} */
#ifdef __cplusplus
}
#endif

#endif                          /* !AXIS2_BASE64_H */
