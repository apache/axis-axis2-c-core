# Makefile.am for native WSDL2C generator tests
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Test programs (only built when tests are enabled)
if AXIS2_NATIVE_CODEGEN_ENABLED
noinst_PROGRAMS = adb_test_runner

# ADB test framework source files
adb_test_runner_SOURCES = \
    src/adb_test_framework.c \
    src/test_basic_types.c \
    src/test_complex_types.c \
    src/test_enumerations.c \
    src/test_choices.c \
    src/test_arrays.c \
    src/test_axis2c_1373_fix.c \
    src/test_axis2c_1515_fix.c \
    src/test_axis2c_1699_fix.c \
    src/test_axis2c_1685_fix.c \
    src/test_axis2c_1679_mtom_feature.c \
    src/test_axis2c_1658_fault_validation.c \
    src/test_axis2c_1617_memory_regression.c \
    src/test_axis2c_1616_type_name_conflict.c \
    src/test_axis2c_1614_required_attribute_validation.c \
    src/test_axis2c_1581_empty_soap_action.c \
    src/test_axis2c_1580_any_type.c \
    src/test_axis2c_1579_xsi_type.c \
    src/test_axis2c_1575_qname_property.c \
    src/test_axis2c_1573_identifier_sanitization.c \
    src/test_axis2c_1529_base64_memleak.c \
    src/test_axis2c_1613_error_on_null.c \
    src/test_axis2c_1182_array_add_function.c \
    src/test_axis2c_1506_ehealth_schema.c \
    src/axis2_stub_compat.c

# Include directories
AM_CPPFLAGS = \
    -I$(srcdir)/include \
    -I$(top_srcdir)/tools/codegen/native/include \
    $(LIBXML2_CFLAGS) \
    -D_GNU_SOURCE \
    -D_POSIX_C_SOURCE=200809L

# Compiler flags
AM_CFLAGS = -Wall -Wextra -std=c99

# Link libraries
adb_test_runner_LDADD = \
    $(LIBXML2_LIBS)

# Test scripts
dist_noinst_SCRIPTS = \
    integration/run_integration_tests.sh \
    integration/test_basic_codegen.sh \
    integration/test_autotools_build.sh

# Make test scripts executable
install-exec-hook:
	chmod +x $(srcdir)/integration/*.sh

# Custom test target
check-local: adb_test_runner
	@echo "Running ADB test suite..."
	./adb_test_runner
	@echo "All tests passed!"

endif

# Extra files to distribute
EXTRA_DIST = \
    README.md \
    ADB_FEATURE_PARITY.md \
    adb_test_plan.md \
    wsdl/ \
    integration/ \
    build/