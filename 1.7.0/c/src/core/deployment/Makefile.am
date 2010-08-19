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
noinst_LTLIBRARIES = libaxis2_deployment.la

noinst_HEADERS = axis2_deployment.h \
                    axis2_arch_file_data.h \
                    axis2_arch_reader.h \
                    axis2_conf_builder.h \
                    axis2_dep_engine.h \
                    axis2_desc_builder.h \
                    axis2_module_builder.h \
                    axis2_repos_listener.h \
                    axis2_svc_builder.h \
                    axis2_svc_grp_builder.h \
                    axis2_ws_info.h \
                    axis2_ws_info_list.h

libaxis2_deployment_la_SOURCES =phases_info.c \
                                desc_builder.c \
                                arch_reader.c \
                                dep_engine.c \
                                arch_file_data.c \
                                ws_info.c \
                                ws_info_list.c \
                                svc_builder.c \
                                svc_grp_builder.c \
                                module_builder.c \
                                conf_builder.c \
                                repos_listener.c \
                                conf_init.c

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/src/core/description \
            -I$(top_builddir)/src/core/engine \
            -I$(top_builddir)/src/core/phaseresolver \
            -I$(top_builddir)/src/core/deployment \
            -I$(top_builddir)/src/core/util \
            -I$(top_builddir)/axiom/include \
            -I$(top_builddir)/neethi/include \
            -I$(top_builddir)/util/include
