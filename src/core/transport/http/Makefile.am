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
# HTTP Transport Build Configuration
#
# HTTP/2 JSON mode (WITH_NGHTTP2): Only builds util and common
# Legacy HTTP/1.1 SOAP mode: Builds full stack including sender and receiver

if WITH_NGHTTP2
# HTTP/2 JSON: Minimal HTTP transport - server built separately after engine
SUBDIRS = util common
else
# Legacy HTTP/1.1 SOAP: Full HTTP transport stack for traditional SOAP services
SUBDIRS = util common sender receiver
endif

# Note: Apache module (server/) is built after engine in main SUBDIRS to avoid circular deps
