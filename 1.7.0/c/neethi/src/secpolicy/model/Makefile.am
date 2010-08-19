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
TESTS =

noinst_LTLIBRARIES = librp_model.la

librp_model_la_SOURCES = algorithmsuite.c asymmetric_binding.c \
							binding_commons.c header.c element.c https_token.c layout.c \
							property.c rampart_config.c secpolicy.c security_context_token.c \
							signed_encrypted_elements.c signed_encrypted_parts.c signed_encrypted_items.c \
							supporting_tokens.c symmetric_asymmetric_binding_commons.c \
							symmetric_binding.c transport_binding.c ut.c wss10.c wss11.c x509_token.c \
							trust10.c issued_token.c saml_token.c token.c


librp_model_la_LIBADD  = ../../../../util/src/libaxutil.la


INCLUDES = -I$(top_builddir)/include \
			-I ../../../include \
			-I ../../../../util/include \
	        -I ../../../../axiom/include \
    	    -I ../../../../include
