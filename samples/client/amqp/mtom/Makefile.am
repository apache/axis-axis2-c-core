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
SUBDIRS = resources

prgbindir = $(prefix)/samples/bin/amqp

prgbin_PROGRAMS = mtom

mtom_SOURCES = mtom_client.c

mtom_LDADD = $(LDFLAGS) \
             -L$(AXIS2C_HOME)/lib \
		     -laxutil \
             -laxis2_axiom \
             -laxis2_engine \
             -laxis2_parser \
             $(GUTHTHILA_LIBS)

mtom_CPPFLAGS = @AXIS2INC@
