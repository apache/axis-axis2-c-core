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
noinst_LTLIBRARIES = libaxis2_soap.la
libaxis2_soap_la_SOURCES = soap_fault.c \
                           soap_fault_code.c \
                           soap_fault_detail.c \
                           soap_fault_node.c \
                           soap_fault_reason.c \
                           soap_fault_role.c \
                           soap_fault_sub_code.c \
                           soap_fault_text.c \
                           soap_fault_value.c \
                           soap_header_block.c \
                           soap_header.c \
                           soap_body.c \
                           soap_envelope.c \
                           soap_builder.c \
                           soap11_builder_helper.c \
                           soap12_builder_helper.c

libaxis2_soap_la_LIBADD = 
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/src/parser \
            -I$(top_builddir)/src/om \
			-I ../../../util/include

EXTRA_DIST =     axiom_soap11_builder_helper.h axiom_soap_builder_internal.h \
		_axiom_soap_fault_code.h _axiom_soap_fault_reason.h \
		_axiom_soap_fault_value.h axiom_soap12_builder_helper.h \
		_axiom_soap_fault_detail.h _axiom_soap_fault_role.h \
		_axiom_soap_header_block.h _axiom_soap_body.h \
		_axiom_soap_fault.h _axiom_soap_fault_sub_code.h \
		_axiom_soap_header.h _axiom_soap_envelope.h \
		_axiom_soap_fault_node.h _axiom_soap_fault_text.h

