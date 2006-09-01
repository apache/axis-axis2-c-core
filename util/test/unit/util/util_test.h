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

#ifndef UTIL_TEST_H
#define UTIL_TEST_H

#include <CuTest.h>
#include "util_stream_test.h"
#include "util_log_test.h"
#include "util_hash_test.h"
#include "util_array_list_test.h"
#include "util_linked_list_test.h"
#include "util_error_test.h"
#include "util_dir_handler_test.h"
#include "util_class_loader_test.h"
#include "util_url_test.h"
#include "util_string_test.h"
#include "util_uri_test.h"
#include "util_properties_test.h"
#include "util_date_time_test.h"

CuSuite* axis2_utilGetSuite();

#endif /* UTIL_TEST_H */
