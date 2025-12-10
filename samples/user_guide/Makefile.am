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

# HTTP/2 Pure JSON Services Documentation (not installed when HTTP/2 disabled)
# EXTRA_DIST = \
#     bigdata-h2-service/README.md \
#     login-service/README.md \
#     testws-service/README.md \
#     bigdata-h2-service/services.xml \
#     login-service/services.xml \
#     testws-service/services.xml

# Note: HTTP/2 service documentation not installed in HTTP/1.1-only builds

# Clean up service builds
clean-local:
	cd clients && $(MAKE) clean || true
