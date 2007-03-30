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

#ifndef WODEN_XML_CHAR_H
#define WODEN_XML_CHAR_H

/**
 * @file woden_xml_char.h
 * @brief Axis2 Xml Char Interface
 * This class has been turned into C from Axis into Woden to support the NCName class.
 * 
 * This class defines the basic XML character properties. The data
 * in this class can be used to verify that a character is a valid
 * XML character or if the character is a space, name start, or name
 * character.
 * <p>
 * A series of convenience methods are supplied to ease the burden
 * of the developer. Because inlining the checks can improve per
 * character performance, the tables of character properties are
 * public. Using the character as an index into the <code>CHARS</code>
 * array and applying the appropriate mask flag (e.g.
 * <code>MASK_VALID</code>), yields the same results as calling the
 * convenience methods. There is one exception: check the comments
 * for the <code>isValid</code> method for details.
 *
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axutil_error.h>
#include <axutil_string.h>
#include <axutil_utils.h>
#include <axutil_hash.h>
#include <axutil_uri.h>
#include <woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup woden_xml_char Xml Char
  * @ingroup woden
  * @{
  */

/**
 * Returns AXIS2_TRUE if the specified character is a supplemental character.
 *
 * @param c The character to check.
 */
axis2_bool_t AXIS2_CALL 
woden_xml_char_is_supplemental(
        int c); 

/**
 * Returns AXIS2_TRUE the supplemental character corresponding to the given
 * surrogates.
 *
 * @param h The high surrogate.
 * @param l The low surrogate.
 */
int AXIS2_CALL 
woden_xml_char_supplemental(
        char h, 
        char l); 

/**
 * Returns the high surrogate of a supplemental character
 *
 * @param c The supplemental character to "split".
 */
axis2_char_t AXIS2_CALL 
woden_xml_char_high_surrogate(
        int c); 

/**
 * Returns the low surrogate of a supplemental character
 *
 * @param c The supplemental character to "split".
 */
axis2_char_t AXIS2_CALL 
woden_xml_char_low_surrogate(
        int c); 

/**
 * Returns whether the given character is a high surrogate
 *
 * @param c The character to check.
 */
axis2_bool_t AXIS2_CALL 
woden_xml_char_is_high_surrogate(
        int c); 

/**
 * Returns whether the given character is a low surrogate
 *
 * @param c The character to check.
 */
axis2_bool_t AXIS2_CALL 
woden_xml_char_is_low_surrogate(
        int c); 


/**
 * Returns AXIS2_TRUE if the specified character is valid. This method
 * also checks the surrogate character range from 0x10000 to 0x10FFFF.
 * <p>
 * If the program chooses to apply the mask directly to the
 * <code>xml_char_impl->CHARS</code> array, then they are responsible for checking
 * the surrogate character range.
 *
 * @param c The character to check.
 */
axis2_bool_t AXIS2_CALL 
woden_xml_char_is_valid(
        int c); 

/**
 * Returns AXIS2_TRUE if the specified character is invalid.
 *
 * @param c The character to check.
 */
axis2_bool_t AXIS2_CALL 
woden_xml_char_is_invalid(
        int c); 

/**
 * Returns AXIS2_TRUE if the specified character can be considered content.
 *
 * @param c The character to check.
 */
axis2_bool_t AXIS2_CALL 
woden_xml_char_is_content(
        int c); 

/**
 * Returns AXIS2_TRUE if the specified character can be considered markup.
 * Markup characters include '&lt;', '&amp;', and '%'.
 *
 * @param c The character to check.
 */
axis2_bool_t AXIS2_CALL 
woden_xml_char_is_markup(
        int c); 

/**
 * Returns AXIS2_TRUE if the specified character is a space character
 * as defined by production [3] in the XML 1.0 specification.
 *
 * @param c The character to check.
 */
axis2_bool_t AXIS2_CALL 
woden_xml_char_is_space(
        int c); 

/**
 * Returns AXIS2_TRUE if the specified character is a space character
 * as amdended in the XML 1.1 specification.
 *
 * @param c The character to check.
 */
axis2_bool_t AXIS2_CALL 
woden_xml_char_is_xml11_space(
        int c); 

/**
 * Returns AXIS2_TRUE if the specified character is a valid name start
 * character as defined by production [5] in the XML 1.0
 * specification.
 *
 * @param c The character to check.
 */
axis2_bool_t AXIS2_CALL 
woden_xml_char_is_name_start(
        int c); 

/**
 * Returns AXIS2_TRUE if the specified character is a valid name
 * character as defined by production [4] in the XML 1.0
 * specification.
 *
 * @param c The character to check.
 */
axis2_bool_t AXIS2_CALL 
woden_xml_char_is_name(
        int c); 

/**
 * Returns AXIS2_TRUE if the specified character is a valid nc_name start
 * character as defined by production [4] in Namespaces in XML
 * recommendation.
 *
 * @param c The character to check.
 */
axis2_bool_t AXIS2_CALL 
woden_xml_char_is_nc_name_start(
        int c); 

/**
 * Returns AXIS2_TRUE if the specified character is a valid NCName
 * character as defined by production [5] in Namespaces in XML
 * recommendation.
 *
 * @param c The character to check.
 */
axis2_bool_t AXIS2_CALL
woden_xml_char_is_nc_name(
        int c); 

/**
 * Returns AXIS2_TRUE if the specified character is a valid Pubid
 * character as defined by production [13] in the XML 1.0
 * specification.
 *
 * @param c The character to check.
 */
axis2_bool_t AXIS2_CALL 
woden_xml_char_is_pubid(
        int c);

/*
 * [5] Name ::= (Letter | '_' | ':'(NameChar)*
 */
/**
 * Check to see if a string is a valid Name according to [5]
 * in the XML 1.0 Recommendation
 *
 * @param name string to check
 * @return AXIS2_TRUE if name is a valid Name
 */
axis2_bool_t AXIS2_CALL 
woden_xml_char_is_valid_name(
        axis2_char_t *name);


/*
 * from the namespace rec
 * [4] NCName ::= (Letter | '_'(NCNameChar)*
 */
/**
 * Check to see if a string is a valid NCName according to [4]
 * from the XML Namespaces 1.0 Recommendation
 *
 * @param nc_name string to check
 * @return AXIS2_TRUE if name is a valid NCName
 */
axis2_bool_t AXIS2_CALL 
woden_xml_char_is_valid_nc_name(
        axis2_char_t * nc_name);

/*
 * [7] Nmtoken ::= (NameChar)+
 */
/**
 * Check to see if a string is a valid Nmtoken according to [7]
 * in the XML 1.0 Recommendation
 *
 * @param nmtoken string to check
 * @return AXIS2_TRUE if nmtoken is a valid nmtoken
 */
axis2_bool_t AXIS2_CALL 
woden_xml_char_is_valid_nmtoken(
        axis2_char_t * nmtoken);

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_XML_CHAR_H */

