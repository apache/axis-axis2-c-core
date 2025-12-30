# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements. See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License. You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Financial Benchmark Service - Makefile.am
#
# Builds the HTTP/2 JSON Financial Benchmark Service for Axis2/C
# Target: Resource-constrained devices (old Android phones, embedded systems)

# Only build if JSON is enabled (required for json-c library)
if AXIS2_JSON_ENABLED

# Service library
servicesdir = $(prefix)/services/FinancialBenchmarkService

lib_LTLIBRARIES = libfinancial_benchmark_service.la

libfinancial_benchmark_service_la_SOURCES = \
    src/financial_benchmark_service.c

libfinancial_benchmark_service_la_CPPFLAGS = \
    -I$(top_srcdir)/include \
    -I$(top_srcdir)/util/include \
    -I$(top_srcdir)/axiom/include \
    -I$(top_srcdir)/neethi/include \
    $(JSON_CFLAGS)

libfinancial_benchmark_service_la_LIBADD = \
    $(top_builddir)/src/core/engine/libaxis2_engine.la \
    $(top_builddir)/axiom/src/om/libaxis2_axiom.la \
    $(top_builddir)/util/src/libaxutil.la \
    $(JSON_LIBS) \
    -lm

libfinancial_benchmark_service_la_LDFLAGS = \
    -module \
    -avoid-version \
    -no-undefined \
    -shared

# Install services.xml to the services directory
services_DATA = services.xml

# Install header for potential extensions
include_HEADERS = src/financial_benchmark_service.h

# Extra files to distribute
EXTRA_DIST = \
    services.xml \
    src/financial_benchmark_service.h \
    README.md \
    test/test_benchmark.sh \
    test/generate_test_data.py

endif # AXIS2_JSON_ENABLED
