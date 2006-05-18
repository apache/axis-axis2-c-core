#ifndef AXIS2_DEFINES_H
#define AXIS2_DEFINES_H

/**
 * @file axis2_defines.h
 * @brief Useful definitions, which may have platform concerns
 */

#include <stddef.h>
#include <axis2_utils_defines.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** 
  * This enum is used to decide the type of storage used 
  */
typedef enum _axis2_xml_parser_type
{
    AXIS2_XML_PARSER_TYPE_BUFFER = 1,
	AXIS2_XML_PARSER_TYPE_FILE,
    AXIS2_XML_PARSER_TYPE_DOC
} axis2_xml_parser_type;

#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_DEFINES_H */
