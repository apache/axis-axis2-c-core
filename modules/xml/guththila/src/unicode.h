/*
 *   Copyright 2004,2005 The Apache Software Foundation.
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
 *
 *	
 * @author Dinesh Premalal (xydinesh@gmail.com, premalwd@cse.mrt.ac.lk)	
 */


#ifndef UNICODE_H
#define UNICODE_H

typedef unsigned char UTF8_char;
typedef unsigned short UTF16_char;

/*! These are the values of UTF-16 Document
 * After , BOM (Byter Oreder Mark) was determined unicodeState
 * variable will set to one of the following values
 * None = Normally treated as Utf-8 Document
 * LE = Little Endian UTF-16 Document
 * BE = Big Endian UTF-16 Document
 * For the time being Endianess make no effect to the parsing  */
enum UTF16_endianess {None=1, LE, BE};

#ifdef __UNICODE__FUNCTIONS__
#define strlen(x) strlenUnicode(x) 
#define strndup(x, y) strdupUnicode(x, y)

UTF8_char strlenUnicode (char *);

char * strdupUnicode (char *, int);

#endif /* __UNICODE__FUNCTIONS__ */


#endif /* UNICODE_H */
