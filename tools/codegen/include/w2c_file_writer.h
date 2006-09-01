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
  
#ifndef W2C_FILE_WRITER_H
#define W2C_FILE_WRITER_H

/**
 * @file w2c_file_writer.h
 * @brief handles Axis2/C w2c  file writing
 */

#include <stdio.h>
#include <axis2_utils.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @defgroup w2c_file_writer handles file writing
 * @ingroup w2c_utils
 * @{
 */    

/**
 * creates folder in the path/ returns the fullpath to file.
 * @param env double pointer to environment struct. MUST NOT be NULL
 * @param root_location location to be written
 * @param package_name  package, can be '.' separated 
 *                      to compatibility with java
 * @param file_name     name of the file
 * @param extension     type of the file c, java, csharp, cpp etc
 * @return the File that was created
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
w2c_file_writer_create_classfile_name ( 
    const axis2_env_t *env,
    axis2_char_t *rootLocation,
    axis2_char_t *package_name,
    axis2_char_t *file_name,
    axis2_char_t *extension );


/** @} */

#ifdef __cplusplus
}
#endif

#endif /* W2C_FILE_WRITER_H */
