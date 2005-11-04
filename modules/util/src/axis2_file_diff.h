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
 
#ifndef AXIS2_FILE_DIFF_H
#define AXIS2_FILE_DIFF_H

/**
  * @file axis2_file_diff.h
  * @brief to check whether two files are same.
  *        Spaces and new lines are ignored
  */


#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_stream.h>
#include <axis2_error.h>
   
/**************************** Function headers ********************************/

/** Test for two files to see whether they are equal. White spaces and new lines
  * are ignored.
  * @param file name
  * @param file name
  * @return status code
  */
axis2_status_t axis2_file_diff(axis2_env_t *env
		, axis2_char_t *expected_file_name, axis2_char_t *actual_file_name);

#endif /* AXIS2_FILE_DIFF_H */
