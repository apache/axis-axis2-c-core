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
prgbindir=$(prefix)/bin/samples
prgbin_PROGRAMS = Calculator
samplesdir=$(prefix)/samples/client/Calculator

Calculator_SOURCES = \
axis2_add.c            axis2_Calculator_stub.c  axis2_divResponse.c    axis2_mulResponse.c    axis2_subResponse.c \
axis2_addRequest.c     axis2_div.c              axis2_mul.c            axis2_sub.c            test_Calcultor.c \
axis2_addResponse20.c  axis2_divRequest.c       axis2_mulRequest.c     axis2_subRequest.c \
axis2_addResponse.c    axis2_divResponse16.c    axis2_mulResponse14.c  axis2_subResponse12.c


Calculator_LDADD   = $(LDFLAGS) \
                    -L$(AXIS2C_HOME)/lib \
                    -laxutil \
                    -laxis2_axiom \
                    -laxis2_wsdl \
                    -laxis2_engine \
                    -laxis2_parser \
                    -laxis2_http_sender \
                    -laxis2_http_receiver \
                    $(GUTHTHILA_LIBS)

INCLUDES = -I$(AXIS2C_HOME)/include \
			@UTILINC@ \
			@AXIOMINC@
