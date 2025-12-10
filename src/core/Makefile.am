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
# REVOLUTIONARY HTTP/2 JSON: Functional baseline with revolutionary direct response
# HTTP/2 JSON direct response implemented in mod_axis2.c bypasses SOAP processing

# Option B: Essential components for functional HTTP/2 JSON system
# CRITICAL: Build order dependencies for revolutionary HTTP/2 JSON architecture
# - receivers must come before engine (engine links to libaxis2_receivers.la)
# - deployment must come before engine (engine links to libaxis2_deployment.la)
# - clientapi must come before engine (engine links to libaxis2_clientapi.la)
# - phaseresolver must come before engine (engine links to libaxis2_phaseresolver.la)
# - engine must come before transport (HTTP common now links to libaxis2_engine.la for service provider)
# - transport/http/server must come after transport (depends on HTTP common)
SUBDIRS = util addr context description deployment receivers clientapi phaseresolver engine transport transport/http/server

# REVOLUTIONARY STRATEGY: Build functional Axis2/C baseline with revolutionary HTTP/2 JSON code
# - Revolutionary code provides direct JSON response BEFORE any SOAP processing
# - Full Axis2/C available but bypassed for HTTP/2 JSON requests
# - Eliminates dependency cascade issues while preserving revolutionary functionality
#
# ELIMINATED: modules (mod_addr, mod_log - SOAP-only processing modules)
