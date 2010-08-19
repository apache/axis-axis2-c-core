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

noinst_LTLIBRARIES = librp_builder.la

librp_builder_la_SOURCES = algorithmsuite_builder.c \
						   layout_builder.c \
						   supporting_tokens_builder.c \
						   transport_binding_builder.c username_token_builder.c \
						   wss10_builder.c transport_token_builder.c \
						   token_identifier.c https_token_builder.c rampart_config_builder.c \
						   asymmetric_binding_builder.c x509_token_builder.c initiator_token_builder.c \
						   recipient_token_builder.c signed_encrypted_parts_builder.c secpolicy_builder.c \
						   symmetric_binding_builder.c protection_token_builder.c signature_token_builder.c \
						   encryption_token_builder.c wss11_builder.c trust10_builder.c \
						   bootstrap_policy_builder.c security_context_token_builder.c \
						   issued_token_builder.c saml_token_builder.c

librp_builder_la_LIBADD  = ../../../../axiom/src/om/libaxis2_axiom.la \
				../../../../util/src/libaxutil.la

INCLUDES = -I$(top_builddir)/include \
			-I ../../../include \
			-I ../../../../util/include \
            -I ../../../../axiom/include \
            -I ../../../../include
