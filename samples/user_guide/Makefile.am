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

# Include only traditional clients (HTTP/2 services require --enable-http2)
SUBDIRS = clients

# Conditionally include HTTP/2 JSON services when both HTTP/2 and JSON are enabled
if AXIS2_JSON_ENABLED
if WITH_NGHTTP2
SUBDIRS += \
    bigdata-h2-service \
    login-service \
    testws-service \
    camera-control-service \
    financial-benchmark-service
endif
endif

# HTTP/2 Pure JSON Services Documentation
EXTRA_DIST = \
    bigdata-h2-service/README.md \
    bigdata-h2-service/services.xml \
    bigdata-h2-service/src/bigdata_h2_service.c \
    bigdata-h2-service/src/bigdata_h2_service.h \
    login-service/services.xml \
    testws-service/services.xml \
    camera-control-service/services.xml \
    financial-benchmark-service/README.md \
    financial-benchmark-service/services.xml \
    financial-benchmark-service/src/financial_benchmark_service.c \
    financial-benchmark-service/src/financial_benchmark_service.h \
    financial-benchmark-service/test/test_benchmark.sh \
    financial-benchmark-service/test/generate_test_data.py

# Clean up service builds
clean-local:
	cd clients && $(MAKE) clean || true
	if test -d bigdata-h2-service; then cd bigdata-h2-service && $(MAKE) clean || true; fi
	if test -d login-service; then cd login-service && $(MAKE) clean || true; fi
	if test -d testws-service; then cd testws-service && $(MAKE) clean || true; fi
	if test -d camera-control-service; then cd camera-control-service && $(MAKE) clean || true; fi
	if test -d financial-benchmark-service; then cd financial-benchmark-service && $(MAKE) clean || true; fi
