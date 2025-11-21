# Makefile.am for native WSDL2C generator
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

# Build the native WSDL2C generator
bin_PROGRAMS = wsdl2c-native

# Source files
wsdl2c_native_SOURCES = \
    src/main.c \
    src/options.c \
    src/wsdl_parser.c \
    src/stub_generator.c

# Include directories
AM_CPPFLAGS = \
    -I$(top_srcdir)/include \
    -I$(top_srcdir)/util/include \
    -I$(top_srcdir)/axiom/include \
    -I$(srcdir)/include \
    $(LIBXML2_CFLAGS) \
    -DHAVE_CONFIG_H

# Compiler flags
AM_CFLAGS = -Wall -Wextra -pedantic

# For standalone testing without full Axis2/C build
AM_CPPFLAGS += \
    -DAXIS2_SUCCESS=0 \
    -DAXIS2_FAILURE=-1 \
    -DAXIS2_TRUE=1 \
    -DAXIS2_FALSE=0 \
    -DAXIS2_LOG_LEVEL_INFO=2 \
    -DSTANDALONE_BUILD=1

# Link libraries
wsdl2c_native_LDADD = \
    $(LIBXML2_LIBS)
    # TODO: Add when integrated with full build:
    # $(top_builddir)/util/src/libaxutil.la \
    # $(top_builddir)/axiom/src/om/libaxis2_axiom.la

# Headers to install
include_HEADERS = include/wsdl2c_native.h

# Install directory for templates (future use)
# templatesdir = $(datadir)/axis2c/codegen/templates
# dist_templates_DATA = templates/*

# Subdirectories to recurse into
SUBDIRS = test

# Extra files to distribute
EXTRA_DIST = \
    SESSION_STATE.md \
    README.md \
    build/